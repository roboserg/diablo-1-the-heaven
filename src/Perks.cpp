#include "stdafx.h"

extern Perk Perks[PERKS_COUNT];
extern int PerksPanelOrder[PERKS_MAX_800];
DisplayObject PerksPanelPos;

bool IsPerksPanelVisible = false;
bool perkUpArrowPressed = false;
bool perkDownArrowPressed = false;
bool spendPerkPointsButtonVisisble = false;
bool celLoaded = false;

char* perksCel;
char* arrow1;
char* arrow2;
char* arrow3;
char* arrow4;
int PerkPanelLine[PERK_TYPE_COUNT] = { 0 };
PERK_TYPE CurPerkPanel = PERK_TYPE_COMMON;

int AvailablePerks[PERKS_COUNT];
int AvailablePerksCount = 0;

//----- (th2) ------------------------------------------------------------
bool IsMouseOnPerkArrowUp()
{
	int offsetY = Screen_TopBorder + PerksPanelPos.Down - 640;
	int buttonLeft  = PerksPanelPos.Left + 316;
	int buttonUp    = offsetY            + 100;
	int buttonRight = buttonLeft         +  17;
	int buttonDown  = buttonUp           +  17;
	return CursorX >= buttonLeft && CursorX <= buttonRight && CursorY >= buttonUp && CursorY <= buttonDown;
}

//----- (th2) ------------------------------------------------------------
bool IsMouseOnPerkArrowDown()
{
    int offsetY = Screen_TopBorder + PerksPanelPos.Down - 640;
	int buttonLeft  = PerksPanelPos.Left + 316;
	int buttonUp    = offsetY            + 125;
	int buttonRight = buttonLeft         +  17;
	int buttonDown  = buttonUp           +  17;
	return CursorX >= buttonLeft && CursorX <= buttonRight && CursorY >= buttonUp && CursorY <= buttonDown;
}

//----- (th2) ------------------------------------------------------------
void addUnlimitedXpToPlayer( i64 xpGain, int index )
{
	Player& player = Players[index];
	if( LLONG_MAX - player.Xp < xpGain ){ // защита от переполнения
		player.Xp = LLONG_MAX;
	}else{
		player.Xp += xpGain;
	}
	if( player.Xp < LevelUp(PlayerMaxLevel() - 1) ){
		int nextLevel = 0;
		while( player.Xp >= LevelUp(nextLevel++) );
		int deltaLevel = nextLevel - player.CharLevel - 1;
		for( int levelUpCount = 0; levelUpCount < deltaLevel; levelUpCount++ ){
			NextPlayerLevel(CurrentPlayerIndex);
		}
        NetSendCmdParam1( 0, CMD_51_PLRLEVEL, player.CharLevel );
	}else{
		player.CharLevel = PlayerMaxLevel();
	}
}

//----- (th2) ------------------------------------------------------------
bool IsMouseOnPerksAddPointButton()
{
    int offsetY = Screen_TopBorder + PerksPanelPos.Down - 640;
	int buttonLeft  = PerksPanelPos.Left + 350;
	int buttonUp    = offsetY            + 432;
	int buttonRight = buttonLeft         +  41;
	int buttonDown  = buttonUp           +  18;
	return CursorX >= buttonLeft && CursorX <= buttonRight && CursorY >= buttonUp && CursorY <= buttonDown;
}

//----- (th2) ------------------------------------------------------------
void SpendPerkPoint(int perk)
{
	const Player& player = Players[CurrentPlayerIndex];
	if( perk == PERK_HERE_AND_NOW ){
		if( player.CharLevel != PlayerMaxLevel() ){
			int charLevel = player.CharLevel;
			i64 curXp = LevelUp(charLevel);
			i64 xpGain = curXp - player.Xp;
			addUnlimitedXpToPlayer( xpGain, CurrentPlayerIndex );
			Players[CurrentPlayerIndex].perk[perk]++;
			PlayGlobalSound(S_1112_PERK_01);
		}
	}else{
		Players[CurrentPlayerIndex].perk[perk]++;
		PlayGlobalSound(S_1112_PERK_01);
	}
	getAvailablePerksList();
	NetSendCmdParam2( 1, CMD_105_SETPERK, perk, Players[CurrentPlayerIndex].perk[perk] );
}

//----- (th2) ------------------------------------------------------------
void HandlePerksWindowClick()
{
	if( IsMouseOnPerkArrowUp() && perkUpArrowPressed == false ){
        perkUpArrowPressed = true;
        PerkPanel_Shift(-1);
	}
	if( IsMouseOnPerkArrowDown() && perkDownArrowPressed == false ){
        perkDownArrowPressed = true;
        PerkPanel_Shift(1);
	}
	if( IsMouseOnPerksAddPointButton() ){
        PerkPanel_ImprovePerk();
	}
}

//----- (th2) ------------------------------------------------------------
void PerkPanel_Shift(int delta)
{
	int prev = PerkPanelLine[CurPerkPanel];
	PerkPanelLine[CurPerkPanel] += delta;
	LimitToRange( PerkPanelLine[CurPerkPanel], 0, AvailablePerksCount - 1 );
	if( prev != PerkPanelLine[CurPerkPanel] ) PlayGlobalSound(S_75_I_TITLEMOV);
}

//----- (th2) ------------------------------------------------------------
void PerkPanel_ShiftLeft()
{
	#if CHEATS
	if( CanSwitchPerkSynergyPanel && CurPerkPanel > 0 ){
		(*(char*)&CurPerkPanel)--;
		getAvailablePerksList();
		PlayGlobalSound(S_75_I_TITLEMOV);
	}
	#else
	if( CanSwitchPerkSynergyPanel && CurPerkPanel != PERK_TYPE_COMMON ){
		CurPerkPanel = PERK_TYPE_COMMON;
		getAvailablePerksList();
		PlayGlobalSound(S_75_I_TITLEMOV);
	}
	#endif
}

//----- (th2) ------------------------------------------------------------
void PerkPanel_ShiftRight()
{
	#if CHEATS
	if( CanSwitchPerkSynergyPanel && CurPerkPanel < PERK_TYPE_COUNT - 1 ){
		(*(char*)&CurPerkPanel)++;
		getAvailablePerksList();
		PlayGlobalSound(S_75_I_TITLEMOV);
	}
	#else
	if( CanSwitchPerkSynergyPanel && CurPerkPanel == PERK_TYPE_COMMON ){
		CurPerkPanel = (Players[CurrentPlayerIndex].gameChanger & BIT(GC_22_BLACK_DEATH)) ? PERK_TYPE_SYMPTOM : PERK_TYPE_SYNERGY;
		getAvailablePerksList();
		PlayGlobalSound(S_75_I_TITLEMOV);
	}
	#endif
}

bool PerkButtonClicked = false;
//----- (th2) ------------------------------------------------------------
void PerkPanel_ImprovePerk()
{
    if( spendPerkPointsButtonVisisble && ! PerkButtonClicked ){
        SpendPerkPoint(AvailablePerks[PerkPanelLine[CurPerkPanel]]);
		PerkButtonClicked = true;
    }
}

//----- (th2) ------------------------------------------------------------
bool PerkAvailable( int playerIndex, int perkIndex, int perkLevel )
{
	bool available = false;
	Player& player = Players[playerIndex];
	Perk& perk = Perks[perkIndex];
	if( (uint)perkIndex < PERKS_COUNT && perk.id != -1 && perk.level[perkLevel].requiredClassMask & (1u << player.fullClassId) ){
		switch( perkIndex ){
		//case PERK_HEALER:      available = HasTrait(playerIndex, TraitId::Leper); break;
		//case PERK_TRANQUILITY: available = player.gameChanger & BIT(GC_5_LOTL); break;
		case PERK_HAGGLER:					available = (!HasTrait(playerIndex, TraitId::Leper, TraitId::Barbarian)) && (!(player.gameChanger & BIT(GC_9_NIGHT_KIN))); break;
		case PERK_SLAYER:			
		case PERK_EXPERT_KILLER:			available = !HasTrait(playerIndex, TraitId::Tormentor); break;
		case PERK_OVERDRAW:					available = (!(HasTrait(playerIndex, TraitId::Black_Witchery)))
											&& (!(HasTrait(playerIndex, TraitId::Paladin)))
											&& (!(HasTrait(playerIndex, TraitId::Ranger)))
											&& player.fullClassId != PFC_DRUID
											&& (!(player.gameChanger & BIT(GC_9_NIGHT_KIN))); break;
		case PERK_SWIFT_LEARNER:	
		case PERK_HERE_AND_NOW:				available = (!HasTrait(playerIndex, TraitId::Sisyphean))
											&& (!(player.gameChanger & BIT(GC_23_TOUGH_LUCK))); break;
		case PERK_EDUCATED:					available = (GameMode != GM_IRONMAN)
											&& (!HasTrait(playerIndex, TraitId::Sisyphean))
											&& (!(player.gameChanger & BIT(GC_23_TOUGH_LUCK))); break;
		case PERK_ACROBATICS:				available = !HasTrait(playerIndex, TraitId::BigFrame); break;
		case PERK_NIGHT_KIN:				available = player.gameChanger & BIT(GC_9_NIGHT_KIN) && HasTrait(playerIndex, TraitId::Barbarian); break; // mor: iow, this never happens (it's on purpose!)
		case PERK_LEECH_LIFE:
		case PERK_LEECH_MANA:
		case PERK_ENHANCED_AGILITY:
		case PERK_DOMINATE:
		case PERK_AVOIDANCE:				
		case PERK_RIP_AND_TEAR:				available = player.gameChanger & BIT(GC_9_NIGHT_KIN); break;
		case PERK_RIGHTEOUS_ANGER:			available = HasTrait(playerIndex, TraitId::HolyAura); break;
		case PERK_BLOODSURGE:				available = (!(HasTrait(playerIndex, TraitId::Barbarian)))
											&& (!(HasTrait(playerIndex, TraitId::Ranger))) 
											&& (!(HasTrait(playerIndex, TraitId::Pistoleer)))
											&& (!(player.gameChanger & BIT(GC_9_NIGHT_KIN)))
											&& GameMode != GM_COLISEUM; break;
		case PERK_HEAVY_DRINKER:			available = (!(HasTrait(playerIndex, TraitId::Barbarian, TraitId::Mamluk)))
											&& GameMode != GM_COLISEUM
											&& (!(player.gameChanger & BIT(GC_14_CLEAN_HANDS)))
											&& (!(player.gameChanger & BIT(GC_9_NIGHT_KIN)))
											&& (!(player.gameChanger & BIT(GC_23_TOUGH_LUCK))); break;
		case PERK_OBSESSION:				available = (!HasTrait(playerIndex, TraitId::Barbarian))
											&& GameMode != GM_COLISEUM
											&& (!(player.gameChanger & BIT(GC_14_CLEAN_HANDS)))
											&& (!(player.gameChanger & BIT(GC_9_NIGHT_KIN)))
											&& (!(player.gameChanger & BIT(GC_23_TOUGH_LUCK))); break;
		case PERK_HIT_HARDER:
		case PERK_WAR_PAINT:
		case PERK_FIRE_INSENSITIVITY:		
		case PERK_STATIC_INSENSITIVITY:		
		case PERK_MAGIC_INSENSITIVITY:		
		case PERK_ACID_INSENSITIVITY:		
		case PERK_COLD_INSENSITIVITY:		available = HasTrait(playerIndex, TraitId::Barbarian); break;
		case PERK_BLOOD_DRUNK:
		case PERK_REHABILITATION:			available = (!(HasTrait(playerIndex, TraitId::Adrenaline, TraitId::DarkPact, TraitId::GrimDeal, TraitId::Barbarism, TraitId::Psion)))
											&& GameMode != GM_COLISEUM
											&& (!(player.gameChanger & BIT(GC_22_BLACK_DEATH)))
											&& (!(player.gameChanger & BIT(GC_13_NO_REGENS))); break;
		case PERK_HOT_TEMPERED:				available = !(player.gameChanger & BIT(GC_12_MY_SECRET)); break;
		case PERK_SERENITY:					available = (GameModeType() == GAME_MODE_TYPE::NORMAL) && (!(player.gameChanger & BIT(GC_13_NO_REGENS)))
											&& GameMode != GM_COLISEUM; break;
		case PERK_AMPLIFY_DAMAGE:
		case PERK_PSYCHOKINESIS:			available = HasTrait(playerIndex, TraitId::Psion); break;
		case PERK_RAISE_THE_BAR:			available = HasTrait(playerIndex, TraitId::BendTheRules); break;
		case PERK_FOCUS:					available = (!(player.gameChanger & BIT(GC_13_NO_REGENS))) && (!HasTrait(playerIndex, TraitId::Mamluk))
											&& GameMode != GM_COLISEUM; break;
		case PERK_QUICK_RECOVERY:
		case PERK_FORTITUDE:
		case PERK_WILL_TO_LIVE:
		case PERK_UNWAVERING_ATTACK:
		case PERK_GIVE_THEM_NOTHING:
		case PERK_PATH_OF_BLOOD:
		case PERK_FIGHTING_STANCE:
		case PERK_TRANQUILITY:				available = (!(player.gameChanger& BIT(GC_13_NO_REGENS))) && GameMode != GM_COLISEUM; break;
		case PERK_GOLD_FEVER:
		case PERK_ADVANCED_USER:			available = !(player.gameChanger & BIT(GC_9_NIGHT_KIN)); break;
		case PERK_AUGMENTED_HEALING:		available = !(player.gameChanger & BIT(GC_9_NIGHT_KIN)) && GameMode != GM_COLISEUM; break;
		case PERK_CONSERVATOR:				available = !(player.gameChanger & BIT(GC_9_NIGHT_KIN)) 
											&& (player.ClassID != PC_2_MAGE || (player.fullClassId == PFC_ELEMENTALIST && HasTrait(playerIndex, TraitId::Mamluk))) ; break;
		case PERK_WEAPON_THRIFT:			available = (!(player.gameChanger & BIT(GC_9_NIGHT_KIN))) && (!HasTrait(playerIndex, TraitId::TwoTowers)); break;
		case PERK_END_THEM_RIGHTLY:			available = HasTrait(playerIndex, TraitId::OldHabit); break;
		case PERK_SEARING_AURA:
		case PERK_STATIC_AURA:
		case PERK_THEURGIC_AURA:
		case PERK_TOXIC_AURA:
		case PERK_AURA_RADIUS:				available = HasTrait(playerIndex, TraitId::Devastator); break;
		case PERK_TUCK_TAILS:
		case PERK_SHIELD_PARRY:
		case PERK_RECOUP:
		case PERK_COUNTERBALANCE:			available = (!HasTrait(playerIndex, TraitId::Fechtmeister))
											&& (!HasTrait(playerIndex, TraitId::Ranger)) && GameMode != GM_COLISEUM; break;
		case PERK_FROM_THE_SHADOWS:			available = MaxCountOfPlayersInGame == 1; break;
		case PERK_FIND_ITEM:				available = (
											( (is(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SPEEDRUN, GM_SURVIVAL))											
											|| (MaxCountOfPlayersInGame != 1) || (player.gameChanger & BIT(GC_1_AUTO_SAVE)) )
											&& (!(player.gameChanger & BIT(GC_14_CLEAN_HANDS))) 
											&& GameMode != GM_COLISEUM
											); break;
		case PERK_CROSSBOW_MASTERY:			available = HasTrait(playerIndex, TraitId::CrossbowTraining); break;
		case PERK_BRAVADO:					available = HasTrait(playerIndex, TraitId::Rudiarius); break;
		case PERK_CRITICAL_DEFENSE:			available = HasTrait(playerIndex, TraitId::ThickSkinned); break;
		case PERK_DUELIST:					available = HasTrait(playerIndex, TraitId::Fechtmeister) && HasTrait(playerIndex, TraitId::BendTheRules); break;
		case PERK_DEADLY_PURPOSE:			available = HasTrait(playerIndex, TraitId::Kamikaze) && HasTrait(playerIndex, TraitId::HeavyHanded); break;
		case PERK_EMERGE_UNSCATHED:			available = HasTrait(playerIndex, TraitId::Kamikaze) && HasTrait(playerIndex, TraitId::RatelHide); break;
		case PERK_VANDALISM:				available = HasTrait(playerIndex, TraitId::Kamikaze) && HasTrait(playerIndex, TraitId::Zealot); break;
		case PERK_STONEWALL:				available = !HasTrait(playerIndex, TraitId::Juggernaut) && GameMode != GM_COLISEUM
											&& (player.fullClassId != PFC_ELEMENTALIST || HasTrait(playerIndex, TraitId::Mamluk)); break;
		case PERK_STONING_MASTERY:			available = HasTrait(playerIndex, TraitId::BreakerOfStones) || HasTrait(playerIndex, TraitId::Petrifier); break;
		case PERK_SUMMONING:				available = (!(HasTrait(playerIndex, TraitId::Hydramancer, TraitId::Mamluk))) && GameMode != GM_COLISEUM; break;
		case PERK_CONJURATION:				
		case PERK_RETENTION:				available = HasTrait(playerIndex, TraitId::Hydramancer); break;
		case PERK_HEAVE_HO:					available = HasTrait(playerIndex, TraitId::BendTheRules); break;
		case PERK_MIGHT_N_MAGIC:
		case PERK_PATH_OF_MANA:
		case PERK_SANCTITY:					available = HasTrait(playerIndex, TraitId::Paladin); break;
		case PERK_BODKIN_ARROWS:
		case PERK_HEAVY_DRAW:				available = HasTrait(playerIndex, TraitId::Ranger); break;
		case PERK_STRONG_ARM:				available = !HasTrait(playerIndex, TraitId::Ranger) && !HasTrait(playerIndex, TraitId::Pistoleer); break;
		case PERK_UNDEAD_HUNTER:
		case PERK_CONCUSSIVE_BLOWS:
		case PERK_VAMPYRISM:				available = !HasTrait(playerIndex, TraitId::Ranger) 
											&& !HasTrait(playerIndex, TraitId::Pistoleer) 
											&& GameMode != GM_COLISEUM; break;
		case PERK_FIRE_MASTERY:
		case PERK_COLD_MASTERY:
		case PERK_LIGHTNING_MASTERY:
		case PERK_ARCANE_MASTERY:
		case PERK_CHAIN_LIGHTNING:
		case PERK_MIND_OVER_MATTER:
		case PERK_WILLPOWER:
		case PERK_MASTER_CASTER:			available = !HasTrait(playerIndex, TraitId::Mamluk); break;
		case PERK_GOLEM_RESISTANCE:
		case PERK_EXTRA_GOLEMS:				available = !HasTrait(playerIndex, TraitId::Mamluk) && GameMode != GM_COLISEUM && MaxCountOfPlayersInGame == 1; break;
		case PERK_FOOTWORK:
		case PERK_CRIPPLING_STRIKES:
		case PERK_TEMPEST:
		case PERK_SHARPENED_SWORD:
		case PERK_HEALING_MASTERY:
		case PERK_GUIDED_HAND:				available = HasTrait(playerIndex, TraitId::Mamluk); break;
		case PERK_SHREWD_BUYER:				available = HasTrait(playerIndex, TraitId::Negotiant) 
											&& (!(player.gameChanger & BIT(GC_14_CLEAN_HANDS)))
											&& (!(player.gameChanger & BIT(GC_23_TOUGH_LUCK)))
											&& (!(player.gameChanger & BIT(GC_5_LOTL))); break;
		case PERK_ABSOLUTION:				available = /* (GameModeType() == GAME_MODE_TYPE::NORMAL) */ 
											(!(HasTrait(playerIndex, TraitId::Barbarism, TraitId::Insensitive, TraitId::Paladin)))
											&& GameMode != GM_COLISEUM 
											&& (!(player.gameChanger & BIT(GC_13_NO_REGENS)))
											&& (!(player.gameChanger & BIT(GC_22_BLACK_DEATH))) 
											&& (!(player.gameChanger & BIT(GC_23_TOUGH_LUCK))); break;
		case PERK_TAKE_DOWN:				available = HasTrait(playerIndex, TraitId::Finesse); break;
		case PERK_HOLY_CAUSE:				available = !HasTrait(playerIndex, TraitId::Paladin); break;
		case PERK_INSULATION:				available = !HasTrait(playerIndex, TraitId::Mamluk); break;
		case PERK_HIT_DEFLECTION:			available = !HasTrait(playerIndex, TraitId::Sandman); break;
		case PERK_HALF_SWORDING:			available = !HasTrait(playerIndex, TraitId::Pistoleer) && GameMode != GM_COLISEUM; break;
		case PERK_PIERCING_BULLETS:
		case PERK_MARKSMAN:					available = HasTrait(playerIndex, TraitId::Pistoleer); break;
		case PERK_STOPPING_POWER:			available = HasTrait(playerIndex, TraitId::Pistoleer) 
											&& (!HasTrait(playerIndex, TraitId::ChromaticSkin)); break;
		case PERK_LETHALITY:
		case PERK_PURGE:
		case PERK_EXORCISM:					available = (player.ClassID == PC_1_ARCHER || HasTrait(playerIndex, TraitId::Ranger))
											&& GameMode != GM_COLISEUM; break;
		case PERK_SIXTH_SENSE:
		case PERK_LIGHT_STEP:
		case PERK_AWARENESS:
		case PERK_PENETRATION:
		case PERK_DEADLY_FLETCHERY:
		case PERK_DEMON_HUNTER:
		case PERK_BEAST_HUNTER:
		case PERK_ETHEREALITY:
		case PERK_CLEAVE:
		case PERK_LATERAL_PRECISION:
		case PERK_CULMINATION:
		case PERK_WINE_FROM_TEARS:
		case PERK_CRUSHING_BLOWS:
		case PERK_GOLEM_MASTERY:
		case PERK_SPELL_DEFLECTION:			available = GameMode != GM_COLISEUM; break;
		case PERK_FESTINA_LENTE:			available = !HasTrait(playerIndex, TraitId::Crupellarius); break;
		case PERK_CASTING_POWERS:			available = player.gameChanger & BIT(GC_9_NIGHT_KIN); break;

		// ************************** SYNERGIES ************************//
		case SYNERGY_WILL_TO_ENDURE:		available = (!(HasTrait(playerIndex, TraitId::Barbarian, TraitId::Mamluk))) && (!(player.gameChanger & BIT(GC_9_NIGHT_KIN))); break;
		case SYNERGY_CYCLE_OF_FURY:			available = (!(HasTrait(playerIndex, TraitId::GrimDeal, TraitId::Barbarism))) && GameMode != GM_COLISEUM
											&& (!(player.gameChanger & BIT(GC_12_MY_SECRET))) && (!(player.gameChanger & BIT(GC_13_NO_REGENS))); break;
		case SYNERGY_BLOOD_THIEVERY:		available = (!(HasTrait(playerIndex, TraitId::Adrenaline, TraitId::GrimDeal, TraitId::Barbarism, TraitId::Bloodless, TraitId::Ranger, TraitId::Pistoleer)))
											&& (!(player.gameChanger & BIT(GC_13_NO_REGENS)))
											&& GameMode != GM_COLISEUM
											&& (!(player.gameChanger & BIT(GC_9_NIGHT_KIN))); break;
		case SYNERGY_INEXORABILITY:			available = (!(HasTrait(playerIndex, TraitId::Ranger, TraitId::Pistoleer)))
											&& (!(player.gameChanger & BIT(GC_9_NIGHT_KIN))); break;
		case SYNERGY_IRON_BULWARK:			available = (!HasTrait(playerIndex, TraitId::Barbarian)) 
											&& (!(player.gameChanger & BIT(GC_9_NIGHT_KIN)))
											&& GameMode != GM_COLISEUM; break;
		case SYNERGY_ONSET:					available = (!(HasTrait(playerIndex, TraitId::Black_Witchery)))
											&& (!(HasTrait(playerIndex, TraitId::Paladin))) 
											&& (!(player.gameChanger & BIT(GC_9_NIGHT_KIN))) ; break;
		case SYNERGY_HARDENED_STEEL:		available = (!(HasTrait(playerIndex, TraitId::Sandman, TraitId::Barbarian))) && (!(player.gameChanger & BIT(GC_9_NIGHT_KIN))); break;
		case SYNERGY_STOICISM:				available = (!(HasTrait(playerIndex, TraitId::Juggernaut, TraitId::BigFrame))) && GameMode != GM_COLISEUM; break;
		case SYNERGY_LONGEVITY:
		case SYNERGY_INTREPIDITY:			available = !(player.gameChanger & BIT(GC_13_NO_REGENS)) 
											&& GameMode != GM_COLISEUM; break;
		case SYNERGY_AWAKENING:
		case SYNERGY_SAPIENCE:				available = (!(player.gameChanger & BIT(GC_13_NO_REGENS))) && (!HasTrait(playerIndex, TraitId::Mamluk)) 
											&& GameMode != GM_COLISEUM; break;
		case SYNERGY_RESUMPTION:			available = (!(HasTrait(playerIndex, TraitId::Adrenaline, TraitId::DarkPact, TraitId::GrimDeal, TraitId::Barbarism, TraitId::Psion, TraitId::Barbarian)))
											&& GameMode != GM_COLISEUM
											&& (!(player.gameChanger & BIT(GC_13_NO_REGENS))); break;
		case SYNERGY_HEAVY_HITTER:			available = HasTrait(playerIndex, TraitId::HeavyHanded); break;
		case SYNERGY_GLAD_DISCIPLINE:		available = HasTrait(playerIndex, TraitId::HeavyHanded) 
											&& HasTrait(playerIndex, TraitId::Bruiser) 
											&& (!(player.gameChanger & BIT(GC_13_NO_REGENS))); break;
		case SYNERGY_RETRIBUTION:			available = HasTrait(playerIndex, TraitId::Zealot) 
											&& (!(player.gameChanger & BIT(GC_23_TOUGH_LUCK)))
											&& (!(player.gameChanger & BIT(GC_14_CLEAN_HANDS))); break;
		case SYNERGY_SILVER_TONGUE:			available = (GameMode != GM_IRONMAN)
											&& (!(HasTrait(playerIndex, TraitId::Leper, TraitId::Barbarian, TraitId::Sisyphean)))
											&& (!(player.gameChanger & BIT(GC_23_TOUGH_LUCK))) 
											&& (!(player.gameChanger & BIT(GC_9_NIGHT_KIN))); break;
		case SYNERGY_AURA_MASTERY:
		case SYNERGY_BASTION_OF_ELEMENTS:	available = HasTrait(playerIndex, TraitId::Devastator); break;
		case PERK_FORTIFIED_SHIELD:
		case SYNERGY_ABJURATION:
		case SYNERGY_VALOR:					available = HasTrait(playerIndex, TraitId::Paladin); break;
		case SYNERGY_THAUMATURGY:			available = HasTrait(playerIndex, TraitId::Paladin) && (!(HasTrait(playerIndex, TraitId::BendTheRules))); break;
		case SYNERGY_CAREFUL_MENDING:		available = ((MaxCountOfPlayersInGame != 1) || (player.gameChanger & BIT(GC_1_AUTO_SAVE)) || (player.gameChanger & BIT(GC_26_ASCETICISM))); break;
		case SYNERGY_RESTORATION:			available = (!(player.gameChanger & BIT(GC_13_NO_REGENS))) 
											&& GameMode != GM_COLISEUM
											&& (!HasTrait(playerIndex, TraitId::Mamluk)); break;
		case SYNERGY_GOLDEN_SUNS:			available = (!HasTrait(playerIndex, TraitId::Barbarian))
											&& (!(player.gameChanger & BIT(GC_14_CLEAN_HANDS)))
											&& (!(player.gameChanger & BIT(GC_9_NIGHT_KIN)))
											&& GameMode != GM_COLISEUM
											&& (!(player.gameChanger & BIT(GC_23_TOUGH_LUCK))); break;
		case SYNERGY_HIGHER_EDUCATION:		available = GameMode != GM_IRONMAN && GameMode != GM_COLISEUM
											&& (!HasTrait(playerIndex, TraitId::Sisyphean))
											&& (!(HasTrait(playerIndex, TraitId::Barbarian, TraitId::Mamluk)))
											&& (!(player.gameChanger & BIT(GC_14_CLEAN_HANDS)))
											&& (!(player.gameChanger & BIT(GC_9_NIGHT_KIN)))
											&& (!(player.gameChanger & BIT(GC_23_TOUGH_LUCK))); break;
		case SYNERGY_FATAL_IMPACTS:			available = !HasTrait(playerIndex, TraitId::Tormentor); break;
		case SYNERGY_REJUVENATION:			available = (!HasTrait(playerIndex, TraitId::Barbarian, TraitId::Pistoleer))
											&& GameMode != GM_COLISEUM 
											&& (!(player.gameChanger & BIT(GC_9_NIGHT_KIN)))
											&& (!(player.gameChanger & BIT(GC_13_NO_REGENS))); break;
		case SYNERGY_MANA_FLOW:				available = (!(player.gameChanger & BIT(GC_13_NO_REGENS)))
											&& (!HasTrait(playerIndex, TraitId::Ranger, TraitId::Pistoleer)) && GameMode != GM_COLISEUM; break;
		case SYNERGY_NEGATION:
		case SYNERGY_BOLSTER:
		case SYNERGY_GRACE:
		case SYNERGY_BUTCHERY:				available = !HasTrait(playerIndex, TraitId::Sisyphean) && (!(player.gameChanger & BIT(GC_23_TOUGH_LUCK))); break;
		case SYNERGY_ACUITY:				available = HasTrait(playerIndex, TraitId::BendTheRules) && (!(player.gameChanger & BIT(GC_13_NO_REGENS))); break;
		case SYNERGY_MORTAL_STRIKE:			available = (!(player.gameChanger & BIT(GC_13_NO_REGENS))) && HasTrait(playerIndex, TraitId::Fechtmeister); break;
		case SYNERGY_SEEKER_ARROWS:			available = HasTrait(playerIndex, TraitId::Ranger); break;
		case SYNERGY_AUXILIARY_ARMOR:		available = (!HasTrait(playerIndex, TraitId::GrimDeal)) 
											&& (!HasTrait(playerIndex, TraitId::ThickSkinned)) 
											&& (!(player.gameChanger & BIT(GC_9_NIGHT_KIN))); break;
		case SYNERGY_DEATH_DENIER:			available = !(player.gameChanger & BIT(GC_9_NIGHT_KIN)); break;
		// Bargain turned off completely, below is an impossible condition
		case SYNERGY_INFERNAL_BARGAIN:		available = HasTrait(playerIndex, TraitId::Paladin) && HasTrait(playerIndex, TraitId::Hydramancer) && HasTrait(playerIndex, TraitId::Barbarism)
											&& (!(HasTrait(playerIndex, TraitId::Skilled, TraitId::GrimDeal, TraitId::Barbarian, TraitId::Mamluk)))
											&& (!(player.gameChanger & BIT(GC_13_NO_REGENS))); break;
		case SYNERGY_ENERGY_FIELD:			available = !HasTrait(playerIndex, TraitId::Mamluk); break;
		case SYNERGY_MESSENGER_OF_DEATH:
		case SYNERGY_DEEP_WOUNDS:
		case SYNERGY_PERSEVERANCE:			available = HasTrait(playerIndex, TraitId::Mamluk); break;
		case SYNERGY_TRADER:				available = (!(HasTrait(playerIndex, TraitId::Barbarian, TraitId::Leper)))
											&& (!(player.gameChanger & BIT(GC_5_LOTL)))
											&& (!(player.gameChanger & BIT(GC_9_NIGHT_KIN)))
											&& GameMode != GM_COLISEUM
											&& (!(player.gameChanger & BIT(GC_23_TOUGH_LUCK))); break;
		case SYNERGY_OSTENTATION:			available = (!HasTrait(playerIndex, TraitId::Sisyphean))
											&& GameMode != GM_COLISEUM
											&& (!HasTrait(playerIndex, TraitId::Barbarian))
											&& (!HasTrait(playerIndex, TraitId::BendTheRules))
											&& (!(player.gameChanger & BIT(GC_14_CLEAN_HANDS)))
											&& (!(player.gameChanger & BIT(GC_9_NIGHT_KIN)))
											&& (!(player.gameChanger & BIT(GC_23_TOUGH_LUCK))); break;
		case SYNERGY_SNOWBALL_EFFECT:		available = (!HasTrait(playerIndex, TraitId::Barbarian)) && GameMode != GM_COLISEUM; break;
		case SYNERGY_DAZZLING_LIGHT:		available = !HasTrait(playerIndex, TraitId::Paladin); break;
		case SYNERGY_PICK_UP_THE_PACE:		available = (is(GameMode, GM_NIGHTMARE, GM_IRONMAN, GM_SPEEDRUN/*, GM_SURVIVAL*/))
											&& (!(HasTrait(playerIndex, TraitId::Sisyphean, TraitId::Adrenaline, TraitId::DarkPact, TraitId::GrimDeal, TraitId::Barbarism, TraitId::Psion)))
											&& (!(player.gameChanger & BIT(GC_23_TOUGH_LUCK)))
											&& (!(player.gameChanger & BIT(GC_22_BLACK_DEATH)))
											&& (!(player.gameChanger & BIT(GC_13_NO_REGENS))); break;
		case SYNERGY_CORNERED_BEAST:		available = !(player.gameChanger & BIT(GC_13_NO_REGENS)) && GameMode != GM_COLISEUM; break;
		case SYNERGY_ELEM_BARRIER:			available = !HasTrait(playerIndex, TraitId::Barbarism); break;
		case SYNERGY_OVERWHELM:				available = (!(player.gameChanger & BIT(GC_9_NIGHT_KIN)))
											&& (!HasTrait(playerIndex, TraitId::Sisyphean))
											&& (is(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SPEEDRUN)); break;
		case SYNERGY_PAINKILLER:			available = (!(HasTrait(playerIndex, TraitId::Adrenaline, TraitId::DarkPact, TraitId::GrimDeal, TraitId::Barbarism, TraitId::Psion)))
											&& GameMode != GM_COLISEUM
											&& (!(player.gameChanger & BIT(GC_22_BLACK_DEATH)))
											&& (!(player.gameChanger & BIT(GC_13_NO_REGENS))); break;
		case SYNERGY_BETTER_CRITS:			available = HasTrait(playerIndex, TraitId::Pistoleer); break;
		case SYNERGY_GUNDOWN:				available = HasTrait(playerIndex, TraitId::Pistoleer) 
											&& (!HasTrait(playerIndex, TraitId::ChromaticSkin)); break;
		case SYNERGY_BOARDING_TECHNIQUE:
		case SYNERGY_VERSATILITY:
		case SYNERGY_REBOUND:
		case SYNERGY_LIGHT_FINGERED:		
		case SYNERGY_LAST_STAND:			available = GameMode != GM_COLISEUM; break;
		case SYNERGY_INVINCIBLE:			available = GameMode != GM_COLISEUM; break; 
		case SYNERGY_STRONGER_QUENCHING:	available = (!HasTrait(playerIndex, TraitId::Leper, TraitId::Barbarian))
											&& (player.ClassID != PC_2_MAGE || (player.fullClassId == PFC_ELEMENTALIST && HasTrait(playerIndex, TraitId::Mamluk)))
											&& GameMode != GM_IRONMAN && GameMode != GM_SPEEDRUN && GameMode != GM_NIGHTMARE
											&& (!(player.gameChanger & BIT(GC_9_NIGHT_KIN))); break;
		case SYNERGY_RIGIDITY:				available = HasTrait(playerIndex, TraitId::Leper, TraitId::Crupellarius); break;
		case SYNERGY_STRONG_HEART:			available = (!HasTrait(playerIndex, TraitId::Ranger, TraitId::Pistoleer, TraitId::FearTheReaper)); break;
		default: available = true;
		}
	}
	return available;
}

//----- (th2) ------------------------------------------------------------
int getPerkMaxLevel( int playerIndex, int perkIndex )
{
	int levels = Perks[perkIndex].levels;
	for( int level = 0; level < levels; ++level ){
		if( ! PerkAvailable(playerIndex, perkIndex, level) ){
			return level;
		}
	}
    return levels;
}

//----- (th2) ------------------------------------------------------------
int PerkPoints(int perk, int playerIndex)
{
	if( perk < PERKS_SYNERGY_MAX_750 || !(Players[CurrentPlayerIndex].gameChanger & BIT(GC_22_BLACK_DEATH)) ){
		return Players[playerIndex].perk[perk];
	}else{
		int levels = Perks[perk].levels;
		int level = 0;
		for( ; level < levels; ++level ) if( ! PerkAvailable(playerIndex, perk, level) || ! checkRequirementsForPerk(perk, level) ) break;
		return level;
	}
}

//----- (th2) ------------------------------------------------------------
int PerkValue(PERKS perk, int playerIndex, int effectIndex)
{
	Player& player = Players[playerIndex];
	int value = 0;
	if( (uint) playerIndex < 4 ){
		int points = PerkPoints(perk, playerIndex);
		if( points > 0 ){
			value = Perks[perk].level[ points - 1 ].value[effectIndex];
		}
	}
	return value;
}

//----- (th2) ------------------------------------------------------------
i64 getPassiveXpFormula()
{
	Player& player = Players[CurrentPlayerIndex];
	return (player.CharLevel * 3) * player.BaseMagic / ( 360 - player.CharLevel * 2 );
}

//----- (th2) ------------------------------------------------------------
void InitPerks()
{
	extern Perk SynergyPerks[PERKS_SYNERGY-PERKS_ONLY_MAX_400];
	extern Perk SymptomPerks[PERKS_COUNT-PERKS_SYNERGY_MAX_750];
	if( PERKS_COUNT > PERKS_MAX_800 ) TerminateWithError( "PERKS_COUNT: %i > PERKS_MAX_800: %i", PERKS_COUNT, PERKS_MAX_800 );
	if( PERKS_ONLY > PERKS_ONLY_MAX_400 ) TerminateWithError( "PERKS_ONLY: %i > PERKS_ONLY_MAX_400: %i", PERKS_ONLY, PERKS_ONLY_MAX_400 );
	if( PERKS_SYNERGY > PERKS_SYNERGY_MAX_750 ) TerminateWithError( "PERKS_SYNERGY: %i > PERKS_SYNERGY_MAX_750: %i", PERKS_SYNERGY, PERKS_SYNERGY_MAX_750 );
	memzero(PerkPanelLine);
	memcpy(Perks + PERKS_ONLY_MAX_400, SynergyPerks, sizeof(SynergyPerks));
	memcpy(Perks + PERKS_SYNERGY_MAX_750, SymptomPerks, sizeof(SymptomPerks));
	set<int> perkSet, synSet, symSet;
	for( int p = 0; p < PERKS_COUNT; ++p ){
	    auto& perk = Perks[p];
		if( p != perk.id && perk.id != -1 ){
			if( ! perk.id && ! perk.description[0] ){
				perk.id = -1;
			}else{
				TerminateWithError( "Perks[] error: perk %s has id %i but position %i", perk.name, perk.id, p );
			}
		}
		if( perk.id == -1 ) continue;
		if(      p < PERKS_ONLY_MAX_400    ){ perk.perkType = PERK_TYPE_COMMON;  perkSet.insert(p); }
		else if( p < PERKS_SYNERGY_MAX_750 ){ perk.perkType = PERK_TYPE_SYNERGY; synSet.insert(p);	}
		else                                { perk.perkType = PERK_TYPE_SYMPTOM; symSet.insert(p);	}

		for( perk.valueCount = 0; perk.description[perk.valueCount] && perk.valueCount < countof(perk.description); ++perk.valueCount );
		int levels = 0;
		for( int l = 0; l < countof(perk.level) && perk.level[l].charLevel; ++l, ++levels ){
			for( auto& req: perk.level[l].perkReq ){
				if( req.level == 0 ){
					break;
				}
				Perk& base = Perks[req.perk];
				if( base.secondaryCount < countof(base.secondary) && ! has( base.secondaryCount, base.secondary, p ) ){
					base.secondary[base.secondaryCount++] = p;
				}
			}
		}
		perk.levels = levels;
	}
	// check and init PerksPanelOrder list
	set<int> perkOrderSet;
	int perkFirst = 0, perkLast = 0, synFirst = 0, synLast = 0, symFirst = 0, symLast = 0;
	for( int p = perkFirst; p < PERKS_MAX_800; ++p ){
		if( PerksPanelOrder[p] >= PERKS_ONLY_MAX_400 ){ 
			perkLast = p - 1; synFirst = p; break;
		}else{
			if( ! perkOrderSet.insert(PerksPanelOrder[p]).second ){
				TerminateWithError( "PerksPanelOrder error: perk %i is duplicated", PerksPanelOrder[p] );
			}
		}
	}
	for( int p = synFirst;  p < PERKS_MAX_800; ++p ){
		if( PerksPanelOrder[p] >= PERKS_SYNERGY_MAX_750 ){
			synLast = p - 1; symFirst = p; break;
		}else{
			if( ! perkOrderSet.insert(PerksPanelOrder[p]).second ){
				TerminateWithError( "PerksPanelOrder error: synergy %i is duplicated", PerksPanelOrder[p] );
			}
		}
	}
	for( int p = symFirst;  p < PERKS_MAX_800; ++p ){
		if( PerksPanelOrder[p] == 0 ){
			symLast = p - 1; break;
		}else{
			if( ! perkOrderSet.insert(PerksPanelOrder[p]).second ){
				TerminateWithError( "PerksPanelOrder error: symptom %i is duplicated", PerksPanelOrder[p] );
			}
		}
	}
	const uint perkCount = perkLast - perkFirst + 1;
	const uint synCount = synLast - synFirst + 1;
	const uint symCount = symLast - symFirst + 1;
	uint p = PERKS_MAX_800 - 1;
	for(; p >= PERKS_SYNERGY_MAX_750 + symCount; --p ) PerksPanelOrder[p] = -1;
	for(; p >= PERKS_SYNERGY_MAX_750; --p ) PerksPanelOrder[p] = PerksPanelOrder[symLast--];
	for(; p >= PERKS_ONLY_MAX_400 + synCount; --p ) PerksPanelOrder[p] = -1;
	for(; p >= PERKS_ONLY_MAX_400; --p ) PerksPanelOrder[p] = PerksPanelOrder[synLast--];
	for(; p >= perkCount; --p ) PerksPanelOrder[p] = -1;
	for(p = 0; p < perkCount; ++p ) perkSet.erase(PerksPanelOrder[p]);
	for(auto i = perkSet.begin(); i != perkSet.end(); ++i ) PerksPanelOrder[p++] = *i;
	for(p = PERKS_ONLY_MAX_400; p < PERKS_ONLY_MAX_400 + synCount; ++p ) synSet.erase(PerksPanelOrder[p]);
	for(auto i = synSet.begin(); i != synSet.end(); ++i ) PerksPanelOrder[p++] = *i;
	for(p = PERKS_SYNERGY_MAX_750; p < symCount; ++p ) symSet.erase(PerksPanelOrder[p]);
	for(auto i = symSet.begin(); i != symSet.end(); ++i ) PerksPanelOrder[p++] = *i;
}

//----- (th2) ------------------------------------------------------------
void getAvailablePerksList()
{
	AvailablePerksCount = 0;
	for( int i = by(CurPerkPanel, 0, PERKS_ONLY_MAX_400, PERKS_SYNERGY_MAX_750), last = by(CurPerkPanel, PERKS_ONLY,PERKS_SYNERGY, PERKS_COUNT); i < last; ++i ){
		int p = PerksPanelOrder[i]; // sorting perks list in panel by PerksPanelOrder
		if( PerkAvailable(CurrentPlayerIndex, p) ){
	        AvailablePerks[ AvailablePerksCount++ ] = p;
        }
	}
	if( PerkPanelLine[CurPerkPanel] >= AvailablePerksCount ){
		PerkPanelLine[CurPerkPanel] = AvailablePerksCount;
	}
	

}

//----- (th2) ------------------------------------------------------------
bool checkLevelReq(int perk, int level)
{
	return Players[CurrentPlayerIndex].CharLevel >= Perks[perk].level[level].charLevel;
}

//----- (th2) ------------------------------------------------------------
bool checkStrReq(int perk, int level)
{
	return Players[CurrentPlayerIndex].BaseStrength >= Perks[perk].level[level].s;
}

//----- (th2) ------------------------------------------------------------
bool checkDexReq(int perk, int level)
{
	return Players[CurrentPlayerIndex].BaseDexterity >= Perks[perk].level[level].d;
}

//----- (th2) ------------------------------------------------------------
bool checkMagReq(int perk, int level)
{
	return Players[CurrentPlayerIndex].BaseMagic >= Perks[perk].level[level].m;
}

//----- (th2) ------------------------------------------------------------
bool checkVitReq(int perk, int level)
{
	return Players[CurrentPlayerIndex].BaseVitality >= Perks[perk].level[level].v;
}

//----- (th2) ------------------------------------------------------------
bool checkSpellReq(int perk, int level)
{
	PerkLevel& p = Perks[perk].level[level];
	Player& player = Players[CurrentPlayerIndex];
	for( auto& req: p.spellReq ){
		if( ! req.level ) return true;
		if( player.SpellLevels[req.spell] < req.level ) return false;
	}
	return true;
}
bool checkSpellReq(int perk, int level, int reqSpell)
{
	PerkLevel& p = Perks[perk].level[level];
	Player& player = Players[CurrentPlayerIndex];
	for( auto& req: p.spellReq ){
		if( ! req.level ) return true;
		if( req.spell == reqSpell ) return player.SpellLevels[req.spell] >= req.level;
	}
	return true;
}

//----- (th2) ------------------------------------------------------------
bool checkPerkReq(int perk, int level)
{
	PerkLevel& p = Perks[perk].level[level];
	Player& player = Players[CurrentPlayerIndex];
	for( auto& req: p.perkReq ){
		if( ! req.level ) return true;
		if( player.perk[req.perk] < req.level ) return false;
	}
	return true;
}
bool checkPerkReq(int perk, int level, int reqPerk)
{
	PerkLevel& p = Perks[perk].level[level];
	Player& player = Players[CurrentPlayerIndex];
	for( auto& req: p.perkReq ){
		if( ! req.level ) return true;
		if( req.perk == reqPerk ) return player.perk[req.perk] >= req.level;
	}
	return true;
}

//----- (th2) ------------------------------------------------------------
bool checkRequirementsForPerk(int p, int l)
{
	return checkLevelReq(p, l) && checkStrReq(p, l) && checkDexReq(p, l) && checkMagReq(p, l) && checkVitReq(p, l) && checkPerkReq(p, l) && checkSpellReq(p, l);
}

//----- (th2) ------------------------------------------------------------
int getPlayerSumOfPerks(PERK_TYPE perkType /*= PERK_TYPE_COMMON*/)
{
	int sum = 0;
	int perkCount = by(perkType, PERKS_ONLY, PERKS_SYNERGY, PERKS_COUNT);
	for( int p = by(perkType, 0, PERKS_ONLY_MAX_400, PERKS_SYNERGY_MAX_750); p < perkCount; ++p ){
		int playerPerkLevel = Players[CurrentPlayerIndex].perk[p];
		for( int l = 0; l < playerPerkLevel; ++l) if( PerkAvailable(CurrentPlayerIndex, p, l) ) ++sum;
	}
	return sum;
}

//----- (th2) ------------------------------------------------------------
void LimitPlayerPerks( int playerIndex )
{
    Player& player = Players[playerIndex];
    for( int perkIndex = 0; perkIndex < PERKS_COUNT; ++perkIndex ){
        char maxlvl = getPerkMaxLevel( playerIndex, perkIndex );
        if( player.perk[perkIndex] > maxlvl ){
            player.perk[perkIndex] = maxlvl;
            
            if( playerIndex == CurrentPlayerIndex ){
                NetSendCmdParam2( 1, CMD_105_SETPERK, perkIndex, Players[CurrentPlayerIndex].perk[perkIndex] );
            }
        }
	}
}

//----- (th2) ------------------------------------------------------------
int getPlayerPerksPointsLeft(PERK_TYPE perkType /*= PERK_TYPE_COMMON*/)
{
	if( GameMode == GM_CLASSIC ) return 0;
	int clvl = Players[CurrentPlayerIndex].CharLevel;
	switch( perkType ){
	case PERK_TYPE_SYNERGY: // Sinergy points
	{
		int synergyPointsGiven = 0;
		int le_divider = 5;
		if (HasTrait(CurrentPlayerIndex, TraitId::Prodigy)) {
			le_divider = 4;
		}
		else if(HasTrait(CurrentPlayerIndex, TraitId::Insensitive)) {
			le_divider = 7;
		}
		synergyPointsGiven = clvl / le_divider;
		return synergyPointsGiven - getPlayerSumOfPerks(perkType);
	}
	break;
	case PERK_TYPE_COMMON: // Perk points
	{
		int perkPointsGiven = 0;
		if( HasTrait(CurrentPlayerIndex, TraitId::Skilled, TraitId::LordOfTheRings) ){
			// 1: (1 + 2)/2 -1    = 3/2 -1 = 1 - 1 = 0
			// 2: (2 + 2)/2 -1  = 4/2 -1 = 2 - 1 = 1
			// 3: (3 + 2)/2 -1  = 5/2 -1 = 2 - 1 = 1
			// 4: (4 + 2)/2 -1  = 6/2 -1 = 3 - 1 = 2
			perkPointsGiven = ((clvl + 2) / 2) - 1;
		}else if( HasTrait(CurrentPlayerIndex, TraitId::GrimDeal) ){
			perkPointsGiven = (clvl - 1) * 2;
		}else{
			// 1: 1 - 1 = 0
			// 2: 2 - 1 = 1
			// 3: 3 - 1 = 2
			perkPointsGiven = clvl - 1 + PerkValue(SYNERGY_INFERNAL_BARGAIN, CurrentPlayerIndex, 0);
		}

		// extra perks
		if( HasTrait(CurrentPlayerIndex, TraitId::Barbarian) ){
			perkPointsGiven += (182 * clvl / 100) - 1;
		}
		else if( Players[CurrentPlayerIndex].gameChanger & BIT(GC_9_NIGHT_KIN) ){
			perkPointsGiven += (clvl * 2) - 2; // + 100 additional perks
		}
		else if (HasTrait(CurrentPlayerIndex, TraitId::FearTheReaper)) {
			perkPointsGiven += clvl - 1;
		}
		return perkPointsGiven - getPlayerSumOfPerks(perkType);
	}
	break;
	case PERK_TYPE_SYMPTOM:
	{
		return 0;
	}
	break;
	default: return 0;
	}
}

//----- (th2) ------------------------------------------------------------
const char* getSpellName( int spell )
{
	auto& fc = Players[CurrentPlayerIndex].fullClassId;
	if( spell >= PS_52_LESSER_SUMMON && spell <= PS_54_GREATER_SUMMON && fc >= PFC_DEMONOLOGIST && fc <= PFC_BEASTMASTER ){
		return SummonerSpellsNames[fc - PFC_DEMONOLOGIST][spell - PS_52_LESSER_SUMMON];
	}else if( spell == PS_3_LIGHTNING && (PerkValue(PERK_CHAIN_LIGHTNING, CurrentPlayerIndex ) || GameMode == GM_CLASSIC) ){
		return "Chain Lightning";
	}else if (spell == PS_13_HYDRA && fc == PFC_ELEMENTALIST) {
		return "Guardian of Ice";
	}else if (spell == PS_22_FURY && is(fc, PFC_ELEMENTALIST, PFC_MAGE, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER, PFC_WARLOCK)) {
		return "Concentration";
	}
	else if (spell == PS_22_FURY && is(fc, PFC_KENSEI) && MaxCountOfPlayersInGame == 1) {
		return "Battle Trance";
	}else if (spell == PS_12_FIREBLAST && GameMode == GM_CLASSIC){
		return "Fire Ball";
	}
	else if (spell == PS_13_HYDRA && GameMode == GM_CLASSIC) {
		return "Guardian";
	}
	else if (spell == PS_20_INCINERATE && GameMode == GM_CLASSIC) {
		return "Inferno";
	}
	else if (spell == PS_35_ARCANE_STAR && GameMode == GM_CLASSIC) {
		return "Blood Star";
	}else{
		return Spells[spell].Name;
	}
}

//----- (th2) ------------------------------------------------------------
int getSpellIcon( int spell )
{
	auto& fc = Players[CurrentPlayerIndex].fullClassId;
	if( spell >= PS_52_LESSER_SUMMON && spell <= PS_54_GREATER_SUMMON && fc >= PFC_DEMONOLOGIST && fc <= PFC_BEASTMASTER ){
		return SummonerSpellIconIndexes[fc - PFC_DEMONOLOGIST][spell - PS_52_LESSER_SUMMON];
	}else if( spell == PS_3_LIGHTNING && (PerkValue(PERK_CHAIN_LIGHTNING, CurrentPlayerIndex ) || GameMode == GM_CLASSIC) ){
		return 16;
	}else if (spell == PS_22_FURY && is(fc, PFC_ELEMENTALIST, PFC_MAGE, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER, PFC_WARLOCK)) {
		return 100;
	}
	else if (spell == PS_22_FURY && fc == PFC_KENSEI && MaxCountOfPlayersInGame == 1) {
		return 211;
	}else{
		return Spells[spell].Icon;
	}
}

//----- (th2) ------------------------------------------------------------
vector<string> split(const string& str, const string& delim)
{
	vector<string> tokens;
	size_t prev = 0, pos = 0;
	do{
		pos = str.find(delim, prev);
		if( pos == string::npos ){
			pos = str.length();
		}
		string token = str.substr(prev, pos - prev);
		if( !token.empty() ){
			tokens.push_back(token);
		}
		prev = pos + delim.length();
	}while( pos < str.length() && prev < str.length() );
	return tokens;
}

//----- (th2) ------------------------------------------------------------
string ReplaceString( string subject, const string& search, const string& replace )
{
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

//----- (th2) ------------------------------------------------------------
void DrawPerksPanel()
{
	int spacing = 20;
	char buff[128];
	
	if (celLoaded == false) {
		celLoaded = true;
		perksCel = (char*)LoadFile("Data\\perks.CEL");
		arrow1 = (char*)LoadFile("Data\\scrlarrw1.cel");
		arrow2 = (char*)LoadFile("Data\\scrlarrw2.cel");
		arrow3 = (char*)LoadFile("Data\\scrlarrw3.cel");
		arrow4 = (char*)LoadFile("Data\\scrlarrw4.cel");
	}
	int screenX = PerksPanelPos.Left;
	int screenY = PerksPanelPos.Down - 640;
	int textX = screenX + 64;
	int textY = screenY + 160;
	int offsetX = Screen_LeftBorder + screenX;
	int offsetY = Screen_TopBorder  + screenY;
	Surface_DrawCEL(offsetX, offsetY + 640, perksCel, 1, PerksPanelPos.Width);

	if (perkUpArrowPressed == true) {
		Surface_DrawCEL(offsetX + 316, offsetY + 275, arrow3, 1, 17);
	}
	else {
		Surface_DrawCEL(offsetX + 316, offsetY + 275, arrow1, 1, 17);
	}

	if (perkDownArrowPressed == true) {
		Surface_DrawCEL(offsetX + 316, offsetY + 300, arrow4, 1, 17);
	}
	else {
		Surface_DrawCEL(offsetX + 316, offsetY + 300, arrow2, 1, 17);
	}
 
	Player& player = Players[CurrentPlayerIndex];
	int pointsLeft = getPlayerPerksPointsLeft(CurPerkPanel);
	
	#if reset_limit_on_drawing
	int perk_limit = 100;
	if (HasTrait(CurrentPlayerIndex, TraitId::Barbarian)) {
		perk_limit = 190;
	}
	else if (player.gameChanger & BIT (GC_9_NIGHT_KIN)) {
		perk_limit = 150;
	}
	if (pointsLeft > perk_limit || pointsLeft < 0) {
		for (int i = 0; i < PERKS_MAX_800; ++i) {
			player.perk[i] = 0;
		}
	}
	#endif
    
	for( int i = size(player.traits) - 1, x = 0; i >= 0; --i ){
		if( player.traits[i] != TraitId::NoTrait ){
			DrawSpellIcon(offsetX + 559 - x, offsetY + 581, Data_SpelIconCEL, GetTraitSpellIcon(player.traits[i]), 56, SO_5_TRAIT);
			x += 56 + 8;
		}
	}

	//getAvailablePerksList();

	DrawText(textX, textY + 55, 126, by(CurPerkPanel, "   Available perks:", "   Available synergies:", "   Available symptoms:"), C_3_Gold);

	for( int i = PerkPanelLine[CurPerkPanel] - 3; i <= PerkPanelLine[CurPerkPanel] + 3; ++i ){
		if( i < 0 || i >= AvailablePerksCount ){
            continue;
        }
		int index = i;
		int perkIndex = AvailablePerks[index];
		Perk& perk = Perks[perkIndex];
		int level = PerkPoints(perkIndex, CurrentPlayerIndex);
		int maxlvl = getPerkMaxLevel( CurrentPlayerIndex, perkIndex );
		char* selectionPrefix = "";
		char* selectionSuffix = "";
		int x = textX - 35;
		int y = textY + 160 + (i - PerkPanelLine[CurPerkPanel]) * spacing;
		if( index == PerkPanelLine[CurPerkPanel] ){
			DrawBox( {x - 5, y - 15, 279, 20}, 252 ); // selection background
			//selectionPrefix = "\202->\202";
		    //selectionSuffix = "\202<-\202";
		}
		sprintf( buff, "%s%s (LVL: %i/%i)%s", selectionPrefix, perk.name, level, maxlvl, selectionSuffix );
		bool allreqsmet = true;
		if (level < maxlvl) {
			allreqsmet = checkRequirementsForPerk( AvailablePerks[index], level );
		}
		DrawTextColored(x, y, 126, buff, (level == maxlvl ? C_3_Gold : ( allreqsmet ? (level < 1 ? C_0_White : C_1_Blue) : (level < 1 ? C_2_Red : C_1_Blue))) );		
		if( ShowSecondaryPerks ){
			bool hasSynergy = false;
			for( int s = 0; s < perk.secondaryCount; ++s ){
				if( PerkAvailable( CurrentPlayerIndex, perk.secondary[s]) ){
					hasSynergy = true;
					break;
				}
			}
			if( false && hasSynergy ) DrawTextColored(x + 260, y, x + 270, "->", C_0_White);
		}
	}
	if (AvailablePerksCount > 0) {
		const int p = AvailablePerks[PerkPanelLine[CurPerkPanel]];
		int level = PerkPoints(p, CurrentPlayerIndex);
		const int maxlvl = getPerkMaxLevel( CurrentPlayerIndex, p );
		bool maxReached = level >= maxlvl;
		if( maxReached ) --level;
		Perk& perk = Perks[p];
		const PerkLevel& l = perk.level[level];
  
		int offset = 275;
		char* infoTitle = "values: (\201current\201), \204next level\204";
		int x = textX + 110 - GetTextWidth(infoTitle) / 2;
		DrawTextColored(x, textY + offset, x, infoTitle, C_0_White); offset += 15;
		int infoLines = 0;
		for( int v = 0; v < perk.valueCount; ++v ) for( char* next = perk.description[v] - 2; next; next = strstr(next + 2, "::"), ++infoLines );
		offset += std::max(0, 5 - infoLines / 2) * 15;
		for( int v = 0; v < perk.valueCount; ++v ){
			int value = l.value[v];
			int prevValue = 0;
			if( maxReached ){
				prevValue = value;
			}else if( level > 0 ){
				prevValue = perk.level[level-1].value[v];
			}
			string desc = perk.description[v];
			desc = ReplaceString( desc, "%i", "(\201%i\201) \204%i\204" );

			sprintf(buff, (char*)desc.c_str(), prevValue, value);

			stringstream desc2;
			desc2 << buff;
			vector<string> lines = split(desc2.str(), "::");
			for( uint i = 0; i < lines.size(); ++i ){
				DrawTextColored(textX + 100 - GetTextWidth((char*)lines[i].c_str()) / 2, textY + offset, textX + 110 - GetTextWidth((char*)lines[i].c_str()) / 2, (char*)lines[i].c_str(), C_0_White);
				offset += 15;
			}
		}
		offset = 55;
		DrawText(textX + 365, textY + offset, textX + 400, "requirements:", C_3_Gold);
		offset += 15;
		{
			sprintf(buff, "Level: %i", l.charLevel);
			DrawText(textX + 330, textY + offset, textX + 330, buff, (checkLevelReq(p, level) ? C_0_White : C_2_Red));
			offset += 15;
		}
		if( l.s || l.d || l.m || l.v ){
			DrawText(textX + 220, textY + offset, textX + 400, "stats:", C_3_Gold);
			offset += 15;
			if( l.s ){
			    sprintf(buff, "str: %i", l.s);
				DrawText(textX + 250, textY + offset, textX + 400, buff, (checkStrReq(p, level) ? C_0_White : C_2_Red));
				offset += 15;
			}
			if( l.d ){
				sprintf(buff, "dex: %i", l.d);
				DrawText(textX + 250, textY + offset, textX + 400, buff, (checkDexReq(p, level) ? C_0_White : C_2_Red));
				offset += 15;
			}
			if( l.m ){
				sprintf(buff, "mag: %i", l.m);
				DrawText(textX + 250, textY + offset, textX + 400, buff, (checkMagReq(p, level) ? C_0_White : C_2_Red));
				offset += 15;
			}
			if( l.v ){
				sprintf(buff, "vit: %i", l.v);
				DrawText(textX + 250, textY + offset, textX + 400, buff, (checkVitReq(p, level) ? C_0_White : C_2_Red));
				offset += 15;
			}
			offset += 15;
		}
		if( l.spellReq[0].level ){
			DrawText(textX + 220, textY + offset, textX + 400, "spells:", C_3_Gold);
			offset += 15;
			for( auto& req: l.spellReq ){
				if( ! req.level ) break;
				sprintf(buff, "%s - level %i", getSpellName( req.spell ), req.level);
				DrawText(textX + 300, textY + offset, textX + 400, buff, checkSpellReq(p, level, req.spell) ? C_0_White : C_2_Red);
				offset += 15;
			}
			offset += 15;
		}
		if( l.perkReq[0].level ){
			DrawText(textX + 220, textY + offset, textX + 400, "perks:", C_3_Gold);
			offset += 15;
			for( auto& req: l.perkReq ){
				if( ! req.level ) break;
				sprintf(buff, "%s - level %i", Perks[ req.perk ].name, req.level);
				DrawText(textX + 300, textY + offset, textX + 400, buff, checkPerkReq(p, level, req.perk) ? C_0_White : C_2_Red);
				offset += 15;
			}
			offset += 15;
		}
		if( ShowSecondaryPerks && perk.secondaryCount ){
			offset += l.spellReq[0].level || l.perkReq[0].level || l.s || l.d || l.m || l.v ? 15 : 30;
			int titleY = textY + offset;
			offset += 15;
			offset += 15;
			int drawedPerks = 0;
			for( int i = 0; i < perk.secondaryCount; ++i ){
				int synergy = perk.secondary[i];
				if( PerkAvailable( CurrentPlayerIndex, synergy ) ){
					++drawedPerks;
					sprintf(buff, "%s (LVL: %i)", Perks[synergy].name, player.perk[synergy]);
					DrawText(textX + 300, textY + offset, textX + 400, buff, C_4_Orange);
					offset += 15;
				}
			}
			if( drawedPerks ) DrawText(textX + 320, titleY, textX + 400, "secondary:", C_4_Orange);
			offset += 15;
		}
		spendPerkPointsButtonVisisble = checkRequirementsForPerk(p, level) && getPlayerPerksPointsLeft(CurPerkPanel) > 0 && ! maxReached;
		if( spendPerkPointsButtonVisisble ){
			DrawPlusCEL(screenX + 350, screenY + 610, 2);
		}
	}
	
	sprintf(buff, "%i", pointsLeft);
	DrawText(textX + 505, textY + 445, textX + 560, buff, (pointsLeft > 0 ? C_0_White : C_2_Red));
	DrawText(textX + 365, textY + 445, textX + 490, "Points to spend:", C_0_White);
}

//----- (th2) ------------------------------------------------------------
void PerksPanel_MouseMove()
{
	int offsetX = PerksPanelPos.Left;
	int offsetY = PerksPanelPos.Down - 640;
	int buttonDown = offsetY + 581;
	
	const Player& player = Players[CurrentPlayerIndex];
    MaybeNeedDrawInfoPanel = 0;
	InfoPanel_ClearBody();
	
	for( int i = size(player.traits) - 1, buttonLeft = offsetX + 559; i >= 0; --i ){
		TraitId trait = player.traits[i];
		if( trait != TraitId::NoTrait ){
			if( CursorX >= buttonLeft && CursorX <= buttonLeft + 56 && CursorY >= buttonDown - 56 && CursorY <= buttonDown ){
				Tooltip_Clear();
				Tooltip_SetOffsetTop();
				sprintf(InfoPanelBuffer, "Trait: %s", GetTraitName(trait) );
				Tooltip_AddLine(InfoPanelBuffer, C_3_Gold);
				Tooltip_AddLine(GetTraitBenefitDescription(trait), C_1_Blue);
				Tooltip_AddLine(GetTraitPenaltyDescription(trait), C_2_Red);
			}
			buttonLeft -= 56 + 8;
        }
    }
}
