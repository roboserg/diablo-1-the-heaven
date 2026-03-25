#include "stdafx.h"

std::tuple<int, int> GetMonsterStunThresholdRange(const Monster& monster) {// #monster stun threshold range
//	int baseMon = monster.SpritePtr->baseMonsterIndex;
	if (GameMode == GM_CLASSIC) {
		return std::make_tuple(2 + monster.MonsterLevel, 4 + monster.MonsterLevel);
	}
	else {
		int ac = monster.ArmorClass;
		switch (monster.SpritePtr->baseMonsterIndex) {
			case BM_29_HIDDEN or BM_30_SHADOW or BM_31_UNSEEN or BM_32_INVISIBLE_ONE or BM_92_ILLUSION_WEAVER or BM_94_LURKER
			or BM_111_OBSIDIAN_GOLEM or BM_125_GOLD_GOLEM or BM_130_BLACK_GOLEM or BM_183_STALKER or BM_184_REALITY_WEAVER or BM_257_SHROUDED:
				ac = 3 * ac / 2;
				break;
				case BM_69_WINGED_DEMON or BM_81_FLYING_DEMON or BM_82_GARGOYLE or BM_84_DEATH_WING or BM_141_SHAMBLER or BM_142_HELL_MEAT or BM_190_WINGED_DEMON
				or BM_193_ABATAN or BM_231_BEASTMASTER_SUMMON_3 or BM_251_WENDIGO or BM_252_HEPHASTO or BM_286_BRINE_CORPSE:
					ac = 8 * ac;
					break;
					case BM_87_DARK_REAPER or BM_120_NECROMORPH or BM_135_GROTESQUE or BM_136_BLOOD_SLAYER or BM_152_BLACK_REAPER or BM_153_DEATH_MINION or BM_154_DEATH_MINION
					or BM_294_BELHIFET_UNO or BM_295_BELHIFET_DOS:
						ac = 8 * (ac - 20);
						break;
					case BM_236_CORPSE_EATER or BM_245_BLUNDERER or BM_292_BUGBEAR:
						ac = 6 * ac;
						break;
					case BM_6_GRAVE_DIGGER or BM_15_TOMB_SLAVE or BM_144_DARK_SERVANT or BM_145_BLACK_MOURNER or BM_288_EARTH_GOLEM:
						ac = 5 * ac + 1;
						break;
					case BM_90_CAVE_SNAKE:
						ac = 4 * (ac - 16);
						break;
					case BM_91_GOLD_SERPENT:
						ac = 4 * (ac - 8);
						break;
						case BM_3_FAMILIAR or BM_7_RISEN_SKELETON or BM_9_SKELETON_WARRIOR or BM_10_BURNING_DEAD or BM_17_SCAVENGER or BM_19_RAVAGER
							or BM_20_RISEN_SKELETON or BM_21_SKELETON_WARRIOR or BM_22_BURNING_DEAD or BM_23_MALICE or BM_24_RISEN_SKELETON or BM_25_SKELETON_WARRIOR or BM_26_BURNING_DEAD
							or BM_40_QUASIT or BM_41_DARK_SERAPH or BM_48_ACID_BAT or BM_101_DARK_MISTRESS or BM_102_SUCCUBUS or BM_103_BLOOD_WITCH or BM_104_DARK_WITCH
							or BM_117_STINGER or BM_119_DARK_GHOST or BM_131_FIRE_BAT or BM_133_SOUL_BURNER
						or BM_143_DOOM_TAIL or BM_191_DARK_MISTRESS or BM_202_SUCCUBUS or BM_206_ACID_SPITTER or BM_278_LEAPER or BM_279_FLOATER or BM_280_FLOATER_X2 or BM_284_SIN_SPAWN:
							ac = 3 * ac;
							break;
						case BM_33_MOON_CLAN or BM_57_HELL_CLAN or BM_166_GOATLORD_BOSS or BM_188_HELL_CLAN or BM_214_SATYR_LORD or BM_272_SPIDER_QUEEN or BM_274_FLESH_DEVOURER:
							ac = 5 * ac;
							break;
						case BM_5_FALLEN_ONE or BM_281_FALLEN_BOW:
							if (Difficulty == 0) {
								switch (Dungeon->level) {
								case 2: ac = 12; break;
								case 3: ac = 21; break;
								case 4: ac = 30; break;
								default: ac *= 4; break;
								}
							}
							else {
								ac *= 4;
							}
							break;
						case BM_0_BURNED_CORPSE:
							ac = Difficulty == 0 && Dungeon->level == 2 ? 15 : 4 * ac;
							break;
						case BM_58_DEFILER or BM_271_THORNED_HULK:
							ac = 10 * ac;
							break;
						case BM_110_DIABLO:
							ac = 9 * ac;
							break;
						case BM_138_SHADOW_BEAST or BM_139_BONE_GASHER:
							ac = 3 * (ac - 3);
							break;
						case BM_177_PHASE_DEMON or BM_264_BITELING:
							ac = 2 * ac;
							break;
						case BM_194_BLOODIED_HIDDEN:
							ac = ac + 120;
							break;
						case BM_222_NIGHT_DEMON or BM_267_BONE_BLADE or BM_268_BONE_BLADE_2 or BM_275_BEHEMOTH:
							ac = 6 * ac;
							break;
						case BM_283_FROST_WARDEN or BM_260_ARMAGEDDON_WALKER or BM_261_SKELETAL_KNIGHT:
							ac = 7 * ac;
							break;
						case BM_50_SKELETON_LORD:
							if (MaxCountOfPlayersInGame != 1) {
								ac = 23 * ac / 5; // +15% in MP
							}
							else {
								ac *= 4;
							}
							break;
						default:
							ac *= 4;
							break;
		}
		LimitToMin(ac, 2);
		return std::make_tuple(ac / 2, 3 * ac / 2);// 50 to 133 % of AC*4 (= 200% to 600%  AC)
	}
}

void __fastcall ScaleMonsterStunThreshold(int& monster_stun_threshold) {
	if (GameMode == GM_HARD) {
		monster_stun_threshold += monster_stun_threshold / 5;
	}
	else if (is(GameMode, GM_EASY/*, GM_CLASSIC*/)) {
		monster_stun_threshold -= monster_stun_threshold / 5;
	}
}

int __fastcall RollMonsterStunThreshold(const Monster& monster) {
	int min, max;
	tie(min, max) = GetMonsterStunThresholdRange(monster);
	int monster_stun_threshold = min + RNG(max - min); // stun threshold: random  (mAC * 2 - mAC * 4)
	ScaleMonsterStunThreshold(monster_stun_threshold);
	LimitToMin(monster_stun_threshold, 1);
	return monster_stun_threshold;
}

int __fastcall GetMonsterLifeStunThreshold(const Monster& monster) {
	return ( (monster.CurrentLife * 3) + (monster.BaseLife / 2 ) ) / 512; // 6-43% of base HP must be damaged to stun monster
}
