#include "stdafx.h"

int TraitIndexToSelect = 0;
TraitId SelectedTraitId = TraitId::NoTrait;
int TraitTitleList[ 2 ] = { ID_CAPTION, 0 };
int TraitOkCancelList[ 3 ] = { ID_OK_BUTTON, ID_CANCEL_BUTTON, 0 };

int TraitButtonList[] = {
    ID_TRAIT_1,
    ID_TRAIT_2,
    ID_TRAIT_3,
    ID_TRAIT_4,
    ID_TRAIT_5,
    ID_TRAIT_6,
    0 };

constexpr std::array nextTraitOrder = {
    ID_TRAIT_2,
    ID_TRAIT_3,
    ID_TRAIT_4,
    ID_TRAIT_5,
    ID_TRAIT_6,
    ID_TRAIT_1,
};
constexpr std::array prevTraitOrder = {
    ID_TRAIT_6,
    ID_TRAIT_1,
    ID_TRAIT_2,
    ID_TRAIT_3,
    ID_TRAIT_4,
    ID_TRAIT_5,
};

enum TRAIT_GROUP_MASK : uint64_t
{
    TGM_NONE                = 0,
    TGM_PERK_POINTS_DOWN    = BIT(1),
    TGM_PERK_POINTS_UP      = BIT(2),
	TGM_DESTROY				= BIT(3),
	TGM_BOUNCE				= BIT(4),
	TGM_REZ_UP				= BIT(5),
	TGM_REZ_DOWN			= BIT(6),
	TGM_CRIT_UP				= BIT(7),
	TGM_CRIT_DOWN			= BIT(8),
	TGM_DMG_UP				= BIT(9),
	TGM_DMG_DOWN			= BIT(10),
	TGM_MELEE_AUTOHIT_UP	= BIT(11),
	TGM_MELEE_AUTOHIT_DOWN	= BIT(12),
	TGM_MAX_REZ_UP			= BIT(13),
	TGM_MAX_REZ_DOWN		= BIT(14),
	TGM_STATS_MORE			= BIT(15),
	TGM_STATS_LESS			= BIT(16),
	TGM_ATK_SPD_DOWN		= BIT(17),
	TGM_ATK_SPD_UP			= BIT(18),
	TGM_MORE_XP				= BIT(19),
	TGM_LESS_XP				= BIT(20),
	TGM_MORE_MAX_STATS		= BIT(21),
	TGM_MANUAL_MAX_STATS	= BIT(22),
    TGM_KB_DOWN             = BIT(23),
    TGM_KB_UP               = BIT(24),
    TGM_SYNS_UP             = BIT(25),
    TGM_SYNS_DOWN           = BIT(26),
    TGM_MF_MORE             = BIT(27),
    TGM_BARBARIAN           = BIT(28),
    TGM_SPELLS_ON           = BIT(29),
    TGM_SPELLS_OFF          = BIT(30),
    TGM_NO_PISTOL           = BIT(31),
    TGM_PISTOL              = BIT(32),
    TGM_FTR                 = BIT(33),
    TGM_WEIRD               = BIT(34),
    TGM_NASTY               = BIT(35),
    TGM_LEPER               = BIT(36),
    TGM_BRUISER             = BIT(37),
    TGM_LITHE               = BIT(38),
    TGM_AC_DOWN             = BIT(39),
    TGM_AC_UP               = BIT(40),
    TGM_BLOOD_OATH          = BIT(41),
    TGM_PSION               = BIT(42),
    TGM_TR_HUNTER           = BIT(43),
    TGM_COINBOUND           = BIT(44),
    TGM_SMALL_FRAME         = BIT(45),
    TGM_DOOMWHORL           = BIT(46),
    TGM_RABID               = BIT(47),
    TGM_LOTR                = BIT(48),
    TGM_OLDFASH             = BIT(49),
    TGM_CRUPELLARIUS        = BIT(50),
    TGM_ABNEG               = BIT(51),
    TGM_FAST_MET            = BIT(52),
    TGM_GIANT               = BIT(53),
    TGM_PAL                 = BIT(54)

};
bit_enum( TRAIT_GROUP_MASK );

std::vector TraitsExcludedCombinations = {
    TGM_PERK_POINTS_DOWN | TGM_PERK_POINTS_UP,
	TGM_DESTROY			 | TGM_BOUNCE,
	TGM_REZ_UP			 | TGM_REZ_DOWN,
	TGM_CRIT_UP			 | TGM_CRIT_DOWN,
	TGM_DMG_UP			 | TGM_DMG_DOWN,
	TGM_MELEE_AUTOHIT_UP | TGM_MELEE_AUTOHIT_DOWN,
	TGM_MAX_REZ_UP		 | TGM_MAX_REZ_DOWN,
	TGM_STATS_MORE		 | TGM_STATS_LESS,
	TGM_ATK_SPD_DOWN	 | TGM_ATK_SPD_UP,
	TGM_MORE_XP			 | TGM_LESS_XP,
	TGM_MANUAL_MAX_STATS | TGM_MORE_MAX_STATS,
    TGM_KB_DOWN          | TGM_KB_UP,
    TGM_SYNS_UP          | TGM_SYNS_DOWN,
    TGM_MF_MORE          | TGM_BARBARIAN,
    TGM_SPELLS_ON        | TGM_SPELLS_OFF,
    TGM_NO_PISTOL        | TGM_PISTOL,
    TGM_DMG_UP           | TGM_PISTOL,
    TGM_FTR              | TGM_PERK_POINTS_UP,
    TGM_FTR              | TGM_LESS_XP,
    TGM_FTR              | TGM_PERK_POINTS_DOWN,
    TGM_FTR              | TGM_ATK_SPD_DOWN,
    TGM_FTR              | TGM_WEIRD,
    TGM_FTR              | TGM_STATS_MORE,
    TGM_NASTY            | TGM_MORE_MAX_STATS,
    TGM_NASTY            | TGM_BARBARIAN,
    TGM_NASTY            | TGM_LEPER,
    TGM_FTR              | TGM_PISTOL,
    TGM_BRUISER          | TGM_LITHE,
    TGM_MANUAL_MAX_STATS | TGM_LITHE,
    TGM_LEPER            | TGM_LITHE,
    TGM_AC_UP            | TGM_AC_DOWN,
    TGM_BLOOD_OATH       | TGM_PSION,
    TGM_PERK_POINTS_DOWN | TGM_COINBOUND,
    TGM_TR_HUNTER        | TGM_COINBOUND,
    TGM_BARBARIAN        | TGM_COINBOUND,
    TGM_SMALL_FRAME      | TGM_DOOMWHORL,
    TGM_CRIT_UP          | TGM_RABID,
    TGM_LOTR             | TGM_PERK_POINTS_DOWN,
    TGM_LOTR             | TGM_PERK_POINTS_UP,
    TGM_LOTR             | TGM_FTR,
    TGM_OLDFASH          | TGM_CRUPELLARIUS,
    TGM_MELEE_AUTOHIT_UP | TGM_CRUPELLARIUS,
    TGM_CRIT_UP          | TGM_CRUPELLARIUS,
    TGM_ABNEG            | TGM_REZ_DOWN,
    TGM_ABNEG            | TGM_FAST_MET,
    TGM_GIANT            | TGM_PISTOL,
    TGM_GIANT            | TGM_FTR,
    TGM_GIANT            | TGM_MELEE_AUTOHIT_UP,
    TGM_GIANT            | TGM_PAL,
    TGM_GIANT            | TGM_SMALL_FRAME,
    TGM_GIANT            | TGM_MANUAL_MAX_STATS,
    TGM_GIANT            | TGM_PERK_POINTS_UP,
    TGM_GIANT            | TGM_RABID,
    TGM_GIANT            | TGM_BRUISER,
    TGM_GIANT            | TGM_MORE_MAX_STATS,
    TGM_GIANT            | TGM_STATS_MORE,
    TGM_GIANT            | TGM_LITHE

};

std::vector<TraitId> TraitsAvailableList;

//----- (th2) ------------------------------------------------------------
const char* GetTraitName( TraitId id )
{
    switch( id )
    {
        default:
        case TraitId::NoTrait:			return "No Trait";
        case TraitId::Gifted:			return "Gifted";
        case TraitId::Kamikaze:			return "Kamikaze";
        case TraitId::HeavyHanded:		return "Heavy Handed";
        case TraitId::Skilled:			return "Skilled";
        case TraitId::Barbarism:		return "Barbarism";
        case TraitId::Finesse:			return "Finesse";
        case TraitId::Survivor:			return "Survivor";
        case TraitId::Weird:			return "Weird";
        case TraitId::OldFashioned:		return "Old Fashioned";
        case TraitId::Domesticated:		return "Domesticated";
        case TraitId::Rabid:			return "Rabid";
        case TraitId::Bruiser:			return "Bruiser";
        case TraitId::Negotiant:		return "Negotiant";
        case TraitId::Leper:			return "Leper";
        case TraitId::Rudiarius:		return "Rudiarius";
        case TraitId::Scrounger:		return "Scrounger";
        case TraitId::Adventurer:		return "Adventurer";
        case TraitId::ThickSkinned:		return "Thick Skinned";
        case TraitId::RatelHide:		return "Ratel Hide";
        case TraitId::Forgetful:		return "Forgetful";
        case TraitId::WildSorcery:		return "Wild Sorcery";
        case TraitId::Cautious:			return "Cautious";
		case TraitId::OldHabit:			return "Old Habit";
		case TraitId::GoldDigger:		return "Gold Digger";
		case TraitId::ArrowDancing:		return "Arrow Dancing";
		case TraitId::PointBlank:		return "Point Blank";
		case TraitId::FastMetabolism:	return "Fast Metabolism";
		case TraitId::Bouncer:			return "Bouncer";
		case TraitId::Sandman:			return "Sandman";
		case TraitId::ARoseWithThorns:	return "A Rose with Thorns";
		case TraitId::Pyromaniac:		return "Pyromaniac";
		case TraitId::AvatarOfCold:		return "Avatar of Cold";
		case TraitId::BestDefense:		return "The Best Defense...";
		case TraitId::GrimDeal:			return "Grim Deal";
		case TraitId::HolyAura:			return "Holy Aura";
		case TraitId::Strafer:			return "Strafer";
		case TraitId::PiercingShot:		return "Piercing Shot";
		case TraitId::Destroyer:		return "Destroyer";
		case TraitId::ChromaticSkin:	return "Chromatic Skin";
		case TraitId::Tormentor:		return "Tormentor";
		case TraitId::Armadillo:		return "Armadillo";
		case TraitId::BloodyMess:		return "Bloody Mess";
		case TraitId::Bestiarius:		return "Bestiarius";
		case TraitId::Petrifier:		return "Petrifier";
		case TraitId::DarkPact:			return "Dark Pact";
		case TraitId::Toxic_at_Heart:	return "Toxic at Heart";
		case TraitId::Hunger:			return "Hunger";
		case TraitId::Fechtmeister:		return "Fechtmeister";
		case TraitId::WreckingBlock:	return "Wrecking Block";
		case TraitId::TwoTowers:		return "Two Towers";
		case TraitId::PuncturingStab:	return "Puncturing Stab";
		case TraitId::BloodAndSand:		return "Blood and Sand";
		case TraitId::BloodForBlood:	return "Blood for Blood";
		case TraitId::BigFrame:			return "Big Frame";
		case TraitId::CrossbowTraining:	return "Crossbow Training";
		case TraitId::BlisteredSkin:	return "Blistered Skin";
		case TraitId::Cleric:			return "Cleric";
		case TraitId::Bloodless:		return "Bloodless";
		case TraitId::Engineer:			return "Engineer";
		case TraitId::Necropathy:		return "Necropathy";
		case TraitId::TreasureHunter:	return "Treasure Hunter";
		case TraitId::BreakerOfStones:	return "Breaker of Stones";
		case TraitId::Cannibalism:		return "Cannibalism";
		case TraitId::Black_Witchery:	return "Black Witchery";
		case TraitId::Axepertise:		return "Axepertise";
		case TraitId::MonkeyGrip:		return "Monkey Grip";
		case TraitId::Zealot:			return "Zealot"; 
		case TraitId::Adrenaline:		return "Adrenaline";
		case TraitId::Sisyphean:		return "Sisyphean Task";
		case TraitId::BendTheRules:		return "Bend the Rules";
        case TraitId::Barbarian:		return "Barbarian";
        case TraitId::Psion:		    return "Psion";
        case TraitId::Devastator:	    return "Devastator";
        case TraitId::Thrill_Seeker:    return "Thrill Seeker";
        case TraitId::Psychotic:        return "Psychotic";
        case TraitId::SmallFrame:       return "Small Frame";
        case TraitId::BlueBlood:        return "Blue Blood";
        case TraitId::Juggernaut:       return "Juggernaut";
        case TraitId::Prodigy:          return "Prodigy";
        case TraitId::Insensitive:      return "Insensitive";
        case TraitId::Hydramancer:      return "Hydramancer";
        case TraitId::IronFisted:       return "Iron Fisted";
        case TraitId::Paladin:          return "Paladin";
        case TraitId::Unshakable:       return "Unshakable";
        case TraitId::ResoluteGuard:    return "Resolute Guard";
        case TraitId::Ranger:           return "Ranger";
        case TraitId::Mamluk:           return "Mamluk";
        case TraitId::Vigorous:         return "Vigorous";
        case TraitId::Pistoleer:        return "Pistoleer";
        case TraitId::FearTheReaper:    return "Fear the Reaper";
        case TraitId::NastyDisposition: return "Nasty Disposition";
        case TraitId::CrowdSeeker:      return "Crowd Seeker";
        case TraitId::Stoneform:        return "Stoneform";
        case TraitId::LitheBuild:       return "Lithe Build";
        case TraitId::BloodOath:        return "Blood Oath";
        case TraitId::ManaFlux:         return "Mana Flux";
        case TraitId::Coinbound:        return "Coinbound";
        case TraitId::Doomwhorl:        return "Doomwhorl";
        case TraitId::LordOfTheRings:   return "Lord of the Rings";
        case TraitId::Crupellarius:     return "Crupellarius";
        case TraitId::Fatality:         return "Fatality";
        case TraitId::Abnegation:       return "Abnegation";
        case TraitId::Giant:            return "Giant";
        case TraitId::Feral:            return "Feral";
    }
}

//----- (th2) ------------------------------------------------------------
const char* GetTraitBenefitDescription( TraitId id )
{
    switch( id )
    {
    default:
    case TraitId::NoTrait:			return  "";
    case TraitId::Gifted:			return  "+5 to base stats and +25 to maximum stats"; //unlocks: 
    case TraitId::Kamikaze:			return  "+CLVL to accuracy"; //done
    case TraitId::HeavyHanded:		return  "+(80%CLVL) to physical melee damage"; //done
    case TraitId::Skilled:			return  "+CLVL+(CLVL^2/150) to all current stats"; //done
    case TraitId::Barbarism:		return  "magical damage resistance cap increased to 90%"; //done
    case TraitId::Finesse:			return  "critical strike chance increased by (CLVL/27+10)%"; //done
    case TraitId::Survivor:			return  "+CLVL*2 to life regeneration"; // done
    case TraitId::Weird:			return  "+(CLVLx2) to life"; // done
    case TraitId::OldFashioned:		return  "+(CLVL^2/45+CLVL) armor, +(CLVL^2/25) damage, +40 maximum base dexterity"; //done
    case TraitId::Domesticated:		return  "+30% to experience gains"; //done
    case TraitId::Rabid:			return  "base damage is doubled for melee attacks"; //done
    case TraitId::Bruiser:			return  "+150 maximum base strength"; //done - needs check!
    case TraitId::Negotiant:		return  "discount of extra 10% prices in shops, opportunities to acquire better goods in them"; //done
    case TraitId::Leper:			return  "+75 maximum base dexterity, +75 maximum base magic"; //done - needs check!
    case TraitId::Rudiarius:		return  "start game with +750 gold"; //done
    case TraitId::Scrounger:		return  "+50% magic find"; //done
    case TraitId::Adventurer:		return  "1 more stat point per level-up"; //done
    case TraitId::ThickSkinned:		return  "+(CLVL/9) melee damage resistance, +(CLVL/9) arrow damage resistance"; //done
    case TraitId::RatelHide:		return  "-(CLVL/7) to DFE"; // done
    case TraitId::Forgetful:		return  "+CLVL^2+100 to experience gains"; //done
    case TraitId::WildSorcery:		return  "all spell damage is increased by 30%"; //done
    case TraitId::Cautious:			return  "+30% damage to targets that are further than point blank range"; //done
	case TraitId::OldHabit:			return  "deal +200% damage to non-unique monster in melee, if it has 25% life or less"; //done
	case TraitId::GoldDigger:		return  "+(2xCLVL) gold dropped"; //done
	case TraitId::ArrowDancing:		return  "+(CLVL/2) to arrow damage resistance"; //done
	case TraitId::PointBlank:		return  "+65% total damage at point blank range"; //done
	case TraitId::FastMetabolism:	return  "+(CLVL/10) to life & mana regeneration"; //done
	case TraitId::Bouncer:			return  "total melee damage increased by 50%"; //done
	case TraitId::Sandman:			return  "melee damage resist: +5, caps: +10% to minimum and maximum"; //done
	case TraitId::ARoseWithThorns:	return  "thorns damage (from every item) increased by (CLVL/2)"; //done
	case TraitId::Pyromaniac:		return  "total damage from fire flasks increased by 25%"; //done
	case TraitId::AvatarOfCold:		return  "+25% to damage of cold spells and to cold resistance"; //done
	case TraitId::BestDefense:		return  "+(25+CLVL/15)% to damage of summoned minions"; //done
	case TraitId::GrimDeal:			return  "get one more perk on each level-up"; //done
	case TraitId::HolyAura:			return  "nearby visible undead take 1+CLVL*MAG/40 damage per second"; //done
	case TraitId::Strafer:			return  "Multishot effect upgraded to 5 arrows, with manual control"; //done
	case TraitId::PiercingShot:		return  "(CLVL/3)% chance for arrows to pass through enemies they hit"; //done
	case TraitId::Destroyer:		return  "total damage increased by 15%"; //done
	case TraitId::ChromaticSkin:	return  "+(50%CLVL) to all magic resistances"; //done
	case TraitId::Tormentor:		return  "elemental damage from weapons is increased by 20%"; //done
	case TraitId::Armadillo:		return  "Reflect layer amount increased by CLVL/13+2"; //done
	case TraitId::BloodyMess:		return  "your attacks cause extra bleeding: melee crit damage increased by (CLVL/2)^2+20"; //done
	case TraitId::Bestiarius:		return  "sharp weapon damage against beasts is increased by 30%"; //done
	case TraitId::Petrifier:		return  "you can damage Stonecursed monsters (halved damage)"; //done
	case TraitId::DarkPact:			return  "damage of acid spells increased by CLVL%"; //done
	case TraitId::Toxic_at_Heart:	return  "acid burns monsters 80% faster"; //done
	case TraitId::Hunger:			return  "your melee hits always leech additional 1% of total life and mana"; //done
	case TraitId::Fechtmeister:		return  "1 frame faster melee attacks, +(CLVL/2) to armor, melee autohit is reduced by 4% more"; //done
	case TraitId::WreckingBlock:	return  "blocking melee hits reflects damage, increased by (CLVL/2)%"; //done
	case TraitId::TwoTowers:		return  "wield dual shields, +(CLVL/6) to current and max. block chance"; //done
	case TraitId::PuncturingStab:	return  "(CLVL/5)% chance to bypass melee damage resistance and DFE of the target"; //done
	case TraitId::BloodAndSand:		return  "DFE can reduce incoming melee damage by 75% (standard limit of absorption: 50%)"; //done
	case TraitId::BloodForBlood:	return  "total melee damage is increased by 30% when life drops below 40%"; //done
	case TraitId::BigFrame:			return  "get 3 life per level, and 3 life per Vitality point"; //done
	case TraitId::CrossbowTraining:	return  "150% of Dexterity added to base damage when using crossbows"; //done
	case TraitId::BlisteredSkin:	return  "base life regeneration depends on maximum life: +1 regeneration for 25 life"; //done
	case TraitId::Cleric:			return  "damage of Holy spell types increased by 50%"; //done
	case TraitId::Bloodless:		return  "monsters cannot leech life from you"; //done
	case TraitId::Engineer:			return  "trap damage is passively increased by (15+CLVL/2)%"; //done
	case TraitId::Necropathy:		return  "monsters receive 20% more damage from your minions when in your light radius"; //done
	case TraitId::TreasureHunter:	return  "unique monsters drop one more item on death"; //done
	case TraitId::BreakerOfStones:	return  "summons will attack stonecursed monsters in melee (50% damage) "; //done
	case TraitId::Cannibalism:		return  "summoned minions leech 20% life from melee hits"; //done
	case TraitId::Black_Witchery:	return  "significantly improved magical powers for ranged combat"; //done
	case TraitId::Axepertise:		return  "use two-handed axes as melee weapon, stun threshold: +2"; //done
	case TraitId::MonkeyGrip:		return  "wield two-handed melee weapons with only one hand"; //done
	case TraitId::Zealot:			return  "your attack speed is increased by 1 frame"; //done
	case TraitId::Adrenaline:		return  "regeneration of life is increased for active fighting"; //done
	case TraitId::Sisyphean:		return  "current life and mana increased by (2xCLVL)"; //done
	case TraitId::BendTheRules:		return  "all base attributes can grow up to 900"; //done
    case TraitId::Barbarian:		return  "+180%CLVL additional perk points, +CLVL/2 stun threshold"; //done
    case TraitId::Psion:		    return  "nearby visible monsters take 3+CLVL*MAG/150 damage per second"; //done
    case TraitId::Devastator:	    return  "access to elemental auras that damage nearby visible monsters"; //done
    case TraitId::Thrill_Seeker:    return  "+50% to experience gains when current life is below 35%"; //done
    case TraitId::Psychotic:        return  "Fury makes you stronger and more resilient"; //done
    case TraitId::SmallFrame:       return  "autohit improved (-5% melee, -3% ranged), base AC formula: DEX/3"; //done
    case TraitId::BlueBlood:        return  "all starting attributes raised by 10 points, +10% XP gains"; //done
    case TraitId::Juggernaut:       return  "knockback resistance set to 100%"; //done
    case TraitId::Prodigy:          return  "get Synergy points every 4th level"; //done
    case TraitId::Insensitive:      return  "armor class raised by (CLVL/2)"; //done
    case TraitId::Hydramancer:      return  "get Hydra as skill and have them use various elemental attacks";
    case TraitId::IronFisted:       return  "damage increased by CLVL (melee & ranged)";
    case TraitId::Paladin:          return  "you get magical and spellcasting powers";
    case TraitId::Unshakable:       return  "stun threshold increased by (40%CLVL)";
    case TraitId::ResoluteGuard:    return  "all magic resistances increased by (60%CLVL)";
    case TraitId::Ranger:           return  "embraces the unique skills of an Archer";
    case TraitId::Mamluk:           return  "embrace the unique skills of a desert warrior";
    case TraitId::Vigorous:         return  "maximum health increased by 35%";
    case TraitId::Pistoleer:        return  "use pistol for a weapon";
    case TraitId::FearTheReaper:    return  "get one more perk on each level-up";
    case TraitId::NastyDisposition: return  "+(10+CLVL/3) to accuracy, +(25+CLVL/2)% to weapon damage";
    case TraitId::CrowdSeeker:      return  "additional 25% chance to avoid item durability loss";
    case TraitId::Stoneform:        return  "+CLVL to armor class";
    case TraitId::LitheBuild:       return  "+100 maximum base dexterity, monsters have extra 8% chance to miss";
    case TraitId::BloodOath:        return  "character states will not change life & mana regeneration rate";
    case TraitId::ManaFlux:         return  "+(1+2xCLVL/21) to spell levels";
    case TraitId::Coinbound:        return  "Goldfind increased by 100%CLVL";
    case TraitId::Doomwhorl:        return  "Innate damage resistance to ranged attacks absorbs 20% more damage";
    case TraitId::LordOfTheRings:   return  "you can equip up to 8 rings";
    case TraitId::Crupellarius:     return  "more melee & arrow resistance caps, +150%CLVL to AC, +100 max VIT";
    case TraitId::Fatality:         return  "you get additional 10% chance to score a 300% damage hit";
    case TraitId::Abnegation:       return  "extra -2 DFE, +(CLVL/3) to MDR / ADR / all resistance";
    case TraitId::Giant:            return  "you get +1 life per base VIT, max VIT increased by 100";
    case TraitId::Feral:            return  "you have no summoner-specific offensive spell cooldowns";
    }
}

//----- (th2) ------------------------------------------------------------
const char* GetTraitPenaltyDescription( TraitId id )
{
    switch( id )
    {
    default:
    case TraitId::NoTrait:			return  "";
    case TraitId::Gifted:			return  "-(CLVLx2) to mana"; // done
    case TraitId::Kamikaze:			return  "-(CLVL/3) to armor class"; //done
    case TraitId::HeavyHanded:		return  "-(1+CLVL/26) to all spell levels"; //done
    case TraitId::Skilled:			return  "perk rate reduced by CLVL/2"; // done
    case TraitId::Barbarism:		return  "-(CLVL/5) mana regeneration"; // done
    case TraitId::Finesse:			return  "-50% base damage"; //done
    case TraitId::Survivor:			return  "-CLVL to all resistances"; //done
    case TraitId::Weird:			return  "spell manacost is doubled"; //done
    case TraitId::OldFashioned:		return  "can't wear body armor"; // done
    case TraitId::Domesticated:		return  "1 less stat per level-up"; //done
    case TraitId::Rabid:			return  "you cannot drink potions that restore life or mana";// done
    case TraitId::Bruiser:			return  "-150 maximum base dexterity"; //done
    case TraitId::Negotiant:		return  "-50 maximum base vitality"; //done
    case TraitId::Leper:			return  "townspeople are afraid of you and will not talk (except Gillian)"; //done
    case TraitId::Rudiarius:		return  "start game with -5 to base vitality"; //done
    case TraitId::Scrounger:		return  "-(1+CLVL/15) to stun threshold"; //done
    case TraitId::Adventurer:		return  "-20% life, -20% mana"; // done
    case TraitId::ThickSkinned:		return  "-66%CLVL to accuracy"; //done
    case TraitId::RatelHide:		return  "-35% to knockback resistance"; //done
    case TraitId::Forgetful:		return  "constantly lose experience points: (CLVL^2/10)"; //done
    case TraitId::WildSorcery:		return  "you cannot drink potions that restore life or mana";// done
    case TraitId::Cautious:			return  "-50% damage at point blank range"; //done
	case TraitId::OldHabit:			return  "+(CLVL/8) damage from enemies"; //done
	case TraitId::GoldDigger:		return  "armor class reduced by (120% CLVL)"; //done
	case TraitId::ArrowDancing:		return  "-(CLVL/2) to melee damage resistance"; //done
	case TraitId::PointBlank:		return  "-25% damage to targets that are further than point blank"; //done
	case TraitId::FastMetabolism:	return  "-CLVL to Acid Resistance"; //done
	case TraitId::Bouncer:			return  "attack speed with melee weapons decreased by 2 frames"; //done
	case TraitId::Sandman:			return  "minimum chance to be hit in melee raised by 1+CLVL/15%"; //done
	case TraitId::ARoseWithThorns:	return  "-(CLVL/3) to current and maximum block chance"; //done
	case TraitId::Pyromaniac:		return  "dexterity doesn't increase armor class"; //done
	case TraitId::AvatarOfCold:		return  "-25% to damage of fire spells and to fire resistance"; //done
	case TraitId::BestDefense:		return  "-(25+CLVL/15)% to total hit points of summoned minions"; //done
	case TraitId::GrimDeal:			return  "AC, %life, %mana, HP regen, DFE, stun threshold: weakened by 40%CLVL"; //done
	case TraitId::HolyAura:			return  "-20% to experience points earned"; //done
	case TraitId::Strafer:			return  "attacking takes 1 frame longer"; //done
	case TraitId::PiercingShot:		return  "-(2*CLVL) to accuracy"; //done
	case TraitId::Destroyer:		return  "weapons break twice as fast"; //done
	case TraitId::ChromaticSkin:	return  "monsters cannot be stunned by your attacks"; //done
	case TraitId::Tormentor:		return  "ability to score critical hits is totally lost"; //done
	case TraitId::Armadillo:		return  "melee damage is reduced by CLVL-1"; //done
	case TraitId::BloodyMess:		return  "melee autohit chance raised by 2%"; //done
	case TraitId::Bestiarius:		return  "blunt weapon damage against undead is lowered by 25%"; //done
	case TraitId::Petrifier:		return  "Stone Curse has a 60 second cooldown"; //done
	case TraitId::DarkPact:			return  "mana regeneration reduced by CLVL/2"; //done
	case TraitId::Toxic_at_Heart:	return  "ability to score critical hits is lost, -40% acid damage"; //done
	case TraitId::Hunger:			return  "monsters have thicker armor (DFE) the deeper you venture"; //done
	case TraitId::Fechtmeister:		return  "cannot use shields"; //done
	case TraitId::WreckingBlock:	return  "shields lose durability twice as fast"; //done
	case TraitId::TwoTowers:		return  "cannot use any melee weapons, melee autohit increased by 10%"; //done
	case TraitId::PuncturingStab:	return  "-CLVL to accuracy"; //done
	case TraitId::BloodAndSand:		return  "-(CLVL/5) to life regeneration"; //done
	case TraitId::BloodForBlood:	return  "no gains to life from level-ups"; //done
	case TraitId::BigFrame:			return  "autohit increased: by 3% in melee combat, by 2% in ranged combat"; //done
	case TraitId::CrossbowTraining:	return  "+5% to autohit in ranged combat"; //done
	case TraitId::BlisteredSkin:	return  "magical damage resistance cap set to 75%"; //done
	case TraitId::Cleric:			return  "damage of other spell types decreased by 20%"; //done
	case TraitId::Bloodless:		return  "you cannot leech life from them either"; //done
	case TraitId::Engineer:			return  "no gains to life or mana from level-ups"; //done
	case TraitId::Necropathy:		return  "-50% basic mana regeneration, -CLVL to mana"; //done
	case TraitId::TreasureHunter:	return  "Goldfind and Magicfind are reduced by (CLVL/2+75)%"; //done
	case TraitId::BreakerOfStones:	return  "Stone Curse has a 120 second cooldown"; //done
	case TraitId::Cannibalism:		return  "minimum chance to be hit in melee raised by 9% for minions"; //done
	case TraitId::Black_Witchery:	return  "there are no throwing knives anymore"; //done
	case TraitId::Axepertise:		return  "you no longer use blunts and sharps"; //done
	case TraitId::MonkeyGrip:		return  "-(10+CLVL) accuracy and AC, -20% maximum chance to hit"; //done
	case TraitId::Zealot:			return  "-(5+CLVL) accuracy, -15% to maximum chance of hitting target"; //done
	case TraitId::Adrenaline:		return  "life regeneration drops when standing still"; //done
	case TraitId::Sisyphean:		return  "total XP gains are reduced by (75+CLVL/10)%. this includes perks and item effects"; //done
	case TraitId::BendTheRules:		return  "total base attribute sum is limited at 1400"; //done
    case TraitId::Barbarian:		return  "uses only white items, can't read books (except Fury, Heal, Portal)"; //done
    case TraitId::Psion:		    return  "reduced base mana, no natural mana regeneration"; //done
    case TraitId::Devastator:	    return  "you have to spend perk points on these auras"; //done
    case TraitId::Thrill_Seeker:	return  "-25% to experience gains if it is above 65%"; //done
    case TraitId::Psychotic:	    return  "you lose control under Fury and go berserking until it ends"; //done
    case TraitId::SmallFrame:	    return  "melee and arrow damage resist lowered: current by 10, maximum by 10%"; //done
    case TraitId::BlueBlood:	    return  "-1 to stats to distribute on level-up"; //done
    case TraitId::Juggernaut:	    return  "cannot learn Teleport spell"; //done
    case TraitId::Prodigy:	        return  "base spell levels reduced by CLVL/18"; //done
    case TraitId::Insensitive:	    return  "get Synergy points only every 7th level, mana regeneration reduced by (CLVL/9)"; //done
    case TraitId::Hydramancer:      return  "most offensive spells have cooldowns";
    case TraitId::IronFisted:       return  "critical strike chance reduced by (CLVL/10+1)%";
    case TraitId::Paladin:          return  "you cannot use mallets and crossbows";
    case TraitId::Unshakable:       return  "armor class reduced by CLVL";
    case TraitId::ResoluteGuard:    return  "stun threshold decreased by (30%CLVL)";
    case TraitId::Ranger:           return  "does not use any weapons but bows";
    case TraitId::Mamluk:           return  "can't use offensive spells effectively";
    case TraitId::Vigorous:         return  "attack speed reduced by 1 frame";
    case TraitId::Pistoleer:        return  "no other weapons can be used";
    case TraitId::FearTheReaper:    return  "initial life is 50. Get no life for base Vitality and level-ups";
    case TraitId::NastyDisposition: return  "vendor prices are tripled";
    case TraitId::CrowdSeeker:      return  "monster chain activation radius increased: +2";
    case TraitId::Stoneform:        return  "vitality doesn't give natural resistances";
    case TraitId::LitheBuild:       return  "inventory size is halved";
    case TraitId::BloodOath:        return  "DFE increased by (1+CLVL/16)";
    case TraitId::ManaFlux:         return  "maximum mana decreased by 20%";
    case TraitId::Coinbound:        return  "All stats decreased by CLVL/5";
    case TraitId::Doomwhorl:        return  "+8% to autohit in ranged combat";
    case TraitId::LordOfTheRings:   return  "perk rate is reduced by CLVL/2";
    case TraitId::Crupellarius:     return  "-50% base damage, -100 max STR, -10 accuracy";
    case TraitId::Fatality:         return  "monsters can do the same";
    case TraitId::Abnegation:       return  "-40%CLVL to life & mana regeneration";
    case TraitId::Giant:            return  "potions are only half effective";
    case TraitId::Feral:            return  "cannot learn Teleport spell, half natural mana regen";
    }
}

//----- (th2) ------------------------------------------------------------
const char* GetTraitDescription( TraitId id )
{
    if( id == TraitId::NoTrait ){
        return "None Selected";
    }else{
        sprintf(InfoPanelBuffer, "Benefit: %s\n\nPenalty: %s", GetTraitBenefitDescription(id), GetTraitPenaltyDescription(id) );
        return InfoPanelBuffer;
    }
}

//----- (th2) ------------------------------------------------------------
int GetTraitSpellIcon( TraitId id )
{
    switch( id )
    {
        default:
        case TraitId::NoTrait:			return 27;
        case TraitId::Gifted:			return 138;
        case TraitId::Kamikaze:			return 186;
        case TraitId::HeavyHanded:		return 147;
        case TraitId::Skilled:			return 165;
        case TraitId::Barbarism:		return 143;
        case TraitId::Finesse:			return 167;
        case TraitId::Survivor:			return 179;
        case TraitId::Weird:			return 180;
        case TraitId::OldFashioned:		return 76;
        case TraitId::Domesticated:		return 82;
        case TraitId::Rabid:			return 127;
        case TraitId::Bruiser:			return 124;
        case TraitId::Negotiant:		return 130;
        case TraitId::Leper:			return 181;
        case TraitId::Rudiarius:		return 162;
        case TraitId::Scrounger:		return 146;
        case TraitId::Adventurer:		return 77;
        case TraitId::ThickSkinned:		return 170;
        case TraitId::RatelHide:		return 229;
        case TraitId::Forgetful:		return 223;
        case TraitId::WildSorcery:		return 154;
        case TraitId::Cautious:			return 184;
		case TraitId::OldHabit:			return 213;
		case TraitId::GoldDigger:		return 162;
		case TraitId::ArrowDancing:		return 141;
		case TraitId::PointBlank:		return 216;
		case TraitId::FastMetabolism:	return 155;
		case TraitId::Bouncer:			return 144;
		case TraitId::Sandman:			return 85;
		case TraitId::ARoseWithThorns:	return 78;
		case TraitId::Pyromaniac:		return 136;
		case TraitId::AvatarOfCold:		return 105;
		case TraitId::BestDefense:		return 48;
		case TraitId::GrimDeal:			return 70;
		case TraitId::HolyAura:			return 200;
		case TraitId::Strafer:			return 166;
		case TraitId::PiercingShot:		return 188;
		case TraitId::Destroyer:		return 171;
		case TraitId::ChromaticSkin:	return 214;
		case TraitId::Tormentor:		return 97;
		case TraitId::Armadillo:		return 91;
		case TraitId::BloodyMess:		return 230;
		case TraitId::Bestiarius:		return 156;
		case TraitId::Petrifier:		return 119;
		case TraitId::DarkPact:			return 69;
		case TraitId::Toxic_at_Heart:	return 183;
		case TraitId::Hunger:			return 134;
		case TraitId::Fechtmeister:		return 148;
		case TraitId::WreckingBlock:	return 101;
		case TraitId::TwoTowers:		return 80;
		case TraitId::PuncturingStab:	return 157;
		case TraitId::BloodAndSand:		return 68;
		case TraitId::BloodForBlood:	return 205;
		case TraitId::BigFrame:			return 122;
		case TraitId::CrossbowTraining:	return 185;
		case TraitId::BlisteredSkin:	return 222;
		case TraitId::Cleric:			return 201;
		case TraitId::Bloodless:		return 76;
		case TraitId::Engineer:			return 195;
		case TraitId::Necropathy:		return 64;
		case TraitId::TreasureHunter:	return 44;
		case TraitId::BreakerOfStones:	return 119;
		case TraitId::Cannibalism:		return 134;
		case TraitId::Black_Witchery:	return 32;
		case TraitId::Axepertise:		return 152;
		case TraitId::MonkeyGrip:		return 204;
		case TraitId::Zealot:			return 212;
		case TraitId::Adrenaline:		return 89;
		case TraitId::Sisyphean:		return 117;
		case TraitId::BendTheRules:		return 114;
        case TraitId::Barbarian:		return 189;
        case TraitId::Psion:		    return 208;
        case TraitId::Devastator:	    return 197;
        case TraitId::Thrill_Seeker:	return 128;
        case TraitId::Psychotic:	    return 129;
        case TraitId::SmallFrame:	    return 83;
        case TraitId::BlueBlood:	    return 231;
        case TraitId::Juggernaut:	    return 75;
        case TraitId::Prodigy:	        return 253;
        case TraitId::Insensitive:      return 247;
        case TraitId::Hydramancer:      return 178;
        case TraitId::IronFisted:       return 246;
        case TraitId::Paladin:          return 175;
        case TraitId::Unshakable:       return 227;
        case TraitId::ResoluteGuard:    return 98;
        case TraitId::Ranger:           return 250;
        case TraitId::Mamluk:           return 245;
        case TraitId::Vigorous:         return 240;
        case TraitId::Pistoleer:        return 119;
        case TraitId::FearTheReaper:    return  66;
        case TraitId::NastyDisposition: return 235;
        case TraitId::CrowdSeeker:      return 243;
        case TraitId::Stoneform:        return  73;
        case TraitId::LitheBuild:       return 215;
        case TraitId::BloodOath:        return 254;
        case TraitId::ManaFlux:         return 234;
        case TraitId::Coinbound:        return 251;
        case TraitId::Doomwhorl:        return 232;
        case TraitId::LordOfTheRings:   return 108;
        case TraitId::Crupellarius:     return 209;
        case TraitId::Fatality:         return 244;
        case TraitId::Abnegation:       return 254;
        case TraitId::Giant:            return 206;
        case TraitId::Feral:            return 176;
    }
}

//----- (th2) ------------------------------------------------------------
uint GetRequiredClassMaskForTrait( TraitId id )
{
    switch( id )
    {
        default:
        case TraitId::NoTrait:			return IPCM_ALL_CLASSES;
        case TraitId::Gifted:			return IPCM_ALL_CLASSES;
        case TraitId::Kamikaze:			return IPCM_ANY_WARRIOR
										     | IPCM_ANY_MONK
										     | IPCM_ANY_SAVAGE;
        case TraitId::HeavyHanded:		return IPCM_ANY_WARRIOR
											 | IPCM_ANY_MONK
											 | IPCM_ANY_SAVAGE;
        case TraitId::Skilled:			return IPCM_ALL_CLASSES;
        case TraitId::Barbarism:		return IPCM_ALL_CLASSES;
        case TraitId::Finesse:			return IPCM_ANY_WARRIOR
											 | IPCM_ARCHER
										     | IPCM_SHARPSHOOTER
										     | IPCM_ANY_MONK
										     | IPCM_ROGUE
										     | IPCM_ASSASSIN
										     | IPCM_BERSERKER
											 | IPCM_EXECUTIONER
											 | IPCM_MURMILLO
											 | IPCM_THRAEX
											 | IPCM_DIMACHAERUS
											 | IPCM_SECUTOR | IPCM_DRUID;
        case TraitId::Survivor:			return IPCM_ALL_CLASSES;
        case TraitId::Weird:			return IPCM_ALL_CLASSES;
        case TraitId::OldFashioned:		return IPCM_ANY_GLADIATOR;
        case TraitId::Domesticated:		return IPCM_ALL_CLASSES;
        case TraitId::Rabid:			return IPCM_SAVAGE
											 | IPCM_EXECUTIONER | IPCM_DRUID;
        case TraitId::Bruiser:			return IPCM_SHINOBI
											 | IPCM_THRAEX
											 | IPCM_DIMACHAERUS
											 | IPCM_SECUTOR;
        case TraitId::Negotiant:		return IPCM_ANY_WARRIOR
											 | IPCM_ANY_ARCHER
											 | IPCM_ANY_MONK
											 | IPCM_ANY_ROGUE;
        case TraitId::Leper:			return IPCM_EXECUTIONER;
        case TraitId::Rudiarius:		return IPCM_ANY_GLADIATOR;
        case TraitId::Scrounger:		return IPCM_ALL_CLASSES;
        case TraitId::Adventurer:		return IPCM_ALL_CLASSES;
        case TraitId::ThickSkinned:		return IPCM_ANY_WARRIOR
											 | IPCM_ARCHER
											 | IPCM_SCOUT
											 | IPCM_ANY_MONK
											 | IPCM_ANY_SAVAGE;
        case TraitId::RatelHide:		return IPCM_ANY_WARRIOR
											 | IPCM_ANY_MONK
											 | IPCM_ANY_ROGUE
											 | IPCM_ANY_SAVAGE;
        case TraitId::Forgetful:		return IPCM_ALL_CLASSES;
        case TraitId::WildSorcery:		return IPCM_MAGE
											 | IPCM_ELEMENTALIST
											 | IPCM_WARLOCK | IPCM_DRUID;
        case TraitId::Cautious:			return IPCM_SHARPSHOOTER;
		case TraitId::OldHabit:			return IPCM_EXECUTIONER;
		case TraitId::GoldDigger:		return IPCM_ROGUE;
		case TraitId::ArrowDancing:		return IPCM_SCOUT
											 | IPCM_MONK
											 | IPCM_KENSEI
											 | IPCM_ROGUE
											 | IPCM_BERSERKER
											 | IPCM_DIMACHAERUS;
		case TraitId::PointBlank:		return IPCM_ARCHER;
		case TraitId::FastMetabolism:	return IPCM_ALL_CLASSES;
		case TraitId::Bouncer:			return IPCM_SAVAGE;
		case TraitId::Sandman:			return IPCM_ANY_WARRIOR
											 | IPCM_ANY_MONK
											 | IPCM_IRON_MAIDEN
											 | IPCM_ANY_SAVAGE;
		case TraitId::ARoseWithThorns:	return IPCM_IRON_MAIDEN;
		case TraitId::Pyromaniac:		return IPCM_BOMBARDIER;
		case TraitId::AvatarOfCold:		return IPCM_ELEMENTALIST;
		case TraitId::BestDefense:		return IPCM_ANY_SUMMONER;
		case TraitId::GrimDeal:			return IPCM_ANY_ARCHER
											 | IPCM_WARRIOR
											 | IPCM_INQUISITOR
											 | IPCM_GUARDIAN
											 | IPCM_MAGE
											 | IPCM_ELEMENTALIST
											 | IPCM_ANY_SUMMONER
											 | IPCM_ANY_MONK
											 | IPCM_ANY_ROGUE
											 | IPCM_ANY_GLADIATOR
											 | IPCM_SAVAGE
											 | IPCM_BERSERKER | IPCM_DRUID;
		case TraitId::HolyAura:			return IPCM_TEMPLAR;
		case TraitId::Strafer:			return IPCM_SCOUT;
		case TraitId::PiercingShot:		return IPCM_SHARPSHOOTER;
		case TraitId::Destroyer:		return IPCM_SAVAGE;
		case TraitId::ChromaticSkin:	return IPCM_GUARDIAN 
											 | IPCM_TEMPLAR 
											 | IPCM_SHUGOKI 
											 | IPCM_ASSASSIN | IPCM_DRUID;
		case TraitId::Tormentor:		return IPCM_INQUISITOR;
		case TraitId::Armadillo:		return IPCM_BERSERKER;
		case TraitId::BloodyMess:		return IPCM_DIMACHAERUS;
		case TraitId::Bestiarius:		return IPCM_SECUTOR;
		case TraitId::Petrifier:		return IPCM_SHINOBI;
		case TraitId::DarkPact: 		return IPCM_WARLOCK;
		case TraitId::Toxic_at_Heart: 	return IPCM_BOMBARDIER;
		case TraitId::Hunger: 			return IPCM_SHUGOKI;
		case TraitId::Fechtmeister:		return IPCM_WARRIOR;
		case TraitId::WreckingBlock:	return IPCM_IRON_MAIDEN;
		case TraitId::TwoTowers:		return IPCM_IRON_MAIDEN;
		case TraitId::PuncturingStab:	return IPCM_ASSASSIN;
		case TraitId::BloodAndSand:		return IPCM_MURMILLO;
		case TraitId::BloodForBlood:	return IPCM_THRAEX;
		case TraitId::BigFrame:			return IPCM_ROGUE;
		case TraitId::CrossbowTraining:	return IPCM_GUARDIAN;
		case TraitId::BlisteredSkin:	return IPCM_KENSEI;
		case TraitId::Cleric:			return IPCM_MAGE;
		case TraitId::Bloodless:		return IPCM_MONK;
		case TraitId::Engineer:			return IPCM_TRAPPER;
		case TraitId::Necropathy:		return IPCM_NECROMANCER;
		case TraitId::TreasureHunter:	return IPCM_ALL_CLASSES;
		case TraitId::BreakerOfStones:	return IPCM_DEMONOLOGIST;
		case TraitId::Cannibalism:		return IPCM_BEASTMASTER;
		case TraitId::Black_Witchery:	return IPCM_ASSASSIN;
		case TraitId::Axepertise:		return IPCM_ROGUE;
		case TraitId::MonkeyGrip:		return IPCM_SAVAGE;
		case TraitId::Zealot:			return IPCM_INQUISITOR
											 | IPCM_GUARDIAN
											 | IPCM_TEMPLAR
											 | IPCM_ARCHER
											 | IPCM_SCOUT 
											 | IPCM_ANY_MONK
											 | IPCM_ROGUE
											 | IPCM_ASSASSIN
											 | IPCM_BERSERKER
											 | IPCM_EXECUTIONER
											 | IPCM_THRAEX
											 | IPCM_SECUTOR; 
		case TraitId::Adrenaline:		return IPCM_BERSERKER;
		case TraitId::Sisyphean:		return IPCM_ANY_WARRIOR & ~IPCM_TEMPLAR
											 | IPCM_ANY_ARCHER
											 | IPCM_ANY_MAGE & ~IPCM_MAGE
											 | IPCM_ANY_MONK
											 | IPCM_ANY_ROGUE
											 | IPCM_ANY_SAVAGE;
		case TraitId::BendTheRules:		return IPCM_ANY_WARRIOR
											 | IPCM_ANY_ARCHER 
											 | IPCM_ANY_MAGE// mor: might wanna remove this as magi have 900 cap anyway
											 | IPCM_MONK 
											 | IPCM_KENSEI 
											 | IPCM_SHUGOKI 
											 | IPCM_ANY_ROGUE 
											 | IPCM_SAVAGE 
											 | IPCM_BERSERKER;
        case TraitId::Barbarian:		return IPCM_SAVAGE;
        case TraitId::Psion:		    return IPCM_MAGE;
        case TraitId::Devastator:	    return IPCM_INQUISITOR;
        case TraitId::Thrill_Seeker:    return IPCM_SCOUT
                                             | IPCM_KENSEI
                                             | IPCM_SHUGOKI
                                             | IPCM_ROGUE
                                             | IPCM_IRON_MAIDEN
                                             | IPCM_SAVAGE
                                             | IPCM_BERSERKER
                                             | IPCM_DIMACHAERUS;
        case TraitId::Psychotic:        return IPCM_BERSERKER;
        case TraitId::SmallFrame:       return IPCM_WARRIOR | IPCM_INQUISITOR | IPCM_TEMPLAR
                                             | IPCM_ANY_ARCHER
                                             | IPCM_ANY_MAGE
                                             | IPCM_MONK | IPCM_KENSEI | IPCM_SHINOBI
                                             | IPCM_ASSASSIN | IPCM_BOMBARDIER
                                             | IPCM_SECUTOR | IPCM_DRUID;
        case TraitId::BlueBlood:		return IPCM_ALL_CLASSES & ~IPCM_ANY_SAVAGE;
        case TraitId::Juggernaut:		return IPCM_ANY_WARRIOR | IPCM_ANY_MONK | IPCM_ANY_SAVAGE & ~IPCM_SAVAGE;
        case TraitId::Prodigy:		    return IPCM_ALL_CLASSES;
        case TraitId::Insensitive:		return IPCM_ALL_CLASSES & ~IPCM_ANY_MAGE;
        case TraitId::Hydramancer:      return IPCM_WARLOCK;
        case TraitId::IronFisted:       return IPCM_WARRIOR | IPCM_TEMPLAR | IPCM_ARCHER | IPCM_ANY_MONK 
                                             | IPCM_ROGUE | IPCM_IRON_MAIDEN | IPCM_BERSERKER | IPCM_EXECUTIONER | IPCM_ANY_GLADIATOR;
        case TraitId::Paladin:          return IPCM_TEMPLAR;
        case TraitId::Unshakable:		return IPCM_ALL_CLASSES;
        case TraitId::ResoluteGuard:	return IPCM_WARRIOR | IPCM_INQUISITOR
                                               | IPCM_ANY_ARCHER
                                               | IPCM_ANY_MAGE
                                               | IPCM_MONK | IPCM_KENSEI | IPCM_SHINOBI
                                               | IPCM_ROGUE | IPCM_IRON_MAIDEN | IPCM_BOMBARDIER
                                               | IPCM_ANY_SAVAGE;
        case TraitId::Ranger:		        return IPCM_WARRIOR;
        case TraitId::Mamluk:		        return IPCM_ELEMENTALIST;
        case TraitId::Vigorous:		        return IPCM_ANY_WARRIOR
                                               | IPCM_ARCHER | IPCM_TRAPPER
                                               | IPCM_SHUGOKI
                                               | IPCM_ASSASSIN /* | IPCM_IRON_MAIDEN*/
                                               | IPCM_BERSERKER/* | IPCM_EXECUTIONER*/
                                               | IPCM_THRAEX | IPCM_SECUTOR | IPCM_DRUID;
        case TraitId::Pistoleer:            return IPCM_GUARDIAN;
        case TraitId::FearTheReaper:        return IPCM_INQUISITOR|IPCM_GUARDIAN|IPCM_ANY_MONK
                                               | IPCM_BERSERKER | IPCM_ANY_GLADIATOR | IPCM_DRUID;
        case TraitId::NastyDisposition:     return IPCM_WARRIOR|IPCM_GUARDIAN|IPCM_ARCHER|IPCM_SHARPSHOOTER|IPCM_TRAPPER|IPCM_ANY_MONK|IPCM_ANY_SAVAGE;
        case TraitId::CrowdSeeker:          return IPCM_ALL_CLASSES;
        case TraitId::Stoneform:            return IPCM_ANY_WARRIOR | IPCM_ANY_ARCHER | IPCM_ANY_MONK 
                                                | IPCM_ASSASSIN | IPCM_IRON_MAIDEN | IPCM_ANY_SAVAGE & ~IPCM_SAVAGE;
        case TraitId::LitheBuild:           return IPCM_ANY_WARRIOR|IPCM_ANY_ARCHER|IPCM_ANY_MONK|IPCM_ANY_ROGUE & ~IPCM_IRON_MAIDEN|IPCM_ANY_SAVAGE;
        case TraitId::BloodOath:            return IPCM_WARRIOR | IPCM_INQUISITOR | IPCM_ANY_ARCHER | IPCM_MAGE | IPCM_ELEMENTALIST| IPCM_MONK | IPCM_SHINOBI 
                                                | IPCM_ANY_ROGUE | IPCM_SAVAGE | IPCM_ANY_GLADIATOR & ~IPCM_THRAEX | IPCM_DRUID;
        case TraitId::ManaFlux:             return IPCM_MAGE | IPCM_WARLOCK | IPCM_ANY_SUMMONER | IPCM_DRUID;
        case TraitId::Coinbound:            return IPCM_ANY_WARRIOR | IPCM_ANY_ARCHER | IPCM_ANY_MAGE | IPCM_ANY_ROGUE & ~IPCM_ROGUE | IPCM_ANY_MONK
                                                | IPCM_SAVAGE | IPCM_EXECUTIONER | IPCM_BERSERKER;
        case TraitId::Doomwhorl:            return IPCM_ANY_WARRIOR & ~IPCM_WARRIOR | IPCM_ANY_ARCHER | IPCM_ANY_MAGE 
                                                | IPCM_ANY_MONK | IPCM_ANY_SAVAGE | IPCM_BOMBARDIER | IPCM_ASSASSIN | IPCM_DRUID;
        case TraitId::LordOfTheRings:       return IPCM_MONK;
        case TraitId::Crupellarius:         return IPCM_MURMILLO;
        case TraitId::Fatality:             return IPCM_ANY_WARRIOR & ~IPCM_INQUISITOR | IPCM_ANY_ARCHER & ~IPCM_SCOUT 
                                                | IPCM_ANY_MONK & ~IPCM_MONK | IPCM_ROGUE | IPCM_ASSASSIN | IPCM_ANY_SAVAGE;
        case TraitId::Abnegation:           return IPCM_TEMPLAR;
        case TraitId::Giant:                return IPCM_ANY_WARRIOR | IPCM_ANY_ARCHER | IPCM_ANY_MONK & ~IPCM_SHINOBI | IPCM_ANY_ROGUE & ~IPCM_ROGUE | IPCM_ANY_SAVAGE;
        case TraitId::Feral:                return IPCM_BEASTMASTER;
    }
}

//----- (th2) ------------------------------------------------------------
TRAIT_GROUP_MASK GetTraitGroups( TraitId id )
{
    switch( id )
    {
        default:
        case TraitId::NoTrait:          return TGM_NONE;
        case TraitId::Gifted:           return TGM_MORE_MAX_STATS;
        case TraitId::Kamikaze:         return TGM_NONE;
        case TraitId::HeavyHanded:      return TGM_DMG_UP;
        case TraitId::Skilled:          return TGM_PERK_POINTS_DOWN;
        case TraitId::Barbarism:        return TGM_MAX_REZ_UP;
        case TraitId::Finesse:          return TGM_CRIT_UP;
        case TraitId::Survivor:         return TGM_REZ_DOWN;
        case TraitId::Weird:            return TGM_WEIRD;
        case TraitId::OldFashioned:     return TGM_OLDFASH;
        case TraitId::Domesticated:     return TGM_STATS_LESS;
        case TraitId::Rabid:            return TGM_RABID;
        case TraitId::Bruiser:          return TGM_BRUISER;
        case TraitId::Negotiant:        return TGM_MORE_MAX_STATS;
        case TraitId::Leper:            return TGM_LEPER;
        case TraitId::Rudiarius:        return TGM_NONE;
        case TraitId::Scrounger:        return TGM_MF_MORE;
        case TraitId::Adventurer:       return TGM_STATS_MORE;
        case TraitId::ThickSkinned:     return TGM_NONE;
        case TraitId::RatelHide:        return TGM_KB_DOWN;
        case TraitId::Forgetful:        return TGM_MORE_XP;
        case TraitId::WildSorcery:      return TGM_SPELLS_ON;
        case TraitId::Cautious:         return TGM_NONE;
		case TraitId::OldHabit:         return TGM_NONE;
		case TraitId::GoldDigger:       return TGM_NONE;
		case TraitId::ArrowDancing:     return TGM_NONE;
		case TraitId::PointBlank:	    return TGM_NONE;
		case TraitId::FastMetabolism:	return TGM_FAST_MET;
		case TraitId::Bouncer:		    return TGM_BOUNCE;
		case TraitId::Sandman:		    return TGM_MELEE_AUTOHIT_UP;
		case TraitId::ARoseWithThorns:	return TGM_NONE;
		case TraitId::Pyromaniac:		return TGM_NONE;
		case TraitId::AvatarOfCold:		return TGM_SPELLS_ON;
		case TraitId::BestDefense:		return TGM_NONE;
		case TraitId::GrimDeal:			return TGM_PERK_POINTS_UP;
		case TraitId::HolyAura:			return TGM_NONE;
		case TraitId::Strafer:			return TGM_ATK_SPD_DOWN;
		case TraitId::PiercingShot:		return TGM_NONE;
		case TraitId::Destroyer:		return TGM_DESTROY;
		case TraitId::ChromaticSkin:	return TGM_REZ_UP;
		case TraitId::Tormentor:		return TGM_CRIT_DOWN;
		case TraitId::Armadillo:		return TGM_DMG_DOWN;
		case TraitId::BloodyMess:		return TGM_NONE;
		case TraitId::Bestiarius:		return TGM_NONE;
		case TraitId::Petrifier:		return TGM_NONE;
		case TraitId::DarkPact:			return TGM_NONE;
		case TraitId::Toxic_at_Heart:	return TGM_NONE;
		case TraitId::Hunger:			return TGM_NONE;
		case TraitId::Fechtmeister:		return TGM_MELEE_AUTOHIT_DOWN;
		case TraitId::WreckingBlock:	return TGM_NONE;
		case TraitId::TwoTowers:		return TGM_NONE;
		case TraitId::PuncturingStab:	return TGM_NONE;
		case TraitId::BloodAndSand:		return TGM_NONE;
		case TraitId::BloodForBlood:	return TGM_NONE;
		case TraitId::BigFrame:			return TGM_NONE;
        case TraitId::CrossbowTraining:	return TGM_NO_PISTOL;
		case TraitId::BlisteredSkin:	return TGM_MAX_REZ_DOWN;
		case TraitId::Cleric:			return TGM_NONE;
		case TraitId::Bloodless:		return TGM_NONE;
		case TraitId::Engineer:			return TGM_NONE;
		case TraitId::Necropathy:		return TGM_NONE;
		case TraitId::TreasureHunter:	return TGM_TR_HUNTER;
		case TraitId::BreakerOfStones:	return TGM_NONE;
		case TraitId::Cannibalism:		return TGM_NONE;
		case TraitId::Black_Witchery:	return TGM_NONE;
		case TraitId::Axepertise:		return TGM_NONE;
		case TraitId::MonkeyGrip:		return TGM_NONE;
		case TraitId::Zealot:			return TGM_ATK_SPD_UP;
		case TraitId::Adrenaline:		return TGM_NONE;
		case TraitId::Sisyphean:		return TGM_LESS_XP;
		case TraitId::BendTheRules:		return TGM_MANUAL_MAX_STATS;
        case TraitId::Barbarian:		return TGM_BARBARIAN;
        case TraitId::Psion:		    return TGM_PSION;
        case TraitId::Devastator:	    return TGM_NONE;
        case TraitId::Thrill_Seeker:    return TGM_NONE;
        case TraitId::Psychotic:        return TGM_NONE;
        case TraitId::SmallFrame:       return TGM_SMALL_FRAME;
        case TraitId::BlueBlood:        return TGM_NONE;
        case TraitId::Juggernaut:       return TGM_KB_UP;
        case TraitId::Prodigy:          return TGM_SYNS_UP;
        case TraitId::Insensitive:      return TGM_SYNS_DOWN;
        case TraitId::Hydramancer:      return TGM_NONE;
        case TraitId::IronFisted:       return TGM_CRIT_DOWN;
        case TraitId::Paladin:          return TGM_PAL;
        case TraitId::Unshakable:       return TGM_AC_DOWN;
        case TraitId::ResoluteGuard:    return TGM_REZ_UP;
        case TraitId::Ranger:           return TGM_MELEE_AUTOHIT_UP;
        case TraitId::Mamluk:           return TGM_SPELLS_OFF;
        case TraitId::Vigorous:         return TGM_ATK_SPD_DOWN;
        case TraitId::Pistoleer:        return TGM_PISTOL;
        case TraitId::FearTheReaper:    return TGM_FTR;
        case TraitId::NastyDisposition: return TGM_NASTY;
        case TraitId::CrowdSeeker:      return TGM_NONE;
        case TraitId::Stoneform:        return TGM_AC_UP;
        case TraitId::LitheBuild:       return TGM_LITHE;
        case TraitId::BloodOath:        return TGM_BLOOD_OATH;
        case TraitId::ManaFlux:         return TGM_NONE;
        case TraitId::Coinbound:        return TGM_COINBOUND;
        case TraitId::Doomwhorl:        return TGM_DOOMWHORL;
        case TraitId::LordOfTheRings:   return TGM_LOTR;
        case TraitId::Crupellarius:     return TGM_CRUPELLARIUS;
        case TraitId::Fatality:         return TGM_NONE;
        case TraitId::Abnegation:       return TGM_ABNEG;
        case TraitId::Giant:            return TGM_GIANT;
        case TraitId::Feral:            return TGM_NONE;
    }
}

//----- (th2) ------------------------------------------------------------
Portrait GetTraitPicId( TraitId id )
{
    if( id == TraitId::NoTrait ){
        return PcxHeros[50];
    }else{
        return PcxHeros[81 + (int)id - 1];
    }
}

//----- (th2) ------------------------------------------------------------
std::vector<TraitId> CalcAvailableTraitList( PLAYER_FULL_CLASS fullClass, decltype(Player::traits) traits )
{
    const uint playerFullClassMask = (1u << static_cast<uint>( fullClass ) );
    
    std::vector<TraitId> result;
    
    auto isTraitExcluded = [&]( TraitId traitId ) -> bool
    {
		// Nothing to exclude if other trait is not selected
        if( traitId == TraitId::NoTrait ) return false;
        // Exclude the same trait
        if( has(traits, traitId) ) return true;

		// Conditions for specific traits
		switch( traitId ){
            case TraitId::Fatality:
		    case TraitId::CrowdSeeker: if( IsMultiplayer() ) return true;
		}

        // Exclude trait if it is from groups that conflicted with other trait groups !! Need to dig deeper here. Much more easier way to exclude trait from trait list after picking up first one
        auto groups = GetTraitGroups( traitId );
		for(auto& trait: traits) groups |= GetTraitGroups(trait);
		if( std::any_of( TraitsExcludedCombinations.begin(), TraitsExcludedCombinations.end(),
                         [groups]( auto excluded ){ return (groups & excluded) == excluded; } ) ){
            return true;
        }
        // Not exclude otherwise
        return false;
    };
    
    for( int i = 0, ie = int(TraitId::TRAIT_COUNT); i < ie; ++i ){
        const auto traitId = static_cast<TraitId>( i );
        const uint requiredClassMask = GetRequiredClassMaskForTrait( traitId );
        if( requiredClassMask & playerFullClassMask ){
            if( !isTraitExcluded( traitId ) ){
                result.push_back( traitId );
            }
        }
    }
    
    return result;
}

//----- (th2) ------------------------------------------------------------
void __fastcall SetTraitInfoText( HWND parent, CharSaveInfo* saveInfo, TraitId traitId, int buttonId )
{
    ShowWindowList( parent, CharParamNameList, SW_HIDE );
    ShowWindowList( parent, CharParamValueList, SW_HIDE );
    ShowWindowList( parent, ClassDescriptionList, SW_SHOW );
    
    Portrait portrait = PcxHeros[5];
    const char* description = "";
    
    // Now we use just hero image
    portrait = getHeroPortrait(saveInfo);
    
    switch(buttonId){
	case ID_TRAITS_CONTINUE:
		description = "Choose up to 4 Traits. This is optional. Traits stay with your character forever. Traits can activate or deactivate some gamechangers, perks and synergies.";
		break;
    case ID_SELECT_TRAIT_1 or ID_SELECT_TRAIT_2 or ID_SELECT_TRAIT_3 or ID_SELECT_TRAIT_4:
		description = GetTraitDescription( saveInfo->Traits[buttonId - ID_SELECT_TRAIT_1] );
		portrait    = GetTraitPicId      ( saveInfo->Traits[buttonId - ID_SELECT_TRAIT_1] );
		break;
	default:
		description = GetTraitDescription( traitId );
		portrait    = GetTraitPicId      ( traitId );
		break;
    }

	//const int BUFFER_SIZE = 256;
	//char buffer[ BUFFER_SIZE ];
	//LoadStringA( HInstance, descriptionIndex, buffer, BUFFER_SIZE );
	//description = buffer;

    SetWindowTextA( GetDlgItem( parent, ID_CLASS_DESCRIPTION ), description );

    DrawTextToElemList( parent, ClassDescriptionList, 1 );
    
    HWND portraitWin = GetDlgItem( parent, ID_PLAYER_PORTRAIT );
    RECT rect;
    InvalidateRect( portraitWin, 0, 0 );
    GetClientRect( portraitWin, &rect );
    AdjustScrollRect( &rect, 0, rect.bottom * portrait.id );
	SDlgSetBitmap( portraitWin, 0, "Static", -1, 1, portrait.pcx->data, &rect, portrait.pcx->size.cx, portrait.pcx->size.cy, -1 );
}

//----- (th2) ------------------------------------------------------------
void TraitSelectButtonRefresh( const HWND hdlg, int buttonId, TraitId traitId )
{
    if( const HWND btn = GetDlgItem( hdlg, buttonId ); btn ){
        if( auto textWin = (TextWin*) GetWindowLongA( btn, GWL_USERDATA ); textWin ){
            WriteTextToElemData( textWin, GetTraitName( traitId ) );
        }
    }
};

//----- (th2) ------------------------------------------------------------
void TraitSelectNextTrait( const HWND hdlg, const HWND button, int order )
{
    const auto& buttonsOrder = ( order > 0 ) ? nextTraitOrder : prevTraitOrder;
    HWND currentButton = button;
    for( auto i{ 0u }, ie{ buttonsOrder.size() }; i < ie; ++i ){
        currentButton = GetDlgItem( hdlg, buttonsOrder[ GetWindowLongA( currentButton, GWL_ID ) - ID_TRAIT_1 ] );
        if( IsWindowEnabled( currentButton ) ){
            SetFocus( currentButton );
            break;
        }
    }
}

//----- (th2) ------------------------------------------------------------
int GetAvailableTraitCount()
{
    return static_cast<int>( TraitsAvailableList.size() );
}

//----- (th2) ------------------------------------------------------------
void __fastcall ResetTraitButtons( HWND hdlg, const int topIndex )
{
    for( int i = 0; TraitButtonList[i] != 0; ++i ){
        int saveButtonId = TraitButtonList[i];
        HWND saveButton = GetDlgItem( hdlg, saveButtonId );
        if( saveButton ){
            const int currentTraitIndex = topIndex + i;
            if( currentTraitIndex < GetAvailableTraitCount() ){
                EnableWindow( saveButton, true );
                if( auto traitWin = (TraitWin*) GetWindowLongA( saveButton, GWL_USERDATA ); traitWin ){
                    WriteTextToElemData( (TextWin *) traitWin, GetTraitName( TraitsAvailableList[currentTraitIndex] ) );
                    traitWin->traitIndex = (void*)currentTraitIndex;
                }
            }else{
                EnableWindow( saveButton, false );
            }
        }
    }
    ResetButtonText( hdlg, TraitButtonList, 2, 1 );
}

//----- (th2) ------------------------------------------------------------
void TraitRefreshCurrentTraitInfo( HWND hdlg, int button )
{
    if( const HWND activeElem = GetDlgItem( hdlg, button ); activeElem ){
        if( const auto traitWin = (TraitWin *)GetWindowLongA( activeElem, GWL_USERDATA ); traitWin ){
            const auto buttonId = GetWindowLongA( activeElem, GWL_ID );
            SelectedTraitId = TraitsAvailableList[(int)traitWin->traitIndex];
            SetTraitInfoText( GetParent( hdlg ), &NewSaveInfo, SelectedTraitId, buttonId );
        }
    }
}

//----- (th2) ------------------------------------------------------------
int __fastcall GetTraitElemIndex( HWND elem )
{
    int result = 0;
    if( elem ){
        if( auto* traitWin = (TraitWin *) GetWindowLongA( elem, GWL_USERDATA ); traitWin ){
            return (int)traitWin->traitIndex;
        }
    }
    return result;
}

//----- (th2) ------------------------------------------------------------
void TraitRefreshScrollState( const HWND hdlg )
{
    SetScrollOnElem( hdlg, ID_SCROLL, GetAvailableTraitCount(), GetTraitElemIndex( GetFocus() ) );
}

//----- (th2) ------------------------------------------------------------
void __fastcall TraitPageDown( const HWND button )
{
    const HWND hdlg = GetParent( button );
    if( !hdlg ){
        return;
    }
    
    const HWND firstBut = GetDlgItem( hdlg, ID_TRAIT_1 );
    if( !firstBut ){
        return;
    }
    
    auto traitWin = (TraitWin *) GetWindowLongA( GetDlgItem( hdlg, ID_TRAIT_6 ), GWL_USERDATA );
    if( !traitWin ){
        return;
    }
    
    if( auto traitIndex = (int)traitWin->traitIndex; traitIndex + 1 < GetAvailableTraitCount() ){
        const int topSaveIndex = GetTraitElemIndex( firstBut );
        const int nextTopTraitIndex = std::min( topSaveIndex + 6, GetAvailableTraitCount() - 6 );
        PlaySoundTitleMove();
        ResetTraitButtons( hdlg, nextTopTraitIndex );
        TraitRefreshCurrentTraitInfo( hdlg, GetWindowLongA( button, GWL_ID ) );
        TraitRefreshScrollState( hdlg );
    }else{
        TraitSelectNextTrait( hdlg, firstBut, -1 );
    }
}

//----- (th2) ------------------------------------------------------------
void __fastcall TraitPageUp( const HWND button )
{
    const auto hdlg = GetParent( button );
    if( !hdlg ){
        return;
    }
    
    const HWND firstBut = GetDlgItem( hdlg, ID_TRAIT_1 );
    if( !firstBut ){
        return;
    }
    
    const auto traitWin = (const TraitWin *) GetWindowLongA( firstBut, GWL_USERDATA );
    if( !traitWin ){
        return;
    }
    
    if( const auto traitIndex = (int)traitWin->traitIndex; traitIndex == 0 ){
        const auto lastBut = GetDlgItem( hdlg, ID_TRAIT_6 );
        TraitSelectNextTrait( hdlg, lastBut, 1 );
    }else{
        const int topSaveIndex = GetTraitElemIndex( firstBut );
        const int nextTopTraitIndex = std::max( topSaveIndex - 6, 0 );
        PlaySoundTitleMove();
        ResetTraitButtons( hdlg, nextTopTraitIndex );
        TraitRefreshCurrentTraitInfo( hdlg, GetWindowLongA( button, GWL_ID ) );
        TraitRefreshScrollState( hdlg );
    }
}

//----- (th2) ------------------------------------------------------------
void __fastcall TraitDown( const HWND button )
{
    const auto hdlg = GetParent( button );
    
    if( auto traitWin = (TraitWin *) GetWindowLongA( button, GWL_USERDATA ); traitWin ){
        if( auto traitIndex = (int)traitWin->traitIndex; traitIndex + 1 < GetAvailableTraitCount() ){
            if( GetWindowLongA( button, GWL_ID ) < ID_TRAIT_6 ){
                TraitSelectNextTrait( hdlg, button, 1 );
            }else{
                if( auto newBut = GetDlgItem( hdlg, ID_TRAIT_2 ); newBut ){
                    if( auto newWin = (TraitWin*)GetWindowLongA( newBut, GWL_USERDATA ); newWin ){
                        const auto nextTopTraitIndex = (int)newWin->traitIndex;
                        PlaySoundTitleMove();
                        ResetTraitButtons( hdlg, nextTopTraitIndex );
                        TraitRefreshCurrentTraitInfo( hdlg, GetWindowLongA( button, GWL_ID ) );
                        TraitRefreshScrollState( hdlg );
                    }
                }
            }
        }
    }
}

//----- (th2) ------------------------------------------------------------
void __fastcall TraitUp( const HWND button )
{
    const auto hdlg = GetParent( button );
    
    if( GetWindowLongA( button, GWL_ID ) > ID_TRAIT_1 ){
        TraitSelectNextTrait( hdlg, button, -1 );
        return;
    }else{
        if( auto traitWin = (TraitWin*)GetWindowLongA( button, GWL_USERDATA ); traitWin ){
            if( auto traitIndex = (int)traitWin->traitIndex; traitIndex > 0 ){
                const auto nextTopTraitIndex = traitIndex - 1;
                PlaySoundTitleMove();
                ResetTraitButtons( hdlg, nextTopTraitIndex );
                TraitRefreshCurrentTraitInfo( hdlg, GetWindowLongA( button, GWL_ID ) );
                TraitRefreshScrollState( hdlg );
            }
        }
    }
}

//----- (th2) ------------------------------------------------------------
LRESULT __stdcall TraitSelectButtonProc( const HWND button, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
    LRESULT result = 0;
    auto oldProc = (WNDPROC)GetPropA( button, "UIOLDPROC" );
    const auto hdlg = GetParent( button );
    
    switch( Msg ){
        case WM_GETDLGCODE:
            result = 4;
            break;
        case WM_KEYFIRST:
            switch( wParam ){
                case VK_RETURN:
                case VK_SPACE:
                    SendMessageA( hdlg, 273, 1, 0 );
                    break;
                case VK_ESCAPE:
                    SendMessageA( hdlg, 273, 2, 0 );
                    break;
                case VK_TAB:
                    TraitSelectNextTrait( hdlg, button, ( GetKeyState( VK_SHIFT ) >= 0 ) ? 1 : -1 );
                    break;
                case VK_LEFT:
                case VK_UP:
                    TraitUp( button );
                    break;
                case VK_RIGHT:
                case VK_DOWN:
                    TraitDown( button );
                    break;
                case VK_PRIOR:
                    TraitPageUp( button );
                    break;
                case VK_NEXT:
                    TraitPageDown( button );
                    break;
                case VK_DELETE:
                    SendMessageA( hdlg, Msg, wParam, lParam );
                    break;
                default:
                    break;
            }
            break;
        case WM_DESTROY:
            RemovePropA( button, "UIOLDPROC" );
            if( oldProc ){
                SetWindowLongA( button, GWL_WNDPROC, (LONG)oldProc );
            }
            [[fallthrough]];
        default:
            if( oldProc ){
                result = CallWindowProcA_( oldProc, button, Msg, wParam, lParam );
            }else{
                result = DefWindowProcA_( button, Msg, wParam, lParam );
            }
            break;
        case WM_PAINT:
            PaintElem( button );
            break;
    }
    return result;
}

//----- (th2) ------------------------------------------------------------
int __stdcall DialogSelectTraits( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
    static int classElemList[] = { ID_CAPTION, 0 };
    static int buttons[] = { ID_TRAITS_CONTINUE, ID_SELECT_TRAIT_1, ID_SELECT_TRAIT_2, ID_SELECT_TRAIT_3, ID_SELECT_TRAIT_4, 0 };
	static int classElementList[] = { ID_OK_BUTTON, ID_CANCEL_BUTTON, 0 };
    
    int result;
	bool needToCallDefDialogProc = true;
    XinputVirtualKeyboard();
	CheckEnter( Msg, wParam, lParam );
	switch( Msg ){
		case WM_INITDIALOG:
		{
            if( is(GameMode, GM_COLISEUM, GM_CLASSIC) ){
                SDlgEndDialog(hdlg, lParam == 1 ? -1 : ID_TRAITS_CONTINUE);
                return SDlgDefDialogProc_(hdlg, Msg, wParam, lParam);
            }
		    char tempBuf[32];
			LoadStringA( HInstance, NewHeroTitle(), tempBuf, 31 );
			WriteAndDrawDialogTitle( GetParent( hdlg ), tempBuf );
			SetWindowLongA( hdlg, GWL_USERDATA, (LONG)GetWindowLongA( GetParent( hdlg ), GWL_USERDATA ) );
			InitButtonListProc( hdlg, buttons );
			InitTextElemList( hdlg, classElemList, 5 );
			InitButtonListText( hdlg, classElementList, 4, 0 );
			InitButtonListText( hdlg, buttons, 2, 1 );
			InitSelectionAndLoadPentagram( "ui_art\\focus16.pcx" );
            
            for(uint i = 0; i < size(NewSaveInfo.Traits); ++i) TraitSelectButtonRefresh( hdlg, ID_SELECT_TRAIT_1 + i, NewSaveInfo.Traits[i] );
			ResetButtonText( hdlg, buttons, 2, 1 );
			
			SDlgSetTimer( hdlg, 1, 55, 0 );
			needToCallDefDialogProc = false;
			result = 0;
			break;
		}
		case WM_DESTROY:
			DeletePentagram();
			DeleteElementListData( hdlg, buttons );
			DeleteElementListData( hdlg, classElementList );
			DeleteElementListData( hdlg, classElemList );
			WriteAndDrawDialogTitle( GetParent( hdlg ), 0 );
			break;
		
		case WM_COMMAND:
			if( HIWORD_IDA( wParam ) == BN_KILLFOCUS ){
				LeaveButton( hdlg, (HWND) lParam );
			}else if( HIWORD_IDA( wParam ) == BN_SETFOCUS ){
	
				SelectButton( (HWND) lParam );
				AnimateSelectionPentagram( hdlg, (HWND) lParam );
    
				SetTraitInfoText( GetParent( hdlg ), &NewSaveInfo, TraitId::NoTrait, (unsigned __int16) wParam );
				
			}else if( HIWORD_IDA(wParam) == BN_DOUBLECLICKED || (ushort)wParam == 1 ){
				PlaySoundTitleSelect();
				SDlgKillTimer( hdlg, 1 );
				SDlgEndDialog( hdlg, GetWindowLongA( GetFocus(), GWL_ID ) );
			}else if( (ushort) wParam == 2 ){
				PlaySoundTitleSelect();
				SDlgKillTimer( hdlg, 1 );
				SDlgEndDialog( hdlg,  2 );
			}
			
			break;
		case WM_LBUTTONDOWN:
			if( CheckCursorOnButton( hdlg, GetDlgItem( hdlg, ID_OK_BUTTON ), (unsigned __int16) lParam, (unsigned int) lParam >> 16u ) ){
				PlaySoundTitleSelect();
				SDlgKillTimer( hdlg, 1 );
				SDlgEndDialog( hdlg,  GetWindowLongA( GetFocus(), GWL_ID ) );
			}else if( CheckCursorOnButton( hdlg, GetDlgItem( hdlg, ID_CANCEL_BUTTON ), (unsigned __int16) lParam, (unsigned int) lParam >> 16u ) ){
				PlaySoundTitleSelect();
				SDlgKillTimer( hdlg, 1 );
				SDlgEndDialog( hdlg,  2 );
			}
			break;
		case WM_TIMER:
			AnimateSelectionPentagram( hdlg, GetFocus() );
			needToCallDefDialogProc = false;
			result = 0;
			break;
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			SendMessageA( SDrawGetFrameWindow( 0 ), Msg, wParam, lParam );
			break;
		default:
			break;
	}

	if( needToCallDefDialogProc )
	{
		result = SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
	}
	
	return result;
}

//----- (th2) ------------------------------------------------------------
int __stdcall DialogSelectTrait( HWND hdlg, unsigned int Msg, WPARAM wParam, LPARAM lParam )
{
    int result;
    bool needToCallDefDialogProc = true;
    
    switch( Msg ){
        case WM_INITDIALOG:
        {
            SelectedTraitId = TraitId::NoTrait;
            const auto fullClassId = GetPlayerFullClass( PLAYER_CLASS(NewSaveInfo.Class)
                                                       , PLAYER_SUBLASS(NewSaveInfo.SubClass)
                                                       , PLAYER_SPECIALIZATION(NewSaveInfo.Specialization) );
            TraitsAvailableList = CalcAvailableTraitList( fullClassId, NewSaveInfo.Traits );
            
            for( int i = 0; TraitButtonList[i] != 0; ++i ){
                int saveButtonId = TraitButtonList[i];
                HWND saveButton = GetDlgItem( hdlg, saveButtonId );
                if( saveButton ){
                    SetPropA( saveButton, "UIOLDPROC", (HANDLE)GetWindowLongA( saveButton, GWL_WNDPROC ) );
                    SetWindowLongA( saveButton, GWL_WNDPROC, (LONG)TraitSelectButtonProc );
                }
            }
            
            char tempBuf[32];
			LoadStringA( HInstance, NewHeroTitle(), tempBuf, 31 );
			WriteAndDrawDialogTitle( GetParent( hdlg ), tempBuf );
            SetWindowLongA( hdlg, GWL_USERDATA, GetWindowLongA( GetParent( hdlg ), GWL_USERDATA ) );
            InitTextElemList( hdlg, TraitTitleList, 5 );
            InitButtonListText( hdlg, TraitOkCancelList, 4, 0 );
            InitButtonListText( hdlg, TraitButtonList, 2, 1 );
            ResetTraitButtons( hdlg, 0 );
            InitSelectionAndLoadPentagram( "ui_art\\focus16.pcx" );
            SDlgSetTimer( hdlg, 1, 55, 0 );
            InitScroll( hdlg, ID_SCROLL );
            if( GetAvailableTraitCount() <= 6 ){
                ShowWindow( GetDlgItem( hdlg, ID_SCROLL ), 0 );
            }
            needToCallDefDialogProc = false;
            result = 0;
            break;
        }
        case WM_DESTROY:
        {
            DeleteScroll( hdlg, ID_SCROLL );
            DeletePentagram();
            DeleteElementListData( hdlg, TraitButtonList );
            DeleteElementListData( hdlg, TraitOkCancelList );
            DeleteElementListData( hdlg, TraitTitleList );
            WriteAndDrawDialogTitle( GetParent( hdlg ), 0 );
            break;
        }
        case WM_COMMAND:
        {
            if( HIWORD_IDA( wParam ) == BN_KILLFOCUS ){
                LeaveButton( hdlg, (HWND) lParam );
            }else if( HIWORD_IDA( wParam ) == BN_SETFOCUS ){
                InvalidateRect( GetParent( hdlg ), NULL, NULL ); //removes menu freezes // character selection menu
                UpdateWindow( GetParent( hdlg ) ); //removes menu freezes // character selection menu
                SelectButton( (HWND) lParam );
                AnimateSelectionPentagram( hdlg, (HWND) lParam );
                TraitRefreshCurrentTraitInfo( hdlg, (unsigned __int16) wParam );
                TraitRefreshScrollState( hdlg );
            }else if( HIWORD_IDA( wParam ) == BN_DOUBLECLICKED || (ushort)wParam == 1 ){ // Enter
                PlaySoundTitleSelect();
                SDlgKillTimer( hdlg, 1 );
                SDlgEndDialog( hdlg, 1 );
            }else if( (ushort)wParam == 2 ){
                PlaySoundTitleSelect();
                SDlgKillTimer( hdlg, 1 );
                SDlgEndDialog( hdlg, 2 );
            }
            break;
        }
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
        {
            int cursorX = (unsigned __int16) lParam;
            int cursorY = (unsigned int) lParam >> 16;
            if( CheckCursorOnButton( hdlg, GetDlgItem( hdlg, ID_OK_BUTTON ), cursorX, cursorY ) ){
                PlaySoundTitleSelect();
                SDlgKillTimer( hdlg, 1 );
                SDlgEndDialog( hdlg,  1 );
            }else if( CheckCursorOnButton( hdlg, GetDlgItem( hdlg, ID_CANCEL_BUTTON ), cursorX, cursorY ) ){
                PlaySoundTitleSelect();
                SDlgKillTimer( hdlg, 1 );
                SDlgEndDialog( hdlg,  2 );
            }else if( CheckCursorOnButton( hdlg, GetDlgItem( hdlg, ID_SCROLL ), cursorX, cursorY ) ){
                switch( GetScrollAction( GetDlgItem( hdlg, ID_SCROLL ), cursorX, cursorY ) ){
                    case 1:
                        TraitUp( GetFocus() );
                        break;
                    case 2:
                        TraitDown( GetFocus() );
                        break;
                    case 3:
                        TraitPageUp( GetFocus() );
                        break;
                    case 4:
                        TraitPageDown( GetFocus() );
                        break;
                    default:
                        break;
                }
            }
            break;
        }
        case WM_LBUTTONUP:
        {
            if( CheckScrollPressAndRelease( GetDlgItem( hdlg, ID_SCROLL ) ) ){
                TraitRefreshScrollState( hdlg );
            }
            break;
        }
        case WM_TIMER:
            AnimateSelectionPentagram( hdlg, GetFocus() );
            needToCallDefDialogProc = false;
            result = 0;
            break;
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
            SendMessageA( SDrawGetFrameWindow( nullptr ), Msg, wParam, lParam );
            break;
    }
    
    if( needToCallDefDialogProc ){
        result = SDlgDefDialogProc_( hdlg, Msg, wParam, lParam );
    }
    
    return result;
}
