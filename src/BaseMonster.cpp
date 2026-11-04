#include "stdafx.h"

// BaseMonster array at 0x009332F0
BaseMonster BaseMonsters[BM_COUNT] = {
// id,	valid,	SpriteHeight,	AnimationSize,	SeedingSize,	AnimationFileName,	HasSecondAttack,	SoundFileName,	HasSpecialSounds,	HasColorTranslationFile,	ColorTranslationFilePtr,	animationFrameCount,	animationDelay,	name,	MinDungeonLevel,	MaxDungeonLevel,	MonsterBaseLevel,	MinHitPoints,	MaxHitPoints,	ai,	flag,	intelligenceFactor,	FirstAtackToHit,	FirstAtackToHitFrame,	FirstAtackMinDamage,	FirstAtackMaxDamage,	SecondAttackToHit,	SecondAtackToHitFrame,	SecondAtackMinDamage,	SecondAtackMaxDamage,	ArmorClass,	MonsterClass,	ResistanceOn1And2Difficulty,	ResistanceOn3Difficulty,	ItemDropSpecials,	SelectionOutline,	ExperiencePoints, IceAgeColorTranslationFile, IceAgeNamePtr
{
	BM_0_BURNED_CORPSE,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Corpse\\corp%c.cl2",	1,	"X\\Corp%c%i.WAV",		1,
	1,	"Monsters\\Zombie\\Bluered.TRN",	// TRNs:   flag and path
	11, 24, 12, 6, 16, 12,		// IWAHDS frames
	4, 0, 0, 0, 0, 3,		// delays
	"Burned Corpse",			// name
	0,	2,	2,		// DLVL range & MLVL
	24,	48, // HP range,
	AI_0_Zombie,	MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	8,	8,	3,	9,// tohit, hit frame of 1st attack, dmg range.
	1,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	2, 0, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	80,		// special flag for drop, animation size, XP
	"X\\Zombie05_blue_.trn", "Rotting Carcass" // ice age  TRN & name
},
{
	BM_1_OBLIVION_KNIGHT,	0, 100,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SKing\\SKing%c.CL2",	1,	"X\\69%c%i.WAV",		1,
	1,	"Monsters\\SkelAxe\\White.TRN",	// TRNs:   flag and path
	8, 8, 16, 6, 16, 12,		// IWAHDS frames
	2, 0, 0, 0, 2, 2,		// delays
	"Oblivion Knight",			// name
	6,	6,	13,		// DLVL range & MLVL
	750,	800, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	19,	8,	35,	53,// tohit, hit frame of 1st attack, dmg range.
	0,	6,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	26, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	500,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_2_ROTTING_CARCASS,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Zombie\\Zombie%c.CL2",	1,	"Monsters\\Zombie\\Zombie%c%i.WAV",		1,
	1,	"X\\ViciousB.TRN",	// TRNs:   flag and path
	11, 24, 12, 6, 16, 11,		// IWAHDS frames
	4, 0, 0, 0, 0, 4,		// delays
	"Rotting Carcass",			// name
	2,	8,	10,		// DLVL range & MLVL
	60,	120, // HP range,
	AI_0_Zombie,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	6,	8,	7,	20,// tohit, hit frame of 1st attack, dmg range.
	0,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	9, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	165,		// special flag for drop, animation size, XP
	/*"X\\Zombie06_white_v3.trn"*/"Monsters\\Zombie\\Bluered.TRN", "Burned Corpse" // ice age  TRN & name
},
{
	BM_3_FAMILIAR,	1, 0,	96,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Bat\\Bat%c.CL2",	0,	"Monsters\\Bat\\bat%c%i.wav",		0,
	1,	"Monsters\\Bat\\orange.trn",	// TRNs:   flag and path
	9, 13, 10, 9, 13, 0,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Familiar",			// name
	12,	16,	23,		// DLVL range & MLVL
	100,	160, // HP range,
	AI_32_Ranger_Firebolt,	MF_9_CHASE_TARGET, 1,	// AI, int flag, int level
	17,	5,	12,	32,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	26, 1, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	16384,		6,	350,		// special flag for drop, animation size, XP
	"X\\Bat_03_bright_blue.trn", "Ice Imp" // ice age  TRN & name
},
{
	BM_4_GREMLIN,	1, 60,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FalSpear\\Phall%c.CL2",	1,	"Monsters\\BigFall\\bfal%c%i.WAV",		1,
	1,	"X\\Gremlin.TRN",	// TRNs:   flag and path
	11, 11, 12, 11, 18, 13,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Green One",			// name
	8,	12,	18,		// DLVL range & MLVL
	165,	240, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	18,	7,	14,	26,// tohit, hit frame of 1st attack, dmg range.
	0,	5,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	21, 1, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN|MR_8_IM_ACID,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_8_IM_ACID,// armor, mon type, and resistances
	0,		3,	300,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_5_FALLEN_ONE,	1, 60,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FalSpear\\Phall%c.CL2",	1,	"Monsters\\BigFall\\bfal%c%i.WAV",		1,
	1,	"X\\Fallen01.TRN",	// TRNs:   flag and path
	11, 11, 13, 11, 18, 13,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Fallen One",			// name
	0,	6,	5,		// DLVL range & MLVL
	20,	35, // HP range,
	AI_8_Fallen_One,	0, 0,	// AI, int flag, int level
	6,	8,	2,	11,// tohit, hit frame of 1st attack, dmg range.
	0,	5,	12,	14,	 // 2nd attack: tohit, hit frame, dmg range
	2, 1, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	110,		// special flag for drop, animation size, XP
	"X\\Fallen06_shivering1.trn", "Shivering One" // ice age  TRN & name
},
{
	BM_6_GRAVE_DIGGER,	1, 0,	124,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Gravdg\\Gravdg%c.CL2",	1,	"Monsters\\newsfx\\GDiggr%c%i.WAV",		1,
	1,	"Monsters\\GoatBow\\Gray.TRN",	// TRNs:   flag and path
	24, 24, 12, 6, 16, 16,		// IWAHDS frames
	2, 0, 0, 2, 2, 0,		// delays
	"Grave Digger",			// name
	0,	2,	2,		// DLVL range & MLVL
	35,	55, // HP range,
	AI_4_Scavenger,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	7,	6,	4,	12,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	3, 0, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	90,		// special flag for drop, animation size, XP
	"X\\Gravdg03bluish.trn", "Ice Digger" // ice age  TRN & name
},
{
	BM_7_RISEN_SKELETON,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelAxe\\SklAx%c.CL2",	1,	"Monsters\\SkelAxe\\SklAx%c%i.WAV",		0,
	1,	"X\\M1.trn",	// TRNs:   flag and path
	12, 8, 13, 6, 17, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Risen Skeleton",			// name
	0,	2,	4,		// DLVL range & MLVL
	18,	30, // HP range,
	AI_2_Skel_Melee,	MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	6,	8,	3,	7,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	4, 0, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	95,		// special flag for drop, animation size, XP
	"X\\SKing006-blue.trn", "Frozen Skeleton" // ice age  TRN & name
},
{
	BM_8_BONE_CREEPER,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelSd\\SklSr%c.CL2",	1,	"Monsters\\SkelSd\\SklSr%c%i.WAV",		1,
	1,	"X\\Skeleton005-crimson.trn",	// TRNs:   flag and path
	13, 8, 11, 7, 15, 16,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Bone Creeper",			// name
	40,	42,	53,		// DLVL range & MLVL
	500,	750, // HP range,
	AI_15_Hidden,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	32,	8,	35,	55,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	73, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	750,		// special flag for drop, animation size, XP
	"X\\Skeleton010-white.trn", "Frozen Creeper" // ice age  TRN & name
},
{
	BM_9_SKELETON_WARRIOR,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelAxe\\SklAx%c.CL2",	1,	"Monsters\\SkelAxe\\SklAx%c%i.WAV",		0,
	1,	"Monsters\\SkelAxe\\White.TRN",	// TRNs:   flag and path
	12, 8, 13, 6, 17, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Skeleton Warrior",			// name
	2,	4,	7,		// DLVL range & MLVL
	45,	75, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	7,	8,	6,	14,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	9, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	130,		// special flag for drop, animation size, XP
	"X\\Skeleton011-icestalker3.trn", "Ice Stalker" // ice age  TRN & name
}, 
{
	BM_10_BURNING_DEAD,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelAxe\\SklAx%c.CL2",	1,	"Monsters\\SkelAxe\\SklAx%c%i.WAV",		0,
	1,	"X\\Skeleton008-orange.trn",	// TRNs:   flag and path
	12, 8, 13, 6, 17, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Burning Dead",			// name
	4,	6,	10,		// DLVL range & MLVL
	75,	120, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	13,	8,	10,	20,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	13, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	180,		// special flag for drop, animation size, XP
	"X\\Skeleton010-white.trn", "Freezing Dead" // ice age  TRN & name
},
{
	BM_11_HATRED,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelAxe\\SklAx%c.CL2",	1,	"Monsters\\SkelAxe\\SklAx%c%i.WAV",		0,
	1,	"Monsters\\SkelAxe\\Black.TRN",	// TRNs:   flag and path
	12, 8, 13, 6, 17, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Hatred",			// name
	6,	8,	14,		// DLVL range & MLVL
	105,	165, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	15,	8,	15,	25,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	16, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	240,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_12_IMP,	1, 60,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FalSword\\Fall%c.CL2",	1,	"Monsters\\BigFall\\bfal%c%i.WAV",		1,
	1,	"Monsters\\FalSword\\Blue.TRN",	// TRNs:   flag and path
	12, 12, 12, 11, 14, 15,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Imp",			// name
	12,	16,	25,		// DLVL range & MLVL
	240,	375, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	16,	8,	23,	36,// tohit, hit frame of 1st attack, dmg range.
	0,	5,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	28, 1, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT|MR_11_IM_COLD,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	375,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_13_DARK_ONE,	1, 60,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FalSword\\Fall%c.CL2",	1,	"Monsters\\BigFall\\bfal%c%i.WAV",		1,
	1,	"X\\Fallen04.TRN",	// TRNs:   flag and path
	12, 12, 12, 11, 14, 15,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Dark One",			// name
	16,	24,	31,		// DLVL range & MLVL
	300,	500, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	16,	8,	25,	45,// tohit, hit frame of 1st attack, dmg range.
	0,	5,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	35, 1, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,// armor, mon type, and resistances
	0,		3,	614,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_14_SOUL_HUNTER,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelBow\\SklBw%c.CL2",	1,	"X\\sh%c%i.wav",		0,
	1,	"X\\Skeleton005-crimson.trn",	// TRNs:   flag and path
	9, 8, 16, 5, 16, 16,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Soul Hunter",			// name
	40,	42,	53,		// DLVL range & MLVL
	475,	755, // HP range,
	AI_37_Ranger_Fire_Arrow,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	32,	11,	38,	48,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	68, 0, MR_1_ARCAN|MR_3_LIGHT|MR_5_IM_FIRE,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	0,		3,	700,		// special flag for drop, animation size, XP
	"X\\Skeleton010-white.trn", 0 // ice age  TRN & name
},
{
	BM_15_TOMB_SLAVE,	1, 0,	124,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Gravdg\\Gravdg%c.CL2",	1,	"Monsters\\newsfx\\diga%c%i.WAV",		1,
	1,	"Monsters\\Mega\\Balr.TRN",	// TRNs:   flag and path
	24, 24, 12, 6, 16, 16,		// IWAHDS frames
	3, 0, 0, 2, 2, 0,		// delays
	"Tomb Slave",			// name
	2,	6,	8,		// DLVL range & MLVL
	90,	180, // HP range,
	AI_4_Scavenger,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	10,	6,	8,	19,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	10, 0, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	140,		// special flag for drop, animation size, XP
	"X\\Gravdg04_iceage_lvl-based.trn", "Rot Eater" // ice age  TRN & name
},
{
	BM_16_HORNED_BEAST,	1, 98,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Rhino\\Rhino%c.CL2",	1,	"Monsters\\Rhino\\Rhino%c%i.WAV",		1,
	1,	"Monsters\\Acid\\AcidB.TRN",	// TRNs:   flag and path
	8, 8, 14, 6, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Horned Beast",			// name
	20,	26,	42,		// DLVL range & MLVL
	630,	825, // HP range,
	AI_5_Charger,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	14,	7,	38,	56,// tohit, hit frame of 1st attack, dmg range.
	40,	0,	54,	56,	 // 2nd attack: tohit, hit frame, dmg range
	48, 2, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_11_IM_COLD,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	895,		// special flag for drop, animation size, XP
	0, "Winter Horn" // ice age  TRN & name
}, 
{
	BM_17_SCAVENGER,	1, 47,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Scav\\Scav%c.CL2",	1,	"Monsters\\Scav\\Scav%c%i.WAV",		0,
	1,	"Monsters\\FalSpear\\FallenT.TRN",	// TRNs:   flag and path
	12, 8, 12, 6, 20, 11,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Scavenger",			// name
	8,	10,	15,		// DLVL range & MLVL
	80,	135, // HP range,
	AI_4_Scavenger,	MF_9_CHASE_TARGET, 2,	// AI, int flag, int level
	17,	7,	10,	24,// tohit, hit frame of 1st attack, dmg range.
	27,	0,	30,	30,	 // 2nd attack: tohit, hit frame, dmg range
	17, 2, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	16384,		3,	255,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_18_VORTEX_ROGUE,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Mega\\Mega%c.CL2",	1,	"Monsters\\Mega\\Mega%c%i.WAV",		1,
	1,	"X\\Mega_02-VortexRogue.TRN",	// TRNs:   flag and path
	6, 7, 14, 1, 24, 5,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Vortex Rogue",			// name
	16,	22,	33,		// DLVL range & MLVL
	550,	600, // HP range,
	AI_26_Balrog,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	16,	8,	30,	40,// tohit, hit frame of 1st attack, dmg range.
	15,	3,	40,	60,	 // 2nd attack: tohit, hit frame, dmg range
	37, 1, MR_1_ARCAN|MR_3_LIGHT|MR_5_IM_FIRE|MR_7_IM_CURSE,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	860,		// special flag for drop, animation size, XP
	"monsters\\mega\\vtexl.TRN", "Vortex Lord" // ice age  TRN & name
},
{
	BM_19_RAVAGER,	1, 47,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Scav\\Scav%c.CL2",	1,	"Monsters\\Scav\\Scav%c%i.WAV",		0,
	0,	"Monsters\\FalSpear\\FallenT.TRN",	// TRNs:   flag and path
	12, 5, 12, 6, 20, 11,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Ravager",			// name
	12,	16,	23,		// DLVL range & MLVL
	160,	220, // HP range,
	AI_4_Scavenger,	MF_9_CHASE_TARGET, 3,	// AI, int flag, int level
	17,	7,	14,	30,// tohit, hit frame of 1st attack, dmg range.
	32,	0,	30,	40,	 // 2nd attack: tohit, hit frame, dmg range
	26, 2, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	16384,		3,	300,		// special flag for drop, animation size, XP
	"x\\Scav_01_Snow_Jumper.trn", "Snow Jumper" // ice age  TRN & name
},
{
	BM_20_RISEN_SKELETON,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelBow\\SklBw%c.CL2",	1,	"Monsters\\SkelBow\\SklBw%c%i.WAV",		0,
	1,	"X\\M1.trn",	// TRNs:   flag and path
	9, 8, 16, 5, 16, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Risen Skeleton",			// name
	0,	2,	4,		// DLVL range & MLVL
	16,	22, // HP range,
	AI_3_Skel_Archer,	0, 0,	// AI, int flag, int level
	6,	12,	1,	5,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	2, 0, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	90,		// special flag for drop, animation size, XP
	"X\\SKing006-blue.trn", "Frozen Skeleton" // ice age  TRN & name
},
{
	BM_21_SKELETON_WARRIOR,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelBow\\SklBw%c.CL2",	1,	"Monsters\\SkelBow\\SklBw%c%i.WAV",		0,
	1,	"Monsters\\SkelBow\\White.TRN",	// TRNs:   flag and path
	9, 8, 16, 5, 16, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Skeleton Warrior",			// name
	2,	4,	6,		// DLVL range & MLVL
	24,	48, // HP range,
	AI_3_Skel_Archer,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	10,	12,	2,	13,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	7, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	105,		// special flag for drop, animation size, XP
	"X\\Skeleton011-icestalker3.trn", "Ice Stalker" // ice age  TRN & name
},
{
	BM_22_BURNING_DEAD,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelBow\\SklBw%c.CL2",	1,	"Monsters\\SkelBow\\SklBw%c%i.WAV",		0,
	1,	"X\\dangel.trn",	// TRNs:   flag and path
	9, 8, 16, 5, 16, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Burning Dead",			// name
	4,	6,	11,		// DLVL range & MLVL
	40,	70, // HP range,
	AI_3_Skel_Archer,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	12,	12,	8,	20,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	11, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	160,		// special flag for drop, animation size, XP
	"X\\Skeleton010-white.trn", "Freezing Dead" // ice age  TRN & name
},
{
	BM_23_MALICE,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelBow\\SklBw%c.CL2",	1,	"Monsters\\SkelBow\\SklBw%c%i.WAV",		0,
	1,	"Monsters\\SkelBow\\Black.TRN",	// TRNs:   flag and path
	9, 8, 16, 5, 16, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Malice",			// name
	6,	10,	14,		// DLVL range & MLVL
	55,	95, // HP range,
	AI_3_Skel_Archer,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	16,	12,	11,	25,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	15, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	190,		// special flag for drop, animation size, XP
	0, "Skeletal Ashes" // ice age  TRN & name
}, 
{
	BM_24_RISEN_SKELETON,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelSd\\SklSr%c.CL2",	1,	"Monsters\\SkelSd\\SklSr%c%i.WAV",		1,
	1,	"X\\M1.trn",	// TRNs:   flag and path
	13, 8, 12, 7, 15, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Risen Skeleton",			// name
	0,	2,	3,		// DLVL range & MLVL
	21,	33, // HP range,
	AI_2_Skel_Melee,	0, 0,	// AI, int flag, int level
	6,	8,	2,	6,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	4, 0, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	110,		// special flag for drop, animation size, XP
	"X\\SKing006-blue.trn", "Frozen Skeleton" // ice age  TRN & name
},
{
	BM_25_SKELETON_WARRIOR,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelSd\\SklSr%c.CL2",	1,	"Monsters\\SkelSd\\SklSr%c%i.WAV",		1,
	1,	"X\\F1.trn",	// TRNs:   flag and path
	13, 8, 12, 7, 15, 16,		// IWAHDS frames
	0, 0, 0, 0, 1, 0,		// delays
	"Skeleton Warrior",			// name
	2,	6,	9,		// DLVL range & MLVL
	58,	77, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	12,	8,	5,	12,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	10, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	150,		// special flag for drop, animation size, XP
	"X\\Skeleton011-icestalker3.trn", "Ice Stalker" // ice age  TRN & name
},
{
	BM_26_BURNING_DEAD,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelSd\\SklSr%c.CL2",	1,	"Monsters\\SkelSd\\SklSr%c%i.WAV",		1,
	1,	"X\\dangel.trn",	// TRNs:   flag and path
	13, 8, 12, 7, 15, 16,		// IWAHDS frames
	0, 0, 0, 0, 1, 0,		// delays
	"Burning Dead",			// name
	4,	8,	12,		// DLVL range & MLVL
	75,	135, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	13,	8,	8,	18,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	15, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	180,		// special flag for drop, animation size, XP
	"X\\Skeleton010-white.trn", "Freezing Dead" // ice age  TRN & name
},
{
	BM_27_HORROR,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelSd\\SklSr%c.CL2",	1,	"Monsters\\SkelSd\\SklSr%c%i.WAV",		1,
	1,	"Monsters\\SkelSd\\Black.TRN",	// TRNs:   flag and path
	13, 8, 12, 7, 15, 16,		// IWAHDS frames
	0, 0, 0, 0, 1, 0,		// delays
	"Horror",			// name
	6,	10,	16,		// DLVL range & MLVL
	135,	225, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	18,	8,	12,	25,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	19, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	260,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_28_BLOOD_WARPER,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\TSneak\\TSneak%c.CL2",	1,	"Monsters\\TSneak\\Bclop%c%i.wav",		1,
	1,	"X\\Warper04_RED.TRN",	// TRNs:   flag and path
	13, 13, 15, 11, 16, 13,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Blood Warper",			// name
	32,	38,	52,		// DLVL range & MLVL
	750,	950, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	19,	10,	44,	56,// tohit, hit frame of 1st attack, dmg range.
	0,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	65, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE | MR_8_IM_ACID | MR_11_IM_COLD,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE| MR_8_IM_ACID| MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	580,		// special flag for drop, animation size, XP
	"X\\Tsneak005_blu.trn", "Snow Warper" // ice age  TRN & name
},
{
	BM_29_HIDDEN,	1, 63,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Sneak\\Sneak%c.CL2",	1,	"Monsters\\Sneak\\Sneak%c%i.WAV",		0,
	1,	"Monsters\\Fat\\FatB.TRN",	// TRNs:   flag and path
	16, 8, 12, 8, 24, 15,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Hidden",			// name
	18,	28,	35,		// DLVL range & MLVL
	300,	400, // HP range,
	AI_15_Hidden,	MF_1_INVISIBLE | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	17,	7,	23,	44,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	40, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	550,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_30_SHADOW,	1, 63,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Sneak\\Sneak%c.CL2",	1,	"Monsters\\Sneak\\Sneak%c%i.WAV",		0,
	1,	"Monsters\\Thin\\Thinv1.TRN",	// TRNs:   flag and path
	16, 8, 12, 8, 24, 15,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Shadow",			// name
	8,	12,	18,		// DLVL range & MLVL
	150,	210, // HP range,
	AI_15_Hidden,	MF_1_INVISIBLE | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	18,	8,	10,	25,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	20, 1, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	300,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_31_UNSEEN,	1, 63,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Sneak\\Sneak%c.CL2",	1,	"Monsters\\Sneak\\Sneak%c%i.WAV",		0,
	1,	"X\\K2.trn",	// TRNs:   flag and path
	16, 8, 12, 8, 24, 15,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Unseen",			// name
	10,	18,	22,		// DLVL range & MLVL
	165,	255, // HP range,
	AI_15_Hidden,	MF_1_INVISIBLE | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	16,	8,	12,	30,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	24, 1, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	350,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_32_INVISIBLE_ONE,	1, 63,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Sneak\\Sneak%c.CL2",	1,	"Monsters\\Sneak\\Sneak%c%i.WAV",		0,
	1,	"X\\warper03-crimzon.TRN",	// TRNs:   flag and path
	16, 8, 12, 8, 24, 15,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Invisible One",			// name
	12,	22,	25,		// DLVL range & MLVL
	225,	315, // HP range,
	AI_15_Hidden,	MF_1_INVISIBLE | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	16,	7,	14,	40,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	29, 1, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT,	MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	380,		// special flag for drop, animation size, XP
	"X\\Hidden28_mixed_blue_w_purple.trn", "Illusion Weaver" // ice age  TRN & name
},
{
	BM_33_MOON_CLAN,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatLord\\GoatL%c.CL2",	1,	"Monsters\\newsfx\\Satyr%c%i.WAV",		1,
	1,	"Monsters\\Rhino\\Blue.TRN",	// TRNs:   flag and path
	13, 13, 13, 8, 16, 9,		// IWAHDS frames
	3, 0, 0, 0, 0, 3,		// delays
	"Moon Clan",			// name
	10,	14,	18,		// DLVL range & MLVL
	400,	450, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	20,	8,	23,	40,// tohit, hit frame of 1st attack, dmg range.
	40,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	24, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	600,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_34_FLESH_CLAN,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatMace\\Goat%c.CL2",	1,	"Monsters\\GoatMace\\Goat%c%i.WAV",		1,
	0,	"Monsters\\Snake\\SnakR.TRN",	// TRNs:   flag and path
	12, 8, 12, 6, 20, 12,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Flesh Clan",			// name
	8,	14,	16,		// DLVL range & MLVL
	200,	250, // HP range,
	AI_6_Goat_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	17,	8,	12,	22,// tohit, hit frame of 1st attack, dmg range.
	22,	7,	20,	30,	 // 2nd attack: tohit, hit frame, dmg range
	19, 1, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	0,		3,	290,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_35_FIRE_CLAN,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatMace\\Goat%c.CL2",	1,	"Monsters\\GoatMace\\Goat%c%i.WAV",		1,
	1,	"X\\Fireclan.TRN",	// TRNs:   flag and path
	12, 8, 12, 6, 20, 12,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Fire Clan",			// name
	10,	16,	20,		// DLVL range & MLVL
	230,	300, // HP range,
	AI_6_Goat_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	17,	8,	14,	28,// tohit, hit frame of 1st attack, dmg range.
	23,	7,	25,	35,	 // 2nd attack: tohit, hit frame, dmg range
	23, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	0,		3,	330,		// special flag for drop, animation size, XP
	"X\\Goat17.trn", "War Clan" // ice age  TRN & name
}, 
{
	BM_36_DARKNESS_CLAN,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatMace\\Goat%c.CL2",	1,	"Monsters\\GoatMace\\Goat%c%i.WAV",		1,
	1,	"Monsters\\GoatMace\\Gray.TRN",	// TRNs:   flag and path
	12, 8, 12, 6, 20, 12,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Darkness Clan",			// name
	12,	18,	23,		// DLVL range & MLVL
	280,	400, // HP range,
	AI_6_Goat_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	17,	8,	16,	36,// tohit, hit frame of 1st attack, dmg range.
	22,	7,	30,	40,	 // 2nd attack: tohit, hit frame, dmg range
	26, 1, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN|MR_11_IM_COLD,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	400,		// special flag for drop, animation size, XP
	0, "Obsidian Clan" // ice age  TRN & name
}, 
{
	BM_37_STONE_CLAN,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatMace\\Goat%c.CL2",	1,	"Monsters\\GoatMace\\Goat%c%i.WAV",		1,
	1,	"Monsters\\Fat\\FatB.TRN",	// TRNs:   flag and path
	12, 8, 12, 6, 20, 12,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Stone Clan",			// name
	14,	20,	26,		// DLVL range & MLVL
	350,	450, // HP range,
	AI_6_Goat_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	17,	8,	20,	44,// tohit, hit frame of 1st attack, dmg range.
	21,	7,	35,	45,	 // 2nd attack: tohit, hit frame, dmg range
	30, 1, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	440,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_38_UNDEAD_WEREWOLF,	1, 57,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"monsters\\wolf\\wolf%c.cl2",	1,	"monsters\\wolf\\wolg%c%i.wav",		1,
	1,	"X\\Acid_05_DIRTY_BLUE.TRN",	// TRNs:   flag and path
	13, 8, 12, 8, 20, 12,		// IWAHDS frames
	0, 0, 0, 0, 2, 0,		// delays
	"Undead Werewolf",			// name
	4,	8,	14,		// DLVL range & MLVL
	90,	145, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET, 0,	// AI, int flag, int level
	12,	7,	8,	18,// tohit, hit frame of 1st attack, dmg range.
	0,	5,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	14, 0, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	270,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_39_WARPING_DEMON,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\TSneak\\TSneak%c.CL2",	0,	"Monsters\\TSneak\\sneakl%c%i.wav",		0,
	1,	"X\\K1.trn",	// TRNs:   flag and path
	13, 13, 15, 10, 16, 0,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Warping Demon",			// name
	42,	44,	57,		// DLVL range & MLVL
	850,	1150, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	28,	9,	47,	58,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	72, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_11_IM_COLD,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	880,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_40_QUASIT,	1, 0,	96,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Bat\\Bat%c.CL2",	1,	"Monsters\\Bat\\bat%c%i.wav",		1,
	1,	"Monsters\\Bat\\grey.trn",	// TRNs:   flag and path
	9, 13, 10, 9, 13, 10,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Quasit",			// name
	4,	10,	16,		// DLVL range & MLVL
	60,	90, // HP range,
	AI_11_Bat,	MF_9_CHASE_TARGET, 2,	// AI, int flag, int level
	14,	5,	6,	18,// tohit, hit frame of 1st attack, dmg range.
	14,	2,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	17, 2, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	16384,		6,	200,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_41_DARK_SERAPH,	1, 0,	96,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Bat\\Bat%c.CL2",	0,	"Monsters\\Bat\\bat%c%i.wav",		0,
	1,	"X\\RO.trn",	// TRNs:   flag and path
	9, 13, 10, 9, 13, 0,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Dark Seraph",			// name
	10,	12,	20,		// DLVL range & MLVL
	75,	110, // HP range,
	AI_41_Ranger_snowbolt,	MF_9_CHASE_TARGET, 3,	// AI, int flag, int level
	17,	5,	11,	27,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	25, 2, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT|MR_11_IM_COLD,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_11_IM_COLD,// armor, mon type, and resistances
	16384,		6,	310,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_42_NIGHT_CLAN,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatBow\\GoatB%c.CL2",	0,	"Monsters\\GoatBow\\GoatB%c%i.WAV",		0,
	1,	"X\\moonclan.TRN",	// TRNs:   flag and path
	12, 8, 16, 6, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Night Clan",			// name
	8,	14,	19,		// DLVL range & MLVL
	105,	150, // HP range,
	AI_33_Ranger_Lite_Arrow,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	16,	13,	8,	24,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	22, 1, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	310,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_43_BLOOD_CLAN,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatBow\\GoatB%c.CL2",	0,	"Monsters\\GoatBow\\GoatB%c%i.WAV",		0,
	1,	"X\\blodclan.TRN",	// TRNs:   flag and path
	12, 8, 16, 6, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Blood Clan",			// name
	10,	16,	22,		// DLVL range & MLVL
	120,	195, // HP range,
	AI_37_Ranger_Fire_Arrow,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	17,	13,	10,	28,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	25, 1, MR_1_ARCAN|MR_3_LIGHT|MR_5_IM_FIRE,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	0,		3,	330,		// special flag for drop, animation size, XP
	"X\\moonclan.TRN", "Night Clan" // ice age  TRN & name
},
{
	BM_44_DOOM_CLAN,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatBow\\GoatB%c.CL2",	0,	"Monsters\\GoatBow\\GoatB%c%i.WAV",		0,
	1,	"Monsters\\GoatMace\\Gray.TRN",	// TRNs:   flag and path
	12, 8, 16, 6, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Doom Clan",			// name
	12,	20,	25,		// DLVL range & MLVL
	135,	240, // HP range,
	AI_7_Goat_Archer,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	16,	13,	16,	46,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	28, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	380,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_45_VENOM_CLAN,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatBow\\GoatB%c.CL2",	1,	"Monsters\\GoatBow\\GoatB%c%i.WAV",		1,
	1,	"Monsters\\Fat\\FatB.TRN",	// TRNs:   flag and path
	12, 8, 16, 6, 20, 16,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Venom Clan",			// name
	14,	22,	27,		// DLVL range & MLVL
	200,	300, // HP range,
	AI_43_Ranger_Acid_Arrow,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	20,	13,	16,	34,// tohit, hit frame of 1st attack, dmg range.
	22,	12,	58,	62,	 // 2nd attack: tohit, hit frame, dmg range
	32, 1, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT|MR_8_IM_ACID,	MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT|MR_8_IM_ACID,// armor, mon type, and resistances
	0,		3,	420,		// special flag for drop, animation size, XP
	"X\\Goat_33_iceage_purple_bluev3.trn", "Frost Clan" // ice age  TRN & name
},
{
	BM_46_PIT_SPIDER,	1, 0,	148,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\bSpidr\\bSpidr%c.CL2",	1,	"Monsters\\newsfx\\SLord%c%i.WAV",		1,
	1,	"X\\Arah.trn",	// TRNs:   flag and path
	12, 10, 15, 6, 20, 10,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Pit Spider",			// name
	32,	38,	53,		// DLVL range & MLVL
	750,	900, // HP range,
	AI_19_Acid_Beast,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	19,	11,	40,	56,// tohit, hit frame of 1st attack, dmg range.
	41,	9,	44,	58,	 // 2nd attack: tohit, hit frame, dmg range
	65, 2, MR_1_ARCAN|MR_3_LIGHT|MR_5_IM_FIRE|MR_8_IM_ACID,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE|MR_8_IM_ACID,// armor, mon type, and resistances
	0,		7,	730,		// special flag for drop, animation size, XP
	"x\\Goat_31_icy.trn", "Black Fang" // ice age  TRN & name
},
{
	BM_47_ACID_HOUND,	1, 57,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Acid\\Acid%c.CL2",	1,	"Monsters\\Acid\\Acid%c%i.WAV",		1,
	1,	"Monsters\\Acid\\AcidBlk.TRN",	// TRNs:   flag and path
	13, 8, 12, 8, 16, 12,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Acid Hound",			// name
	12,	18,	24,		// DLVL range & MLVL
	220,	290, // HP range,
	AI_19_Acid_Beast,	0, 1,	// AI, int flag, int level
	20,	8,	14,	36,// tohit, hit frame of 1st attack, dmg range.
	16,	8,	1,	2,	 // 2nd attack: tohit, hit frame, dmg range
	29, 2, MR_1_ARCAN|MR_3_LIGHT|MR_5_IM_FIRE|MR_8_IM_ACID,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_8_IM_ACID,// armor, mon type, and resistances
	0,		3,	350,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_48_ACID_BAT,	1, 0,	96,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Hellbat2\\bhelbt%c.CL2",	1,	"Monsters\\newsfx\\HelBat%c%i.WAV",		0,
	1,	"X\\M2.trn",	// TRNs:   flag and path
	16, 16, 14, 6, 18, 14,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Acid Bat",			// name
	32,	36,	51,		// DLVL range & MLVL
	300,	420, // HP range,
	AI_20_Acid_Beast_Ranger,	0, 1,	// AI, int flag, int level
	20,	8,	24,	48,// tohit, hit frame of 1st attack, dmg range.
	20,	3,	4,	8,	 // 2nd attack: tohit, hit frame, dmg range
	63, 2, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_8_IM_ACID,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_8_IM_ACID,// armor, mon type, and resistances
	16384,		7,	335,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_49_BILE_SPITTER,	1, 57,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Acid\\Acid%c.CL2",	1,	"Monsters\\Acid\\Acid%c%i.WAV",		1,
	1,	"X\\Mega_01.TRN",	// TRNs:   flag and path
	13, 8, 12, 7, 16, 12,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Bile Spitter",			// name
	40,	44,	55,		// DLVL range & MLVL
	650,	1050, // HP range,
	AI_19_Acid_Beast,	MF_9_CHASE_TARGET, 2,	// AI, int flag, int level
	30,	6,	46,	60,// tohit, hit frame of 1st attack, dmg range.
	30,	4,	28,	46,	 // 2nd attack: tohit, hit frame, dmg range
	73, 2, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_8_IM_ACID,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_8_IM_ACID,// armor, mon type, and resistances
	0,		3,	875,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_50_SKELETON_LORD,	0, 100,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SKing\\SKing%c.CL2",	1,	"Monsters\\SKing\\SKing%c%i.WAV",		0,
	1,	"Monsters\\Scav\\ScavW.TRN",	// TRNs:   flag and path
	8, 8, 16, 5, 16, 12,		// IWAHDS frames
	2, 0, 0, 0, 0, 2,		// delays
	"Skeleton Lord",			// name
	6,	6,	57,		// DLVL range & MLVL
	7000,	7000, // HP range,
	AI_10_Skel_King,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	35,	8,	55,	60,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	80, 0, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	4000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_51_THE_BUTCHER,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FatC\\FatC%c.CL2",	1,	"X\\Buch%c%i.wav",		1,
	1,	"Monsters\\Snake\\SnakR.TRN",	// TRNs:   flag and path
	10, 8, 9, 1, 16, 1,		// IWAHDS frames
	1, 0, 0, 0, 0, 0,		// delays
	"The Butcher",			// name
	0,	0,	55,		// DLVL range & MLVL
	4440,	4440, // HP range,
	AI_8_Fallen_One,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	30,	6,	25,	55,// tohit, hit frame of 1st attack, dmg range.
	30,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	70, 1, MR_1_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	2500,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_52_BILE_DEMON,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Fat\\Fat%c.CL2",	1,	"Monsters\\Fat\\Fat%c%i.WAV",		1,
	1,	"X\\Fat_05new.TRN",	// TRNs:   flag and path
	8, 10, 15, 6, 16, 10,		// IWAHDS frames
	4, 0, 0, 0, 0, 0,		// delays
	"Bile Demon",			// name
	8,	16,	20,		// DLVL range & MLVL
	350,	400, // HP range,
	AI_1_Overlord,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	16,	7,	17,	38,// tohit, hit frame of 1st attack, dmg range.
	21,	6,	25,	45,	 // 2nd attack: tohit, hit frame, dmg range
	27, 1, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN|MR_7_IM_CURSE,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	450,		// special flag for drop, animation size, XP
	"X\\Fat_09_iceage_mudman_darkened_blue.trn", "Mud Man" // ice age  TRN & name
}, 
{
	BM_53_HELL_HOUND,	1, 57,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Acid\\Acid%c.CL2",	1,	"X\\hh%c%i.WAV",		1,
	1,	"X\\HH2.TRN",	// TRNs:   flag and path
	13, 8, 12, 8, 17, 4,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Hell Hound",			// name
	18,	24,	33,		// DLVL range & MLVL
	335,	445, // HP range,
	AI_26_Balrog,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	20,	7,	20,	45,// tohit, hit frame of 1st attack, dmg range.
	16,	2,	15,	40,	 // 2nd attack: tohit, hit frame, dmg range
	39, 2, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	0,		3,	631,		// special flag for drop, animation size, XP
	"X\\Acid_06_white_v2.trn", "Snow Hound" // ice age  TRN & name
},
{
	BM_54_BEHOLDER,	1, 0,	140,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Eye2\\Eye2%c.CL2",	0,	"Monsters\\Eye2\\Orb%c%i.wav",		0,
	1,	"Monsters\\Fat\\FatF.TRN",	// TRNs:   flag and path
	12, 3, 13, 7, 21, 0,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Beholder",			// name
	22,	26,	38,		// DLVL range & MLVL
	250,	300, // HP range,
	AI_38_Beholder,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	18,	8,	24,	45,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	43, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		6,	520,		// special flag for drop, animation size, XP
	"cf\\iceage\\_nonsense.trn", "Orb" // ice age  TRN & name
},
{
	BM_55_GRIM_DEMON,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Fat\\Fat%c.CL2",	1,	"Monsters\\Fat\\Fat%c%i.WAV",		1,
	1,	"X\\Gru.trn",	// TRNs:   flag and path
	8, 10, 15, 5, 16, 10,		// IWAHDS frames
	4, 0, 0, 0, 0, 0,		// delays
	"Grim Demon",			// name
	42,	44,	57,		// DLVL range & MLVL
	1400,	1900, // HP range,
	AI_1_Overlord,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 8,	// AI, int flag, int level
	28,	6,	60,	61,// tohit, hit frame of 1st attack, dmg range.
	36,	5,	57,	63,	 // 2nd attack: tohit, hit frame, dmg range
	77, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE | MR_8_IM_ACID |MR_11_IM_COLD,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE | MR_8_IM_ACID |MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	1460,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_56_PIT_DEMON,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Fat\\Fat%c.CL2",	1,	"Monsters\\Fat\\Fat%c%i.WAV",		1,
	1,	"X\\PB.trn",	// TRNs:   flag and path
	8, 10, 15, 6, 16, 10,		// IWAHDS frames
	4, 0, 0, 0, 0, 0,		// delays
	"Pit Demon",			// name
	14,	22,	28,		// DLVL range & MLVL
	500,	550, // HP range,
	AI_1_Overlord,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	17,	7,	25,	45,// tohit, hit frame of 1st attack, dmg range.
	27,	5,	40,	55,	 // 2nd attack: tohit, hit frame, dmg range
	32, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE | MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	666,		// special flag for drop, animation size, XP
	"X\\Fat_08_icy.trn", "Cave Demon" // ice age  TRN & name
},
{
	BM_57_HELL_CLAN,	0, 0,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatLord\\GoatL%c.CL2",	1,	"Monsters\\GoatLord\\GoatL%c%i.WAV",		0,
	1,	"X\\RO.trn",	// TRNs:   flag and path
	13, 13, 13, 8, 16, 9,		// IWAHDS frames
	3, 0, 1, 1, 2, 1,		// delays
	"Hell Clan",			// name
	10,	10,	20,		// DLVL range & MLVL
	400,	500, // HP range,
	AI_54_Mixed_Chargedbolt,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	17,	8,	21,	37,// tohit, hit frame of 1st attack, dmg range.
	57,	3,	30,	40,	 // 2nd attack: tohit, hit frame, dmg range
	25, 1, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN|MR_7_IM_CURSE,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	1000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_58_DEFILER,	0, 134,	226,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Nkr\\Nkr%c.CL2",	1,	"Monsters\\newsfx\\Nakrul%c%i.WAV",		1,
	1,	"X\\Abg.trn",	// TRNs:   flag and path
	16, 6, 16, 6, 16, 16,		// IWAHDS frames
	1, 0, 0, 0, 0, 0,		// delays
	"Abyss Guardian",			// name
	38,	38,	59,		// DLVL range & MLVL
	1400,	1900, // HP range,
	AI_27_Diablo,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	55,	5,	48,	62,// tohit, hit frame of 1st attack, dmg range.
	55,	5,	57,	62,	 // 2nd attack: tohit, hit frame, dmg range
	70, 1, MR_1_ARCAN|MR_3_LIGHT|MR_5_IM_FIRE|MR_7_IM_CURSE,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	12000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_59_PIT_FIEND,	1, 120,	198,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Hellbug\\Hellbg%c.CL2",	1,	"Monsters\\newsfx\\Defile%c%i.WAV",		1,
	1,	"X\\Pitfy.trn",	// TRNs:   flag and path
	8, 8, 14, 6, 14, 12,		// IWAHDS frames
	2, 0, 1, 1, 2, 0,		// delays
	"Pit Fiend",			// name
	26,	30,	48,		// DLVL range & MLVL
	750,	975, // HP range,
	AI_1_Overlord,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 6,	// AI, int flag, int level
	14,	6,	27,	49,// tohit, hit frame of 1st attack, dmg range.
	35,	6,	59,	66,	 // 2nd attack: tohit, hit frame, dmg range
	53, 2, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE|MR_8_IM_ACID,	MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_8_IM_ACID,// armor, mon type, and resistances
	0,		7,	750,		// special flag for drop, animation size, XP
	"X\\HellBug_03_darko.trn", 0 // ice age  TRN & name
}, 
{
	BM_60_HELL_STONE,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Magma\\Magma%c.CL2",	1,	"Monsters\\Magma\\Magma%c%i.WAV",		1,
	1,	"X\\hs.trn",	// TRNs:   flag and path
	8, 10, 14, 7, 18, 18,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Hell Stone",			// name
	22,	26,	45,		// DLVL range & MLVL
	610,	780, // HP range,
	AI_9_Magma,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	7,	4,	24,	54,// tohit, hit frame of 1st attack, dmg range.
	34,	13,	18,	48,	 // 2nd attack: tohit, hit frame, dmg range
	48, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	700,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_61_BLOOD_STONE,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Magma\\Magma%c.CL2",	1,	"Monsters\\Magma\\Magma%c%i.WAV",		1,
	1,	"X\\W2.trn",	// TRNs:   flag and path
	8, 10, 14, 7, 18, 18,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Blood Stone",			// name
	14,	18,	28,		// DLVL range & MLVL
	330,	450, // HP range,
	AI_9_Magma,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	16,	4,	12,	48,// tohit, hit frame of 1st attack, dmg range.
	26,	14,	17,	32,	 // 2nd attack: tohit, hit frame, dmg range
	32, 1, MR_1_ARCAN|MR_3_LIGHT|MR_5_IM_FIRE|MR_7_IM_CURSE,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	513,		// special flag for drop, animation size, XP
	"X\\Magma_11_frozen_stone.trn", "Frozen Stone" // ice age  TRN & name
},
{
	BM_62_MAULER,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Magma\\Magma%c.CL2",	1,	"Monsters\\Magma\\Magma%c%i.WAV",		1,
	1,	"X\\Sm.trn",	// TRNs:   flag and path
	8, 10, 14, 7, 18, 18,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Mauler",			// name
	46,	46,	58,		// DLVL range & MLVL
	1550,	1900, // HP range,
	AI_9_Magma,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	34,	4,	60,	65,// tohit, hit frame of 1st attack, dmg range.
	49,	14,	58,	66,	 // 2nd attack: tohit, hit frame, dmg range
	84, 1, MR_1_ARCAN|MR_3_LIGHT|MR_5_IM_FIRE|MR_7_IM_CURSE,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	1930,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_63_LAVA_LORD,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Magma\\Magma%c.CL2",	1,	"Monsters\\Magma\\Magma%c%i.WAV",		1,
	1,	"Monsters\\Magma\\Yellow.TRN",	// TRNs:   flag and path
	8, 10, 14, 7, 18, 18,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Lava Lord",			// name
	18,	22,	34,		// DLVL range & MLVL
	420,	600, // HP range,
	AI_9_Magma,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	16,	4,	20,	50,// tohit, hit frame of 1st attack, dmg range.
	31,	14,	22,	42,	 // 2nd attack: tohit, hit frame, dmg range
	39, 1, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN|MR_7_IM_CURSE,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	809,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_64_DEATH_GUARDIAN,	0, 100,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SKing\\SKing%c.CL2",	1,	"Monsters\\SKing\\SKing%c%i.WAV",		1,
	1,	"X\\SKing001-beige.TRN",	// TRNs:   flag and path
	8, 8, 16, 6, 16, 12,		// IWAHDS frames
	2, 0, 0, 0, 2, 0,		// delays
	"Death Guardian",			// name
	40,	40,	55,		// DLVL range & MLVL
	1400,	1700, // HP range,
	AI_8_Fallen_One,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 6,	// AI, int flag, int level
	33,	8,	55,	65,// tohit, hit frame of 1st attack, dmg range.
	0,	6,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	75, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1590,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_65_HELL_SPAWN,	1, 98,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Rhino\\Rhino%c.CL2",	1,	"Monsters\\Rhino\\Rhino%c%i.WAV",		1,
	1,	"Monsters\\GoatMace\\Red.TRN",	// TRNs:   flag and path
	8, 8, 14, 6, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Hell Spawn",			// name
	26,	28,	43,		// DLVL range & MLVL
	675,	900, // HP range,
	AI_5_Charger,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	20,	7,	50,	56,// tohit, hit frame of 1st attack, dmg range.
	30,	0,	55,	60,	 // 2nd attack: tohit, hit frame, dmg range
	52, 2, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	835,		// special flag for drop, animation size, XP
	"X\\Rhino015_darker_orange.trn", 0 // ice age  TRN & name
}, 
{
	BM_66_HORNED_DEATH,	1, 98,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Rhino\\Rhino%c.CL2",	1,	"Monsters\\Rhino\\Rhino%c%i.WAV",		1,
	1,	"X\\Rhino010-crimson.TRN",	// TRNs:   flag and path
	8, 8, 14, 5, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Horned Death",			// name
	42,	46,	57,		// DLVL range & MLVL
	1350,	1950, // HP range,
	AI_5_Charger,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 9,	// AI, int flag, int level
	36,	6,	48,	56,// tohit, hit frame of 1st attack, dmg range.
	48,	0,	62,	66,	 // 2nd attack: tohit, hit frame, dmg range
	84, 2, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	1709,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_67_BLACK_HORN,	1, 98,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Rhino\\Rhino%c.CL2",	1,	"Monsters\\Rhino\\Rhino%c%i.WAV",		1,
	1,	"X\\TE.trn",	// TRNs:   flag and path
	8, 8, 14, 6, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Black Horn",			// name
	30,	36,	53,		// DLVL range & MLVL
	1200,	1500, // HP range,
	AI_5_Charger,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	16,	7,	44,	64,// tohit, hit frame of 1st attack, dmg range.
	45,	0,	60,	65,	 // 2nd attack: tohit, hit frame, dmg range
	65, 2, MR_1_ARCAN|MR_3_LIGHT|MR_5_IM_FIRE|MR_11_IM_COLD,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1050,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_68_GORE_VIPER,	1, 99,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Snake\\Snake%c.CL2",	1,	"Monsters\\Snake\\Snake%c%i.WAV",		1,
	1,	"X\\Xw.trn",	// TRNs:   flag and path
	12, 11, 13, 6, 18, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Gore Viper",			// name
	34,	38,	56,		// DLVL range & MLVL
	750,	1350, // HP range,
	AI_24_Viper,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 8,	// AI, int flag, int level
	14,	8,	44,	63,// tohit, hit frame of 1st attack, dmg range.
	44,	9,	49,	65,	 // 2nd attack: tohit, hit frame, dmg range
	67, 2, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	999,		// special flag for drop, animation size, XP
	"X\\Snake_13_iceage_goreviper.trn", "Gelid Viper" // ice age  TRN & name
}, 
{
	BM_69_WINGED_DEMON,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Gargoyle\\Gargo%c.CL2",	1,	"Monsters\\Gargoyle\\Gargo%c%i.WAV",		0,
	0,	"Monsters\\Gargoyle\\GargBr.TRN",	// TRNs:   flag and path
	14, 14, 14, 10, 18, 14,		// IWAHDS frames
	0, 0, 0, 0, 0, 2,		// delays
	"Winged Demon",			// name
	8,	14,	17,		// DLVL range & MLVL
	250,	350, // HP range,
	AI_12_Gargoyle,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	23,	7,	20,	35,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	23, 1, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		6,	300,		// special flag for drop, animation size, XP
	"cf\\iceage\\Gargoyle_03_iceage_icy_catacombz_v2.trn", "Ice Gargoyle" // ice age  TRN & name
},
{
	BM_70_FROST_WING,	1, 103,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Demskel\\Demskl%c.CL2",	1,	"Monsters\\newsfx\\SWing%c%i.WAV",		1,
	1,	"CF\\SH\\Stormwing.TRN",	// TRNs:   flag and path
	10, 8, 20, 6, 24, 16,		// IWAHDS frames
	3, 0, 0, 0, 1, 1,		// delays
	"Frost Wing",			// name
	14,	22,	29,		// DLVL range & MLVL
	400,	500, // HP range,
	AI_40_Bonewing_cold_blasts,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	20,	7,	24,	46,// tohit, hit frame of 1st attack, dmg range.
	20,	11,	27,	39,	 // 2nd attack: tohit, hit frame, dmg range
	32, 0, MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	560,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_71_THUNDER_LORD,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Thin\\Thin%c.CL2",	1,	"Monsters\\Thin\\Thin%c%i.WAV",		1,
	1,	"X\\TL.trn",	// TRNs:   flag and path
	8, 8, 18, 3, 17, 14,		// IWAHDS frames
	3, 0, 0, 0, 3, 0,		// delays
	"Thunder Lord",			// name
	46,	46,	58,		// DLVL range & MLVL
	1350,	1620, // HP range,
	AI_16_Thunder_Demon,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	34,	4,	61,	66,// tohit, hit frame of 1st attack, dmg range.
	34,	5,	60,	62,	 // 2nd attack: tohit, hit frame, dmg range
	82, 1, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	1770,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_72_FIREWING,	1, 103,	128,	1808,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Demskel\\Demskl%c.CL2",	1,	"Monsters\\newsfx\\SWing%c%i.WAV",		1,
	1,	"X\\skelwing_05_darkred2.TRN",	// TRNs:   flag and path
	10, 8, 20, 6, 24, 4,		// IWAHDS frames
	1, 0, 0, 0, 1, 1,		// delays
	"Firewing",			// name
	6,	8,	12,		// DLVL range & MLVL
	150,	210, // HP range,
	AI_26_Balrog,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	14,	7,	4,	18,// tohit, hit frame of 1st attack, dmg range.
	14,	3,	6,	24,	 // 2nd attack: tohit, hit frame, dmg range
	13, 0, MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE | MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	370,		// special flag for drop, animation size, XP
	"X\\demskel_06_Frozen Horror.trn", "Glacial Horror" // ice age  TRN & name
},
{
	BM_73_DEATH_ANGEL,	1, 103,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Demskel\\Demskl%c.CL2",	1,	"Monsters\\newsfx\\SWing%c%i.WAV",		1,
	1,	"X\\dangel.trn",	// TRNs:   flag and path
	10, 8, 20, 5, 24, 4,		// IWAHDS frames
	3, 0, 0, 0, 1, 0,		// delays
	"Death Angel",			// name
	40,	42,	55,		// DLVL range & MLVL
	1170,	1300, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 6,	// AI, int flag, int level
	28,	7,	42,	61,// tohit, hit frame of 1st attack, dmg range.
	29,	3,	62,	64,	 // 2nd attack: tohit, hit frame, dmg range
	72, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	800,		// special flag for drop, animation size, XP
	"X\\demskel_06Frozen.TRN", "Frozen Wing" // ice age  TRN & name
},
{
	BM_74_GREATER_HULK,	1, 104,	138,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\HorkD\\HorkD%c.CL2",	1,	"Monsters\\newsfx\\HDemon%c%i.WAV",		1,
	1,	"X\\Hork07-HellGuy.TRN",	// TRNs:   flag and path
	15, 8, 16, 6, 16, 9,		// IWAHDS frames
	2, 0, 0, 0, 2, 2,		// delays
	"Greater Hulk",			// name
	26,	28,	44,		// DLVL range & MLVL
	675,	900, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	16,	9,	31,	57,// tohit, hit frame of 1st attack, dmg range.
	11,	3,	51,	56,	 // 2nd attack: tohit, hit frame, dmg range
	50, 0, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	900,		// special flag for drop, animation size, XP
	0, "Ice Hulk" // ice age  TRN & name
}, 
{
	BM_75_BLOOD_HULK,	1, 104,	138,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\HorkD\\HorkD%c.CL2",	1,	"Monsters\\newsfx\\HDemon%c%i.WAV",		1,
	1,	"X\\Hork05.TRN",	// TRNs:   flag and path
	15, 8, 16, 6, 16, 9,		// IWAHDS frames
	2, 0, 0, 0, 2, 2,		// delays
	"Blood Hulk",			// name
	34,	36,	54,		// DLVL range & MLVL
	1200,	1575, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	17,	9,	35,	60,// tohit, hit frame of 1st attack, dmg range.
	0,	3,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	66, 0, MR_1_ARCAN|MR_3_LIGHT|MR_5_IM_FIRE|MR_7_IM_CURSE,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	1300,		// special flag for drop, animation size, XP
	"X\\Hork07-HellGuy.TRN", "Ice Hulk" // ice age  TRN & name
},
{
	BM_76_DEATH_CLAW,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Thin\\Thin%c.CL2",	1,	"Monsters\\Thin\\Thin%c%i.WAV",		1,
	1,	"X\\Thin_14.TRN",	// TRNs:   flag and path
	8, 8, 18, 4, 17, 14,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Death Claw",			// name
	16,	20,	32,		// DLVL range & MLVL
	405,	555, // HP range,
	AI_16_Thunder_Demon,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	16,	5,	16,	36,// tohit, hit frame of 1st attack, dmg range.
	18,	8,	20,	50,	 // 2nd attack: tohit, hit frame, dmg range
	37, 1, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	707,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_77_STORM_DEMON,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Thin\\Thin%c.CL2",	1,	"Monsters\\Thin\\Thin%c%i.WAV",		1,
	1,	"X\\Thin_13.TRN",	// TRNs:   flag and path
	8, 8, 18, 4, 17, 14,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Storm Demon",			// name
	20,	24,	38,		// DLVL range & MLVL
	555,	720, // HP range,
	AI_16_Thunder_Demon,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	17,	5,	22,	52,// tohit, hit frame of 1st attack, dmg range.
	19,	8,	48,	60,	 // 2nd attack: tohit, hit frame, dmg range
	44, 1, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	917,		// special flag for drop, animation size, XP
	"x\\Thin_18_white_v2.trn", "Blizzard Chaser" // ice age  TRN & name
},
{
	BM_78_EVISCERATOR,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Thin\\Thin%c.CL2",	1,	"Monsters\\Thin\\Thin%c%i.WAV",		1,
	1,	"X\\Thin_12.TRN",	// TRNs:   flag and path
	8, 8, 18, 3, 17, 14,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Eviscerator",			// name
	46,	46,	58,		// DLVL range & MLVL
	1425,	1800, // HP range,
	AI_16_Thunder_Demon,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	34,	4,	63,	66,// tohit, hit frame of 1st attack, dmg range.
	34,	5,	62,	62,	 // 2nd attack: tohit, hit frame, dmg range
	85, 1, MR_1_ARCAN|MR_3_LIGHT|MR_5_IM_FIRE|MR_7_IM_CURSE,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	1870,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_79_FEAR_HUNTER,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Thin\\Thin%c.CL2",	1,	"Monsters\\Thin\\Thin%c%i.WAV",		1,
	1,	"Monsters\\Thin\\Thinv1.TRN",	// TRNs:   flag and path
	8, 8, 18, 4, 17, 14,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Fear Hunter",			// name
	26,	30,	47,		// DLVL range & MLVL
	720,	855, // HP range,
	AI_16_Thunder_Demon,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	13,	5,	35,	60,// tohit, hit frame of 1st attack, dmg range.
	16,	8,	50,	60,	 // 2nd attack: tohit, hit frame, dmg range
	51, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	860,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_80_DEVIL_KIN,	1, 77,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\BigFall\\Fallg%c.CL2",	0,	"Monsters\\newsfx\\KBrute%c%i.WAV",		0,
	0,	"X\\Wrm.trn",	// TRNs:   flag and path
	10, 8, 9, 8, 17, 0,		// IWAHDS frames
	0, 0, 0, 0, 2, 0,		// delays
	"Devil Kin",			// name
	42,	44,	56,		// DLVL range & MLVL
	900,	1200, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	32,	5,	47,	54,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	80, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	860,		// special flag for drop, animation size, XP
	"x\\BigFal04_blue.trn", 0 // ice age  TRN & name
},
{
	BM_81_FLYING_DEMON,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Gargoyle\\Gargo%c.CL2",	1,	"Monsters\\Gargoyle\\Gargo%c%i.WAV",		0,
	1,	"Monsters\\Gargoyle\\GargBr.TRN",	// TRNs:   flag and path
	14, 14, 14, 10, 18, 14,		// IWAHDS frames
	0, 0, 0, 0, 0, 2,		// delays
	"Flying Demon",			// name
	14,	18,	29,		// DLVL range & MLVL
	330,	450, // HP range,
	AI_12_Gargoyle,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	15,	7,	25,	45,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	31, 1, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		6,	440,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_82_GARGOYLE,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Gargoyle\\Gargo%c.CL2",	1,	"Monsters\\Gargoyle\\Gargo%c%i.WAV",		0,
	1,	"X\\Gargoyle_02_beige.TRN",	// TRNs:   flag and path
	14, 14, 14, 10, 18, 14,		// IWAHDS frames
	0, 0, 0, 0, 0, 2,		// delays
	"Gargoyle",			// name
	20,	24,	37,		// DLVL range & MLVL
	450,	750, // HP range,
	AI_12_Gargoyle,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	19,	7,	35,	55,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	42, 1, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		6,	680,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_83_LASH_WORM,	1, 0,	176,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Clasp\\Clasp%c.CL2",	0,	"Monsters\\newsfx\\Lworm%c%i.WAV",		0,
	1,	"Monsters\\Fat\\FatB.TRN",	// TRNs:   flag and path
	10, 12, 14, 6, 16, 0,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Lash Worm",			// name
	32,	38,	53,		// DLVL range & MLVL
	600,	1100, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	33,	8,	40,	55,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	64, 2, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN|MR_7_IM_CURSE,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	567,		// special flag for drop, animation size, XP
	0, "Cold Scissor" // ice age  TRN & name
}, 
{
	BM_84_DEATH_WING,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Gargoyle\\Gargo%c.CL2",	1,	"Monsters\\Gargoyle\\Gargo%c%i.WAV",		0,
	1,	"X\\Gargoyle_01_blue.TRN",	// TRNs:   flag and path
	14, 14, 14, 10, 18, 14,		// IWAHDS frames
	0, 0, 0, 0, 0, 2,		// delays
	"Death Wing",			// name
	42,	44,	55,		// DLVL range & MLVL
	975,	1700, // HP range,
	AI_12_Gargoyle,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	36,	6,	56,	64,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	78, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		6,	880,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_85_BALOR,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Mega\\Mega%c.CL2",	1,	"X\\Balor%c%i.wav",		1,
	1,	"X\\F1.trn",	// TRNs:   flag and path
	6, 7, 13, 1, 24, 5,		// IWAHDS frames
	3, 0, 0, 0, 3, 0,		// delays
	"Balor",			// name
	44,	46,	58,		// DLVL range & MLVL
	1350,	1800, // HP range,
	AI_26_Balrog,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	34,	8,	53,	67,// tohit, hit frame of 1st attack, dmg range.
	46,	2,	62,	62,	 // 2nd attack: tohit, hit frame, dmg range
	84, 1, MR_1_ARCAN|MR_3_LIGHT|MR_5_IM_FIRE|MR_7_IM_CURSE,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	1825,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_86_SLAYER,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Mega\\Mega%c.CL2",	1,	"Monsters\\Mega\\Mega%c%i.WAV",		1,
	1,	"X\\M1.trn",	// TRNs:   flag and path
	6, 7, 14, 1, 24, 5,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Slayer",			// name
	20,	26,	39,		// DLVL range & MLVL
	600,	750, // HP range,
	AI_26_Balrog,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	17,	8,	40,	50,// tohit, hit frame of 1st attack, dmg range.
	17,	3,	45,	60,	 // 2nd attack: tohit, hit frame, dmg range
	44, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE|MR_11_IM_COLD,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	950,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_87_DARK_REAPER,	1, 0,	180,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Reaper\\Reap%c.CL2",	1,	"Monsters\\newsfx\\Reaper%c%i.WAV",		1,
	1,	"X\\Kr.trn",	// TRNs:   flag and path
	12, 10, 14, 4, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 2, 3,		// delays
	"Dark Reaper",			// name
	20,	24,	37,		// DLVL range & MLVL
	560,	660, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	16,	7,	28,	52,// tohit, hit frame of 1st attack, dmg range.
	16,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	43, 1, MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	705,		// special flag for drop, animation size, XP
	"x\\Reaper_17_White.trn", "Night Reaper" // ice age  TRN & name
}, 
{
	BM_88_BALROG,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Mega\\Mega%c.CL2",	1,	"Monsters\\Mega\\Mega%c%i.WAV",		1,
	1,	"X\\Balrog.TRN",	// TRNs:   flag and path
	6, 7, 14, 1, 24, 5,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Balrog",			// name
	26,	30,	46,		// DLVL range & MLVL
	800,	900, // HP range,
	AI_26_Balrog,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	14,	8,	48,	60,// tohit, hit frame of 1st attack, dmg range.
	16,	3,	50,	60,	 // 2nd attack: tohit, hit frame, dmg range
	51, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	1070,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_89_AZURE_DRAKE,	1, 100,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Snake\\Snake%c.CL2",	1,	"Monsters\\Snake\\Snake%c%i.WAV",		1,
	1,	"X\\AD.trn",	// TRNs:   flag and path
	12, 11, 13, 6, 18, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Azure Drake",			// name
	26,	34,	49,		// DLVL range & MLVL
	750,	900, // HP range,
	AI_24_Viper,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	11,	8,	43,	58,// tohit, hit frame of 1st attack, dmg range.
	36,	9,	48,	63,	 // 2nd attack: tohit, hit frame, dmg range
	55, 2, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN|MR_11_IM_COLD,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	1000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_90_CAVE_SNAKE,	1, 100,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Snake\\Snake%c.CL2",	1,	"Monsters\\Snake\\Snake%c%i.WAV",		1,
	1,	"Monsters\\Snake\\Snakg.TRN",	// TRNs:   flag and path
	12, 11, 13, 6, 18, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Cave Snake",			// name
	16,	22,	31,		// DLVL range & MLVL
	375,	495, // HP range,
	AI_24_Viper,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	16,	8,	17,	48,// tohit, hit frame of 1st attack, dmg range.
	36,	9,	30,	57,	 // 2nd attack: tohit, hit frame, dmg range
	36, 2, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	0,		7,	697,		// special flag for drop, animation size, XP
	"X\\Snake_12_iceage_icy_caves_v2.trn", "Ice Snake" // ice age  TRN & name
}, 
{
	BM_91_GOLD_SERPENT,	1, 100,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Snake\\Snake%c.CL2",	1,	"Monsters\\Snake\\Snake%c%i.WAV",		1,
	1,	"X\\Sp.trn",	// TRNs:   flag and path
	12, 11, 13, 6, 18, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Gold Serpent",			// name
	20,	26,	37,		// DLVL range & MLVL
	495,	660, // HP range,
	AI_24_Viper,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	17,	8,	33,	53,// tohit, hit frame of 1st attack, dmg range.
	36,	9,	38,	60,	 // 2nd attack: tohit, hit frame, dmg range
	42, 2, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	670,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_92_ILLUSION_WEAVER,	1, 63,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Sneak\\Sneak%c.CL2",	1,	"Monsters\\Sneak\\Sneak%c%i.WAV",		0,
	1,	"X\\Hidden22-weaver.trn",	// TRNs:   flag and path
	16, 8, 12, 7, 24, 15,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Illusion Weaver",			// name
	42,	46,	57,		// DLVL range & MLVL
	800,	950, // HP range,
	AI_15_Hidden,	MF_1_INVISIBLE | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	31,	7,	41,	57,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	79, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	840,		// special flag for drop, animation size, XP
	"X\\Hidden27_can_blu_B_as_cool.trn", "Cold Weaver" // ice age  TRN & name
},
{
	BM_93_FLAYED_ONE,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Fat\\Fat%c.CL2",	1,	"Monsters\\Fat\\Fat%c%i.WAV",		1,
	1,	"X\\Wrm2.trn",	// TRNs:   flag and path
	8, 10, 15, 6, 16, 10,		// IWAHDS frames
	4, 0, 0, 0, 0, 0,		// delays
	"Flayed One",			// name
	18,	24,	34,		// DLVL range & MLVL
	550,	700, // HP range,
	AI_1_Overlord,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	17,	7,	33,	51,// tohit, hit frame of 1st attack, dmg range.
	32,	5,	49,	59,	 // 2nd attack: tohit, hit frame, dmg range
	39, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	808,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_94_LURKER,	1, 63,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Sneak\\Sneak%c.CL2",	1,	"Monsters\\Sneak\\Sneak%c%i.WAV",		0,
	1,	"X\\X01.trn",	// TRNs:   flag and path
	16, 8, 12, 8, 24, 15,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Lurker",			// name
	2,	8,	7,		// DLVL range & MLVL
	30,	65, // HP range,
	AI_15_Hidden,	MF_1_INVISIBLE | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	11,	8,	6,	15,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	9, 1, MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	150,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_95_GHOST_WARRIOR,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Black\\Black%c.CL2",	1,	"Monsters\\Black\\Black%c%i.WAV",		1,
	1,	"X\\hgd.TRN",	// TRNs:   flag and path
	8, 8, 16, 4, 24, 4,		// IWAHDS frames
	3, 0, 0, 0, 0, 3,		// delays
	"Ghost Warrior",			// name
	22,	26,	41,		// DLVL range & MLVL
	650,	800, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	15,	8,	50,	55,// tohit, hit frame of 1st attack, dmg range.
	24,	1,	48,	54,	 // 2nd attack: tohit, hit frame, dmg range
	46, 0, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_11_IM_COLD,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	700,		// special flag for drop, animation size, XP
	0, "Black Knight" // ice age  TRN & name
}, 
{
	BM_96_CURSE_BEARER,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Black\\Black%c.CL2",	1,	"Monsters\\Black\\Black%c%i.WAV",		1,
	1,	"X\\K2.trn",	// TRNs:   flag and path
	8, 8, 16, 4, 24, 4,		// IWAHDS frames
	3, 0, 0, 0, 0, 3,		// delays
	"Curse Bearer",			// name
	30,	30,	50,		// DLVL range & MLVL
	900,	1000, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	15,	8,	53,	65,// tohit, hit frame of 1st attack, dmg range.
	0,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	58, 0, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1104,		// special flag for drop, animation size, XP
	0, "Doom Guard" // ice age  TRN & name
},
{
	BM_97_DARK_CHARGER,	1, 0,	188,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Fork\\Fork%c.CL2",	1,	"Monsters\\Fork\\DBoar%c%i.wav",		1,
	1,	"X\\Boar8.trn",	// TRNs:   flag and path
	10, 10, 15, 6, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 2, 2,		// delays
	"Dark Charger",			// name
	18,	20,	32,		// DLVL range & MLVL
	460,	500, // HP range,
	AI_5_Charger,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	16,	7,	25,	50,// tohit, hit frame of 1st attack, dmg range.
	16,	0,	52,	58,	 // 2nd attack: tohit, hit frame, dmg range
	37, 2, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	500,		// special flag for drop, animation size, XP
	0, "White Mane" // ice age  TRN & name
}, 
{
	BM_98_IMMORTAL,	1, 97,	96,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Lich\\Lich%c.CL2",	0,	"Monsters\\newsfx\\Lich%c%i.WAV",		1,
	1,	"X\\TM.trn",	// TRNs:   flag and path
	12, 10, 10, 7, 21, 0,		// IWAHDS frames
	2, 0, 0, 0, 2, 0,		// delays
	"Immortal",			// name
	40,	42,	51,		// DLVL range & MLVL
	600,	750, // HP range,
	AI_35_Lich,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	36,	5,	50,	64,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	69, 0, MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	725,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_99_CLEAVER_BOSS,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FatC\\FatC%c.CL2",	0,	"Monsters\\FatC\\FatC%c%i.WAV",		0,
	1,	"X\\OL.trn",	// TRNs:   flag and path
	10, 8, 12, 6, 16, 0,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Grim Cleaver",			// name
	12,	12,	22,		// DLVL range & MLVL
	800,	800, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	23,	8,	29,	43,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	26, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	1500,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_100_TORTURED_SPIRIT,	1, 99,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Unrav\\Unrav%c.CL2",	1,	"Monsters\\newsfx\\Shred%c%i.WAV",		1,
	1,	"X\\To.trn",	// TRNs:   flag and path
	10, 10, 12, 4, 16, 13,		// IWAHDS frames
	2, 1, 0, 1, 0, 0,		// delays
	"Tortured Spirit",			// name
	10,	22,	22,		// DLVL range & MLVL
	170,	370, // HP range,
	AI_42_Mixed_Firebolt,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	17,	7,	16,	32,// tohit, hit frame of 1st attack, dmg range.
	16,	7,	18,	32,	 // 2nd attack: tohit, hit frame, dmg range
	25, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	320,		// special flag for drop, animation size, XP
	"X\\Unrav_002_white_exp.trn", "Snow Wight" // ice age  TRN & name
},
{
	BM_101_DARK_MISTRESS,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Succ\\Scbs%c.CL2",	0,	"Monsters\\Succ\\Scbs%c%i.WAV",		0,
	0,	"Monsters\\Acid\\AcidR.TRN",	// TRNs:   flag and path
	14, 8, 16, 7, 24, 0,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Dark Mistress",			// name
	10,	12,	21,		// DLVL range & MLVL
	100,	160, // HP range,
	AI_14_Witch,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	15,	10,	12,	30,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	22, 1, MR_1_ARCAN|MR_3_LIGHT|MR_5_IM_FIRE,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	310,		// special flag for drop, animation size, XP
	"x\\Sucub_09_snow_witch_iceage.trn", "Snow Witch" // ice age  TRN & name
},
{
	BM_102_SUCCUBUS,	1, 115,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Succ\\Scbs%c.CL2",	0,	"Monsters\\Succ\\Scbs%c%i.WAV",		0,
	1,	"X\\RO.trn",	// TRNs:   flag and path
	14, 8, 16, 7, 24, 0,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Succubus",			// name
	12,	14,	23,		// DLVL range & MLVL
	125,	200, // HP range,
	AI_14_Witch,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	17,	10,	15,	34,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	25, 1, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	350,		// special flag for drop, animation size, XP
	"x\\Sucub_10_BB.trn", "Azure Witch" // ice age  TRN & name
},
{
	BM_103_BLOOD_WITCH,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Succ\\Scbs%c.CL2",	0,	"Monsters\\Succ\\Scbs%c%i.WAV",		0,
	1,	"X\\W1.trn",	// TRNs:   flag and path
	14, 8, 16, 7, 24, 0,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Blood Witch",			// name
	28,	30,	49,		// DLVL range & MLVL
	300,	600, // HP range,
	AI_14_Witch,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	16,	10,	30,	52,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	54, 1, MR_1_ARCAN|MR_3_LIGHT|MR_5_IM_FIRE,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	678,		// special flag for drop, animation size, XP
	0, "Red Witch" // ice age  TRN & name
}, 
{
	BM_104_DARK_WITCH,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Succ\\Scbs%c.CL2",	1,	"Monsters\\Succ\\Scbs%c%i.WAV",		0,
	1,	"X\\M1.trn",	// TRNs:   flag and path
	14, 8, 16, 7, 24, 1,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Dark Witch",			// name
	42,	44,	52,		// DLVL range & MLVL
	600,	775, // HP range,
	AI_14_Witch,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	26,	9,	50,	66,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	75, 1, MR_1_ARCAN|MR_3_LIGHT|MR_5_IM_FIRE,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	890,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_105_IMP_ROGUE,	0, 60,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FalSpear\\Phall%c.CL2",	1,	"Monsters\\BigFall\\bfal%c%i.WAV",		1,
	1,	"X\\S3.TRN",	// TRNs:   flag and path
	11, 11, 13, 11, 18, 13,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Imp Rogue",			// name
	20,	24,	33,		// DLVL range & MLVL
	225,	315, // HP range,
	AI_32_Ranger_Firebolt/*800*/,	MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	19,	6,	20,	44,// tohit, hit frame of 1st attack, dmg range.
	19,	5,	35,	45,	 // 2nd attack: tohit, hit frame, dmg range
	36, 1, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	550,		// special flag for drop, animation size, XP
	"X\\Fallen08_iceage_improg_cave.trn", 0 // ice age  TRN & name
},
{
	BM_106_CORRUPT_ORDER,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Order\\order%c.cl2",	0,	"Monsters\\Order\\order%c%i.wav",		0,
	1,	"X\\FR.trn",	// TRNs:   flag and path
	8, 8, 12, 7, 20, 1,		// IWAHDS frames
	2, 0, 1, 1, 2, 0,		// delays
	"Corrupt Order",			// name
	22,	30,	43,		// DLVL range & MLVL
	300,	375, // HP range,
	AI_7_Goat_Archer,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	16,	8,	49,	63,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	50, 2, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	700,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_107_DARK_MAGE,	1, 0,	128,	1808,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\DarkMage\\Dmage%c.CL2",	1,	"Monsters\\DarkMage\\Dmag%c%i.WAV",		0,
	0,	"Monsters\\GoatMace\\Gray.TRN",	// TRNs:   flag and path
	6, 1, 20, 6, 23, 18,		// IWAHDS frames
	3, 0, 0, 0, 1, 0,		// delays
	"Dark Mage",			// name
	42,	44,	53,		// DLVL range & MLVL
	450,	700, // HP range,
	AI_25_Advocate,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	36,	8,	48,	64,// tohit, hit frame of 1st attack, dmg range.
	28,	0,	60,	60,	 // 2nd attack: tohit, hit frame, dmg range
	82, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	985,		// special flag for drop, animation size, XP
	0, "Black Mage" // ice age  TRN & name
},
{
	BM_108_MAGISTRATE,	1, 0,	128,	1808,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Mage\\Mage%c.cl2",	1,	"Monsters\\Mage\\Mage%c%i.wav",		1,
	0,	"X\\M1.trn",	// TRNs:   flag and path
	12, 1, 20, 8, 28, 20,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Magistrate",			// name
	42,	44,	55,		// DLVL range & MLVL
	465,	690, // HP range,
	AI_25_Advocate,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	35,	9,	58,	67,// tohit, hit frame of 1st attack, dmg range.
	35,	0,	60,	60,	 // 2nd attack: tohit, hit frame, dmg range
	80, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	0,		7,	975,		// special flag for drop, animation size, XP
	"x\\Mage_01_crypta.trn", 0 // ice age  TRN & name
},
{
	BM_109_GOLEM,	0, 0,	96,	386,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Golem\\Golem%c.CL2",	1,	"Monsters\\Golem\\Golm%c%i.WAV",		0,
	1,	"X\\Golem_02_Redd.TRN",	// TRNs:   flag and path
	0, 16, 12, 0, 12, 20,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Golem",			// name
	0,	0,	40,		// DLVL range & MLVL
	1,	1, // HP range,
	AI_21_Golem,	MF_8_KNOCKBACK | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	0,	7,	1,	1,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	1, 1, MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT,	MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	0,		// special flag for drop, animation size, XP
	"X\\Golem_03_Blu.trn", 0 // ice age  TRN & name
},
{
	BM_110_DIABLO,	0, 140,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Diablo\\Diablo%c.CL2",	1,	"Monsters\\Diablo\\Diablo%c%i.WAV",		1,
	1,	"X\\RO.trn",	// TRNs:   flag and path
	16, 6, 16, 6, 18, 16,		// IWAHDS frames
	1, 0, 0, 0, 2, 0,		// delays
	"Greater Demon",			// name
	30,	30,	58,		// DLVL range & MLVL
	9440,	9440, // HP range,
	AI_27_Diablo,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	40,	12,	55,	60,// tohit, hit frame of 1st attack, dmg range.
	0,	11,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	63, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	10000,		// special flag for drop, animation size, XP
	"X\\Diablo6_blue.trn", 0 // ice age  TRN & name
},
{
	BM_111_OBSIDIAN_GOLEM,	1, 0,	154,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Bubba\\Bubba%c.CL2",	0,	"X\\GG%c%i.wav",		1,
	1,	"X\\Ys.trn",	// TRNs:   flag and path
	8, 18, 12, 1, 21, 0,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Obsidian Golem",			// name
	18,	24,	35,		// DLVL range & MLVL
	750,	1000, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	17,	10,	36,	48,// tohit, hit frame of 1st attack, dmg range.
	15,	0,	35,	55,	 // 2nd attack: tohit, hit frame, dmg range
	41, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	600,		// special flag for drop, animation size, XP
	"x\\Bubba_03_stoneGolem_iceAge_fixt_v2.trn", "Stone Golem" // ice age  TRN & name
},
{
	BM_112_WAILING_BEAST,	1, 0,	188,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Fork\\Fork%c.CL2",	1,	"Monsters\\newsfx\\HBoar%c%i.WAV",		1,
	1,	"X\\caveboar.TRN",	// TRNs:   flag and path
	10, 10, 15, 6, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 2, 2,		// delays
	"Wailing Beast",			// name
	14,	18,	28,		// DLVL range & MLVL
	375,	445, // HP range,
	AI_5_Charger,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	16,	7,	20,	43,// tohit, hit frame of 1st attack, dmg range.
	37,	1,	50,	55,	 // 2nd attack: tohit, hit frame, dmg range
	32, 2, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN | MR_11_IM_COLD,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	496,		// special flag for drop, animation size, XP
	"x\\Fork_01_purple.trn", "Cave Charger" // ice age  TRN & name
},
{
	BM_113_ASH_GOLEM,	1, 0,	96,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Muddy\\Muddy%c.cl2",	1,	"Monsters\\Muddy\\Muddy%c%i.wav",		1,
	1,	"Monsters\\Muddy\\Muddy.trn",	// TRNs:   flag and path
	3, 16, 12, 3, 12, 20,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Ash Golem",			// name
	22,	26,	38,		// DLVL range & MLVL
	450,	600, // HP range,
	AI_15_Hidden,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	19,	7,	38,	50,// tohit, hit frame of 1st attack, dmg range.
	0,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	43, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE | MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	650,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_114_FROZEN_HORROR,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelBow\\SklBw%c.CL2",	1,	"Monsters\\newsfx\\psyco%c%i.WAV",		0,
	1,	"Monsters\\FalSword\\Blue.TRN",	// TRNs:   flag and path
	9, 8, 16, 5, 16, 16,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Frozen Horror",			// name
	40,	42,	53,		// DLVL range & MLVL
	490,	660, // HP range,
	AI_44_Ranger_Cold_Arrow,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	31,	11,	40,	52,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	70, 0, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	700,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_115_GRUNT_BEAST,	1, 0,	180,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Byclps\\Byclps%c.CL2",	1,	"Monsters\\newsfx\\Biclop%c%i.WAV",		1,
	1,	"X\\G1.trn",	// TRNs:   flag and path
	10, 11, 16, 6, 16, 16,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Grunt Beast",			// name
	14,	18,	28,		// DLVL range & MLVL
	390,	480, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	16,	9,	28,	45,// tohit, hit frame of 1st attack, dmg range.
	0,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	32, 2, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE|MR_11_IM_COLD,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	596,		// special flag for drop, animation size, XP
	"X\\Biclops03_better_blue_eyes.trn", "Snow Brute" // ice age  TRN & name
},
{
	BM_116_IMP_BRUTE,	1, 77,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\BigFall\\Fallg%c.CL2",	1,	"Monsters\\newsfx\\KBrute%c%i.WAV",		1,
	1,	"X\\M1.trn",	// TRNs:   flag and path
	10, 8, 11, 8, 17, 5,		// IWAHDS frames
	0, 0, 0, 0, 2, 2,		// delays
	"Imp Brute",			// name
	20,	24,	37,		// DLVL range & MLVL
	510,	675, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	16,	6,	28,	52,// tohit, hit frame of 1st attack, dmg range.
	0,	3,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	43, 1, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN|MR_11_IM_COLD,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	600,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_117_STINGER,	1, 46,	64,	438,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Scorp\\Scorp%c.CL2",	1,	"Monsters\\newsfx\\Stingr%c%i.WAV",		0,
	1,	"Monsters\\GoatMace\\Gray.TRN",	// TRNs:   flag and path
	10, 4, 12, 6, 15, 8,		// IWAHDS frames
	2, 0, 0, 0, 0, 1,		// delays
	"Stinger",			// name
	32,	38,	49,		// DLVL range & MLVL
	190,	330, // HP range,
	AI_11_Bat,	0, 9,	// AI, int flag, int level
	17,	7,	9,	27,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	59, 2, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	16384,		1,	190,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_118_DRAGON_KIN,	1, 100,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Snake\\Snake%c.CL2",	1,	"Monsters\\Snake\\Snake%c%i.WAV",		1,
	1,	"X\\Sa.trn",	// TRNs:   flag and path
	12, 11, 13, 5, 18, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Dragon Kin",			// name
	44,	46,	54,		// DLVL range & MLVL
	1150,	1300, // HP range,
	AI_24_Viper,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 10,	// AI, int flag, int level
	41,	7,	50,	65,// tohit, hit frame of 1st attack, dmg range.
	62,	9,	62,	66,	 // 2nd attack: tohit, hit frame, dmg range
	84, 2, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	1750,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_119_DARK_GHOST,	1, 99,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Unrav\\Unrav%c.CL2",	0,	"Monsters\\newsfx\\Shred%c%i.WAV",		0,
	1,	"X\\DG.TRN",	// TRNs:   flag and path
	10, 10, 12, 5, 16, 0,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Dark Ghost",			// name
	4,	8,	7,		// DLVL range & MLVL
	30,	60, // HP range,
	AI_38_Beholder,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	16,	7,	4,	12,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	6, 0, MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	180,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_120_NECROMORPH,	1, 115,	164,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Flesh\\Flesh%c.CL2",	1,	"monsters\\flesh\\Necro2%c%i.wav",		1,
	1,	"X\\FleshG.trn",	// TRNs:   flag and path
	15, 24, 15, 6, 16, 1,		// IWAHDS frames
	2, 0, 0, 0, 2, 0,		// delays
	"Necromorph",			// name
	40,	46,	55,		// DLVL range & MLVL
	2700,	3000, // HP range,
	AI_8_Fallen_One,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	39,	12,	60,	65,// tohit, hit frame of 1st attack, dmg range.
	0,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	72, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	1500,		// special flag for drop, animation size, XP
	"x\\Flesh_02_blu.trn", "Adipocire Warrior" // ice age  TRN & name
},
{
	BM_121_SPECTRE,	1, 99,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Unrav\\Unrav%c.CL2",	1,	"Monsters\\newsfx\\Shred%c%i.WAV",		0,
	1,	"Monsters\\Mega\\Balr.TRN",	// TRNs:   flag and path
	10, 10, 12, 5, 16, 13,		// IWAHDS frames
	2, 1, 0, 1, 0, 0,		// delays
	"Spectre",			// name
	2,	4,	7,		// DLVL range & MLVL
	36,	54, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	11,	7,	3,	15,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	6, 0, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	130,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_122_BLOOD_SPECTRE,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Black\\Black%c.CL2",	1,	"Monsters\\Black\\Black%c%i.WAV",		1,
	1,	"X\\Ghostwar.TRN",	// TRNs:   flag and path
	8, 8, 16, 4, 24, 4,		// IWAHDS frames
	3, 0, 0, 0, 0, 3,		// delays
	"Blood Spectre",			// name
	44,	46,	57,		// DLVL range & MLVL
	1400,	1650, // HP range,
	AI_13_Butcher,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	34,	8,	47,	59,// tohit, hit frame of 1st attack, dmg range.
	35,	1,	40,	50,	 // 2nd attack: tohit, hit frame, dmg range
	83, 0, MR_4_IM_ARCAN |MR_3_LIGHT|MR_5_IM_FIRE|MR_11_IM_COLD,	MR_4_IM_ARCAN |MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1880,		// special flag for drop, animation size, XP
	"X\\Knight_08_iceage_crimson.trn", "Frost Knight" // ice age  TRN & name
},
{
	BM_123_BICLOPS,	1, 0,	180,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Byclps\\Byclps%c.CL2",	1,	"Monsters\\newsfx\\Biclop%c%i.WAV",		1,
	1,	"X\\Biclops02_darkorangemixedwithcrimson.TRN",	// TRNs:   flag and path
	10, 11, 16, 6, 16, 16,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Biclops",			// name
	18,	22,	34,		// DLVL range & MLVL
	480,	645, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	18,	9,	31,	53,// tohit, hit frame of 1st attack, dmg range.
	19,	1,	58,	58,	 // 2nd attack: tohit, hit frame, dmg range
	39, 2, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE | MR_8_IM_ACID | MR_11_IM_COLD,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE |MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	793,		// special flag for drop, animation size, XP
	"x\\Biclops05_black.trn", "Two-headed Beast" // ice age  TRN & name
},
{
	BM_124_GLOOM_CLAN,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatBow\\GoatB%c.CL2",	0,	"Monsters\\GoatBow\\GoatB%c%i.WAV",		0,
	1,	"X\\Goat17.TRN",	// TRNs:   flag and path
	12, 8, 16, 6, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Gloom Clan",			// name
	42,	46,	58,		// DLVL range & MLVL
	480,	780, // HP range,
	AI_45_Ranger_Arcane_Arrow,	MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	28,	12,	44,	56,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	78, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	990,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_125_GOLD_GOLEM,	1, 0,	154,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Bubba\\Bubba%c.CL2",	1,	"X\\GG%c%i.wav",		1,
	1,	"X\\GoldenGolem2.trn",	// TRNs:   flag and path
	8, 18, 12, 1, 21, 8,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Gold Golem",			// name
	40,	46,	56,		// DLVL range & MLVL
	2000,	2500, // HP range,
	AI_13_Butcher,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	38,	10,	60,	64,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	78, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	1453,		// special flag for drop, animation size, XP
	"X\\Bubba_02_silver.trn", "Silver Golem" // ice age  TRN & name
},
{
	BM_126_PLAGUE_EATER,	1, 68,	104,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Rat\\Rat%c.CL2",	1,	"Monsters\\newsfx\\TmbRat%c%i.WAV",		1,
	1,	"X\\Ghostwar.TRN",	// TRNs:   flag and path
	11, 8, 12, 6, 20, 11,		// IWAHDS frames
	2, 0, 0, 0, 2, 0,		// delays
	"Plague Eater",			// name
	8,	14,	20,		// DLVL range & MLVL
	210,	290, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	21,	8,	12,	30,// tohit, hit frame of 1st attack, dmg range.
	30,	7,	25,	40,	 // 2nd attack: tohit, hit frame, dmg range
	23, 2, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	275,		// special flag for drop, animation size, XP
	"X\\Rat004_blue_better.trn", "Plague Rat" // ice age  TRN & name
},
{
	BM_127_WYRM,	1, 0,	192,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\AntWorm\\Worm%c.CL2",	1,	"Monsters\\newsfx\\TchAnt%c%i.WAV",		1,
	1,	"X\\Wrm.trn",	// TRNs:   flag and path
	14, 12, 12, 6, 20, 12,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Wyrm",			// name
	16,	20,	30,		// DLVL range & MLVL
	420,	510, // HP range,
	AI_19_Acid_Beast,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	18,	10,	23,	49,// tohit, hit frame of 1st attack, dmg range.
	19,	9,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	31, 2, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_8_IM_ACID,	MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_8_IM_ACID,// armor, mon type, and resistances
	0,		7,	530,		// special flag for drop, animation size, XP
	"x\\Wyrm_05_black_wyrm.trn", "Acid Wyrm" // ice age  TRN & name
},
{
	BM_128_GREATER_IMP,	1, 77,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\BigFall\\Fallg%c.CL2",	1,	"monsters\\bigfall\\gi%c%i.wav",		1,
	1,	"X\\M3.trn",	// TRNs:   flag and path
	10, 8, 11, 8, 17, 5,		// IWAHDS frames
	0, 0, 0, 0, 2, 2,		// delays
	"Greater Imp",			// name
	18,	22,	34,		// DLVL range & MLVL
	450,	570, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	16,	6,	28,	48,// tohit, hit frame of 1st attack, dmg range.
	0,	3,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	39, 1, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	500,		// special flag for drop, animation size, XP
	"X\\Fallen08_iceage_improg_cave.trn", 0 // ice age  TRN & name
},
{
	BM_129_LICH,	1, 97,	96,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Lich\\Lich%c.CL2",	1,	"Monsters\\newsfx\\Lich%c%i.WAV",		1,
	1,	"Monsters\\Thin\\Thinv1.TRN",	// TRNs:   flag and path
	12, 10, 10, 7, 21, 19,		// IWAHDS frames
	2, 0, 0, 0, 2, 0,		// delays
	"Lich",			// name
	6,	8,	13,		// DLVL range & MLVL
	80,	100, // HP range,
	AI_35_Lich,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	12,	7,	8,	19,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	11, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	300,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_130_BLACK_GOLEM,	1, 0,	154,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Bubba\\Bubba%c.CL2",	1,	"X\\GG%c%i.wav",		1,
	1,	"X\\17.trn",	// TRNs:   flag and path
	8, 18, 12, 1, 21, 8,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Black Golem",			// name
	26,	30,	47,		// DLVL range & MLVL
	1000,	1200, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	16,	10,	40,	55,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	54, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	730,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_131_FIRE_BAT,	1, 0,	96,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Hellbat2\\bhelbt%c.CL2",	1,	"Monsters\\newsfx\\HelBat%c%i.WAV",		0,
	0,	"X\\TE.trn",	// TRNs:   flag and path
	16, 16, 14, 6, 18, 14,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Fire Bat",			// name
	16,	18,	25,		// DLVL range & MLVL
	165,	225, // HP range,
	AI_32_Ranger_Firebolt,	MF_9_CHASE_TARGET, 1,	// AI, int flag, int level
	21,	8,	19,	32,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	29, 2, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT,	MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	16384,		7,	360,		// special flag for drop, animation size, XP
	0, "Snow Bat" // ice age  TRN & name
},
{
	BM_132_BONEWING,	1, 103,	128,	1808,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Demskel\\Demskl%c.CL2",	1,	"Monsters\\newsfx\\SWing%c%i.WAV",		1,
	1,	"X\\Skeleton007-black-n-white.TRN",	// TRNs:   flag and path
	10, 8, 20, 6, 24, 16,		// IWAHDS frames
	3, 0, 0, 0, 1, 0,		// delays
	"Bonewing",			// name
	6,	8,	12,		// DLVL range & MLVL
	165,	195, // HP range,
	AI_40_Bonewing_cold_blasts,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	14,	7,	9,	24,// tohit, hit frame of 1st attack, dmg range.
	16,	12,	12,	28,	 // 2nd attack: tohit, hit frame, dmg range
	14, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	380,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_133_SOUL_BURNER,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Succ\\Scbs%c.CL2",	1,	"Monsters\\Succ\\Scbs%c%i.WAV",		0,
	1,	"X\\Bitch.TRN",	// TRNs:   flag and path
	14, 8, 16, 7, 24, 1,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Soul Burner",			// name
	44,	46,	57,		// DLVL range & MLVL
	600,	975, // HP range,
	AI_36_Arch_Lich,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	40,	9,	36,	60,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	80, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	920,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_134_MUD_RUNNER,	1, 0,	188,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Fork\\Fork%c.CL2",	1,	"Monsters\\Fork\\VBoar%c%i.wav",		1,
	1,	"X\\Boar5.trn",	// TRNs:   flag and path
	10, 10, 15, 6, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 2, 2,		// delays
	"Mud Runner",			// name
	20,	22,	36,		// DLVL range & MLVL
	480,	660, // HP range,
	AI_5_Charger,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	17,	7,	30,	55,// tohit, hit frame of 1st attack, dmg range.
	47,	1,	55,	60,	 // 2nd attack: tohit, hit frame, dmg range
	41, 2, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN | MR_8_IM_ACID,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE | MR_8_IM_ACID,// armor, mon type, and resistances
	0,		3,	675,		// special flag for drop, animation size, XP
	0, "Snow Runner" // ice age  TRN & name
},
{
	BM_135_GROTESQUE,	1, 115,	164,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Flesh\\Flesh%c.CL2",	1,	"Monsters\\newsfx\\FleshT%c%i.WAV",		1,
	1,	"X\\Grotesqu.trn",	// TRNs:   flag and path
	15, 24, 15, 6, 16, 15,		// IWAHDS frames
	2, 0, 0, 0, 2, 1,		// delays
	"Grotesque",			// name
	32,	38,	55,		// DLVL range & MLVL
	2050,	2500, // HP range,
	AI_2_Skel_Melee,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	20,	13,	52,	64,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	60, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	1103,		// special flag for drop, animation size, XP
	0, "Plague Spreader" // ice age  TRN & name
}, 
{
	BM_136_BLOOD_SLAYER,	1, 0,	180,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Reaper\\Reap%c.CL2",	0,	"Monsters\\newsfx\\Reaper%c%i.WAV",		1,
	1,	"X\\Ka.trn",	// TRNs:   flag and path
	12, 10, 14, 4, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 2, 3,		// delays
	"Blood Slayer",			// name
	22,	30,	40,		// DLVL range & MLVL
	650,	800, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	17,	7,	35,	55,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	46, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_11_IM_COLD,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	962,		// special flag for drop, animation size, XP
	"x\\Reaper_16_pale_blue.trn", "Crystalline Scythe" // ice age  TRN & name
},
{
	BM_137_UBER_DIABLO,	0, 140,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Diablo\\Diablo%c.CL2",	1,	"Monsters\\Uber\\uber%c%i.wav",		1,
	1,	"X\\Uberlord.TRN",	// TRNs:   flag and path
	16, 6, 16, 1, 18, 16,		// IWAHDS frames
	1, 0, 0, 0, 2, 0,		// delays
	"Uber Diablo",			// name
	46,	46,	62,		// DLVL range & MLVL
	10000,	10000, // HP range,
	AI_27_Diablo,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 6,	// AI, int flag, int level
	70,	3,	62,	62,// tohit, hit frame of 1st attack, dmg range.
	0,	10,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	85, 1, MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	15000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_138_SHADOW_BEAST,	1, 47,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Scav\\Scav%c.CL2",	1,	"Monsters\\Scav\\Scav%c%i.WAV",		0,
	1,	"X\\GremAgla.TRN",	// TRNs:   flag and path
	12, 8, 12, 8, 20, 11,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Shadow Beast",			// name
	0,	2,	3,		// DLVL range & MLVL
	18,	24, // HP range,
	AI_4_Scavenger,	0, 0,	// AI, int flag, int level
	9,	8,	1,	8,// tohit, hit frame of 1st attack, dmg range.
	10,	0,	10,	10,	 // 2nd attack: tohit, hit frame, dmg range
	6, 2, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	16384,		3,	75,		// special flag for drop, animation size, XP
	"CF\\ICEAGE\\shbt.trn", "Snow Scavenger" // ice age  TRN & name
},
{
	BM_139_BONE_GASHER,	1, 48,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Scav\\Scav%c.CL2",	1,	"Monsters\\Scav\\Scav%c%i.WAV",		0,
	1,	"X\\Fallen01.TRN",	// TRNs:   flag and path
	12, 8, 12, 7, 20, 11,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Bone Gasher",			// name
	4,	6,	9,		// DLVL range & MLVL
	40,	80, // HP range,
	AI_4_Scavenger,	MF_9_CHASE_TARGET, 1,	// AI, int flag, int level
	15,	7,	6,	16,// tohit, hit frame of 1st attack, dmg range.
	16,	0,	20,	20,	 // 2nd attack: tohit, hit frame, dmg range
	14, 2, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	16384,		3,	165,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_140_GHOUL,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Zombie\\Zombie%c.CL2",	1,	"Monsters\\Zombie\\Zombie%c%i.WAV",		1,
	1,	"X\\Zombie04_red.TRN",	// TRNs:   flag and path
	11, 24, 12, 6, 16, 11,		// IWAHDS frames
	4, 0, 0, 0, 0, 4,		// delays
	"Ghoul",			// name
	8,	12,	17,		// DLVL range & MLVL
	260,	340, // HP range,
	AI_0_Zombie,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	25,	7,	20,	28,// tohit, hit frame of 1st attack, dmg range.
	0,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	19, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	320,		// special flag for drop, animation size, XP
	"monsters\\zombie\\grey.trn", "Hungry Dead" // ice age  TRN & name
},
{
	BM_141_SHAMBLER,	1, 115,	164,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Flesh\\Flesh%c.CL2",	1,	"monsters\\flesh\\Necro2%c%i.wav",		1,
	1,	"X\\Grot02x1.TRN",	// TRNs:   flag and path
	15, 24, 15, 6, 16, 1,		// IWAHDS frames
	2, 0, 0, 0, 2, 0,		// delays
	"Shambler",			// name
	14,	22,	32,		// DLVL range & MLVL
	850,	1050, // HP range,
	AI_8_Fallen_One,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	15,	12,	30,	40,// tohit, hit frame of 1st attack, dmg range.
	0,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	33, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	805,		// special flag for drop, animation size, XP
	"x\\Flesh_03-greyish.trn", "Brute" // ice age  TRN & name
},
{
	BM_142_HELL_MEAT,	1, 115,	164,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Flesh\\Flesh%c.CL2",	1,	"monsters\\flesh\\Necro2%c%i.wav",		1,
	1,	"X\\Grot04.TRN",	// TRNs:   flag and path
	15, 24, 15, 6, 16, 1,		// IWAHDS frames
	2, 0, 0, 0, 2, 0,		// delays
	"Hell Meat",			// name
	24,	30,	45,		// DLVL range & MLVL
	1300,	1700, // HP range,
	AI_8_Fallen_One,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	15,	12,	40,	52,// tohit, hit frame of 1st attack, dmg range.
	0,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	49, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	640,		// special flag for drop, animation size, XP
	0, "Harvester" // ice age  TRN & name
}, 
{
	BM_143_DOOM_TAIL,	0, 46,	64,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Scorp\\Scorp%c.CL2",	1,	"Monsters\\newsfx\\Stingr%c%i.WAV",		0,
	1,	"X\\Doomtail3.TRN",	// TRNs:   flag and path
	10, 4, 12, 6, 15, 8,		// IWAHDS frames
	2, 0, 0, 0, 0, 1,		// delays
	"Doom Tail",			// name
	42,	46,	53,		// DLVL range & MLVL
	320,	400, // HP range,
	AI_11_Bat,	0, 9,	// AI, int flag, int level
	25,	7,	15,	33,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	76, 2, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	16384,		1,	440,		// special flag for drop, animation size, XP
	"x\\Scorp_02_blue.trn", 0 // ice age  TRN & name
}, 
{
	BM_144_DARK_SERVANT,	1, 0,	124,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Gravdg\\Gravdg%c.CL2",	1,	"Monsters\\newsfx\\diga%c%i.WAV",		1,
	1,	"X\\Gravdg01.TRN",	// TRNs:   flag and path
	24, 24, 12, 6, 16, 16,		// IWAHDS frames
	3, 0, 0, 2, 2, 0,		// delays
	"Dark Servant",			// name
	6,	10,	15,		// DLVL range & MLVL
	170,	250, // HP range,
	AI_4_Scavenger,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	16,	5,	16,	26,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	18, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	255,		// special flag for drop, animation size, XP
	"X\\Gravdg05_iceage_crimsonish.trn", 0 // ice age  TRN & name
},
{
	BM_145_BLACK_MOURNER,	1, 0,	124,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Gravdg\\Gravdg%c.CL2",	1,	"Monsters\\newsfx\\diga%c%i.WAV",		1,
	1,	"X\\Gravdg02.TRN",	// TRNs:   flag and path
	24, 24, 12, 6, 16, 16,		// IWAHDS frames
	3, 0, 0, 2, 2, 0,		// delays
	"Black Mourner",			// name
	10,	14,	21,		// DLVL range & MLVL
	260,	400, // HP range,
	AI_4_Scavenger,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	18,	5,	20,	32,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	24, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	320,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_146_CORRUPT_ORDER,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Order\\order%c.cl2",	0,	"Monsters\\Order\\order%c%i.wav",		0,
	1,	"X\\FR.trn",	// TRNs:   flag and path
	8, 8, 11, 7, 20, 1,		// IWAHDS frames
	2, 0, 1, 1, 2, 0,		// delays
	"Corrupt Order",			// name
	24,	24,	43,		// DLVL range & MLVL
	300,	375, // HP range,
	AI_7_Goat_Archer,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	16,	8,	49,	63,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	50, 2, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	800,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_147_FIEND,	1, 0,	96,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Bat\\Bat%c.CL2",	1,	"Monsters\\Bat\\bat%c%i.wav",		1,
	1,	"X\\Fiend.TRN",	// TRNs:   flag and path
	9, 13, 10, 9, 13, 10,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Fiend",			// name
	0,	2,	4,		// DLVL range & MLVL
	18,	24, // HP range,
	AI_11_Bat,	MF_9_CHASE_TARGET, 0,	// AI, int flag, int level
	7,	5,	1,	8,// tohit, hit frame of 1st attack, dmg range.
	14,	2,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	4, 2, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	16384,		6,	80,		// special flag for drop, animation size, XP
	"X\\Bat_02_frost.trn", "Winter Wing" // ice age  TRN & name
}, 
{
	BM_148_TURBAK_THE_DEUCE,	0, 77,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\BigFall\\Fallg%c.CL2",	1,	"monsters\\bigfall\\gi%c%i.wav",		1,
	0,	"X\\M3.trn",	// TRNs:   flag and path
	10, 8, 10, 8, 17, 5,		// IWAHDS frames
	0, 0, 0, 0, 2, 2,		// delays
	"Turbak the Deuce",			// name
	14,	14,	33,		// DLVL range & MLVL
	250,	250, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	15,	5,	30,	40,// tohit, hit frame of 1st attack, dmg range.
	0,	3,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	35, 1, MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT,	MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	1000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_149_THRESHER,	1, 77,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\BigFall\\Fallg%c.CL2",	1,	"monsters\\bigfall\\gi%c%i.wav",		1,
	1,	"X\\Xw.trn",	// TRNs:   flag and path
	10, 8, 10, 8, 17, 5,		// IWAHDS frames
	0, 0, 0, 0, 2, 2,		// delays
	"Thresher",			// name
	24,	30,	44,		// DLVL range & MLVL
	600,	750, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	12,	5,	30,	52,// tohit, hit frame of 1st attack, dmg range.
	0,	3,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	51, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	675,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_150_FELLTWIN,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\TSneak\\TSneak%c.CL2",	1,	"Monsters\\TSneak\\sneakl%c%i.wav",		0,
	1,	"X\\warper03-crimzon.TRN",	// TRNs:   flag and path
	13, 13, 15, 11, 16, 13,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Felltwin",			// name
	16,	22,	32,		// DLVL range & MLVL
	280,	380, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	17,	9,	22,	45,// tohit, hit frame of 1st attack, dmg range.
	0,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	37, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_11_IM_COLD,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	425,		// special flag for drop, animation size, XP
	"CF\\iceage\\Hidden29_iceage_Sneakmind_boss.trn", "Warper" // ice age  TRN & name
},
{
	BM_151_HYBRID,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\TSneak\\TSneak%c.CL2",	1,	"Monsters\\TSneak\\sneakl%c%i.wav",		0,
	0,	"X\\Fireclan.TRN",	// TRNs:   flag and path
	13, 13, 15, 11, 16, 13,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Hybrid",			// name
	24,	30,	42,		// DLVL range & MLVL
	515,	665, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	21,	9,	33,	50,// tohit, hit frame of 1st attack, dmg range.
	0,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	51, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	720,		// special flag for drop, animation size, XP
	0, "Ice Blinker" // ice age  TRN & name
}, 
{
	BM_152_BLACK_REAPER,	1, 0,	180,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Reaper\\Reap%c.CL2",	1,	"Monsters\\newsfx\\Reaper%c%i.WAV",		1,
	1,	"X\\Reaper_08_Darkest.trn",	// TRNs:   flag and path
	12, 10, 14, 6, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 2, 3,		// delays
	"Black Reaper",			// name
	32,	38,	56,		// DLVL range & MLVL
	900,	1100, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	18,	7,	45,	57,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	65, 1, MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	1075,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_153_DEATH_MINION,	1, 0,	180,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Reaper\\Reap%c.CL2",	1,	"Monsters\\newsfx\\Reaper%c%i.WAV",		1,
	1,	"X\\Reaper_12.trn",	// TRNs:   flag and path
	12, 10, 14, 6, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 2, 3,		// delays
	"Death Reaper",			// name
	40,	46,	57,		// DLVL range & MLVL
	1200,	1500, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 6,	// AI, int flag, int level
	35,	7,	50,	62,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	81, 1, MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN |MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	1525,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_154_DEATH_MINION,	0, 0,	180,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Reaper\\Reap%c.CL2",	1,	"Monsters\\newsfx\\Reaper%c%i.WAV",		1,
	1,	"X\\Reaper_12.trn",	// TRNs:   flag and path
	12, 10, 14, 6, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 2, 3,		// delays
	"Death Minion",			// name
	40,	46,	58,		// DLVL range & MLVL
	1200,	1500, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 6,	// AI, int flag, int level
	45,	7,	50,	62,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	83, 1, MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	4000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_155_FALLEN_ONE,	1, 60,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FalSword\\Fall%c.CL2",	1,	"Monsters\\BigFall\\bfal%c%i.WAV",		1,
	0,	"Monsters\\FalSword\\Blue.TRN",	// TRNs:   flag and path
	12, 12, 13, 11, 14, 15,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Fallen One",			// name
	0,	6,	5,		// DLVL range & MLVL
	22,	36, // HP range,
	AI_8_Fallen_One,	0, 0,	// AI, int flag, int level
	6,	8,	3,	10,// tohit, hit frame of 1st attack, dmg range.
	0,	5,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	2, 1, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	0,		3,	115,		// special flag for drop, animation size, XP
	"X\\Fallen06_shivering1.trn", "Shivering One" // ice age  TRN & name
},
{
	BM_156_IMP_ROGUE,	1, 61,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FalSpear\\Phall%c.CL2",	1,	"Monsters\\BigFall\\bfal%c%i.WAV",		1,
	1,	"X\\S3.TRN",	// TRNs:   flag and path
	11, 11, 13, 11, 18, 13,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Imp Rogue",			// name
	20,	24,	33,		// DLVL range & MLVL
	225,	315, // HP range,
	AI_32_Ranger_Firebolt/*800*/,	MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	19,	6,	20,	44,// tohit, hit frame of 1st attack, dmg range.
	19,	5,	35,	45,	 // 2nd attack: tohit, hit frame, dmg range
	36, 1, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	550,		// special flag for drop, animation size, XP
	"X\\Fallen08_iceage_improg_cave.trn", 0 // ice age  TRN & name
},
{
	BM_157_YELLOW_ONE,	1, 60,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FalSword\\Fall%c.CL2",	1,	"Monsters\\BigFall\\bfal%c%i.WAV",		1,
	1,	"Monsters\\falsword\\orange.TRN",	// TRNs:   flag and path
	12, 12, 13, 11, 14, 15,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Yellow One",			// name
	4,	8,	11,		// DLVL range & MLVL
	60,	140, // HP range,
	AI_8_Fallen_One,	MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	16,	8,	7,	17,// tohit, hit frame of 1st attack, dmg range.
	0,	5,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	12, 1, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	175,		// special flag for drop, animation size, XP
	"X\\Fallen07_iceage_yellow1.trn", "Carver" // ice age  TRN & name
}, 
{
	BM_158_DEATH_RETINUE,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelAxe\\SklAx%c.CL2",	1,	"Monsters\\SkelAxe\\SklAx%c%i.WAV",		0,
	1,	"X\\Skeleton005-crimson.trn",	// TRNs:   flag and path
	11, 8, 12, 5, 17, 15,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Death Retinue",			// name
	40,	40,	53,		// DLVL range & MLVL
	450,	700, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	33,	8,	36,	54,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	72, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	760,		// special flag for drop, animation size, XP
	"X\\Skeleton010-white.trn", "Soul Hunter" // ice age  TRN & name
},
{
	BM_159_GUARDIAN,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Mega\\Mega%c.CL2",	1,	"X\\Balor%c%i.wav",		1,
	1,	"X\\Mega_01.TRN",	// TRNs:   flag and path
	6, 7, 13, 1, 24, 5,		// IWAHDS frames
	3, 0, 0, 0, 3, 0,		// delays
	"Guardian",			// name
	34,	38,	55,		// DLVL range & MLVL
	950,	1300, // HP range,
	AI_26_Balrog,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	20,	8,	50,	63,// tohit, hit frame of 1st attack, dmg range.
	25,	2,	60,	60,	 // 2nd attack: tohit, hit frame, dmg range
	68, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	1155,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_160_CHASM_DEMON,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Thin\\Thin%c.CL2",	1,	"Monsters\\Thin\\Thin%c%i.WAV",		1,
	1,	"X\\Thin_11.TRN",	// TRNs:   flag and path
	8, 8, 18, 3, 17, 14,		// IWAHDS frames
	3, 0, 0, 0, 3, 0,		// delays
	"Chasm Demon",			// name
	32,	38,	54,		// DLVL range & MLVL
	875,	1225, // HP range,
	AI_16_Thunder_Demon,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	17,	4,	48,	62,// tohit, hit frame of 1st attack, dmg range.
	18,	5,	50,	62,	 // 2nd attack: tohit, hit frame, dmg range
	66, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_11_IM_COLD,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	915,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_161_CHASM_DRAKE,	1, 100,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Snake\\Snake%c.CL2",	1,	"Monsters\\Snake\\Snake%c%i.WAV",		1,
	1,	"X\\Drake01_Crypt.TRN",	// TRNs:   flag and path
	12, 11, 13, 6, 18, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Chasm Drake",			// name
	40,	42,	56,		// DLVL range & MLVL
	1000,	1250, // HP range,
	AI_24_Viper,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 9,	// AI, int flag, int level
	28,	8,	47,	64,// tohit, hit frame of 1st attack, dmg range.
	38,	9,	56,	65,	 // 2nd attack: tohit, hit frame, dmg range
	76, 2, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	0,		7,	1335,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_162_CORRUPT_PALADIN,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\renwar01\\rw%c.CL2",	1,	"Monsters\\renwar01\\rw%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	6, 8, 14, 6, 8, 2,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Corrupt Paladin",			// name
	30,	30,	50,		// DLVL range & MLVL
	1000,	1000, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	19,	9,	30,	40,// tohit, hit frame of 1st attack, dmg range.
	24,	1,	55,	55,	 // 2nd attack: tohit, hit frame, dmg range
	58, 2, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,// armor, mon type, and resistances
	0,		7,	4000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_163_STINGER_LORD,	0, 45,	86,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\wscorp\\WScorp%c.CL2",	1,	"Monsters\\newsfx\\Stingr%c%i.WAV",		0,
	0,	0,	// TRNs:   flag and path
	10, 10, 12, 6, 15, 12,		// IWAHDS frames
	2, 0, 0, 0, 2, 0,		// delays
	"Stinger Lord",			// name
	36,	36,	53,		// DLVL range & MLVL
	1000,	1000, // HP range,
	AI_11_Bat,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 9,	// AI, int flag, int level
	24,	7,	30,	40,// tohit, hit frame of 1st attack, dmg range.
	29,	6,	35,	45,	 // 2nd attack: tohit, hit frame, dmg range
	68, 2, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN|MR_7_IM_CURSE,	MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		1,	2000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_164_LIGHTNING_DEMON,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Thin\\Thin%c.CL2",	1,	"Monsters\\Thin\\Thin%c%i.WAV",		1,
	1,	"X\\Thin_17x1_Cryptic.TRN",	// TRNs:   flag and path
	8, 8, 18, 3, 17, 14,		// IWAHDS frames
	3, 0, 0, 0, 3, 0,		// delays
	"Lightning Demon",			// name
	40,	44,	56,		// DLVL range & MLVL
	1000,	1400, // HP range,
	AI_16_Thunder_Demon,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	30,	4,	54,	64,// tohit, hit frame of 1st attack, dmg range.
	30,	5,	55,	62,	 // 2nd attack: tohit, hit frame, dmg range
	78, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	1400,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_165_APOCALYPSE_CLAN,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatBow\\GoatB%c.CL2",	1,	"Monsters\\GoatBow\\GoatB%c%i.WAV",		1,
	1,	"X\\Goat_22.TRN",	// TRNs:   flag and path
	12, 8, 16, 6, 20, 16,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Apocalypse Clan",			// name
	32,	38,	53,		// DLVL range & MLVL
	400,	500, // HP range,
	AI_44_Ranger_Cold_Arrow,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	18,	13,	36,	52,// tohit, hit frame of 1st attack, dmg range.
	22,	12,	58,	62,	 // 2nd attack: tohit, hit frame, dmg range
	64, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	725,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_166_GOATLORD_BOSS,	0, 0,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatLord\\GoatL%c.CL2",	0,	"Monsters\\GoatLord\\GoatL%c%i.WAV",		0,
	0,	"X\\RO.trn",	// TRNs:   flag and path
	13, 13, 13, 8, 16, 0,		// IWAHDS frames
	3, 0, 1, 1, 2, 0,		// delays
	"Goatlord Boss (18-19)",			// name
	34,	36,	54,		// DLVL range & MLVL
	500,	500, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	30,	8,	21,	37,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	70, 1, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN|MR_7_IM_CURSE,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	3000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_167_SKULLWING,	1, 103,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Demskel\\Demskl%c.CL2",	1,	"Monsters\\newsfx\\SWing%c%i.WAV",		1,
	1,	"X\\Skeleton004x1_darker_beige.TRN",	// TRNs:   flag and path
	10, 8, 20, 5, 24, 16,		// IWAHDS frames
	3, 0, 0, 0, 1, 0,		// delays
	"Skullwing",			// name
	44,	46,	57,		// DLVL range & MLVL
	1300,	1700, // HP range,
	AI_40_Bonewing_cold_blasts,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	34,	7,	56,	64,// tohit, hit frame of 1st attack, dmg range.
	36,	11,	63,	65,	 // 2nd attack: tohit, hit frame, dmg range
	83, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1650,		// special flag for drop, animation size, XP
	"x\\demskel_07_iceage_deathangel.trn", 0 // ice age  TRN & name
},
{
	BM_168_HELL_BAT,	1, 0,	96,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\hellbat\\helbat%c.CL2",	1,	"Monsters\\newsfx\\HelBat%c%i.WAV",		0,
	0,	0,	// TRNs:   flag and path
	16, 16, 14, 6, 18, 14,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Hell Bat",			// name
	38,	42,	52,		// DLVL range & MLVL
	400,	500, // HP range,
	AI_49_Ranger_Fireblast,	MF_9_CHASE_TARGET, 2,	// AI, int flag, int level
	21,	8,	40,	60,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	72, 2, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT,	MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	16384,		7,	500,		// special flag for drop, animation size, XP
	"x\\Firebat_01_purp_blue.trn", "Arctic Bat" // ice age  TRN & name
}, 
{
	BM_169_ZOMBIE,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Zombie\\Zombie%c.CL2",	1,	"Monsters\\Zombie\\Zombie%c%i.WAV",		1,
	1,	"X\\grn.TRN",	// TRNs:   flag and path
	11, 24, 12, 6, 16, 11,		// IWAHDS frames
	4, 0, 0, 0, 0, 4,		// delays
	"Zombie",			// name
	4,	6,	9,		// DLVL range & MLVL
	75,	110, // HP range,
	AI_0_Zombie,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	19,	8,	15,	19,// tohit, hit frame of 1st attack, dmg range.
	0,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	12, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	172,		// special flag for drop, animation size, XP
	"x\\Zombie07_white-red_v2.trn", "Plague Zombie" // ice age  TRN & name
},
{
	BM_170_LICH,	0, 97,	96,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Lich\\Lich%c.CL2",	1,	"Monsters\\newsfx\\Lich%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	12, 10, 10, 7, 21, 19,		// IWAHDS frames
	2, 0, 0, 0, 2, 0,		// delays
	"Lich",			// name
	6,	8,	13,		// DLVL range & MLVL
	80,	100, // HP range,
	AI_35_Lich,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	12,	7,	8,	19,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	11, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	250,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_171_MUD_LORD,	1, 98,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Rhino\\Rhino%c.CL2",	1,	"Monsters\\Rhino\\Rhino%c%i.WAV",		1,
	1,	"X\\Rhino009-beige.TRN",	// TRNs:   flag and path
	8, 8, 14, 6, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Mud Lord",			// name
	38,	40,	55,		// DLVL range & MLVL
	1300,	1700, // HP range,
	AI_5_Charger,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 7,	// AI, int flag, int level
	25,	6,	46,	64,// tohit, hit frame of 1st attack, dmg range.
	46,	0,	61,	65,	 // 2nd attack: tohit, hit frame, dmg range
	72, 2, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	1260,		// special flag for drop, animation size, XP
	0, "Snow Trampler" // ice age  TRN & name
},
{
	BM_172_SPITTING_TERROR,	1, 57,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Acid\\Acid%c.CL2",	1,	"Monsters\\Acid\\Acid%c%i.WAV",		1,
	1,	"X\\SpittingTerror.TRN",	// TRNs:   flag and path
	13, 8, 12, 7, 16, 12,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Spitting Terror",			// name
	46,	46,	56,		// DLVL range & MLVL
	800,	1150, // HP range,
	AI_19_Acid_Beast,	MF_9_CHASE_TARGET, 3,	// AI, int flag, int level
	38,	6,	50,	62,// tohit, hit frame of 1st attack, dmg range.
	38,	4,	30,	50,	 // 2nd attack: tohit, hit frame, dmg range
	80, 2, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE | MR_8_IM_ACID | MR_11_IM_COLD,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	935,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_173_GHARBAD_BASEMNSTR,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatMace\\Goat%c.CL2",	1,	"Monsters\\GoatMace\\Goat%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	12, 8, 10, 5, 20, 9,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Gharbad basemnstr",			// name
	20,	20,	33,		// DLVL range & MLVL
	350,	450, // HP range,
	AI_6_Goat_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	35,	8,	27,	43,// tohit, hit frame of 1st attack, dmg range.
	45,	8,	51,	57,	 // 2nd attack: tohit, hit frame, dmg range
	50, 1, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	4000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_174_ZHAR_BASEMNSTR,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\DarkMage\\Dmage%c.CL2",	1,	"Monsters\\DarkMage\\Dmag%c%i.WAV",		0,
	0,	"Monsters\\GoatMace\\Gray.TRN",	// TRNs:   flag and path
	6, 1, 20, 6, 23, 18,		// IWAHDS frames
	3, 0, 0, 0, 1, 0,		// delays
	"Zhar basemnstr",			// name
	14,	14,	30,		// DLVL range & MLVL
	450,	700, // HP range,
	AI_25_Advocate,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	36,	8,	48,	64,// tohit, hit frame of 1st attack, dmg range.
	28,	0,	60,	60,	 // 2nd attack: tohit, hit frame, dmg range
	30, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	1500,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_175_TERROR_CLAN,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatMace\\Goat%c.CL2",	1,	"Monsters\\GoatMace\\Goat%c%i.WAV",		1,
	1,	"X\\Goat_23_Church.TRN",	// TRNs:   flag and path
	12, 8, 12, 6, 20, 12,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Terror Clan",			// name
	6,	6,	15,		// DLVL range & MLVL
	250,	300, // HP range,
	AI_6_Goat_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	9,	8,	12,	20,// tohit, hit frame of 1st attack, dmg range.
	14,	7,	20,	27,	 // 2nd attack: tohit, hit frame, dmg range
	15, 1, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	0,		3,	290,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_176_DESTROYER,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Mega\\Mega%c.CL2",	1,	"X\\Balor%c%i.wav",		1,
	1,	"X\\Mega_03x1.TRN",	// TRNs:   flag and path
	6, 7, 13, 1, 24, 5,		// IWAHDS frames
	3, 0, 0, 0, 3, 0,		// delays
	"Destroyer",			// name
	40,	42,	56,		// DLVL range & MLVL
	1200,	1500, // HP range,
	AI_26_Balrog,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	28,	8,	52,	65,// tohit, hit frame of 1st attack, dmg range.
	33,	2,	61,	61,	 // 2nd attack: tohit, hit frame, dmg range
	76, 1, MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	1456,		// special flag for drop, animation size, XP
	"x\\Mega_07_bluish.trn", "Frost Guardian" // ice age  TRN & name
},
{
	BM_177_PHASE_DEMON,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\TSneak\\TSneak%c.CL2",	0,	"Monsters\\TSneak\\sneakl%c%i.wav",		0,
	1,	"X\\Warper02x1.TRN",	// TRNs:   flag and path
	13, 13, 15, 9, 16, 0,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Phase Demon",			// name
	46,	46,	58,		// DLVL range & MLVL
	900,	1200, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	34,	9,	51,	59,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	79, 1, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,// armor, mon type, and resistances
	0,		3,	952,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_178_OVERLORD,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Fat\\Fat%c.CL2",	1,	"Monsters\\Fat\\Fat%c%i.WAV",		1,
	1,	"X\\Fat_07_greyish.TRN",	// TRNs:   flag and path
	8, 10, 15, 5, 16, 10,		// IWAHDS frames
	4, 0, 0, 0, 0, 0,		// delays
	"Overlord",			// name
	26,	32,	48,		// DLVL range & MLVL
	830,	1100, // HP range,
	AI_1_Overlord,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 6,	// AI, int flag, int level
	15,	6,	41,	54,// tohit, hit frame of 1st attack, dmg range.
	30,	5,	51,	60,	 // 2nd attack: tohit, hit frame, dmg range
	56, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	995,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_179_FILTH_DEMON,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Fat\\Fat%c.CL2",	1,	"Monsters\\Fat\\Fat%c%i.WAV",		1,
	1,	"X\\Fat_06_brownie.TRN",	// TRNs:   flag and path
	8, 10, 15, 5, 16, 10,		// IWAHDS frames
	4, 0, 0, 0, 0, 0,		// delays
	"Filth Demon",			// name
	34,	40,	54,		// DLVL range & MLVL
	1100,	1500, // HP range,
	AI_1_Overlord,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 7,	// AI, int flag, int level
	21,	6,	50,	57,// tohit, hit frame of 1st attack, dmg range.
	36,	5,	54,	62,	 // 2nd attack: tohit, hit frame, dmg range
	70, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	1065,		// special flag for drop, animation size, XP
	"X\\Fat_10_black_v3.TRN", "Blackheart Demon" // ice age  TRN & name
}, 
{
	BM_180_CRYPT_STONE,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Magma\\Magma%c.CL2",	1,	"Monsters\\Magma\\Magma%c%i.WAV",		1,
	1,	"X\\Magma_07.trn",	// TRNs:   flag and path
	8, 10, 14, 7, 18, 18,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Crypt Stone",			// name
	40,	44,	56,		// DLVL range & MLVL
	1250,	1600, // HP range,
	AI_9_Magma,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	34,	4,	52,	60,// tohit, hit frame of 1st attack, dmg range.
	49,	14,	56,	64,	 // 2nd attack: tohit, hit frame, dmg range
	77, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	1497,		// special flag for drop, animation size, XP
	"cf\\iceage\\Magma_13_iceage_cryptstone.trn", 0 // ice age  TRN & name
}, 
{
	BM_181_ADVOCATE,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\DarkMage\\Dmage%c.CL2",	1,	"Monsters\\DarkMage\\Dmag%c%i.WAV",		0,
	1,	"X\\Darkmage01.TRN",	// TRNs:   flag and path
	6, 1, 19, 5, 23, 18,		// IWAHDS frames
	3, 0, 0, 0, 1, 0,		// delays
	"Advocate",			// name
	46,	46,	58,		// DLVL range & MLVL
	500,	800, // HP range,
	AI_25_Advocate,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	36,	8,	64,	66,// tohit, hit frame of 1st attack, dmg range.
	28,	0,	64,	64,	 // 2nd attack: tohit, hit frame, dmg range
	85, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1100,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_182_BLINK_DEMON,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\TSneak\\TSneak%c.CL2",	0,	"Monsters\\TSneak\\sneakl%c%i.wav",		0,
	1,	"X\\Tsneak002x1.TRN",	// TRNs:   flag and path
	13, 13, 15, 10, 16, 0,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Blink Demon",			// name
	38,	42,	55,		// DLVL range & MLVL
	800,	1050, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	28,	9,	45,	57,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	69, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	0,		3,	780,		// special flag for drop, animation size, XP
	"X\\Tsneak004_frostblinker.trn", "Blinker" // ice age  TRN & name
},
{
	BM_183_STALKER,	1, 63,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Sneak\\Sneak%c.CL2",	1,	"Monsters\\Sneak\\Sneak%c%i.WAV",		0,
	1,	"X\\Hidden08x1.TRN",	// TRNs:   flag and path
	16, 8, 12, 8, 24, 15,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Stalker",			// name
	38,	42,	56,		// DLVL range & MLVL
	700,	900, // HP range,
	AI_15_Hidden,	MF_1_INVISIBLE | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	26,	7,	35,	50,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	74, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	800,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_184_REALITY_WEAVER,	1, 63,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Sneak\\Sneak%c.CL2",	1,	"Monsters\\Sneak\\Sneak%c%i.WAV",		0,
	1,	"X\\Hidden15-unseenboss.TRN",	// TRNs:   flag and path
	16, 8, 12, 7, 24, 15,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Reality Weaver",			// name
	44,	46,	58,		// DLVL range & MLVL
	850,	1000, // HP range,
	AI_15_Hidden,	MF_1_INVISIBLE | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	32,	7,	45,	60,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	81, 1, MR_1_ARCAN|MR_3_LIGHT|MR_5_IM_FIRE,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	875,		// special flag for drop, animation size, XP
	"X\\Hidden26_just_a_cool_version_of_a_crimson_hidden.trn", 0 // ice age  TRN & name
},
{
	BM_185_BLOOD_HORN,	1, 98,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Rhino\\Rhino%c.CL2",	1,	"Monsters\\Rhino\\Rhino%c%i.WAV",		1,
	1,	"X\\Rhino014-red-orng.TRN",	// TRNs:   flag and path
	8, 8, 14, 6, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Blood Horn",			// name
	40,	44,	56,		// DLVL range & MLVL
	1325,	1825, // HP range,
	AI_5_Charger,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 7,	// AI, int flag, int level
	30,	6,	50,	64,// tohit, hit frame of 1st attack, dmg range.
	50,	0,	65,	66,	 // 2nd attack: tohit, hit frame, dmg range
	78, 2, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	0,		7,	1300,		// special flag for drop, animation size, XP
	"X\\Rhino017_combo_red_black.trn", 0 // ice age  TRN & name
},
{
	BM_186_DEATH_CLAN,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatMace\\Goat%c.CL2",	1,	"Monsters\\GoatMace\\Goat%c%i.WAV",		1,
	1,	"X\\Goat_24.TRN",	// TRNs:   flag and path
	12, 8, 12, 6, 20, 12,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Death Clan",			// name
	18,	22,	34,		// DLVL range & MLVL
	400,	550, // HP range,
	AI_6_Goat_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	16,	8,	26,	45,// tohit, hit frame of 1st attack, dmg range.
	21,	7,	42,	46,	 // 2nd attack: tohit, hit frame, dmg range
	38, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	612,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_187_BLOOD_CLAN,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatMace\\Goat%c.CL2",	1,	"Monsters\\GoatMace\\Goat%c%i.WAV",		1,
	1,	"X\\Goat_26_HELL.TRN",	// TRNs:   flag and path
	12, 8, 12, 6, 20, 12,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Blood Clan",			// name
	24,	30,	45,		// DLVL range & MLVL
	650,	850, // HP range,
	AI_6_Goat_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	14,	8,	39,	51,// tohit, hit frame of 1st attack, dmg range.
	24,	7,	45,	55,	 // 2nd attack: tohit, hit frame, dmg range
	50, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	0,		3,	615,		// special flag for drop, animation size, XP
	"X\\moonclan.TRN", "Night Clan" // ice age  TRN & name
},
{
	BM_188_HELL_CLAN,	0, 0,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatLord\\GoatL%c.CL2",	0,	"Monsters\\GoatLord\\GoatL%c%i.WAV",		0,
	0,	"X\\RO.trn",	// TRNs:   flag and path
	13, 13, 12, 8, 16, 0,		// IWAHDS frames
	3, 0, 1, 1, 2, 0,		// delays
	"Hell Clan",			// name
	30,	30,	56,		// DLVL range & MLVL
	500,	500, // HP range,
	AI_13_Butcher,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	12,	7,	40,	50,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	60, 1, MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	2000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_189_CARVER,	1, 61,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FalSpear\\Phall%c.CL2",	1,	"Monsters\\BigFall\\bfal%c%i.WAV",		1,
	1,	"X\\Fallen03.TRN",	// TRNs:   flag and path
	11, 11, 12, 11, 18, 13,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Carver",			// name
	6,	10,	17,		// DLVL range & MLVL
	140,	200, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	18,	7,	13,	23,// tohit, hit frame of 1st attack, dmg range.
	0,	5,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	19, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	280,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_190_WINGED_DEMON,	0, 0,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Gargoyle\\Gargu%c.CL2",	1,	"Monsters\\Gargoyle\\Gargo%c%i.WAV",		0,
	0,	0,	// TRNs:   flag and path
	14, 14, 14, 10, 18, 10,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Winged Demon",			// name
	8,	8,	19,		// DLVL range & MLVL
	250,	250, // HP range,
	AI_12_Gargoyle,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	25,	7,	20,	35,// tohit, hit frame of 1st attack, dmg range.
	25,	4,	2,	4,	 // 2nd attack: tohit, hit frame, dmg range
	25, 1, MR_1_ARCAN|MR_2_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		6,	1200,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_191_DARK_MISTRESS,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Succ\\Scbs%c.CL2",	0,	"Monsters\\Succ\\Scbs%c%i.WAV",		0,
	0,	"Monsters\\Acid\\AcidR.TRN",	// TRNs:   flag and path
	14, 8, 16, 7, 24, 0,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Dark Mistress",			// name
	10,	10,	24,		// DLVL range & MLVL
	100,	100, // HP range,
	AI_14_Witch,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	15,	10,	12,	30,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	22, 1, MR_1_ARCAN|MR_3_LIGHT|MR_5_IM_FIRE,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	1500,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_192_UNRAVELLER,	1, 99,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Unrav\\Unrav%c.CL2",	1,	"Monsters\\newsfx\\Shred%c%i.WAV",		0,
	1,	"X\\unravb.TRN",	// TRNs:   flag and path
	10, 10, 12, 5, 16, 13,		// IWAHDS frames
	2, 1, 0, 1, 0, 0,		// delays
	"Unraveller",			// name
	0,	0,	2,		// DLVL range & MLVL
	20,	30, // HP range,
	AI_2_Skel_Melee,	0, 0,	// AI, int flag, int level
	11,	7,	1,	7,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	2, 0, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	60,		// special flag for drop, animation size, XP
	0, "Ghost" // ice age  TRN & name
}, 
{
	BM_193_ABATAN,	0, 0,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Mega\\Mega%c.CL2",	1,	"X\\Balor%c%i.wav",		1,
	0,	0,	// TRNs:   flag and path
	6, 7, 11, 1, 24, 5,		// IWAHDS frames
	2, 0, 0, 0, 3, 0,		// delays
	"DLVL12 Uberboss",			// name
	22,	22,	58,		// DLVL range & MLVL
	1000,	1000, // HP range,
	AI_26_Balrog,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	70,	8,	60,	60,// tohit, hit frame of 1st attack, dmg range.
	40,	2,	60,	60,	 // 2nd attack: tohit, hit frame, dmg range
	80, 1, MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	14000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_194_BLOODIED_HIDDEN,	0, 63,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Sneak\\Sneak%c.CL2",	1,	"Monsters\\Sneak\\Sneak%c%i.WAV",		0,
	1,	"X\\Tsneak003.TRN",	// TRNs:   flag and path
	16, 8, 11, 7, 24, 15,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Bloodied Hidden",			// name
	22,	22,	62,		// DLVL range & MLVL
	400,	450, // HP range,
	AI_15_Hidden,	MF_1_INVISIBLE | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	30,	7,	20,	50,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	40, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	1000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_195_UBER_BUTCHER,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FatC\\FatC%c.CL2",	1,	"X\\Buch%c%i.wav",		1,
	0,	0,	// TRNs:   flag and path
	10, 8, 7, 1, 16, 1,		// IWAHDS frames
	1, 0, 0, 0, 0, 0,		// delays
	"Uber Butcher",			// name
	6,	6,	57,		// DLVL range & MLVL
	1000,	1000, // HP range,
	AI_8_Fallen_One,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	62,	6,	25,	55,// tohit, hit frame of 1st attack, dmg range.
	50,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	75, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	10000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_196_EVIL_ONE,	0, 60,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FalSpear\\Phall%c.CL2",	1,	"Monsters\\BigFall\\bfal%c%i.WAV",		1,
	1,	"X\\Fallen05.TRN",	// TRNs:   flag and path
	11, 11, 12, 6, 18, 13,		// IWAHDS frames
	1, 0, 0, 0, 0, 0,		// delays
	"Evil One",			// name
	6,	6,	62,		// DLVL range & MLVL
	350,	400, // HP range,
	AI_15_Hidden,	MF_1_INVISIBLE | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	25,	7,	25,	50,// tohit, hit frame of 1st attack, dmg range.
	0,	5,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	70, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	0,		3,	800,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_197_CEROMORTH,	0, 98,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Rhino\\Rhino%c.CL2",	1,	"Monsters\\Rhino\\Rhino%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	8, 8, 13, 5, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Ceromorth",			// name
	14,	14,	58,		// DLVL range & MLVL
	1000,	1000, // HP range,
	AI_5_Charger,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	65,	6,	45,	60,// tohit, hit frame of 1st attack, dmg range.
	50,	0,	66,	66,	 // 2nd attack: tohit, hit frame, dmg range
	78, 2, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	13750,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_198_HORN_CLAN,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatBow\\GoatB%c.CL2",	0,	"Monsters\\GoatBow\\GoatB%c%i.WAV",		0,
	1,	"CF\\MD\\EXORNG.TRN",	// TRNs:   flag and path
	12, 8, 14, 5, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Horn Clan",			// name
	14,	14,	63,		// DLVL range & MLVL
	350,	400, // HP range,
	AI_7_Goat_Archer,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	30,	12,	66,	66,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	78, 1, MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	1500,		// special flag for drop, animation size, XP
	"CF\\iceage\\Goat_32_horn_clan_icy.trn", "Winter Clan" // ice age  TRN & name
},
{
	BM_199_HELL_CLAN,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatBow\\GoatB%c.CL2",	1,	"Monsters\\GoatBow\\GoatB%c%i.WAV",		1,
	1,	"X\\HCA.TRN",	// TRNs:   flag and path
	12, 8, 16, 6, 20, 16,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Hell Clan",			// name
	24,	30,	41,		// DLVL range & MLVL
	325,	400, // HP range,
	AI_7_Goat_Archer,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	25,	13,	45,	65,// tohit, hit frame of 1st attack, dmg range.
	25,	12,	64,	66,	 // 2nd attack: tohit, hit frame, dmg range
	48, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	600,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_200_CABALIST,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Mage\\Mage%c.cl2",	1,	"Monsters\\Mage\\Mage%c%i.wav",		1,
	1,	"X\\Mage_04_Blue.TRN",	// TRNs:   flag and path
	12, 1, 20, 8, 28, 20,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Cabalist",			// name
	24,	26,	42,		// DLVL range & MLVL
	250,	350, // HP range,
	AI_25_Advocate,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	35,	9,	35,	55,// tohit, hit frame of 1st attack, dmg range.
	35,	0,	60,	60,	 // 2nd attack: tohit, hit frame, dmg range
	47, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	650,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_201_ADEPT,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Mage\\Mage%c.cl2",	1,	"Monsters\\Mage\\Mage%c%i.wav",		1,
	0,	"X\\Mage_04_Blue.TRN",	// TRNs:   flag and path
	12, 1, 20, 8, 28, 20,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Adept",			// name
	24,	26,	42,		// DLVL range & MLVL
	250,	350, // HP range,
	AI_25_Advocate,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	35,	9,	35,	55,// tohit, hit frame of 1st attack, dmg range.
	35,	0,	60,	60,	 // 2nd attack: tohit, hit frame, dmg range
	47, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	650,		// special flag for drop, animation size, XP
	"X\\Mage_05_crimz.trn", 0 // ice age  TRN & name
},
{
	BM_202_SUCCUBUS,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Succ\\Scbs%c.CL2",	0,	"Monsters\\Succ\\Scbs%c%i.WAV",		0,
	1,	"X\\Sucub_03.TRN",	// TRNs:   flag and path
	14, 8, 16, 7, 24, 0,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Succubus",			// name
	16,	20,	30,		// DLVL range & MLVL
	150,	225, // HP range,
	AI_14_Witch,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	25,	10,	24,	42,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	33, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	477,		// special flag for drop, animation size, XP
	"x\\Sucub_09_snow_witch_iceage.trn", "Snow Witch" // ice age  TRN & name
},
{
	BM_203_DEATH_KNIGHT,	1, 100,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SKing\\SKing%c.CL2",	1,	"X\\69%c%i.WAV",		1,
	1,	"Monsters\\SkelAxe\\White.TRN",	// TRNs:   flag and path
	8, 8, 14, 5, 16, 12,		// IWAHDS frames
	2, 0, 0, 0, 2, 1,		// delays
	"Death Knight",			// name
	46,	46,	58,		// DLVL range & MLVL
	1500,	2000, // HP range,
	AI_8_Fallen_One,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 6,	// AI, int flag, int level
	39,	8,	55,	65,// tohit, hit frame of 1st attack, dmg range.
	0,	6,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	85, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	1980,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_204_SHADOW_CLAN,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatMace\\Goat%c.CL2",	1,	"Monsters\\GoatMace\\Goat%c%i.WAV",		1,
	1,	"X\\Goat_27_Cryptic.TRN",	// TRNs:   flag and path
	12, 8, 11, 5, 20, 9,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Shadow Clan",			// name
	42,	44,	58,		// DLVL range & MLVL
	1000,	1500, // HP range,
	AI_6_Goat_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	33,	8,	60,	70,// tohit, hit frame of 1st attack, dmg range.
	63,	7,	75,	85,	 // 2nd attack: tohit, hit frame, dmg range
	80, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	850,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_205_PINKY,	0, 0,	188,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Fork\\Fork%c.CL2",	1,	"Monsters\\Fork\\DBoar%c%i.wav",		1,
	0,	0,	// TRNs:   flag and path
	10, 10, 14, 1, 16, 6,		// IWAHDS frames
	1, 0, 0, 0, 2, 1,		// delays
	"Pinky",			// name
	0,	0,	56,		// DLVL range & MLVL
	1,	1, // HP range,
	AI_5_Charger,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	60,	7,	45,	65,// tohit, hit frame of 1st attack, dmg range.
	60,	0,	65,	65,	 // 2nd attack: tohit, hit frame, dmg range
	65, 2, MR_0_NO,	MR_0_NO,// armor, mon type, and resistances
	0,		3,	11500,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_206_ACID_SPITTER,	0, 57,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Acid\\Acid%c.CL2",	1,	"Monsters\\Acid\\Acid%c%i.WAV",		1,
	1,	"CF\\MD\\BSTS.TRN",	// TRNs:   flag and path
	13, 8, 12, 7, 16, 11,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Acid Spitter",			// name
	0,	0,	63,		// DLVL range & MLVL
	25,	675, // HP range,
	AI_20_Acid_Beast_Ranger,	0, 3,	// AI, int flag, int level
	60,	6,	56,	64,// tohit, hit frame of 1st attack, dmg range.
	60,	4,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	60, 2, MR_0_NO,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_8_IM_ACID,// armor, mon type, and resistances
	0,		3,	3000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_207_DREGBEZ,	0, 77,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\BigFall\\Fallg%c.CL2",	1,	"monsters\\bigfall\\gi%c%i.wav",		1,
	0,	0,	// TRNs:   flag and path
	10, 8, 7, 8, 17, 5,		// IWAHDS frames
	0, 0, 0, 0, 2, 2,		// delays
	"Dregbez",			// name
	2,	2,	57,		// DLVL range & MLVL
	1000,	1200, // HP range,
	AI_13_Butcher,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	65,	4,	30,	50,// tohit, hit frame of 1st attack, dmg range.
	0,	3,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	70, 1, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	9000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_208_WARPED_ONE,	0, 58,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Warped1\\w1%c.CL2",	1,	"Monsters\\Warped1\\w1%c%i.wav",		1,
	1,	"X\\Fallen06.trn",	// TRNs:   flag and path
	12, 11, 13, 6, 14, 5,		// IWAHDS frames
	1, 0, 0, 0, 0, 0,		// delays
	"Warped One",			// name
	2,	2,	62,		// DLVL range & MLVL
	50,	250, // HP range,
	AI_11_Bat,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	60,	7,	20,	45,// tohit, hit frame of 1st attack, dmg range.
	24,	1,	55,	55,	 // 2nd attack: tohit, hit frame, dmg range
	65, 1, MR_1_ARCAN|MR_3_LIGHT|MR_5_IM_FIRE|MR_8_IM_ACID,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	800,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_209_HATRED_CLAN,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatMace\\Goat%c.CL2",	1,	"Monsters\\GoatMace\\Goat%c%i.WAV",		1,
	1,	"X\\Goat_17x3.TRN",	// TRNs:   flag and path
	12, 8, 12, 6, 20, 10,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Hatred Clan",			// name
	32,	38,	55,		// DLVL range & MLVL
	750,	950, // HP range,
	AI_6_Goat_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	15,	8,	42,	56,// tohit, hit frame of 1st attack, dmg range.
	25,	7,	46,	58,	 // 2nd attack: tohit, hit frame, dmg range
	65, 1, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	765,		// special flag for drop, animation size, XP
	"x\\Goat_31_icy.trn", "Ice Clan" // ice age  TRN & name
},
{
	BM_210_HELLWING,	1, 103,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Demskel\\Demskl%c.CL2",	1,	"Monsters\\newsfx\\SWing%c%i.WAV",		1,
	1,	"X\\04_dark_red-orng.TRN",	// TRNs:   flag and path
	10, 8, 20, 5, 24, 15,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Hellwing",			// name
	24,	30,	47,		// DLVL range & MLVL
	425,	575, // HP range,
	AI_40_Bonewing_cold_blasts,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	12,	7,	30,	54,// tohit, hit frame of 1st attack, dmg range.
	16,	11,	40,	60,	 // 2nd attack: tohit, hit frame, dmg range
	51, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	600,		// special flag for drop, animation size, XP
	"x\\demskel_08_hell_iceage_purp-blue.trn", "Bone Stalker" // ice age  TRN & name
},
{
	BM_211_TORCHANT,	1, 0,	192,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\AntWorm\\Worm%c.CL2",	1,	"Monsters\\newsfx\\TchAnt%c%i.WAV",		1,
	1,	"X\\Wrm2.trn",	// TRNs:   flag and path
	14, 12, 12, 6, 20, 12,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Torchant",			// name
	32,	38,	54,		// DLVL range & MLVL
	800,	1000, // HP range,
	AI_9_Magma,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	16,	9,	35,	55,// tohit, hit frame of 1st attack, dmg range.
	46,	9,	60,	65,	 // 2nd attack: tohit, hit frame, dmg range
	65, 2, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	0,		7,	810,		// special flag for drop, animation size, XP
	"X\\Wyrm_04_iceage_purp_blue.trn", "Snow Crawler" // ice age  TRN & name
},
{
	BM_212_CACODEMON,	1, 0,	156,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Eye\\Eye%c.CL2",	1,	"Monsters\\Eye2\\Orb%c%i.wav",		1,
	1,	"X\\TAZZ.TRN",	// TRNs:   flag and path
	12, 3, 12, 7, 21, 12,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Cacodemon",			// name
	36,	38,	56,		// DLVL range & MLVL
	400,	450, // HP range,
	AI_14_Witch,	0, 2,	// AI, int flag, int level
	25,	6,	20,	50,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	72, 1, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_7_IM_CURSE | MR_8_IM_ACID|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		6,	755,		// special flag for drop, animation size, XP
	"x\\Eye_1_blue.trn", "Chaos Floater" // ice age  TRN & name
},
{
	BM_213_POISONED_ONE,	0, 60,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FalSpear\\Phall%c.CL2",	1,	"Monsters\\BigFall\\bfal%c%i.WAV",		1,
	1,	"X\\Gremlin.TRN",	// TRNs:   flag and path
	11, 11, 12, 11, 18, 13,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Poisoned One",			// name
	2,	2,	15,		// DLVL range & MLVL
	40,	120, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	14,	7,	7,	17,// tohit, hit frame of 1st attack, dmg range.
	0,	5,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	15, 1, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN|MR_8_IM_ACID,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_8_IM_ACID,// armor, mon type, and resistances
	0,		3,	175,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_214_SATYR_LORD,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\satyr\\satyr%c.CL2",	1,	"Monsters\\GoatLord\\GoatL%c%i.WAV",		0,
	1,	"X\\Fat_04.TRN",	// TRNs:   flag and path
	12, 13, 13, 8, 16, 13,		// IWAHDS frames
	2, 0, 0, 0, 2, 0,		// delays
	"Satyr Lord",			// name
	40,	44,	57,		// DLVL range & MLVL
	1100,	1400, // HP range,
	AI_54_Mixed_Chargedbolt,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	28,	8,	35,	60,// tohit, hit frame of 1st attack, dmg range.
	0,	7,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	78, 1, MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	1223,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_215_CADAVER,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Corpse\\corp%c.cl2",	1,	"X\\Corp%c%i.WAV",		1,
	1,	"Monsters\\Zombie\\Bluered.TRN",	// TRNs:   flag and path
	11, 24, 12, 6, 16, 12,		// IWAHDS frames
	2, 0, 0, 0, 0, 1,		// delays
	"Cadaver",			// name
	14,	18,	20,		// DLVL range & MLVL
	400,	600, // HP range,
	AI_0_Zombie,	MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	20,	7,	30,	40,// tohit, hit frame of 1st attack, dmg range.
	1,	7,	35,	45,	 // 2nd attack: tohit, hit frame, dmg range
	27, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	450,		// special flag for drop, animation size, XP
	"x\\Zombie06_white_v3.trn", "Frozen Dead" // ice age  TRN & name
},
{
	BM_216_ZEALOT,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Order\\order%c.cl2",	0,	"Monsters\\Order\\order%c%i.wav",		0,
	1,	"X\\Weird.TRN",	// TRNs:   flag and path
	8, 8, 8, 7, 20, 1,		// IWAHDS frames
	2, 0, 1, 1, 2, 0,		// delays
	"Zealot",			// name
	4,	4,	58,		// DLVL range & MLVL
	25,	525, // HP range,
	AI_7_Goat_Archer,	0, 2,	// AI, int flag, int level
	50,	7,	64,	65,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	85, 2, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	0,		7,	5000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_217_GRIMSPIKE,	0, 120,	198,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Hellbug\\Hellbg%c.CL2",	1,	"Monsters\\newsfx\\Defile%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	8, 8, 12, 3, 14, 12,		// IWAHDS frames
	2, 0, 1, 1, 2, 0,		// delays
	"Grimspike",			// name
	44,	44,	59,		// DLVL range & MLVL
	1,	1, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	52,	5,	1,	1,// tohit, hit frame of 1st attack, dmg range.
	52,	7,	65,	65,	 // 2nd attack: tohit, hit frame, dmg range
	84, 1, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	8000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_218_ICE_GOLEM,	0, 0,	96,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Muddy\\Muddy%c.cl2",	1,	"Monsters\\Muddy\\Muddy%c%i.wav",		1,
	1,	"cf\\sh\\shbt.trn",	// TRNs:   flag and path
	3, 16, 12, 3, 12, 20,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Ice Golem",			// name
	48,	48,	29,		// DLVL range & MLVL
	320,	500, // HP range,
	AI_15_Hidden,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	16,	7,	28,	40,// tohit, hit frame of 1st attack, dmg range.
	0,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	32, 0, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT | MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	280,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_219_FROST_BONE,	0, 103,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Demskel\\Demskl%c.CL2",	1,	"Monsters\\newsfx\\SWing%c%i.WAV",		1,
	1,	"X\\demskel_04.trn",	// TRNs:   flag and path
	10, 8, 20, 5, 24, 15,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Frost Bone",			// name
	48,	48,	31,		// DLVL range & MLVL
	375,	425, // HP range,
	AI_40_Bonewing_cold_blasts,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	12,	7,	19,	37,// tohit, hit frame of 1st attack, dmg range.
	16,	11,	31,	47,	 // 2nd attack: tohit, hit frame, dmg range
	35, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT | MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_7_IM_CURSE | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	345,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_220_SNOW_LICH,	0, 97,	136,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Lich2\\Lich2%c.CL2",	0,	"Monsters\\newsfx\\Lich%c%i.WAV",		0,
	1,	"X\\Lich2_01-Glacial.trn",	// TRNs:   flag and path
	12, 10, 10, 7, 21, 0,		// IWAHDS frames
	2, 0, 0, 0, 2, 0,		// delays
	"Snow Lich",			// name
	48,	48,	30,		// DLVL range & MLVL
	175,	245, // HP range,
	AI_41_Ranger_snowbolt,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	12,	7,	19,	39,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	33, 0, MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT | MR_11_IM_COLD,	MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	335,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_221_RAT_MAN,	1, 68,	104,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Rat\\Rat%c.CL2",	1,	"Monsters\\newsfx\\TmbRat%c%i.WAV",		1,
	1,	"X\\Boar5.trn",	// TRNs:   flag and path
	11, 8, 12, 6, 20, 11,		// IWAHDS frames
	2, 0, 0, 0, 2, 0,		// delays
	"Rat Man",			// name
	0,	2,	3,		// DLVL range & MLVL
	25,	65, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	6,	8,	4,	8,// tohit, hit frame of 1st attack, dmg range.
	13,	7,	6,	10,	 // 2nd attack: tohit, hit frame, dmg range
	3, 2, MR_2_FIRE|MR_3_LIGHT|MR_4_IM_ARCAN,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE,// armor, mon type, and resistances
	0,		3,	91,		// special flag for drop, animation size, XP
	"X\\Rat004_blue_better.trn", "Grey Fang" // ice age  TRN & name
},
{
	BM_222_NIGHT_DEMON,	1, 123,	256,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Baboon\\Baboon_%c.CL2",	1,	"Monsters\\Baboon\\_%c%i.wav",		1,
	0,	0,	// TRNs:   flag and path
	16, 10, 12, 10, 16, 14,		// IWAHDS frames
	2, 1, 1, 1, 2, 1,		// delays
	"Night Demon",			// name
	34,	40,	51,		// DLVL range & MLVL
	1100,	1500, // HP range,
	AI_1_Overlord,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 6,	// AI, int flag, int level
	33,	6,	52,	59,// tohit, hit frame of 1st attack, dmg range.
	53,	8,	57,	61,	 // 2nd attack: tohit, hit frame, dmg range
	45, 1, MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT|MR_11_IM_COLD,	MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	1200,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_223_DEMONOLOG_SUMMON_1,	0, 0,	96,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Bat\\Bat%c.CL2",	1,	"Monsters\\Bat\\bat2%c%i.wav",		1,
	1,	"X\\Bat_01brightRed.TRN",	// TRNs:   flag and path
	9, 13, 8, 8, 13, 10,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Impling",			// name
	50,	50,	4,		// DLVL range & MLVL
	12,	24, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET, 3,	// AI, int flag, int level
	7,	3,	1,	8,// tohit, hit frame of 1st attack, dmg range.
	14,	2,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	4, 1, MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	16384,		6,	80,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_224_DEMONOLOG_SUMMON_2,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatBow\\GoatB%c.CL2",	1,	"Monsters\\GoatBow\\GoatB%c%i.WAV",		1,
	1,	"X\\Goat_28x2briteRed.TRN",	// TRNs:   flag and path
	12, 8, 14, 6, 20, 16,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Goat Archer",			// name
	50,	50,	19,		// DLVL range & MLVL
	200,	300, // HP range,
	AI_7_Goat_Archer,	MF_9_CHASE_TARGET, 3,	// AI, int flag, int level
	20,	12,	20,	50,// tohit, hit frame of 1st attack, dmg range.
	22,	12,	58,	62,	 // 2nd attack: tohit, hit frame, dmg range
	32, 1, MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	420,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_225_DEMONOLOG_SUMMON_3,	0, 0,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatLord\\GoatL%c.CL2",	1,	"Monsters\\newsfx\\Satyr%c%i.WAV",		1,
	1,	"X\\GoatLord_02briteRed.TRN",	// TRNs:   flag and path
	13, 13, 14, 7, 16, 9,		// IWAHDS frames
	3, 0, 0, 0, 0, 3,		// delays
	"Satyr Lord",			// name
	50,	50,	40,		// DLVL range & MLVL
	250,	300, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET, 5,	// AI, int flag, int level
	20,	8,	23,	40,// tohit, hit frame of 1st attack, dmg range.
	40,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	24, 1, MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	330,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_226_NECROMANT_SUMMON_1,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelBow\\SklBw%c.CL2",	1,	"Monsters\\SkelBow\\SklBw%c%i.WAV",		0,
	1,	"X\\Skeleton010-white.trn",	// TRNs:   flag and path
	9, 8, 14, 4, 16, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Skeletal Archer",			// name
	50,	50,	4,		// DLVL range & MLVL
	12,	24, // HP range,
	AI_7_Goat_Archer,	MF_9_CHASE_TARGET, 3,	// AI, int flag, int level
	6,	11,	1,	5,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	2, 0, MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	90,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, //* 227	*/ { 124,	0,	"Monsters\\Gravdg\\Gravdg%c.CL2",	1,	"Monsters\\newsfx\\diga%c%i.WAV",	1,	1,	"X\\Skeleton010-white.TRN",	24, 24, 11, 5, 16, 16,	3, 0, 0, 2, 2, 0,	"Gravedigger",	50,	50,	21,	260,	400,	AI_4,	MF_B9_PATH,	4,	18,	5,	20,	32,	0,	0,	0,	0,	24,	0,	MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,	0,	3,	320, 0, 0 }, 
{
	BM_227_NECROMANT_SUMMON_2,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelAxe\\SklAx%c.CL2",	1,	"Monsters\\SkelAxe\\SklAx%c%i.WAV",		0,
	1,	"X\\Skeleton010-white.trn",	// TRNs:   flag and path
	11, 8, 12, 5, 17, 15,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Skeletal Warrior",			// name
	50,	50,	23,		// DLVL range & MLVL
	450,	700, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET, 5,	// AI, int flag, int level
	33,	8,	36,	54,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	72, 0, MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	760,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_228_NECROMANT_SUMMON_3,	0, 103,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Demskel\\Demskl%c.CL2",	1,	"Monsters\\newsfx\\SWing%c%i.WAV",		1,
	1,	"X\\demskel_06Frozen.TRN",	// TRNs:   flag and path
	10, 8, 20, 5, 24, 15,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Greater Skeleton",			// name
	50,	50,	47,		// DLVL range & MLVL
	425,	575, // HP range,
	AI_39_Mixed_White_Star_phys,	MF_9_CHASE_TARGET, 4,	// AI, int flag, int level
	12,	7,	30,	54,// tohit, hit frame of 1st attack, dmg range.
	16,	11,	40,	60,	 // 2nd attack: tohit, hit frame, dmg range
	51, 0, MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	600,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_229_BEASTMASTER_SUMMON_1,	0, 46,	64,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Scorp\\Scorp%c.CL2",	1,	"Monsters\\newsfx\\Stingr%c%i.WAV",		0,
	1,	"X\\Scorp_01darker.TRN",	// TRNs:   flag and path
	10, 4, 9, 6, 15, 8,		// IWAHDS frames
	2, 0, 0, 0, 0, 1,		// delays
	"Stinger",			// name
	50,	50,	5,		// DLVL range & MLVL
	190,	330, // HP range,
	AI_11_Bat,	MF_9_CHASE_TARGET, 9,	// AI, int flag, int level
	17,	7,	9,	27,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	59, 2, MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	16384,		1,	290,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
}, 
{
	BM_230_BEASTMASTER_SUMMON_2,	0, 0,	188,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Fork\\Fork%c.CL2",	1,	"Monsters\\newsfx\\HBoar%c%i.WAV",		1,
	1,	"X\\Boar_09green.TRN",	// TRNs:   flag and path
	10, 10, 12, 5, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 1, 2,		// delays
	"Hellboar",			// name
	50,	50,	25,		// DLVL range & MLVL
	375,	445, // HP range,
	AI_19_Acid_Beast,	MF_9_CHASE_TARGET, 5,	// AI, int flag, int level
	16,	7,	20,	43,// tohit, hit frame of 1st attack, dmg range.
	37,	1,	50,	55,	 // 2nd attack: tohit, hit frame, dmg range
	32, 2, MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	455,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},//* 230	*/ { 128,	0,	"Monsters\\Acid\\Acid%c.CL2",	1,	"Monsters\\Acid\\Acid%c%i.WAV",	1,	1,	"X\\Mega_01.TRN",	13, 8, 12, 7, 16, 12,	0, 0, 0, 0, 0, 0,	"Acid Spitter",	40,	44,	35,	650,	1050,	AI_19,	MF_B9_PATH,	3,	30,	6,	46,	60,	30,	4,	28,	46,	73,	2,	MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,	0,	3,	875, 0, 0 }, 
{
	BM_231_BEASTMASTER_SUMMON_3,	0, 177,	256,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\siegebreaker\\siegebreaker_%c.CL2",	1,	"Monsters\\siegebreaker\\_%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	12, 9, 12, 6, 18, 13,		// IWAHDS frames
	2, 1, 1, 1, 2, 1,		// delays
	"Siege Beast",			// name
	0,	46,	50,		// DLVL range & MLVL
	10000,	10000, // HP range,
	AI_1_Overlord,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 9,	// AI, int flag, int level
	20,	7,	55,	60,// tohit, hit frame of 1st attack, dmg range.
	30,	9,	60,	65,	 // 2nd attack: tohit, hit frame, dmg range
	80, 2, MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		7,	2000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},//* 231	*/ { 160,	0,	"Monsters\\Snake\\Snake%c.CL2",	1,	"Monsters\\Snake\\Snake%c%i.WAV",	1,	1,	"X\\Snake_11x1green.trn",	12, 11, 9, 6, 18, 16,	2, 0, 0, 0, 1, 0,	"Viper",	50,	50,	37,	495,	660,	AI_24,	MF_B9_PATH,	4,	17,	8,	33,	53,	36,	9,	38,	60,	42,	2,	MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,	MR_1_ARCAN|MR_2_FIRE|MR_3_LIGHT|MR_7_IM_CURSE,	0,	7,	670, 0, 0 },
{
	BM_232_ABYSSAL_SUCCUBUS,	1, 109,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Goatmother\\1\\Onoskelis%c.CL2",	1,	"Monsters\\Goatmother\\1\\gmcaster_%c%i.WAV",		1,
	1,	"CF\\SH\\Grimspike.TRN",	// TRNs:   flag and path
	18, 10, 8, 6, 7, 8,		// IWAHDS frames
	2, 1, 0, 1, 2, 0,		// delays
	"Abyssal Succubus",			// name
	32,	38,	53,		// DLVL range & MLVL
	900,	1200, // HP range,
	AI_57_Hybrid_Arc_Star,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	20,	4,	30,	50,// tohit, hit frame of 1st attack, dmg range.
	35,	4,	44,	50,	 // 2nd attack: tohit, hit frame, dmg range
	69, 1, MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_11_IM_COLD,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT|MR_8_IM_ACID|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	700,		// special flag for drop, animation size, XP
	"CF\\iceage\\_GOATLORD_BLUE2.trn", 0 // ice age  TRN & name
},
{
	BM_233_BLOOD_LORD,	1, 110,	224,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Minotaur\\minotaur%c.CL2",	0,	"Monsters\\minotaur\\_%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	8, 8, 20, 6, 15, 0,		// IWAHDS frames
	2, 1, 0, 1, 2, 0,		// delays
	"Blood Lord",			// name
	32,	38,	50,		// DLVL range & MLVL
	1300,	1500, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	25,	8,	40,	55,// tohit, hit frame of 1st attack, dmg range.
	40,	8,	45,	60,	 // 2nd attack: tohit, hit frame, dmg range
	67, 2, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE| MR_8_IM_ACID|MR_11_IM_COLD,	MR_1_ARCAN|MR_5_IM_FIRE|MR_6_IM_LIGHT| MR_8_IM_ACID|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1290,		// special flag for drop, animation size, XP
	"X\\Minotaur_03_bllue_moonlord_v2.trn", "Moon Lord" // ice age  TRN & name
},
{
	BM_234_STEEL_LORD,	1, 113,	224,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Minotaur\\bloodlord%c.CL2",	1,	"Monsters\\minotaur\\_steellord_%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	8, 8, 20, 6, 15, 6,		// IWAHDS frames
	2, 1, 0, 1, 2, 10,		// delays
	"Steel Lord",			// name
	42,	46,	54,		// DLVL range & MLVL
	1500,	2000, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	40,	8,	50,	60,// tohit, hit frame of 1st attack, dmg range.
	55,	1,	55,	60,	 // 2nd attack: tohit, hit frame, dmg range
	79, 2, MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_8_IM_ACID|MR_11_IM_COLD,	MR_3_LIGHT|MR_4_IM_ARCAN|MR_5_IM_FIRE|MR_8_IM_ACID|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	2000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_235_STYGIAN_FURY,	1, 120,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Hell_witch\\HellWitch%c.CL2",	1,	"Monsters\\Hell_witch\\HellWitch%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	9, 9, 8, 5, 13, 9,		// IWAHDS frames
	1, 1, 0, 1, 1, 1,		// delays
	"Stygian Fury",			// name
	22,	26,	46,		// DLVL range & MLVL
	400,	470, // HP range,
	AI_51_Mixed_Arcane_Star,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	4,	5,	22,	33,// tohit, hit frame of 1st attack, dmg range.
	18,	6,	27,	47,	 // 2nd attack: tohit, hit frame, dmg range
	46, 1, MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,	MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	400,		// special flag for drop, animation size, XP
	"x\\_ORNG2CRIMZEE-BLUE.trn", 0 // ice age  TRN & name
},
{
	BM_236_CORPSE_EATER,	1, 104,	224,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Regurgitator\\Regurgitator_%c.CL2",	1,	"Monsters\\Regurgitator\\_regurgitator_%c%i.WAV",		1,
	1,	"Monsters\\Regurgitator\\Regurgitator_01.trn",	// TRNs:   flag and path
	8, 12, 16, 6, 20, 12,		// IWAHDS frames
	2, 1, 0, 1, 1, 1,		// delays
	"Corpse Eater",			// name
	32,	38,	51,		// DLVL range & MLVL
	2800,	3300, // HP range,
	AI_4_Scavenger,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	28,	9,	36,	44,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	57, 2, MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_11_IM_COLD,	MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1600,		// special flag for drop, animation size, XP
	0, "Corpse Devourer" // ice age  TRN & name
},
{
	BM_237_CONTAMINATOR,	1, 141,	224,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\putrid_defiler\\putriddefiler_%c.CL2",	1,	"Monsters\\putrid_defiler\\_demon_%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	9, 10, 10, 5, 17, 16,		// IWAHDS frames
	2, 1, 0, 1, 2, 1,		// delays
	"Contaminator",			// name
	32,	38,	51,		// DLVL range & MLVL
	1110,	1333, // HP range,
	AI_10_Skel_King,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	25,	5,	39,	42,// tohit, hit frame of 1st attack, dmg range.
	28,	12,	57,	65,	 // 2nd attack: tohit, hit frame, dmg range
	60, 0, MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,	MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1507,		// special flag for drop, animation size, XP
	0, "Reanimator" // ice age  TRN & name
},
{
	BM_238_TAINTED,	1, 114,	224,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\tainted\\tainted_%c.CL2",	1,	"Monsters\\tainted\\tainted_%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	8, 8, 8, 8, 19, 10,		// IWAHDS frames
	2, 1, 0, 1, 2, 1,		// delays
	"Tainted",			// name
	32,	38,	48,		// DLVL range & MLVL
	1200,	1500, // HP range,
	AI_52_Mixed_Snowbolt,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	34,	4,	38,	47,// tohit, hit frame of 1st attack, dmg range.
	34,	7,	52,	60,	 // 2nd attack: tohit, hit frame, dmg range
	60, 1, MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID|MR_11_IM_COLD,	MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT| MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	1059,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_239_AFFLICTED,	1, 114,	224,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\tainted\\tainted_%c.CL2",	1,	"Monsters\\tainted\\tainted_%c%i.WAV",		1,
	1,	"X\\Goat_06.TRN",	// TRNs:   flag and path
	8, 8, 8, 8, 19, 10,		// IWAHDS frames
	2, 1, 0, 1, 2, 1,		// delays
	"Afflicted",			// name
	40,	46,	54,		// DLVL range & MLVL
	1400,	1700, // HP range,
	AI_52_Mixed_Snowbolt,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	41,	4,	47,	57,// tohit, hit frame of 1st attack, dmg range.
	56,	7,	62,	66,	 // 2nd attack: tohit, hit frame, dmg range
	80, 1, MR_3_LIGHT | MR_5_IM_FIRE | MR_4_IM_ARCAN | MR_8_IM_ACID | MR_11_IM_COLD,	MR_3_LIGHT | MR_5_IM_FIRE | MR_4_IM_ARCAN | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	1690,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_240_DEMON_PRIEST,	1, 124,	224,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\council\\councilmember_%c.CL2",	1,	"Monsters\\council\\_%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	7, 11, 10, 4, 19, 14,		// IWAHDS frames
	2, 1, 0, 1, 2, 1,		// delays
	"Demon Priest",			// name
	24,	30,	46,		// DLVL range & MLVL
	600,	700, // HP range,
	AI_50_Mixed_Fireblast,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	13,	5,	30,	45,// tohit, hit frame of 1st attack, dmg range.
	23,	6,	40,	50,	 // 2nd attack: tohit, hit frame, dmg range
	80, 1, MR_3_LIGHT | MR_5_IM_FIRE | MR_4_IM_ARCAN | MR_8_IM_ACID,	MR_3_LIGHT | MR_5_IM_FIRE | MR_4_IM_ARCAN | MR_8_IM_ACID,// armor, mon type, and resistances
	0,		7,	979,		// special flag for drop, animation size, XP
	0, "Hell Priest" // ice age  TRN & name
},
{
	BM_241_ACID_TERROR,	1, 57,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Acid\\Acid%c.CL2",	1,	"Monsters\\Acid\\Acid%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	13, 8, 12, 8, 16, 12,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Acid Terror",			// name
	24,	30,	39,		// DLVL range & MLVL
	450,	600, // HP range,
	AI_19_Acid_Beast,	0, 3,	// AI, int flag, int level
	19,	8,	27,	46,// tohit, hit frame of 1st attack, dmg range.
	29,	8,	20,	33,	 // 2nd attack: tohit, hit frame, dmg range
	45, 2, MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID,	MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID,// armor, mon type, and resistances
	0,		3,	570,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_242_REANIMATED_HORROR,	1, 107,	256,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\reanimated\\ReanimatedHorde_%c.CL2",	1,	"Monsters\\reanimated\\reanimated_%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	11, 13, 12, 6, 14, 28,		// IWAHDS frames
	2, 0, 0, 0, 0, 2,		// delays
	"Reanimated Horror",			// name
	12,	16,	26,		// DLVL range & MLVL
	200,	250, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	10,	4,	20,	34,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	36,	40,	 // 2nd attack: tohit, hit frame, dmg range
	27, 0, MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	800,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_243_IZUAL,	0, 165,	288,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Izual\\Izual_%c.CL2",	1,	"Monsters\\Izual\\_Izual_%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	8, 7, 16, 6, 20, 16,		// IWAHDS frames
	2, 1, 1, 1, 3, 1,		// delays
	"Izual",			// name
	48,	48,	56,		// DLVL range & MLVL
	1000,	1000, // HP range,
	AI_26_Balrog,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	49,	9,	27,	46,// tohit, hit frame of 1st attack, dmg range.
	29,	9,	57,	66,	 // 2nd attack: tohit, hit frame, dmg range
	80, 1, MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID,	MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID,// armor, mon type, and resistances
	0,		7,	4000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_244_HORADRIM_ANCIENT,	0, 150,	224,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\radament\\radament_%c.CL2",	1,	"Monsters\\radament\\_%c%i.WAV",		1,
	1,	"X\\Whore_a_dream.trn",	// TRNs:   flag and path
	12, 18, 16, 6, 20, 16,		// IWAHDS frames
	2, 1, 1, 1, 2, 1,		// delays
	"Horadrim Ancient",			// name
	48,	48,	29,		// DLVL range & MLVL
	600,	700, // HP range,
	AI_10_Skel_King,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	12,	10,	31,	35,// tohit, hit frame of 1st attack, dmg range.
	22,	4,	36,	40,	 // 2nd attack: tohit, hit frame, dmg range
	30, 0, MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	600,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_245_BLUNDERER,	0, 191,	352,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\blunderbore\\blunderbore_%c.CL2",	1,	"Monsters\\Fat\\Fat%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	8, 8, 15, 7, 23, 15,		// IWAHDS frames
	2, 1, 1, 1, 2, 1,		// delays
	"Blunderer",			// name
	18,	18,	45,		// DLVL range & MLVL
	4000,	4000, // HP range,
	AI_1_Overlord,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	3,	31,	27,	46,// tohit, hit frame of 1st attack, dmg range.
	55,	2,	57,	66,	 // 2nd attack: tohit, hit frame, dmg range
	50, 2, MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,	MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	2000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_246_GREATER_MUMMY,	0, 150,	224,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Greater_Mummy\\greatermummy_%c.CL2",	1,	"Monsters\\radament\\_%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	12, 18, 16, 6, 20, 16,		// IWAHDS frames
	2, 1, 1, 1, 2, 1,		// delays
	"Greater Mummy",			// name
	48,	48,	36,		// DLVL range & MLVL
	1000,	1000, // HP range,
	AI_10_Skel_King,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	20,	10,	40,	45,// tohit, hit frame of 1st attack, dmg range.
	30,	3,	45,	55,	 // 2nd attack: tohit, hit frame, dmg range
	40, 0, MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_7_IM_CURSE|MR_11_IM_COLD,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT |MR_7_IM_CURSE | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1400,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_247_SIEGEBREAKER_BEAST,	0, 177,	256,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\siegebreaker\\siegebreaker_%c.CL2",	1,	"Monsters\\siegebreaker\\_%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	12, 9, 12, 6, 18, 13,		// IWAHDS frames
	2, 1, 1, 1, 2, 1,		// delays
	"Siegebreaker Beast",			// name
	48,	48,	58,		// DLVL range & MLVL
	10000,	10000, // HP range,
	AI_1_Overlord,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 9,	// AI, int flag, int level
	20,	7,	55,	60,// tohit, hit frame of 1st attack, dmg range.
	30,	9,	60,	65,	 // 2nd attack: tohit, hit frame, dmg range
	80, 2, MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_7_IM_CURSE | MR_11_IM_COLD,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_7_IM_CURSE | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	2000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_248_SALAMANDER,	1, 137,	288,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\salamander\\salamander_%c.CL2",	1,	"Monsters\\salamander\\_salamander_%c%i.WAV",		1,
	0,	0/*"Monsters\\salamander\\Salamander_01_two_toned.trn"*/,	// TRNs:   flag and path
	8, 3, 13, 5, 19, 14,		// IWAHDS frames
	2, 1, 1, 1, 2, 1,		// delays
	"Salamander",			// name
	32,	38,	50,		// DLVL range & MLVL
	800,	1100, // HP range,
	AI_1_Overlord,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	30,	7,	46,	53,// tohit, hit frame of 1st attack, dmg range.
	35,	8,	54,	59,	 // 2nd attack: tohit, hit frame, dmg range
	63, 2, MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_8_IM_ACID,	MR_2_FIRE|MR_4_IM_ARCAN|MR_6_IM_LIGHT|MR_8_IM_ACID,// armor, mon type, and resistances
	0,		3,	730,		// special flag for drop, animation size, XP
	"cf\\iceage\\_nonsense.trn", "Frost Stinger" // ice age  TRN & name
},
{
	BM_249_SCARAB,	1, 132,	256,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\scarab\\scarabdemon_%c.CL2",	1,	"Monsters\\scarab\\_%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	8, 14, 16, 6, 20, 14,		// IWAHDS frames
	2, 1, 1, 1, 1, 1,		// delays
	"Scarab",			// name
	12,	16,	24,		// DLVL range & MLVL
	320,	420, // HP range,
	AI_54_Mixed_Chargedbolt,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	15,	9,	25,	37,// tohit, hit frame of 1st attack, dmg range.
	25,	9,	38,	42,	 // 2nd attack: tohit, hit frame, dmg range
	27, 2, MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID| MR_11_IM_COLD,	MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	380,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_250_BONE_SCARAB,	1, 132,	256,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\scarab\\scarabdemon_%c.CL2",	1,	"Monsters\\scarab\\_%c%i.WAV",		1,
	1,	"CF\\SH\\XTJ.TRN",	// TRNs:   flag and path
	8, 14, 16, 6, 20, 14,		// IWAHDS frames
	2, 1, 1, 1, 1, 1,		// delays
	"Bone Scarab",			// name
	18,	22,	36,		// DLVL range & MLVL
	350,	450, // HP range,
	AI_54_Mixed_Chargedbolt,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	12,	9,	29,	42,// tohit, hit frame of 1st attack, dmg range.
	22,	9,	38,	48,	 // 2nd attack: tohit, hit frame, dmg range
	38, 2, MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,	MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	777,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_251_WENDIGO,	0, 146,	224,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\wendigo\\wendigo_%c.CL2",	1,	"Monsters\\wendigo\\_%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	8, 12, 12, 6, 18, 17,		// IWAHDS frames
	2, 1, 1, 1, 1, 1,		// delays
	"Wendigo",			// name
	48,	48,	36,		// DLVL range & MLVL
	950,	1250, // HP range,
	AI_1_Overlord,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 6,	// AI, int flag, int level
	12,	7,	28,	40,// tohit, hit frame of 1st attack, dmg range.
	22,	10,	52,	56,	 // 2nd attack: tohit, hit frame, dmg range
	34, 2, MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_7_IM_CURSE|MR_8_IM_ACID | MR_11_IM_COLD,	MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_7_IM_CURSE| MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	433,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_252_HEPHASTO,	0, 140,	224,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Smith\\_%c.CL2",	1,	"Monsters\\newsfx\\Biclop%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	12, 12, 14, 8, 20, 1,		// IWAHDS frames
	2, 0, 0, 0, 2, 0,		// delays
	"Hephasto",			// name
	48,	48,	28,		// DLVL range & MLVL
	666,	666, // HP range,
	AI_8_Fallen_One,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 5,	// AI, int flag, int level
	16,	7,	28,	45,// tohit, hit frame of 1st attack, dmg range.
	0,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	32, 1, MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_7_IM_CURSE | MR_11_IM_COLD,	MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_7_IM_CURSE | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	3000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_253_EXPLODER,	1, 130,	192,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\exploder\\_%c.CL2",	0,	"Monsters\\exploder\\_%c%i.WAV",		0,
	0,	0,	// TRNs:   flag and path
	8, 9, 19, 1, 19, 1,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Exploder",			// name
	34,	46,	44,		// DLVL range & MLVL
	500,	600, // HP range,
	AI_53_Exploder,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	16,	7,	28,	45,// tohit, hit frame of 1st attack, dmg range.
	0,	1,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	73, 2, MR_1_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	600,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_254_TWILIGHT_CLAN,	1, 106,	192,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Goat_Long_Mace\\GoatmanLongMace_%c.CL2",	1,	"Monsters\\Goat_Long_Mace\\_%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	10, 8, 20, 6, 19, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Twilight Clan",			// name
	30,	36,	52,		// DLVL range & MLVL
	700,	1000, // HP range,
	AI_6_Goat_Melee,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	19,	25,	43,	49,// tohit, hit frame of 1st attack, dmg range.
	29,	8,	59,	63,	 // 2nd attack: tohit, hit frame, dmg range
	66, 1, MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT|MR_8_IM_ACID,	MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE|MR_8_IM_ACID,// armor, mon type, and resistances
	0,		3,	1200,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_255_RAGE_CLAN,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatBow\\GoatB%c.CL2",	0,	"Monsters\\GoatBow\\GoatB%c%i.WAV",		0,
	1,	"X\\Goat_29_Rage_Clan.TRN",	// TRNs:   flag and path
	12, 8, 14, 5, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Rage Clan",			// name
	32,	38,	49,		// DLVL range & MLVL
	450,	550, // HP range,
	AI_7_Goat_Archer,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	25,	12,	55,	65,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	64, 1, MR_2_FIRE|MR_4_IM_ARCAN | MR_6_IM_LIGHT|MR_11_IM_COLD,	MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	800,		// special flag for drop, animation size, XP
	"X\\moonclan.TRN", "Sunset Clan" // ice age  TRN & name
},
{
	BM_256_RAGE_CLAN,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatBow\\GoatB%c.CL2",	0,	"Monsters\\GoatBow\\GoatB%c%i.WAV",		0,
	1,	"X\\Goat_29_Rage_Clan.TRN",	// TRNs:   flag and path
	12, 8, 14, 5, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Rage Clan",			// name
	40,	46,	52,		// DLVL range & MLVL
	600,	750, // HP range,
	AI_7_Goat_Archer,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	39,	12,	62,	65,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	77, 1, MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_11_IM_COLD,	MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	1226,		// special flag for drop, animation size, XP
	"X\\moonclan.TRN", "Midnight Clan" // ice age  TRN & name
},
{
	BM_257_SHROUDED,	1, 63,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Sneak\\Sneak%c.CL2",	1,	"Monsters\\Sneak\\Sneak%c%i.WAV",		0,
	1,	"X\\Hidden23_white2.TRN",	// TRNs:   flag and path
	16, 8, 12, 8, 24, 15,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Shrouded",			// name
	30,	36,	44,		// DLVL range & MLVL
	500,	700, // HP range,
	AI_15_Hidden,	MF_1_INVISIBLE | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	22,	7,	30,	45,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	57, 1, MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT| MR_8_IM_ACID|MR_11_IM_COLD,	MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT| MR_8_IM_ACID|MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	579,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_258_RAT_HORDE,	1, 68,	104,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Rat\\Rat%c.CL2",	1,	"Monsters\\newsfx\\TmbRat%c%i.WAV",		1,
	1,	"X\\_RAT_UNO2.trn",	// TRNs:   flag and path
	11, 8, 12, 6, 20, 11,		// IWAHDS frames
	2, 0, 0, 0, 2, 0,		// delays
	"Rat Horde",			// name
	4,	6,	10,		// DLVL range & MLVL
	80,	130, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	13,	8,	10,	17,// tohit, hit frame of 1st attack, dmg range.
	20,	7,	14,	21,	 // 2nd attack: tohit, hit frame, dmg range
	12, 2, MR_2_FIRE | MR_4_IM_ARCAN|MR_6_IM_LIGHT,	MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	160,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_259_WATER_WYRM,	0, 0,	382,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\wasserman\\sobsna_%c.CL2",	1,	"Monsters\\wasserman\\x_%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	13, 13/*placeholder*/, 15, 6, 18, 21,		// IWAHDS frames
	2, 1, 1, 1, 2, 1,		// delays
	"Water Wyrm",			// name
	18,	18,	35,		// DLVL range & MLVL
	1000,	1200, // HP range,
	AI_55_Wyrm,	0, 3,	// AI, int flag, int level
	25,	3,	35,	50,// tohit, hit frame of 1st attack, dmg range.
	55,	2,	57,	66,	 // 2nd attack: tohit, hit frame, dmg range
	45, 2, MR_3_LIGHT|MR_4_IM_ARCAN | MR_5_IM_FIRE|MR_8_IM_ACID,	MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_8_IM_ACID| MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1300,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_260_ARMAGEDDON_WALKER,	1, 0,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\snakb\\abi%c.CL2",	1,	"Monsters\\wendigo\\_%c%i.WAV",		1,
	1,	"X\\Abi_01_red_dish.trn",	// TRNs:   flag and path
	12, 16, 16, 12, 16, 16,		// IWAHDS frames
	2, 1, 1, 1, 2, 1,		// delays
	"Armageddon Walker",			// name
	32,	38,	56,		// DLVL range & MLVL
	2500,	2900, // HP range,
	AI_39_Mixed_White_Star_phys,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	20,	18/*hit frame is hardcoded at 8*/,	38,	49,// tohit, hit frame of 1st attack, dmg range.
	30,	7,	60,	75,	 // 2nd attack: tohit, hit frame, dmg range
	68, 1, MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,	MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1666,		// special flag for drop, animation size, XP
	"x\\__red2white.trn", 0 // ice age  TRN & name
},
{
	BM_261_SKELETAL_KNIGHT,	1, 120,	160,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\skelbigax\\_%c.CL2",	1,	"Monsters\\skelbigax\\SKnight%c%i.WAV",		1,
	1,	"X\\zug_blue.TRN",	// TRNs:   flag and path
	12, 16, 16, 12, 16, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Skeletal Knight",			// name
	32,	42,	54,		// DLVL range & MLVL
	1000,	1300, // HP range,
	AI_1_Overlord,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 6,	// AI, int flag, int level
	20,	18/*hit frame is hardcoded at 9*/,	37,	47,// tohit, hit frame of 1st attack, dmg range.
	30,	6,	50,	70,	 // 2nd attack: tohit, hit frame, dmg range
	70, 0, MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	1400,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_262_DCLAW,	0, 0,	124,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\dclaw\\dc_%c.CL2",	1,	"Monsters\\Thin\\Thin%c%i.WAV",		1,
	1,	"X\\Dclaw_01_crimzee.TRN",	// TRNs:   flag and path
	22, 24, 11, 5, 11, 16,		// IWAHDS frames
	1, 0, 0, 1, 2, 0,		// delays
	"DClaw Test",			// name
	0,	2,	2,		// DLVL range & MLVL
	35,	55, // HP range,
	AI_4_Scavenger,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	7,	6,	4,	12,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	3, 0, MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_7_IM_CURSE,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	90,		// special flag for drop, animation size, XP
	"X\\Gravdg03bluish.trn", "Ice Digger" // ice age  TRN & name
},
{
	BM_263_RED_WYRM,	0, 0,	382,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\wasserman\\sobsna_%c.CL2",	1,	"Monsters\\wasserman\\x_%c%i.WAV",		1,
	1,	"X\\Dclaw_01_crimzee.TRN",	// TRNs:   flag and path
	13, 13/*placeholder*/, 15, 6, 18, 21,		// IWAHDS frames
	2, 1, 1, 1, 2, 1,		// delays
	"Red Wyrm",			// name
	38,	38,	58,		// DLVL range & MLVL
	2000,	2400, // HP range,
	AI_55_Wyrm,	0, 3,	// AI, int flag, int level
	40,	3,	50,	65,// tohit, hit frame of 1st attack, dmg range.
	60,	2,	58,	66,	 // 2nd attack: tohit, hit frame, dmg range
	74, 2, MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE,	MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1800,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_264_BITELING,	0, 58,	164,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\spawn\\Spawn%c.CL2",	1,	"Monsters\\spawn\\Hspawn%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	15, 12, 14, 11, 14, 15,		// IWAHDS frames
	2, 1, 1, 1, 1, 1,		// delays
	"Biteling",			// name
	8,	8,	17,		// DLVL range & MLVL
	100,	120, // HP range,
	AI_2_Skel_Melee,	0, 3,	// AI, int flag, int level
	15,	6,	13,	20,// tohit, hit frame of 1st attack, dmg range.
	25,	6,	17,	23,	 // 2nd attack: tohit, hit frame, dmg range
	18, 2, MR_2_FIRE | MR_3_LIGHT | MR_4_IM_ARCAN,	MR_2_FIRE | MR_3_LIGHT | MR_4_IM_ARCAN,// armor, mon type, and resistances
	16384,		3,	200,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_265_CROC,	0, 131,	200,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\croc\\200w\\_%c.CL2",	1,	"Monsters\\Baboon\\_%c%i.wav",		1,
	1,	"X\\Croc01_darker.TRN",	// TRNs:   flag and path
	16, 14, 18, 6, 10, 16,		// IWAHDS frames
	1, 1, 1, 1, 1, 1,		// delays
	"Swamp Beast",			// name
	0,	0,	39,		// DLVL range & MLVL
	500,	650, // HP range,
	AI_19_Acid_Beast,	0, 3,	// AI, int flag, int level
	15,	22,	40,	50,// tohit, hit frame of 1st attack, dmg range.
	20,	6,	45,	55,	 // 2nd attack: tohit, hit frame, dmg range
	42, 2, MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID,	MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID,// armor, mon type, and resistances
	0,		7,	800,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_266_CROC_BERS,	0, 131,	200,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\croc\\200w\\brs\\_%c.CL2",	1,	"Monsters\\Croc\\croc_bers_%c%i.wav",		1,
	1,	"X\\Croc01_darker.TRN",	// TRNs:   flag and path
	16, 14, 18, 6, 10, 22,		// IWAHDS frames
	1, 1, 1, 1, 1, 2,		// delays
	"Swamp Beast",			// name
	0,	0,	40,		// DLVL range & MLVL
	550,	700, // HP range,
	AI_8_Fallen_One,	0, 3,	// AI, int flag, int level
	20,	22,	45,	55,// tohit, hit frame of 1st attack, dmg range.
	25,	8,	50,	60,	 // 2nd attack: tohit, hit frame, dmg range
	45, 2, MR_2_FIRE | MR_4_IM_ARCAN| MR_6_IM_LIGHT | MR_8_IM_ACID,	MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT| MR_8_IM_ACID,// armor, mon type, and resistances
	0,		7,	900,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_267_BONE_BLADE,	0, 172,	252,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\boneblade\\_%c.CL2",	1,	"Monsters\\newsfx\\SWingX%c%i.WAV",		1,
	0,	"X\\Croc01_darker.TRN",	// TRNs:   flag and path
	30, 23, 16, 11, 17, 16,		// IWAHDS frames
	1, 1, 1, 1, 1, 1,		// delays
	"Bone Blade",			// name
	32,	34,	55,		// DLVL range & MLVL
	1400,	1700, // HP range,
	AI_6_Goat_Melee,	0, 3,	// AI, int flag, int level
	19,	2,	47,	55,// tohit, hit frame of 1st attack, dmg range.
	34,	10,	64,	72,	 // 2nd attack: tohit, hit frame, dmg range
	61, 0, MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_7_IM_CURSE | MR_11_IM_COLD,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_7_IM_CURSE | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1200,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_268_BONE_BLADE_2,	0, 172,	252,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\boneblade\\bb2\\bb2_%c.CL2",	1,	"Monsters\\newsfx\\SWingX%c%i.WAV",		1,
	0,	"X\\Croc01_darker.TRN",	// TRNs:   flag and path
	30, 23, 15, 11, 17, 16,		// IWAHDS frames
	1, 1, 1, 1, 1, 1,		// delays
	"Bone Blade",			// name
	36,	38,	57,		// DLVL range & MLVL
	1800,	2000, // HP range,
	AI_1_Overlord,	0, 3,	// AI, int flag, int level
	19,	20,	50,	60,// tohit, hit frame of 1st attack, dmg range.
	49,	10,	77,	80,	 // 2nd attack: tohit, hit frame, dmg range
	66, 0, MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_7_IM_CURSE | MR_11_IM_COLD,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_7_IM_CURSE | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1500,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_269_BOG_CRAWLER,	0, 96,	148,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\bog_crawler\\voreworm_%c.CL2",	1,	"Monsters\\bog_crawler\\_%c%i.WAV",		1,
	0,	"X\\Croc01_darker.TRN",	// TRNs:   flag and path
	18, 10, 16, 6, 8, 9,		// IWAHDS frames
	1, 1, 1, 1, 1, 1,		// delays
	"Bog Crawler",			// name
	0,	0,	40,		// DLVL range & MLVL
	450,	500, // HP range,
	AI_15_Hidden,	MF_1_INVISIBLE | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	15,	9,	31,	44,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	38, 2, MR_3_LIGHT|MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_8_IM_ACID,	MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_8_IM_ACID,// armor, mon type, and resistances
	0,		7,	550,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_270_MUD_CRAWLER,	1, 96,	148,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\bog_crawler\\voreworm_%c.CL2",	1,	"Monsters\\bog_crawler\\_%c%i.WAV",		1,
	0,	"X\\Croc01_darker.TRN",	// TRNs:   flag and path
	18, 10, 16, 6, 8, 9,		// IWAHDS frames
	1, 1, 1, 1, 1, 1,		// delays
	"Mud Crawler",			// name
	32,	38,	52,		// DLVL range & MLVL
	450,	650, // HP range,
	AI_15_Hidden,	MF_1_INVISIBLE | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	25,	9,	36,	48,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	58, 2, MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_8_IM_ACID,	MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_8_IM_ACID,// armor, mon type, and resistances
	0,		7,	650,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_271_THORNED_HULK,	0, 160,	260,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\thorned_hulk\\TH_%c.CL2",	1,	"Monsters\\thorned_hulk\\_%c%i.WAV",		1,
	0,	"X\\Croc01_darker.TRN",	// TRNs:   flag and path
	12, 14, 10, 4, 23, 16,		// IWAHDS frames
	1, 1, 1, 1, 1, 1,		// delays
	"Thorned Hulk",			// name
	0,	38,	52,		// DLVL range & MLVL
	5000,	5500, // HP range,
	AI_2_Skel_Melee,	0, 2,	// AI, int flag, int level
	25,	26,	36,	48,// tohit, hit frame of 1st attack, dmg range.
	34,	9,	50,	60,	 // 2nd attack: tohit, hit frame, dmg range
	64, 1, MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_8_IM_ACID,	MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_8_IM_ACID,// armor, mon type, and resistances
	0,		7,	3500,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_272_SPIDER_QUEEN,	0, 110,	170,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\spider_queen\\SpQ_%c.CL2",	1,	"Monsters\\thorned_hulk\\_%c%i.WAV",		0,
	0,	"X\\Croc01_darker.TRN",	// TRNs:   flag and path
	15, 14/*14*/, 14, 4, 15, 10,		// IWAHDS frames
	1, 1, 1, 1, 1, 1,		// delays
	"Spider Queen",			// name
	0,	38,	52,		// DLVL range & MLVL
	3150,	3350, // HP range,
	AI_2_Skel_Melee,	0, 2,	// AI, int flag, int level
	25,	6,	36,	48,// tohit, hit frame of 1st attack, dmg range.
	34,	8,	50,	60,	 // 2nd attack: tohit, hit frame, dmg range
	64, 2, MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_8_IM_ACID,	MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_8_IM_ACID,// armor, mon type, and resistances
	0,		7,	3500,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_273_FLAME_FIEND,	0, 107,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\cof\\FEs_%c.CL2",	1,	"Monsters\\cof\\_%c%i.WAV",		1,
	0,	"X\\Croc01_darker.TRN",	// TRNs:   flag and path
	20, 10, 14, 3, 20, 14,		// IWAHDS frames
	1, 1, 1, 1, 1, 1,		// delays
	"Flame Fiend",			// name
	0,	0,	50,		// DLVL range & MLVL
	1150,	1250, // HP range,
	AI_50_Mixed_Fireblast,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	25,	5,	41,	58,// tohit, hit frame of 1st attack, dmg range.
	40,	6,	50,	60,	 // 2nd attack: tohit, hit frame, dmg range
	64, 1, MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_8_IM_ACID,	MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_8_IM_ACID,// armor, mon type, and resistances
	0,		7,	1250,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_274_FLESH_DEVOURER,	1, 122,	174,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\glabrezu\\1\\_%c.CL2",	1,	"Monsters\\glabrezu\\1\\1_%c%i.WAV",		1,
	1,	"X\\Glabrezu_01_darker.TRN",	// TRNs:   flag and path
	12, 16, 16, 12, 16, 16,		// IWAHDS frames
	1, 1, 1, 1, 1, 1,		// delays
	"Flesh Devourer",			// name
	32,	38,	50,		// DLVL range & MLVL
	1050,	1200, // HP range,
	AI_54_Mixed_Chargedbolt,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	25,	7,	39,	53,// tohit, hit frame of 1st attack, dmg range.
	40,	10,	48,	57,	 // 2nd attack: tohit, hit frame, dmg range
	65, 1, MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,	MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1200,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_275_BEHEMOTH,	0, 111,	170,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\chicken\\gronk_%c.CL2",	1,	"Monsters\\chicken\\%c%i.WAV",		1,
	1,	"X\\chicken_02_darker_m1.TRN",	// TRNs:   flag and path
	12, 16, 16, 12, 16, 16,		// IWAHDS frames
	1, 1, 1, 1, 1, 1,		// delays
	"Behemoth",			// name
	0,	0,	60,		// DLVL range & MLVL
	5500,	6000, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	25,	18,	45,	55,// tohit, hit frame of 1st attack, dmg range.
	40,	5,	48,	57,	 // 2nd attack: tohit, hit frame, dmg range
	65, 2, MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_7_IM_CURSE | MR_11_IM_COLD,	MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_7_IM_CURSE | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	4000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_276_SWORD_SPIDER,	0, 0,	126,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\spider\\bigg\\GiantSpider_%c.CL2",	1,	"Monsters\\spider\\bigg\\%c%i.WAV",		1,
	0,	"X\\chicken_02_darker_m1.TRN",	// TRNs:   flag and path
	10, 12, 10, 3, 13, 11,		// IWAHDS frames
	1, 1, 1, 1, 1, 1,		// delays
	"Sword Spider",			// name
	0,	0,	54,		// DLVL range & MLVL
	1500,	1800, // HP range,
	AI_10_Skel_King,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	25,	6,	30,	45,// tohit, hit frame of 1st attack, dmg range.
	40,	7,	48,	57,	 // 2nd attack: tohit, hit frame, dmg range
	61, 2, MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_8_IM_ACID,	MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_8_IM_ACID,// armor, mon type, and resistances
	0,		3,	800,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_277_SPIDERLING,	0, 47,	66,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\spider\\small\\SmallSpider_%c.CL2",	1,	"Monsters\\spider\\bigg\\%c%i.WAV",		1,
	0,	"X\\chicken_02_darker_m1.TRN",	// TRNs:   flag and path
	10, 12, 10, 3, 13, 11,		// IWAHDS frames
	1, 1, 1, 1, 1, 1,		// delays
	"Spiderling",			// name
	0,	0,	50,		// DLVL range & MLVL
	230,	360, // HP range,
	AI_6_Goat_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	25,	6,	12,	28,// tohit, hit frame of 1st attack, dmg range.
	40,	7,	28,	45,	 // 2nd attack: tohit, hit frame, dmg range
	58, 2, MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID,	MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID,// armor, mon type, and resistances
	16384,		3,	230,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_278_LEAPER,	1, 97,	186,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\boevoy_baran_boris\\trelon_%c.CL2",	1,	"Monsters\\boevoy_baran_boris\\%c%i.WAV",		1,
	0,	"Monsters\\GoatMace\\Gray.TRN",	// TRNs:   flag and path
	12, 16, 12, 8, 16, 12,		// IWAHDS frames
	1, 0, 0, 0, 1, 0,		// delays
	"Leaper",			// name
	32,	38,	54,		// DLVL range & MLVL
	700,	1200, // HP range,
	AI_56_Rammer,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	13,	7,	39,	53,// tohit, hit frame of 1st attack, dmg range.
	42,	7,	47,	61,	 // 2nd attack: tohit, hit frame, dmg range
	63, 2, MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_11_IM_COLD,	MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	666,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_279_FLOATER,	1, 0,	132,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\floater\\GrFeyr_%c.CL2",	1,	"Monsters\\floater\\SFX_uno\\%c%i.WAV",		1,
	0,	"X\\?.TRN",	// TRNs:   flag and path
	30, 17, 15, 10, 16, 15,		// IWAHDS frames
	1, 0, 0, 0, 1, 0,		// delays
	"Floater",			// name
	32,	34,	52,		// DLVL range & MLVL
	500,	600, // HP range,
	AI_58_Mixed_Psychic_Bite,	0, 1,	// AI, int flag, int level
	19,	3,	37,	49,// tohit, hit frame of 1st attack, dmg range.
	242,	8,	60,	70,	 // 2nd attack: tohit, hit frame, dmg range
	58, 2, MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_11_IM_COLD,	MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	650,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_280_FLOATER_X2,	1, 0,	132,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\floater\\X2\\GrFeyr_%c.CL2",	1,	"Monsters\\floater\\SFX_dos\\%c%i.WAV",		1,
	0,	"X\\?.TRN",	// TRNs:   flag and path
	15, 17, 15, 10, 16, 15,		// IWAHDS frames
	1, 0, 0, 0, 1, 0,		// delays
	"Floater",			// name
	36,	38,	54,		// DLVL range & MLVL
	550,	650, // HP range,
	AI_58_Mixed_Psychic_Bite,	0, 2,	// AI, int flag, int level
	23,	8,	41,	53,// tohit, hit frame of 1st attack, dmg range.
	342,	8,	75,	85,	 // 2nd attack: tohit, hit frame, dmg range
	63, 2, MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID,	MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	700,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_281_FALLEN_BOW,	1, 0,	98,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Falbow\\%c.CL2",	1,	"Monsters\\Falbow\\%c%i.WAV",		1,
	1,	"monsters\\falbow\\Goblin_01_Red.TRN",	// TRNs:   flag and path
	12, 15, 15, 10, 16, 15,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Fallen One",			// name
	2,	4,	6,		// DLVL range & MLVL
	24,	48, // HP range,
	AI_3_Skel_Archer,	0, 1,	// AI, int flag, int level
	10,	10,	6,	9,// tohit, hit frame of 1st attack, dmg range.
	20,	11,	10,	15,	 // 2nd attack: tohit, hit frame, dmg range
	7, 1, MR_2_FIRE | MR_3_LIGHT | MR_4_IM_ARCAN,	MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_8_IM_ACID,// armor, mon type, and resistances
	0,		3,	110,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_282_GREMLIN_BOW,	1, 0,	98,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Falbow\\%c.CL2",	1,	"Monsters\\Falbow\\%c%i.WAV",		1,
	0,	"monsters\\falbow\\Goblin_01_Red.TRN",	// TRNs:   flag and path
	12, 15, 15, 10, 16, 15,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Green One",			// name
	6,	10,	14,		// DLVL range & MLVL
	60,	95, // HP range,
	AI_3_Skel_Archer,	0, 2,	// AI, int flag, int level
	16,	10,	16,	20,// tohit, hit frame of 1st attack, dmg range.
	25,	11,	20,	33,	 // 2nd attack: tohit, hit frame, dmg range
	15, 1, MR_3_LIGHT | MR_5_IM_FIRE | MR_4_IM_ARCAN,	MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_8_IM_ACID,// armor, mon type, and resistances
	0,		3,	190,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_283_FROST_WARDEN,	0, 0,	124,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Frostie\\ice_golem_%c.CL2",	1,	"Monsters\\Frostie\\_%c%i.WAV",		1,
	0,	"monsters\\falbow\\Goblin_01_Red.TRN",	// TRNs:   flag and path
	30, 20, 15, 8, 15, 15,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Frost Warden",			// name
	0,	10,	30,		// DLVL range & MLVL
	1200,	1500, // HP range,
	AI_2_Skel_Melee,	0, 2,	// AI, int flag, int level
	16,	9,	33,	42,// tohit, hit frame of 1st attack, dmg range.
	26,	9,	37,	48,	 // 2nd attack: tohit, hit frame, dmg range
	35, 0, MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_4_IM_ARCAN | MR_11_IM_COLD,	MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_4_IM_ARCAN | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	500,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_284_SIN_SPAWN,	1, 0,	128,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Succ\\Scbs%c.CL2",	0,	"Monsters\\Succ\\Scbs%c%i.WAV",		0,
	1,	"X\\Sucub_11_crimzon.TRN",	// TRNs:   flag and path
	14, 8, 16, 7, 24, 0,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Sin Spawn",			// name
	22,	26,	39,		// DLVL range & MLVL
	250,	425, // HP range,
	AI_14_Witch,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	25,	10,	27,	47,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	43, 1, MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID,	MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	603,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_285_PHANTOM_KNIGHT,	0, 0,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Black\\Black%c.CL2",	1,	"Monsters\\Black\\Black%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	8, 8, 16, 4, 24, 4,		// IWAHDS frames
	3, 0, 0, 0, 0, 3,		// delays
	"Phantom Knight",			// name
	0,	0,	44,		// DLVL range & MLVL
	1,	2, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	30,	8,	1,	2,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	50, 0, MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_11_IM_COLD,	MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	2500,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_286_BRINE_CORPSE,	1, 102,	120,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\brine_corpse\\drowned_dead_%c.CL2",	1,	"Monsters\\brine_corpse\\_%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	30, 20, 15, 11, 31, 15,		// IWAHDS frames
	1, 0, 0, 0, 2, 0,		// delays
	"Brine Corpse",			// name
	12,	20,	28,		// DLVL range & MLVL
	800,	960, // HP range,
	AI_1_Overlord,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	28,	8,	27,	36,// tohit, hit frame of 1st attack, dmg range.
	38,	9,	37,	45,	 // 2nd attack: tohit, hit frame, dmg range
	31, 0, MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,	MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_7_IM_CURSE | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	779,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_287_ARMORED_SKELETON,	1, 124,	150,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\armored_skeleton\\armrd_skel_m_%c.CL2",	1,	"Monsters\\armored_skeleton\\_%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	30, 18, 15, 11, 21, 15,		// IWAHDS frames
	1, 0, 0, 0, 1, 0,		// delays
	"Armored Skeleton",			// name
	12,	16,	25,		// DLVL range & MLVL
	650,	750, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	20,	7,	26,	35,// tohit, hit frame of 1st attack, dmg range.
	30,	7,	35,	41,	 // 2nd attack: tohit, hit frame, dmg range
	29, 0, MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	600,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_288_EARTH_GOLEM,	1, 90,	122,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\earth_golem\\e_elmntl_%c.CL2",	1,	"Monsters\\earth_golem\\%c%i.WAV",		0,
	0,	0,	// TRNs:   flag and path
	15, 20, 15, 9, 14, 14,		// IWAHDS frames
	1, 0, 0, 0, 1, 0,		// delays
	"Earth Golem",			// name
	16,	22,	33,		// DLVL range & MLVL
	777,	888, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	17,	9,	31,	39,// tohit, hit frame of 1st attack, dmg range.
	27,	9,	41,	49,	 // 2nd attack: tohit, hit frame, dmg range
	37, 0, MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	750,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_289_FUNGAL_ROOT,	0, 0,	90,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\fungal_root\\shrieker_%c.CL2",	0,	"Monsters\\fungal_root\\%c%i.WAV",		0,
	0,	0,	// TRNs:   flag and path
	1, 0, 15, 7, 30, 0,		// IWAHDS frames
	1, 0, 0, 0, 1, 0,		// delays
	"Fungal Root",			// name
	0,	0,	33,		// DLVL range & MLVL
	251,	350, // HP range,
	AI_49_Ranger_Fireblast,	0, 3,	// AI, int flag, int level
	17,	15,	31,	39,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	33, 2, MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_8_IM_ACID | MR_11_IM_COLD,	MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	570,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_290_FUNGAL_ROOT_LITE,	0, 0,	90,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\fungal_root\\shrieker_%c.CL2",	0,	"Monsters\\fungal_root\\%c%i.WAV",		0,
	0,	0,	// TRNs:   flag and path
	1, 0, 15, 7, 30, 0,		// IWAHDS frames
	1, 0, 0, 0, 1, 0,		// delays
	"Fungal Root",			// name
	0,	0,	33,		// DLVL range & MLVL
	251,	350, // HP range,
	AI_25_Advocate,	0, 2,	// AI, int flag, int level
	17,	15,	31,	39,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	33, 2, MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_8_IM_ACID | MR_11_IM_COLD,	MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	620,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_291_MYCONID,	0, 0,	94,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\myconid\\myconid_%c.CL2",	1,	"Monsters\\myconid\\%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	15, 18, 14, 7, 19, 14,		// IWAHDS frames
	1, 0, 0, 0, 1, 0,		// delays
	"Myconid",			// name
	0,	0,	32,		// DLVL range & MLVL
	401,	500, // HP range,
	AI_40_Bonewing_cold_blasts,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	15,	6,	23,	31,// tohit, hit frame of 1st attack, dmg range.
	24,	6,	30,	40,	 // 2nd attack: tohit, hit frame, dmg range
	32, 2, MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,	MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	365,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_292_BUGBEAR,	0, 154,	204,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\bugbear\\bgbr_cptn_%c.CL2",	1,	"Monsters\\bugbear\\%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	15, 16, 15, 10, 26, 15,		// IWAHDS frames
	2, 0, 0, 0, 2, 0,		// delays
	"Bugbear",			// name
	0,	0,	40,		// DLVL range & MLVL
	5,	9, // HP range,
	AI_1_Overlord,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 7,	// AI, int flag, int level
	18,	6,	1,	2,// tohit, hit frame of 1st attack, dmg range.
	28,	6,	3,	4,	 // 2nd attack: tohit, hit frame, dmg range
	46, 2, MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,	MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_293_PIT_FIEND,	0, 160,	218,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\pit_fiend\\pit_fiend_%c.CL2",	1,	"Monsters\\pit_fiend\\%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	33, 10, 14, 7, 20, 20,		// IWAHDS frames
	1, 0, 0, 0, 2, 0,		// delays
	"Pit Fiend",			// name
	0,	40,	40,		// DLVL range & MLVL
	5,	9, // HP range,
	AI_9_Magma,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	18,	5,	60,	70,// tohit, hit frame of 1st attack, dmg range.
	28,	19,	80,	90,	 // 2nd attack: tohit, hit frame, dmg range
	46, 2, MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,	MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_294_BELHIFET_UNO,	0, 130,	200,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\belhi\\1\\belhifet_%c.CL2",	1,	"Monsters\\pit_fiend\\%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	30, 17, 15, 14, 61, 30,		// IWAHDS frames
	1, 0, 0, 0, 1, 0,		// delays
	"Belhifet Uno",			// name
	0,	0,	42,		// DLVL range & MLVL
	900,	900, // HP range,
	AI_9_Magma,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	27,	16,	60,	70,// tohit, hit frame of 1st attack, dmg range.
	37,	15,	30,	50,	 // 2nd attack: tohit, hit frame, dmg range
	55, 1, MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,	MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_295_BELHIFET_DOS,	0, 130,	200,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\belhi\\2\\belhifet_%c.CL2",	1,	"Monsters\\pit_fiend\\%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	30, 17, 15, 14, 61, 17,		// IWAHDS frames
	1, 0, 0, 0, 1, 0,		// delays
	"Belhifet Dos",			// name
	0,	0,	42,		// DLVL range & MLVL
	900,	900, // HP range,
	AI_9_Magma,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	27,	2,	60,	70,// tohit, hit frame of 1st attack, dmg range.
	37,	7,	30,	50,	 // 2nd attack: tohit, hit frame, dmg range
	55, 1, MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,	MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_8_IM_ACID | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	1000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_296_ICE_TROLL,	0, 98,	170,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\ICE_TROLL\\ice_troll_%c.CL2",	1,	"Monsters\\ice_troll\\sfx\\%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	29, 20, 15, 13, 31, 15,		// IWAHDS frames
	1, 0, 0, 0, 1, 0,		// delays
	"Ice Troll",			// name
	0,	40,	31,		// DLVL range & MLVL
	375,	425, // HP range,
	AI_40_Bonewing_cold_blasts,	MF_9_CHASE_TARGET, 2,	// AI, int flag, int level
	12,	8,	30,	43,// tohit, hit frame of 1st attack, dmg range.
	16,	7,	31,	47,	 // 2nd attack: tohit, hit frame, dmg range
	35, 1, MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,	MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		3,	345,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_297_OROG,	0, 149,	230,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\orog\\orog_chieftan_%c.CL2",	1,	"Monsters\\Fat\\Fat%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	14, 18, 14, 10, 13, 14,		// IWAHDS frames
	1, 0, 0, 0, 2, 0,		// delays
	"Borosteron",			// name
	0,	40,	52,		// DLVL range & MLVL
	10,	10, // HP range,
	AI_40_Bonewing_cold_blasts,	MF_9_CHASE_TARGET, 2,	// AI, int flag, int level
	12,	9,	30,	43,// tohit, hit frame of 1st attack, dmg range.
	16,	5,	61,	77,	 // 2nd attack: tohit, hit frame, dmg range
	75, 2, MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,	MR_1_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	3000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_298_TENTACLIST_BOSS,	0, 127,	236,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\tentaclist\\X_%c.CL2",	1,	"Monsters\\tentaclist\\_%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	13, 12, 11, 5, 14, 15,		// IWAHDS frames
	1, 0, 0, 0, 2, 0,		// delays
	"Tentaclist Boss",			// name
	0,	0,	55,		// DLVL range & MLVL
	2000,	3200, // HP range,
	AI_59_Tentaclist,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	22,	7,	56,	62,// tohit, hit frame of 1st attack, dmg range.
	32,	9,	56,	60,	 // 2nd attack: tohit, hit frame, dmg range
	65, 2, MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	2000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_299_TENTACLIST2,	1, 117,	201,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\tentaclist\\85_death_mauler_%c.CL2",	1,	"Monsters\\tentaclist\\_%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	13, 12, 11, 5, 14, 15,		// IWAHDS frames
	1, 0, 0, 0, 2, 0,		// delays
	"Tentaclist",			// name
	20,	30,	41,		// DLVL range & MLVL
	425,	575, // HP range,
	AI_59_Tentaclist,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	14,	7,	44,	49,// tohit, hit frame of 1st attack, dmg range.
	24,	9,	34,	48,	 // 2nd attack: tohit, hit frame, dmg range
	47, 2, MR_2_FIRE | MR_3_LIGHT | MR_4_IM_ARCAN | MR_11_IM_COLD,	MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	600,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_300_TENTACLIST_ABYSS,	1, 117,	201,	750,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\tentaclist\\85_death_mauler_%c.CL2",	1,	"Monsters\\tentaclist\\_%c%i.WAV",		1,
	1,	"X\\_RAT_UNO2.trn",	// TRNs:   flag and path
	13, 12, 11, 5, 14, 15,		// IWAHDS frames
	1, 0, 0, 0, 2, 0,		// delays
	"Abyssal Tentaclist",			// name
	32,	38,	53,		// DLVL range & MLVL
	675,	900, // HP range,
	AI_59_Tentaclist,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	17,	7,	50,	54,// tohit, hit frame of 1st attack, dmg range.
	27,	9,	46,	54,	 // 2nd attack: tohit, hit frame, dmg range
	53, 2, MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_11_IM_COLD,	MR_3_LIGHT | MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	800,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
//---------------------------------------------------------------------
//----------------------------------CLASSIC----------------------------
// Zombies
{
	BM_C_301_ZOMBIE,	2, 0,	128,	799,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\zombie\\zombie%c.cl2",	0,	"Monsters\\zombie\\zombie%c%i.WAV",		0,
	0,	"Monsters\\Zombie\\Bluered.TRN",	// TRNs:   flag and path
	11, 24, 12, 6, 16, 12,		// IWAHDS frames
	4, 0, 0, 0, 0, 3,		// delays
	"Zombie",			// name
	0,	2,	1,		// DLVL range & MLVL
	4,	7, // HP range,
	AI_0_Zombie,	MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	10,	8,	2,	5,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	5, 0, MR_2_FIRE | MR_3_LIGHT | MR_4_IM_ARCAN,	MR_2_FIRE | MR_3_LIGHT | MR_4_IM_ARCAN,// armor, mon type, and resistances
	0,		3,	54,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_302_GHOUL,	2, 0,	128,	799,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\zombie\\zombie%c.cl2",	0,	"Monsters\\zombie\\zombie%c%i.WAV",		0,
	1,	"Monsters\\Zombie\\ghoul.TRN",	// TRNs:   flag and path
	11, 24, 12, 6, 16, 12,		// IWAHDS frames
	4, 0, 0, 0, 0, 3,		// delays
	"Ghoul",			// name
	2,	4,	2,		// DLVL range & MLVL
	7,	11, // HP range,
	AI_0_Zombie,	MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	10,	8,	3,	10,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	10, 0, MR_2_FIRE | MR_3_LIGHT | MR_4_IM_ARCAN,	MR_2_FIRE | MR_3_LIGHT | MR_4_IM_ARCAN,// armor, mon type, and resistances
	0,		3,	58,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_303_ROTTING_CARCASS,	2, 0,	128,	799,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\zombie\\zombie%c.cl2",	0,	"Monsters\\zombie\\zombie%c%i.WAV",		0,
	1,	"Monsters\\Zombie\\grey.TRN",	// TRNs:   flag and path
	11, 24, 12, 6, 16, 12,		// IWAHDS frames
	4, 0, 0, 0, 0, 3,		// delays
	"Rotting Carcass",			// name
	2,	6,	4,		// DLVL range & MLVL
	15,	25, // HP range,
	AI_0_Zombie,	MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	25,	8,	5,	15,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	15, 0, MR_2_FIRE | MR_3_LIGHT | MR_4_IM_ARCAN,	MR_2_FIRE | MR_3_LIGHT | MR_4_IM_ARCAN,// armor, mon type, and resistances
	0,		3,	136,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_304_BLACK_DEATH,	2, 0,	128,	799,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\zombie\\zombie%c.cl2",	0,	"Monsters\\zombie\\zombie%c%i.WAV",		0,
	1,	"Monsters\\Zombie\\yellow.TRN",	// TRNs:   flag and path
	11, 24, 12, 6, 16, 12,		// IWAHDS frames
	4, 0, 0, 0, 0, 3,		// delays
	"Black Death",			// name
	4,	8,	6,		// DLVL range & MLVL
	25,	30, // HP range,
	AI_0_Zombie,	MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	30,	8,	6,	22,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	20, 0, MR_2_FIRE | MR_3_LIGHT | MR_4_IM_ARCAN,	MR_2_FIRE | MR_3_LIGHT | MR_4_IM_ARCAN,// armor, mon type, and resistances
	0,		3,	240,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
// Fallen - spear
{
	BM_C_305_FALLEN_ONE,	2, 60,	128,	543,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FalSpear\\Phall%c.CL2",	1,	"Monsters\\FalSpear\\Phall%c%i.WAV",		1,
	1,	"Monsters\\FalSpear\\FallenT.TRN",	// TRNs:   flag and path
	11, 11, 13, 11, 18, 13,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Fallen One",			// name
	0,	4,	1,		// DLVL range & MLVL
	1,	4, // HP range,
	AI_8_Fallen_One,	0, 0,	// AI, int flag, int level
	15,	8,	1,	3,// tohit, hit frame of 1st attack, dmg range.
	0,	5,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	0, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	46,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_306_CARVER,	2, 60,	128,	543,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FalSpear\\Phall%c.CL2",	1,	"Monsters\\FalSpear\\Phall%c%i.WAV",		1,
	1,	"Monsters\\FalSpear\\Dark_ORIG.TRN",	// TRNs:   flag and path
	11, 11, 13, 11, 18, 13,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Carver",			// name
	2,	4,	3,		// DLVL range & MLVL
	4,	8, // HP range,
	AI_8_Fallen_One,	0, 1,	// AI, int flag, int level
	20,	8,	2,	5,// tohit, hit frame of 1st attack, dmg range.
	0,	5,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	5, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	80,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_307_DEVIL_KIN,	2, 60,	128,	543,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FalSpear\\Phall%c.CL2",	1,	"Monsters\\FalSpear\\Phall%c%i.WAV",		1,
	0,	"Monsters\\FalSpear\\FallenT.TRN",	// TRNs:   flag and path
	11, 11, 13, 11, 18, 13,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Devil Kin",			// name
	2,	6,	5,		// DLVL range & MLVL
	12,	24, // HP range,
	AI_8_Fallen_One,	0, 2,	// AI, int flag, int level
	25,	8,	3,	7,// tohit, hit frame of 1st attack, dmg range.
	0,	5,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	10, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	155,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_308_DARK_ONE,	2, 60,	128,	543,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FalSpear\\Phall%c.CL2",	1,	"Monsters\\FalSpear\\Phall%c%i.WAV",		1,
	1,	"Monsters\\FalSpear\\Blue.TRN",	// TRNs:   flag and path
	11, 11, 13, 11, 18, 13,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Dark One",			// name
	4,	8,	7,		// DLVL range & MLVL
	20,	36, // HP range,
	AI_8_Fallen_One,	0, 3,	// AI, int flag, int level
	30,	8,	4,	8,// tohit, hit frame of 1st attack, dmg range.
	0,	5,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	15, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	255,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
// Fallen - sword
{
	BM_C_309_FALLEN_ONE,	2, 60,	128,	543,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FalSword\\Fall%c.CL2",	1,	"Monsters\\FalSpear\\Phall%c%i.WAV",		1,
	1,	"Monsters\\FalSpear\\FallenT.TRN",	// TRNs:   flag and path
	12, 12, 13, 11, 14, 15,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Fallen One",			// name
	0,	4,	1,		// DLVL range & MLVL
	2,	5, // HP range,
	AI_8_Fallen_One,	0, 0,	// AI, int flag, int level
	15,	8,	1,	4,// tohit, hit frame of 1st attack, dmg range.
	0,	5,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	10, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	52,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_310_CARVER,	2, 60,	128,	543,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FalSword\\Fall%c.CL2",	1,	"Monsters\\FalSpear\\Phall%c%i.WAV",		1,
	1,	"Monsters\\FalSpear\\Dark_ORIG.TRN",	// TRNs:   flag and path
	12, 12, 13, 11, 14, 15,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Carver",			// name
	2,	4,	3,		// DLVL range & MLVL
	5,	9, // HP range,
	AI_8_Fallen_One,	0, 1,	// AI, int flag, int level
	20,	8,	2,	7,// tohit, hit frame of 1st attack, dmg range.
	0,	5,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	15, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	90,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_311_DEVIL_KIN,	2, 60,	128,	543,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FalSword\\Fall%c.CL2",	1,	"Monsters\\FalSpear\\Phall%c%i.WAV",		1,
	0,	"Monsters\\FalSpear\\FallenT.TRN",	// TRNs:   flag and path
	12, 12, 13, 11, 14, 15,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Devil Kin",			// name
	2,	6,	5,		// DLVL range & MLVL
	16,	24, // HP range,
	AI_8_Fallen_One,	0, 2,	// AI, int flag, int level
	25,	8,	4,	10,// tohit, hit frame of 1st attack, dmg range.
	0,	5,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	20, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	180,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_312_DARK_ONE,	2, 60,	128,	543,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FalSword\\Fall%c.CL2",	1,	"Monsters\\FalSpear\\Phall%c%i.WAV",		1,
	1,	"Monsters\\FalSpear\\Blue.TRN",	// TRNs:   flag and path
	12, 12, 13, 11, 14, 15,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Dark One",			// name
	4,	8,	7,		// DLVL range & MLVL
	24,	36, // HP range,
	AI_8_Fallen_One,	0, 3,	// AI, int flag, int level
	30,	8,	4,	12,// tohit, hit frame of 1st attack, dmg range.
	0,	5,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	25, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	280,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
// Mages
{
	BM_C_313_COUNSELOR,	2, 0,	128,	2000,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Mage\\Mage%c.cl2",	1,	"Monsters\\Mage\\Mage%c%i.wav",		1,
	0,	"X\\M1.trn",	// TRNs:   flag and path
	12, 1, 20, 8, 28, 20,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Counselor",			// name
	22,	24,	25,		// DLVL range & MLVL
	65,	75, // HP range,
	AI_25_Advocate,	/*MF_9_CHASE_TARGET | */MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	90,	9,	8,	20,// tohit, hit frame of 1st attack, dmg range.
	35,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	35, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	3876,		// special flag for drop, animation size, XP
	"x\\Mage_01_crypta.trn", 0 // ice age  TRN & name
},
{
	BM_C_314_MAGISTRATE,	2, 0,	128,	2000,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Mage\\Mage%c.cl2",	1,	"Monsters\\Mage\\Mage%c%i.wav",		1,
	1,	"Monsters\\Mage\\Cnselg.TRN",	// TRNs:   flag and path
	12, 1, 20, 8, 28, 20,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Magistrate",			// name
	24,	26,	27,		// DLVL range & MLVL
	80,	90, // HP range,
	AI_25_Advocate,	/*MF_9_CHASE_TARGET | */MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	100,	9,	10,	24,// tohit, hit frame of 1st attack, dmg range.
	35,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	40, 1, MR_1_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	4478,		// special flag for drop, animation size, XP
	"x\\Mage_01_crypta.trn", 0 // ice age  TRN & name
},
{
	BM_C_315_CABALIST,	2, 0,	128,	2000,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Mage\\Mage%c.cl2",	1,	"Monsters\\Mage\\Mage%c%i.wav",		1,
	1,	"Monsters\\Mage\\Cnselgd.TRN",	// TRNs:   flag and path
	12, 1, 20, 8, 28, 20,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Cabalist",			// name
	26,	28,	29,		// DLVL range & MLVL
	115,	125, // HP range,
	AI_25_Advocate,	/*MF_9_CHASE_TARGET | */MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	110,	9,	14,	40,// tohit, hit frame of 1st attack, dmg range.
	35,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	45, 1, MR_1_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	4929,		// special flag for drop, animation size, XP
	"x\\Mage_01_crypta.trn", 0 // ice age  TRN & name
},
{
	BM_C_316_ADVOCATE,	2, 0,	128,	2000,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Mage\\Mage%c.cl2",	1,	"Monsters\\Mage\\Mage%c%i.wav",		1,
	1,	"Monsters\\Mage\\Cnselbk.TRN",	// TRNs:   flag and path
	12, 1, 20, 8, 28, 20,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Advocate",			// name
	28,	30,	30,		// DLVL range & MLVL
	140,	150, // HP range,
	AI_25_Advocate,	/*MF_9_CHASE_TARGET | */MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	120,	9,	15,	25,// tohit, hit frame of 1st attack, dmg range.
	35,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	50, 1, MR_4_IM_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	4968,		// special flag for drop, animation size, XP
	"x\\Mage_01_crypta.trn", 0 // ice age  TRN & name
},
// Knights
{
	BM_C_317_BLACK_KNIGHT,	2, 0,	160,	2120,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Black\\Black%c.CL2",	1,	"Monsters\\Black\\Black%c%i.WAV",		0,
	0,	"X\\hgd.TRN",	// TRNs:   flag and path
	8, 8, 16, 4, 24, 0,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Black Knight",			// name
	22,	26,	24,		// DLVL range & MLVL
	150,	150, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	100,	8,	15,	20,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	70, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	3360,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_318_DOOM_GUARD,	2, 0,	160,	2120,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Black\\Black%c.CL2",	1,	"Monsters\\Black\\Black%c%i.WAV",		0,
	1,	"Monsters\\Black\\BlkKntRT.TRN",	// TRNs:   flag and path
	8, 8, 16, 4, 24, 0,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Doom Guard",			// name
	24,	28,	26,		// DLVL range & MLVL
	165,	165, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	110,	8,	18,	25,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	75, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	3650,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_319_STEEL_LORD,	2, 0,	160,	2120,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Black\\Black%c.CL2",	1,	"Monsters\\Black\\Black%c%i.WAV",		0,
	1,	"Monsters\\Black\\BlkKntBT.TRN",	// TRNs:   flag and path
	8, 8, 16, 4, 24, 0,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Steel Lord",			// name
	26,	28,	28,		// DLVL range & MLVL
	180,	180, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	120,	8,	21,	30,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	80, 1, MR_1_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	4252,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_320_BLOOD_KNIGHT,	2, 0,	160,	2120,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Black\\Black%c.CL2",	1,	"Monsters\\Black\\Black%c%i.WAV",		0,
	1,	"Monsters\\Black\\Blood.TRN",	// TRNs:   flag and path
	8, 8, 16, 4, 24, 0,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Blood Knight",			// name
	26,	30,	30,		// DLVL range & MLVL
	200,	200, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	130,	8,	25,	35,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	85, 1, MR_4_IM_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	5130,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
// Witches
{
	BM_C_321_SUCCUBUS,	2, 0,	128,	980,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Succ\\Scbs%c.CL2",	0,	"Monsters\\Succ\\Scbs%c%i.WAV",		0,
	0,	"Monsters\\Acid\\AcidR.TRN",	// TRNs:   flag and path
	14, 8, 16, 7, 24, 0,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Succubus",			// name
	22,	26,	24,		// DLVL range & MLVL
	120,	150, // HP range,
	AI_14_Witch,	/*MF_9_CHASE_TARGET | */MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	100,	10,	1,	20,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	60, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	3700,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_322_SNOW_WITCH,	2, 0,	128,	980,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Succ\\Scbs%c.CL2",	0,	"Monsters\\Succ\\Scbs%c%i.WAV",		0,
	1,	"Monsters\\Succ\\Succb.TRN",	// TRNs:   flag and path
	14, 8, 16, 7, 24, 0,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Snow Witch",			// name
	24,	28,	26,		// DLVL range & MLVL
	135,	175, // HP range,
	AI_14_Witch,	/*MF_9_CHASE_TARGET | */MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	110,	10,	1,	24,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	65, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	4000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_323_HELL_SPAWN,	2, 0,	128,	980,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Succ\\Scbs%c.CL2",	0,	"Monsters\\Succ\\Scbs%c%i.WAV",		0,
	1,	"Monsters\\Succ\\Succrw.TRN",	// TRNs:   flag and path
	14, 8, 16, 7, 24, 0,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Hell Spawn",			// name
	26,	28,	28,		// DLVL range & MLVL
	150,	200, // HP range,
	AI_14_Witch,	/*MF_9_CHASE_TARGET | */MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	115,	10,	1,	30,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	70, 1, MR_1_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	4500,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_324_SOUL_BURNER,	2, 0,	128,	980,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Succ\\Scbs%c.CL2",	0,	"Monsters\\Succ\\Scbs%c%i.WAV",		0,
	1,	"Monsters\\Succ\\Succbw.TRN",	// TRNs:   flag and path
	14, 8, 16, 7, 24, 0,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Soul Burner",			// name
	28,	30,	30,		// DLVL range & MLVL
	140,	225, // HP range,
	AI_14_Witch,	/*MF_9_CHASE_TARGET | */MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	120,	10,	1,	35,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	75, 1, MR_1_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	4650,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
// Overlords
{
	BM_C_325_OVERLORD,	2, 0,	128,	1130,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Fat\\Fat%c.CL2",	1,	"Monsters\\Fat\\Fat%c%i.WAV",		1,
	0,	"X\\Fat_05new.TRN",	// TRNs:   flag and path
	8, 10, 15, 6, 16, 10,		// IWAHDS frames
	4, 0, 0, 0, 0, 0,		// delays
	"Overlord",			// name
	8,	12,	10,		// DLVL range & MLVL
	60,	80, // HP range,
	AI_1_Overlord,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	55,	7,	6,	12,// tohit, hit frame of 1st attack, dmg range.
	55,	6,	6,	13,	 // 2nd attack: tohit, hit frame, dmg range
	55, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	635,		// special flag for drop, animation size, XP
	"X\\Fat_09_iceage_mudman_darkened_blue.trn", 0 // ice age  TRN & name
},
{
	BM_C_326_MUD_MAN,	2, 0,	128,	1130,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Fat\\Fat%c.CL2",	1,	"Monsters\\Fat\\Fat%c%i.WAV",		1,
	1,	"Monsters\\Fat\\Blue.TRN",	// TRNs:   flag and path
	8, 10, 15, 6, 16, 10,		// IWAHDS frames
	4, 0, 0, 0, 0, 0,		// delays
	"Mud Man",			// name
	12,	16,	14,		// DLVL range & MLVL
	100,	125, // HP range,
	AI_1_Overlord,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	60,	7,	8,	16,// tohit, hit frame of 1st attack, dmg range.
	60,	6,	8,	17,	 // 2nd attack: tohit, hit frame, dmg range
	60, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		3,	1165,		// special flag for drop, animation size, XP
	"X\\Fat_09_iceage_mudman_darkened_blue.trn", 0 // ice age  TRN & name
},
{
	BM_C_327_TOAD_DEMON,	2, 0,	128,	1130,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Fat\\Fat%c.CL2",	1,	"Monsters\\Fat\\Fat%c%i.WAV",		1,
	1,	"Monsters\\Fat\\FatB.TRN",	// TRNs:   flag and path
	8, 10, 15, 6, 16, 10,		// IWAHDS frames
	4, 0, 0, 0, 0, 0,		// delays
	"Toad Demon",			// name
	14,	18,	16,		// DLVL range & MLVL
	135,	160, // HP range,
	AI_1_Overlord,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	70,	7,	9,	17,// tohit, hit frame of 1st attack, dmg range.
	70,	6,	10,	19,	 // 2nd attack: tohit, hit frame, dmg range
	65, 1, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	1380,		// special flag for drop, animation size, XP
	"X\\Fat_09_iceage_mudman_darkened_blue.trn", 0 // ice age  TRN & name
},
{
	BM_C_328_FLAYED_ONE,	2, 0,	128,	1130,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Fat\\Fat%c.CL2",	1,	"Monsters\\Fat\\Fat%c%i.WAV",		1,
	1,	"Monsters\\Fat\\FatF.TRN",	// TRNs:   flag and path
	8, 10, 15, 6, 16, 10,		// IWAHDS frames
	4, 0, 0, 0, 0, 0,		// delays
	"Flayed One",			// name
	18,	22,	20,		// DLVL range & MLVL
	160,	200, // HP range,
	AI_1_Overlord,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	85,	7,	10,	20,// tohit, hit frame of 1st attack, dmg range.
	85,	6,	11,	21,	 // 2nd attack: tohit, hit frame, dmg range
	70, 1, MR_1_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	2058,		// special flag for drop, animation size, XP
	"X\\Fat_09_iceage_mudman_darkened_blue.trn", 0 // ice age  TRN & name
},
// Skeleton - axes
{
	BM_C_329_SKELETON,	2, 0,	128,	553,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelAxe\\SklAx%c.CL2",	1,	"Monsters\\SkelAxe\\SklAx%c%i.WAV",		0,
	1,	"Monsters\\SkelAxe\\White.TRN",	// TRNs:   flag and path
	12, 8, 13, 6, 17, 16,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Skeleton",			// name
	0,	2,	1,		// DLVL range & MLVL
	2,	4, // HP range,
	AI_2_Skel_Melee,	MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	20,	8,	1,	4,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	0, 0, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	64,		// special flag for drop, animation size, XP
	"X\\SKing006-blue.trn", 0 // ice age  TRN & name
},
{
	BM_C_330_CORPSE_AXE,	2, 0,	128,	553,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelAxe\\SklAx%c.CL2",	1,	"Monsters\\SkelAxe\\SklAx%c%i.WAV",		0,
	1,	"Monsters\\SkelAxe\\Skelt.TRN",	// TRNs:   flag and path
	12, 8, 13, 6, 17, 16,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Corpse Axe",			// name
	2,	4,	2,		// DLVL range & MLVL
	4,	7, // HP range,
	AI_2_Skel_Melee,	MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	25,	8,	3,	5,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	0, 0, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	68,		// special flag for drop, animation size, XP
	"X\\SKing006-blue.trn", 0 // ice age  TRN & name
},
{
	BM_C_331_BURNING_DEAD,	2, 0,	128,	553,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelAxe\\SklAx%c.CL2",	1,	"Monsters\\SkelAxe\\SklAx%c%i.WAV",		0,
	0,	"X\\M1.trn",	// TRNs:   flag and path
	12, 8, 13, 6, 17, 16,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Burning Dead",			// name
	2,	6,	4,		// DLVL range & MLVL
	8,	12, // HP range,
	AI_2_Skel_Melee,	MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	30,	8,	3,	7,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	5, 0, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	154,		// special flag for drop, animation size, XP
	"X\\SKing006-blue.trn", 0 // ice age  TRN & name
},
{
	BM_C_332_HORROR,	2, 0,	128,	553,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelAxe\\SklAx%c.CL2",	1,	"Monsters\\SkelAxe\\SklAx%c%i.WAV",		0,
	1,	"Monsters\\SkelAxe\\Black.TRN",	// TRNs:   flag and path
	12, 8, 13, 6, 17, 16,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Horror",			// name
	4,	8,	6,		// DLVL range & MLVL
	12,	20, // HP range,
	AI_2_Skel_Melee,	MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	35,	8,	4,	9,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	15, 0, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	264,		// special flag for drop, animation size, XP
	"X\\SKing006-blue.trn", 0 // ice age  TRN & name
},
// Skeleton - bow
{
	BM_C_333_SKELETON_ARCHER,	2, 0,	128,	567,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelBow\\SklBw%c.CL2",	1,	"Monsters\\SkelBow\\SklBw%c%i.WAV",		0,
	1,	"Monsters\\SkelAxe\\White.TRN",	// TRNs:   flag and path
	9, 8, 16, 5, 16, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Skeleton Archer",			// name
	2,	4,	3,		// DLVL range & MLVL
	2,	4, // HP range,
	AI_3_Skel_Archer,	0, 0,	// AI, int flag, int level
	15,	12,	1,	2,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	0, 0, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	110,		// special flag for drop, animation size, XP
	"X\\SKing006-blue.trn", 0 // ice age  TRN & name
},
{
	BM_C_334_CORPSE_BOW,	2, 0,	128,	567,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelBow\\SklBw%c.CL2",	1,	"Monsters\\SkelBow\\SklBw%c%i.WAV",		0,
	1,	"Monsters\\SkelAxe\\Skelt.TRN",	// TRNs:   flag and path
	9, 8, 16, 5, 16, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Corpse Bow",			// name
	2,	6,	5,		// DLVL range & MLVL
	8,	16, // HP range,
	AI_3_Skel_Archer,	0, 1,	// AI, int flag, int level
	25,	12,	1,	4,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	0, 0, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	210,		// special flag for drop, animation size, XP
	"X\\SKing006-blue.trn", 0 // ice age  TRN & name
},
{
	BM_C_335_BURNING_DEAD_ARCHER,	2, 0,	128,	567,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelBow\\SklBw%c.CL2",	1,	"Monsters\\SkelBow\\SklBw%c%i.WAV",		0,
	0,	"X\\M1.trn",	// TRNs:   flag and path
	9, 8, 16, 5, 16, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Burning Dead Archer",			// name
	4,	8,	7,		// DLVL range & MLVL
	10,	24, // HP range,
	AI_3_Skel_Archer,	0, 2,	// AI, int flag, int level
	30,	12,	1,	6,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	5, 0, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	356,		// special flag for drop, animation size, XP
	"X\\SKing006-blue.trn", 0 // ice age  TRN & name
},
{
	BM_C_336_HORROR_ARCHER,	2, 0,	128,	567,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelBow\\SklBw%c.CL2",	1,	"Monsters\\SkelBow\\SklBw%c%i.WAV",		0,
	1,	"Monsters\\SkelAxe\\Black.TRN",	// TRNs:   flag and path
	9, 8, 16, 5, 16, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Horror Archer",			// name
	6,	10,	9,		// DLVL range & MLVL
	15,	45, // HP range,
	AI_3_Skel_Archer,	0, 3,	// AI, int flag, int level
	35,	12,	2,	9,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	15, 0, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	594,		// special flag for drop, animation size, XP
	"X\\SKing006-blue.trn", 0 // ice age  TRN & name
},
// Skeleton - captain
{
	BM_C_337_SKELETON_CAPTAIN,	2, 0,	128,	575,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelSd\\SklSr%c.CL2",	1,	"Monsters\\SkelSd\\SklSr%c%i.WAV",		1,
	1,	"Monsters\\SkelAxe\\White.TRN",	// TRNs:   flag and path
	13, 8, 12, 7, 15, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Skeleton Captain",			// name
	0,	4,	2,		// DLVL range & MLVL
	3,	6, // HP range,
	AI_2_Skel_Melee,	0, 0,	// AI, int flag, int level
	20,	8,	2,	7,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	5, 0, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	90,		// special flag for drop, animation size, XP
	"X\\SKing006-blue.trn", 0 // ice age  TRN & name
},
{
	BM_C_338_CORPSE_CAPTAIN,	2, 0,	128,	575,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelSd\\SklSr%c.CL2",	1,	"Monsters\\SkelSd\\SklSr%c%i.WAV",		1,
	1,	"Monsters\\SkelAxe\\Skelt.TRN",	// TRNs:   flag and path
	13, 8, 12, 7, 15, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Corpse Captain",			// name
	2,	6,	4,		// DLVL range & MLVL
	12,	20, // HP range,
	AI_2_Skel_Melee,	0, 1,	// AI, int flag, int level
	30,	8,	3,	9,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	10, 0, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	200,		// special flag for drop, animation size, XP
	"X\\SKing006-blue.trn", 0 // ice age  TRN & name
},
{
	BM_C_339_BURNING_DEAD_CAPTAIN,	2, 0,	128,	575,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelSd\\SklSr%c.CL2",	1,	"Monsters\\SkelSd\\SklSr%c%i.WAV",		1,
	0,	"X\\M1.trn",	// TRNs:   flag and path
	13, 8, 12, 7, 15, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Burning Dead Captain",			// name
	4,	8,	6,		// DLVL range & MLVL
	16,	30, // HP range,
	AI_2_Skel_Melee,	0, 2,	// AI, int flag, int level
	35,	8,	4,	10,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	15, 0, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	400,		// special flag for drop, animation size, XP
	"X\\SKing006-blue.trn", 0 // ice age  TRN & name
},
{
	BM_C_340_HORROR_CAPTAIN,	2, 0,	128,	575,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SkelSd\\SklSr%c.CL2",	1,	"Monsters\\SkelSd\\SklSr%c%i.WAV",		1,
	1,	"Monsters\\SkelAxe\\Black.TRN",	// TRNs:   flag and path
	13, 8, 12, 7, 15, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Horror Captain",			// name
	6,	10,	8,		// DLVL range & MLVL
	35,	50, // HP range,
	AI_2_Skel_Melee,	0, 3,	// AI, int flag, int level
	40,	8,	5,	14,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	30, 0, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	600,		// special flag for drop, animation size, XP
	"X\\SKing006-blue.trn", 0 // ice age  TRN & name
},
// Hidden
{
	BM_C_341_HIDDEN,	2, 63,	128,	992,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Sneak\\Sneak%c.CL2",	1,	"Monsters\\Sneak\\Sneak%c%i.WAV",		0,
	0,	"Monsters\\Fat\\FatB.TRN",	// TRNs:   flag and path
	16, 8, 12, 8, 24, 15,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Hidden",			// name
	2,	8,	5,		// DLVL range & MLVL
	8,	24, // HP range,
	AI_15_Hidden,	MF_1_INVISIBLE | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	35,	7,	3,	6,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	25, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	278,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_342_STALKER,	2, 63,	128,	992,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Sneak\\Sneak%c.CL2",	1,	"Monsters\\Sneak\\Sneak%c%i.WAV",		0,
	1,	"Monsters\\Sneak\\Sneakv2.TRN",	// TRNs:   flag and path
	16, 8, 12, 8, 24, 15,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Stalker",			// name
	8,	12,	9,		// DLVL range & MLVL
	30,	45, // HP range,
	AI_15_Hidden,	MF_1_INVISIBLE | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	40,	7,	8,	16,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	30, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	630,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_343_UNSEEN,	2, 63,	128,	992,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Sneak\\Sneak%c.CL2",	1,	"Monsters\\Sneak\\Sneak%c%i.WAV",		0,
	1,	"Monsters\\Sneak\\Sneakv3.TRN",	// TRNs:   flag and path
	16, 8, 12, 8, 24, 15,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Unseen",			// name
	10,	14,	11,		// DLVL range & MLVL
	35,	50, // HP range,
	AI_15_Hidden,	MF_1_INVISIBLE | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	45,	7,	12,	20,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	30, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	935,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_344_ILLUSION_WEAVER,	2, 63,	128,	992,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Sneak\\Sneak%c.CL2",	1,	"Monsters\\Sneak\\Sneak%c%i.WAV",		0,
	1,	"Monsters\\Sneak\\weeeeeewaaaaa.trn",	// TRNs:   flag and path
	16, 8, 12, 8, 24, 15,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Illusion Weaver",			// name
	14,	18,	13,		// DLVL range & MLVL
	40,	60, // HP range,
	AI_15_Hidden,	MF_1_INVISIBLE | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	60,	7,	16,	24,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	30, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	1500,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
// Hornies
{
	BM_C_345_HORNED_DEMON,	2, 98,	160,	1630,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Rhino\\Rhino%c.CL2",	1,	"Monsters\\Rhino\\Rhino%c%i.WAV",		1,
	0,	"Monsters\\GoatMace\\Red.TRN",	// TRNs:   flag and path
	8, 8, 14, 6, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Horned Demon",			// name
	12,	16,	13,		// DLVL range & MLVL
	40,	80, // HP range,
	AI_5_Charger,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	60,	7,	2,	16,// tohit, hit frame of 1st attack, dmg range.
	90,	0,	4,	32,	 // 2nd attack: tohit, hit frame, dmg range
	40, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	1172,		// special flag for drop, animation size, XP
	"X\\Rhino015_darker_orange.trn", 0 // ice age  TRN & name
},
{
	BM_C_346_MUD_RUNNER,	2, 98,	160,	1630,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Rhino\\Rhino%c.CL2",	1,	"Monsters\\Rhino\\Rhino%c%i.WAV",		1,
	1,	"Monsters\\Rhino\\Orange.TRN",	// TRNs:   flag and path
	8, 8, 14, 6, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Mud Runner",			// name
	14,	18,	15,		// DLVL range & MLVL
	50,	90, // HP range,
	AI_5_Charger,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	70,	7,	6,	18,// tohit, hit frame of 1st attack, dmg range.
	90,	0,	12,	36,	 // 2nd attack: tohit, hit frame, dmg range
	45, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	1404,		// special flag for drop, animation size, XP
	"X\\Rhino015_darker_orange.trn", 0 // ice age  TRN & name
},
{
	BM_C_347_FROST_CHARGER,	2, 98,	160,	1630,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Rhino\\Rhino%c.CL2",	1,	"Monsters\\Rhino\\Rhino%c%i.WAV",		1,
	1,	"Monsters\\Rhino\\Blue.TRN",	// TRNs:   flag and path
	8, 8, 14, 6, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Frost Charger",			// name
	16,	20,	17,		// DLVL range & MLVL
	60,	100, // HP range,
	AI_5_Charger,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	80,	7,	8,	20,// tohit, hit frame of 1st attack, dmg range.
	90,	0,	20,	40,	 // 2nd attack: tohit, hit frame, dmg range
	50, 2, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	1721,		// special flag for drop, animation size, XP
	"X\\Rhino015_darker_orange.trn", 0 // ice age  TRN & name
},
{
	BM_C_348_OBSIDIAN_LORD,	2, 98,	160,	1630,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Rhino\\Rhino%c.CL2",	1,	"Monsters\\Rhino\\Rhino%c%i.WAV",		1,
	1,	"Monsters\\Rhino\\RhinoB.TRN",	// TRNs:   flag and path
	8, 8, 14, 6, 16, 6,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Obsidian Lord",			// name
	18,	22,	19,		// DLVL range & MLVL
	70,	110, // HP range,
	AI_5_Charger,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	90,	7,	10,	22,// tohit, hit frame of 1st attack, dmg range.
	90,	0,	20,	50,	 // 2nd attack: tohit, hit frame, dmg range
	55, 2, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	1809,		// special flag for drop, animation size, XP
	"X\\Rhino015_darker_orange.trn", 0 // ice age  TRN & name
},
// Gargoyles
{
	BM_C_349_WINGED_DEMON,	2, 0,	160,	1650,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Gargoyle\\Gargo%c.CL2",	1,	"Monsters\\Gargoyle\\Gargo%c%i.WAV",		0,
	0,	"Monsters\\Gargoyle\\GargBr.TRN",	// TRNs:   flag and path
	14, 14, 14, 10, 18, 14,		// IWAHDS frames
	0, 0, 0, 0, 0, 2,		// delays
	"Winged Demon",			// name
	8,	12,	9,		// DLVL range & MLVL
	45,	60, // HP range,
	AI_12_Gargoyle,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	50,	7,	10,	16,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	45, 1, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT | MR_7_IM_CURSE,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT | MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		6,	662,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_350_GARGOYLE,	2, 0,	160,	1650,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Gargoyle\\Gargo%c.CL2",	1,	"Monsters\\Gargoyle\\Gargo%c%i.WAV",		0,
	1,	"Monsters\\Gargoyle\\GarE.TRN",	// TRNs:   flag and path
	14, 14, 14, 10, 18, 14,		// IWAHDS frames
	0, 0, 0, 0, 0, 2,		// delays
	"Gargoyle",			// name
	12,	16,	13,		// DLVL range & MLVL
	60,	90, // HP range,
	AI_12_Gargoyle,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	65,	7,	12,	19,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	45, 1, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT | MR_7_IM_CURSE,	MR_4_IM_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT | MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		6,	1200,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_351_BLOOD_CLAW,	2, 0,	160,	1650,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Gargoyle\\Gargo%c.CL2",	1,	"Monsters\\Gargoyle\\Gargo%c%i.WAV",		0,
	1,	"Monsters\\Gargoyle\\GargBr.TRN",	// TRNs:   flag and path
	14, 14, 14, 10, 18, 14,		// IWAHDS frames
	0, 0, 0, 0, 0, 2,		// delays
	"Blood Claw",			// name
	16,	20,	19,		// DLVL range & MLVL
	75,	125, // HP range,
	AI_12_Gargoyle,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	80,	7,	14,	22,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	50, 1, MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT | MR_7_IM_CURSE,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT | MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		6,	1870,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_352_DEATH_WING,	2, 0,	160,	1650,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Gargoyle\\Gargo%c.CL2",	1,	"Monsters\\Gargoyle\\Gargo%c%i.WAV",		0,
	1,	"Monsters\\Gargoyle\\GargB.TRN",	// TRNs:   flag and path
	14, 14, 14, 10, 18, 14,		// IWAHDS frames
	0, 0, 0, 0, 0, 2,		// delays
	"Death Wing",			// name
	18,	22,	23,		// DLVL range & MLVL
	90,	150, // HP range,
	AI_12_Gargoyle,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	95,	7,	16,	28,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	60, 1, MR_4_IM_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT | MR_7_IM_CURSE,	MR_4_IM_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT | MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		6,	2270,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
// Goats
{
	BM_C_353_FLESH_CLAN,	2, 0,	128,	1030,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatMace\\Goat%c.CL2",	1,	"Monsters\\GoatMace\\Goat%c%i.WAV",		1,
	0,	"Monsters\\Snake\\SnakR.TRN",	// TRNs:   flag and path
	12, 8, 12, 6, 20, 12,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Flesh Clan",			// name
	6,	10,	8,		// DLVL range & MLVL
	30,	45, // HP range,
	AI_6_Goat_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	50,	8,	4,	10,// tohit, hit frame of 1st attack, dmg range.
	55,	7,	6,	12,	 // 2nd attack: tohit, hit frame, dmg range
	40, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	460,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_354_STONE_CLAN,	2, 0,	128,	1030,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatMace\\Goat%c.CL2",	1,	"Monsters\\GoatMace\\Goat%c%i.WAV",		1,
	1,	"Monsters\\GoatMace\\Beige.TRN",	// TRNs:   flag and path
	12, 8, 12, 6, 20, 12,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Stone Clan",			// name
	8,	12,	10,		// DLVL range & MLVL
	40,	55, // HP range,
	AI_6_Goat_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	60,	8,	6,	12,// tohit, hit frame of 1st attack, dmg range.
	65,	7,	8,	14,	 // 2nd attack: tohit, hit frame, dmg range
	40, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	685,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_355_FIRE_CLAN,	2, 0,	128,	1030,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatMace\\Goat%c.CL2",	1,	"Monsters\\GoatMace\\Goat%c%i.WAV",		1,
	1,	"Monsters\\GoatMace\\Red.TRN",	// TRNs:   flag and path
	12, 8, 12, 6, 20, 12,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Fire Clan",			// name
	10,	14,	12,		// DLVL range & MLVL
	50,	65, // HP range,
	AI_6_Goat_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	70,	8,	8,	16,// tohit, hit frame of 1st attack, dmg range.
	75,	7,	10,	18,	 // 2nd attack: tohit, hit frame, dmg range
	45, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	906,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_356_NIGHT_CLAN,	2, 0,	128,	1030,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatMace\\Goat%c.CL2",	1,	"Monsters\\GoatMace\\Goat%c%i.WAV",		1,
	1,	"Monsters\\GoatMace\\Gray.TRN",	// TRNs:   flag and path
	12, 8, 12, 6, 20, 12,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Night Clan",			// name
	12,	16,	14,		// DLVL range & MLVL
	55,	70, // HP range,
	AI_6_Goat_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	80,	8,	10,	20,// tohit, hit frame of 1st attack, dmg range.
	85,	7,	12,	22,	 // 2nd attack: tohit, hit frame, dmg range
	50, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	1190,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_357_FLESH_CLAN_ARCHER,	2, 0,	128,	1040,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatBow\\GoatB%c.CL2",	0,	"Monsters\\GoatBow\\GoatB%c%i.WAV",		0,
	0,	"Monsters\\GoatMace\\Gray.TRN",	// TRNs:   flag and path
	12, 8, 16, 6, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Flesh Clan Archer",			// name
	6,	10,	8,		// DLVL range & MLVL
	20,	35, // HP range,
	AI_7_Goat_Archer,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	35,	13,	1,	7,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	35, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	448,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_358_STONE_CLAN_ARCHER,	2, 0,	128,	1040,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatBow\\GoatB%c.CL2",	0,	"Monsters\\GoatBow\\GoatB%c%i.WAV",		0,
	1,	"Monsters\\GoatMace\\Beige.TRN",	// TRNs:   flag and path
	12, 8, 16, 6, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Stone Clan Archer",			// name
	8,	12,	10,		// DLVL range & MLVL
	30,	40, // HP range,
	AI_7_Goat_Archer,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	40,	13,	2,	9,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	35, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	645,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_359_FIRE_CLAN_ARCHER,	2, 0,	128,	1040,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatBow\\GoatB%c.CL2",	0,	"Monsters\\GoatBow\\GoatB%c%i.WAV",		0,
	1,	"Monsters\\GoatMace\\Red.TRN",	// TRNs:   flag and path
	12, 8, 16, 6, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Fire Clan Archer",			// name
	10,	14,	12,		// DLVL range & MLVL
	40,	50, // HP range,
	AI_7_Goat_Archer,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	45,	13,	3,	11,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	35, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	822,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_360_NIGHT_CLAN_ARCHER,	2, 0,	128,	1040,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\GoatBow\\GoatB%c.CL2",	0,	"Monsters\\GoatBow\\GoatB%c%i.WAV",		0,
	1,	"Monsters\\GoatMace\\Gray.TRN",	// TRNs:   flag and path
	12, 8, 16, 6, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 0, 0,		// delays
	"Night Clan Archer",			// name
	12,	16,	14,		// DLVL range & MLVL
	50,	65, // HP range,
	AI_7_Goat_Archer,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	50,	13,	4,	13,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	40, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	1092,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
// Storms
{
	BM_C_361_RED_STORM,	2, 0,	160,	1740,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Thin\\Thin%c.CL2",	1,	"Monsters\\Thin\\Thin%c%i.WAV",		1,
	1,	"Monsters\\Thin\\Thinv3.TRN",	// TRNs:   flag and path
	8, 8, 18, 4, 17, 14,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Red Storm",			// name
	16,	18,	18,		// DLVL range & MLVL
	55,	110, // HP range,
	AI_16_Thunder_Demon,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	80,	5,	8,	18,// tohit, hit frame of 1st attack, dmg range.
	19,	8,	48,	60,	 // 2nd attack: tohit, hit frame, dmg range
	30, 1, MR_1_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	2160,		// special flag for drop, animation size, XP
	"x\\Thin_18_white_v2.trn", 0 // ice age  TRN & name
},
{
	BM_C_362_STORM_RIDER,	2, 0,	160,	1740,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Thin\\Thin%c.CL2",	1,	"Monsters\\Thin\\Thin%c%i.WAV",		1,
	0,	"X\\Thin_13.TRN",	// TRNs:   flag and path
	8, 8, 18, 4, 17, 14,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Storm Demon",			// name
	18,	22,	20,		// DLVL range & MLVL
	60,	120, // HP range,
	AI_16_Thunder_Demon,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	80,	5,	10,	20,// tohit, hit frame of 1st attack, dmg range.
	19,	8,	48,	60,	 // 2nd attack: tohit, hit frame, dmg range
	30, 1, MR_1_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	2391,		// special flag for drop, animation size, XP
	"x\\Thin_18_white_v2.trn", 0 // ice age  TRN & name
},
{
	BM_C_363_STORM_LORD,	2, 0,	160,	1740,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Thin\\Thin%c.CL2",	1,	"Monsters\\Thin\\Thin%c%i.WAV",		1,
	1,	"Monsters\\Thin\\Thinv2.TRN",	// TRNs:   flag and path
	8, 8, 18, 4, 17, 14,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Storm Lord",			// name
	20,	24,	22,		// DLVL range & MLVL
	75,	135, // HP range,
	AI_16_Thunder_Demon,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	85,	5,	12,	24,// tohit, hit frame of 1st attack, dmg range.
	19,	8,	48,	60,	 // 2nd attack: tohit, hit frame, dmg range
	35, 1, MR_1_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	2775,		// special flag for drop, animation size, XP
	"x\\Thin_18_white_v2.trn", 0 // ice age  TRN & name
},
{
	BM_C_364_MAELSTORM,	2, 0,	160,	1740,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Thin\\Thin%c.CL2",	1,	"Monsters\\Thin\\Thin%c%i.WAV",		1,
	1,	"Monsters\\Thin\\Thinv1.TRN",	// TRNs:   flag and path
	8, 8, 18, 4, 17, 14,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Maelstrom",			// name
	22,	26,	24,		// DLVL range & MLVL
	90,	150, // HP range,
	AI_16_Thunder_Demon,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	90,	5,	12,	28,// tohit, hit frame of 1st attack, dmg range.
	19,	8,	48,	60,	 // 2nd attack: tohit, hit frame, dmg range
	40, 1, MR_1_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	3177,		// special flag for drop, animation size, XP
	"x\\Thin_18_white_v2.trn", 0 // ice age  TRN & name
},
// Magmas
{
	BM_C_365_MAGMA_DEMON,	2, 0,	128,	1680,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Magma\\Magma%c.CL2",	1,	"Monsters\\Magma\\Magma%c%i.WAV",		1,
	0,	"X\\W2.trn",	// TRNs:   flag and path
	8, 10, 14, 7, 18, 18,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Magma Demon",			// name
	14,	16,	13,		// DLVL range & MLVL
	50,	70, // HP range,
	AI_9_Magma,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	45,	4,	4,	10,// tohit, hit frame of 1st attack, dmg range.
	26,	14,	17,	32,	 // 2nd attack: tohit, hit frame, dmg range
	45, 1, MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	1076,		// special flag for drop, animation size, XP
	"X\\Magma_11_frozen_stone.trn", 0 // ice age  TRN & name
},
{
	BM_C_366_BLOOD_STONE,	2, 0,	128,	1680,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Magma\\Magma%c.CL2",	1,	"Monsters\\Magma\\Magma%c%i.WAV",		1,
	1,	"Monsters\\Magma\\Yellow.TRN",	// TRNs:   flag and path
	8, 10, 14, 7, 18, 18,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Blood Stone",			// name
	16,	18,	14,		// DLVL range & MLVL
	55,	75, // HP range,
	AI_9_Magma,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	50,	4,	5,	14,// tohit, hit frame of 1st attack, dmg range.
	26,	14,	17,	32,	 // 2nd attack: tohit, hit frame, dmg range
	45, 1, MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	1309,		// special flag for drop, animation size, XP
	"X\\Magma_11_frozen_stone.trn", 0 // ice age  TRN & name
},
{
	BM_C_367_HELL_STONE,	2, 0,	128,	1680,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Magma\\Magma%c.CL2",	1,	"Monsters\\Magma\\Magma%c%i.WAV",		1,
	1,	"Monsters\\Magma\\Blue.TRN",	// TRNs:   flag and path
	8, 10, 14, 7, 18, 18,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Hell Stone",			// name
	18,	20,	16,		// DLVL range & MLVL
	60,	80, // HP range,
	AI_9_Magma,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	60,	4,	6,	20,// tohit, hit frame of 1st attack, dmg range.
	26,	14,	17,	32,	 // 2nd attack: tohit, hit frame, dmg range
	50, 1, MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	1680,		// special flag for drop, animation size, XP
	"X\\Magma_11_frozen_stone.trn", 0 // ice age  TRN & name
},
{
	BM_C_368_LAVA_LORD,	2, 0,	128,	1680,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Magma\\Magma%c.CL2",	1,	"Monsters\\Magma\\Magma%c%i.WAV",		1,
	1,	"Monsters\\Magma\\Wierd.TRN",	// TRNs:   flag and path
	8, 10, 14, 7, 18, 18,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Lava Lord",			// name
	20,	22,	18,		// DLVL range & MLVL
	70,	85, // HP range,
	AI_9_Magma,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	75,	4,	7,	24,// tohit, hit frame of 1st attack, dmg range.
	26,	14,	17,	32,	 // 2nd attack: tohit, hit frame, dmg range
	60, 1, MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	2124,		// special flag for drop, animation size, XP
	"X\\Magma_11_frozen_stone.trn", 0 // ice age  TRN & name
},
// Balrogs
{
	BM_C_369_SLAYER,	2, 0,	160,	2220,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Mega\\Mega%c.CL2",	1,	"Monsters\\Mega\\Mega%c%i.WAV",		1,
	0,	"X\\M1.trn",	// TRNs:   flag and path
	6, 7, 14, 1, 24, 5,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Slayer",			// name
	18,	22,	20,		// DLVL range & MLVL
	120,	140, // HP range,
	AI_26_Balrog,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	100,	8,	12,	20,// tohit, hit frame of 1st attack, dmg range.
	17,	3,	45,	60,	 // 2nd attack: tohit, hit frame, dmg range
	60, 1, MR_1_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	2300,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_370_GUARDIAN,	2, 0,	160,	2220,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Mega\\Mega%c.CL2",	1,	"Monsters\\Mega\\Mega%c%i.WAV",		1,
	1,	"Monsters\\Mega\\Guard.TRN",	// TRNs:   flag and path
	6, 7, 14, 1, 24, 5,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Guardian",			// name
	20,	24,	22,		// DLVL range & MLVL
	140,	160, // HP range,
	AI_26_Balrog,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	110,	8,	14,	22,// tohit, hit frame of 1st attack, dmg range.
	17,	3,	45,	60,	 // 2nd attack: tohit, hit frame, dmg range
	65, 1, MR_1_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	2722,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_371_VORTEX_LORD,	2, 0,	160,	2220,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Mega\\Mega%c.CL2",	1,	"Monsters\\Mega\\Mega%c%i.WAV",		1,
	1,	"Monsters\\Mega\\Vtexl.TRN",	// TRNs:   flag and path
	6, 7, 14, 1, 24, 5,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Vortex Lord",			// name
	22,	26,	24,		// DLVL range & MLVL
	160,	180, // HP range,
	AI_26_Balrog,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	120,	8,	18,	24,// tohit, hit frame of 1st attack, dmg range.
	17,	3,	45,	60,	 // 2nd attack: tohit, hit frame, dmg range
	70, 1, MR_1_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	3252,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_372_BALROG,	2, 0,	160,	2220,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Mega\\Mega%c.CL2",	1,	"Monsters\\Mega\\Mega%c%i.WAV",		1,
	1,	"Monsters\\Mega\\Balr.TRN",	// TRNs:   flag and path
	6, 7, 14, 1, 24, 5,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Balrog",			// name
	24,	28,	26,		// DLVL range & MLVL
	180,	200, // HP range,
	AI_26_Balrog,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	130,	8,	22,	30,// tohit, hit frame of 1st attack, dmg range.
	17,	3,	45,	60,	 // 2nd attack: tohit, hit frame, dmg range
	75, 1, MR_1_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	3643,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
// Vipers
{
	BM_C_373_CAVE_VIPER,	2, 99,	160,	1270,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Snake\\Snake%c.CL2",	1,	"Monsters\\Snake\\Snake%c%i.WAV",		1,
	0,	"Monsters\\Snake\\Snakg.TRN",	// TRNs:   flag and path
	12, 11, 13, 6, 18, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Cave Snake",			// name
	20,	24,	21,		// DLVL range & MLVL
	100,	150, // HP range,
	AI_24_Viper,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 0,	// AI, int flag, int level
	90,	8,	8,	20,// tohit, hit frame of 1st attack, dmg range.
	96,	9,	12,	24,	 // 2nd attack: tohit, hit frame, dmg range
	36, 2, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	2725,		// special flag for drop, animation size, XP
	"X\\Snake_12_iceage_icy_caves_v2.trn", 0 // ice age  TRN & name
},
{
	BM_C_374_FIRE_DRAKE,	2, 99,	160,	1270,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Snake\\Snake%c.CL2",	1,	"Monsters\\Snake\\Snake%c%i.WAV",		1,
	1,	"Monsters\\Snake\\SnakR.TRN",	// TRNs:   flag and path
	12, 11, 13, 6, 18, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Fire Drake",			// name
	22,	26,	23,		// DLVL range & MLVL
	120,	170, // HP range,
	AI_24_Viper,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	105,	8,	12,	24,// tohit, hit frame of 1st attack, dmg range.
	116,	9,	16,	28,	 // 2nd attack: tohit, hit frame, dmg range
	36, 2, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	3139,		// special flag for drop, animation size, XP
	"X\\Snake_12_iceage_icy_caves_v2.trn", 0 // ice age  TRN & name
},
{
	BM_C_375_GOLD_VIPER,	2, 99,	160,	1270,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Snake\\Snake%c.CL2",	1,	"Monsters\\Snake\\Snake%c%i.WAV",		1,
	1,	"Monsters\\Snake\\SnakgG.TRN",	// TRNs:   flag and path
	12, 11, 13, 6, 18, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Gold Viper",			// name
	24,	26,	25,		// DLVL range & MLVL
	140,	180, // HP range,
	AI_24_Viper,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	120,	8,	15,	26,// tohit, hit frame of 1st attack, dmg range.
	136,	9,	19,	30,	 // 2nd attack: tohit, hit frame, dmg range
	36, 2, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	3484,		// special flag for drop, animation size, XP
	"X\\Snake_12_iceage_icy_caves_v2.trn", 0 // ice age  TRN & name
},
{
	BM_C_376_AZURE_DRAKE,	2, 99,	160,	1270,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Snake\\Snake%c.CL2",	1,	"Monsters\\Snake\\Snake%c%i.WAV",		1,
	1,	"Monsters\\Snake\\Snakb.TRN",	// TRNs:   flag and path
	12, 11, 13, 6, 18, 16,		// IWAHDS frames
	2, 0, 0, 0, 1, 0,		// delays
	"Azure Drake",			// name
	28,	28,	27,		// DLVL range & MLVL
	160,	200, // HP range,
	AI_24_Viper,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	130,	8,	18,	30,// tohit, hit frame of 1st attack, dmg range.
	146,	9,	22,	34,	 // 2nd attack: tohit, hit frame, dmg range
	36, 2, MR_4_IM_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_4_IM_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,// armor, mon type, and resistances
	0,		7,	3791,		// special flag for drop, animation size, XP
	"X\\Snake_12_iceage_icy_caves_v2.trn", 0 // ice age  TRN & name
},
// Hounds
{
	BM_C_377_ACID_BEAST,	2, 57,	128,	716,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Acid\\Acid%c.CL2",	1,	"Monsters\\Acid\\Acid%c%i.WAV",		1,
	0,	"X\\Mega_01.TRN",	// TRNs:   flag and path
	13, 8, 12, 7, 16, 12,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Acid Beast",			// name
	10,	14,	11,		// DLVL range & MLVL
	40,	66, // HP range,
	AI_19_Acid_Beast,	MF_9_CHASE_TARGET, 0,	// AI, int flag, int level
	40,	6,	4,	12,// tohit, hit frame of 1st attack, dmg range.
	30,	4,	28,	46,	 // 2nd attack: tohit, hit frame, dmg range
	30, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	846,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_378_POSON_SPITTER,	2, 57,	128,	716,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Acid\\Acid%c.CL2",	1,	"Monsters\\Acid\\Acid%c%i.WAV",		1,
	1,	"Monsters\\Acid\\AcidBlk.TRN",	// TRNs:   flag and path
	13, 8, 12, 7, 16, 12,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Poison Spitter",			// name
	14,	18,	15,		// DLVL range & MLVL
	60,	85, // HP range,
	AI_19_Acid_Beast,	MF_9_CHASE_TARGET, 1,	// AI, int flag, int level
	45,	6,	4,	16,// tohit, hit frame of 1st attack, dmg range.
	30,	4,	28,	46,	 // 2nd attack: tohit, hit frame, dmg range
	30, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	1248,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_379_PIT_BEAST,	2, 57,	128,	716,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Acid\\Acid%c.CL2",	1,	"Monsters\\Acid\\Acid%c%i.WAV",		1,
	1,	"Monsters\\Acid\\AcidB.TRN",	// TRNs:   flag and path
	13, 8, 12, 7, 16, 12,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Pit Beast",			// name
	18,	22,	21,		// DLVL range & MLVL
	80,	110, // HP range,
	AI_19_Acid_Beast,	MF_9_CHASE_TARGET, 2,	// AI, int flag, int level
	55,	6,	8,	18,// tohit, hit frame of 1st attack, dmg range.
	30,	4,	28,	46,	 // 2nd attack: tohit, hit frame, dmg range
	35, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	2060,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_380_LAVA_MAW,	2, 57,	128,	716,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Acid\\Acid%c.CL2",	1,	"Monsters\\Acid\\Acid%c%i.WAV",		1,
	1,	"Monsters\\Acid\\AcidR.TRN",	// TRNs:   flag and path
	13, 8, 12, 7, 16, 12,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"Lava Maw",			// name
	22,	26,	25,		// DLVL range & MLVL
	100,	150, // HP range,
	AI_19_Acid_Beast,	MF_9_CHASE_TARGET, 3,	// AI, int flag, int level
	65,	6,	10,	20,// tohit, hit frame of 1st attack, dmg range.
	30,	4,	28,	46,	 // 2nd attack: tohit, hit frame, dmg range
	35, 2, MR_1_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_5_IM_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	2940,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
// Blinks
{
	BM_C_381_FIEND,	2, 0,	96,	364,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Bat\\Bat%c.CL2",	0,	"Monsters\\Bat\\bat%c%i.wav",		0,
	1,	"Monsters\\Bat\\red.trn",	// TRNs:   flag and path
	9, 13, 10, 9, 13, 0,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Fiend",			// name
	2,	4,	3,		// DLVL range & MLVL
	3,	6, // HP range,
	AI_11_Bat,	MF_9_CHASE_TARGET, 0,	// AI, int flag, int level
	35,	5,	1,	6,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	26, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	16384,		6,	102,		// special flag for drop, animation size, XP
	"X\\Bat_03_bright_blue.trn", 0 // ice age  TRN & name
},
{
	BM_C_382_BLINK,	2, 0,	96,	364,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Bat\\Bat%c.CL2",	0,	"Monsters\\Bat\\bat%c%i.wav",		0,
	0,	"Monsters\\Bat\\orange.trn",	// TRNs:   flag and path
	9, 13, 10, 9, 13, 0,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Blink",			// name
	4,	8,	7,		// DLVL range & MLVL
	12,	28, // HP range,
	AI_11_Bat,	MF_9_CHASE_TARGET, 1,	// AI, int flag, int level
	45,	5,	2,	8,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	26, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	16384,		6,	350,		// special flag for drop, animation size, XP
	"X\\Bat_03_bright_blue.trn", 0 // ice age  TRN & name
},
{
	BM_C_383_GLOOM,	2, 0,	96,	364,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Bat\\Bat%c.CL2",	0,	"Monsters\\Bat\\bat%c%i.wav",		0,
	1,	"Monsters\\Bat\\grey.trn",	// TRNs:   flag and path
	9, 13, 10, 9, 13, 0,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Gloom",			// name
	6,	10,	9,		// DLVL range & MLVL
	28,	36, // HP range,
	AI_11_Bat,	MF_9_CHASE_TARGET, 2,	// AI, int flag, int level
	70,	5,	4,	12,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	26, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	16384,		6,	509,		// special flag for drop, animation size, XP
	"X\\Bat_03_bright_blue.trn", 0 // ice age  TRN & name
},
{
	BM_C_384_FAMILIAR,	2, 0,	96,	364,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Bat\\Bat%c.CL2",	0,	"Monsters\\Bat\\bat%c%i.wav",		0,
	1,	"Monsters\\Bat\\orange.trn",	// TRNs:   flag and path
	9, 13, 10, 9, 13, 0,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Familiar",			// name
	10,	14,	13,		// DLVL range & MLVL
	20,	35, // HP range,
	AI_11_Bat,	MF_9_CHASE_TARGET, 3,	// AI, int flag, int level
	50,	5,	4,	16,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	26, 1, MR_1_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_6_IM_LIGHT,// armor, mon type, and resistances
	16384,		6,	448,		// special flag for drop, animation size, XP
	"X\\Bat_03_bright_blue.trn", 0 // ice age  TRN & name
},
// Scavengers
{
	BM_C_385_SCAVENGER,	2, 45,	128,	410,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Scav\\Scav%c.CL2",	1,	"Monsters\\Scav\\Scav%c%i.WAV",		0,
	0,	"Monsters\\FalSpear\\FallenT.TRN",	// TRNs:   flag and path
	12, 8, 12, 6, 20, 11,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Scavenger",			// name
	0,	4,	2,		// DLVL range & MLVL
	3,	6, // HP range,
	AI_4_Scavenger,	MF_9_CHASE_TARGET, 0,	// AI, int flag, int level
	20,	7,	1,	5,// tohit, hit frame of 1st attack, dmg range.
	27,	0,	30,	30,	 // 2nd attack: tohit, hit frame, dmg range
	10, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	16384,		3,	80,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_386_PLAGUE_EATER,	2, 46,	128,	410,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Scav\\Scav%c.CL2",	1,	"Monsters\\Scav\\Scav%c%i.WAV",		0,
	1,	"Monsters\\Scav\\ScavBr.TRN",	// TRNs:   flag and path
	12, 8, 12, 6, 20, 11,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Plague Eater",			// name
	2,	6,	4,		// DLVL range & MLVL
	12,	24, // HP range,
	AI_4_Scavenger,	MF_9_CHASE_TARGET, 1,	// AI, int flag, int level
	30,	7,	1,	8,// tohit, hit frame of 1st attack, dmg range.
	27,	0,	30,	30,	 // 2nd attack: tohit, hit frame, dmg range
	20, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	16384,		3,	188,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_387_SHADOW_BEAST,	2, 47,	128,	410,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Scav\\Scav%c.CL2",	1,	"Monsters\\Scav\\Scav%c%i.WAV",		0,
	1,	"Monsters\\Scav\\ScavBe.TRN",	// TRNs:   flag and path
	12, 8, 12, 6, 20, 11,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Shadow Beast",			// name
	4,	8,	6,		// DLVL range & MLVL
	24,	36, // HP range,
	AI_4_Scavenger,	MF_9_CHASE_TARGET, 2,	// AI, int flag, int level
	35,	7,	3,	12,// tohit, hit frame of 1st attack, dmg range.
	27,	0,	30,	30,	 // 2nd attack: tohit, hit frame, dmg range
	25, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	16384,		3,	375,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_388_BONE_GASHER,	2, 48,	128,	410,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Scav\\Scav%c.CL2",	1,	"Monsters\\Scav\\Scav%c%i.WAV",		0,
	1,	"Monsters\\Scav\\ScavW.TRN",	// TRNs:   flag and path
	12, 8, 12, 6, 20, 11,		// IWAHDS frames
	2, 0, 0, 0, 0, 0,		// delays
	"Bone Gasher",			// name
	6,	10,	8,		// DLVL range & MLVL
	28,	40, // HP range,
	AI_4_Scavenger,	MF_9_CHASE_TARGET, 3,	// AI, int flag, int level
	35,	7,	5,	15,// tohit, hit frame of 1st attack, dmg range.
	27,	0,	30,	30,	 // 2nd attack: tohit, hit frame, dmg range
	30, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	16384,		3,	552,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
// Specials
{
	BM_C_389_DIABLO,	0, 140,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\Diablo\\cls\\Diablo%c.CL2",	1,	"Monsters\\Diablo\\Diablo%c%i.WAV",		0,
	0,	0,	// TRNs:   flag and path
	16, 6, 16, 2, 16, 16,		// IWAHDS frames
	0, 0, 0, 0, 0, 0,		// delays
	"The Dark Lord",			// name
	0,	0,	40,		// DLVL range & MLVL
	10,	10, // HP range,
	AI_27_Diablo,	MF_9_CHASE_TARGET, 3,	// AI, int flag, int level
	220,	11,	30,	60,// tohit, hit frame of 1st attack, dmg range.
	0,	11,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	80, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	31666,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_390_BUTCHER,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\FatC\\FatC%c.CL2",	1,	"Monsters\\FatC\\classique\\FatC%c%i.wav",		1,
	0,	"Monsters\\Snake\\SnakR.TRN",	// TRNs:   flag and path
	10, 8, 12, 6, 16, 0,		// IWAHDS frames
	1, 0, 0, 0, 0, 0,		// delays
	"The Butcher",			// name
	0,	0,	1,		// DLVL range & MLVL
	320,	320, // HP range,
	AI_13_Butcher,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET/* | MF_10_OPEN_DOORS*/, 3,	// AI, int flag, int level
	50,	7,	6,	12,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	50, 1, MR_1_ARCAN | MR_5_IM_FIRE | MR_7_IM_CURSE,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_7_IM_CURSE,// armor, mon type, and resistances
	0,		3,	710,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_C_391_LEORIC,	0, 100,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\SKing\\SKing%c.CL2",	1,	"Monsters\\SKing\\SKing%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	8, 8, 16, 5, 16, 12,		// IWAHDS frames
	2, 0, 0, 0, 0, 2,		// delays
	"Skeleton King",			// name
	6,	6,	9,		// DLVL range & MLVL
	140,	140, // HP range,
	AI_10_Skel_King,	MF_8_KNOCKBACK | MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	60,	8,	6,	16,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	70, 0, MR_2_FIRE | MR_4_IM_ARCAN | MR_6_IM_LIGHT | MR_7_IM_CURSE | MR_11_IM_COLD,	MR_4_IM_ARCAN | MR_5_IM_FIRE | MR_6_IM_LIGHT | MR_7_IM_CURSE | MR_11_IM_COLD,// armor, mon type, and resistances
	0,		7,	570,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},

//--------------------------------CLASSIC END--------------------------
//---------------------------------------------------------------------
{
	BM_392_WARLORD_MERC,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"Monsters\\renwar01\\rw%c.CL2",	1,	"Monsters\\renwar01\\rw%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	6, 8, 13, 4, 8, 2,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Mercenary",			// name
	50,	50,	4,		// DLVL range & MLVL
	12,	24, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET, 5,	// AI, int flag, int level
	19,	9,	30,	40,// tohit, hit frame of 1st attack, dmg range.
	24,	1,	55,	55,	 // 2nd attack: tohit, hit frame, dmg range
	58, 1, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT | MR_7_IM_CURSE,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT | MR_7_IM_CURSE,// armor, mon type, and resistances
	16384,		7,	80,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
//--------------------------------DC HEROES--------------------------
{
	BM_H_393_SORC_L_T,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\Sorc_L_T\\sor_l_t_%c.CL2",	0,	"DC\\Heroes\\Sorc_L_T\\mageF%c%i.WAV",		0,
	0,	0,	// TRNs:   flag and path
	8, 8, 12, 8, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Wizard",			// name
	0,	0,	5,		// DLVL range & MLVL
	22,	32, // HP range,
	AI_32_Ranger_Firebolt,	MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	10,	8,	5,	10,// tohit, hit frame of 1st attack, dmg range.
	24,	8,	4,	8,	 // 2nd attack: tohit, hit frame, dmg range
	3, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	220,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_394_ROG_L_B,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\rlb\\rog_l_b_%c.CL2",	0,	"DC\\Heroes\\rlb\\rog%c%i.WAV",		0,
	0,	0,	// TRNs:   flag and path
	8, 8, 12, 7, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Archer",			// name
	0,	0,	4,		// DLVL range & MLVL
	34,	40, // HP range,
	AI_7_Goat_Archer,	MF_9_CHASE_TARGET| MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	10,	8,	3,	7,// tohit, hit frame of 1st attack, dmg range.
	24,	8,	4,	8,	 // 2nd attack: tohit, hit frame, dmg range
	4, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	165,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_395_WAR_L_X,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\WLX\\war_l_ax_%c.CL2",	1,	"DC\\Heroes\\WLX\\wlx%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	10, 8, 20, 6, 20, 20,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Barbarian",			// name
	0,	0,	5,		// DLVL range & MLVL
	44,	50, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	12,	8,	6,	12,// tohit, hit frame of 1st attack, dmg range.
	20,	14,	9,	14,	 // 2nd attack: tohit, hit frame, dmg range
	5, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	195,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_396_SORC_L_T,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\Elementalist_LT\\sor_l_t_%c.CL2",	0,	"DC\\Heroes\\Sorc_L_T\\elm%c%i.WAV",		0,
	1,	"X\\plrtrn\\Mage01blue04.trn",	// TRNs:   flag and path
	8, 8, 12, 8, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Elementalist",			// name
	0,	0,	7,		// DLVL range & MLVL
	32,	42, // HP range,
	AI_41_Ranger_snowbolt,	MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	10,	8,	7,	11,// tohit, hit frame of 1st attack, dmg range.
	24,	8,	4,	8,	 // 2nd attack: tohit, hit frame, dmg range
	8, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	320,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_397_ROG_L_B,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\rlb\\rog_l_b_%c.CL2",	0,	"DC\\Heroes\\rlb\\rog%c%i.WAV",		0,
	0,	0,	// TRNs:   flag and path
	8, 8, 12, 7, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Archer",			// name
	0,	0,	6,		// DLVL range & MLVL
	44,	50, // HP range,
	AI_7_Goat_Archer,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	14,	8,	4,	9,// tohit, hit frame of 1st attack, dmg range.
	24,	8,	5,	12,	 // 2nd attack: tohit, hit frame, dmg range
	9, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	265,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_398_WAR_L_X,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\WLX\\war_l_ax_%c.CL2",	1,	"DC\\Heroes\\WLX\\wlx%c%i.WAV",		1,
	1,	"X\\plrtrn\\Savage01fixt02.TRN",	// TRNs:   flag and path
	10, 8, 20, 6, 20, 20,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Savage",			// name
	0,	0,	8,		// DLVL range & MLVL
	56,	62, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	12,	8,	9,	14,// tohit, hit frame of 1st attack, dmg range.
	20,	14,	11,	16,	 // 2nd attack: tohit, hit frame, dmg range
	10, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	295,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_399_WAR_L_SS,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\WLSwordShield\\war_l_ssh_%c.CL2",	1,	"DC\\Heroes\\WLSwordShield\\wlss%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	10, 8, 15, 6, 20, 2,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Warrior",			// name
	0,	0,	10,		// DLVL range & MLVL
	76,	90, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	14,	9,	10,	15,// tohit, hit frame of 1st attack, dmg range.
	20,	1,	11,	16,	 // 2nd attack: tohit, hit frame, dmg range
	12, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	385,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_400_SORC_M_T,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\SMT\\sor_m_t_%c.CL2",	1,	/*"DC\\Heroes\\SMT\\smt%c%i.WAV"*/"DC\\Heroes\\Sorc_L_T\\mageF%c%i.WAV",		0,
	0,	0,	// TRNs:   flag and path
	8, 8, 12, 8, 20, 12,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Battle Mage",			// name
	0,	0,	13,		// DLVL range & MLVL
	70,	90, // HP range,
	AI_25_Advocate,	MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	22,	9,	18,	24,// tohit, hit frame of 1st attack, dmg range.
	24,	8,	4,	8,	 // 2nd attack: tohit, hit frame, dmg range
	23, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	500,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_401_MONK_M_MS,	0, 0,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\Monx\\MMS\\mon_m_msh_%c.CL2",	1,	"DC\\Heroes\\Monx\\MMS\\mms%c%i.WAV",		1,
	1,	"X\\PLRTRN\\Monk08_orange.trn",	// TRNs:   flag and path
	8, 8, 15, 6, 20, 3,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Monk",			// name
	0,	0,	16,		// DLVL range & MLVL
	150,	180, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	25,	10,	20,	25,// tohit, hit frame of 1st attack, dmg range.
	30,	1,	30,	35,	 // 2nd attack: tohit, hit frame, dmg range
	25, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	600,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_402_BERS_M_X,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\WMX\\war_m_ax_%c.CL2",	1,	"DC\\Heroes\\WLX\\wlx%c%i.WAV",		1,
	1,	"X\\plrtrn\\Savage04blackish01mid1.TRN",	// TRNs:   flag and path
	10, 8, 20, 6, 20, 20,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Berserker",			// name
	0,	0,	17,		// DLVL range & MLVL
	280,	360, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	27,	22,	20,	25,// tohit, hit frame of 1st attack, dmg range.
	20,	14,	11,	16,	 // 2nd attack: tohit, hit frame, dmg range
	26, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	625,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_403_ROG_M_B,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\rmb\\rog_m_b_%c.CL2",	0,	"DC\\Heroes\\rlb\\rog%c%i.WAV",		0,
	1,	"X\\plrtrn\\Archer02black01upd1.TRN",	// TRNs:   flag and path
	8, 8, 12, 7, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Sharpshooter",			// name
	0,	0,	16,		// DLVL range & MLVL
	120,	180, // HP range,
	AI_7_Goat_Archer,	/*MF_9_CHASE_TARGET | */MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	45,	8,	25,	35,// tohit, hit frame of 1st attack, dmg range.
	24,	8,	5,	12,	 // 2nd attack: tohit, hit frame, dmg range
	18, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	575,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_404_WAR_M_MS,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\WMMaceShield\\war_m_msh_%c.CL2",	1,	"DC\\Heroes\\WLSwordShield\\wlss%c%i.WAV",		1,
	1,	"X\\plrtrn\\Templar02x2.TRN",	// TRNs:   flag and path
	10, 8, 16, 6, 20, 2,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Templar",			// name
	0,	0,	22,		// DLVL range & MLVL
	275,	325, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	30,	18,	22,	28,// tohit, hit frame of 1st attack, dmg range.
	20,	1,	11,	16,	 // 2nd attack: tohit, hit frame, dmg range
	30, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	700,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_405_SOR_M_T_HYBRID,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\SMT_hybrid\\sor_m_t_%c.CL2",	1,	/*"DC\\Heroes\\SMT\\smt%c%i.WAV"*/"DC\\Heroes\\SMT_hybrid\\mageF%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	8, 8, 16, 8, 20, 12,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Battle Mage",			// name
	0,	0,	24,		// DLVL range & MLVL
	215,	265, // HP range,
	AI_16_Thunder_Demon,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 1,	// AI, int flag, int level
	25,	9,	19,	26,// tohit, hit frame of 1st attack, dmg range.
	30,	9,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	27, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	725,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_406_SCOUT,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\rmb\\rog_m_b_%c.CL2",	0,	"DC\\Heroes\\rlb\\rog%c%i.WAV",		0,
	1,	"X\\plrtrn\\Archer04red1.TRN",	// TRNs:   flag and path
	8, 8, 12, 7, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Scout",			// name
	0,	0,	26,		// DLVL range & MLVL
	160,	200, // HP range,
	AI_45_Ranger_Arcane_Arrow,	/*MF_9_CHASE_TARGET | */MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	35,	7,	18,	28,// tohit, hit frame of 1st attack, dmg range.
	24,	8,	5,	12,	 // 2nd attack: tohit, hit frame, dmg range
	31, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	775,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_407_MONK_M_X,	0, 0,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\Monx\\MMX\\mon_m_ax_%c.CL2",	1,	"DC\\Heroes\\Monx\\MMS\\mms%c%i.WAV",		1,
	1,	"X\\PLRTRN\\Monk02RedBlackSkin.TRN",	// TRNs:   flag and path
	8, 8, 23, 6, 20, 18,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Shugoki",			// name
	0,	0,	27,		// DLVL range & MLVL
	150,	180, // HP range,
	AI_5_Charger ,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	30,	27,	20,	29,// tohit, hit frame of 1st attack, dmg range.
	33,	7,	31,	37,	 // 2nd attack: tohit, hit frame, dmg range
	33, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	800,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_408_WAR_M_SS,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\WMSwordShield\\war_m_ssh_%c.CL2",	1,	"DC\\Heroes\\WLSwordShield\\wlss%c%i.WAV",		1,
	0,	0,	// TRNs:   flag and path
	10, 8, 16, 6, 20, 2,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Warrior Veteran",			// name
	0,	0,	30,		// DLVL range & MLVL
	350,	400, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	30,	18,	25,	35,// tohit, hit frame of 1st attack, dmg range.
	30,	1,	30,	40,	 // 2nd attack: tohit, hit frame, dmg range
	35, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	1'000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_409_MONK_H_T,	0, 0,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\Monx\\MHT\\mon_h_t_%c.CL2",	1,	"DC\\Heroes\\Monx\\MMS\\mms%c%i.WAV",		1,
	1,	"X\\PLRTRN\\Monk08_orange.TRN",	// TRNs:   flag and path
	8, 8, 9, 5, 20, 18,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Monk",			// name
	0,	0,	35,		// DLVL range & MLVL
	425,	475, // HP range,
	AI_2_Skel_Melee ,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	35,	8,	30,	40,// tohit, hit frame of 1st attack, dmg range.
	40,	7,	35,	45,	 // 2nd attack: tohit, hit frame, dmg range
	40, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	1'200,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_410_MONK_H_SS,	0, 0,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\Monx\\MHSS\\mon_h_ssh_%c.CL2",	1,	"DC\\Heroes\\Monx\\MMS\\mms%c%i.WAV",		1,
	1,	"X\\PLRTRN\\Monk07crimzonPlusBlackSkin1.trn",	// TRNs:   flag and path
	8, 8, 16, 5, 20, 3,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Shinobi",			// name
	0,	0,	38,		// DLVL range & MLVL
	375,	425, // HP range,
	AI_15_Hidden,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	35,	19,	32,	43,// tohit, hit frame of 1st attack, dmg range.
	40,	1,	37,	48,	 // 2nd attack: tohit, hit frame, dmg range
	45, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	1'500,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_411_ROG_H_B,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\rhb\\rog_h_b_%c.CL2",	0,	"DC\\Heroes\\rlb\\rog%c%i.WAV",		0,
	0,	"X\\plrtrn\\Archer02black01upd1.TRN",	// TRNs:   flag and path
	8, 8, 12, 7, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Sharpshooter",			// name
	0,	0,	37,		// DLVL range & MLVL
	300,	350, // HP range,
	AI_7_Goat_Archer,	/*MF_9_CHASE_TARGET | */MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	50,	8,	30,	45,// tohit, hit frame of 1st attack, dmg range.
	24,	8,	5,	12,	 // 2nd attack: tohit, hit frame, dmg range
	41, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	1'425,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_412_WAR_H_X_EXEC,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\WHX\\war_h_ax_%c.CL2",	1,	"DC\\Heroes\\WLX\\wlx%c%i.WAV",		1,
	0,	"X\\plrtrn\\Savage04blackish01mid1.TRN",	// TRNs:   flag and path
	10, 8, 20, 6, 20, 20,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Executioner",			// name
	0,	0,	41,		// DLVL range & MLVL
	650,	850, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	35,	22,	35,	50,// tohit, hit frame of 1st attack, dmg range.
	40,	8,	45,	55,	 // 2nd attack: tohit, hit frame, dmg range
	50, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	2'000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_413_SOR_H_T,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\SHT\\sor_h_t_%c.CL2",	1,	/*"DC\\Heroes\\SMT\\smt%c%i.WAV"*/"DC\\Heroes\\Sorc_l_t\\mageF%c%i.WAV",		0,
	0,	0,	// TRNs:   flag and path
	8, 8, 12, 8, 20, 12,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Sorcerer",			// name
	0,	0,	44,		// DLVL range & MLVL
	330,	420, // HP range,
	AI_25_Advocate,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	25,	8,	40,	55,// tohit, hit frame of 1st attack, dmg range.
	30,	7,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	55, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	2'250,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_414_WAR_H_MS,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\WHMS\\war_h_msh_%c.CL2",	1,	"DC\\Heroes\\WLSwordShield\\wlss%c%i.WAV",		1,
	0,	"X\\plrtrn\\Templar02x2.TRN",	// TRNs:   flag and path
	10, 8, 16, 6, 20, 2,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Paladin",			// name
	0,	0,	47,		// DLVL range & MLVL
	600,	750, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	35,	18,	39,	52,// tohit, hit frame of 1st attack, dmg range.
	20,	1,	48,	57,	 // 2nd attack: tohit, hit frame, dmg range
	55, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	2'500,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_415_ROG_H_B_ELITE_ARCHER,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\rhb\\rog_h_b_%c.CL2",	0,	"DC\\Heroes\\rlb\\rog%c%i.WAV",		0,
	0,	"X\\plrtrn\\Archer02black01upd1.TRN",	// TRNs:   flag and path
	8, 8, 11, 6, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Elite Archer",			// name
	0,	0,	49,		// DLVL range & MLVL
	350,	450, // HP range,
	AI_7_Goat_Archer,	/*MF_9_CHASE_TARGET | */MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	35,	7,	44,	56,// tohit, hit frame of 1st attack, dmg range.
	36,	8,	5,	12,	 // 2nd attack: tohit, hit frame, dmg range
	60, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	2'750,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_416_MON_H_MS,	0, 0,	160,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\Monx\\MHMS\\mon_h_msh_%c.CL2",	1,	"DC\\Heroes\\Monx\\MMS\\mms%c%i.WAV",		1,
	1,	"X\\PLRTRN\\Monk07crimzonPlusBlackSkin1.trn",	// TRNs:   flag and path
	8, 8, 16, 5, 20, 3,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Shinobi Champion",			// name
	0,	0,	51,		// DLVL range & MLVL
	500,	600, // HP range,
	AI_15_Hidden,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	35,	19,	46,	58,// tohit, hit frame of 1st attack, dmg range.
	40,	1,	50,	60,	 // 2nd attack: tohit, hit frame, dmg range
	63, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,	3,	3'000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_417_WAR_H_SS_WARRIOR_ELITE,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\WHSwordShield\\war_h_ssh_%c.CL2",	1,	"DC\\Heroes\\WLSwordShield\\wlss%c%i.WAV",		1,
	0,	"X\\plrtrn\\Templar02x2.TRN",	// TRNs:   flag and path
	10, 8, 15, 5, 20, 2,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Warrior Elite",			// name
	0,	0,	53,		// DLVL range & MLVL
	800,	1'000, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	35,	18,	50,	60,// tohit, hit frame of 1st attack, dmg range.
	20,	1,	55,	65,	 // 2nd attack: tohit, hit frame, dmg range
	70, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	3'250,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_418_SOR_H_T_SORC_ELITE,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\SHT\\sor_h_t_%c.CL2",	1,	/*"DC\\Heroes\\SMT\\smt%c%i.WAV"*/"DC\\Heroes\\Sorc_l_t\\mageF%c%i.WAV",		0,
	0,	0,	// TRNs:   flag and path
	8, 8, 11, 7, 20, 12,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Sorcerer Elite",			// name
	0,	0,	55,		// DLVL range & MLVL
	425,	500, // HP range,
	AI_25_Advocate,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 2,	// AI, int flag, int level
	25,	8,	60,	70,// tohit, hit frame of 1st attack, dmg range.
	30,	7,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	75, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	3'500,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_419_ROG_H_B_SS_ELITE,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\rhb\\rog_h_b_%c.CL2",	0,	"DC\\Heroes\\rlb\\rog%c%i.WAV",		0,
	1,	"X\\plrtrn\\Archer02black01upd1.TRN",	// TRNs:   flag and path
	8, 8, 11, 6, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Sharpshooter Elite",			// name
	0,	0,	57,		// DLVL range & MLVL
	500,	600, // HP range,
	AI_7_Goat_Archer,	/*MF_9_CHASE_TARGET | */MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	50,	7,	60,	70,// tohit, hit frame of 1st attack, dmg range.
	45,	8,	65,	75,	 // 2nd attack: tohit, hit frame, dmg range
	80, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	3'750,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_NPC_420_GRISWOLD,	0, 0,	150,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\NPC\\Grizzie\\gris_dc_%c.CL2",	0,	"monsters\\zombie\\zmbie%c%i.WAV",		0,
	0,	0,	// TRNs:   flag and path
	6, 8, 16, 6, 20, 0,		// IWAHDS frames
	3, 1, 0, 0, 2, 0,		// delays
	"Griswold Corpse",			// name
	0,	0,	63,		// DLVL range & MLVL
	1'000,	1'000, // HP range,
	AI_2_Skel_Melee,	MF_9_CHASE_TARGET /* | MF_10_OPEN_DOORS*/, 4,	// AI, int flag, int level
	40,	10,	70,	80,// tohit, hit frame of 1st attack, dmg range.
	0,	0,	0,	0,	 // 2nd attack: tohit, hit frame, dmg range
	100, 0, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		7,	5'000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_421_WAR_H_X_EXEC_ELITE,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\WHX\\war_h_ax_%c.CL2",	1,	"DC\\Heroes\\WLX\\wlx%c%i.WAV",		1,
	0,	"X\\plrtrn\\Savage04blackish01mid1.TRN",	// TRNs:   flag and path
	10, 8, 19, 5, 20, 20,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Executioner Elite",			// name
	0,	0,	59,		// DLVL range & MLVL
	1'200,	1'400, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 3,	// AI, int flag, int level
	35,	22,	55,	65,// tohit, hit frame of 1st attack, dmg range.
	40,	8,	70,	80,	 // 2nd attack: tohit, hit frame, dmg range
	85, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	4'000,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_422_WAR_H_ROYAL_GUARD,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\WHX\\war_h_ax_%c.CL2",	1,	"DC\\Heroes\\WLX\\wlx%c%i.WAV",		1,
	0,	"X\\plrtrn\\Savage04blackish01mid1.TRN",	// TRNs:   flag and path
	10, 8, 19, 5, 20, 20,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Royal Guard",			// name
	0,	0,	63,		// DLVL range & MLVL
	1'500,	2'000, // HP range,
	AI_8_Fallen_One,	MF_9_CHASE_TARGET | MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	45,	22,	70,	80,// tohit, hit frame of 1st attack, dmg range.
	50,	8,	80,	90,	 // 2nd attack: tohit, hit frame, dmg range
	95, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	4'600,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},
{
	BM_H_423_ROG_H_B_BLACK_ARCHER,	0, 0,	128,	0,	// index id,   activated flag,   anim height for HP bar,   animation size (width),    seeding size
	"DC\\Heroes\\rhb\\rog_h_b_%c.CL2",	0,	"DC\\Heroes\\rlb\\rog%c%i.WAV",		0,
	1,	"X\\plrtrn\\Archer02black01upd1.TRN",	// TRNs:   flag and path
	8, 8, 11, 6, 20, 0,		// IWAHDS frames
	3, 0, 0, 0, 2, 0,		// delays
	"Black Archer",			// name
	0,	0,	62,		// DLVL range & MLVL
	800,	1'000, // HP range,
	AI_7_Goat_Archer,	/*MF_9_CHASE_TARGET | */MF_10_OPEN_DOORS, 4,	// AI, int flag, int level
	60,	7,	73,	84,// tohit, hit frame of 1st attack, dmg range.
	65,	8,	85,	95,	 // 2nd attack: tohit, hit frame, dmg range
	90, 2, MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,	MR_1_ARCAN | MR_2_FIRE | MR_3_LIGHT,// armor, mon type, and resistances
	0,		3,	4'300,		// special flag for drop, animation size, XP
	0, 0 // ice age  TRN & name
},

//--------------------------------END OF DC HEROES---------------------
};
