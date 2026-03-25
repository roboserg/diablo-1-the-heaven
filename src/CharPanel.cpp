#include "stdafx.h"

DisplayObject CharPanelRect;
DisplayObject CharPanelCurStrRect;
DisplayObject CharPanelCurMagRect;
DisplayObject CharPanelCurDexRect;
DisplayObject CharPanelCurVitRect;
DisplayObject CharPanelBaseStrRect;
DisplayObject CharPanelBaseMagRect;
DisplayObject CharPanelBaseDexRect;
DisplayObject CharPanelBaseVitRect;
DisplayObject CharPanelDamageRect;
DisplayObject CharPanelResistAcid;
DisplayObject CharPanelResistMagic;
DisplayObject CharPanelResistFire;
DisplayObject CharPanelResistCold;
DisplayObject CharPanelResistLightning;
DisplayObject CharPanelResistArrow;
DisplayObject CharPanelResistMelee;
DisplayObject CharPanelXp;
DisplayObject CharPanelXpNext;
DisplayObject CharPanelArmorClass;
DisplayObject charPanelToHit;
DisplayObject CharPanelLifeRegen;
DisplayObject CharPanelManaRegen;
DisplayObject CharPanelCritDmg;
DisplayObject CharPanelCritChance;

int MinDmgFromChar = 0;
int MaxDmgFromChar = 0;

//----- (th2) ------------------------------------------------------------
void DrawBox(const DisplayObject& displayObjectRef, uchar color)
{
	int xPos = displayObjectRef.Left + Screen_LeftBorder;
	int yPos = displayObjectRef.Top + Screen_TopBorder;
	int width = displayObjectRef.Width;
	int bottom = yPos + displayObjectRef.Heigth;
	for( int y = yPos; y < bottom; ++y ){
		memset( WorkingSurface + y * WorkingWidth + xPos, color, width );
	}
}

//----- (th3) ------------------------------------------------------------
void DrawRect(const DisplayObject& displayObjectRef, uchar color)
{
	int xPos = displayObjectRef.Left + Screen_LeftBorder;
	int yPos = displayObjectRef.Top + Screen_TopBorder;
	int width = displayObjectRef.Width;
	int bottom = yPos + displayObjectRef.Heigth;
	memset( WorkingSurface + yPos * WorkingWidth + xPos, color, width );
	for( int y = yPos + 1; y < bottom - 1; ++y ){
		WorkingSurface[y * WorkingWidth + xPos] = color;
		WorkingSurface[y * WorkingWidth + xPos + width - 1] = color;
	}
	memset( WorkingSurface + (bottom - 1) * WorkingWidth + xPos, color, width );
}

//----- (th2) ------------------------------------------------------------
void PrintDamageRangeToStringBuffer( const i64 minValue, const i64 maxValue, bool parenthesis = false )
{
    auto printK = []( auto& buffer, i64 value )
    {
	    if (value < 500) {
	        sprintf( buffer, "%lld", value );
        }else{
            const i64 numk = value / 1000;
            const i64 decimal = (value - (1000 * numk)) / 100;
            sprintf( buffer, "%lld", numk );
            if( decimal > 0 ){
                sprintf( buffer, "%s.%lld", buffer, decimal );
            }
            sprintf( buffer, "%sK", buffer );
        }
    };
	char minK[20], maxK[20];
	if( parenthesis ){
		printK( minK, abs(minValue) );
		printK( maxK, abs(maxValue) );
		if( maxValue && minValue != maxValue ){
			sprintf( InfoPanelBuffer, "%c(%s-%s)", minValue < 0 ? '-' : '+', minK, maxK);
		}else{
			sprintf( InfoPanelBuffer, "%c%s", minValue < 0 ? '-' : '+', minK );
		}
	}else{
		printK( minK, minValue );
		printK( maxK, maxValue );
		sprintf( InfoPanelBuffer, "%s-%s", minK, maxK );
	}
}

//----- (th2) -------------------------------------------------------------
int GetColorNumber(int pointsAddedByItems)
{
	if( pointsAddedByItems > 0 ){
		return C_1_Blue;
	}else if( pointsAddedByItems < 0 ){
		return C_2_Red;
	}else{
		return C_0_White;
	}
}

//----- (th2) -------------------------------------------------------------
int ResistColor(float resistPercent)
{
	if( resistPercent == 0 ){
		return C_0_White;
	}else if( resistPercent < Players[CurrentPlayerIndex].resistPercentLimit ){
		return C_1_Blue;
	}else{
		return C_3_Gold;
	}
}

//----- (th2) -------------------------------------------------------------
const char* GetFullClassName( int playerIndex )
{
	Player& player = Players[playerIndex];
	PLAYER_FULL_CLASS fullClass = player.fullClassId;
	const char* name = FullPlayerClasses[fullClass].name;
	switch( fullClass ){
	case PFC_SAVAGE:      if( HasTrait(playerIndex, TraitId::Barbarian)  )	name = "Barbarian";		break;
	case PFC_GUARDIAN:    if (HasTrait(playerIndex, TraitId::Pistoleer))	name = "Pistoleer";		break;
	case PFC_EXECUTIONER: if( player.gameChanger & BIT(GC_9_NIGHT_KIN)   )	name = "Night Kin";		break;
	case PFC_MAGE:        if (HasTrait(playerIndex, TraitId::Psion))		name = "Psion";
						  else if (GameMode == GM_CLASSIC)					name = "Sorcerer";		break;
	case PFC_INQUISITOR:  if( HasTrait(playerIndex, TraitId::Devastator) )	name = "Devastator";	break;
	case PFC_WARLOCK:	  if (HasTrait(playerIndex, TraitId::Hydramancer))	name = "Hydramancer";	break;
	case PFC_MURMILLO:	  if (HasTrait(playerIndex, TraitId::Crupellarius))	name = "Crupellarius";	break;
	case PFC_TEMPLAR:	  if (HasTrait(playerIndex, TraitId::Paladin))		name = "Paladin";		break;
	case PFC_ELEMENTALIST:if (HasTrait(playerIndex, TraitId::Mamluk))		name = "Mamluk";		break;
	case PFC_WARRIOR:	  if (HasTrait(playerIndex, TraitId::Fechtmeister)){ name = "Fechtmeister"; }
						  else if (HasTrait(playerIndex, TraitId::Ranger)) { name = "Ranger"; }		break;
	case PFC_ARCHER:	  if (GameMode == GM_CLASSIC)						name = "Rogue";			break;
	}
	return name;
}

//----- (00405B43) --------------------------------------------------------
void CharPanel_Draw()
{
	char stringBuffer[64];
	int fontColor;
	
	int xoffset = CharPanelRect.Left;
	int yoffset = CharPanelRect.Top;
	
	Surface_DrawCEL(CharPanelRect.Left + Screen_LeftBorder, CharPanelRect.Down + Screen_TopBorder, Data_CharCEL, 1, CharPanelRect.Width);
    
	Player& player = Players[CurrentPlayerIndex];
	DrawText(20 + xoffset, 32 + yoffset, 151 + xoffset, player.playerName, 0);// Имя персонажа
	
	DrawText(168 + xoffset, 32 + yoffset, 299 + xoffset, GetFullClassName(CurrentPlayerIndex), 0);// Класс
	sprintf(stringBuffer, "%i", player.CharLevel);// Уровень
	DrawText(66 + xoffset, 69 + yoffset, 109 + xoffset, stringBuffer, player.CharLevel >= PlayerMaxLevel() ? 3 : 0);
	PrintBigNum(stringBuffer, player.Xp, '.', 9); // XP unsigned patch (9)

	DrawText(215 + xoffset, 69 + yoffset, 300 + xoffset, stringBuffer, player.Xp >= LevelUp(PlayerMaxLevel() - 1) ? 3 : 0 );// Количество опыта, максимальный опыт золотым
	if( player.CharLevel >= PlayerMaxLevel() ){
		strcpy(stringBuffer, "None");
		fontColor = C_3_Gold;
	}else{
		PrintBigNum(stringBuffer, player.NextLevelUp, '.', 9); // XP unsigned patch (10)
		fontColor = C_0_White;
	}
	int separator = 225;
	char* separator_text = "/";
	DrawText(215 + xoffset, 97 + yoffset, 300 + xoffset, stringBuffer, fontColor);// опыта до следующего уровня
	sprintf(stringBuffer, "%i", player.DamageFromEnemy);
	DrawText(190 + xoffset, 137 + yoffset, separator-GetTextWidth(separator_text) + xoffset, stringBuffer, (player.DamageFromEnemy > 0?C_2_Red:C_0_White));// DFE
	if (player.resistMeleeMin == player.resistMeleeMax) {
		sprintf(stringBuffer, "%i", player.resistMeleeMin);
	}
	else {
		sprintf(stringBuffer, "%i-%i", player.resistMeleeMin, player.resistMeleeMax);
	}
	DrawText(separator + xoffset,137 + yoffset, separator+GetTextWidth(separator_text) + xoffset, separator_text, 0);// %DR
	DrawText(separator+ GetTextWidth(separator_text) + xoffset, 137 + yoffset, 300 + xoffset, stringBuffer, 0);// %DR
	int basic_AC_divider = 5;
	if (HasTrait(CurrentPlayerIndex, TraitId::SmallFrame)) {
		basic_AC_divider = 3;
	}
	sprintf(stringBuffer, "%i", player.ACFromItems 
		+ (/*HasTrait(CurrentPlayerIndex, TraitId::Kamikaze) || */HasTrait(CurrentPlayerIndex, TraitId::Pyromaniac) ? 0 : player.CurDexterity / basic_AC_divider)
		+ player.ACFromClass);
	DrawText(258 + xoffset, 162 + yoffset, 301 + xoffset, stringBuffer, GetColorNumber(player.ACFromItems));// AC
	int tohit_bonus = GameMode == GM_EASY ? 30 : 0;
	if (player.fullClassId == PFC_EXECUTIONER) {
		tohit_bonus += player.CharLevel;
	}
	if (GameMode == GM_CLASSIC) {
		sprintf(stringBuffer, "%i", player.CharLevel + player.CurDexterity / 2 + player.ItemsAddToHitPercents + 50 + tohit_bonus);
	}
	else {
		sprintf(stringBuffer, "%i", player.CharLevel + player.CurDexterity + player.ItemsAddToHitPercents + 30 + tohit_bonus);
	}
	DrawText(258 + xoffset, 187 + yoffset, 301 + xoffset, stringBuffer, GetColorNumber(player.ItemsAddToHitPercents));// To Hit

	// Повреждения
	int minDamage = 0;
	int maxDamage = 0;
	
	Item* throwable = findThrowableItem( CurrentPlayerIndex );
	if( throwable )// flask / trap damage calculation
	{
		// +damage removed. +damage% applied from all items
		minDamage = throwable->MinDamage + throwable->MinDamage * player.ItemsAddDamagePercents / 100;
		maxDamage = throwable->MaxDamage + throwable->MaxDamage * player.ItemsAddDamagePercents / 100;
		int elem = BaseMissiles[ThrowableMissile(throwable)].DamageType;
		minDamage += minDamage * player.elementPercent[ elem ] / 100 + player.elementMinDamage[ elem ];
		maxDamage += maxDamage * player.elementPercent[ elem ] / 100 + player.elementMaxDamage[ elem ];
		if( player.effectFlag[EA_RANDOM_DAMAGE] ){ // dread
			minDamage = 4 * minDamage / 5;
			maxDamage = 3 * maxDamage / 2;
		}

	}
	else
	{
		minDamage = player.ItemsAddDamage + player.BaseDamage + player.ItemsAddDamagePercents * player.MinDamageFromItem / 100 + player.MinDamageFromItem;
		maxDamage = player.ItemsAddDamage + player.BaseDamage + player.ItemsAddDamagePercents * player.MaxDamageFromItem / 100 + player.MaxDamageFromItem;
		if (player.gameChanger & BIT(GC_20_2X_DAMAGE) && MaxCountOfPlayersInGame == 1) {
			minDamage *= 2; 
			maxDamage *= 2; 
		}
		if( player.effectFlag[EA_RANDOM_DAMAGE] ){//dread
			minDamage = 8 * minDamage / 10;
			maxDamage = 15 * maxDamage / 10;
		}
		//if (player.affixFlag2 & AF_33_CHANCE_OF_DOUBLE_DAMAGE){//ruin
		//	minDamage = 59 * minDamage / 50;
		//	maxDamage = 59 * maxDamage / 50;
		//}
	}
	LimitToMin(minDamage, 1);
	LimitToMin(maxDamage, 1);
	//maxDamage = maxDamage < minDamage ? minDamage : maxDamage;
	if (maxDamage < minDamage) {
		maxDamage = minDamage;
	}

	if (minDamage == maxDamage){ sprintf(stringBuffer, "%i", minDamage); }
	else { sprintf(stringBuffer, "%i-%i", minDamage, maxDamage); }
	MinDmgFromChar = minDamage;
	MaxDmgFromChar = maxDamage;
	int scale = -1;	int minX = 242;	int maxX = 305;
	if( minDamage < 100 && maxDamage < 100 ){
		scale = 0;		minX = 258;		maxX = 301;
	}
	DrawTextWithScale(minX + xoffset, 211 + yoffset, maxX + xoffset, stringBuffer, GetColorNumber(player.ItemsAddDamagePercents), scale);

	sprintf(stringBuffer, "%i", player.resist[ET_4_ACID] ); // Acid resist
	DrawText(257 + xoffset, 236 + yoffset, 300 + xoffset, stringBuffer, player.resist[ET_4_ACID] > 0 ? C_1_Blue : C_0_White);
	sprintf(stringBuffer, "%i", player.resist[ET_3_ARCAN] );
	DrawText(257 + xoffset, 260 + yoffset, 300 + xoffset, stringBuffer, player.resist[ET_3_ARCAN] > 0 ? C_1_Blue : C_0_White);
	sprintf( stringBuffer, "%i", player.resist[ET_1_FIRE] );
	DrawText(257 + xoffset, 284 + yoffset, 300 + xoffset, stringBuffer, player.resist[ET_1_FIRE] > 0 ? C_1_Blue : C_0_White);
	sprintf(stringBuffer, "%i", player.resist[ET_6_COLD] ); // Cold resist
	DrawText(257 + xoffset, 308 + yoffset, 300 + xoffset, stringBuffer, player.resist[ET_6_COLD] > 0 ? C_1_Blue : C_0_White);
	sprintf( stringBuffer, "%i", player.resist[ ET_2_LIGHTNING ] );
	DrawText(257 + xoffset, 332 + yoffset, 300 + xoffset, stringBuffer, player.resist[ET_2_LIGHTNING] > 0 ? C_1_Blue : C_0_White);

	// Base attribute values
	const PlayerStat maxStat = GetPlayerMaxStat( CurrentPlayerIndex );
	sprintf(stringBuffer, "%i", player.BaseStrength);
	DrawText(95 + xoffset, 155 + yoffset, 126 + xoffset, stringBuffer, maxStat.Strength  == player.BaseStrength  || CannotIncreaseStat(CurrentPlayerIndex) == 1 ? C_3_Gold : C_0_White );
	sprintf(stringBuffer, "%i", player.BaseMagic );
	DrawText(95 + xoffset, 183 + yoffset, 126 + xoffset, stringBuffer, maxStat.Magic     == player.BaseMagic     || CannotIncreaseStat(CurrentPlayerIndex) == 1 ? C_3_Gold : C_0_White );
	sprintf(stringBuffer, "%i", player.BaseDexterity);
	DrawText(95 + xoffset, 211 + yoffset, 126 + xoffset, stringBuffer, maxStat.Dexterity == player.BaseDexterity || CannotIncreaseStat(CurrentPlayerIndex) == 1 ? C_3_Gold : C_0_White );
	sprintf(stringBuffer, "%i", player.BaseVitality);
	DrawText(95 + xoffset, 239 + yoffset, 126 + xoffset, stringBuffer, maxStat.Vitality  == player.BaseVitality  || CannotIncreaseStat(CurrentPlayerIndex) == 1 ? C_3_Gold : C_0_White );

	// Current attribute values
	sprintf(stringBuffer, "%i", player.CurStrength);
	DrawText(143 + xoffset, 155 + yoffset, 173 + xoffset, stringBuffer, GetColorNumber (player.CurStrength  - player.BaseStrength));
	sprintf(stringBuffer, "%i", player.CurMagic);
	DrawText(143 + xoffset, 183 + yoffset, 173 + xoffset, stringBuffer, GetColorNumber (player.CurMagic     - player.BaseMagic));
	sprintf(stringBuffer, "%i", player.CurDexterity);
	DrawText(143 + xoffset, 211 + yoffset, 173 + xoffset, stringBuffer, GetColorNumber (player.CurDexterity - player.BaseDexterity));
	sprintf(stringBuffer, "%i", player.CurVitality);
	DrawText(143 + xoffset, 239 + yoffset, 173 + xoffset, stringBuffer, GetColorNumber (player.CurVitality  - player.BaseVitality));

	// Количество лэвэл поинтов и картинки кнопок добавления
	if( player.AvailableLvlPoints > 0 && LevelPointsToMax(CurrentPlayerIndex) < player.AvailableLvlPoints ){
		player.AvailableLvlPoints = LevelPointsToMax(CurrentPlayerIndex);// Доступных лэвел поинтов никогда не будет больше количества которого не хватает до максимальной раскачки
	}
	if( player.AvailableLvlPoints > 0 ){
		sprintf(stringBuffer, "%i", player.AvailableLvlPoints);
		DrawText(95 + xoffset, 266 + yoffset, 126 + xoffset, stringBuffer, 2);// Пишем сколько очков есть
		// Рисуем плюсик везде где есть возможность добавить поинт
		if( player.BaseStrength  < maxStat.Strength  ) DrawPlusCEL(137 + xoffset, 159 + 28 * 0 + yoffset, AttributeUpAvail[0] + 2);
		if( player.BaseMagic     < maxStat.Magic     ) DrawPlusCEL(137 + xoffset, 159 + 28 * 1 + yoffset, AttributeUpAvail[1] + 4);
		if( player.BaseDexterity < maxStat.Dexterity ) DrawPlusCEL(137 + xoffset, 159 + 28 * 2 + yoffset, AttributeUpAvail[2] + 6);
		if( player.BaseVitality  < maxStat.Vitality  ) DrawPlusCEL(137 + xoffset, 159 + 28 * 3 + yoffset, AttributeUpAvail[3] + 8);
	}
	// life and mana
	int offsLeft = 0;
	int offsRight = 4;
	fontColor = player.MaxCurLife > player.MaxBaseLife;
	sprintf(stringBuffer, "%i", player.MaxCurLife >> 6);
	DrawText(95 + offsLeft + xoffset, 304 + yoffset, 126 + xoffset, stringBuffer, fontColor);// Пишем максимальное количество жизни
	if( player.CurLife != player.MaxCurLife ){
		fontColor = C_2_Red;
	}
	sprintf(stringBuffer, "%i", player.CurLife >> 6);
	DrawText(143 + offsRight + xoffset, 304 + yoffset, 174 + xoffset, stringBuffer, fontColor);// Пишем текущее количество жизни
	fontColor = player.MaxCurMana > player.MaxBaseMana;
	sprintf(stringBuffer, "%i", player.MaxCurMana >> 6);
	DrawText(95 + offsLeft + xoffset, 332 + yoffset, 126 + xoffset, stringBuffer, fontColor);// Пишем максимальное количество маны
	if( player.CurMana != player.MaxCurMana ){
		fontColor = C_2_Red;
	}
	sprintf(stringBuffer, "%i", player.CurMana >> 6);
	DrawText(143 + offsRight + xoffset, 332 + yoffset, 174 + xoffset, stringBuffer, fontColor);// Пишем текущее количество маны

	/*if (GameMode == GM_COLISEUM){ 
		sprintf(stringBuffer, "0"); 
	}
	else { */
		sprintf(stringBuffer, "%i", HealthRegen); 
	//}
	DrawText(95 + xoffset, 359 + yoffset, 126 + xoffset, stringBuffer, C_0_White);
	/*if (GameMode == GM_COLISEUM) { 
		sprintf(stringBuffer, "0"); 
	}
	else { */
		sprintf(stringBuffer, "%i", ManaRegen); 
	//}
	DrawText(143 + xoffset, 359 + yoffset, 178 + xoffset, stringBuffer, C_0_White);
	

	int critType = CR_MELEE;
	if (player.OnBodySlots[IS_RightHand].ItemCode != IC_M1_NONE && player.OnBodySlots[IS_RightHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_RightHand].IsReqMet
		&& is(player.OnBodySlots[IS_RightHand].ItemCode, IC_3_BOW, IC_21_KNIFE, IC_22_MALLET, IC_23_PISTOL, IC_19_TRAP)) {
		critType = CR_ARROW;
	}
	if (player.OnBodySlots[IS_LeftHand].ItemCode != IC_M1_NONE && player.OnBodySlots[IS_LeftHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_LeftHand].IsReqMet
		&& is(player.OnBodySlots[IS_LeftHand].ItemCode, IC_3_BOW, IC_21_KNIFE, IC_22_MALLET, IC_23_PISTOL, IC_19_TRAP)) {
		critType = CR_ARROW;
	}
	/*if (player.fullClassId == PFC_SCOUT || player.fullClassId == PFC_INQUISITOR) {
		critType = CR_ELEMENTAL;
	}*/
	else if ( /*(player.fullClassId == PFC_ASSASSIN && HasTrait(CurrentPlayerIndex, TraitId::Black_Witchery)) 
		||*/ player.fullClassId == PFC_BOMBARDIER || player.fullClassId == PFC_MAGE 
		|| (player.fullClassId == PFC_ELEMENTALIST && (!HasTrait(CurrentPlayerIndex, TraitId::Mamluk))) 
		|| player.fullClassId == PFC_WARLOCK/* || HasTrait(CurrentPlayerIndex, TraitId::Paladin)*/) {
		critType = CR_MAGIC;
	}
	
	BaseCrit bc = GetBaseCrits(critType, CurrentPlayerIndex);
	int critChance = bc.chance + (bc.levelDiv ? player.CharLevel / bc.levelDiv : 0) + player.critChance;
	int critBonusDamagePercent = bc.percent + player.critPercent;

	int damageMin2 = bc.damageMin + player.critDamageMin;
	int damageMax2 = bc.damageMax + player.critDamageMax;
	int minCritDmg = minDamage + (minDamage * critBonusDamagePercent / 100) + (player.fullClassId == PFC_TRAPPER ? 0 : damageMin2);
	int maxCritDmg = maxDamage + (maxDamage * critBonusDamagePercent / 100) + (player.fullClassId == PFC_TRAPPER ? 0 : damageMax2);

	int color = C_0_White;
	if (critChance > bc.chance) {
		color = C_1_Blue;
	}
	else if (critChance < bc.chance) {
		color = C_2_Red;
	}
	// ----special condition for classes (or traits) that have crits turned off. Displays crit chance as 0% and in red color---------------------
	if (player.fullClassId == PFC_SAVAGE || player.fullClassId == PFC_DEMONOLOGIST || player.fullClassId == PFC_NECROMANCER || player.fullClassId == PFC_BEASTMASTER
		|| HasTrait(CurrentPlayerIndex, TraitId::Tormentor) || HasTrait(CurrentPlayerIndex, TraitId::Toxic_at_Heart)) {
		color = C_2_Red;
		critChance = 0;
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------
	sprintf(stringBuffer, "%i%%", critChance);
	DrawText(95 + xoffset, 387 + yoffset, 126 + xoffset, stringBuffer, color);
	// Пишем максимальное количество маны
	//sprintf(stringBuffer, "%i-%i", minCritDmg,maxCritDmg);

    PrintDamageRangeToStringBuffer( minCritDmg, maxCritDmg );
	sprintf(stringBuffer, "%s", InfoPanelBuffer);
	int critxpos = 140;
	DrawTextWithScale(critxpos + xoffset, 387 + yoffset, critxpos + 70 + xoffset, stringBuffer, C_0_White, 0);

	sprintf(stringBuffer, "%i%%", player.goldFind+100);
	DrawText(93 + xoffset, 415 + yoffset, 126 + xoffset, stringBuffer, (player.goldFind == 0 ? C_0_White : (player.goldFind<0 ? C_2_Red : C_1_Blue)));

    PrintDamageRangeToStringBuffer( player.goldAbsFindMin, player.goldAbsFindMax );
	sprintf(stringBuffer, "%s", InfoPanelBuffer);
	int goldxpos = 140;
	DrawTextWithScale(goldxpos + xoffset, 415 + yoffset, goldxpos + 70 + xoffset, stringBuffer, C_0_White, 0);

	int xpGainPercent = player.xpGainPercent;
	sprintf(stringBuffer, "%i%%", xpGainPercent+100);
	DrawText(93 + xoffset, 443 + yoffset, 126 + xoffset, stringBuffer, (xpGainPercent == 0 ? C_0_White : (xpGainPercent<0? C_2_Red: C_1_Blue)));

    PrintDamageRangeToStringBuffer( player.xpGainMin, player.xpGainMax, true );
	sprintf(stringBuffer, "%s", InfoPanelBuffer);
	int expxpos = 140;
	DrawTextWithScale(expxpos + xoffset, 443 + yoffset, expxpos + 70 + xoffset, stringBuffer, C_0_White, 0);

	int magicFind = player.magicFind;
	sprintf(stringBuffer, "%i%%", magicFind);
	DrawText(257 + xoffset, 382 + yoffset, 300 + xoffset, stringBuffer, (player.magicFind == 0 ? C_0_White : (player.magicFind<0 ? C_2_Red : C_1_Blue)));
	
	//------------------------------------new------
	if (player.resistArrowMin == player.resistArrowMax) {
		sprintf(stringBuffer, "%i", player.resistArrowMin);
	}
	else {
		sprintf(stringBuffer, "%i-%i", player.resistArrowMin, player.resistArrowMax);
	}
	DrawTextWithScale(239 + xoffset, 357 + yoffset, 309 + xoffset, stringBuffer, C_0_White, 0);

	//-----------------------
	sprintf(stringBuffer, "%d", player.allSpellExtraLevel);
	DrawText(257 + xoffset, 407 + yoffset, 300 + xoffset, stringBuffer, (player.allSpellExtraLevel == 0 ? C_0_White : (player.allSpellExtraLevel<0 ? C_2_Red : C_1_Blue)));
	int kbchance = GetPlayerKnockbackResistChance( CurrentPlayerIndex );
	sprintf(stringBuffer, "%i%%", kbchance);
	int kb_color = kbchance == 100 ? C_3_Gold : C_0_White;
	if (HasTrait(CurrentPlayerIndex, TraitId::RatelHide)) {
		kb_color = C_2_Red;
	}
	else if (PerkValue(PERK_STONEWALL, CurrentPlayerIndex) >= 1 || PerkValue(SYNERGY_STOICISM, CurrentPlayerIndex) >= 1 || PerkValue(SYNERGY_ANCHOR, CurrentPlayerIndex) >= 1) {
		kb_color = C_1_Blue;
	}
	DrawText(257 + xoffset, 432 + yoffset, 300 + xoffset, stringBuffer, kb_color);
}

bool CursorIntoStatUpButton( int buttonIndex )
{
	int buttonLeft   = StatUpButtons[ buttonIndex ].X + CharPanelRect.Left;
	int buttonUp     = StatUpButtons[ buttonIndex ].Y + CharPanelRect.Top;
	int buttonWidth  = StatUpButtons[ buttonIndex ].Width;
	int buttonHeight = StatUpButtons[ buttonIndex ].Height;
	int buttonRight  = buttonLeft + buttonWidth;
	int buttonDown   = buttonUp + buttonHeight;

	if( CursorX >= buttonLeft && CursorX <= buttonRight && CursorY >= buttonUp && CursorY <= buttonDown ){
		return 1;
	}else{
		return 0;
	}
}

//----- (00406808) -------------------------------------------------------- interface
void CharPanel_MouseDown()
{
	Player& player = Players[CurrentPlayerIndex];
	if( ButtonStatDown || !player.AvailableLvlPoints ){
		return;
	}
	const PlayerStat maxStat = GetPlayerMaxStat( CurrentPlayerIndex );
	for( int curButton = 0; curButton < 4; curButton++ ){
		bool hasButton;
		switch( curButton ){
		case 0:	hasButton = player.BaseStrength < maxStat.Strength;		break;
		case 1:	hasButton = player.BaseMagic	< maxStat.Magic;		break;
		case 2:	hasButton = player.BaseDexterity< maxStat.Dexterity;	break;
		case 3:	hasButton = player.BaseVitality < maxStat.Vitality;		break;
		}
		if( hasButton && CursorIntoStatUpButton (curButton) ){
			AttributeUpAvail[curButton] = 1;
			ButtonStatDown = 1;
			PlayGlobalSound(75);
			break;
		}
	}
}

//----- (004068FF) -------------------------------------------------------- interface
void CharPanel_MouseClick()
{
	ButtonStatDown = 0;
	for( int currentButton = 0; currentButton < 4; currentButton++){
		if( AttributeUpAvail[currentButton] && CursorIntoStatUpButton (currentButton) ){
            int pointsToSpend = 1;
			#if CHEATS
			if( IsShiftPressed ) pointsToSpend = 5;
			#endif
			LimitToMax(pointsToSpend, Players[CurrentPlayerIndex].AvailableLvlPoints);
			NetSendCmdParam1( 1, CMD_3_ADDSTR + currentButton, pointsToSpend );
			Players[CurrentPlayerIndex].AvailableLvlPoints -= pointsToSpend;
		}
		AttributeUpAvail[currentButton] = 0;
	}
}
