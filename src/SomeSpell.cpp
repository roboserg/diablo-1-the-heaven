#include "stdafx.h"

//----- (0046BEF5) --------------------------------------------------------
int __fastcall CalculateManaRequiredToCastSpell(int casterIndex, int spellIndex)
{
	Player& player = Players[casterIndex];
	Spell& spell = Spells[spellIndex];
	int spellLevelToDecrement = PlayerSpellLevel(casterIndex, spellIndex) - 1;
	int spellDecrement = 0;
	if( spellLevelToDecrement > 0 ){
		spellDecrement = spellLevelToDecrement * spell.ManaDecrementOnLevel;
	}
	// Если фаер болт, делим пополам
	if( spellIndex == PS_1_FIREBOLT ){
		spellDecrement /= 2;
	}
	// cast not used
	if( spellIndex == PS_32_BONE_SPIRIT_REAL_RESSURECT && spellLevelToDecrement > 0 ){
		spellDecrement = spellLevelToDecrement * ((uint)spell.InitialManaToCast >> 3);
	}
	int initialManaToCast = spell.InitialManaToCast;
	// Если -1, значит тратим абсолютно всю имеющуюся ману.
	int manaToCast;
	if( initialManaToCast == -1 ){
		manaToCast = player.MaxBaseMana;
	}else{
		manaToCast = initialManaToCast;
	}
	int calculatedCostMana = (manaToCast - spellDecrement) * 64;
	if( spellIndex == PS_2_HEALING ){
		calculatedCostMana = GetHealManaCost( Spells[PS_2_HEALING].InitialManaToCast + 2 * player.CharLevel - spellDecrement, 8) << 6;
	}
	if( spellIndex == PS_34_HEAL_OTHER ){
		calculatedCostMana = GetHealOtherManaCost( Spells[PS_2_HEALING].InitialManaToCast + 2 * player.CharLevel - spellDecrement, 20) << 6;
	}
	if (HasTrait(casterIndex, TraitId::Weird)) {
		calculatedCostMana *= 2; // weird trait penalty, spell cost is doubled
	}
	if( player.effectFlag[EA_MANACOST] ){
		calculatedCostMana -= calculatedCostMana / 4; // -25%
	}
	// Классовые скидки стоимости заклинаний
	#ifdef CLASS_DISCOUNT
	int playerClass = player.ClassID;
	if( playerClass == PC_2_MAGE ){
		calculatedCostMana /= 2;// Магу скидка на стоимость 50%
	}else if( playerClass == PC_1_ARCHER || playerClass == PC_3_MONK || playerClass == PC_4_ROGUE ){
		calculatedCostMana -= calculatedCostMana / 4;// Лучнице, монаху и аське скидка 25%
	}
	#endif
	// Если в результате получилась цена маны меньше минимальной, приравниваем её к минимальной
	int minimalMana = Spells[spellIndex].MinimalMana;
	if( minimalMana > calculatedCostMana / 64 ){
		calculatedCostMana = minimalMana * 64;
	}
	return calculatedCostMana; // * (100 - player.ExtraSpellVal) / 100; // оригинальная баго фича, здесь по идее должно было быть ExtraSpellLevel
}

//----- (00714810) ---------------------------------------------------------
int	__fastcall GetHealManaCost( int healMana, int minHealManaLimit )
{
	//int mana;	
	int maxHealMana = Players[CurrentPlayerIndex].MaxCurMana >> 9;
	if( healMana > maxHealMana ){
		healMana = maxHealMana;
	}
	int minHealMana = maxHealMana >> 1;
	if( healMana < minHealMana ){
		healMana = minHealMana;
	}
	if( healMana < minHealManaLimit ){
		healMana = minHealManaLimit;
	}
	return healMana;
}

//----- (00714840) ---------------------------------------------------------
int	__fastcall GetHealOtherManaCost( int healMana, int minHealManaLimit )
{
	int maxHealMana = Players[CurrentPlayerIndex].MaxCurMana >> 8;
	if( healMana > maxHealMana ){
		healMana = maxHealMana;
	}
	int minHealMana = maxHealMana >> 1;
	if( healMana < minHealMana ){
		healMana = minHealMana;
	}
	if( healMana < minHealManaLimit ){
		healMana = minHealManaLimit;
	}
	return healMana;
}

//----- (0046BFFC) ---------------------------------------------------------
void __fastcall MinusManaOrChargeOrRelicByPriceOfSSpell( int playerIndex, int spellIndex )
{
	if( playerIndex != CurrentPlayerIndex || DevelopMode && FreeSpell ){
		return;
	}
 
	Player& player = Players[playerIndex];
	if( const auto [cdType, cooldown] = GetSpellCooldownValue( CurrentPlayerIndex, spellIndex ); cdType != CD_NONE ){
        player.cooldowns[cdType] = cooldown;
    }

	switch (player.readySpellType){
		case SO_1_SPELL:
			{
				int manaCost = CalculateManaRequiredToCastSpell(playerIndex, spellIndex);
				player.CurMana -= manaCost;
				player.BaseMana -= manaCost;
				break;
			}
		case SO_2_RELIC:	MinusRelictFromInventoryOrBelt(playerIndex);	break;
		case SO_3_EQUIPED_ITEM:	MinusChargeFromStaff(playerIndex, spellIndex);	break;
	}
}

//----- (0046C06F) --------------------------------------------------------
bool __fastcall IsManaEnoughtForSpell(int playerIndex, int spellNumber, char spellType, int a4)
{
	if( !a4 && Cur.GraphicsID != CM_1_NORMAL_HAND ){
		return 0;
	}
	if( !spellType ){
		return 1;
	}
	if( PlayerSpellLevel(playerIndex, spellNumber) > 0 ){
		return Players[playerIndex].CurMana >= CalculateManaRequiredToCastSpell(playerIndex, spellNumber);
	}else{
		return 0;
	}
}

//----- (00713D80) --------------------------------------------------------
bool __fastcall ReadySpellAllowed(int casterIndex, int spellIndex )
{
	switch( Players[casterIndex].readySpellType ){
	case SO_0_SKILL: return true;
	case SO_1_SPELL: return IsManaEnoughtForSpell(casterIndex, spellIndex, SO_1_SPELL, 0);
	case SO_2_RELIC: return IsHasRelicForSpell();
	case SO_3_EQUIPED_ITEM: return IsHasChargesForSpell(spellIndex);
	default:		 return true;
	}
}

//----- (0046C0C7) --------------------------------------------------------
void __fastcall CastPlayerSpell(int casterIndex, int spellIndex, int casterX, int casterY, int targetX, int targetY, int casterType, int spellLevel)
{
	if (PlayersSpellcastQueue[casterIndex].size()) {
		spellIndex = PlayersSpellcastQueue[casterIndex].front();
		PlayersSpellcastQueue[casterIndex].pop();
	}
	Spell& spell = Spells[ spellIndex ];
	if( !ReadySpellAllowed( casterIndex, spellIndex ) && !( DevelopMode && FreeSpell) ){
		return;
	}
	if( CooldownCheck( casterIndex, spellIndex ) ){
		return;
	}

	int direction = 0;
	if( casterType == CT_0_PLAYER ){
		direction = Players[ casterIndex ].dir;
		int baseMissile = PS_0_NONE;
		switch (spellIndex) { // стена/кольцо молний и стена/кольцо огня.  This should exclude Bombardier's flask flames.
		case PS_6_FIRE_WALL:
		case PS_19_FLAME_RING:
			baseMissile = MI_5_FIRE_WALL_SEGMENT; // Why not MI_81_FLAME_RING???
			break;
		case PS_40_LIGHTING_WALL:
		case PS_17_LIGHTING_RING:
			baseMissile = MI_70_LIGHTNING_WALL_SEGMENT;
			break;
		default:
			break;
		}

		if( baseMissile ){ // убирание предыдущих кастов стен/кольц огня/молнии.
			direction = Players[casterIndex].newDirection;
			if (SpellIdxToWallType.find(spellIndex) != SpellIdxToWallType.end()) { // If spell type is one of the recast types, remember spell parameters for recast.
				uchar wallSpellIndex = SpellIdxToWallType[spellIndex];
				PlayerWall[casterIndex][wallSpellIndex][0] = casterX;
				PlayerWall[casterIndex][wallSpellIndex][1] = casterY;
				PlayerWall[casterIndex][wallSpellIndex][2] = targetX;
				PlayerWall[casterIndex][wallSpellIndex][3] = targetY;
				PlayerWall[casterIndex][wallSpellIndex][4] = spellLevel;
				PlayerWall[casterIndex][wallSpellIndex][5] = direction;
				PlayerWall[casterIndex][wallSpellIndex][6] = 0; // Help determine remaining wall time for recast.
			}
			for( int spellIndexIndex = 0; spellIndexIndex < MissileAmount; spellIndexIndex++ ){
				Missile& missile = Missiles[ MissileIndexes[ spellIndexIndex ] ];
				if( missile.BaseMissileIndex == baseMissile
				 && (baseMissile == MI_17_BLOOD || baseMissile == MI_2_HYDRA || missile.CasterIndex == casterIndex ) // First 2 checks seem redundant
				 && missile.TimeToLive > 15
				 && missile.spellIndex != 0 ){ // Fix for Fire Wall / Lightning Wall cast cancelling Bombardier's flask flames of the same type.  Flask flames have .spellIndex = 0 while walls > 0.  Better fix would be creating seperate flame missiles for flasks.
					missile.TimeToLive = 15;
				}
			}
		}
	}else if( casterType == CT_1_MONSTER_AND_TRAP ){ // caster is monster
		direction = Monsters[ casterIndex ].Orientation;
	} // else caster is trap ?
	for( int i = 0; i < 3 && spell.Effect[ i ]; i++ ){
		CastMissile( casterX, casterY, targetX, targetY, direction, spell.Effect[ i ], casterType, casterIndex, 0, spellLevel, spellIndex ); // Dragon здесь кастуется манашилд
	}
	if( spell.Effect[ 0 ] == MI_10_TOWN_PORTAL ){ // Town portal
		MinusManaOrChargeOrRelicByPriceOfSSpell( casterIndex, PS_7_TOWN_PORTAL );// Почему взятие платы не добавили туда к порталу или наоборот все взятие платы не добавили сюда?
	}
	if( spell.Effect[ 0 ] == MI_52_CHARGED_BOLT ){ // Костыль. По хорошему нужно сделать как у всех нормальных кастов. две составляющих.
												 // одна берёт плату и создаёт болты, а другая собственно болты.
												 // Charged bolt молния
		MinusManaOrChargeOrRelicByPriceOfSSpell( casterIndex, PS_30_CHARGED_BOLT );
		int boltCount = (spellLevel / 4) + 2;
		while( boltCount-- ){
			CastMissile( casterX, casterY, targetX, targetY, direction, MI_52_CHARGED_BOLT, casterType, casterIndex, 0, spellLevel, spellIndex );
		}
	}
}

// For wall recast as player enters map.
void __fastcall ReCastPlayerSpell(int casterIndex, int spellIndex, int casterX, int casterY, int targetX, int targetY, int casterType, int spellLevel)
{
	if (SpellIdxToWallType.find(spellIndex) == SpellIdxToWallType.end()) { // Don't recast if spell is not designated wall type.
		return;
	}
	Spell& spell = Spells[spellIndex];
	uchar wallSpellIndex = SpellIdxToWallType[spellIndex];
	int direction = PlayerWall[casterIndex][wallSpellIndex][5]; // Retrieve direction for wall recast.
	for (int i = 0; i < 3 && spell.Effect[i]; i++) {
		CastMissile(casterX, casterY, targetX, targetY, direction, spell.Effect[i], casterType, casterIndex, 0, spellLevel, spellIndex);
	}
}

//----- (0046C1AE) --------------------------------------------------------
void __fastcall RisingPlayer(uint casterPlayerIndex, uint revivedPlayerIndex)
{
	Player& caster = Players[casterPlayerIndex];
	Player& revivedPlayer = Players[revivedPlayerIndex];
	if( casterPlayerIndex < 4 && revivedPlayerIndex < 4 ){
		CastMissile(revivedPlayer.Row, revivedPlayer.Col, revivedPlayer.Row, revivedPlayer.Col, 0, MI_62_RISING_2, CT_0_PLAYER, casterPlayerIndex, 0, 0, 0 );
	}
	if( casterPlayerIndex == CurrentPlayerIndex ){
		SetCursorGraphics(CM_1_NORMAL_HAND);
	}
	if( casterPlayerIndex < 4 && revivedPlayerIndex < 4 ){
		if( ! revivedPlayer.CurLife ){
			if( revivedPlayerIndex == CurrentPlayerIndex ){
				IsPlayerDead = 0;
				ResetMainMenu();
			}
			ClearPlayerPath(revivedPlayerIndex);
			revivedPlayer.destAction = PCMD_M1_NOTHING;
			revivedPlayer.notHittableInTransition = 0;
			SetPlayerHitPoints(revivedPlayerIndex, 10 * 64);
			revivedPlayer.BaseLife = revivedPlayer.MaxBaseLife + revivedPlayer.CurLife - revivedPlayer.MaxCurLife;
			revivedPlayer.CurMana = 0;
			revivedPlayer.BaseMana = revivedPlayer.MaxBaseMana + revivedPlayer.CurMana - revivedPlayer.MaxCurMana;
			RecalcPlayer(revivedPlayerIndex, 1);
			if( revivedPlayer.dungeon == Dungeon ){
				StartStand(revivedPlayerIndex, revivedPlayer.dir);
			}else{
				revivedPlayer.CurAction = PCA_0_STAND;
			}
		}
	}
}

//----- (0046C2DF) --------------------------------------------------------
void __fastcall HealOther( int casterPlayerIndex, int healedPlayerIndex )
{
	Player& caster = Players[casterPlayerIndex];
	Player& healedPlayer = Players[healedPlayerIndex];
	if( casterPlayerIndex < 0 || casterPlayerIndex >= 4 ){
		return;
	}
	if( casterPlayerIndex == CurrentPlayerIndex ){
		SetCursorGraphics(CM_1_NORMAL_HAND);
	}
	if( healedPlayerIndex < 0 || healedPlayerIndex >= 4 ){
		return;
	}
	if( healedPlayer.CurLife <= 0 ){
		return;
	}
	uint healValue = (RNG(10) + 1) << 6;
	for( int i = 0; i < caster.CharLevel; i++ ){
		healValue += (RNG(4) + 1) << 6;
	}
	for( int i = 0; i < PlayerSpellLevel(casterPlayerIndex, PS_34_HEAL_OTHER); i++ ){
		healValue += (RNG(6) + 1) << 6;
	}
	switch (caster.ClassID ){
		case PC_5_SAVAGE:
		case PC_0_WARRIOR:		healValue *= 2;			break;
		case PC_4_ROGUE:
		case PC_1_ARCHER:		healValue += healValue >> 1;	break;
		case PC_3_MONK:			healValue *= 3;			break;
	}
	healedPlayer.CurLife += healValue;
	if( healedPlayer.CurLife > healedPlayer.MaxCurLife ){
		healedPlayer.CurLife = healedPlayer.MaxCurLife;
	}
	healedPlayer.BaseLife += healValue;
	if( healedPlayer.BaseLife > healedPlayer.MaxBaseLife ){
		healedPlayer.BaseLife = healedPlayer.MaxBaseLife;
	}
}
