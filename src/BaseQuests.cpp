#include "stdafx.h"

// BaseQuest array at 0x004B6908
BaseQuest BaseQuests[Quest_Count] = {
	// id,                           flag,                          spDungeonStart,   mpDungeonStart,   classicStart,     dungeonType,   spQuestDungeon,                 mpQuestDungeon,         retRow, retCol, speechIndex,                            questName
	{ Q_0_THE_MAGIC_ROCK,            QF_CLASSIC|QF_SINGLE,          DUN_5_CATACOMB_1, DUN_NO,           DUN_5_CATACOMB_1, DT_M1_CUR,     DUN_NO,                         DUN_NO,                          1,  0, SP_115,                                 "The Magic Rock"            }, 
	{ Q_1_BLACK_MUSHROOM,            QF_CLASSIC|QF_SINGLE,          DUN_12_CAVE_4,    DUN_NO,           DUN_9_CAVE_1,     DT_M1_CUR,     DUN_NO,                         DUN_NO,                          1,  0, SP_128,                                 "Black Mushroom"            }, 
	{ Q_2_GHARBAD_THE_WEAK,          QF_CLASSIC|QF_SINGLE,          DUN_11_CAVE_3,    DUN_NO,           DUN_4_CHURCH_4,   DT_M1_CUR,     DUN_NO,                         DUN_NO,                          1,  0, SP_144,                                 "Gharbad The Weak"          }, 
	{ Q_3_ZHAR_THE_MAD,              QF_CLASSIC|QF_SINGLE,          DUN_8_CATACOMB_4, DUN_NO,           DUN_8_CATACOMB_4, DT_M1_CUR,     DUN_NO,                         DUN_NO,                          1,  0, SP_148,                                 "Zhar the Mad"              }, 
	{ Q_4_LACHDANAN,                 QF_CLASSIC|QF_SINGLE,          DUN_14_HELL_2,    DUN_NO,           DUN_14_HELL_2,    DT_M1_CUR,     DUN_NO,                         DUN_NO,                          1,  0, SP_81,                                  "Lachdanan"                 }, 
	{ Q_5_DIABLO,                    QF_CLASSIC|QF_SINGLE|QF_MULTI, DUN_15_HELL_3,    DUN_15_HELL_3,    DUN_15_HELL_3,    DT_M1_CUR,     DUN_NO,                         DUN_NO,                          1,  0, SP_25_KAIN_ABOUT_DIABLO,                "Diablo"                    }, 
	{ Q_6_THE_BUTCHER,               QF_CLASSIC|QF_SINGLE|QF_MULTI, DUN_1_CHURCH_1,   DUN_1_CHURCH_1,   DUN_2_CHURCH_2,   DT_M1_CUR,     DUN_NO,                         DUN_NO,                          1,  0, SP_63,                                  "The Butcher"               }, 
	{ Q_7_OGDENS_SIGN,               QF_CLASSIC|QF_SINGLE,          DUN_4_CHURCH_4,   DUN_NO,           DUN_4_CHURCH_4,   DT_M1_CUR,     DUN_NO,                         DUN_NO,                          1,  0, SP_12,                                  "Ogden's Sign"              }, 
	{ Q_8_HALLS_OF_THE_BLIND,        QF_CLASSIC|QF_SINGLE,          DUN_7_CATACOMB_3, DUN_NO,           DUN_7_CATACOMB_3, DT_M1_CUR,     DUN_NO,                         DUN_NO,                          1,  0, SP_237,                                 "Halls of the Blind"        }, 
	{ Q_9_VALOR,					 QF_CLASSIC|QF_SINGLE,          DUN_5_CATACOMB_1, DUN_NO,           DUN_5_CATACOMB_1, DT_2_CATACOMB, DUN_58_GATEWAY_OF_BLOOD,        DUN_NO,                          0,  1, SP_236,                                 "Valor"                     },
	{ Q_10_ANVIL_OF_FURY,            QF_CLASSIC|QF_SINGLE|QF_MULTI, DUN_10_CAVE_2,    DUN_10_CAVE_2,    DUN_10_CAVE_2,    DT_M1_CUR,     DUN_NO,                         DUN_NO,                          1,  0, SP_88,                                  "Anvil of Fury"             }, 
	{ Q_11_WARLORD_OF_BLOOD,         QF_CLASSIC|QF_SINGLE,          DUN_13_HELL_1,    DUN_NO,           DUN_13_HELL_1,    DT_M1_CUR,     DUN_NO,                         DUN_NO,                          1,  0, SP_238,                                 "Warlord of Blood"          }, 
	{ Q_12_THE_CURSE_OF_KING_LEORIC, QF_CLASSIC|QF_SINGLE|QF_MULTI, DUN_3_CHURCH_3,   DUN_21_CRYPT_1,   DUN_3_CHURCH_3,   DT_1_CHURCH,   DUN_25_KING_LEORICS_TOMB,       DUN_NO,                          1,  0, SP_1,                                   "The Curse of King Leoric"  }, 
	{ Q_13_POISONED_WATER_SUPPLY,    QF_CLASSIC|QF_SINGLE|QF_MULTI, DUN_2_CHURCH_2,   DUN_2_CHURCH_2,   DUN_2_CHURCH_2,   DT_3_CAVE,     DUN_28_POISONED_WATER_SUPPLY,   DUN_28_POISONED_WATER_SUPPLY,    0,  1, SP_39,                                  "Poisoned Water Supply"     }, 
	{ Q_14_THE_CHAMBER_OF_BONE,      QF_CLASSIC|QF_SINGLE|QF_MULTI, DUN_6_CATACOMB_2, DUN_7_CATACOMB_3, DUN_6_CATACOMB_2, DT_2_CATACOMB, DUN_109_HALL_OF_HEROES,         DUN_109_HALL_OF_HEROES,          1, -1, SP_235,                                 "The Chamber of Bone"       },
	{ Q_15_ARCHBISHOP_LAZARUS,       QF_CLASSIC|QF_SINGLE|QF_MULTI, DUN_15_HELL_3,    DUN_15_HELL_3,    DUN_15_HELL_3,    DT_1_CHURCH,   DUN_29_ARCHBISHOP_LAZARUS_LAIR, DUN_NO,                          1,  0, SP_23,                                  "Archbishop Lazarus"        }, 
	{ Q_16_HORAZON_S_DEMONS,                    QF_SINGLE|QF_MULTI, DUN_23_CRYPT_3,   DUN_23_CRYPT_3,   DUN_23_CRYPT_3,   DT_1_CHURCH,   DUN_32_HORAZONS_HAVEN,          DUN_32_HORAZONS_HAVEN,           0,  1, SP_361,                                 "Horazon's Demons"          }, 
	{ Q_17_FALLEN_ANGEL,                        QF_SINGLE|QF_MULTI, DUN_19_ABYSS_3,   DUN_19_ABYSS_3,   DUN_19_ABYSS_3,   DT_3_CAVE,     DUN_35_PASSAGE_OF_FIRE,         DUN_35_PASSAGE_OF_FIRE,          1,  0, SP_364,                                 "The Fallen Angel"          }, 
	{ Q_18_LITTLE_GIRL,                         QF_SINGLE,          DUN_17_ABYSS_1,   DUN_NO,           DUN_17_ABYSS_1,   DT_M1_CUR,     DUN_NO,                         DUN_NO,                          1,  0, SP_283,                                 "Celia's Toy"               }, 
	{ Q_19_LOST_TREASURE,                       QF_SINGLE|QF_MULTI, DUN_0_TOWN,       DUN_0_TOWN,       DUN_0_TOWN,       DT_1_CHURCH,   DUN_37_DISUSED_RELIQUARY,       DUN_37_DISUSED_RELIQUARY,        0,  1, SP_374,                                 "Lost Treasure"             }, // могила слева от входа в церковь, открывается картой, три этажа, лед
	{ Q_20_SUNLESS_SEA,                         QF_SINGLE|QF_MULTI, DUN_11_CAVE_3,    DUN_11_CAVE_3,    DUN_11_CAVE_3,    DT_3_CAVE,     DUN_40_ISLAND,                  DUN_40_ISLAND,                   1,  1, SP_381,                                 "Island of the Sunless Sea" }, 
	{ Q_21_THE_CURSED_BLADE,                    QF_SINGLE|QF_MULTI, DUN_14_HELL_2,    DUN_14_HELL_2,    DUN_14_HELL_2,    DT_4_HELL,     DUN_41_FLESHDOOM,               DUN_41_FLESHDOOM,                1,  0, SP_389,                                 "The Cursed Blade"          },
	{ Q_22_MAIDEN_OF_ANGUISH,                   QF_SINGLE|QF_MULTI, DUN_0_TOWN,       DUN_0_TOWN,       DUN_0_TOWN,       DT_4_HELL,     DUN_30_HALLS_OF_ANGUISH,        DUN_30_HALLS_OF_ANGUISH,         0,  1, SP_296,                                 "Maiden of Anguish"         }, 
	{ Q_23_INFESTED_CELLAR,                     QF_SINGLE|QF_MULTI, DUN_0_TOWN,       DUN_0_TOWN,       DUN_0_TOWN,       DT_3_CAVE,     DUN_27_THE_INFESTED_CELLAR,     DUN_27_THE_INFESTED_CELLAR,      1,  0, SP_300,                                 "Infested Cellar"           },
	{ Q_24_THE_RAMPAGING_DEMON,                 QF_SINGLE,          DUN_0_TOWN,       DUN_0_TOWN,       DUN_0_TOWN,       DT_0_TOWN,     DUN_44_RAVENHOLM,               DUN_NO,                          1,  0, SP_402_OGDEN_THE_RAMPAGING_DEMON_START, "Rampaging Demon"           },
	{ Q_25_THE_LOCKED_CHEST,                    QF_SINGLE|QF_MULTI, DUN_18_ABYSS_2,   DUN_18_ABYSS_2,   DUN_18_ABYSS_2,   DT_3_CAVE,     DUN_47_DARK_NEST,               DUN_47_DARK_NEST,                1,  0, SP_408,                                 "The Locked Chest"          },
	{ Q_26_SPECIAL_WEAPON,                      QF_SINGLE|QF_MULTI, DUN_20_ABYSS_4,   DUN_20_ABYSS_4,   DUN_20_ABYSS_4,   DT_3_CAVE,     DUN_48_BLOOD_HIVE,              DUN_48_BLOOD_HIVE,               1,  0, SP_411,                                 "Special Weapon"            },
	{ Q_27_DEMON_IN_THE_TOWN,                   QF_SINGLE|QF_MULTI, DUN_0_TOWN,       DUN_0_TOWN,       DUN_0_TOWN,       DT_3_CAVE,     DUN_49_SICK_VILLAGERS_BASEMENT, DUN_49_SICK_VILLAGERS_BASEMENT,  1,  0, SP_415,                                 "Demon in the Town"         },
	{ Q_28_VISIONS_OF_VICTORY,                  QF_SINGLE|QF_MULTI, DUN_22_CRYPT_2,   DUN_22_CRYPT_2,   DUN_22_CRYPT_2,   DT_1_CHURCH,   DUN_50_TEMPLE_OF_SUNSET,        DUN_50_TEMPLE_OF_SUNSET,         0,  1, SP_418,                                 "Visions of Victory"        },
	{ Q_29_IRON_FIGURINE,                       QF_SINGLE|QF_MULTI, DUN_12_CAVE_4,    DUN_12_CAVE_4,    DUN_12_CAVE_4,    DT_3_CAVE,     DUN_51_FETID_CAVERN,            DUN_51_FETID_CAVERN,             1,  0, SP_420,                                 "Iron Figurine"             },
	{ Q_30_DEMON_PLAGUE,                        QF_SINGLE|QF_MULTI, DUN_9_CAVE_1,     DUN_9_CAVE_1,     DUN_9_CAVE_1,     DT_3_CAVE,     DUN_52_MOSSY_GROTTO,            DUN_52_MOSSY_GROTTO,             1,  1, SP_423,                                 "Demon Plague"              },
	{ Q_31_GOOD_DEAL,                           QF_SINGLE,          DUN_8_CATACOMB_4, DUN_NO,           DUN_8_CATACOMB_4, DT_2_CATACOMB, DUN_53_VAULTKEEP,               DUN_NO,                         -1,  0, SP_426,                                 "Good Deal"                 },
	{ Q_32_A_SIMPLE_TASK,                       QF_SINGLE|QF_MULTI, DUN_21_CRYPT_1,   DUN_21_CRYPT_1,   DUN_21_CRYPT_1,   DT_1_CHURCH,   DUN_54_ABANDONED_FANE,          DUN_54_ABANDONED_FANE,           0,  1, SP_429,                                 "A Simple Task"             },
	{ Q_33_STRANGE_BOOK,                        QF_SINGLE|QF_MULTI, DUN_17_ABYSS_1,   DUN_17_ABYSS_1,   DUN_17_ABYSS_1,   DT_3_CAVE,     DUN_56_WEB_TUNNELS,             DUN_56_WEB_TUNNELS,              1,  0, SP_432,                                 "Strange Book"              },
	{ Q_34_GRAVE_MATTERS,                       QF_SINGLE|QF_MULTI, DUN_2_CHURCH_2,   DUN_2_CHURCH_2,   DUN_2_CHURCH_2,   DT_1_CHURCH,   DUN_110_BLACK_CHAPEL,           DUN_110_BLACK_CHAPEL,            1,  0, SP_435,                                 "Grave Matters"             },
	{ Q_35_IRON_OATH,                           QF_SINGLE|QF_MULTI, DUN_1_CHURCH_1,   DUN_1_CHURCH_1,   DUN_1_CHURCH_1,   DT_1_CHURCH,   DUN_111_CHURCH_SACRISTY,        DUN_111_CHURCH_SACRISTY,         1,  0, SP_437,                                 "Iron Oath"                 },
	{ Q_36_CLEAR_WATER,                         QF_SINGLE|QF_MULTI, DUN_11_CAVE_3,    DUN_11_CAVE_3,    DUN_11_CAVE_3,    DT_3_CAVE,     DUN_112_STILL_SPRINGS,          DUN_112_STILL_SPRINGS,           1,  0, SP_440,                                 "Clear Water"               },
	{ Q_37_PRICE_OF_HUBRIS,                     QF_SINGLE|QF_MULTI, DUN_13_HELL_1,    DUN_13_HELL_1,    DUN_13_HELL_1,    DT_4_HELL,     DUN_113_THE_MAZE,               DUN_113_THE_MAZE,                1,  0, SP_443,                                 "The Price of Hubris"       },
	{ Q_38_OGDENS_SIGN_MP,                                QF_MULTI, DUN_NO,           DUN_4_CHURCH_4,   DUN_NO,           DT_1_CHURCH,   DUN_NO,                         DUN_114_LOWER_SCRIPTORIUM,       0,  1, SP_12,                                  "Ogden's Sign"              },
};

BaseDungeon Dungeons[DungeonCount] = {
	// id,                             genType,       graphType,  level, quest, name,                     warpName
	{ DUN_0_TOWN,                      DT_0_TOWN,     DT_0_TOWN,      0, 0, "Tristram Town"            , "Town"                     , 0 },
	{ DUN_1_CHURCH_1,                  DT_1_CHURCH,   DT_1_CHURCH,    1, 0, "Church 1"                 , "Level 1"                  , 0 },
	{ DUN_2_CHURCH_2,                  DT_1_CHURCH,   DT_1_CHURCH,    2, 0, "Church 2"                 , "Level 2"                  , 0 },
	{ DUN_3_CHURCH_3,                  DT_1_CHURCH,   DT_1_CHURCH,    3, 0, "Church 3"                 , "Level 3"                  , 0 },
	{ DUN_4_CHURCH_4,                  DT_1_CHURCH,   DT_1_CHURCH,    4, 0, "Church 4"                 , "Level 4"                  , 0 },
	{ DUN_5_CATACOMB_1,                DT_2_CATACOMB, DT_2_CATACOMB,  5, 0, "Catacomb 1"               , "Level 5"                  , 0 },
	{ DUN_6_CATACOMB_2,                DT_2_CATACOMB, DT_2_CATACOMB,  6, 0, "Catacomb 2"               , "Level 6"                  , 0 },
	{ DUN_7_CATACOMB_3,                DT_2_CATACOMB, DT_2_CATACOMB,  7, 0, "Catacomb 3"               , "Level 7"                  , 0 },
	{ DUN_8_CATACOMB_4,                DT_2_CATACOMB, DT_2_CATACOMB,  8, 0, "Catacomb 4"               , "Level 8"                  , 0 },
	{ DUN_9_CAVE_1,                    DT_3_CAVE,     DT_3_CAVE,      9, 0, "Cave 1"                   , "Level 9"                  , 0 },
	{ DUN_10_CAVE_2,                   DT_3_CAVE,     DT_3_CAVE,     10, 0, "Cave 2"                   , "Level 10"                 , 0 },
	{ DUN_11_CAVE_3,                   DT_3_CAVE,     DT_3_CAVE,     11, 0, "Cave 3"                   , "Level 11"                 , 0 },
	{ DUN_12_CAVE_4,                   DT_3_CAVE,     DT_3_CAVE,     12, 0, "Cave 4"                   , "Level 12"                 , 0 },
	{ DUN_13_HELL_1,                   DT_4_HELL,     DT_4_HELL,     13, 0, "Hell 1"                   , "Level 13"                 , 0 },
	{ DUN_14_HELL_2,                   DT_4_HELL,     DT_4_HELL,     14, 0, "Hell 2"                   , "Level 14"                 , 0 },
	{ DUN_15_HELL_3,                   DT_4_HELL,     DT_4_HELL,     15, 0, "Hell 3"                   , "Level 15"                 , 0 },
	{ DUN_16_HELL_4,                   DT_4_HELL,     DT_4_HELL,     16, 0, "Hell 4"                   , "Level 16"                 , 0 },
	{ DUN_17_ABYSS_1,                  DT_3_CAVE,     DT_6_ABYSS,    17, 0, "Abyss 1"                  , "Level 17"                 , 0 },
	{ DUN_18_ABYSS_2,                  DT_3_CAVE,     DT_6_ABYSS,    18, 0, "Abyss 2"                  , "Level 18"                 , 0 },
	{ DUN_19_ABYSS_3,                  DT_3_CAVE,     DT_6_ABYSS,    19, 0, "Abyss 3"                  , "Level 19"                 , 0 },
	{ DUN_20_ABYSS_4,                  DT_3_CAVE,     DT_6_ABYSS,    20, 0, "Abyss 4"                  , "Level 20"                 , 0 },
	{ DUN_21_CRYPT_1,                  DT_1_CHURCH,   DT_5_CRYPT,    21, 0, "Crypt 1"                  , "Level 21"                 , 0 },
	{ DUN_22_CRYPT_2,                  DT_1_CHURCH,   DT_5_CRYPT,    22, 0, "Crypt 2"                  , "Level 22"                 , 0 },
	{ DUN_23_CRYPT_3,                  DT_1_CHURCH,   DT_5_CRYPT,    23, 0, "Crypt 3"                  , "Level 23"                 , 0 },
	{ DUN_24_CRYPT_4,                  DT_1_CHURCH,   DT_5_CRYPT,    24, 0, "Crypt 4"                  , "Level 24"                 , 0 },
	{ DUN_25_KING_LEORICS_TOMB,        DT_1_CHURCH,   DT_1_CHURCH,    3, 1, "King Leoric's Tomb"       , "King Leoric's Tomb"		, 0 },
	{ DUN_26_BONE_CHAMBER,             DT_2_CATACOMB, DT_2_CATACOMB,  6, 1, "Bone Chamber"             , "the Chamber of Bone"		, 0 },
	{ DUN_27_THE_INFESTED_CELLAR,      DT_3_CAVE,     DT_3_CAVE,     12, 1, "the Infested Cellar"      , "the Infested Cellar"		, 0 },
	{ DUN_28_POISONED_WATER_SUPPLY,    DT_3_CAVE,     DT_3_CAVE,      2, 1, "Poisoned Water Supply"    , "a Dark Passage"			, 0 },
	{ DUN_29_ARCHBISHOP_LAZARUS_LAIR,  DT_1_CHURCH,   DT_1_CHURCH,   15, 1, "Archbishop Lazarus' Lair" , "Unholy Altar"				, 0 },
	{ DUN_30_HALLS_OF_ANGUISH,         DT_4_HELL,     DT_4_HELL,     13, 1, "Halls of Anguish"         , "the Halls of Anguish"		, "(Danger: high)" },
	{ DUN_31_ANDARIELS_THRONE,         DT_4_HELL,     DT_4_HELL,     14, 1, "Andariel's Throne"        , "Andariel's Throne"		, 0 },
	{ DUN_32_HORAZONS_HAVEN,           DT_1_CHURCH,   DT_5_CRYPT,    23, 1, "Horazon's Haven"          , "Horazon's Sanctum"		, 0 },
	{ DUN_33_SUMMONING_ROOMS,          DT_1_CHURCH,   DT_5_CRYPT,    23, 1, "Summoning Rooms"          , "Summoning Rooms"			, 0 },
	{ DUN_34_THE_PIT,                  DT_1_CHURCH,   DT_5_CRYPT,    23, 1, "The Pit"                  , "the Pit"					, 0 },
	{ DUN_35_PASSAGE_OF_FIRE,          DT_3_CAVE,     DT_3_CAVE,     15, 1, "Passage of Fire"          , "the Passage of Fire"		, 0 },
	{ DUN_36_VALLEY_OF_DESPAIR,        DT_3_CAVE,     DT_6_ABYSS,    20, 1, "Valley of Despair"        , "the Valley of Despair"	, 0 },
	{ DUN_37_DISUSED_RELIQUARY,        DT_1_CHURCH,   DT_1_CHURCH,    8, 1, "Disused Reliquary"        , "Disused Reliquary"		, "(Danger: high)" },
	{ DUN_38_GLACIAL_CAVERNS,          DT_3_CAVE,     DT_3_CAVE,      9, 1, "Glacial Caverns"          , "Glacial Caverns"			, 0 },
	{ DUN_39_FROZEN_LOCH,              DT_3_CAVE,     DT_3_CAVE,     10, 1, "Frozen Loch"              , "Frozen Loch"				, 0 },
	{ DUN_40_ISLAND,                   DT_3_CAVE,     DT_3_CAVE,     12, 1, "Island of the Sunless Sea", "the Sunless Sea Island"	, 0 },
	{ DUN_41_FLESHDOOM,                DT_4_HELL,     DT_4_HELL,     17, 1, "Pit of Flame"             , "the Pit of Flame" 		, 0 },
	{ DUN_42_WIELDER,                  DT_4_HELL,     DT_4_HELL,     17, 1, "City of the Damned"       , "the City of the Damned"	, 0 },
	{ DUN_43_HEPHASTO,                 DT_4_HELL,     DT_4_HELL,     18, 1, "Hellforge"                , "the Hellforge" 			, 0 },
	{ DUN_44_RAVENHOLM,                DT_0_TOWN,     DT_0_TOWN,      7, 1, "Ravenholm"                , "Ravenholm"				, 0 },
	{ DUN_45_THE_DEN,                  DT_2_CATACOMB, DT_2_CATACOMB,  8, 1, "The Den"                  , "the Den"					, 0 },
	{ DUN_46_COLISEUM,                 DT_2_CATACOMB, DT_2_CATACOMB,  1, 1, "Coliseum"                 , "Coliseum"					, 0 },
	{ DUN_47_DARK_NEST,				   DT_3_CAVE,	  DT_6_ABYSS,	 18, 1, "Dark Nest"				   , "the Dark Nest"			, 0 },
	{ DUN_48_BLOOD_HIVE,			   DT_3_CAVE,	  DT_6_ABYSS,	 20, 1, "Blood Hive"			   , "the Blood Hive"			, 0 },
	{ DUN_49_SICK_VILLAGERS_BASEMENT,  DT_3_CAVE,	  DT_3_CAVE,	  5, 1, "Sick Villager's Basement" , "Sick Villager's Home"		, 0 },
	{ DUN_50_TEMPLE_OF_SUNSET,		   DT_1_CHURCH,	  DT_5_CRYPT,	 22, 1, "Temple of Sunset"		   , "the Temple of Sunset"		, 0 },
	{ DUN_51_FETID_CAVERN,			   DT_3_CAVE,     DT_3_CAVE,	 12, 1, "Fetid Cavern"			   , "the Fetid Cavern"			, 0 },
	{ DUN_52_MOSSY_GROTTO,			   DT_3_CAVE,     DT_3_CAVE,	  9, 1, "Mossy Grotto"			   , "the Mossy Grotto"			, 0 },
	{ DUN_53_VAULTKEEP,				   DT_2_CATACOMB, DT_2_CATACOMB,  8, 1, "Vaultkeep"				   , "the Vaultkeep"			, 0 },
	{ DUN_54_ABANDONED_FANE,		   DT_1_CHURCH,	  DT_5_CRYPT,	 21, 1, "Abandoned Fane"		   , "the Abandoned Fane"		, 0 },
	{ DUN_55_DEEP_HOLLOW,			   DT_3_CAVE,	  DT_6_ABYSS,	 18, 1, "Deep Hollow"			   , "the Deep Hollow"			, 0 },
	{ DUN_56_WEB_TUNNELS,			   DT_3_CAVE,	  DT_6_ABYSS,	 17, 1, "Web Tunnels"			   , "the Web Tunnels"			, 0 },
	{ DUN_57_ARACHNID_LAIR,			   DT_3_CAVE,	  DT_6_ABYSS,	 17, 1, "Arachnid Lair"			   , "the Arachnid Lair"		, 0 },
	{ DUN_58_GATEWAY_OF_BLOOD,		   DT_2_CATACOMB, DT_2_CATACOMB,  5, 1, "Gateway of Blood"		   , "the Gateway of Blood"		, 0 },
	{ DUN_59_HALL_OF_FIRE,			   DT_2_CATACOMB, DT_2_CATACOMB,  5, 1, "Hall of Fire"			   , "the Hall of Fire"			, 0 },
	{ DUN_60_HAUNTED_LABYRINTH,		   DT_1_CHURCH,	  DT_1_CHURCH,	  2, 1, "Haunted Labyrinth"		   , "Haunted Labyrinth"		, 0 },
	{ DUN_61_CURSED_SANCTUM,		   DT_1_CHURCH,	  DT_1_CHURCH,	  2, 1, "Cursed Sanctum"		   , "the Cursed Sanctum"		, 0 },
	{ DUN_62_DIM_UNDERCROFT,		   DT_1_CHURCH,	  DT_1_CHURCH,	  3, 1, "Dim Undercroft"		   , "the Dim Undercroft"		, 0 },
	{ DUN_63_FORGOTTEN_OSSUARY,		   DT_1_CHURCH,	  DT_1_CHURCH,	  3, 1, "Forgotten Ossuary"		   , "Forgotten Ossuary"		, 0 },
	{ DUN_64_MAUSOLEUM,				   DT_1_CHURCH,	  DT_1_CHURCH,	  4, 1, "Mausoleum"				   , "the Mausoleum"			, 0 },
	{ DUN_65_RUINED_TEMPLE,			   DT_1_CHURCH,	  DT_1_CHURCH,	  4, 1, "Ruined Temple"			   , "the Ruined Temple"		, 0 },
	{ DUN_66_SEWERS,				   DT_2_CATACOMB, DT_2_CATACOMB,  5, 1, "Sewers"				   , "the Sewers"				, 0 },
	{ DUN_67_SEPULCHER,				   DT_2_CATACOMB, DT_2_CATACOMB,  6, 1, "Sepulcher"				   , "Sepulcher"				, 0 },
	{ DUN_68_OUBLIETTE,				   DT_2_CATACOMB, DT_2_CATACOMB,  6, 1, "Oubliette"				   , "the Oubliette"			, 0 },
	{ DUN_69_GIBBET,				   DT_2_CATACOMB, DT_2_CATACOMB,  7, 1, "Gibbet"				   , "the Gibbet"				, 0 },
	{ DUN_70_CITADEL,				   DT_2_CATACOMB, DT_2_CATACOMB,  7, 1, "Citadel"				   , "the Citadel"				, 0 },
	{ DUN_71_GRIM_VAULTS,			   DT_2_CATACOMB, DT_2_CATACOMB,  8, 1, "Grim Vaults"			   , "Grim Vaults"				, 0 },
	{ DUN_72_NECROPOLIS,			   DT_2_CATACOMB, DT_2_CATACOMB,  8, 1, "Necropolis"			   , "Necropolis"				, 0 },
	{ DUN_73_LAVA_PITS,				   DT_3_CAVE,	  DT_3_CAVE,	  9, 1, "Lava Pits"				   , "Lava Pits"				, 0 },
	{ DUN_74_CRATER,				   DT_3_CAVE,	  DT_3_CAVE,	  9, 1, "Crater"				   , "Crater"					, 0 },
	{ DUN_75_RIVER_OF_FIRE,			   DT_3_CAVE,	  DT_3_CAVE,	 10, 1, "River of Fire"			   , "River of Fire"			, 0 },
	{ DUN_76_DEN_OF_EVIL,			   DT_3_CAVE,	  DT_3_CAVE,	 10, 1, "Den of Evil"			   , "Den of Evil"				, 0 },
	{ DUN_77_VILE_TUNNELS,			   DT_3_CAVE,	  DT_3_CAVE,	 11, 1, "Vile Tunnels"			   , "Vile Tunnels"				, 0 },
	{ DUN_78_THE_BRIDGE,			   DT_3_CAVE,	  DT_3_CAVE,	 11, 1, "The Bridge"			   , "The Bridge"				, 0 },
	{ DUN_79_COLONNADE,				   DT_3_CAVE,	  DT_3_CAVE,	 12, 1, "Colonnade"				   , "Colonnade"				, 0 },
	{ DUN_80_HELL_GATE,				   DT_3_CAVE,	  DT_3_CAVE,	 12, 1, "Hell Gate"				   , "Hell Gate"				, 0 },
	{ DUN_81_UNDERWORLD,			   DT_4_HELL,     DT_4_HELL,	 13, 1, "Underworld"			   , "the Underworld"			, 0 },
	{ DUN_82_HALLS_OF_SUFFERING,	   DT_4_HELL,     DT_4_HELL,	 13, 1, "Halls of Suffering"	   , "Halls of Suffering"		, 0 },
	{ DUN_83_BLOOD_KEEP,			   DT_4_HELL,     DT_4_HELL,	 14, 1, "Blood Keep"			   , "Blood Keep"				, 0 },
	{ DUN_84_ARMORIES_OF_HELL,		   DT_4_HELL,     DT_4_HELL,	 14, 1, "Armories of Hell"		   , "Armories of Hell"			, 0 },
	{ DUN_85_DEMON_STRONGHOLD,		   DT_4_HELL,     DT_4_HELL,	 15, 1, "Demon Stronghold"		   , "Demon Stronghold"			, 0 },
	{ DUN_86_VALLEY_OF_WRATH,		   DT_4_HELL,     DT_4_HELL,	 15, 1, "Valley of Wrath"		   , "Valley of Wrath"			, 0 },
	{ DUN_87_PANDEMONIUM,			   DT_4_HELL,     DT_4_HELL,	 16, 1, "Pandemonium"			   , "Pandemonium"				, 0 },
	{ DUN_88_REALM_OF_TERROR,		   DT_4_HELL,     DT_4_HELL,	 16, 1, "Realm of Terror"		   , "Realm of Terror"			, 0 },
	{ DUN_89_BEDLAM,				   DT_4_HELL,     DT_4_HELL,	 16, 1, "Bedlam"				   , "Bedlam"					, 0 },
	{ DUN_90_FIELDS_OF_MADNESS,		   DT_3_CAVE,	  DT_6_ABYSS,	 17, 1, "Fields of Madness"		   , "Fields of Madness"		, 0 },
	{ DUN_91_STYGIAN_PASSAGE,		   DT_3_CAVE,	  DT_6_ABYSS,	 17, 1, "Stygian Passage"		   , "Stygian Passage"			, 0 },
	{ DUN_92_FLESH_PIT,				   DT_3_CAVE,	  DT_6_ABYSS,	 18, 1, "Flesh Pit"				   , "Flesh Pit"				, 0 },
	{ DUN_93_BLEEDING_VEINS_PATH,	   DT_3_CAVE,	  DT_6_ABYSS,	 18, 1, "Bleeding Veins Path"	   , "Bleeding Veins Path"		, 0 },
	{ DUN_94_ROT_CHAMBERS,			   DT_3_CAVE,	  DT_6_ABYSS,	 19, 1, "Rot Chambers"			   , "Rot Chambers"				, 0 },
	{ DUN_95_MARROW_GARDEN,			   DT_3_CAVE,	  DT_6_ABYSS,	 19, 1, "Marrow Garden"			   , "Marrow Garden"			, 0 },
	{ DUN_96_BONE_MIRE,				   DT_3_CAVE,	  DT_6_ABYSS,	 20, 1, "Bone Mire"				   , "Bone Mire"				, 0 },
	{ DUN_97_GLOOMY_DEPTHS,			   DT_3_CAVE,	  DT_6_ABYSS,	 20, 1, "Gloomy Depths"			   , "Gloomy Depths"			, 0 },
	{ DUN_98_EBON_CHASM,			   DT_3_CAVE,	  DT_6_ABYSS,	 20, 1, "Ebon Chasm"			   , "Ebon Chasm"				, 0 },
	{ DUN_99_ANCIENT_CRYPTS,		   DT_1_CHURCH,	  DT_5_CRYPT,	 21, 1, "Ancient Crypts"		   , "Ancient Crypts"			, 0 },
	{ DUN_100_HORADRIM_TREASURY,	   DT_1_CHURCH,	  DT_5_CRYPT,	 21, 1, "Horadrim Treasury"		   , "Horadrim Treasury"		, 0 },
	{ DUN_101_VOID_CHAMBERS,		   DT_1_CHURCH,	  DT_5_CRYPT,	 22, 1, "Void Chambers"			   , "Void Chambers"			, 0 },
	{ DUN_102_OBLIVION_TRAIL,		   DT_1_CHURCH,	  DT_5_CRYPT,	 22, 1, "Oblivion Trail"		   , "Oblivion Trail"			, 0 },
	{ DUN_103_ALTAR_OF_IMMOLATION,	   DT_1_CHURCH,	  DT_5_CRYPT,	 23, 1, "Altar of Immolation"	   , "Altar of Immolation"		, 0 },
	{ DUN_104_NETHERWORLD_GATES,	   DT_1_CHURCH,	  DT_5_CRYPT,	 23, 1, "Netherworld Gates"		   , "Netherworld Gates"		, 0 },
	{ DUN_105_DARK_DESCENT,			   DT_1_CHURCH,	  DT_5_CRYPT,	 23, 1, "Dark Descent"			   , "Dark Descent"				, 0 },
	{ DUN_106_BASTION_OF_NIGHT,		   DT_1_CHURCH,	  DT_5_CRYPT,	 24, 1, "Bastion of Night"		   , "Bastion of Night"			, 0 },
	{ DUN_107_DREAD_SANCTUARY,		   DT_1_CHURCH,	  DT_5_CRYPT,	 24, 1, "Dread Sanctuary"		   , "Dread Sanctuary"			, 0 },
	{ DUN_108_ARCANE_PRISON,		   DT_1_CHURCH,	  DT_5_CRYPT,	 24, 1, "Arcane Prison"			   , "Arcane Prison"			, 0 },
	{ DUN_109_HALL_OF_HEROES,		   DT_2_CATACOMB, DT_2_CATACOMB,  6, 1, "Hall of Heroes"		   , "the Hall of Heroes"		, 0 },
	{ DUN_110_BLACK_CHAPEL,			   DT_1_CHURCH,	  DT_1_CHURCH,	  2, 1, "Black Chapel"			   , "the Black Chapel"			, 0 },
	{ DUN_111_CHURCH_SACRISTY,		   DT_1_CHURCH,	  DT_1_CHURCH,	  1, 1, "Church Sacristy"		   , "the Church Sacristy"		, 0 },
	{ DUN_112_STILL_SPRINGS,		   DT_3_CAVE,	  DT_3_CAVE,	 12, 1, "Still Springs"			   , "the Still Springs"		, 0 },
	{ DUN_113_THE_MAZE,				   DT_4_HELL,	  DT_4_HELL,	 14, 1, "Maze"					   , "the Maze"					, 0 },
	{ DUN_114_LOWER_SCRIPTORIUM,	   DT_1_CHURCH,	  DT_1_CHURCH,	  4, 1, "Lower Scriptorium"		   , "the Lower Scriptorium"	, 0 }

};

DungeonType DungeonTypes[DT_COUNT] = {
	// id,              name,         floor
	{ DT_0_TOWN,        "Town",       0 }, 
	{ DT_1_CHURCH,      "Church",     CHR_WAFM_13_FLOOR },
	{ DT_2_CATACOMB,    "Catacomb",   CATA_WAFM_3_FLOOR },
	{ DT_3_CAVE,        "Cave",       CAVE_WAFM_7_FLOOR },
	{ DT_4_HELL,        "Hell",       HELL_WAFM_6_FLOOR },
	{ DT_5_CRYPT,       "Crypt",      CHR_WAFM_13_FLOOR },
	{ DT_6_ABYSS,       "Abyss",      HELL_WAFM_6_FLOOR },
};

CurDungeon::operator DUNGEON() const
{
	return Players[CurrentPlayerIndex].dungeon;
}

void CurDungeon::operator=(DUNGEON dungeon)
{
	Players[CurrentPlayerIndex].dungeon = dungeon;
}

DUNGEON CurDungeon::operator+(int i)
{
	DUNGEON dungeon = DUNGEON(Players[CurrentPlayerIndex].dungeon + i);
	if( dungeon >= DungeonCount ){
		dungeon = DUN_0_TOWN;
	}
	return dungeon;
}

DUNGEON CurDungeon::operator-(int i)
{
	DUNGEON dungeon = DUNGEON(Players[CurrentPlayerIndex].dungeon - i);
	if( dungeon < DUN_0_TOWN ){
		dungeon = DUNGEON(DungeonCount - 1);
	}
	return dungeon;
}

const BaseDungeon* CurDungeon::operator->()
{
	return &Dungeons[Players[CurrentPlayerIndex].dungeon];
}
