#include "stdafx.h"

// BaseMissile array at 0x004A7968
BaseMissile BaseMissiles[MI_count] = {
	// BaseMissileIndex, CastFunc, InteractionFunc, HasSprite, EffectType, DamageType, Image, AfterEffect, CastSound, ActSound
/* 0	*/ { MI_0_NONE_ARROW,	CastArrow,	ActArrow,	1,	MT_0_ARROW,	ET_0_PHYSICAL,	MIS_0_ARROWS,	0,	S_M1_NO_SOUND,	S_95_ARROWALL },													//	Arrow
/* 1	*/ { MI_1_FIREBOLT,	CastFirebolt,	ActFireArcanBolt,	1,	MT_1_MAGIC,	ET_1_FIRE,	MIS_1_FIREBA,	MI_9_MAGMA_BALL_EX,	S_109_FBOLT1,	S_112_FIRIMP2 }, 											//	Firebolt ?
/* 2	*/ { MI_2_HYDRA,	CastHydra,	ActHydra,	1,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_2_GUARD,	0,	S_119_GUARD,	S_120_GRDLANCH }, 														//	Guardian ?
/* 3	*/ { MI_3_PHASING,	CastPhasing,	ActTeleport,	0,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_143_TELEPORT,	S_M1_NO_SOUND }, 												//	Phasing ?
/* 4	*/ { MI_4_LIGHTNING_NOVA_SEGMENT,	CastLightingNovaSegment,	ActLightningNovaSegment,	1,	MT_1_MAGIC,	ET_2_LIGHTNING,	MIS_3_LGHNING,	0,	S_M1_NO_SOUND,	S_105_ELECIMP1 }, 		//	Lightning ball ?
/* 5	*/ { MI_5_FIRE_WALL_SEGMENT,	CastSpell5_FireWallSegment,	ActFireWallSegment,	1,	MT_1_MAGIC,	ET_1_FIRE,	MIS_4_FIREWAL,	0,	S_145_WALLLOOP,	S_112_FIRIMP2 }, 					// Fire wall segment,	Fire trap
/* 6	*/ { MI_6_FIREBLAST,	CastFireblast,	ActFireblast,	1,	MT_1_MAGIC,	ET_1_FIRE,	MIS_1_FIREBA,	0,	S_109_FBOLT1,	S_108_FBALL }, 												//	Fireball  ?
/* 7	*/ { MI_7_LIGHTNING,	CastLightning,	ActLightning,	0,	MT_1_MAGIC,	ET_2_LIGHTNING,	MIS_3_LGHNING,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 										//	Player Lightning ?
/* 8	*/ { MI_8_LIGHTING_SEGMENT,	Cast_Lightning_Segment,	ActLightningSegment,	1,	MT_1_MAGIC,	ET_2_LIGHTNING,	MIS_3_LGHNING,	0,	S_126_LNING1,	S_105_ELECIMP1 }, 						// Player lightning segment
/* 9	*/ { MI_9_MAGMA_BALL_EX,	CastMagicExplosion,	ActMagicExplosion,	1,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_5_MAGBLOS,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 						//	Magic Blast
/* 10	*/ { MI_10_TOWN_PORTAL,	CastTownPortal,	ActTownPortal,	1,	MT_1_MAGIC,	ET_3_ARCAN,	MIS_6_PORTAL,	0,	S_136_SENTINEL,	S_106_ELEMENTL }, 											//	TownPortal
/* 11	*/ { MI_11_FLASH_FR,	CastFlashForward,	ActFlashForward,	1,	MT_1_MAGIC,	ET_3_ARCAN,	MIS_7_BLUEXFR,	0,	S_130_NOVA,	S_105_ELECIMP1 }, 										//	Flash ?
/* 12	*/ { MI_12_FLASH_BACK,	CastFlashBack,	ActFlashBack,	1,	MT_1_MAGIC,	ET_3_ARCAN,	MIS_8_BLUEXBK,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 											//	Flash ?
/* 13	*/ { MI_13_MANA_SHIELD,	CastManaShield,	ActManaShield,	1,	MT_1_MAGIC,	ET_3_ARCAN,	MIS_9_MANASHLD,	0,	S_128_MSHIELD,	S_M1_NO_SOUND }, 											//	Mana Shield
/* 14	*/ { MI_14_HELLFIRE_SEGMENT,	CastHellfireSegment,	ActForcewaveSegment,	1,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_25_KRULL,	0,	S_M1_NO_SOUND,	S_113_FLAMWAVE }, 				//	Flame wave segment?
/* 15	*/ { MI_15_ARCANE_NOVA,	CastNova,	ActNova,	1,	MT_1_MAGIC,	ET_3_ARCAN,	MIS_3_LGHNING,	MI_24_ARCANE_STAR,	S_114_FLASH,	S_M1_NO_SOUND }, 
/* 16	*/ { MI_16_NULL_SPELL_1,	CastFireblast,	(void (__fastcall *)(int))ActTrash16SpellEffect,	1,	MT_1_MAGIC,	ET_2_LIGHTNING,	MIS_49_MS_BLA,	0,	S_126_LNING1,	S_127_LTNING }, 
/* 17	*/ { MI_17_BLOOD,	CastBlood,	ActSpecialEffect,	1,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_10_BLOOD,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 
/* 18	*/ { MI_18_BONE,	CastBone,	ActSpecialEffect,	1,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_11_BONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 										//	Bone spirit ?
/* 19	*/ { MI_19_METLHIT,	CastMetlhit,	ActSpecialEffect,	1,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_12_METLHIT,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 									//	Stone curse ?
/* 20	*/ { MI_20_RUN_ATTACK,	CastRunAttack,	ActMonstersRun,	1,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 									//	20 Horned Demon Run
/* 21	*/ { MI_21_MAGMA_BALL,	CastMagmaBall,	ActFireArcanBolt,	1,	MT_1_MAGIC,	ET_1_FIRE,	MIS_24_MAGBALL,	MI_9_MAGMA_BALL_EX,	S_M1_NO_SOUND,	S_1670},
/* 22	*/ { MI_22_MONSTER_LIGHTNING,	CastLightning,	ActLightning,	0,	MT_1_MAGIC,	ET_2_LIGHTNING,	MIS_21_THINLGHT,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 							//	Monster lightning ?
/* 23	*/ { MI_23_MONSTER_LIGHTING_SEGMENT,	Cast_Lightning_Segment,	ActLightningSegment,	1,	MT_1_MAGIC,	ET_2_LIGHTNING,	MIS_21_THINLGHT,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 		// Monster lightning segment ?
/* 24	*/ { MI_24_ARCANE_STAR,	CastArcaneStar,	ActFireArcanBolt,	1,	MT_1_MAGIC,	ET_3_ARCAN,	MIS_22_FLARE,	MI_25_ARCANE_STAR_EX,	S_M1_NO_SOUND,	S_619_SOR_101 }, 										//	Blood star ?
/* 25	*/ { MI_25_ARCANE_STAR_EX,	CastMagicExplosion,	ActMagicExplosion,	1,	MT_2_MAGIC_EXPLODE,	ET_3_ARCAN,	MIS_23_FLAREEXP,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 
/* 26	*/ { MI_26_TELEPORT,	CastTeleport,	ActTeleport,	0,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_106_ELEMENTL,	S_M1_NO_SOUND }, 											//	Teleport
/* 27	*/ { MI_27_FIRE_ARROW,	CastMagicArrow,	ActMagicArrow,	1,	MT_0_ARROW,	ET_1_FIRE,	MIS_13_FARROW,	0,	S_111_FIRIMP1,	S_112_FIRIMP2 }, 											//	Fire arrow
/* 28	*/ { MI_28_BALL_LIGHTNING,	CastBallLightning,	ActHolyBolt,	1,	MT_1_MAGIC,	ET_2_LIGHTNING,	MIS_49_MS_BLA,	0,	S_141_STORM,	S_103_CHLTNING }, 
/* 29	*/ { MI_29_FIRE_TRAP,	CastSpell29,	ActFireWallSegment,	1,	MT_2_MAGIC_EXPLODE,	ET_1_FIRE,	MIS_4_FIREWAL,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 								//	Fire trap ?
/* 30	*/ { MI_30_STONE_CURSE,	CastStoneCurse,	ActStoneCurse,	1,	MT_1_MAGIC,	ET_3_ARCAN,	MIS_M1_NONE,	0,	S_135_SCURIMP,	S_M1_NO_SOUND }, 											//	Stone curse ?
/* 31	*/ { MI_31_NULL_SPELL_3,	CastSpell31_Emply,	ActNullSpell,	1,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 
/* 32	*/ { MI_32_HOLY_NOVA,	CastNova,	ActNova,	1,	MT_1_MAGIC,	ET_5_HOLY,	MIS_27_HOLY,	MI_53_HOLY_BOLT,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 
/* 33	*/ { MI_33_GOLEM,	CastGolem,	ActGolem,	0,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 														//	Golem
/* 34	*/ { MI_34_ETHEREAL,	CastEthereal,	ActEthereal,	1,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_34_ETHRSHLD,	0,	S_107_DSERP,	S_M1_NO_SOUND }, 										//	Etherealize ?
/* 35	*/ { MI_35_FURY_SPLASH,	CastFurySplash,	ActSpecialEffect,	1,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_16_BLODBUR,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 
/* 36	*/ { MI_36_APOC_EXPLODE,	CastApocExplode,	ActApocExplode,	1,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_17_NEWEXP,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 							//	Apocalypse explode?
/* 37	*/ { MI_37_HEALING,	CastHealing,	ActNullSpell,	0,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 												//	Healing ?
/* 38	*/ { MI_38_FIRE_WALL,	CastMagicWall,	ActFireWall,	0,	MT_1_MAGIC,	ET_1_FIRE,	MIS_4_FIREWAL,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 											//	FireWall ?
/* 39	*/ { MI_39_INFRAVISION,	CastInfravision,	ActInfravision,	0,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_123_INVISIBL,	S_M1_NO_SOUND }, 										//	Infravision ?
/* 40	*/ { MI_40_IDENTIFY,	CastIdentify,	ActNullSpell,	0,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 											//	Identify
/* 41	*/ { MI_41_HELLFIRE,	Cast_Forcewave,	ActForcewave,	1,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_4_FIREWAL,	0,	S_113_FLAMWAVE,	S_M1_NO_SOUND }, 											//	Flame wave ?
/* 42	*/ { MI_42_LIGHTNING_NOVA,	CastNova,	ActNova,	1,	MT_1_MAGIC,	ET_2_LIGHTNING,	MIS_3_LGHNING,	MI_4_LIGHTNING_NOVA_SEGMENT,	S_130_NOVA,	S_M1_NO_SOUND }, 									//	Nova ?
/* 43	*/ { MI_43_FURY,	CastFury,	ActFury,	0,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 														//	Fury (Rage)
/* 44	*/ { MI_44_APOCALYPSE,	CastApocalypse,	ActApocalypse,	1,	MT_1_MAGIC,	ET_3_ARCAN,	MIS_17_NEWEXP,	0,	S_94_AP,	S_M1_NO_SOUND }, 												//	Apocalypse
/* 45	*/ { MI_45_ITEM_REPAIR,	CastItemRepair,	ActNullSpell,	0,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 									//	Item repair
/* 46	*/ { MI_46_STAFF_RECHARGE,	CastStaffRecharge,	ActNullSpell,	0,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 							//	Staff recharge ?
/* 47	*/ { MI_47_TRAP_DISARM,	CastTrapDisarm,	ActNullSpell,	0,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_142_TRAPDIS,	S_M1_NO_SOUND }, 									//	Trap disarm
/* 48	*/ { MI_48_INCINERATE_SEGMENT,	CastIncinerateSegment,	ActIncinerateSegment,	1,	MT_1_MAGIC,	ET_1_FIRE,	MIS_20_INCINERATE,	0,	S_140_SPOUTSTR,	S_M1_NO_SOUND }, 					//	Incinerate ?
/* 49	*/ { MI_49_INCINERATE,	CastIncinerate,	ActIncinerate,	0,	MT_1_MAGIC,	ET_1_FIRE,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 
/* 50	*/ { MI_50_RUN_ATTACK_2,	CastSomeAnotherMonsterRun,	ActSomeAnotherMonsterRun,	1,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 
/* 51	*/ { MI_51_KRULL,	CastSpell51_KRULL,	ActSpell51_KRULL,	1,	MT_0_ARROW,	ET_1_FIRE,	MIS_25_KRULL,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 
/* 52	*/ { MI_52_CHARGED_BOLT,	CastChargedBolt,	ActChargedBolt,	1,	MT_1_MAGIC,	ET_2_LIGHTNING,	MIS_26_MINILTNG,	0,	S_102_CBOLT,	S_83_CAST3 }, 								//	Charged	bolt молния
/* 53	*/ { MI_53_HOLY_BOLT,	CastHolyBolt,	ActHolyBolt,	1,	MT_1_MAGIC,	ET_5_HOLY,	MIS_27_HOLY,	0,	S_121_HOLYBOLT,	S_105_ELECIMP1 }, 											//	Holy bolt ?
/* 54	*/ { MI_54_RISING,	CastSpellRising,	ActNullSpell,	0,	MT_1_MAGIC,	ET_3_ARCAN,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_133_RESUR }, 												//	Ressurect ?
/* 55	*/ { MI_55_TELEKINES,	CastTelekinesis,	ActNullSpell,	0,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_107_DSERP,	S_M1_NO_SOUND }, 											//	Telekinesis ?
/* 56	*/ { MI_56_LIGHTING_ARROW,	CastMagicArrow,	ActMagicArrow,	1,	MT_0_ARROW,	ET_2_LIGHTNING,	MIS_29_LARROW,	0,	S_83_CAST3,	S_105_ELECIMP1 }, 										//	Lightning arrow
/* 57	*/ { MI_57_ACID_MISSILE,	CastAcidMissile,	ActFireArcanBolt,	1,	MT_1_MAGIC,	ET_4_ACID,	MIS_31_ACIDBF,	MI_58_ACID_SPLASH,	S_92_ACIDS1,	S_M1_NO_SOUND }, 
/* 58	*/ { MI_58_ACID_SPLASH,	CastMagicExplosion,	ActAcidSplash,	1,	MT_2_MAGIC_EXPLODE,	ET_4_ACID,	MIS_32_ACIDSPLA,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 
/* 59	*/ { MI_59_ACID_POOLS,	CastSpell59AcidPools,	ActSpell59AcidPools,	1,	MT_2_MAGIC_EXPLODE,	ET_4_ACID,	MIS_33_ACIDPUD,	0,	S_132_PUDDLE,	S_M1_NO_SOUND }, 
/* 60	*/ { MI_60_HEAL_OTHER,	CastHealOther,	ActNullSpell,	0,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 											//	Heal other ?
/* 61	*/ { MI_61_ELEMENTAL,	CastElemental,	ActElemental,	1,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_35_FIRERUN,	0,	S_106_ELEMENTL,	S_1671_ELM_HIT }, 											//	Elemental ?
/* 62	*/ { MI_62_RISING_2,	Cast62_RisingLight,	ActSpell62_RisingLight,	1,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_36_RESSUR1,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 
/* 63	*/ { MI_63_BONE_SPIRIT,	CastBoneSpirit,	ActBoneSpirit,	1,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_37_SKLBALL,	0,	S_99_BONESP,	S_100_BSIMPCT }, 											//	Bone Spirit ?
/* 64	*/ { MI_64_WEAPON_ELEMENTAL_DAMAGE,	CastWeaponElementalDamage,	ActWeaponElementalDamage,	1,	MT_0_ARROW,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 		//	Weapon fire	damage
/* 65	*/ { MI_65_RED_PORTAL,	CastRedPortal,	ActRedPortal,	1,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_38_RPORTAL,	0,	S_136_SENTINEL,	S_106_ELEMENTL }, 
/* 66	*/ { MI_66_MONSTER_APOC_EXPLODE,	CastApocExplode,	ActApocExplode,	1,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_39_FIREPLAR,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 
/* 67	*/ { MI_67_MONSTER_APOC,	CastMonsterApoc,	ActNullSpell,	0,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 
/* 68	*/ { MI_68_MANA_RECHARGE,	CastManaRecharge,	ActNullSpell,	0,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 									//	Mana ?
/* 69	*/ { MI_69_MAGI,	CastMagi,	ActNullSpell,	0,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 													//	Magi ?
/* 70	*/ { MI_70_LIGHTNING_WALL_SEGMENT,	CastSpell70LightingWallSegment,	ActSpell70LightningWallSegment,	1,	MT_1_MAGIC,	ET_2_LIGHTNING,	MIS_3_LGHNING,	0,	S_147_LMAG,	S_105_ELECIMP1 }, 
/* 71	*/ { MI_71_LIGHTING_WALL,	CastMagicWall,	ActLightningWall,	0,	MT_1_MAGIC,	ET_2_LIGHTNING,	MIS_3_LGHNING,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 								//	Lightning wall ?
/* 72	*/ { MI_72_FIRE_NOVA,	CastNova,	ActNova,	1,	MT_1_MAGIC,	ET_1_FIRE,	MIS_1_FIREBA,	MI_74_FIREBOLT_ARROW,	S_M1_NO_SOUND/*S_109_FBOLT1*/,	S_M1_NO_SOUND/*S_112_FIRIMP2*/ }, 											//	Fiery nova
/* 73	*/ { MI_73_SPECIAL_ARROW,	CastSpecialArrowSpell,	ActSpecialArrowSpell,	1,	MT_0_ARROW,	ET_0_PHYSICAL,	MIS_0_ARROWS,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 						//	Unused ?
/* 74	*/ { MI_74_FIREBOLT_ARROW,	CastFireNovaSegment,	ActFireblast,	1,	MT_1_MAGIC,	ET_1_FIRE,	MIS_1_FIREBA,	0,	S_13_FBALLBOW,	S_108_FBALL }, 									//	Fire nova segment
/* 75	*/ { MI_75_LIGHTNING_ARROW,	CastLightningAsArrow,	ActLightningAsArrow,	0,	MT_1_MAGIC,	ET_2_LIGHTNING,	MIS_3_LGHNING,	0,	S_13_FBALLBOW,	S_M1_NO_SOUND }, 
/* 76	*/ { MI_76_CHARGED_BOLT_ARROW,	CastChargedBoltArrow,	ActChargedBolt,	1,	MT_1_MAGIC,	ET_2_LIGHTNING,	MIS_26_MINILTNG,	0,	S_102_CBOLT,	S_M1_NO_SOUND }, 
/* 77	*/ { MI_77_HOLY_BOLT_ARROW,	CastMagicArrow,	ActMagicArrow,	1,	MT_0_ARROW,	ET_5_HOLY,	MIS_59_HARROW,	0,	S_1107_HOLY_ARROW_SOUND,	S_105_ELECIMP1 },
/* 78	*/ { MI_78_WARP,	CastWarp,	ActTeleport,	0,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_121_HOLYBOLT,	S_M1_NO_SOUND }, 														//	Warp
/* 79	*/ { MI_79_REFLECT,	CastReflect,	ActReflect,	1,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_47_REFLECT,	0,	S_128_MSHIELD,	S_M1_NO_SOUND }, 													//	Reflect ?
/* 80	*/ { MI_80_BERSERK,	CastBerserk,	ActNullSpell,	0,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 												//	Berserk // Отключено в 1.150
/* 81	*/ { MI_81_FLAME_RING,	Cast_Fire_or_Lightning_Ring,	ActFireRing,	0,	MT_1_MAGIC,	ET_1_FIRE,	MIS_4_FIREWAL,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 										//	Ring of fire ?
/* 82	*/ { MI_82_STEAL_BOTTLES_FORM_BELT,	CastSpell82_StealBottlesFromBelt,	ActNullSpell,	0,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 
/* 83	*/ { MI_83_MANA_TRAP,	CastManaTrap,	ActNullSpell,	0,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_87_CAST7,	S_M1_NO_SOUND }, 												//	Mana trap ?
/* 84	*/ { MI_84_LIGHTING_RING,	Cast_Fire_or_Lightning_Ring,	ActLightningRing,	0,	MT_1_MAGIC,	ET_2_LIGHTNING,	MIS_3_LGHNING,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 
/* 85	*/ { MI_85_SEARCH,	CastNullSpell,	ActNullSpell,	0,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 														//	Search ?
/* 86	*/ { MI_86_SOME_ANOTHER_FLASH_FR,	CastNullSpell,	ActSomeAnotherFlashFr,	1,	MT_1_MAGIC,	ET_3_ARCAN,	MIS_7_BLUEXFR,	0,	S_M1_NO_SOUND,	S_105_ELECIMP1 }, 
/* 87	*/ { MI_87_SOME_ANOTHER_FLASH_BK,	CastSomeAnotherFlashBk,	ActSomeAnotherFlashBk,	1,	MT_1_MAGIC,	ET_3_ARCAN,	MIS_8_BLUEXBK,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 
/* 88	*/ { MI_88_FIREBALL,	CastSpell88FireBall,	ActSpell88FireBall,	1,	MT_1_MAGIC,	ET_1_FIRE,	MIS_1_FIREBA,	0,	S_109_FBOLT1,	S_112_FIRIMP2 }, 
/* 89	*/ { MI_89_RUNE_OF_FIRE,	CastRuneOfFire,	ActRune,	1,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_52_RGLOWS1,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 											//	Rune of Fire ?
/* 90	*/ { MI_90_RUNE_OF_LIGHTNING,	CastRuneOfLighting,	ActRune,	1,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_52_RGLOWS1,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 									//	Rune of Lightning ?
/* 91	*/ { MI_91_RUNE_OF_GREAT_LIGHTNING,	CastRuneOfGreatLightning,	ActRune,	1,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_52_RGLOWS1,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 						//	Greater Rune of Lightning ?
/* 92	*/ { MI_92_RUNE_OF_IMMOLATION,	CastRuneOfImmolation,	ActRune,	1,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_52_RGLOWS1,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 								//	Greater Rune of Fire ?
/* 93	*/ { MI_93_RUNE_OF_STONE,	CastRuneOfStone,	ActRune,	1,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_52_RGLOWS1,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 										//	Rune of Stone ?
/* 94	*/ { MI_94_BIG_FIRE_EX,	CastExplosiveRuneExplode,	ActAbyssEnterExplodeFragment,	1,	MT_1_MAGIC,	ET_1_FIRE,	MIS_19_BIGEXP,	0,	S_129_NESTXPLD,	S_129_NESTXPLD },				//	Rune of FireBall ?
/* 95	*/ { MI_95_SPAWN_MEAT_BALL,	CastSpawnMeatBall,	ActSpawnMeatBall,	1,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_46_SPAWNS,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 						//	Spawning ?
/* 96	*/ { MI_96_JESTER,	CastJester,	ActNullSpell,	0,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 											//	Jester ?
/* 97	*/ { MI_97_ENTER_TO_ABYSS_EXPLODE,	CastAbyssEnterExplode,	ActNullSpell,	0,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND },
/* 98	*/ { MI_98_LICH_ORA_BLAST,	CastArcaneStar,	ActFireArcanBolt,	1,	MT_1_MAGIC,	ET_3_ARCAN,	MIS_48_MS_ORA,	MI_107_LICH_ORA_EX,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 
/* 99	*/ { MI_99_BLUE_BLAST,	CastArcaneStar,	ActFireArcanBolt,	1,	MT_1_MAGIC,	ET_3_ARCAN,	MIS_56_MS_BLB,	MI_105_BLU_EX,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 
/* 100	*/ { MI_100_RED_BLAST,	CastArcaneStar,	ActFireArcanBolt,	1,	MT_1_MAGIC,	ET_3_ARCAN,	MIS_50_MS_REB,	MI_104_RED_EX,	S_1217,	S_M1_NO_SOUND }, 
/* 101	*/ { MI_101_YELOW_BLAST,	CastArcaneStar,	ActFireArcanBolt,	1,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_51_MS_YEB,	MI_103_YEL_EX,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, 
/* 102	*/ { MI_102_BLUE_BLAST_2,	CastArcaneStar,	ActFireArcanBolt,	1,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_56_MS_BLB,	MI_106_BLU_EX,	S_103_CHLTNING,	S_M1_NO_SOUND }, 
/* 103	*/ { MI_103_YEL_EX,	CastMagicExplosion,	ActMagicExplosion,	1,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_53_EX_YEL2,	0,	S_112_FIRIMP2,	S_M1_NO_SOUND }, 
/* 104	*/ { MI_104_RED_EX,	CastMagicExplosion,	ActMagicExplosion,	1,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_55_EX_RED3,	0,	S_112_FIRIMP2,	S_M1_NO_SOUND }, 
/* 105	*/ { MI_105_BLU_EX,	CastMagicExplosion,	ActMagicExplosion,	1,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_54_EX_BLU2,	0,	S_112_FIRIMP2,	S_M1_NO_SOUND }, 
/* 106	*/ { MI_106_BLU_EX,	CastMagicExplosion,	ActMagicExplosion,	1,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_58_EX_BLU3,	0,	S_112_FIRIMP2,	S_M1_NO_SOUND }, 
/* 107	*/ { MI_107_LICH_ORA_EX,	CastMagicExplosion,	ActMagicExplosion,	1,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_57_EX_ORA1,	0,	S_112_FIRIMP2,	S_M1_NO_SOUND }, 
/* 108	*/ { MI_108_ARCANE_ARROW,	CastMagicArrow,	ActMagicArrow,	1,	MT_0_ARROW,	ET_3_ARCAN,	MIS_61_ARCARROW,	0,	S_1108_ARCANE_ARROW_SHOOT_SOUND,	S_1109_ARCANE_ARROW_HIT_SOUND },
/* 109	*/ { MI_109_ACID_ARROW,	CastMagicArrow,	ActMagicArrow,	1,	MT_0_ARROW,	ET_4_ACID,	MIS_63_ACIDARROW,	0,	S_M1_NO_SOUND,	S_1109_ARCANE_ARROW_HIT_SOUND },
/* 110	*/ { MI_110_FLASK_OF_FIRE,	CastFlaskThrow,	ActFlaskThrow,	1,	MT_1_MAGIC,	ET_1_FIRE,	MIS_65_FLASK_ORANGE,	0,	S_39_I_FLIPPOT,	S_M1_NO_SOUND },
/* 111	*/ { MI_111_FLASK_OF_LIGHTNING,	CastFlaskThrow,	ActFlaskThrow,	1,	MT_1_MAGIC,	ET_2_LIGHTNING,	MIS_69_FLASK_BLUE,	0,	S_39_I_FLIPPOT,	S_M1_NO_SOUND },
/* 112	*/ { MI_112_FLASK_OF_ACID,	CastFlaskThrow,	ActFlaskThrow,	1,	MT_1_MAGIC,	ET_4_ACID,	MIS_66_FLASK_ACID,	0,	S_39_I_FLIPPOT,	S_M1_NO_SOUND },
/* 113	*/ { MI_113_FLASK_OF_FIREBOMB,	CastFlaskThrow,	ActFlaskThrow,	1,	MT_1_MAGIC,	ET_1_FIRE,	MIS_67_FLASK_BLACK,	0,	S_39_I_FLIPPOT,	S_M1_NO_SOUND },
/* 114	*/ { MI_114_FLASK_OF_LIGHTNINGBOMB,	CastFlaskThrow,	ActFlaskThrow,	1,	MT_1_MAGIC,	ET_2_LIGHTNING,	MIS_68_FLASK_WHITE,	0,	S_39_I_FLIPPOT,	S_M1_NO_SOUND },
/* 115	*/ { MI_115_FLASK_OF_ACIDBOMB,	CastFlaskThrow,	ActFlaskThrow,	1,	MT_1_MAGIC,	ET_4_ACID,	MIS_70_FLASK_BEIGE,	0,	S_39_I_FLIPPOT,	S_M1_NO_SOUND },
/* 116	*/ { MI_116_FLASK_FIRE_EX,	CastMagicExplosion,	ActMagicExplosion,	1,	MT_2_MAGIC_EXPLODE,	ET_1_FIRE,	MIS_19_BIGEXP,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND },
/* 117	*/ { MI_117_FLASK_ACID_EX,	CastMagicExplosion,	ActMagicExplosion,	1,	MT_2_MAGIC_EXPLODE,	ET_4_ACID,	MIS_32_ACIDSPLA,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND },
/* 118	*/ { MI_118_FLASK_OF_ARCANEBOMB,	CastFlaskThrow,	ActFlaskThrow,	1,	MT_1_MAGIC,	ET_3_ARCAN,	/*MIS_55_EX_RED3*/MIS_71_FLASK_CRIMSON,	0,	S_39_I_FLIPPOT,	S_M1_NO_SOUND },
/* 119	*/ { MI_119_TRAP_OF_ARROWS,	CastTrapThrow,	ActTrapThrow,	1,	MT_0_ARROW,	ET_0_PHYSICAL,	MIS_72_TRAP_OF_ARROWS,	0,	S_62_I_INVROCK,	S_M1_NO_SOUND },
/* 120	*/ { MI_120_TRAPPER_GUN,	CastTrapperTrap,	ActTrapperTrap,	1,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_52_RGLOWS1,	0,	S_M1_NO_SOUND/* old S_1180_TRAP_SET or S_119_GUARD*/,	S_120_GRDLANCH },
/* 121	*/ { MI_121_TRAPPER_GUN_ARROW,	CastTrapperTrapArrow,	ActTrapperTrapArrow,	1,	MT_0_ARROW,	ET_0_PHYSICAL,	MIS_0_ARROWS,	0,	S_1179_TRAP_LOOSE,	S_95_ARROWALL },
/* 122	*/ { MI_122_LESSER_SUMMON,	CastGolem,	ActGolem,	0,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND },
/* 123	*/ { MI_123_COMMON_SUMMON,	CastGolem,	ActGolem,	0,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND },
/* 124	*/ { MI_124_GREATER_SUMMON,	CastGolem,	ActGolem,	0,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND },
/* 125	*/ { MI_125_UNSUMMON,	CastUnsummon,	ActNullSpell,	0,	MT_2_MAGIC_EXPLODE,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND },
/* 126	*/ { MI_126_COLD_ARROW,	CastMagicArrow,	ActMagicArrow,	1,	MT_0_ARROW,	ET_6_COLD,	MIS_74_COLDARROW,	0,	S_M1_NO_SOUND,	S_1213_COLD2 },
/* 127	*/ { MI_127_HABIBI,	CastArcaneStar,	ActFireArcanBolt,	1,	MT_1_MAGIC,	ET_6_COLD,	MIS_75_HABIBI,	MI_128_HABIBI_EX,	S_M1_NO_SOUND,	S_1212_COLD4 }, 
/* 128	*/ { MI_128_HABIBI_EX,	CastMagicExplosion,	ActMagicExplosion,	1,	MT_2_MAGIC_EXPLODE,	ET_6_COLD,	MIS_28_HOLYEXPL,	0,	S_1212_COLD4,	S_M1_NO_SOUND }, 
/* 129	*/ { MI_129_SNOW_BOLT,	CastArcaneStar,	ActFireArcanBolt,	1,	MT_1_MAGIC,	ET_6_COLD,	MIS_76_SNOW_BOLT,	MI_130_SNOW_EXPL,	S_1214,	S_M1_NO_SOUND }, 
/* 130	*/ { MI_130_SNOW_EXPL,	CastMagicExplosion,	ActMagicExplosion,	1,	MT_2_MAGIC_EXPLODE,	ET_6_COLD,	MIS_77_SNOW_EXPL,	0,	S_1212_COLD4,	S_M1_NO_SOUND }, 
/* 131	*/ { MI_131_ARCANE_STAR_BLUE,	CastArcaneStar,	ActFireArcanBolt,	1,	MT_1_MAGIC,	ET_3_ARCAN,	MIS_78_SCUBMISBLU,	MI_132_ARCANE_STAR_EXP_BLUE,	S_M1_NO_SOUND,	S_619_SOR_101 },
/* 132	*/ { MI_132_ARCANE_STAR_EXP_BLUE,	CastMagicExplosion,	ActMagicExplosion,	1,	MT_2_MAGIC_EXPLODE,	ET_3_ARCAN,	MIS_79_SCBSEXPBLU,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND },
/* 133	*/ { MI_133_CHAIN_LIGTNING,	CastChainLightning,	ActChainLightning,	0,	MT_1_MAGIC,	ET_2_LIGHTNING,	MIS_3_LGHNING,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND },
/* 134	*/ { MI_134_FROST_NOVA,	CastNova,	ActNova,	1,	MT_1_MAGIC,	ET_6_COLD,	MIS_76_SNOW_BOLT,	MI_129_SNOW_BOLT,	S_1299,	S_M1_NO_SOUND },
/* 135	*/ { MI_135_TOXIC_BALL,	CastAcidMissile,	ActFireArcanBolt,	1,	MT_1_MAGIC,	ET_4_ACID,	MIS_31_ACIDBF,	MI_58_ACID_SPLASH,	S_1302,	S_M1_NO_SOUND },
/* 136	*/ { MI_136_ACID_NOVA,	CastNova,	ActNova,	1,	MT_1_MAGIC,	ET_4_ACID,	MIS_31_ACIDBF,	MI_135_TOXIC_BALL,	S_1296,	S_M1_NO_SOUND },
/* 137	*/ { MI_137_RANCID_BOLT,	CastAcidMissile,	ActFireArcanBolt,	1,	MT_1_MAGIC,	ET_4_ACID,	MIS_31_ACIDBF,	MI_58_ACID_SPLASH,	S_1301,	S_M1_NO_SOUND },
/* 138	*/ { MI_138_FREEZING_BALL,	CastArcaneStar,	ActFireArcanBolt,	1,	MT_1_MAGIC,	ET_6_COLD,	MIS_75_HABIBI,	MI_128_HABIBI_EX,	S_1297,	S_M1_NO_SOUND }, 
/* 139	*/ { MI_139_THROWING_KNIFE,	CastArrow,	ActArrow,	1,	MT_0_ARROW,	ET_0_PHYSICAL,	MIS_83_THROWING_KNIFE,	0,	S_1504,	S_95_ARROWALL }, //copy of MI_0_NONE_ARROW
/* 140	*/ { MI_140_EXPLODER,	CastExploder,	ActExploder,	1,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND },
/* 141	*/ { MI_141_THROWING_MALLET,	CastArrow,	ActArrow,	1,	MT_0_ARROW,	ET_0_PHYSICAL,	MIS_80_THROWING_MALLET,	0,	S_1504,	S_1509 }, //copy of MI_0_NONE_ARROW
/* 142	*/ { MI_142_AURA_HIT_GFX,	CastAuraElementalGFX,	ActAuraElementalGFX,	1,	MT_0_ARROW,	ET_0_PHYSICAL,	MIS_M1_NONE,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND },	//Aura on hit visuals
/* 143	*/ { MI_143_BULLET,	CastArrow,	ActArrow,	1,	MT_0_ARROW,	ET_0_PHYSICAL,	MIS_84_BULLET,	0,	S_M1_NO_SOUND,	S_M1_NO_SOUND }, //copy of MI_0_NONE_ARROW
/* 144	*/ { MI_144_HYDRAMANCER_CHARGED_BOLT,	CastHydramancerChargedBolt,	ActHydramancerChargedBolt,	1,	MT_1_MAGIC,	ET_2_LIGHTNING,	MIS_26_MINILTNG,	0,	S_102_CBOLT,	S_83_CAST3 }, // Charged Bolt used by Hydramancer's Hydra
/* 145	*/ { MI_145_TENTACLE,	CastTentacle,	ActTentacle,	1,	MT_1_MAGIC,	ET_0_PHYSICAL,	MIS_85_TENTACLE,	0,	S_M1_NO_SOUND,	S_1849_TENTACLIST_HIT }, 											//	Firebolt ?
};
