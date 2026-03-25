#include "stdafx.h"

volatile int forceCallforSync;

ItemGet ItemGets[MaxItems_255];
int ItemGetCount = 0;

//----- (th2) -------------------------------------------------------------
bool __fastcall IsItemFiltered(Player& player, Item& tempItem) {
	return (!is(player.fullClassId, PFC_DEMONOLOGIST, PFC_BEASTMASTER, PFC_NECROMANCER)
		&& (tempItem.MagicCode == MC_24_BOOKS)
		&& is(tempItem.SpellIndex, PS_52_LESSER_SUMMON, PS_53_COMMON_SUMMON, PS_54_GREATER_SUMMON)
		// && MaxCountOfPlayersInGame == 1 // MP smart drop
		);/* {// mor: tried to remove certain books in Coliseum but failed
		return true;
	}
	else if (GameMode == GM_COLISEUM && tempItem.MagicCode == MC_24_BOOKS
		&& (is(tempItem.SpellIndex,
			PS_2_HEALING, PS_7_TOWN_PORTAL, PS_13_HYDRA, PS_15_FORCE_WAVE, PS_21_GOLEM, PS_42_WARP, PS_9_INFRAVISION, PS_5_IDENTIFY,
			PS_31_HOLY_BOLT, PS_33_TELEKINES, PS_39_HOLY_NOVA, PS_8_STONE_CURSE, PS_16_REFLECT, PS_24_APOCALYPSE, PS_28_TRAP_DISARM, PS_37_MANA_RECHARGE, PS_38_MAGI))) {
		return false;
	}
	else {
		return false;
	}*/
}

//----- (th2) -------------------------------------------------------------
Item* __fastcall FindItemInInventoryBySeed(int playerIndex, int seed, int baseItemIndex, int* itemIndex = nullptr)
{
	Player& player = Players[playerIndex];
	for( int i = 0; i < player.InvItemCount; ++i ){
		Item& item = player.InventorySlots[i];
		if( item.ItemCode != IC_M1_NONE && item.seed == seed && item.baseItemIndex == baseItemIndex ){
		    if( itemIndex ){
		        *itemIndex = i;
            }
			return &item;
		}
	}
	return nullptr;
}

//----- (th2) -------------------------------------------------------------
Item* __fastcall FindItemInBeltBySeed(int playerIndex, int seed, int baseItemIndex, int* itemIndex = nullptr)
{
	Player& player = Players[playerIndex];
	for( int i = 0; i < IS_8_Belt_Count; ++i ){
		Item& item = player.BeltInventory[i];
		if( item.ItemCode != IC_M1_NONE && item.seed == seed && item.baseItemIndex == baseItemIndex ){
		    if( itemIndex ){
		        *itemIndex = i;
            }
			return &item;
		}
	}
	return nullptr;
}

//----- (th2) -------------------------------------------------------------
Item* __fastcall FindItemInInventoryOrBeltBySeed(int playerIndex, int seed, int baseItemIndex)
{
    Item* item = nullptr;
    item = FindItemInInventoryBySeed(playerIndex, seed, baseItemIndex);
    if(!item) item = FindItemInBeltBySeed(playerIndex, seed, baseItemIndex);
    return item;
}

//----- (th2) -------------------------------------------------------------
void ClearPlayerItemBySeed(int playerIndex, int seed, int baseItemIndex )
{
	if( seed == 0 || baseItemIndex == -1 ) { return; } // this is supposed to fix quest items disappearing from inventory
	Item* item = nullptr;
    int index = 0;
    if( FindItemInInventoryBySeed(playerIndex, seed, baseItemIndex, &index) ){
        ClearInventorySlot(playerIndex, index);
        return;
    }
    if( FindItemInBeltBySeed(playerIndex, seed, baseItemIndex, &index) ){
        ClearBeltSlot(playerIndex, index);
        return;
    }
}

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           jmp inline chunks                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

//----- (th2) -------------------------------------------------------------
bool UseFullHealingPotion(int playerIndex)
{
	Player& player = Players[playerIndex];
	if( player.CurLife != player.MaxCurLife ){
		if (HasTrait(playerIndex, TraitId::Giant)) {
			int lifeAdd = player.MaxCurLife / 2;
			player.CurLife += lifeAdd;
			player.BaseLife += lifeAdd;
			if (player.CurLife > player.MaxCurLife) {
				player.CurLife = player.MaxCurLife;
				player.BaseLife = player.MaxBaseLife;
			}
		}
		else {
			player.CurLife = player.MaxCurLife;
			player.BaseLife = player.MaxBaseLife;
		}
        return true;
    }else{
        return false;
    }
}

//----- (th2) -------------------------------------------------------------
bool UseHealingPotion(int playerIndex)
{
	Player& player = Players[playerIndex];
	if( player.CurLife != player.MaxCurLife ){
        int lifeAdd = RNG(player.MaxCurLife >> 8);
        int maxLife = (player.MaxCurLife >> 8) & ~1/*Зачем обнуление 1го бита?*/; // Возможно это получение модуля числа ?
        lifeAdd = VariatePotionsByDifficulty( lifeAdd, maxLife );
        if( (player.ClassID == PC_2_MAGE && (!HasTrait(playerIndex, TraitId::Mamluk))) || HasTrait(playerIndex, TraitId::Giant) ){
            lifeAdd /= 2;
        }
        player.CurLife += lifeAdd;
        player.BaseLife += lifeAdd;
        if( player.CurLife > player.MaxCurLife ){
            player.CurLife = player.MaxCurLife;
            player.BaseLife = player.MaxBaseLife;
        }
        return true;
    }else{
        return false;
    }
}

//----- (th2) -------------------------------------------------------------
bool UseFullManaPotion(int playerIndex)
{
	Player& player = Players[playerIndex];
	if( player.CurMana != player.MaxCurMana ){
		if (HasTrait(playerIndex, TraitId::Giant)) {
			int manaAdd = player.MaxCurMana / 2;
			player.CurMana += manaAdd;
			player.BaseMana += manaAdd;
			if (player.CurMana > player.MaxCurMana) {
				player.CurMana = player.MaxCurMana;
				player.BaseMana = player.MaxBaseMana;
			}
		}
		else {
			player.CurMana = player.MaxCurMana;
			player.BaseMana = player.MaxBaseMana;
		}
        return true;
    }else{
        return false;
    }
}

//----- (th2) -------------------------------------------------------------
bool UseManaPotion(int playerIndex)
{
	Player& player = Players[playerIndex];
	if( player.CurMana != player.MaxCurMana ){
        int manaAdd = RNG(player.MaxCurMana >> 8);
        int maxMana = (player.MaxCurMana >> 8) & ~1;
        manaAdd = VariatePotionsByDifficulty( manaAdd, maxMana ); 
        if( is( player.fullClassId, PFC_EXECUTIONER, PFC_SHUGOKI, PFC_SAVAGE ) ){
            manaAdd /= 2;
        }
		if (HasTrait(playerIndex, TraitId::Giant)) {
			manaAdd /= 2;
		}
        player.CurMana += manaAdd;
        player.BaseMana += manaAdd;
        if( player.CurMana > player.MaxCurMana ){
            player.CurMana = player.MaxCurMana;
        }
        if( player.BaseMana > player.MaxBaseMana ){
            player.BaseMana = player.MaxBaseMana;
        }
        return true;
    }else{
        return false;
    }
}

//----- (th2) -------------------------------------------------------------
void UseItemWithMagicCode_47_51_rune(int playerIndex, int magicCode, int seed, int baseItemIndex)
{
	Player& player = Players[playerIndex];
	player.SpellOnCursor = magicCode;// У рун маджик код совпадает с player_cast кодом
	player.spellOnCursorType = SO_4_NONE;
	if( playerIndex == CurrentPlayerIndex ){
		Cur.UsedItemSeed = seed; Cur.UsedItemBase = baseItemIndex;
		SetCursorGraphics(CM_9_TARGET);
	}
}

//----- (th2) -------------------------------------------------------------
bool UseItemWithMagicCode_21_22_relic(int playerIndex, int magicCode, int spellID, int seed, int baseItemIndex)
{
	bool used = false;
	Player& player = Players[playerIndex];
	if( ! Spells[spellID].targeted ){
		ClearPlayerPath(playerIndex);
		player.spellIndex = spellID;
		player.readySpellType = SO_4_NONE;
		player.readySpellTargetType = SO_3_EQUIPED_ITEM;
		player.destAction = PCMD_12_CAST_SPELL_TO_NONE;
		PlayersSpellcastQueue[playerIndex] = {}; // clear the queue upon relic use (relict do not queue with any other spell)
		if (player.CurAction == PCA_9_SPELL) { // stop auto recast (queued spell relict abuse fix)
			player.CurAction = PCA_0_STAND;  // relict has the top priotity to cast
		}
		short playerSumSpellLevel = PlayerSpellLevel( playerIndex, player.spellIndex );
        NetSendCmdLocParam2( 1, CMD_15_TSPELLXY, Cur.Row, Cur.Col, player.spellIndex, playerSumSpellLevel );
		player.destParam1 = Cur.Row;
		player.destParam2 = Cur.Col;
		used = true;
	}else{
		player.SpellOnCursor = spellID;
		player.spellOnCursorType = SO_4_NONE;
		if( playerIndex == CurrentPlayerIndex ){
			SetCursorGraphics(CM_9_TARGET);
		}
	}
	if( playerIndex == CurrentPlayerIndex ){
		Cur.UsedItemSeed = seed; Cur.UsedItemBase = baseItemIndex;
	}
	return used;
}

//----- (th2) -------------------------------------------------------------
void UseItemWithMagicCode_24_book(int playerIndex, int magicCode, int spellID)
{
	Player& player = Players[playerIndex];
	player.AvailableSpellMask |= 1i64 << (spellID - 1);
	if( player.SpellLevels[spellID] < SpellLevelBookMax() ){
		player.SpellLevels[spellID]++;
	}
	int manaAdd = Spells[spellID].InitialManaToCast << 6;
	if( player.effectFlag[EA_LOSE_REGENERATION] ) return;
	player.CurMana += manaAdd;
	player.BaseMana += manaAdd;
	if( player.CurMana > player.MaxCurMana ){
		player.CurMana = player.MaxCurMana;
		player.BaseMana = player.MaxBaseMana;
	}
	if( playerIndex == CurrentPlayerIndex ){
		RecalcBooksReq(playerIndex);
	}
}

//----- (th2) -------------------------------------------------------------
void UseItemWithMagicCode_31_40_oil(int playerIndex, int magicCode, int seed, int baseItemIndex)
{
	Player& player = Players[playerIndex];
	if( playerIndex != CurrentPlayerIndex ){
		return;
	}
	player.OilEffectOnCursor = magicCode;
	Cur.UsedItemSeed = seed; Cur.UsedItemBase = baseItemIndex;
    OpenInventoryPanel();
	SetCursorGraphics(CM_6_OIL);
}

//----- (th2) -------------------------------------------------------------
void UseIdentifyRelic(int playerIndex, int magicCode, int seed, int baseItemIndex)
{
	Player& player = Players[playerIndex];
	if( playerIndex != CurrentPlayerIndex ){
		return;
	}
	player.OilEffectOnCursor = magicCode;
	Cur.UsedItemSeed = seed; Cur.UsedItemBase = baseItemIndex;
	Cur.IsTradeService = false;
	OpenInventoryPanel();
	SetCursorGraphics(CM_2_IDENTIFY);
	PlayLocalSound(Spells[PS_5_IDENTIFY].SoundType, player.Row, player.Col);
}

//----- (th2) -------------------------------------------------------------
void UseItemWithMagicCode_42_caphedralMap(int playerIndex, int magicCode, int spellID)
{
	OpenPopupScreen();
}

//----- (th2) -------------------------------------------------------------
void UseItemWithMagicCode_44_spectralElixir(int playerIndex, int magicCode, int spellID)
{/*
	int stat_val = 1;
	if (GameMode == GM_CLASSIC) stat_val = 3;*/
	ModifyPlayerStrength(playerIndex, 1);
	ModifyPlayerMagic(playerIndex, 1);
	ModifyPlayerDexterity(playerIndex, 1);
	ModifyPlayerVitality(playerIndex, 1);
	UseFullHealingPotion(playerIndex);
	UseFullManaPotion(playerIndex);
}

//----- (th2) -------------------------------------------------------------
void UseItemWithMagicCodeNull(int playerIndex, int magicCode, int spellID)
{
	return;
}

//----- (th2) -------------------------------------------------------------
bool FullCreateAndPutItem(Item& item, int baseItemIndex)
{
	if( baseItemIndex != -1 ){
		SetPlayerHandItem(item, baseItemIndex);
		item.MakeSeed();
		return true;
	}
	return false;
}

//----- (th2) -------------------------------------------------------------
void PutStartItemToInventory(int playerIndex, int baseItemIndex)
{
	Player& player = Players[playerIndex];
	if( FullCreateAndPutItem(player.ItemOnCursor, baseItemIndex) ){
		AutoPutCursorToInventoryOrDrop();
	}
}

//----- (th2) -------------------------------------------------------------
void findFullClassIdByMask( uint requiredClassMask, PLAYER_FULL_CLASS result[], size_t resultBufferSize, size_t* aFoundClasses )
{
    size_t foundClasses = 0u;

	bitset<32> classReqBitset( requiredClassMask );
    
    for( size_t i = 0; i < PFC_COUNT && foundClasses < resultBufferSize; ++i )
    {
        if( classReqBitset.test( i ) )
        {
            result[ foundClasses++ ] = static_cast<PLAYER_FULL_CLASS>( i );
        }
    }
    
    if( aFoundClasses )
    {
        *aFoundClasses = foundClasses;
    }
}

//----- (th2) -------------------------------------------------------------
bool findClassGroupsByMask( uint requiredClassMask, PLAYER_CLASS_GROUPS result[], size_t resultBufferSize, size_t* aFoundClassGroups )
{
    size_t foundClassGroups = 0;
	uint checkMask = 0;
 
	auto checkAndAddClassGroup = [&](ITEM_PLAYER_CLASS_MASK _MASK_, PLAYER_CLASS_GROUPS _GROUP_){
		if( (requiredClassMask & (_MASK_)) == (_MASK_) ){
			if( foundClassGroups < resultBufferSize ){
				result[ foundClassGroups++ ] = (_GROUP_);
				checkMask |= (_MASK_);
			}
			return true;
		}
		return false;
	};
	
	checkAndAddClassGroup( IPCM_ANY_WARRIOR, PCG_WARRIORS );
	checkAndAddClassGroup( IPCM_ANY_ARCHER, PCG_ARCHERS );
	
	checkAndAddClassGroup( IPCM_ANY_MAGE, PCG_MAGES )
 	|| checkAndAddClassGroup( IPCM_ANY_SUMMONER, PCG_SUMMONERS );
	
	checkAndAddClassGroup( IPCM_ANY_MONK, PCG_MONKS );
	checkAndAddClassGroup( IPCM_ANY_ROGUE, PCG_ROGUES );
	
	checkAndAddClassGroup( IPCM_ANY_SAVAGE, PCG_SAVAGES )
	|| checkAndAddClassGroup( IPCM_ANY_GLADIATOR, PCG_GLADIATORS );

    if( aFoundClassGroups ) *aFoundClassGroups = foundClassGroups;
    
    return checkMask == requiredClassMask;
}

//----- (th2) -------------------------------------------------------------
void printItemClassReqInfoInBuffer( const Item* aItem )
{
	const BaseItem& baseItem = BaseItems[aItem->baseItemIndex];
    uint requiredClassMask = GetRequiredClassMask( aItem );
    if( requiredClassMask == IPCM_ALL_CLASSES ){
        strcpy(InfoPanelBuffer, "[No class req]");
    }
	else if( requiredClassMask == 0 ){
		strcpy(InfoPanelBuffer, "[No one can wear it]");
	}
	else{
		bitset<32> classReqBitset( requiredClassMask );
        
        size_t allowedClassesCount = classReqBitset.count();
        
        if( allowedClassesCount < 16u ){
            if( allowedClassesCount <= 2u ){
                // Concrete specialization or two
				PLAYER_FULL_CLASS classes[2]{};
                findFullClassIdByMask( requiredClassMask, classes, 2, nullptr );
            
                if( allowedClassesCount == 1u ){
					if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
						sprintf(InfoPanelBuffer, "[Mamluk only]");
					}
					else if (Players[CurrentPlayerIndex].fullClassId == PFC_ELEMENTALIST && !HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
						sprintf(InfoPanelBuffer, "[Elementalist only]");
					}
					else if (HasTrait(CurrentPlayerIndex, TraitId::Pistoleer) && baseItem.ItemCode == IC_23_PISTOL) {
						sprintf(InfoPanelBuffer, "[Pistoleer only]");
					}
					else if (HasTrait(CurrentPlayerIndex, TraitId::Ranger) && baseItem.ItemCode == IC_3_BOW) {
						sprintf(InfoPanelBuffer, "[Ranger only]");
					}
					else if (HasTrait(CurrentPlayerIndex, TraitId::Barbarian) && baseItem.ItemCode == IC_7_HELM) {
						sprintf(InfoPanelBuffer, "[Barbarian only]");
					}
					else {
						sprintf(InfoPanelBuffer, "[%s only]", FullPlayerClasses[classes[0]].name);
					}
                }
				else{
					if (HasTrait(CurrentPlayerIndex, TraitId::Barbarian) && baseItem.ItemCode == IC_7_HELM) {
						sprintf(InfoPanelBuffer, "[Barbarian only]");
					}
					else {
						sprintf(InfoPanelBuffer, "[%s and %s only]", FullPlayerClasses[classes[0]].name, FullPlayerClasses[classes[1]].name);
					}
                }
            }
			else{
            	size_t classGroupsFound = 0u;
				PLAYER_CLASS_GROUPS classGroups[2]{};
				bool isOk = findClassGroupsByMask( requiredClassMask, classGroups, 2, &classGroupsFound );

				if( !isOk || classGroupsFound == 0u ){
				   strcpy(InfoPanelBuffer, "[Available to multiple classes]");
				}
				else{
					if (HasTrait(CurrentPlayerIndex, TraitId::Barbarian) && baseItem.ItemCode == IC_7_HELM) {
						sprintf(InfoPanelBuffer, "[Barbarian only]");
					}
					else {
						if (classGroupsFound == 1u) {
							sprintf(InfoPanelBuffer, "[%s only]", ClassGroupsNames[classGroups[0]]);
						}
						else {
							sprintf(InfoPanelBuffer, "[%s and %s only]", ClassGroupsNames[classGroups[0]], ClassGroupsNames[classGroups[1]]);
						}
					}
				}
            }
        }else{
            size_t disAllowedClassesCount = 32u - allowedClassesCount;
            requiredClassMask = ~requiredClassMask;

            if( disAllowedClassesCount <= 2u ){
                // Concrete specialization or two
				PLAYER_FULL_CLASS classes[2]{};
                findFullClassIdByMask( requiredClassMask, classes, 2, nullptr );

                if( disAllowedClassesCount == 1u ){
                    sprintf(InfoPanelBuffer, "[for all but %s]", FullPlayerClasses[classes[0]].name);
				}else{
					sprintf(InfoPanelBuffer, "[for all but %s and %s]", FullPlayerClasses[classes[0]].name, FullPlayerClasses[classes[1]].name);
                }
            }else{
				size_t classGroupsFound = 0;
				PLAYER_CLASS_GROUPS classGroups[2]{};
				bool isOk = findClassGroupsByMask( requiredClassMask, classGroups, 2, &classGroupsFound );
	
				if( !isOk || classGroupsFound == 0u ){
					strcpy(InfoPanelBuffer, "[Available to multiple classes]");
				}else{
					if( classGroupsFound == 1u ){
						sprintf(InfoPanelBuffer, "[for all but %s]", ClassGroupsNames[ classGroups[0] ]);
					}else{
						sprintf(InfoPanelBuffer, "[for all but %s and %s]", ClassGroupsNames[ classGroups[0] ], ClassGroupsNames[ classGroups[1] ]);
					}
				}
            }
        }
	}
}

//----- (th2) -------------------------------------------------------------
void printItemClassReqInfo( const Item* aItem )
{
	uint requiredClassMask = GetRequiredClassMask( aItem );
	if( requiredClassMask != IPCM_ALL_CLASSES )
	{
	    printItemClassReqInfoInBuffer(aItem);
	    InfoPanel_AddLine( InfoPanelBuffer, true );
	}
	
	const Player& player = Players[CurrentPlayerIndex];
	const BaseItem& baseItem = BaseItems[aItem->baseItemIndex];
	uint playerFullClassMask = (1u << static_cast<uint>( player.fullClassId ) );
	if( (playerFullClassMask & IPCM_ANY_EXILE) != 0 && baseItem.ForbiddenForExiles )
	{
		InfoPanel_AddLine( "[Can't wear item]", true );
	}
}

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           end of inline	                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

// функция для опциональной подмены графики бутылок на оригинальные
//----- (th2) -------------------------------------------------------------
char* GetItemSprites( int& spriteId )
{
	extern char UserPotion[64];
	if( UserPotionB || GameMode == GM_CLASSIC){
		switch( spriteId - 12 ){
		case 314 + 168: spriteId = 32 + 12; return ClassicItemSprites; // potion of healing
		case 496 + 168: spriteId = 39 + 12; return ClassicItemSprites; // potion of mana
		case 449 + 168: spriteId = 35 + 12; return ClassicItemSprites; // potion of full healing
		case 497 + 168: spriteId = 0  + 12; return ClassicItemSprites; // potion of full mana
		case  37:		spriteId = 36 + 12; return ClassicModItemSprites; // elixir of dexterity
		case 179 + 168: spriteId = 241 + 168 + 12 - 179; return AddonModItemSprites; // relict of mana
		}
	}
	if( spriteId <= 179){ // выбор файла для отрисовки графики вещи
		return GameMode == GM_CLASSIC ? ClassicItemSprites : ClassicModItemSprites;
	}else if( spriteId <= 3896 ){
		spriteId -= 179;
		return AddonModItemSprites;
	}else{
		spriteId -= 3896;
		return Th3ItemSprites;
	}
}

#ifdef FLIP_TEST
//----- (th2) -------------------------------------------------------------
char testStr[512];
char* test()
{
	testStr[0] = 0;
	char buf[16];
	char prev[32]; prev[0] = 0;
	char next[32]; next[0] = 0;
	for( auto& i : BaseItems ){
		if( is( i.ItemCode, IC_4_MACE ) /*&& (!i.Name2 || !strcmp(i.Name2, "Armor" )*/ ) ){
			//if( i.Name2 ){
			//	strcat( next, i.Name2 );
			//	strcat( next, " " );
			//}
			strcat( next, itoa( ItemFlipTable[ i.GraphicValue ], buf, 10 ) );
			strcat( next, ", " );
			if( strcmp( next, prev ) ){
				strcpy( prev, next );
			}else{
				next[ 0 ] = 0;
				continue;
			}
			strcat( testStr, next );
			next[0] = 0;
		}
	}
	return testStr;
}
#endif

// функция для определения анимации предмета по данным базовых предметов и опциональной подмены графики бутылок на оригинальные
//----- (th2) -------------------------------------------------------------
void SetFlipTable()
{
	for( size_t I = 0; I < count_BaseItems; ++I ){
		auto& i = BaseItems[I];
		auto& f = ItemFlipTable[i.GraphicValue];
		switch( i.ItemCode ){
		case IC_0_OTHER      :
			if( strstr( i.NamePtr, "ushroom" ) ) f = FL_43_MUSHROOM;
			else{
				switch( i.MagicCode ){
					// case MC_21_RELIC_NEED_NO_TARGET or MC_22_RELIC_NEED_TARGET: f = FL_32_BLDSTN; break;
					case MC_55_FLASK_OF_FIRE:
					case MC_56_FLASK_OF_LIGHTNING:
					case MC_57_FLASK_OF_ACID:
					case MC_58_FLASK_OF_FIREBOMB:
					case MC_59_FLASK_OF_LIGHTNINGBOMB:
					case MC_60_FLASK_OF_ACIDBOMB:
					case MC_61_FLASK_OF_ARCANEBOMB:
						if( !f ) f = FL_26_FBTTLEWH;
						break;
				}
			}
			break;
		//case IC_1_SWORD	     : f = FL_8_SWORD; break;
		case IC_2_AXE	     : f = *(int*)i.NamePtr == 'aelC' ? FL_10_CLEAVER : FL_1_AXE; break;	 
		case IC_3_BOW	     : f = FL_3_BOW; break;
		//case IC_4_MACE	     : f = i.EquippedLocation == EL_1_ONE_HANDED ? (i.GraphicValue, 2819+168 ? FL_65_LITECLUB : FL_6_MACE) : FL_51_MAUL; break;
		//case IC_5_SHIELD     : f = i.Material ? FL_7_SHIELD : FL_15_WSHIELD; break; //is( *(int*)i.NamePtr, 'kcuB', 'graT', 'dooW' ) ? FL_15_WSHIELD : FL_7_SHIELD; break;
		case IC_6_LIGHT_ARMOR: f = i.Material ? FL_14_LARMOR : FL_42_CLOTH; break; //is( *(int*)i.NamePtr, 'epaC', 'sgaR', 'oalC', 'ddaP', 'inuT', 'cihT', 'aziW', 'lraW', 'liuQ', 'dnaW', 'csiD' ) ? FL_42_CLOTH : FL_14_LARMOR; break;
		case IC_8_MID_ARMOR  : f = FL_0_ARMOR2; break;
		case IC_9_HEAVY_ARMOR: f = FL_17_FPLATEAR; break;
		//case IC_7_HELM       : f = i.Material == 0 ? FL_31_CP : ( i.Material == 1 ? FL_5_HELMUT : FL_13_CROWNF ); break; //( i.Name2 && is( *(int*)i.Name2, 'worC', 'criC', 'raiT') ) ? FL_13_CROWNF : ( *(int*)i.NamePtr == 'taeL' ? FL_31_CP : FL_5_HELMUT ); break;
		//case IC_10_STAFF     : f = ( i.Name2 && *(int*)i.Name2 == 'dnaW' || 'doR' ) ? FL_36_WAND : FL_11_STAFF; break;
		//case IC_11_GOLD      : break;
		case IC_12_RING      :
		case IC_13_AMULET    : f = FL_12_RING; break;
		case IC_15_GLOVE     : f = FL_45_GLOVES + i.Material; break;
		//case IC_16_BOOTS     : f = FL_48_BOOTS  + i.Material; break;
		case IC_17_BELT      : f = FL_44_BELT; break;
		//case IC_20_CLAW		 : f = FL_8_SWORD; break; // will be used if need be.
		}
	}
	extern char UserPotion[ 64 ];
	if( UserPotionB || GameMode == GM_CLASSIC){
		ItemFlipTable[ 664 ] = FL_20_FBTTLEBB; // potion of mana
		ItemFlipTable[ 665 ] = FL_20_FBTTLEBB; // potion of full mana
		ItemFlipTable[  37 ] = FL_24_FBTTLEBL; // elixir of dexterity
		ItemFlipTable[ 179 + 168 ] = FL_56_RELICTBB; // relict of mana
	}
}

//----- (0041E584) -------------------------------------------------------- interface
void FreeInvPanelCEL()
{
	FreeMemZero(InvPanelCEL);

	FreeMemZero(VisualStashPanelCEL);
	FreeMemZero(VisualStashButtCEL);
	//FreeMemZero(VisualStashButtScrlCEL);
	FreeMemZero(VisualStashButtLeftCEL);
	FreeMemZero(VisualStashButtRightCEL);
	FreeMemZero(VisualTradePanelCEL);
	FreeMemZero(VisualTradeTabsCEL);
	FreeMemZero(VisualTradeBtnsCEL);
	FreeMemZero(CraftPanelCEL);
	FreeMemZero(CraftTabsCEL);
    FreeMemZero(CraftPanel2CEL);
    FreeMemZero(CraftSocketsLockCEL);
    FreeMemZero(CraftSocketCEL);
	FreeMemZero(EnchantPanelCEL);
	FreeMemZero(TransmutePanelCEL);
	//FreeMemZero(QuenchPanelCEL);
	FreeMemZero(LongButtonCEL);
	FreeMemZero(WpnSwapBtnImg);
}

//----- (0041E596) --------------------------------------------------------
void LoadPanelsGraphic()
{
	char* invPanelName;
	switch( Players[CurrentPlayerIndex].ClassID ){
	case PC_2_MAGE   : invPanelName = GameMode == GM_CLASSIC ? "Data\\Inv\\thclassic_inv_sor.cel" : "Data\\Inv\\inv3x1.cel"; break;
	case PC_3_MONK   : invPanelName = "Data\\Inv\\inv4x1.cel"; break;
	case PC_1_ARCHER :
	case PC_4_ROGUE  : invPanelName = GameMode == GM_CLASSIC ? "Data\\Inv\\thclassic_inv_rog.cel" : "Data\\Inv\\inv2x1.cel"; break;
	case PC_0_WARRIOR:
	case PC_5_SAVAGE : invPanelName = GameMode == GM_CLASSIC ? "Data\\Inv\\thclassic_inv_war.cel" : "Data\\Inv\\inv1x1.cel"; break;
	default: TerminateWithError("Wrong class id");
	}
	if( HasTrait(CurrentPlayerIndex, TraitId::Mamluk)         ) invPanelName = "Data\\Inv\\inv4x1.cel";
	if( HasTrait(CurrentPlayerIndex, TraitId::LordOfTheRings) ) invPanelName = "Data\\Inv\\inv4x2-LOTR.cel"; // TODO: change to draw additional slot programmaticaly
	InvPanelCEL = (char*)LoadFile(invPanelName);

	WpnSwapBtnImg            = (char*)LoadFile("data\\inv\\wpn_btns2.cel");
	VisualStashPanelCEL      = (char*)LoadFile("data\\stash_21\\stash_panel2.cel"); // was "data\\stash8.cel"
	VisualStashButtCEL       = (char*)LoadFile("data\\stash_21\\stash_tab.cel"); // was "data\\butt.cel"
	//VisualStashButtScrlCEL = (char*)LoadFile("data\\stashbtnscr.cel");
	VisualStashButtLeftCEL   = (char*)LoadFile("data\\stash_21\\stash_arrow_left.cel");
	VisualStashButtRightCEL  = (char*)LoadFile("data\\stash_21\\stash_arrow_right.cel");
	VisualTradePanelCEL      = (char*)LoadFile("data\\tradeWindow02.cel");
	VisualTradeTabsCEL       = (char*)LoadFile("data\\tradeWindowTabs.cel");
	VisualTradeBtnsCEL       = (char*)LoadFile("data\\stash_21\\tradewindowbutn.cel");
	CraftPanelCEL            = (char*)LoadFile("data\\aio_v2.cel");
	CraftTabsCEL             = (char*)LoadFile("data\\tradeWindowTabs.cel");
	//CraftBtnsCEL           = (char*)LoadFile("data\\tradewindowbutn.cel");
	//CraftBtnsCEL           = (char*)LoadFile("data\\stash_21\\tradewindowbutn.cel");
	CraftPanel2CEL			 = (char*)LoadFile("data\\craft\\socket_panel_v7.cel");// th2_item_craft_v6.cel");
	CraftSocketsLockCEL      = (char*)LoadFile("data\\craft\\ic_b.cel");
	CraftSocketCEL           = (char*)LoadFile("data\\craft\\sockets.cel");
	EnchantPanelCEL          = (char*)LoadFile("data\\enchant\\EnchantPanel.cel");
	LongButtonCEL            = (char*)LoadFile("data\\enchant\\longbutton.cel");
	TransmutePanelCEL        = (char*)LoadFile("data\\transmute\\tmpanel_v2.cel");
	//QuenchPanelCEL         = (char*)LoadFile("data\\enchant\\EnchantPanel.cel");
	IsINVPanelVisible   = false;
    IsPerksPanelVisible = false;
	IsInfoWindowVisible = false;
}

//----- (0041E5F8) --------------------------------------------------------
void __fastcall ShadowItemSlot( int leftPosition, int downPosition, ushort picWidth, ushort picHeight, uint8_t mark )
{
	uchar* dstPtr = (uchar*)&WorkingSurface[leftPosition] + YOffsetHashTable[downPosition];
	uint8_t curColor = mark ? mark == 1 ? blColor.cooked : blColor.notavailable : blColor.main;
	for( ushort i = picHeight, ie = 0; i > ie; --i ){
		for( ushort j = picWidth, je = 0; j > je; --j){
			/*if( *dstPtr >= 240 ){
				*dstPtr -= 80;
			}else if( *dstPtr >= 176 && *dstPtr <= 191 ){
				*dstPtr -= 16;
			}*/
			if (*dstPtr >= 240) {
				*dstPtr = (*dstPtr & 0b1111) + curColor;
			}
			++dstPtr;
		}
		dstPtr -= WorkingWidth + picWidth;
	}
}

//----- (th2) -------------------------------------------------------------
void __fastcall CustomShadowItemSlot(int xPosition, int yPosition, ushort width, ushort height, int colorGradient, uint8_t stroke)
{
	int frameBorderColor = (colorGradient & 0b11110000) + 5; // цвет рамки
	uchar* dstPtr = (uchar*)&WorkingSurface[xPosition] + YOffsetHashTable[yPosition];
	if (stroke == 1) {
		for (ushort i = height; i > 0; --i) {
			for (ushort j = width; j > 0; --j) {
				if (*dstPtr >= 160) {
					if (j < 4 || j > width - 3 || i > height - 3 || i < 4) // Рисуем рамку
						*dstPtr = ((*dstPtr & 0b1111) >> 1) + frameBorderColor; 
					else
						*dstPtr = (*dstPtr & 0b1111) + colorGradient;
				}
				++dstPtr;
			}
			dstPtr -= WorkingWidth + width;
		}
	} else if (stroke == 2) {
		for (ushort i = height; i > 0; --i) {
			for (ushort j = width; j > 0; --j) {
				if (*dstPtr >= 160) {
					if (j < 4 || j > width - 3 || i > height - 3 || i < 4) // Рисуем рамку
						*dstPtr = frameBorderColor + 4;
					else
						*dstPtr = (*dstPtr & 0b1111) + colorGradient;
				}
				++dstPtr;
			}
			dstPtr -= WorkingWidth + width;
		}
	} else {
		for (ushort i = height; i > 0; --i) {
			for (ushort j = width; j > 0; --j) {
				if (*dstPtr >= 160)
					*dstPtr = (*dstPtr & 0b1111) + colorGradient;
				++dstPtr;
			}
			dstPtr -= WorkingWidth + width;
		}
	}
}

//----- (th2) -------------------------------------------------------------
void __fastcall ChangeItemBacklight(int xPos, int yPos, int width, int height, int xShift, int yShift, uint8_t stroke) {
	static constexpr int invCellSize = CELL_SIZE + 1;

	int curScrX = CursorX + Screen_LeftBorder;
	int curScrY = CursorY + Screen_TopBorder;
	
	int itemWidth = Cur.ItemWidth;
	int itemHeight = Cur.ItemHeight;

	int xCells = itemWidth / CELL_SIZE;
	int yCells = itemHeight / CELL_SIZE;

	int cols = 1 + width  - xCells;
	int rows = 1 + height - yCells;

	int left   = xPos - itemWidth  / 2 + (itemWidth  - CELL_SIZE) / 2;
	int right  = left + invCellSize;
	int top    = yPos - itemHeight / 2 + (itemHeight - CELL_SIZE) / 2 - (yCells < 3 ? 1 : 0);
	int bottom = top  + invCellSize;

	int shiftX, shiftY;

	for (int col = 0; col < cols; ++col) {
		shiftX = col * invCellSize;
		for (int row = 0; row < rows; ++row) {
			shiftY = row * invCellSize;
			if (   curScrX >= left   + shiftX
				&& curScrX <  right  + shiftX
				&& curScrY >  top    + shiftY
				&& curScrY <= bottom + shiftY
				) {
				CustomShadowItemSlot(
					xPos + shiftX + xShift,
					yPos + shiftY - yShift + yCells * invCellSize,
					invCellSize * xCells,
					invCellSize * yCells,
					blColor.overcells,
					stroke);
				break;
			}
		}
	}
}

//----- (0041F147) --------------------------------------------------------
void DrawBeltInventory()
{
	Player& player = Players[CurrentPlayerIndex];
	int curScrX = CursorX + Screen_LeftBorder;
	int curScrY = CursorY + Screen_TopBorder;
	int shadowColor = -1;
	bool useRecolor = true;
	extern bool InventoryTurnOffRedColorItems;

	// аналогично отрисовке инвентаря, сделана оптимизация, путем замены координат клеток на вычисляемые
	// ну и отрисовка вся аналогична, только добавлено рисование цифры в нижнем правом углу у клеток
	// в которых есть используемый предмет
	if( TalkPanelMode ){
		return;
	}
	CopyFromMainPanelToWorkingSurface(205, 21, 232, 28, 269 + (Screen_LeftBorder - 64), 517);

	for( int i = 0; i < IS_8_Belt_Count; ++i ){
		Item& item = player.BeltInventory[i];
		int cellLeft = Belt->rect.Left + i * (CELL_SIZE + 1);

		// Подсветка предметов на поясе
		Item& curItem = player.ItemOnCursor;
		useRecolor = true;

		if (Cur.GraphicsID >= CM_12_ITEMS_PICS_START // TODO: change to IsItemAllowedForBelt
			&& curItem.ItemCode == IC_0_OTHER
			&& curItem.TypeID == ITEM_3_JEWELRY_POTION_RELICT
			&& is(curItem.MagicCode,
				MC_2_POTION_OF_FULL_HEALING,
				MC_3_POTION_OF_HEALING,
				MC_6_POTION_OF_MANA,
				MC_7_POTION_OF_FULL_MANA,
				MC_18_POTION_OF_REJUVENATION,
				MC_19_POTION_OF_FULL_REJUVENATION,
				MC_10_ELIXIR_OF_STRENGTH,
				MC_11_ELIXIR_OF_MAGIC,
				MC_12_ELIXIR_OF_DEXTERITY,
				MC_13_ELIXIR_OF_VITALITY,
				MC_21_RELIC_NEED_NO_TARGET,
				MC_22_RELIC_NEED_TARGET,
				MC_31_OIL_OF_REPAIR_I,
				MC_32_OIL_OF_REPAIR_II,
				MC_33_OIL_OF_REPAIR_III,
				MC_34_OIL_OF_REPAIR_IV,
				MC_35_OIL_OF_REPAIR_V,
				MC_36_BLACKSMITH_OIL,
				MC_37_OIL_OF_FORTITUDE,
				MC_38_OIL_OF_REPAIR_VI,
				MC_39_OIL_OF_REPAIR_VII,
				MC_40_OIL_OF_HARDENING,
				MC_47_RUNE_OF_FIRE,
				MC_48_RUNE_OF_LIGHTNING,
				MC_49_RUNE_OF_SHOCK,
				MC_50_RUNE_OF_BLAZE,
				MC_51_RUNE_OF_STONE ) ) {
			shadowColor = blColor.overcells;
			// if (!curItem.IsReqMet) shadowColor = 208;			
			if (curScrX >= cellLeft - 14
				&& curScrX <= cellLeft + 14
				&& curScrY >= Belt->rect.Top - 14
				&& curScrY <= Belt->rect.Down - 15)
			{
				// устанавливаем цвет фона слота, если курсор с предметом НАД доступным ему слотом
				if (curItem.IsReqMet)
					shadowColor = blColor.overslot;
				else
					shadowColor = blColor.notavailable;
			}
			CustomShadowItemSlot( cellLeft, Belt->rect.Down, CELL_SIZE, CELL_SIZE, shadowColor, 1);
			useRecolor = false;
		}

		if (item.ItemCode == IC_M1_NONE) {
			continue;
		}

		extern bool InventoryBackgroundOldStyle;
		uint8_t colorMark = InventoryBackgroundOldStyle ? 0 : GetItemBackgroundColorType(item);

		if (useRecolor) { // пропускаем перекраску, если уже покрашена выше по коду
			ShadowItemSlot(cellLeft, Belt->rect.Down, CELL_SIZE, CELL_SIZE, colorMark);
		}
		//if( !strcmp( item.Name, "Potion of Mana" ) ) __debugbreak();
		int graphicValue = item.GraphicValue + CM_12_ITEMS_PICS_START;
		char itemWidth = ItemWidthTable[graphicValue];
		char* itemsCELArray = GetItemSprites( graphicValue );
		bool itemUnderCursor = Cur.InventoryCellID == i + IS_50_47_Belt_Start;
		int colorOfBorder = -1;
		if(itemUnderCursor){
			colorOfBorder = 197;
			if( item.MagicLevel ){
				colorOfBorder = 181;
			}
			if( !item.IsReqMet ){
				colorOfBorder = 229;
			}
		}
		CEL_Draw(cellLeft, Belt->rect.Down, itemsCELArray, graphicValue, itemWidth, 0, 8, (int)!(item.IsReqMet || InventoryTurnOffRedColorItems || itemUnderCursor) | item.mirrorImage << 1, colorOfBorder);

		if( BaseItems[item.baseItemIndex].UseOnceFlag && item.IsReqMet && item.ItemCode != IC_11_GOLD ){
			int charIndex = '1' + i;
			uchar letterId = FontIndexSmall[Codepage[charIndex]];
			DrawLetter(Belt->rect.Down * WorkingWidth + (cellLeft + CELL_SIZE - FontWidthSmall[letterId]), letterId, 0);
		}
	}
}

//----- (0041F373) --------------------------------------------------------
bool __fastcall AutoPutCursorToCell( Player& player, int cellIndex, int width, int height, bool needPlaceItem )
{
	int startX = cellIndex % InvCols;
	int endX = startX + width;

	int startY = cellIndex / InvCols;
	int endY = startY + height;

	if( endX > player.invCols || endY > InvRows ){
		return false;
	}

	// check space
	for( int y = startY, ye = endY; y < ye; ++y ){
		for( int x = startX, xe = endX; x < xe; ++x ){
			if( player.InvUsedAlt[y][x] ){
				return false;
			}
		}
	}

	// return true if need only check space
	if( !needPlaceItem ){
		return true;
	}

	// place item
	// InvItemCount бывает отрицательное, бывает меньше чем обозначено в InvUsedAlt
	// возможно надо добавить пересчет и вынести все изменения в одну функцию
	int placedItemIndex = player.InvItemCount++;
	int placedItemNumber = placedItemIndex + 1;
	player.InventorySlots[placedItemIndex] = player.ItemOnCursor;

	for( int y = startY, ye = endY; y < ye; ++y ){
		for( int x = startX, xe = endX; x < xe; ++x ){
			if( y == endY - 1 && x == startX ){
				player.InvUsedAlt[y][x] = placedItemNumber;
			}else{
				player.InvUsedAlt[y][x] = -placedItemNumber;
			}
		}
	}
	//player.ItemOnCursor.ItemCode = IC_M1_NONE; // TODO: remove cursor item data from net sync
	UpdateRelictsState(player);
	return true;
}

// Та же функция что и выше, но чуть подкостыленная для проверки на свободное место при покупке и Адрии и Пепина
// Костыль сломает к чертям собачим инвентарь при попытке этой же функцией не только проверить,
// но и положить вещь 1 на 1 при заполненном инвентаре и незаполненном поясе.
// При использовании для проверки при той же ситуации всрет все вещи 1 на 1, которые нельзя автоматом поставить на пояс
// Что на текущий момент успешно делает
//----- (0041F505) --------------------------------------------------------
bool __fastcall TryPutItemInInventoryOrBelt( int playerIndex, int cellIndex, int width, int height, bool needPlaceItem )
{
	Player& player = Players[playerIndex];

	int startX = cellIndex % InvCols;
	int endX = startX + width;

	int startY = cellIndex / InvCols;
	int endY = startY + height;

	if( endX > player.invCols || endY > InvRows ){
		return false;
	}

	// check space
	for( int y = startY, ye = endY; y < ye; ++y ){
		for( int x = startX, xe = endX; x < xe; ++x ){
			if( player.InvUsedAlt[y][x] ){
				// stupid blizzard shit
				if( width == 1 && height == 1 ){
					for( int i = 0, ie = IS_8_Belt_Count; i < ie; ++i ){
						if( player.BeltInventory[i].ItemCode == IC_M1_NONE ){
							goto AFTER_CHECK;
						}
					}
				}
				return false;
			}
		}
	}

AFTER_CHECK:
	// return true if need only check space
	if( !needPlaceItem ){
		return true;
	}

	// place item
	int placedItemIndex = player.InvItemCount++;
	int placedItemNumber = placedItemIndex + 1;
	player.InventorySlots[placedItemIndex] = player.ItemOnCursor;

	for( int y = startY, ye = endY; y < ye; ++y ){
		for( int x = startX, xe = endX; x < xe; ++x ){
			if( y == endY - 1 && x == startX ){
				player.InvUsedAlt[y][x] = placedItemNumber;
			}else{
				player.InvUsedAlt[y][x] = -placedItemNumber;
			}
		}
	}

	UpdateRelictsState(player);
	return true;
}

//----- (0041F6CB) --------------------------------------------------------
bool __fastcall GoldAutoPlace( int playerIndex )
{
	Player& player = Players[playerIndex];
	Item& cursorItem = player.ItemOnCursor;
	// Вначале пытаемся раскидать золото по уже имеющимся неполным кучкам золота
	for( int i = 0; i < player.InvItemCount; ++i ){
		Item& item = player.InventorySlots[i];
		if( item.ItemCode == IC_11_GOLD ){
			int itemAmount = item.amount;
			int goldToPut = itemAmount + cursorItem.amount;
			if( goldToPut <= MaxGoldInCell ){
				item.amount = goldToPut;
				SetGraphicToGold(item);// оптимизация
				player.TotalGold = CalcTotalGold(playerIndex);
				cursorItem.amount = 0;
				SetCursorGraphics(CM_1_NORMAL_HAND);
				return true;
			}else if( item.amount < MaxGoldInCell ){
				item.amount = MaxGoldInCell;
				SetGraphicToGold(item);// оптимизация
				cursorItem.amount -= MaxGoldInCell - itemAmount;
				bool isPutAllGold = false;
				if( cursorItem.amount <= 0 ){
					cursorItem.amount = 0;
					isPutAllGold = true;
				}
				cursorItem.MakeSeed();
				SetGoldViewOnCursor(playerIndex);
				player.TotalGold = CalcTotalGold(playerIndex);
				if( isPutAllGold ){
					SetCursorGraphics(CM_1_NORMAL_HAND);
					return isPutAllGold;
				}
			}
		}
	}

	// если не получилось, то создаем новые кучки золота до тех пор пока не кончится место, либо пока все не раскидается
	for( int y = InvRows - 1, ye = 0; y >= ye; --y ){
		for( int x = player.invCols - 1, xe = 0; x >= xe; --x ){
			if( player.InvUsedAlt[y][x] ){
				continue;
			}
			int placedItemIndex = player.InvItemCount++;
			int placedItemNumber = placedItemIndex + 1;
			Item& item = player.InventorySlots[placedItemIndex];
			memcpy(&item, &cursorItem, sizeof(Item) );

			player.InvUsedAlt[y][x] = placedItemNumber;

			SetGraphicToGold(cursorItem);// оптимизация

			if( cursorItem.amount <= MaxGoldInCell ){
				cursorItem.amount = 0;
				player.TotalGold = CalcTotalGold(playerIndex);
				SetCursorGraphics(CM_1_NORMAL_HAND);
				return true;
			}else{
				cursorItem.amount -= MaxGoldInCell;
				cursorItem.MakeSeed();
				item.amount = MaxGoldInCell;
			}
		}
	}

	// не хватило места
	return false;
}

//----- (th3) -------------------------------------------------------------
bool OneHandSpecial(int playerIndex, Item& item)
{
	Player& player = Players[playerIndex];
	return is( item.ItemCode, IC_1_SWORD, IC_4_MACE )
	    && ( HasTrait(playerIndex, TraitId::MonkeyGrip)
		  || player.fullClassId == PFC_ROGUE && !HasTrait(playerIndex, TraitId::Axepertise)
		  || player.fullClassId == PFC_DIMACHAERUS )
		  || item.ItemCode == IC_5_SHIELD && HasTrait(playerIndex, TraitId::TwoTowers);
}

//----- (0041F9AF) --------------------------------------------------------
bool __fastcall PutWeaponFromCursorToArm( int playerIndex, int takenCell )
{
	Player& player = Players[playerIndex];
	Item& item = player.ItemOnCursor;
	
	bool oneHandSpecial = OneHandSpecial(playerIndex, item);
	ITEM_SLOT s1 = oneHandSpecial && takenCell != -1 && FastSwapRightOnAlt && IsAltPressed ? IS_RightHand : IS_LeftHand;
	ITEM_SLOT s2 = s1 == IS_LeftHand ? IS_RightHand : IS_LeftHand;
	Item& h1 = player.OnBodySlots[s1];
	Item& h2 = player.OnBodySlots[s2];
	Item temp;

	bool allowSpecial = oneHandSpecial && (h1.ItemCode == IC_M1_NONE || h1.EquippedLocation == EL_1_ONE_HANDED || OneHandSpecial(playerIndex, h1))
		                               && (h2.ItemCode == IC_M1_NONE || h2.EquippedLocation == EL_1_ONE_HANDED || OneHandSpecial(playerIndex, h2));

	// определение, одноручное ли оружие. У глада двуручные мечи и булавы считаются одноручными
	// одноручку можно надеть только если ни в одной из рук нет оружия или если это аська, которая орудует двумя оружиями
	if( item.EquippedLocation == EL_1_ONE_HANDED || allowSpecial ){
		if( h1.ItemCode == IC_M1_NONE && (is(h2.ItemCode, IC_M1_NONE, IC_5_SHIELD) || allowSpecial) && (item.ItemCode != IC_5_SHIELD || allowSpecial)){
            NetSendCmdChItem( true, s1 );
			h1 = item;
			return true;
		}
		else if( (h1.ItemCode != IC_M1_NONE && (item.ItemCode != IC_5_SHIELD || allowSpecial) || h1.ItemCode == IC_5_SHIELD && item.ItemCode == IC_5_SHIELD)
			  && ((h1.ItemCode != IC_5_SHIELD || allowSpecial) && item.TypeID == ITEM_1_WEAPON || (h1.ItemCode == IC_5_SHIELD || allowSpecial) && item.TypeID == ITEM_2_ARMOR) ){
			NetSendCmdChItem(true, s1);
			temp = h1;
			h1 = item;
			item = temp;
			return false;
		}
		if( h2.ItemCode == IC_M1_NONE && ((h1.ItemCode != IC_M1_NONE && h1.EquippedLocation != EL_2_TWO_HANDED) || h1.ItemCode == IC_M1_NONE)){
            NetSendCmdChItem( true, s2 );
			h2 = item;
			return true;
		}
		else if ((h1.ItemCode != IC_M1_NONE && h1.EquippedLocation != EL_2_TWO_HANDED) || h1.ItemCode == IC_M1_NONE) {
			temp = h2;
			h2 = item;
			item = temp;
			if( h1.ItemCode == IC_M1_NONE && oneHandSpecial ){
				temp = h2;
				h2 = h1;
				h1 = temp;
			}
			return false;
		}
		else if (h1.ItemCode != IC_M1_NONE && h1.EquippedLocation == EL_2_TWO_HANDED) {
			NetSendCmdChItem(true, s1);
			NetSendCmdChItem(true, s2);
			h2 = item;
			item = h1;
			h1.ItemCode = IC_M1_NONE;
			if( h1.ItemCode == IC_M1_NONE && oneHandSpecial ){
				temp = h2;
				h2 = h1;
				h1 = temp;
			}
			return false;
		}
	}else if( h1.ItemCode == IC_M1_NONE && h2.ItemCode == IC_M1_NONE ){
		// двуручное оружие можно взять только если обе руки пусты
        NetSendCmdChItem( true, s1 );
		h1 = item;
		return true;
	}
	else {
		if (h1.ItemCode != IC_M1_NONE) {
			NetSendCmdChItem(true, s1);
			temp = h1;
			h1 = item;
			item = temp;
			AutoPutCursorToInventoryOrDrop(true, takenCell);
		}
		else if (h1.ItemCode == IC_M1_NONE) {
			NetSendCmdChItem(true, s1);
			h1 = item;
		}
		if (h2.ItemCode != IC_M1_NONE) {
			NetSendCmdChItem(true, s2);
			item = h2;
			h2.ItemCode = IC_M1_NONE;
			return false;
		}
		else {
			return true;
		}
	}
	return false;
}

//----- (th2) -------------------------------------------------------------
bool PutArmorToBodySlots()
{
	Player& player = Players[CurrentPlayerIndex];
	Item& item = player.ItemOnCursor;
	ITEM_SLOT bodySlot;
	Item temp2;
	bool isBodySlotEmpty = false;

	switch (item.EquippedLocation){
	case EL_3_TORSO:
		bodySlot = IS_Torso;
		if (player.OnBodySlots[IS_Torso].ItemCode == IC_M1_NONE) {
			isBodySlotEmpty = true;
		}
		break;
	case EL_4_HEAD:
		bodySlot = IS_Head;
		if (player.OnBodySlots[IS_Head].ItemCode == IC_M1_NONE) {
			isBodySlotEmpty = true;
		}
		break;
	case EL_5_FINGER:
		if( FastSwapRightOnAlt ){
			bodySlot = IsAltPressed ? IS_RightRing : IS_LeftRing;
			isBodySlotEmpty = player.OnBodySlots[bodySlot].ItemCode == IC_M1_NONE;
			break;
		}
		if (player.OnBodySlots[IS_LeftRing].ItemCode == IC_M1_NONE) {
			bodySlot = IS_LeftRing;
			isBodySlotEmpty = true;
		}
		else if (player.OnBodySlots[IS_RightRing].ItemCode == IC_M1_NONE) {
			bodySlot = IS_RightRing;
			isBodySlotEmpty = true;
		}
		else {
			bodySlot = IS_LeftRing;
		}
		break;
	case EL_6_NECK:
		bodySlot = IS_Neck;
		if (player.OnBodySlots[IS_Neck].ItemCode == IC_M1_NONE) {
			isBodySlotEmpty = true;
		}
		break;
	case EL_9_WAIST:
		bodySlot = IS_Waist;
		if (player.OnBodySlots[IS_Waist].ItemCode == IC_M1_NONE) {
			isBodySlotEmpty = true;
		}
		break;
	case EL_10_HANDS:
		bodySlot = IS_Hands;
		if (player.OnBodySlots[IS_Hands].ItemCode == IC_M1_NONE) {
			isBodySlotEmpty = true;
		}
		break;
	case EL_11_FEET:
		bodySlot = IS_Feet;
		if (player.OnBodySlots[IS_Feet].ItemCode == IC_M1_NONE) {
			isBodySlotEmpty = true;
		}
		break;
	default:
		isBodySlotEmpty = true;
		break;
	}
	
	if (isBodySlotEmpty) {
		NetSendCmdChItem(true, bodySlot);
		memcpy(&player.OnBodySlots[bodySlot], &item, sizeof(Item));
		return true;
	}
	else {
		NetSendCmdChItem(true, bodySlot);
		memcpy(&temp2, &player.OnBodySlots[bodySlot], sizeof(Item));
		memcpy(&player.OnBodySlots[bodySlot], &item, sizeof(Item));
		memcpy(&item, &temp2, sizeof(Item));
		return false;
	}

	return false;
}

// костыльная функция обмена вещей. Используется тольво в PutCursorItem
// одновременно выставляет graphicValue в значение, которое было у первой вещи
// а после обмена стало у второй. За счет этого если вторым аргументом идет вещь с курсора
// а первым вещь с инвентаря, то вызвав SetCursorGraphics(cursorGraphics), 
// курсор будет сразу настроен на вещь, которая попала обменом с игрока на курсор
//----- (0041FA98) --------------------------------------------------------
int __fastcall SwapItemsAndGetFirstItemGraphicsValue( Item* item1, Item* item2 )
{
	Item tempItem = *item1;
	*item1 = *item2;
	*item2 = tempItem;
	return item2->GraphicValue + CM_12_ITEMS_PICS_START;
}

//----- (004208FC) --------------------------------------------------------
void __fastcall EquipItemFromNetwork( int playerIndex, int onBodySlotIndex, LastItemInfo info )
{
	Player& player = Players[playerIndex];
	Item& cursorItem = player.ItemOnCursor;
	Item& item = Items[MaxItems_255];
	//const bool identified = !notIdentified;//Caused all equipments and affixes to desync upon slot switching / re-equipping.
	RecreateItem(MaxItems_255, info.baseItemIndex, info.dropType, info.seed, info.amount, info.magicLevelAndIdentified, info.genVersion, info.quality, info.magicFind, info.uniqIndex, info.enchantIndexes, info.quenchAffix, info.quenchLevel, info.socketsAdded, info.sockets, info.difficulty, info.charFlag);
	item.CurDurability = info.curDurability;
	item.BaseDurability = info.baseDurability;
	item.CurCharges = info.curCharges;
	item.BaseCharges = info.baseCharges;
	cursorItem = item;
	if( onBodySlotIndex < IS_Inventory ){ //  если слот на теле героя
		memcpy(&player.OnBodySlots[onBodySlotIndex], &cursorItem, sizeof(Item));

		// херня. Во первых нет проверки на класс - гладиатор
		// во-вторых двуручка всегда кладется в 4х слот (у всех кроме глада, но у него это и не двуручкой считается)
		// если положить двуручку в 5й слот, то все сломается
		if( onBodySlotIndex == IS_LeftHand ){ //  левая рука
			if( cursorItem.EquippedLocation == EL_2_TWO_HANDED ){
				player.OnBodySlots[IS_RightHand].ItemCode = IC_M1_NONE;
			}
		}else if( onBodySlotIndex == IS_RightHand ){ //  правая рука
			if( cursorItem.EquippedLocation == EL_2_TWO_HANDED ){
				player.OnBodySlots[IS_LeftHand].ItemCode = IC_M1_NONE;
			}
		}
	}
	RecalcPlayer(playerIndex, 1);
}

//----- (th2) -------------------------------------------------------------
bool IsItemAllowedForBelt(const Item& item)
{
	switch(item.MagicCode){
	case MC_2_POTION_OF_FULL_HEALING:
	case MC_3_POTION_OF_HEALING:
	case MC_6_POTION_OF_MANA:
	case MC_7_POTION_OF_FULL_MANA:
	case MC_18_POTION_OF_REJUVENATION:
	case MC_19_POTION_OF_FULL_REJUVENATION: return true;
	case MC_21_RELIC_NEED_NO_TARGET:
	case MC_22_RELIC_NEED_TARGET: return is(item.SpellIndex, PS_2_HEALING, PS_37_MANA_RECHARGE);
	}
	return false;
}

//----- (00719070) --------------------------------------------------------
int CountOfItemsOnBelt ()
{
	Player& player = Players[CurrentPlayerIndex];
	int countOfItems = 0;
	for( int beltIndex = 0; beltIndex < IS_8_Belt_Count; beltIndex++ ){
		if( player.BeltInventory[beltIndex].ItemCode != IC_M1_NONE ){
			++countOfItems;
		}
	}
	return countOfItems;
}

//----- (00420E36) --------------------------------------------------------
void __fastcall DestroyOnBodyItem( int playerIndex, int onBodySlot )
{
	Player& player = Players[playerIndex];
	if( onBodySlot < IS_Inventory ){
		player.OnBodySlots[onBodySlot].ItemCode = IC_M1_NONE;
	}
	RecalcPlayer(playerIndex, player.CurAction != PCA_8_DEATH);
}

//----- (00420E7F) --------------------------------------------------------
void __fastcall ClearInventorySlot( int playerIndex, int invItemIndex, bool mouse )
{
	Player& player = Players[playerIndex];

	//int invItemCount;
	//int result; 

	int itemNumber = invItemIndex + 1;
	for( int y = 0, ye = InvRows; y < ye; ++y ){
		for( int x = 0, xe = InvCols; x < xe; ++x ){
			if( abs(player.InvUsedAlt[y][x]) == itemNumber ){
				player.InvUsedAlt[y][x] = 0;
			}
		}
	}

	int lastItemIndex = --player.InvItemCount;
	int lastItemNumber = lastItemIndex + 1;

	Item& item = player.InventorySlots[invItemIndex];
	Item& lastItem = player.InventorySlots[lastItemIndex];

	if( player.InvItemCount > 0 && player.InvItemCount != invItemIndex ){
		memcpy(&item, &lastItem, sizeof(Item));
		for( int y = 0, ye = InvRows; y < ye; ++y ){
			for( int x = 0, xe = InvCols; x < xe; ++x ){
				char& curItemNumber = player.InvUsedAlt[y][x];
				if( curItemNumber == lastItemNumber ){
					curItemNumber = itemNumber;
				}else if( curItemNumber == -lastItemNumber ){
					curItemNumber = -itemNumber;
				}
			}
		}
	}
	if( mouse ) DispatchCursorAboveIsoland();

	UpdateRelictsState(player);
	// полный дубль хвоста UpdateRelictsState. Не знаю нафига
	if( player.SpellType == SO_2_RELIC ){
		if( !(player.AvailableRelictMask & BIT(player.CurrentSpellIndex)) ){
			player.CurrentSpellIndex = PS_M1_NONE;
			player.SpellType = SO_4_NONE;
		}
	}
}

// Саб используется чтобы удалить все вещи нехэлфаеровского перса для избежания морфа вещей золото не морфится, так что его оставляем.
//----- (00420FD0) --------------------------------------------------------
void __fastcall StripPlayer( int playerIndex )
{
	Player& player = Players[playerIndex];

	if( player.Appearance != 0 ){
		player.Appearance = 0;
		player.LoadPlayerAnimationMask = PAF_0_NO;
		SetPlrAnims(playerIndex);
	}

	for( int i = 0; i < IS_Inventory; ++i){
		player.OnBodySlots[i].ItemCode = IC_M1_NONE;
	}

	for( int y = 0, ye = InvRows; y < ye; ++y ){
		for( int x = 0, xe = InvCols; x < xe; ++x ){
			player.InvUsedAlt[y][x] = 0;
		}
	}

	int invItemCount = player.InvItemCount;
	player.InvItemCount = 0;
	for( int i = 0; i < invItemCount; ++i ){
		Item& topItem = player.InventorySlots[player.InvItemCount];
		Item& item = player.InventorySlots[i];
		if( player.InventorySlots[i].ItemCode == IC_11_GOLD ){
			if( player.InvItemCount != i ){
				memcpy(&topItem, &item, sizeof(Item));
			}
			int x = player.InvItemCount % InvCols;
			int y = player.InvItemCount / InvCols;
			player.InvUsedAlt[y][x] = ++player.InvItemCount;
		}
	}

	for( int i = 0; i < IS_8_Belt_Count; i++){ // Очищаем вещи на поясе
		player.BeltInventory[i].ItemCode = IC_M1_NONE;
	}

	CalcCharParams(playerIndex, 0);
}

//----- (00421122) --------------------------------------------------------
void __fastcall ClearBeltSlot( int playerIndex, int beltSlot )
{
	Player& player = Players[playerIndex];
	player.BeltInventory[beltSlot].ItemCode = IC_M1_NONE;
	UpdateRelictsState(player);

	// полный дубль хвоста UpdateRelictsState. Не знаю нафига
	if( player.SpellType == SO_2_RELIC ){
		if( !(player.AvailableRelictMask & BIT(player.CurrentSpellIndex)) ){
			player.CurrentSpellIndex = PS_M1_NONE;
			player.SpellType = SO_4_NONE;
		}
	}
}

//----- (004211DE) --------------------------------------------------------
void __fastcall CheckItemStats( int playerIndex )
{
	Player& player = Players[playerIndex];
	player.ItemOnCursor.IsReqMet = CheckItemReq( playerIndex, &player.ItemOnCursor );
}

//----- (00712250) --------------------------------------------------------
unsigned char __fastcall GetBookLevelReq( int spellIndex, int playerIndex )
{
	const Player& player = Players[playerIndex];
	int spellLevel = player.SpellLevels[spellIndex];
	int clvlReqFromCurrentSlvl = 0;
	if( spellLevel >= SpellLevelBookMax() ){ // if spell level is 100 or more
		clvlReqFromCurrentSlvl = 255; // ...this must mean: CLVL 255 required (cannot read book anymore, because slvl 100 is max)
	}else if( spellLevel >= 3 ){
		clvlReqFromCurrentSlvl = 3 * spellLevel / 2;// minimium: 4, maximum: 148
	}
	
	const Spell& spell = Spells[spellIndex];
	uchar result = std::max( spell.RequiredClvl, clvlReqFromCurrentSlvl );
	return result;
}

//----- (0042122B) --------------------------------------------------------
void __fastcall CheckBookLevel( int playerIndex )
{
	Player& player = Players[playerIndex];
	Item& itemOnCursor = player.ItemOnCursor;

	if( itemOnCursor.MagicCode != MC_24_BOOKS ){
		return;
	}
	itemOnCursor.RequiredMagic = Spells[itemOnCursor.SpellIndex].RequiredMagic;
	itemOnCursor.RequiredLevel = GetBookLevelReq(itemOnCursor.SpellIndex, playerIndex);
	int bookSpellLevel = player.SpellLevels[itemOnCursor.SpellIndex];

	for( int i = 0; i < bookSpellLevel; ++i ){
		itemOnCursor.RequiredMagic += 20;
	}
}

//----- (004212E2) --------------------------------------------------------
void __fastcall CheckQuestItem( int playerIndex )
{
	Player& player = Players[playerIndex];
	Player& curPlayer = Players[CurrentPlayerIndex];
	Item& cursorItem = player.ItemOnCursor;

	for (auto&& uid : blindRewards) {
		if (cursorItem.UniqueItemIndex == uid) {
			Quest& hallsOfTheBlindQuest = Quests[Q_8_HALLS_OF_THE_BLIND];
			if (hallsOfTheBlindQuest.status == QS_2_IN_PROGRESS) {
				hallsOfTheBlindQuest.status = QS_3_COMPLETE;
			}
			break;
		}
	}

	if( cursorItem.baseItemIndex == BI_17_HUGE_AXE ){
		Quest& hugeAxeQuest = Quests[Q_1_BLACK_MUSHROOM];
		if( hugeAxeQuest.status == QS_2_IN_PROGRESS 
			&& hugeAxeQuest.status2 == QS_3_COMPLETE){
				PlayerSoundMessageDelay = 10;

				switch (player.ClassID){
				case PC_0_WARRIOR:
				case PC_5_SAVAGE:SoundMessage = S_825_WAR_SL;	break;
				case PC_2_MAGE:		SoundMessage = S_613_SOR_SL;	break;
				case PC_3_MONK:		SoundMessage = S_933_MONK_SL;		break;
				case PC_1_ARCHER:
				case PC_4_ROGUE:	SoundMessage = S_716_ROG_SL;		break;
				}
				hugeAxeQuest.status2 = 4;
		}
	}

	if( cursorItem.baseItemIndex == BI_16_ANVIL_OF_FURY ){
		Quest& anvilOfFuryQuest = Quests[Q_10_ANVIL_OF_FURY];
		if( MaxCountOfPlayersInGame == 1 ){ // Original SP logic
			if( anvilOfFuryQuest.status == QS_1_ACCEPT ){
				anvilOfFuryQuest.status = QS_2_IN_PROGRESS;
				anvilOfFuryQuest.status2 = 1;
			}
		}else{ // MP Anvil quest anti-cheat measure.  Quest is completed as Anvil is picked up.  This disables Anvil spawn for all other players who have not visited Dlvl 10.
			anvilOfFuryQuest.status = QS_3_COMPLETE;
			NetSendCmdQuest(true, Q_10_ANVIL_OF_FURY);
		}
		if( anvilOfFuryQuest.talkToFlag == 1 ){
			PlayerSoundMessageDelay = 10;

			switch (curPlayer.ClassID){
			case PC_0_WARRIOR:
			case PC_5_SAVAGE:SoundMessage = S_819_WAR_89;	break;
			case PC_2_MAGE:		SoundMessage = S_607_SOR_89;	break;
			case PC_3_MONK:		SoundMessage = S_927_MONK_89;		break;
			case PC_1_ARCHER:
			case PC_4_ROGUE:	SoundMessage = S_710_ROG_89;		break;
			}
		}
	}

	if( cursorItem.baseItemIndex == BI_328_GOLDEN_ELIXIR ){
		PlayerSoundMessageDelay = 30;

		switch (curPlayer.ClassID){
		case PC_0_WARRIOR:
		case PC_5_SAVAGE:SoundMessage = S_1074;	break;
		case PC_2_MAGE:		SoundMessage = S_1076;	break;
		case PC_3_MONK:		SoundMessage = S_1077;		break;
		case PC_1_ARCHER:
		case PC_4_ROGUE:	SoundMessage = S_1075;		break;
		}
	}

	if( player.ItemOnCursor.baseItemIndex == BI_9_MAGIC_ROCK ){
		Quest& magicRockQuest = Quests[Q_0_THE_MAGIC_ROCK];

		if( magicRockQuest.status == QS_1_ACCEPT ){
			magicRockQuest.status = QS_2_IN_PROGRESS;
			magicRockQuest.status2 = 1;
		}
		if( magicRockQuest.talkToFlag == 1 ){
			PlayerSoundMessageDelay = 10;

			switch (curPlayer.ClassID){
			case PC_0_WARRIOR:
			case PC_5_SAVAGE:SoundMessage = S_817_WAR_87;	break;
			case PC_2_MAGE:		SoundMessage = S_605_SOR_87;	break;
			case PC_3_MONK:		SoundMessage = S_925_MONK_87;		break;
			case PC_1_ARCHER:
			case PC_4_ROGUE:	SoundMessage = S_708_ROG_87;		break;
			}
		}
	}

	for (int i = 0; i < VALOR_CLASSES; i++) {
		for (auto&& uid : valorRewards[i]) {
			if (player.ItemOnCursor.UniqueItemIndex == (int)uid) {
				Quest& valorQuest = Quests[Q_9_VALOR];
				if (valorQuest.status == QS_2_IN_PROGRESS) {
					valorQuest.status = QS_3_COMPLETE;
					PlayerSoundMessageDelay = 20;
					switch (curPlayer.ClassID) {
					case PC_0_WARRIOR:
					case PC_5_SAVAGE:SoundMessage = S_821_WAR_SL;	break;
					case PC_2_MAGE:		SoundMessage = S_609_SOR_SL;	break;
					case PC_3_MONK:		SoundMessage = S_929_MONK_SL;		break;
					case PC_1_ARCHER:
					case PC_4_ROGUE:	SoundMessage = S_712_ROG_SL;		break;
					}
				}
				break;
			}
		}
	}
	

	if( player.ItemOnCursor.baseItemIndex == BI_22_CRYPT_MAP ){
		Quest& lostTreasureQuest = Quests[Q_19_LOST_TREASURE];
		if( lostTreasureQuest.status2 > QS_1_ACCEPT ){
			return;
		}
		lostTreasureQuest.status = QS_2_IN_PROGRESS;// After picking up Treasure Map on cursor, .status/.status2/.status3/.talkToFlag must be 2/2/0/1.
		lostTreasureQuest.status2 = 2;
		lostTreasureQuest.talkToFlag = true;
		if( MaxCountOfPlayersInGame > 1 ){
			NetSendCmdQuest(true, Q_19_LOST_TREASURE);
		}
		PlayerSoundMessageDelay = 10;

		switch (curPlayer.ClassID){
		case PC_0_WARRIOR:
		case PC_5_SAVAGE:SoundMessage = S_809_WAR_46;	break;
		case PC_2_MAGE:		SoundMessage = S_597_SOR_46;	break;
		case PC_3_MONK:		SoundMessage = S_917_MONK_46;		break;
		case PC_1_ARCHER:
		case PC_4_ROGUE:	SoundMessage = S_700_ROG_46;		break;
		}
	}

	int baseItem = cursorItem.baseItemIndex;
	if( baseItem == BI_335_TORN_NOTE_1 || baseItem == BI_336_TORN_NOTE_2 || baseItem == BI_337_TORN_NOTE_3 ){
		int notesFlags = 0;
		int invItemIndex;
		if( FindItemInInventory(playerIndex, BI_335_TORN_NOTE_1, invItemIndex) 
			|| baseItem == BI_335_TORN_NOTE_1 ){
				notesFlags |= BIT(1);
		}
		if( FindItemInInventory(playerIndex, BI_336_TORN_NOTE_2, invItemIndex) 
			|| baseItem == BI_336_TORN_NOTE_2 ){
				notesFlags |= BIT(2);
		}
		if( FindItemInInventory(playerIndex, BI_337_TORN_NOTE_3, invItemIndex) 
			|| baseItem == BI_337_TORN_NOTE_3 ){
				notesFlags |= BIT(3);
		}
		if( notesFlags == (BIT(1)|BIT(2)|BIT(3)) ){
			PlayerSoundMessageDelay = 10;

			switch (curPlayer.ClassID){
			case PC_0_WARRIOR:
			case PC_5_SAVAGE:SoundMessage = S_775_WAR_46;	break;
			case PC_2_MAGE:		SoundMessage = S_563_SOR_46;	break;
			case PC_3_MONK:		SoundMessage = S_883_MONK_46;		break;
			case PC_1_ARCHER:
			case PC_4_ROGUE:	SoundMessage = S_666_ROG_46;		break;
			}

			if( baseItem == BI_335_TORN_NOTE_1 ){
				FindItemInInventory(playerIndex, BI_336_TORN_NOTE_2, invItemIndex);
				ClearInventorySlot(playerIndex, invItemIndex);
				FindItemInInventory(playerIndex, BI_337_TORN_NOTE_3, invItemIndex);
				ClearInventorySlot(playerIndex, invItemIndex);
			}else if( baseItem == BI_336_TORN_NOTE_2 ){
				FindItemInInventory(playerIndex, BI_335_TORN_NOTE_1, invItemIndex);
				ClearInventorySlot(playerIndex, invItemIndex);
				FindItemInInventory(playerIndex, BI_337_TORN_NOTE_3, invItemIndex);
				ClearInventorySlot(playerIndex, invItemIndex);
			}else if( baseItem == BI_337_TORN_NOTE_3 ){
				FindItemInInventory(playerIndex, BI_335_TORN_NOTE_1, invItemIndex);
				ClearInventorySlot(playerIndex, invItemIndex);
				FindItemInInventory(playerIndex, BI_336_TORN_NOTE_2, invItemIndex);
				ClearInventorySlot(playerIndex, invItemIndex);
			}
			Item& item = Items[ItemAvailable[0]];
			Item tempItem;

			memcpy(&tempItem, &item, sizeof(Item));
			{
				GetItemAttrs(ItemAvailable[0], BI_338_RECONSTRUCTED_NOTE, 16);
				SetupItemAnim(ItemAvailable[0]);
				memcpy(&cursorItem, &item, sizeof(Item));
			}
			memcpy(&item, &tempItem, sizeof(Item));
			return;
		}
	}
	if( baseItem == BI_399_SHADOWFANG ){ // fleshdoom patch (8) did
		// обнаружил баг с поднятием вещи, в частности этого меча, иногда (load/new game) не реагирует на клик на вещь
		// в th1 при сверке тоже самое, разобраться
		PlayerSoundMessageDelay = 10;
		if( Quests[Q_21_THE_CURSED_BLADE].status2 >= 7 ){
			switch( curPlayer.ClassID ){
			case PC_0_WARRIOR:
			case PC_5_SAVAGE:	SoundMessage = S_823_WAR_93;	break;
			case PC_2_MAGE:		SoundMessage = S_1085;			break;
			case PC_3_MONK:		SoundMessage = S_1086;			break; // для монка пока нет файла monk93.wav, используется речь воина
			case PC_1_ARCHER:
			case PC_4_ROGUE:	SoundMessage = S_714_ROG_93;	break;
			}
		}else{
			switch( curPlayer.ClassID ){
			case PC_0_WARRIOR:
			case PC_5_SAVAGE:	SoundMessage = S_816_WAR_86;	break;
			case PC_2_MAGE:		SoundMessage = S_1087;			break;
			case PC_3_MONK:		SoundMessage = S_1088;			break; // для монка пока нет файла monk86.wav, используется речь воина
			case PC_1_ARCHER:
			case PC_4_ROGUE:	SoundMessage = S_707_ROG_86;	break;
			}
		}
	}
}

// Processes CMD_8_GETITEM player command
//----- (004217DB) --------------------------------------------------------
void __fastcall InvGetItem( int playerIndex, int itemIndex )
{
	if( (uint)itemIndex >= MaxItems_255 ) return;
	IsGoldSplitPanelVisible = false;
	Player& player = Players[playerIndex];
	Player& curPlayer = Players[CurrentPlayerIndex];
	Item& item = Items[itemIndex];
	Item& cursorItem = player.ItemOnCursor;
	if( !ItemMap[item.MapRow][item.MapCol] ){
		return;
	}
	if( CurrentPlayerIndex == playerIndex && Cur.GraphicsID >= CM_12_ITEMS_PICS_START ){
        NetSendCmdPItem( true, CMD_94_SYNCPUTITEM, curPlayer.Row, curPlayer.Col );
	}
	if( item.UniqueItemIndex ){
		item.dropType &= ~D_PREGEN;
	}
	memcpy(&cursorItem, &item, sizeof(Item));
	CheckQuestItem(playerIndex);
	CheckBookLevel(playerIndex);
	CheckItemStats(playerIndex);
	bool isAllGoldFromCursorPlaced = false;
	if( cursorItem.ItemCode == IC_11_GOLD ){
		if( GoldAutoPlace(playerIndex) ){
			isAllGoldFromCursorPlaced = true;
		}
	}
	PlayLocalSound(S_56_I_INVGRAB, item.MapRow, item.MapCol);

	ItemMap[item.MapRow][item.MapCol] = 0;
	int freeIndex;
	for( int i = 0; i < ItemsCount; ){
		freeIndex = ItemActive[i];
		if( freeIndex == itemIndex ){
			DeleteItem( freeIndex, i);
			i = 0;
		}else{
			++i;
		}
	}
	Cur.ItemID = -1;
	if( !isAllGoldFromCursorPlaced ){
		SetCursorGraphics(cursorItem.GraphicValue + CM_12_ITEMS_PICS_START);
	}
}

//----- (th3) -------------------------------------------------------------
Item* __fastcall CanPutCursorToBelt(Player& player)
{
	Item& cursorItem = player.ItemOnCursor;
	BaseItem& base = BaseItems[cursorItem.baseItemIndex];
	if( cursorItem.IsReqMet && base.UseOnceFlag
	 && (is(base.MagicCode, MC_2_POTION_OF_FULL_HEALING, MC_3_POTION_OF_HEALING, MC_6_POTION_OF_MANA, MC_7_POTION_OF_FULL_MANA, MC_18_POTION_OF_REJUVENATION, MC_19_POTION_OF_FULL_REJUVENATION)
	  || is(base.MagicCode, MC_21_RELIC_NEED_NO_TARGET, MC_22_RELIC_NEED_TARGET) && is(base.SpellNumber, PS_2_HEALING, PS_37_MANA_RECHARGE)) ){
		for( auto& beltItem: player.BeltInventory ){
			if( beltItem.ItemCode == IC_M1_NONE ) return &beltItem;
		}
	}
	return 0;
}

//----- (th3) -------------------------------------------------------------
bool __fastcall AutoPutCursorToInventory(Player& player, int cell /*= -1*/)
{
	bool put = false;
	if( cell != -1 ){
		put = AutoPutCursorToCell( player, cell, ItemWidthCells, ItemHeightCells, true);
		if( put ) return put;
	}
	if( ItemWidthCells == 1 && ItemHeightCells == 1 ){
		Item* beltItem = CanPutCursorToBelt(player);
		if( beltItem && ! IsTransmutePanelVisible ){
			*beltItem = player.ItemOnCursor;
			//player.ItemOnCursor.ItemCode = IC_M1_NONE; // TODO: remove cursor item data from net sync
			UpdateRelictsState(player);
			put = true;
		}else{
			for( int y = InvRows - 1, ye = 0; y >= ye && ! put; --y ) for( int x = 0, xe = player.invCols; x < xe && !put; ++x ){
				put = AutoPutCursorToCell( player, InvCols * y + x, ItemWidthCells, ItemHeightCells, true);
			}
			if( ! put && beltItem && IsTransmutePanelVisible ){
				*beltItem = player.ItemOnCursor;
				UpdateRelictsState(player);
				put = true;
			}
		}
	}else{
		for( int y = 0, ye = InvRows - ItemHeightCells; y <= ye && !put; ++y ) for( int x = 0, xe = player.invCols - ItemWidthCells; x <=xe && !put; ++x ){
			put = AutoPutCursorToCell( player, InvCols * y + x, ItemWidthCells, ItemHeightCells, true );
		}
	}
	return put;
}

// Processes CMD_9_AGETITEM player command
//----- (00421978) --------------------------------------------------------
void __fastcall AutoGetItem( int playerIndex, int itemIndex )
{
	IsGoldSplitPanelVisible = false;

	Player& player = Players[playerIndex];
	Item& item = Items[itemIndex];
	Item& cursorItem = player.ItemOnCursor;
	
	if( itemIndex != MaxItems_255 && ! ItemMap[item.MapRow][item.MapCol] || Cur.GraphicsID >= CM_12_ITEMS_PICS_START ) return; // v2 fix belt duping and shift-click gear swap item disapear
	
	if( item.UniqueItemIndex ) item.dropType &= ~D_PREGEN;
	cursorItem = item;
	CheckQuestItem(playerIndex);
	CheckBookLevel(playerIndex);
	CheckItemStats(playerIndex);

	SetupItemSizes(cursorItem.GraphicValue + CM_12_ITEMS_PICS_START);
	bool put = false;
	if( cursorItem.ItemCode == IC_11_GOLD ){
		put = GoldAutoPlace(playerIndex);
		if( !put ){
			item.amount = cursorItem.amount;
		}
	}else{
		// auto equip weapons
		#ifdef DISABLED
		int apearance = player.Appearance & 0xF;
		if( (apearance == 0 || apearance == 1 || player.ClassID >= PC_4_ROGUE && (apearance == 6 || apearance == 2))
		 && player.CurAction <= PCA_3_WALK_HORISONTAL
		 && cursorItem.IsReqMet
		 && cursorItem.TypeID == ITEM_1_WEAPON ){
			put = PutWeaponFromCursorToArm(playerIndex);
			if( put ){
				RecalcPlayer(playerIndex, 1);
			}
		}
		#endif // DISABLED
	}
	if( ! put ){
		put = AutoPutCursorToInventory(player);
	}
	if( put ){
		int freeIndex;
		PlayLocalSound(S_56_I_INVGRAB, item.MapRow, item.MapCol);

		ItemMap[item.MapRow][item.MapCol] = 0;
		for( int i = 0; i < ItemsCount; ){
			freeIndex = ItemActive[i];
			if( freeIndex == itemIndex ){
				DeleteItem( freeIndex, i );
				i = 0;
			}else{
				++i;
			}
		}
	}else{ // has not found space in inventory
		if( playerIndex == CurrentPlayerIndex ){
			SOUND soundIndex = S_M1_NO_SOUND;
			switch (player.ClassID){
			case PC_0_WARRIOR:
			case PC_5_SAVAGE:soundIndex = S_737_WAR_14;	break;
			case PC_2_MAGE:		soundIndex = S_531_SOR_14;	break;
			case PC_3_MONK:		soundIndex = S_851_MONK_14;	break;
			case PC_1_ARCHER:
			case PC_4_ROGUE:	soundIndex = S_634_ROG_14;	break;
			}
			if( soundIndex != S_M1_NO_SOUND ){
				PlayGlobalSound( soundIndex + RNG(3) );
			}
		}
		memcpy(&cursorItem, &item, sizeof(Item));
		RespawnItem(itemIndex, true);
		if( NetItem ) NetSendCmdPItem( true, CMD_11_RESPAWNITEM, item.MapRow, item.MapCol );
		cursorItem.ItemCode = IC_M1_NONE;
		SetCursorGraphics(CM_1_NORMAL_HAND); // TODO: check if need (potention unwanted current player Cur erase)
	}
}

//----- (00421ECD) --------------------------------------------------------
int __fastcall FindGetItem( int baseItemIndex, int dropType, int seed, int genVersion, int quality, int magicFind, int uniqIndex)
{
	for( int i = 0; i < ItemsCount; i++){
		Item& ig = Items[ItemActive[i]];
		if( ig.baseItemIndex == baseItemIndex && ig.seed == seed && ig.dropType == dropType && ig.genVersion == genVersion
		 && ig.quality == quality && ig.magicFindSeed == magicFind && ig.uniqIndexSeed == uniqIndex){
			// TODO: check for amount needing
			return ItemActive[i];
		}
	}
	return -1;
}

//----- (00421F2B) --------------------------------------------------------
void __fastcall SyncGetItem( int row, int col, int baseItemIndex, int dropType, int seed, int genVersion, int quality, int magicFind, int uniqIndex )
{
	int itemNumber = ItemMap[row][col];
	int itemIndex = 0;
	// try optimal location first
    if( itemNumber != 0 ){
		// Attempting to find item by (row, col)
        itemIndex = itemNumber - 1;
        Item& item = Items[itemIndex];
        if( item.baseItemIndex != baseItemIndex || item.seed != seed || item.dropType != dropType || item.genVersion != genVersion
		 || item.quality != quality || item.magicFindSeed != magicFind || item.uniqIndexSeed != uniqIndex){
            itemIndex = FindGetItem( baseItemIndex, dropType, seed, genVersion, quality, magicFind, uniqIndex );
        }else{
			//DROPLOG("(row, col) said itemIndex == %d, FindGetItem says itemIndex == %d\n", itemIndex,
			//	FindGetItem( baseItemIndex, dropType, seed, genVersion, quality, magicFind, uniqIndex ));
			//itemIndex = itemNumber - 1;
        }
    }else{
		// Nongood (row, col)
        itemIndex = FindGetItem( baseItemIndex, dropType, seed, genVersion, quality, magicFind, uniqIndex );
    }
    if( itemIndex != -1 ){
		// removing item from map
        Item& item = Items[itemIndex];

		ItemMap[item.MapRow][item.MapCol] = 0;
		// remove from list
		bool needRepeat = false;
		do{
			needRepeat = false;
			for( int i = 0; i < ItemsCount; ++i ){
				if( ItemActive[i] == itemIndex ){ 
					// Removing item index from item list
					DeleteItem(itemIndex, i);
					// Item removed
					//jj = FindGetItem(idx,ci,iseed);
					//DROPLOG("  Item removed.  FindGetItem->%d, numitems->%d\n", jj, numitems);
					needRepeat = true;
					break;
				}
			}
		}while( needRepeat );
    }
}

//----- (0042204C) --------------------------------------------------------
bool __fastcall ItemSpacePathOk(int row, int col, int playerIndex)
{
	char steps[28];
	Player& player = Players[playerIndex];
	return ItemSpaceOk(row, col, playerIndex)
		&& (FindPath(CellFreeForPlayer, playerIndex, player.NextRow, player.NextCol, row, col, steps)
		|| player.NextRow == row && player.NextCol == col);
}

// TODO: all calls should check result
//----- (00422123) --------------------------------------------------------
int DropCursorItemBeforeTrig() // TODO: add player param
{
	if( ItemsCount < MaxItems_255 ){ // TODO: add check to item space on ground GetItemSpace
		if( NetItem ){
			// TODO: fix command queue overflow (easy check with Alt + V spaming)
			NetSendCmdPItem( 1, CMD_10_PUTITEM, PlayerRowPos, PlayerColPos ); // was Cur.Row, Cur.Col
		}else{
			if( InvPutItem(CurrentPlayerIndex, PlayerRowPos, PlayerColPos ) == -1 ) return false;
		}
		SetCursorGraphics(CM_1_NORMAL_HAND); // TODO: check all such code to find reason of nova rune sudden death
		//Players[CurrentPlayerIndex].ItemOnCursor.ItemCode = IC_M1_NONE; // TODO: check why this corrupt item on land with alt pressed (may be broken net command sequence)
		return true;
	}
	return false;
}

//----- (00422221) --------------------------------------------------------
void __fastcall AddWorldMessage(char* message)
{
	i64 now = GetTickCountTh(); // это таймер не влияет на гемплей, не перехватываем
	if( now - StatusMessageTimeout >= 5000_ms ){
		StatusMessageTimeout = now;
		ErrorPlrMsg( message );
	}
}

// Processes CMD_10_PUTITEM player command from myself
//----- (00422248) --------------------------------------------------------
int __fastcall InvPutItem(int playerIndex, int row /*= -1*/, int col /*= -1*/)
{
	int itemIndex = -1;
	Player& player = Players[playerIndex];
	if( col == -1 ){ row = player.Row; col = player.Col; }
	auto& ic = player.ItemOnCursor;
	if( FindGetItem(ic.baseItemIndex, ic.dropType, ic.seed, ic.genVersion, ic.quality, ic.magicFindSeed, ic.uniqIndexSeed ) != -1 ){
		AddWorldMessage( "A duplicate item has been detected.  Destroying duplicate..." );
		SyncGetItem(row, col, ic.baseItemIndex, ic.dropType, ic.seed, ic.genVersion, ic.quality, ic.magicFindSeed, ic.uniqIndexSeed );
	}
	int dir = OrientationToTarget( player.Row, player.Col, row, col );
	int dx = row - player.Row, dy = col - player.Col;
	if( player.Row && (abs(dx) > 1 || abs(dy) > 1) ){ // TOOD: find out clean fix for player coordinate corrupt ON_WARP
		col = player.Col + ColDelta[ dir ];
		row = player.Row + RowDelta[ dir ];
	}
	// TODO: check, there is no such conditions in SyncPutItem()
	if( 0 && ic.GraphicValue == 187 ){ // ITEM_RUNEBOMB
		NetSendCmdLocParam2(FALSE, CMD_101_OPEN_NEST, player.Row, player.Col, dx, dy);
		Quests[17].status = QS_3_COMPLETE; // Q_FARMER
		if( MaxCountOfPlayersInGame != 1 ) NetSendCmdQuest(1, Q_17_FALLEN_ANGEL); // Q_FARMER
		return -1;
	}
	// TODO: check, there is no such conditions in SyncPutItem()
	if( Dungeon == DUN_0_TOWN && ic.baseItemIndex == BI_22_CRYPT_MAP
	 && Cur.Row >= 19 && Cur.Row <= 20 && Cur.Col >= 31 && Cur.Col <= 32
	 && Quests[ Q_19_LOST_TREASURE ].status2 <= QS_2_IN_PROGRESS ){
		Quests[Q_19_LOST_TREASURE].status = QS_2_IN_PROGRESS;// After player dropped Treasure Map on the Disused Reliquary entrance, quest state should be set correctly .status/.status2/.status3/.talkToFlag = 2/3/0/1.
		Quests[ Q_19_LOST_TREASURE ].status2 = QS_3_COMPLETE;
		Quests[Q_19_LOST_TREASURE].talkToFlag = 1;
		if (MaxCountOfPlayersInGame > 1) {
			NetSendCmdQuest(true, Q_19_LOST_TREASURE);
		}
		SetCursorGraphics(CM_1_NORMAL_HAND);
		FixItemCursor(1);
		if(0){
			NetSendCmd(FALSE, CMD_102_OPEN_CRYPT);
			Quests[Q_16_HORAZON_S_DEMONS].status = QS_3_COMPLETE;
			if( MaxCountOfPlayersInGame != 1 ) NetSendCmdQuest(1, Q_16_HORAZON_S_DEMONS); // Q_CRYPTMAP
		}
		return -1;
	}
	if( GetItemSpace( row, col, itemIndex, false, ItemSpacePathOk, &ic) ){ // TODO: check why randomNear is false
		RespawnItem( itemIndex, 1 );
		SetCursorGraphics( CM_1_NORMAL_HAND );
		FixItemCursor(1);
		PlayGlobalSound(75);
	}else{
		SetCursorGraphics(ic.GraphicValue + CM_12_ITEMS_PICS_START); //TODO: check for current player check needing in this func
	}
	return itemIndex;
}

//----- (004225BB) --------------------------------------------------------
int __fastcall SyncPutItem( uint playerIndex, int row, int col, LastItemInfo info)
{
	int itemIndex = -1;
	if( FindGetItem( info.baseItemIndex, info.dropType, (int)info.seed, info.genVersion, info.quality, info.magicFind, info.uniqIndex ) != -1 ){
		AddWorldMessage( "A duplicate item has been detected from another player." );
        SyncGetItem( row, col, info.baseItemIndex, info.dropType, (int)info.seed, info.genVersion, info.quality, info.magicFind, info.uniqIndex );
	}
	Player& player = Players[ playerIndex ];
	int dir = OrientationToTarget( player.Row, player.Col, row, col );
	if( abs( row - player.Row ) > 1 || abs( col - player.Col ) > 1 ){
		row = player.Row + RowDelta[ dir ];
		col = player.Col + ColDelta[ dir ];
	}
	if( GetItemSpace( row, col, itemIndex, false, ItemSpacePathOk) ){
		Item& item = Items[itemIndex];
		RecreateItem( itemIndex, info.baseItemIndex, info.dropType, info.seed, info.amount, info.magicLevelAndIdentified, info.genVersion, info.quality, info.magicFind, info.uniqIndex, info.enchantIndexes, info.quenchAffix, info.quenchLevel, info.socketsAdded, info.sockets, info.difficulty, info.charFlag );
		item.CurDurability = info.curDurability;
		item.BaseDurability = info.baseDurability;
		item.CurCharges = info.curCharges;
		item.BaseCharges = info.baseCharges;
		RespawnItem( itemIndex, 1 );
	}
	return itemIndex;
}

//----- (00422B23) --------------------------------------------------------
bool __fastcall MinusRelictFromInventoryOrBelt(int playerIndex)
{
	Player& player = Players[playerIndex];
	// Ищем нужный реликт в инвентаре
	for( int i = 0; i < Players[playerIndex].InvItemCount; i++){
		Item& item = player.InventorySlots[i];
		if( Players[playerIndex].InventorySlots[i].ItemCode != IC_M1_NONE
		 && is( Players[playerIndex].InventorySlots[i].MagicCode, MC_21_RELIC_NEED_NO_TARGET, MC_22_RELIC_NEED_TARGET)
		 && Players[playerIndex].spellIndex == Players[playerIndex].InventorySlots[i].SpellIndex
		 && Players[playerIndex].CurMagic >= Players[playerIndex].InventorySlots[i].RequiredMagic
		 && Players[playerIndex].CurStrength >= Players[playerIndex].InventorySlots[i].RequiredStrength
		 && Players[playerIndex].CurDexterity >= Players[playerIndex].InventorySlots[i].RequiredDexterity){
			ClearInventorySlot(playerIndex, i);
			UpdateRelictsState(player);
			return true;
		}
	}
	// Ищем нужный реликт на поясе
	for( int i = 0; i < IS_8_Belt_Count; i++){
		if( Players[playerIndex].BeltInventory[i].ItemCode != IC_M1_NONE 
		 && is( Players[playerIndex].BeltInventory[i].MagicCode, MC_21_RELIC_NEED_NO_TARGET, MC_22_RELIC_NEED_TARGET)
		 && Players[playerIndex].spellIndex == Players[playerIndex].BeltInventory[i].SpellIndex
		 && Players[playerIndex].CurMagic >= Players[playerIndex].BeltInventory[i].RequiredMagic
		 && Players[playerIndex].CurStrength >= Players[playerIndex].BeltInventory[i].RequiredStrength
		 && Players[playerIndex].CurDexterity >= Players[playerIndex].BeltInventory[i].RequiredDexterity){
			ClearBeltSlot(playerIndex, i);
			return true;
		}
	}
	return false;
}

//----- (00717F50) --------------------------------------------------
int __fastcall IsSpellEnabledHere(int spell, int playerIndex)// #quest level spell restrictions
{
	if( DevelopMode && FreeSpell ) return 1;
	if( isTownPortalForbidden() ){
		if (spell == PS_7_TOWN_PORTAL) {
			return 0;
		}
	}
	//if(GameMode == GM_CLASSIC && Dungeon == DUN_16_HELL_4 && is(spell, PS_10_PHASING, PS_23_TELEPORT)){ return 0; }
	if( Dungeon->isQuest /*&& GameMode != GM_EASY*/ ){
		if( MonstersCount > SummonMonsters_Count && !SaveAlwaysEnabled && not(GameMode, GM_EASY, GM_CLASSIC) && spell == PS_7_TOWN_PORTAL && MaxCountOfPlayersInGame == 1 //Allow quest area TP in MP
		
			|| is( Dungeon, DUN_32_HORAZONS_HAVEN, DUN_33_SUMMONING_ROOMS, DUN_34_THE_PIT ) && is( spell, PS_10_PHASING, PS_23_TELEPORT, PS_33_TELEKINES, PS_42_WARP)
		
			|| Dungeon == DUN_35_PASSAGE_OF_FIRE && is(spell, PS_10_PHASING, PS_23_TELEPORT, PS_7_TOWN_PORTAL) 
		
			|| is(Dungeon, DUN_75_RIVER_OF_FIRE, DUN_78_THE_BRIDGE, DUN_91_STYGIAN_PASSAGE, DUN_98_EBON_CHASM, DUN_97_GLOOMY_DEPTHS, DUN_103_ALTAR_OF_IMMOLATION, DUN_104_NETHERWORLD_GATES, DUN_113_THE_MAZE) 
				&& is(spell, PS_10_PHASING, PS_23_TELEPORT, PS_7_TOWN_PORTAL, PS_42_WARP)
		
			|| is( Dungeon, DUN_39_FROZEN_LOCH, DUN_38_GLACIAL_CAVERNS) && is( spell, PS_1_FIREBOLT, PS_6_FIRE_WALL, PS_9_INFRAVISION, PS_10_PHASING, PS_12_FIREBLAST,
			/*PS_13_HYDRA,*/ PS_15_FORCE_WAVE, PS_19_FLAME_RING, PS_20_INCINERATE, PS_41_FIERY_NOVA, PS_45_RING_OF_FIRE, PS_23_TELEPORT, PS_42_WARP, PS_24_APOCALYPSE)
		
			|| Dungeon == DUN_36_VALLEY_OF_DESPAIR && is( spell, PS_6_FIRE_WALL, PS_15_FORCE_WAVE, PS_17_LIGHTING_RING, PS_18_LIGHTNING_NOVA, PS_19_FLAME_RING,
			PS_24_APOCALYPSE, PS_39_HOLY_NOVA, PS_40_LIGHTING_WALL, PS_41_FIERY_NOVA, PS_43_ARCANE_NOVA )
		
			|| Dungeon == DUN_40_ISLAND && is( spell, PS_6_FIRE_WALL, PS_10_PHASING, /*PS_13_HYDRA,*/ PS_15_FORCE_WAVE, PS_17_LIGHTING_RING, PS_19_FLAME_RING, PS_21_GOLEM,
			PS_23_TELEPORT, PS_40_LIGHTING_WALL, PS_42_WARP, PS_45_RING_OF_FIRE )
		
			|| is(Dungeon, DUN_40_ISLAND, DUN_39_FROZEN_LOCH, DUN_38_GLACIAL_CAVERNS ) 
				&& spell == PS_13_HYDRA 
				&& (!HasTrait(playerIndex, TraitId::Hydramancer))
		
			|| Dungeon == DUN_45_THE_DEN && is(spell, PS_42_WARP)
		
			|| Dungeon == DUN_29_ARCHBISHOP_LAZARUS_LAIR && is(spell, PS_9_INFRAVISION, PS_23_TELEPORT, PS_10_PHASING)
		
			|| is( Dungeon, DUN_41_FLESHDOOM, DUN_42_WIELDER, DUN_43_HEPHASTO) && is( spell, /*PS_23_TELEPORT,*/ PS_10_PHASING, PS_42_WARP )

			|| is(Dungeon, DUN_44_RAVENHOLM) && is(spell, PS_23_TELEPORT, PS_10_PHASING, PS_42_WARP)

			|| is(Dungeon, DUN_47_DARK_NEST, DUN_48_BLOOD_HIVE, DUN_54_ABANDONED_FANE, DUN_55_DEEP_HOLLOW, DUN_56_WEB_TUNNELS, DUN_57_ARACHNID_LAIR)
				&& is(spell, PS_42_WARP, /*PS_23_TELEPORT,*/ PS_10_PHASING, PS_9_INFRAVISION )

			|| spell == PS_42_WARP // warp is forbidden on all quest levels
		 /*|| is(Dungeon, DUN_56_WEB_TUNNELS, DUN_57_ARACHNID_LAIR)
			&& is(spell, PS_42_WARP, PS_10_PHASING, PS_9_INFRAVISION)*/
			){
			return 0;
//			return 1;//For faster testing quest area
		}
		return 1;
	}
	if( Dungeon != DUN_0_TOWN ){
		return 1;
	}
	return Spells[spell].IsEnabledInTown;
}

//----- (th2) -------------------------------------------------------------
int GetNumberOfRelicWithCurrentSpell(int currentSpellNumber)
{
	int numberOfRelicWithCurrentSpell = 0;
	Player& player = Players[CurrentPlayerIndex];

	for( int currentItemIndex = 0; currentItemIndex < player.InvItemCount; currentItemIndex++ ){
		int magicCode = player.InventorySlots[currentItemIndex].MagicCode;
		int spellNumber = player.InventorySlots[currentItemIndex].SpellIndex;
		int itemCode = player.InventorySlots[currentItemIndex].ItemCode;
		if( (magicCode == MC_21_RELIC_NEED_NO_TARGET || magicCode == MC_22_RELIC_NEED_TARGET) && spellNumber == currentSpellNumber && itemCode == 0 ){
			numberOfRelicWithCurrentSpell++;
		}
	}

	for( int currentItemIndex = 0; currentItemIndex < 8; currentItemIndex++ ){
		int magicCode = player.BeltInventory[currentItemIndex].MagicCode;
		int spellNumber = player.BeltInventory[currentItemIndex].SpellIndex;
		int itemCode = player.BeltInventory[currentItemIndex].ItemCode;
		if( (magicCode == MC_21_RELIC_NEED_NO_TARGET || magicCode == MC_22_RELIC_NEED_TARGET) && spellNumber == currentSpellNumber && itemCode == 0 ){
			numberOfRelicWithCurrentSpell++;
		}
	}

	return numberOfRelicWithCurrentSpell;
}

//----- (00422BC9) --------------------------------------------------------
bool IsHasRelicForSpell()
{
	int currentSpellNumber = Players[CurrentPlayerIndex].CurrentSpellIndex;
	if( Cur.GraphicsID == CM_1_NORMAL_HAND && IsSpellEnabledHere(currentSpellNumber, CurrentPlayerIndex) && GetNumberOfRelicWithCurrentSpell (currentSpellNumber) ){
		return true;
	}else{
		return false;
	}
}

//----- (00422C89) --------------------------------------------------------
// Если это магический посох с зарядами более 0 и в качестве магии выбрано заклинание с посоха
void __fastcall MinusChargeFromStaff(int casterIndex, int spellIndex)
{
	int slotIndex = FindBodySlotWithSpell(casterIndex, spellIndex);
	if( slotIndex == -1 ){
		return;
	}
	Player& player = Players[CurrentPlayerIndex];
	Item& item = player.OnBodySlots[slotIndex];
	if( item.CurCharges > 0 ){
		--item.CurCharges;
		SetAvailableChargesMask(casterIndex);
	}
}

//----- (00422CE9) --------------------------------------------------------
// Тут проверяется код заклинания посоха и код выбранного заклинания. Именно поэтому многозаклинательные посохи не будут работать.
bool IsHasChargesForSpell(int spellIndex)
{
	int slotIndex = FindBodySlotWithSpell(CurrentPlayerIndex, spellIndex);
	if( slotIndex == -1 ){
		return false;
	}
	const Player& player = Players[CurrentPlayerIndex];
	const Item& item = player.OnBodySlots[slotIndex];
	return item.CurCharges > 0;
}

//----- (00422DB2) --------------------------------------------------------
bool __fastcall InteractWithQuestItem(uint baseItemIndex, int playerIndex, uint itemIndex)
{
	Player& player = Players[playerIndex];
	bool IMNM = isTownPortalForbidden();
	switch (baseItemIndex) {
	case BI_327_TAVERN_SIGN:
		if (IMNM || HasTrait(playerIndex, TraitId::Leper)) {
			Quest& ogdenSingQuest = Quests[Q_7_OGDENS_SIGN];
			ogdenSingQuest.status = QS_3_COMPLETE;
			ogdenSingQuest.status2 = 3;
			ChangeMapRectAfterScriptEvent(CurPattern.row, CurPattern.col, CurPattern.sizeRow + CurPattern.row + 1, CurPattern.sizeCol + CurPattern.col + 1);
			ChangeAllPlayersVisibility();
			for(Monster& monster: Monsters)
				if (monster.speechIndex == SP_21) {
					monster.ActivationCounter = 255;
					monster.speechIndex = 0;
					monster.goal = MG_1_ATTACK;
					break;
				}
			PlayGlobalSound(S_1084);
			ClearInventorySlot(playerIndex, itemIndex);
			int uniqIndex;
			switch (Difficulty) {
			case DL_0_HORROR:		uniqIndex = 160;	break;
			case DL_1_PURGATORY:	uniqIndex = 161;	break;
			case DL_2_DOOM:			uniqIndex = 162;	break;
			case DL_3_CRUCIBLE:		uniqIndex = 2365;	break;
			case DL_4_TORMENT:		uniqIndex = 2366;	break;
			case DL_5_AGONY:		uniqIndex = 2367;	break;
			case DL_6_TERROR:		uniqIndex = 2368;	break;
			case DL_7_DESTRUCTION:	uniqIndex = 2369;	break;
			case DL_8_INFERNO:		uniqIndex = 2370;	break;
			}
			InvGetItem(playerIndex, SpawnUnique(uniqIndex, player.Row, player.Col));
			AutoPutCursorToInventoryOrDrop();
			SetCursorGraphics(CM_1_NORMAL_HAND);
			return true;
		} break;
	case BI_9_MAGIC_ROCK:
		if (IMNM || HasTrait(playerIndex, TraitId::Leper)) {
			Quest& magicRockQuest = Quests[Q_0_THE_MAGIC_ROCK];
			magicRockQuest.status = QS_3_COMPLETE;
			magicRockQuest.status2 = 2;
			magicRockQuest.status3 = 2;
			PlayGlobalSound(S_1084);
			ClearInventorySlot(playerIndex, itemIndex);
			int uniqIndex;
			switch (Difficulty) {
			case DL_0_HORROR:		uniqIndex = 152;	break;
			case DL_1_PURGATORY:	uniqIndex = 153;	break;
			case DL_2_DOOM:			uniqIndex = 154;	break;
			case DL_3_CRUCIBLE:		uniqIndex = 2383;	break;
			case DL_4_TORMENT:		uniqIndex = 2384;	break;
			case DL_5_AGONY:		uniqIndex = 2385;	break;
			case DL_6_TERROR:		uniqIndex = 2386;	break;
			case DL_7_DESTRUCTION:	uniqIndex = 2387;	break;
			case DL_8_INFERNO:		uniqIndex = 2388;	break;			
			}
			InvGetItem(playerIndex, SpawnUnique(uniqIndex, player.Row, player.Col));
			AutoPutCursorToInventoryOrDrop();
			SetCursorGraphics(CM_1_NORMAL_HAND);
			return true;
		} break;
	#if 0
	case BI_17_HUGE_AXE:
		PlayerSoundMessageDelay = 10;
		switch (player.ClassID) {
		case PC_4_ROGUE:
		case PC_1_ARCHER:		SoundMessage = S_716_ROG_SL;				return true;
		case PC_5_SAVAGE:
		case PC_0_WARRIOR:		SoundMessage = S_825_WAR_SL;				return true;
		case PC_2_MAGE:			SoundMessage = S_613_SOR_SL;				return true;
		case PC_3_MONK:			SoundMessage = S_933_MONK_SL;				return true;
		} break;
	#endif
	case BI_19_FUNGAL_TOME:
		if (IMNM || HasTrait(playerIndex, TraitId::Leper)) {
			PlayGlobalSound(S_1084);
			ClearInventorySlot(playerIndex, itemIndex);
			FullCreateAndPutItem(player.ItemOnCursor, BI_20_SPECTRAL_ELIXIR);
			AutoPutCursorToInventoryOrDrop();
			SetCursorGraphics(CM_1_NORMAL_HAND);
			return true;
		}
		else {
			PlayGlobalSound(S_53_I_INVBOOK);
			PlayerSoundMessageDelay = 10;
			switch (player.ClassID) {
			case PC_4_ROGUE:
			case PC_1_ARCHER:		SoundMessage = S_649_ROG_29;				return true;
			case PC_5_SAVAGE:
			case PC_0_WARRIOR:		SoundMessage = S_758_WAR_29;				return true;
			case PC_2_MAGE:			SoundMessage = S_546_SOR_29;				return true;
			case PC_3_MONK:			SoundMessage = S_866_MONK_29;				return true;
			}
		} break;
	case BI_2457_IRON_FIGURINE:
		if (IMNM || HasTrait(playerIndex, TraitId::Leper)) {
			Quest& iron_Figurine_Quest = Quests[Q_29_IRON_FIGURINE];
			PlayGlobalSound(S_1084);
			ClearInventorySlot(playerIndex, itemIndex);
			iron_Figurine_Quest.status = QS_3_COMPLETE;
			iron_Figurine_Quest.status2 = 2;
			iron_Figurine_Quest.status3 = 2;
			InvGetItem(playerIndex, SpawnUnique(2807 + Difficulty, player.Row, player.Col)); 
			AutoPutCursorToInventoryOrDrop(); 
			SetCursorGraphics(CM_1_NORMAL_HAND); 
			return true;
		} 
		break;
	case BI_2512_FULL_CONTAINERS:
		if (IMNM || HasTrait(playerIndex, TraitId::Leper)) {
			Quest& clW_Quest = Quests[Q_36_CLEAR_WATER];
			int cont;
			if (FindItemInInventory(playerIndex, BI_2512_FULL_CONTAINERS, cont)) {
				FindItemInInventory(playerIndex, BI_2512_FULL_CONTAINERS, cont);
				ClearInventorySlot(playerIndex, cont);
				InvGetItem(playerIndex, SpawnSomething(RngFrom(BI_87_ELIXIR_OF_STRENGTH, BI_88_ELIXIR_OF_MAGIC, BI_89_ELIXIR_OF_DEXTERITY, BI_90_ELIXIR_OF_VITALITY), player.Row, player.Col,0,0,0,0));
				PlayGlobalSound(S_1084);
				AutoPutCursorToInventoryOrDrop();
				SetCursorGraphics(CM_1_NORMAL_HAND);
				return true;
			}
		}
		break;
	case BI_2472_THE_LOCKED_CHEST:
		int the_key, chest;
		if (FindItemInInventory(playerIndex, BI_2456_CHEST_KEY, the_key) && FindItemInInventory(playerIndex, BI_2472_THE_LOCKED_CHEST, chest)) {
			Quest& locked_chest_Quest = Quests[Q_25_THE_LOCKED_CHEST];
			PlayGlobalSound(S_1084);
			FindItemInInventory(playerIndex, BI_2472_THE_LOCKED_CHEST, chest);
			ClearInventorySlot(playerIndex, chest);
			FindItemInInventory(playerIndex, BI_2456_CHEST_KEY, the_key);
			ClearInventorySlot(playerIndex, the_key);
			locked_chest_Quest.status = QS_3_COMPLETE;
			locked_chest_Quest.status = QS_4_TOOWEAK1;
			InvGetItem(playerIndex, SpawnUnique(2843 + Difficulty, player.Row, player.Col));
			AutoPutCursorToInventoryOrDrop();
			SetCursorGraphics(CM_1_NORMAL_HAND);
			int stone_type = by(Difficulty, 301, 300, 299, 298, 297, 296, 296, 296, 296); 
			SpawnSomething(stone_type, player.Row, player.Col, 0, 0, 0, 0);
			return true;
		}
		break;
	case BI_2476_THE_STRANGE_BOOK:
		int crystal_eyepiece, strange_book;
		if (FindItemInInventory(playerIndex, BI_2475_CRYSTAL_EYEPIECE, crystal_eyepiece) 
			&& FindItemInInventory(playerIndex, BI_2476_THE_STRANGE_BOOK, strange_book)) {
			Quest& strange_book_Quest = Quests[Q_33_STRANGE_BOOK];
			PlayGlobalSound(S_1084);
			FindItemInInventory(playerIndex, BI_2476_THE_STRANGE_BOOK, strange_book);
			ClearInventorySlot(playerIndex, strange_book);
			FindItemInInventory(playerIndex, BI_2475_CRYSTAL_EYEPIECE, crystal_eyepiece);
			ClearInventorySlot(playerIndex, crystal_eyepiece);
			strange_book_Quest.status = QS_3_COMPLETE; 
			strange_book_Quest.status = QS_4_TOOWEAK1; 
			int goldAmount = 8'000 + 4'000 * Difficulty; 
			SpawnSomething(BI_0_GOLD, player.Row, player.Col, 0, 0, 0, 0, goldAmount); 
			SetCursorGraphics(CM_1_NORMAL_HAND); 

			//InvGetItem(playerIndex, SpawnUnique(2843 + Difficulty, player.Row, player.Col));
			//AutoPutCursorItemToInventory();
			//SetCursorGraphics(CM_1_NORMAL_HAND);
			// 
			//int stone_type = by(Difficulty, 301, 300, 299, 298, 297, 296, 296, 296, 296);
			//SpawnSomething(stone_type, player.Row, player.Col, 0, 0, 0, 0);
			return true;
		}
		break;
	case BI_2461_MUNDANE_INVENTORY:
		if (IMNM || HasTrait(playerIndex, TraitId::Leper)) {
			Quest& ast_Quest = Quests[Q_32_A_SIMPLE_TASK]; 
			PlayGlobalSound(S_1084); 
			ClearInventorySlot(playerIndex, itemIndex); 
			ast_Quest.status = QS_3_COMPLETE; 
			ast_Quest.status2 = 2; 
			ast_Quest.status3 = 2; 
			InvGetItem(playerIndex, SpawnUnique(mundaneInventoryRewards[Difficulty], player.Row, player.Col));
			AutoPutCursorToInventoryOrDrop(); 
			SetCursorGraphics(CM_1_NORMAL_HAND); 
			return true;
		}
		break;
	case BI_2460_THE_RIGHT_GOODS:
		if (IMNM) {  
			Quest& good_Deal_Quest = Quests[Q_31_GOOD_DEAL];  
			PlayGlobalSound(S_1084);  
			ClearInventorySlot(playerIndex, itemIndex);  
			good_Deal_Quest.status = QS_3_COMPLETE;  
			good_Deal_Quest.status2 = 2;  
			good_Deal_Quest.status3 = 2;  
			int goldAmount = 4'000 + 2'000 * Difficulty;  
			SpawnSomething(BI_0_GOLD, player.Row, player.Col, 0, 0, 0, 0, goldAmount);   
			SetCursorGraphics(CM_1_NORMAL_HAND);
			return true;
		}
		break;
	case BI_2510_DUSTY_CRATE:
		if (IMNM) {
			Quest& iron_Oath_Quest = Quests[Q_35_IRON_OATH];
			PlayGlobalSound(S_1084);
			ClearInventorySlot(playerIndex, itemIndex);
			iron_Oath_Quest.status = QS_3_COMPLETE;
			iron_Oath_Quest.status2 = 2;
			iron_Oath_Quest.status3 = 2;
			int goldAmount = 1'000 + 2'000 * Difficulty;
			SpawnSomething(BI_0_GOLD, player.Row, player.Col, 0, 0, 0, 0, goldAmount);
			SetCursorGraphics(CM_1_NORMAL_HAND);
			return true;
		}
		break;
	case BI_2459_REAGENTS:
		if (IMNM || HasTrait(playerIndex, TraitId::Leper)) {
			Quest& demon_Plague_Quest = Quests[Q_30_DEMON_PLAGUE];
			PlayGlobalSound(S_1084);
			ClearInventorySlot(playerIndex, itemIndex);
			demon_Plague_Quest.status = QS_3_COMPLETE;
			demon_Plague_Quest.status2 = 2;
			demon_Plague_Quest.status3 = 2;
			InvGetItem(playerIndex, SpawnUnique(2816 + Difficulty, player.Row, player.Col));
			AutoPutCursorToInventoryOrDrop();
			SetCursorGraphics(CM_1_NORMAL_HAND);
			return true;
		}
		break;
	case BI_16_ANVIL_OF_FURY:
		if (IMNM || HasTrait(playerIndex, TraitId::Leper)) {
			Quest& anvilOfFuryQuest = Quests[Q_10_ANVIL_OF_FURY];
			PlayGlobalSound(S_1084);
			ClearInventorySlot(playerIndex, itemIndex);
			anvilOfFuryQuest.status = QS_3_COMPLETE;
			anvilOfFuryQuest.status2 = 2;
			anvilOfFuryQuest.status3 = 2;
			PLAYER_FULL_CLASS fullClassId = player.fullClassId;
			if (player.fullClassId == PFC_ROGUE && HasTrait(playerIndex, TraitId::Axepertise)) {
				fullClassId = PFC_SHUGOKI;
			}
			/*else if (player.fullClassId == PFC_IRON_MAIDEN && HasTrait(playerIndex, TraitId::TwoTowers)) {
				fullClassId = PFC_BOMBARDIER;
			}*/
			else if (player.fullClassId == PFC_ELEMENTALIST && HasTrait(playerIndex, TraitId::Mamluk)) {
				fullClassId = PFC_WARRIOR;
			}
			else if (HasTrait(playerIndex, TraitId::Barbarian)) {
				// TODO: may be change to SpawnSomething
				SetPlayerHandItem(player.ItemOnCursor, UniqueItems[AnvilOfFuryUniqsTable[fullClassId][Difficulty]].baseId);
				Item_MakeNonCollisionSeed(CurrentPlayerIndex, player.ItemOnCursor);
				AutoPutCursorToInventoryOrDrop();
				SetCursorGraphics(CM_1_NORMAL_HAND);
				return true;
			}
			else if (HasTrait(playerIndex, TraitId::Ranger)) {
				InvGetItem(playerIndex, SpawnUnique(2549 + Difficulty, player.Row, player.Col)); // first Griswold's Bow (unique for Ranger) is index #2549 in unique table
				AutoPutCursorToInventoryOrDrop();
				SetCursorGraphics(CM_1_NORMAL_HAND);
				return true;
			}
			else if (HasTrait(playerIndex, TraitId::Pistoleer)) {
				InvGetItem(playerIndex, SpawnUnique(2735 + Difficulty, player.Row, player.Col));
				AutoPutCursorToInventoryOrDrop();
				SetCursorGraphics(CM_1_NORMAL_HAND);
				return true;
			}
			InvGetItem(playerIndex, SpawnUnique(AnvilOfFuryUniqsTable[fullClassId][Difficulty], player.Row, player.Col));
			AutoPutCursorToInventoryOrDrop();
			SetCursorGraphics(CM_1_NORMAL_HAND);
			return true;
		} break;
	case BI_33_STAFF_OF_LAZARUS:
		if (IMNM || HasTrait(playerIndex, TraitId::Leper)) {
			Quest& lazarusQuest = Quests[Q_15_ARCHBISHOP_LAZARUS];
			ClearInventorySlot(playerIndex, itemIndex);
			lazarusQuest.status2 = LQS_2_GIVE_LAZARUS_STAFF_TO_CAIN;
			lazarusQuest.status = QS_2_IN_PROGRESS;
			lazarusQuest.talkToFlag = true;
			if (Dungeon == lazarusQuest.dungeonStart
				&& !Dungeon->isQuest
				&& lazarusQuest.status2 >= LQS_2_GIVE_LAZARUS_STAFF_TO_CAIN
				&& (lazarusQuest.status == QS_2_IN_PROGRESS || lazarusQuest.status == QS_3_COMPLETE)) {
				if (lazarusQuest.status3 == LQAS_0 || lazarusQuest.status3 == LQAS_2) {
					int row = lazarusQuest.row;
					int col = lazarusQuest.col;
					CastMissile(row, col, row, col, 0, MI_65_RED_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, 0);
					lazarusQuest.status3 = LQAS_1;
					if (lazarusQuest.status == QS_2_IN_PROGRESS) {
						lazarusQuest.status2 = LQS_3_MAKE_PORTAL_TO_UNHOLY_ALTAR;
					}
				}
			}
			return true;
		} break;
	case BI_334_THEODORE:
		if (IMNM || HasTrait(playerIndex, TraitId::Leper)) {
			Quest& littleGirlQuest = Quests[Q_18_LITTLE_GIRL];
			littleGirlQuest.status = QS_3_COMPLETE;
			PlayGlobalSound(S_1084);
			ClearInventorySlot(playerIndex, itemIndex);

			if (HasTrait(playerIndex, TraitId::Mamluk)) {
				InvGetItem(playerIndex, SpawnUnique(Difficulty + 2852, player.Row, player.Col));
				AutoPutCursorToInventoryOrDrop();
				SetCursorGraphics(CM_1_NORMAL_HAND);
			}
			else {
				InvGetItem(playerIndex, SpawnUnique(CeliaUniqsTable[player.ClassID][Difficulty], player.Row, player.Col));
				AutoPutCursorToInventoryOrDrop();
				SetCursorGraphics(CM_1_NORMAL_HAND);
			}
			/*if (MaxCountOfPlayersInGame != 1)
				NetSendCmdQuest(1, Q_18_LITTLE_GIRL); */
			return true;
		} break;
	}
	if (baseItemIndex >= BI_296_QUARTZ && baseItemIndex <= BI_301_AQUAMARINE && IMNM) {
		Item& cursorItem = player.ItemOnCursor;
		SetPlayerHandItem(cursorItem, BI_0_GOLD);
		Item_MakeNonCollisionSeed(playerIndex, cursorItem);
		cursorItem.amount = BaseItems[baseItemIndex].MaxPrice / 32;
		cursorItem.IsReqMet = true;
		SetGoldViewOnCursor(playerIndex);
		player.TotalGold = CalcTotalGold(playerIndex);
		ClearInventorySlot(playerIndex, itemIndex);
		PlayGlobalSound(S_1287);
		return true;
	}
	return false;
}

//----- (th2) -------------------------------------------------------------
bool __fastcall RightClickOnItem(int playerIndex, int invCellIndex, bool mouse, bool xinput /*= false*/)
{
	Player& player = Players[playerIndex];
	if( /*Berserk*/HasTrait(playerIndex, TraitId::Psychotic) && (player.activeBuffFlag & BF_2_FURY_ACTIVE) && Dungeon != DUN_0_TOWN ){
		return 0;
	}
	
	if( player.notHittableInTransition && !player.CurLife && playerIndex == CurrentPlayerIndex
	 || /*Cur.GraphicsID != CM_1_NORMAL_HAND ||*/ CurrentDialogIndex 
	 || invCellIndex < IS_Inventory ){// Если кликнули на нательный предмет
		return false;
	}
	Item* item;
	uint itemIndex;
	bool isBelt = false;
	if( invCellIndex < IS_50_47_Belt_Start ){
		itemIndex = invCellIndex - IS_Inventory;
		item = &player.InventorySlots[itemIndex];
	}else if( invCellIndex < IS_50_47_Belt_Start + IS_8_Belt_Count ){
		if( TalkPanelMode ){
			return true;
		}
		itemIndex = invCellIndex - IS_50_47_Belt_Start;
		isBelt = true;
		item = &player.BeltInventory[itemIndex];
	}  else{
		return false;
	}

	uint baseItemIndex = item->baseItemIndex;

	if (InteractWithQuestItem(baseItemIndex, playerIndex, itemIndex)) {
		return true;
	}

	if( !BaseItems[baseItemIndex].UseOnceFlag ){
		return false;
	}

	if( item->IsReqMet ){
		if( item->MagicCode == MC_0_NONE && item->ItemCode == IC_11_GOLD ){
			OpenGoldSplitPanel();
			return true;
		}
		IsGoldSplitPanelVisible = false;

		bool isRune = item->MagicCode >= MC_47_RUNE_OF_FIRE && item->MagicCode <= MC_51_RUNE_OF_STONE;
		bool isRelic = item->MagicCode == MC_21_RELIC_NEED_NO_TARGET || item->MagicCode == MC_22_RELIC_NEED_TARGET;
		
		if( isRelic && ! IsSpellEnabledHere(item->SpellIndex, playerIndex) ){
			return true;
		}

		if( item->MagicCode != MC_24_BOOKS && CooldownCheck(playerIndex, item->SpellIndex) || isRune && Dungeon == DUN_0_TOWN ){
			return true;
		}

		switch(item->MagicCode){
		// у всех предметов кроме книги звук использования и укладки в инвентарь одинаковы
		case MC_24_BOOKS:
			PlayGlobalSound(71);
			break; 
		// мор: внизу к предметам прибавились еще и склянки
		case MC_10_ELIXIR_OF_STRENGTH or MC_11_ELIXIR_OF_MAGIC or MC_12_ELIXIR_OF_DEXTERITY or MC_13_ELIXIR_OF_VITALITY or MC_44_SPECTRAL_ELIXIR // elixirs
		  or MC_18_POTION_OF_REJUVENATION or MC_19_POTION_OF_FULL_REJUVENATION // holy waters
		  or MC_2_POTION_OF_FULL_HEALING or MC_3_POTION_OF_HEALING // healing potions
		  or MC_6_POTION_OF_MANA or MC_7_POTION_OF_FULL_MANA // mana potions
		  or MC_30_OIL_OF_SOMETHING or MC_31_OIL_OF_REPAIR_I or MC_32_OIL_OF_REPAIR_II or MC_33_OIL_OF_REPAIR_III
		  or MC_34_OIL_OF_REPAIR_IV or MC_35_OIL_OF_REPAIR_V or MC_36_BLACKSMITH_OIL or MC_37_OIL_OF_FORTITUDE
		  or MC_38_OIL_OF_REPAIR_VI or MC_39_OIL_OF_REPAIR_VII or MC_40_OIL_OF_HARDENING // oils
		  or MC_55_FLASK_OF_FIRE or MC_56_FLASK_OF_LIGHTNING or MC_57_FLASK_OF_ACID or MC_58_FLASK_OF_FIREBOMB // flasks
		  or MC_59_FLASK_OF_LIGHTNINGBOMB or MC_60_FLASK_OF_ACIDBOMB or MC_61_FLASK_OF_ARCANEBOMB:
			PlayGlobalSound(1485);
			break;
		default:
			if( playerIndex == CurrentPlayerIndex ) PlayGlobalSound(Flip_InventorySound[ItemFlipTable[item->GraphicValue]]);
			break;
		}
		if( ! UsePotionOrScroll(playerIndex, item) ) return true;

		// если спэлл айди из списка то не удаляем, обозначаем вещь как используемую с задержкой. Даже так. Если курсор стал картинкой значит не удаляем
		if( isBelt ){
			if (xinput) {
				bool found = false;
				for (int i = 0; i < player.InvItemCount; i++) {
					Item& item2 = player.InventorySlots[i];
					if( item2.ItemCode != IC_M1_NONE 
					 && item2.MagicCode == item->MagicCode 
					 && is(item->MagicCode, MC_2_POTION_OF_FULL_HEALING, MC_3_POTION_OF_HEALING, 
											MC_6_POTION_OF_MANA, MC_7_POTION_OF_FULL_MANA, 
											MC_18_POTION_OF_REJUVENATION, MC_19_POTION_OF_FULL_REJUVENATION) ){
						ClearInventorySlot(playerIndex, i);
						found = true;
						break;
					}
				}
				if (!found) {
					ClearBeltSlot(playerIndex, itemIndex);
				}
			}
			else {
				ClearBeltSlot(playerIndex, itemIndex);
			}
		}else{
			ClearInventorySlot(playerIndex, itemIndex, mouse);
		}
		return true;
	}
	// Если используемый предмет не подходит по статам
	uint soundIndex;
	switch (player.ClassID ){
	case PC_4_ROGUE:
	case PC_1_ARCHER:		soundIndex = S_633_ROG_13;				break;
	case PC_5_SAVAGE:
	case PC_0_WARRIOR:		soundIndex = S_736_WAR_13;				break;
	case PC_2_MAGE:			soundIndex = S_530_SOR_13;				break;
	case PC_3_MONK:			soundIndex = S_850_MONK_13;				break;
	}
	PlayGlobalSound(soundIndex);
	return true;
}

bool TelekinesRecentlyAppliedToMonster = false;
//----- (004230CD) --------------------------------------------------------
void TelekinesApply()
{
	Player& player = Players[CurrentPlayerIndex];
	// если расстояние больше 10 клеток, телекинез тратится в пустую
	// надо сделать отображение превышения расстояния (красный курсор) и возможно сделать несрабатывание
	if( abs( player.Row - Cur.Row ) <= 10 && abs( player.Col - Cur.Col ) <= 10 ){
		if( Cur.ObjectIndex != -1 ){
            NetSendCmdParam1( 1, CMD_27_OPOBJT, Cur.ObjectIndex );
		}else if( Cur.ItemID != -1 ){
            NetSendCmdGItem( 1, CMD_40_REQUESTAGITEM, CurrentPlayerIndex, CurrentPlayerIndex, Cur.ItemID );
		}else if( (Dungeon->genType || Dungeon->isQuest) && CurMon != -1 && !IsAttackTypes_1(CurMon) && !Monsters[CurMon].speechIndex && CurMon >= SummonMonsters_Count ){
            NetSendCmdParam2( 1, CMD_28_KNOCKBACK, CurMon, GameMode == GM_HARD ? 1 : 2 );
			TelekinesRecentlyAppliedToMonster = true;
		}else if ((IsAltPressed || AltHighlight) && HighlightedItem.ItemID != -1)
            NetSendCmdGItem( 1, CMD_40_REQUESTAGITEM, CurrentPlayerIndex, CurrentPlayerIndex, HighlightedItem.ItemID );
	}
	SetCursorGraphics(CM_1_NORMAL_HAND);
}

//----- (0042313F) --------------------------------------------------------
uint __fastcall CalcTotalGold(int playerIndex)
{
	Player& player = Players[playerIndex];
	uint sum = 0;
	for( int itemIndex = 0; itemIndex < IS_8_Belt_Count; itemIndex++ ){
		Item& item = player.BeltInventory[itemIndex];
		if( item.ItemCode == IC_11_GOLD ){
			sum += item.amount;
		}
	}
	for( int itemIndex = 0; itemIndex < player.InvItemCount; ++itemIndex ){
		Item& item = player.InventorySlots[itemIndex];
		if( item.ItemCode == IC_11_GOLD ){
			sum += item.amount;
		}
	}
	return sum;
}

//----- (004231CA) --------------------------------------------------------
int __fastcall MaxItemPrice(Player& player, int itemCodes ) // TODO: change itemCodes to bit mask
{
	int maxPrice = 0;
	for( auto& i: player.OnBodySlots    ) if( i.ItemCode == itemCodes && maxPrice < i.Price ) maxPrice = i.Price;
	for( auto& i: player.InventorySlots ) if( i.ItemCode == itemCodes && maxPrice < i.Price ) maxPrice = i.Price;
	return maxPrice;
}

//----- (00423230) --------------------------------------------------------
int __fastcall GetMaxPriceofPlayerBows( int playerIndex )
{
	int maxPrice = 0;
	Item* item = Players[playerIndex].OnBodySlots;
	for( int i = 0; i < IS_Inventory; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_3_BOW && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	item = Players[playerIndex].InventorySlots;
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_3_BOW && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	return maxPrice;
}

//----- (00423296) --------------------------------------------------------
int __fastcall GetMaxPriceofPlayerStaffs( int playerIndex )
{
	int maxPrice = 0;
	Item* item = Players[playerIndex].OnBodySlots;
	for( int i = 0; i < IS_Inventory; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_10_STAFF && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	item = Players[playerIndex].InventorySlots;
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_10_STAFF && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	return maxPrice;
}

//----- (004232FC) --------------------------------------------------------
int __fastcall GetMaxPriceofPlayerSwords( int playerIndex )
{
	int maxPrice = 0;
	Item* item = Players[playerIndex].OnBodySlots;
	for( int i = 0; i < IS_Inventory; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_1_SWORD && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	item = Players[playerIndex].InventorySlots;
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_1_SWORD && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	return maxPrice;
}

//----- (00423362) --------------------------------------------------------
int __fastcall GetMaxPriceofPlayerHelms( int playerIndex )
{
	int maxPrice = 0;
	Item* item = Players[playerIndex].OnBodySlots;
	for( int i = 0; i < IS_Inventory; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_7_HELM && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	item = Players[playerIndex].InventorySlots;
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_7_HELM && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	return maxPrice;
}

//----- (004233C8) --------------------------------------------------------
int __fastcall GetMaxPriceofPlayerShields( int playerIndex )
{
	int maxPrice = 0;
	Item* item = Players[playerIndex].OnBodySlots;
	for( int i = 0; i < IS_Inventory; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_5_SHIELD && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	item = Players[playerIndex].InventorySlots;
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_5_SHIELD && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	return maxPrice;
}

//----- (0042342E) --------------------------------------------------------
int __fastcall GetMaxPriceofPlayerArmors( int playerIndex )
{
	int maxPrice = 0;
	Item* item = Players[playerIndex].OnBodySlots;
	for( int i = 0; i < IS_Inventory; ++i ){
		int itemCode = item[i].ItemCode;
		bool isArmor = itemCode == IC_6_LIGHT_ARMOR || itemCode == IC_8_MID_ARMOR || itemCode == IC_9_HEAVY_ARMOR;
		if( item[i].TypeID != -1 && isArmor && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	item = Players[playerIndex].InventorySlots;
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		int itemCode = item[i].ItemCode;
		bool isArmor = itemCode == IC_6_LIGHT_ARMOR || itemCode == IC_8_MID_ARMOR || itemCode == IC_9_HEAVY_ARMOR;
		if( item[i].TypeID != -1 && isArmor && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	return maxPrice;
}

//----- (004234B2) --------------------------------------------------------
int __fastcall GetMaxPriceofPlayerMaces( int playerIndex )
{
	int maxPrice = 0;
	Item* item = Players[playerIndex].OnBodySlots;
	for( int i = 0; i < IS_Inventory; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_4_MACE && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	item = Players[playerIndex].InventorySlots;
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_4_MACE && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	return maxPrice;
}

//----- (00423518) --------------------------------------------------------
int __fastcall GetMaxPriceofPlayerAmulets( int playerIndex )
{
	int maxPrice = 0;
	Item* item = Players[playerIndex].OnBodySlots;
	for( int i = 0; i < IS_Inventory; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_13_AMULET && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	item = Players[playerIndex].InventorySlots;
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_13_AMULET && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	return maxPrice;
}

//----- (0042357E) --------------------------------------------------------
int __fastcall GetMaxPriceofPlayerAxes( int playerIndex )
{
	int maxPrice = 0;
	Item* item = Players[playerIndex].OnBodySlots;
	for( int i = 0; i < IS_Inventory; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_2_AXE && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	item = Players[playerIndex].InventorySlots;
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		if( item[i].TypeID != -1 && item[i].ItemCode == IC_2_AXE && maxPrice < item[i].Price ){
			maxPrice = item[i].Price;
		}
	}
	return maxPrice;
}

//----- (004235E4) --------------------------------------------------------
void InitDoublePrice()
{
	DoubleMaxGoldInCell = 2 * MaxGoldInCell;
}

// GetEffLevel
//----- (004235F6) --------------------------------------------------------
int ItemLevelByDungeonLevel()
{
	unsigned int itemLevel; // eax@1

	itemLevel = (unsigned __int8) Dungeon->level;
	if( Difficulty == 1 ){
		itemLevel = (Dungeon->level / 4) + 19;
	}
	else if( Difficulty == 2 ){
		itemLevel = (Dungeon->level / 4) + 25;
	}
	else if (Difficulty >= 3) {
		//Crucible: 32-42, Torment: 42-52,   then: 52-62, 62-72, 72-82, 82-92 (92 is maximum, as it should be, DOUBLE gives max level of cursed affix: 184)
		itemLevel = (5 * Dungeon->level / 12) + (Difficulty * 10) + 2;
	}
	else if( Dungeon->level >= 17 ){
		itemLevel = (Dungeon->level / 2) + 8;
	}
	return itemLevel;
}

//----- (0042361C) --------------------------------------------------------
void LoadFlipItemCELFiles()
{
	unsigned int FileCount; // esi@1
	char FileName[64];      // [sp+4h] [bp-40h]@2
	FileCount = 0;
	do{
		sprintf( FileName, "Items\\%s.CEL", Flip_FileName[ FileCount ] );
		FlipItemCELFiles[ FileCount ] = (char*) LoadFile( FileName );
		++FileCount;
	}while( FileCount <= FL_43_MUSHROOM );

	HANDLE cel;
	do{
		sprintf( FileName, "Items\\%s.CEL", Flip_FileName[ FileCount ] );
		if( !File_Open( FileName, &cel, ONE_TRY) ){
			FlipItemCELFiles[ FileCount ] = (char*) LoadFile( "Items\\cp.cel" );
		}else{
			File_Close(cel);
			FlipItemCELFiles[ FileCount ] = (char*) LoadFile( FileName );
		}
		++FileCount;
	}while( FileCount < FL_68_COUNT );
}

// for dungeon generation item init only
//----- (0042366E) --------------------------------------------------------
bool __fastcall ItemPlace( int row, int col, int playerIndex )
{
	return ! ( MonsterMap[ row ][ col ]
			|| PlayerMap[ row ][ col ]
			|| ItemMap[ row ][ col ]
			|| ObjectsMap[ row ][ col ]
			|| FlagMap[ row ][ col ] & CF_8_FORBIDDEN
			|| TileBlockWalking[FineMap[ row ][ col ]] );
}

//----- (004236BB) --------------------------------------------------------
void AddInitItems()
{
	int qlvl = ItemLevelByDungeonLevel();
	for( int count = RNG(3) + 3; count > 0; --count ){
		int row = 40, col = 40;
		RandomArea(row, col, 1, ItemPlace);
		int itemIndex = -1;
		if( GetItemSpace(row, col, itemIndex, false, ItemPlace) ){
			Item& item = Items[itemIndex];
			item.MakeSeed();
			SetRndSeed( item.seed );
			if( RNG(2) ){
				GetItemAttrs( itemIndex, GameMode == GM_CLASSIC ? BI_2652_POTION_OF_HEAL_CLS : BI_24_POTION_OF_HEALING, qlvl );
			} else{
				GetItemAttrs( itemIndex, GameMode == GM_CLASSIC ? BI_2653_POTION_OF_MANA_CLS : BI_25_POTION_OF_MANA, qlvl );
			}
			item.dropType = D_PREGEN;
			item.quality = qlvl;
			SetupItemAnim( itemIndex );
			item.FrameIndex = item.FramesCount;
			item.playAnimation = 0;
			item.animPhase = 1;
			if( NetItem ) DeltaAddItem( itemIndex );
		}
	}
}

//----- (004237E7) --------------------------------------------------------
void InitItems()
{
	GetItemAttrs(MaxItems_255, 0, 1);
	ItemsCount = 0;
	memcpy(&ItemGoldBuffer, &Items[MaxItems_255], sizeof( Item ));
	ItemGoldBuffer.IsReqMet = true;

	// Обнуление массива ItemsOnGround и массива его индексов MapItemsFreeIndexes
	for( int i = 0; i < MaxItems_255; i++ ){
		Items[i].ItemCode = IC_0_OTHER;
		Items[i].MapRow = 0;
		Items[i].MapCol = 0;
		Items[i].playAnimation = 0;
		Items[i].animPhase = 0;
		Items[i].dropping = 0;
		Items[i].Identified = 0;
	}
	
	memzero(ItemActive);
	for( int i = 0; i < MaxItems_255; i++ ){ ItemAvailable[i] = i; }

	if( !Dungeon->isQuest ){
		GetRndSeed();
		if( IsQuestOnLevel(Q_0_THE_MAGIC_ROCK) ){
			SpawnMagicRock();
		}
		if( IsQuestOnLevel(Q_10_ANVIL_OF_FURY) ){ // Need to make sure Anvil does NOT respawn in MP as player rejoins game
			SpawnSomething(BI_16_ANVIL_OF_FURY, 2 * CurPattern.row + 27, 2 * CurPattern.col + 27, 0, 0, 1, 1);
			if( MaxCountOfPlayersInGame > 1 && Quests[Q_10_ANVIL_OF_FURY].status != QS_3_COMPLETE ){ // In MP, killing Anvil boss or picking up Anvil will set .status = QS_3_COMPLETE, which disables Anvil spawn for other players if it's not yet spawned.  So a player cannot repeatedly join the game and get reward.  If a player did NOT go to Dlvl 10 when quest is done, no Anvil for him, sorry.
				Quest& anvilOfFuryQuest = Quests[Q_10_ANVIL_OF_FURY];
				anvilOfFuryQuest.status = QS_2_IN_PROGRESS;
				anvilOfFuryQuest.status2 = 1;
				anvilOfFuryQuest.status3 = 1;
				anvilOfFuryQuest.talkToFlag = 1;
				NetSendCmdQuest(true, Q_10_ANVIL_OF_FURY);
			}
		}
		if( Dungeon == DUN_20_ABYSS_4 && CowQuestOn ){
			SpawnSomething(BI_46_BONE_MACE, 25, 25, 0, 3, 1, 1); // Костюм кретина
		}
		if( Dungeon == DUN_19_ABYSS_3 && CowQuestOn ){
			SpawnSomething(BI_47_BLUDGEON, 25, 25, 0, 3, 1, 1); // Второй костюм кретина
		}
		#ifdef NOTE_FRAGMENT
		if( Dungeon >= DUN_21_CRYPT_1 && Dungeon <= DUN_23_CRYPT_3 ){
			PutFragmentsOfReconstructedNote();
		}
		#endif
		if( Dungeon != DUN_0_TOWN ){
			AddInitItems(); // теперь кидаем банки на пол во всех подземельях
		}
		#ifdef WTF
		SpawnSomething(BI_42_LIGHT_CANE, 20*2+16, 30*2+16, 0, 0, 0, 1, 1);
		#endif
	}
}

// Кидает фрагмент блокнота (номер зависит от длвл) в случайное доступное место
//----- (0042390F) --------------------------------------------------------
void PutFragmentsOfReconstructedNote()
{
	int baseItemIndex, row, col;
	do{
		row = RNG(80) + 16;
		col = RNG(80) + 16;
	}while( !ItemPlace(row, col) );
	if( Dungeon == DUN_22_CRYPT_2 ){
		baseItemIndex = BI_43_LARGE_MACE; // Torn note 2
	}else if( Dungeon == DUN_23_CRYPT_3 ){
		baseItemIndex = BI_44_WAR_MACE; // Torn note 3
	}else{
		baseItemIndex = BI_42_LIGHT_CANE; // Torn note 1
	}
	SpawnSomething(baseItemIndex, row, col, 0, 0, 1, 1);
}

//----- (th2) -------------------------------------------------------------
PLAYER_FULL_CLASS GetPlayerFullClass( PLAYER_CLASS classId, PLAYER_SUBLASS subclassId, PLAYER_SPECIALIZATION specializationId )
{
    PLAYER_FULL_CLASS fullClassId;

    switch( classId )
    {
    default:// PC_0_WARRIOR
        switch( subclassId ){
        default:/*PSC_WARRIOR_WARRIOR*/     fullClassId = PFC_WARRIOR;		break;
        case PSC_WARRIOR_INQUISITOR:	    fullClassId = PFC_INQUISITOR;	    break;
        case PSC_WARRIOR_GUARDIAN:	        fullClassId = PFC_GUARDIAN;	    break;
        case PSC_WARRIOR_TEMPLAR:	        fullClassId = PFC_TEMPLAR;	        break;
        }
        break;

    case PC_1_ARCHER:
        switch( subclassId ){
        default:/*PSC_ARCHER_ARCHER*/       fullClassId = PFC_ARCHER;		    break;
        case PSC_ARCHER_SCOUT:	            fullClassId = PFC_SCOUT;	        break;
        case PSC_ARCHER_SHARPSHOOTER:	    fullClassId = PFC_SHARPSHOOTER;	break;
        case PSC_ARCHER_TRAPPER:	        fullClassId = PFC_TRAPPER;	        break;
        }
        break;

    case PC_2_MAGE:
        switch( subclassId ){
        default:/*PSC_MAGE_MAGE*/           fullClassId = PFC_MAGE;		        break;
        case PSC_MAGE_ELEMENTALIST:	        fullClassId = PFC_ELEMENTALIST;	    break;
        case PSC_MAGE_SUMMONER:
            switch( specializationId ){
            default:/*PSCL_MAGE_SUMMONER_DEMONOLOGIST*/     fullClassId = PFC_DEMONOLOGIST;		break;
            case PSCL_MAGE_SUMMONER_NECROMANCER:	        fullClassId = PFC_NECROMANCER;	    break;
            case PSCL_MAGE_SUMMONER_BEASTMASTER:	        fullClassId = PFC_BEASTMASTER;	    break;
            }
            break;
        case PSC_MAGE_WARLOCK:	            fullClassId = PFC_WARLOCK;	        break;
        }
        break;

    case PC_3_MONK:
        switch( subclassId ){
        default:/*PSC_MONK_MONK*/           fullClassId = PFC_MONK;		        break;
        case PSC_MONK_KENSEI:	            fullClassId = PFC_KENSEI;	        break;
        case PSC_MONK_SHUGOKI:	            fullClassId = PFC_SHUGOKI;	        break;
        case PSC_MONK_SHINOBI:	            fullClassId = PFC_SHINOBI;	        break;
        }
        break;

    case PC_4_ROGUE:
        switch( subclassId ){
        default:/*PSC_ROGUE_ROGUE*/         fullClassId = PFC_ROGUE;		    break;
        case PSC_ROGUE_ASSASSIN:	        fullClassId = PFC_ASSASSIN;	        break;
        case PSC_ROGUE_IRON_MAIDEN:	        fullClassId = PFC_IRON_MAIDEN;	    break;
        case PSC_ROGUE_BOMBARDIER:	        fullClassId = PFC_BOMBARDIER;	    break;
        }
        break;

    case PC_5_SAVAGE:
        switch( subclassId ){
        default:/*PSC_SAVAGE_SAVAGE*/       fullClassId = PFC_SAVAGE;		    break;
        case PSC_SAVAGE_BERSERKER:	        fullClassId = PFC_BERSERKER;	    break;
        case PSC_SAVAGE_EXECUTIONER:	    fullClassId = PFC_EXECUTIONER;	    break;
        case PSC_SAVAGE_GLADIATOR:
            switch( specializationId ){
            default:/*PSCL_SAVAGE_GLADIATOR_THRAEX*/    fullClassId = PFC_THRAEX;		break;
            case PSCL_SAVAGE_GLADIATOR_MURMILLO:	    fullClassId = PFC_MURMILLO;	    break;
            case PSCL_SAVAGE_GLADIATOR_DIMACHAERUS:	    fullClassId = PFC_DIMACHAERUS;	break;
            case PSCL_SAVAGE_GLADIATOR_SECUTOR:	        fullClassId = PFC_SECUTOR;	    break;
            }
            break;
		case PSC_SAVAGE_DRUID:	    fullClassId = PFC_DRUID;	    break;
		}
        break;
    }

    return fullClassId;
}

//----- (00423965) --------------------------------------------------------
void __fastcall CalcCharParams(int playerIndex, int canLoadAnimFlag)
{
	// Find mana shiled if it is active
	int manaShieldIndex = -1;
	for (int item = 0; item < MissileAmount; item++) {
		int missileIndex = MissileIndexes[item];
		Missile& cast = Missiles[missileIndex];
		if (cast.BaseMissileIndex == MI_13_MANA_SHIELD && cast.CasterIndex == playerIndex) {
			manaShieldIndex = missileIndex;
			break;
		}
	}

	// mana shield fix. Call ManaShieldAction if need before calculations
	// to prevent storing damaged hp in ms code that cause MS HP penetration bugs
	if (manaShieldIndex != -1) {
		ManaShieldAction(manaShieldIndex);
	}
	int minDamage = 0;
	int maxDamage = 0;
	Player& player = Players[playerIndex];
	PLAYER_CLASS classID = player.ClassID;
	int armorClass = 0;                             
	int damagePercent = 0;                          
	int accuracy = 0;                           
	int ACFromItems = 0;                            
	int statAdds[ST_count] = {0};
	int statPercent[ST_count] = {0};
	__int64 staffSpellsSpeedbook = 0;
	int addsDamage = 0;
	int damageFromEnemies = 0;
	int lightRadiusDiv10 = 9;
	if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) { lightRadiusDiv10 = 7; }
	int addsHitPointsBy64 = 0;
	int addsManaBy64 = 0;
	int allSpellLevel = 0;
	int armorPiercing = 0;

	int goldFind = 0;
	int goldAbsFindMin = 0;
	int goldAbsFindMax = 0;
	int magicFind = 0;
	memzero( player.spellExtraLevel );
	memzero( player.spellPercent );
	memzero( player.spellDamageMin );
	memzero( player.spellDamageMax );
	memzero( player.elementPercent );
	memzero( player.elementMinDamage );
	memzero( player.elementMaxDamage );
	memzero( player.elementToHit );
	memzero( player.effectAction ); int actions = 0; // [0].id = EA_NO;
	memzero( player.effectActionValue );
	memzero( player.effectActionPercent );
	player.effectFlag.reset();
	int lifeRegen = 0;
	int manaRegen = 0;
	int lifeRegenPercent = 0;
	int manaRegenPercent = 0;
	int xpGainMin = 0;
	int xpGainMax = 0;
	int xpGainPercent = 0;
	int meleeAbsorbPercentMin = 0;
    int meleeAbsorbPercentMax = 0;
	int arrowAbsorbPercentMin = 0;
    int arrowAbsorbPercentMax = 0;
	int summonHpPercent = 0;
	int summonDamageMin = 0;
	int summonDamageMax = 0;
	int summonDamagePercent = 0;
	int summonAcPercent = 0;
	int summonToHit = 0;
	int summonHp = 0;
	int summonAc = 0;
	int blockChance = 0;
	int critChance = 0;
	int critPercent = 0;
	int critDamageMin = 0;
	int critDamageMax = 0;
	memzero( player.spiciesDamageMin );
	memzero( player.spiciesDamageMax );
	memzero( player.spiciesDamagePercent );
	memzero( player.spiciesToHit );
	memzero( player.spiciesAc );
	memzero( player.spiciesAcPercent );
	memzero( player.spiciesDfe );
	memzero( player.spiciesDamageTotal );
	int lifePercent = 0;
	int manaPercent = 0;
	memzero( player.weaponPercent );
	memzero( player.resist );
	memzero( player.stunThreshold );
	int attackSpeed = 0;
	int castSpeed = 0;
	int hitRecovery = 0;
	int lifeStealPercent = 0;
	int manaStealPercent = 0;

    player.fullClassId = GetPlayerFullClass( player.ClassID, player.subclassID, player.specializationID );

    std::vector<Item*> equippedItems; equippedItems.reserve(size(player.OnBodySlots));
    for( Item& item: player.OnBodySlots ){
		if( item.ItemCode != IC_M1_NONE && item.IsReqMet ) equippedItems.push_back( &item );
    }
    // Add fake items with applied set effects in list of "equippedItems"
    SetItem_GetActiveSetItemEffects( playerIndex, equippedItems );

	for( Item* itemPtr : equippedItems ){
	    Item& item = *itemPtr;
		minDamage += item.MinDamage;
		maxDamage += item.MaxDamage;
		armorClass += item.ArmorClass;
        damageFromEnemies += item.damageFromEnemies;
        meleeAbsorbPercentMin += item.minMeleeAbsorbPercent;
        meleeAbsorbPercentMax += item.maxMeleeAbsorbPercent;
        arrowAbsorbPercentMin += item.minArrowAbsorbPercent;
        arrowAbsorbPercentMax += item.maxArrowAbsorbPercent;
        int minThorns = item.minThorns;
        int maxThorns = item.maxThorns;
		int spell_ID = item.SpellIndex;
		if( spell_ID != 0 ){
			staffSpellsSpeedbook |= 1i64 << ((uchar)spell_ID - 1);
		}
		for(Effect& e : item.effect){
			switch( e.id ){
			case AE_TO_HIT                     : accuracy            += e.chance; break;
			case AE_PERCENT_DAMAGE             : damagePercent       += e.minVal; break;
			case AE_TO_HIT_DAMAGE              : damagePercent       += e.minVal; accuracy += e.chance; break;
			case AE_PERCENT_AC                 : ACFromItems         += e.minVal * item.ArmorClass / 100; break;
			case AE_LOW_DUR_ADD_DAMAGE         : damagePercent       += 2 * e.minVal + 140; break;
			case AE_STAT                       : statAdds[e.type]    += e.minVal; break;
			case AE_ALL_STAT                   : each(statAdds)      += e.minVal; break;
			case AE_ALL_STAT_PERCENT           : each(statPercent)   += e.minVal; break;
			case AE_STAT_PERCENT               : statPercent[e.type] += e.minVal; break;

			case AE_SPICIES_DAMAGE             : player.spiciesDamageMin[e.type]     += e.minVal; player.spiciesDamageMax[e.type] += e.maxVal; break;
			case AE_SPICIES_DAMAGE_PERCENT     : player.spiciesDamagePercent[e.type] += e.minVal; break;
			case AE_SPICIES_TO_HIT             : player.spiciesToHit[e.type]         += e.chance; break;
			case AE_SPICIES_AC                 : player.spiciesAc[e.type]            += e.minVal; break;
			case AE_SPICIES_AC_PERCENT         : player.spiciesAcPercent[e.type]     += e.minVal; break;
			case AE_SPICIES_DFE                : player.spiciesDfe[e.type]           += e.minVal; break;
			case AE_SPICIES_DAMAGE_TOTAL	   : player.spiciesDamageTotal[e.type]    = max(player.spiciesDamageTotal[e.type], e.minVal); break;

			case AE_AC                         : armorClass            += e.minVal; break;
			case AE_RESIST                     : player.resist[e.type] += e.minVal; break;
			case AE_GOLD_ABS_FIND              : goldAbsFindMin        += e.minVal; goldAbsFindMax += e.maxVal; break;
			case AE_XP_GAIN                    : xpGainMin             += e.minVal; xpGainMax += e.maxVal; break;// работает
			case AE_DAMAGE                     : addsDamage            += e.minVal; break;
			case AE_GOLD_FIND                  : goldFind              += e.minVal; break;
			case AE_MAGIC_FIND                 : magicFind             += e.minVal; break;
			case AE_LIFE_REGEN                 : lifeRegen             += e.minVal; break;
			case AE_MANA_REGEN                 : manaRegen             += e.minVal; break;
			case AE_LIFE_REGEN_PERCENT         : lifeRegenPercent      += e.minVal; break;
			case AE_MANA_REGEN_PERCENT         : manaRegenPercent      += e.minVal; break;
			case AE_XP_GAIN_PERCENT	           : xpGainPercent         += e.minVal; break;
			case AE_ABSORB_MELEE_DAMAGE        : addTo(e.minVal, meleeAbsorbPercentMin, meleeAbsorbPercentMax); break;
			case AE_ABSORB_RANGED_DAMAGE       : addTo(e.minVal, arrowAbsorbPercentMin, arrowAbsorbPercentMax); break;

			case AE_ELEM_HIT or AE_ACTION_VALUE: player.effectActionValue  [ e.type ]   += e;
			case AE_FLAG                       : player.effectFlag[ e.type ] = 1;             break;
			case AE_ACTION_PERCENT             : player.effectActionPercent[ e.type ]   += e; break;
			case AE_ACTION                     : player.effectAction       [ actions++ ] = e; break;

			case AE_SPELLS_LEVEL               : allSpellLevel        += e.minVal; break;
			case AE_DFE	                       : damageFromEnemies    += e.minVal; break;
			case AE_HIT_POINTS	               : addsHitPointsBy64    += e.minVal * 64; break;
			case AE_MANA	                   : addsManaBy64         += e.minVal * 64; break;
			case AE_MANA_MOVED_TO_HEALTH       : { int movingValue = 50 * (player.MaxBaseMana / 64) / 100; addsManaBy64 -= movingValue * 64; addsHitPointsBy64 += movingValue * 64; } break;
			case AE_HEALTH_MOVED_TO_MANA       : { int movingValue = 40 * (player.MaxBaseLife / 64) / 100; addsHitPointsBy64 -= movingValue * 64; addsManaBy64 += movingValue * 64; } break;
			case AE_PLUS_LIGHT_RADIUS	       : lightRadiusDiv10     += e.minVal; break;
			case AE_MINUS_LIGHT_RADIUS	       : lightRadiusDiv10     -= e.minVal; break;
			case AE_ARMOR_PIERCING	           : armorPiercing        += e.minVal; break;
			case AE_SUMMON_HP_PERCENT          : summonHpPercent      += e.minVal; break;
			case AE_SUMMON_DAMAGE_PERCENT      : summonDamagePercent  += e.minVal; break;
			case AE_SUMMON_AC_PERCENT          : summonAcPercent      += e.minVal; break;
			case AE_SUMMON_TO_HIT              : summonToHit          += e.chance; break;
			case AE_SUMMON_HP                  : summonHp             += e.minVal; break;
			case AE_SUMMON_DAMAGE              : summonDamageMin      += e.minVal; summonDamageMax += e.maxVal; break;
			case AE_SUMMON_AC                  : summonAc             += e.minVal; break;
			case AE_BLOCK_CHANCE               : blockChance          += e.chance; break;
			case AE_CRIT_CHANCE                : critChance           += e.chance; break;
			case AE_CRIT_DAMAGE_PERCENT        : critPercent          += e.minVal; break;
			case AE_CRIT_PERCENT               : critPercent          += e.minVal; critChance += e.chance; break;
			case AE_CRIT_DAMAGE                : critDamageMin        += e.minVal; critDamageMax += e.maxVal; break;
			case AE_LIFE_PERCENT               : lifePercent          += e.minVal; break;
			case AE_MANA_PERCENT               : manaPercent          += e.minVal; break;
			case AE_THORNS_PERCENT             : minThorns            += minThorns * e.minVal / 100; maxThorns += maxThorns * e.minVal / 100; break;
			case AE_ATTACK_SPEED	           : attackSpeed          = max(attackSpeed, (int)e.minVal); break;
			case AE_CAST_SPEED                 : castSpeed            = max(castSpeed,   (int)e.minVal); break;
			case AE_HIT_RECOVERY               : hitRecovery          = max(hitRecovery, (int)e.minVal); break;
			case AE_LIFE_STEAL_PERCENT	       : lifeStealPercent     = max(lifeStealPercent, (int)e.minVal); break;
			case AE_MANA_STEAL_PERCENT	       : manaStealPercent     = max(manaStealPercent, (int)e.minVal); break;
			case AE_WEAPON_PERCENT             : player.weaponPercent[e.type] += e.minVal; break;
			case AE_RESIST_All                 : for1(ET_COUNT) player.resist[i] += e.minVal; break; // except physical
			case AE_STUN_RESIST                : player.stunThreshold[CT_0_STUN] += e.minVal; break;

			case AE_SPELL_LEVEL                : player.spellExtraLevel [e.type] += e.minVal; break;
			case AE_SPELL_PERCENT              : player.spellPercent    [e.type] += e.minVal; break;
			case AE_SPELL_DAMAGE               : player.spellDamageMin  [e.type] += e.minVal; player.spellDamageMax[e.type] += e.maxVal; break;
			case AE_ELEMENT_PERCENT            : player.elementPercent  [e.type] += e.minVal; break;
			case AE_ELEMENT_DAMAGE             : player.elementMinDamage[e.type] += e.minVal; player.elementMaxDamage[e.type] += e.maxVal; player.elementToHit[e.type] += e.chance; break;

			case AE_CLVL_AC                    : armorClass                     += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_HIT                   : accuracy                       += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_DAMAGE                : addsDamage                     += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_DAMAGE_PERCENT        : damagePercent                  += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_DFE                   : damageFromEnemies              += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_RESIST_ALL            : for1(ET_COUNT)player.resist[i] += e.minVal + player.CharLevel * e.maxVal / 100; break; // except physical
			case AE_CLVL_STAT_ALL              : each(statAdds)                 += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_HP                    : addsHitPointsBy64         += 64 * e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_MANA                  : addsManaBy64              += 64 * e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_MELEE_RES             : addTo(e.minVal + player.CharLevel * e.maxVal / 100, meleeAbsorbPercentMin, meleeAbsorbPercentMax); break;
			case AE_CLVL_ARROW_RES             : addTo(e.minVal + player.CharLevel * e.maxVal / 100, arrowAbsorbPercentMin, arrowAbsorbPercentMax); break;
			case AE_CLVL_STUN_RES              : player.stunThreshold[CT_0_STUN] += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_GOLD                  : goldFind                       += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_XP                    : xpGainPercent                  += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_SPELL_ALL             : fori(PS_COUNT)player.spellPercent[i] += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_RESIST                : player.resist         [e.type] += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_STAT                  : statAdds              [e.type] += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_SPELL_LVL             : player.spellExtraLevel[e.type] += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_ELEM_PERCENT          : player.elementPercent [e.type] += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_SPELL_PERCENT         : player.spellPercent   [e.type] += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_CRIT_CHANCE           : critChance                     += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_CRIT_DAMAGE_PERCENT   : critPercent                    += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_SUMMON_DAMAGE         : addTo(e.minVal + player.CharLevel * e.maxVal / 100, summonDamageMin, summonDamageMax); break;
			case AE_CLVL_SUMMON_DAMAGE_PERCENT : summonDamagePercent            += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_SUMMON_TO_HIT         : summonToHit                    += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_SUMMON_AC             : summonAc                       += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_SUMMON_HP             : summonHp                       += e.minVal + player.CharLevel * e.maxVal / 100; break;
			case AE_CLVL_SUMMON_HP_PERCENT     : summonHpPercent                += e.minVal + player.CharLevel * e.maxVal / 100; break;
			}
		}
		if( minThorns || maxThorns ){
            EffectAction& ea = player.effectAction[ actions++ ];
            ea.id = EFFECT_ACTION::EA_THORNS;
            ea.minVal = minThorns;
            ea.maxVal = maxThorns;
            ea.chance = 100;
        }
	}
	// wine_from_tears fix
	if( player.fullClassId == PFC_IRON_MAIDEN ){
	    int wineFromTears = PerkValue(PERK_WINE_FROM_TEARS, playerIndex);
	    if( wineFromTears > 0 ){
            EffectAction& ea = player.effectAction[actions++];
            ea.id = EFFECT_ACTION::EA_WINE_FROM_TEARS;
            ea.minVal = wineFromTears;
            ea.chance = 100;
        }
    }
	if( actions < 200 ) player.effectAction[actions].id = EA_NO;
	if( player.activeBuffFlag & BF_4_ARMOR_BLESS ){
		armorClass += by( Difficulty, 3, 9, 15, 21, 27, 33, 39, 45, 51 ) + Dungeon->level / 4;
	}
	if (player.activeBuffFlag & BF_14_ACCU_BLESS) {
		accuracy += by( Difficulty, 8, 32, 56, 80, 104, 128, 152, 176, 200 ) + Dungeon->level;
	}
	if( player.activeBuffFlag & BF_5_DAMAGE_BLESS ){
		addsDamage += by( Difficulty, 5, 15, 25, 35, 45, 55, 65, 75, 85 ) + Dungeon->level / 3;
	}
	if (player.activeBuffFlag & BF_6_RESIST_BLESS) {
		for (int element = 1; element < ET_COUNT; ++element) // except physical
			player.resist[element] += 20 + by(Difficulty, 0, 3, 6, 9, 12, 15, 18, 21, 24) + Dungeon->level / 7;
	}
	
	int& classAttrib = (classID == PC_0_WARRIOR || classID == PC_5_SAVAGE) ? statAdds[ST_STRENGTH] : (classID == PC_2_MAGE ? statAdds[ST_MAGIC] : statAdds[ST_DEXTERITY] );
	// #quest level restrictions
	if( Dungeon->isQuest ){
		switch( (DUNGEON)Dungeon ){
		case DUN_38_GLACIAL_CAVERNS:
			armorClass -= 10;
			accuracy -= 10;
			damageFromEnemies += 2;
			break;
		case DUN_39_FROZEN_LOCH:
			armorClass -= 20;
			accuracy -= 20;
			damageFromEnemies += 2;
			break;
		case DUN_40_ISLAND:
			if (!is( GameMode, GM_NIGHTMARE, GM_SPEEDRUN, GM_SURVIVAL, GM_IRONMAN, GM_EASY/*, GM_CLASSIC */)) {
				damageFromEnemies += 2 + (Difficulty * 2);
			}
			break;
		case DUN_41_FLESHDOOM: // fleshdoom patch (12) did
			if (is(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SPEEDRUN, GM_EASY/*, GM_CLASSIC*/)) {
				armorClass -= 10;
				player.resist[ET_1_FIRE] -= 20;
				damageFromEnemies += 2;
				accuracy -= 20;
			}
			else {
				armorClass -= 9 + Difficulty * 9;
				player.resist[ET_1_FIRE] -= 20 + Difficulty * 10;
				damageFromEnemies += 3 + (11 * Difficulty / 4);
				accuracy -= 20 + Difficulty * 5;
			}
			if (classID == PC_2_MAGE && (!HasTrait(playerIndex, TraitId::Mamluk))) { 
				allSpellLevel -= 2; 
			}
			break;
		case DUN_42_WIELDER:
			if (is(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SPEEDRUN, GM_EASY/*, GM_CLASSIC*/)) {
				armorClass -= 12;
				player.resist[ET_1_FIRE] -= 25;
				damageFromEnemies += 4;
				accuracy -= 25;
			}
			else {
				armorClass -= 10 + Difficulty * 10;
				player.resist[ET_1_FIRE] -= 25 + Difficulty * 10;
				damageFromEnemies += 4 + (24 * Difficulty / 8);
				accuracy -= 25 + Difficulty * 5;
			}
			if (classID == PC_2_MAGE && (!HasTrait(playerIndex, TraitId::Mamluk))) { 
				allSpellLevel -= 2; 
			}
			if( lightRadiusDiv10 > 0 ){ 
				lightRadiusDiv10 *= 90; 
				lightRadiusDiv10 /= 100; 
			}
			break;
		case DUN_43_HEPHASTO:
			if (is(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SPEEDRUN, GM_EASY/*, GM_CLASSIC*/)) {
				armorClass -= 15;
				player.resist[ET_1_FIRE] -= 30;
				damageFromEnemies += 5;
				accuracy -= 30;
			}
			else {
				armorClass -= 12 + Difficulty * 11;
				player.resist[ET_1_FIRE] -= 30 + Difficulty * 10;
				damageFromEnemies += 5 + (25 * Difficulty / 8);
				accuracy -= 30 + Difficulty * 5;
			}
			if (classID == PC_2_MAGE && (!HasTrait(playerIndex, TraitId::Mamluk))) { 
				allSpellLevel -= 2; 
			}
			if( lightRadiusDiv10 > 0 ){ 
				lightRadiusDiv10 *= 80; 
				lightRadiusDiv10 /= 100; 
			}
			break;
		case DUN_62_DIM_UNDERCROFT:
			if (lightRadiusDiv10 > 0) {
				lightRadiusDiv10 *= 90;
				lightRadiusDiv10 /= 100;
			}
			break;
		case DUN_48_BLOOD_HIVE:
			if (not(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SPEEDRUN, GM_EASY/*, GM_CLASSIC*/)) {
				armorClass -= 10 + (45 * Difficulty / 4);// -100 on Inferno
				player.resist[ET_1_FIRE] -= 20 + Difficulty * 10;
				player.resist[ET_2_LIGHTNING] -= 20 + Difficulty * 10;
				player.resist[ET_3_ARCAN] -= 20 + Difficulty * 10;
				player.resist[ET_4_ACID] -= 20 + Difficulty * 10;
				player.resist[ET_6_COLD] -= 20 + Difficulty * 10;
				damageFromEnemies += 5 + 5 * Difficulty;
				accuracy -= 20 + Difficulty * 10;
				if (classID == PC_2_MAGE && (!HasTrait(playerIndex, TraitId::Mamluk))) {
					allSpellLevel -= 2;
				}
			}
			break;
		}
	}
	if( minDamage == 0 && maxDamage == 0 ){
		minDamage = 1;
		maxDamage = 1;
		if( player.OnBodySlots[IS_LeftHand].ItemCode == IC_5_SHIELD && player.OnBodySlots[IS_LeftHand].IsReqMet 
		 || player.OnBodySlots[IS_RightHand].ItemCode == IC_5_SHIELD && player.OnBodySlots[IS_RightHand].IsReqMet ){
			maxDamage = 3;
		}
		if( classID == PC_3_MONK ){ // monk base damage with shield and palm..?
			if( player.CharLevel / 2 >= 1 ){
				minDamage = player.CharLevel / 2;
			}
			if( maxDamage <= player.CharLevel ){
				maxDamage = player.CharLevel;
			}
		}
	}
	// TODO: extract to func GetFuryValue and apply to GetDamageOfPlayerSpell
	if( player.activeBuffFlag & BF_2_FURY_ACTIVE ){  // Fury effects on classes, #player Fury, #character Fury
		int charLevel = player.CharLevel; // в th1 uchar, при uint почему то разницы при сверке, разобраться
		int resistAll = 0;
		switch( player.fullClassId ){
		//WARRIORS
		case PFC_WARRIOR:
			blockChance += charLevel / 10 + 5;
			accuracy += charLevel / 3 + 5;
			ACFromItems += charLevel / 4 + 2;
			addsDamage += 7 * charLevel / 4;
			//spellsIncreasedLevels -= 2;
			//damageFromEnemies += charLevel / 9;
			break;
		case PFC_INQUISITOR:
			accuracy += charLevel / 2 - 3;
			ACFromItems += charLevel / 3 - 1;
			addsDamage += 2 * charLevel - 1;
			//spellsIncreasedLevels -= 2;
			break;
		case PFC_GUARDIAN:
			accuracy += charLevel / 2;
			ACFromItems += 4 * charLevel / 5 + 5;
			//addsDamage -= ( charLevel / 5 ) * ( charLevel / 5 );
			//spellsIncreasedLevels -= 3;
			damageFromEnemies -= charLevel / 10;
			arrowAbsorbPercentMin += charLevel / 5;
			arrowAbsorbPercentMax += charLevel / 5;
			break;
		case PFC_TEMPLAR:
			accuracy += charLevel / 5 + 10;
			ACFromItems += charLevel / 9 + 15;
			addsDamage += 2 * charLevel + 1;
			//spellsIncreasedLevels -= charLevel / 10 + 1;
			break;
		//ARCHERS
		case PFC_ARCHER:
			addsDamage += charLevel + 5;
			//accuracy -= charLevel / 2 + 5;
			//ACFromItems -= charLevel / 3 + 3;
			//damageFromEnemies += 2 * charLevel / 7 + 1; // +28% of clvl, + 1
			break;
		case PFC_SCOUT:
			addsDamage += 3 * charLevel / 2 + 3; // 150% of clvl, + 3
			//accuracy -= charLevel / 2;
			//ACFromItems -= charLevel / 3 + 2;
			//damageFromEnemies += 2 * charLevel / 9 + 3;
			break;
		case PFC_SHARPSHOOTER:
			addsDamage += 2 * charLevel - 2;
			accuracy += charLevel / 3 + 7;
			//ACFromItems -= 2 * charLevel / 3;
			//damageFromEnemies += 3 * charLevel / 13 + 3;
			break;
		case PFC_TRAPPER:
			ACFromItems += 2 * charLevel / 3;
			damageFromEnemies -= 2 * charLevel / 9;
			break;
		//MAGES
		case PFC_MAGE:
			allSpellLevel += (charLevel / 15) + 2;
			//ACFromItems -= charLevel / 2;
			//damageFromEnemies += charLevel / 5;
			break;
		case PFC_ELEMENTALIST:
		{
			if (HasTrait(playerIndex, TraitId::Mamluk)) {
				accuracy += charLevel / 5 + 10;
				ACFromItems += charLevel / 9 + 15;
				addsDamage += 2 * charLevel + 1;
			}
			else {

				allSpellLevel += (charLevel / 10) + 2;
			}
			//ACFromItems -= charLevel / 2;
			//damageFromEnemies += charLevel / 4;
		}
			break;
		case PFC_DEMONOLOGIST:
			allSpellLevel += 2;
			summonAc += charLevel / 5;
			//ACFromItems -= charLevel / 2;
			//damageFromEnemies += charLevel / 4;
			break;
		case PFC_NECROMANCER:
			allSpellLevel += 2;
			summonHpPercent += charLevel / 10 + 5;
			//ACFromItems -= charLevel / 2;
			//damageFromEnemies += charLevel / 4;
			break;
		case PFC_BEASTMASTER:
			allSpellLevel += 2;
			summonDamageMin += charLevel / 5;
			summonDamageMax += charLevel / 5 + 1;
			//ACFromItems -= charLevel / 2;
			//damageFromEnemies += charLevel / 4;
			break;
		case PFC_WARLOCK:
			allSpellLevel += 2;
			manaRegen += charLevel / 2 + 1;
			//ACFromItems -= 3 * charLevel / 4; // 0,75 * clvl
			//damageFromEnemies += charLevel / 5 + 3;
			break;
		//MONKS
		case PFC_MONK:
			addsDamage += 2 * charLevel;
			damageFromEnemies -= charLevel / 4;
			resistAll = charLevel / 4;
			//ACFromItems -= charLevel / 2;
			//--spellsIncreasedLevels;
			break;
		case PFC_KENSEI:
			{
			if (MaxCountOfPlayersInGame != 1)
				addsDamage += 3 * charLevel / 2;
			}
			damageFromEnemies -= charLevel / 5;
			resistAll = charLevel / 4;
			//ACFromItems -= charLevel / 3;
			//--spellsIncreasedLevels;
			break;
		case PFC_SHUGOKI:
			addsDamage += 2 * charLevel;
			damageFromEnemies -= charLevel / 5;
			resistAll = charLevel / 4;
			//ACFromItems -= charLevel / 3;
			//--spellsIncreasedLevels;
			break;
		case PFC_SHINOBI:
			addsDamage += 5 * charLevel / 4; // +125% of clvl
			damageFromEnemies -= charLevel / 4;
			resistAll = charLevel / 4;
			//ACFromItems -= charLevel / 2;
			//--spellsIncreasedLevels;
			break;
		//ROGUES
		case PFC_ROGUE:
			ACFromItems += 3 * charLevel / 8; // 3/8 of clvl
			addsDamage += 3 * (charLevel / 2); // 3/2 of clvl
			//damageFromEnemies += charLevel / 4;
			resistAll = charLevel / 4;
			break;
		case PFC_ASSASSIN:
			critChance += charLevel / 10 + 2;
			critDamageMin += 7 * charLevel;
			critDamageMax += 7 * charLevel;
			ACFromItems += charLevel / 5 + 5;
			resistAll = charLevel / 4;
			//arrowAbsorbPercentMin -= charLevel / 2 + 5;
			//arrowAbsorbPercentMax -= charLevel / 2 + 5;
			//spellsIncreasedLevels -= charLevel / 10 + 2;
			break;
		case PFC_IRON_MAIDEN:
			critChance += charLevel / 25 + 4;
			meleeAbsorbPercentMin += charLevel / 5 + 5;
			meleeAbsorbPercentMax += charLevel / 5 + 5;
			//ACFromItems -= charLevel / 2;
			damageFromEnemies -= charLevel / 11 + 4 ;
			//resistAll -= charLevel / 4;
			break;
		case PFC_BOMBARDIER:
			ACFromItems += charLevel / 2;
			//damageFromEnemies += charLevel / 11 + 1;
			resistAll = charLevel / 3;
			break;
		//SAVAGES
		case PFC_SAVAGE:
			//xpGainPercent -= 3 * charLevel / 2; // -1,5 * clvl % to xpGain percent
			addsDamage += 5 * charLevel / 2; // +250% of clvl
			damageFromEnemies -= charLevel / 3;
			accuracy += charLevel / 3;
			//ACFromItems -= charLevel / 3;
			break;
		case PFC_BERSERKER:
			accuracy += 10 + 3 * charLevel / 5 + PerkValue(PERK_BASHING_HITS, playerIndex);
			addsDamage += charLevel * player.BaseStrength / 357 + charLevel / 2 + PerkValue(PERK_BLOODTHIRST, playerIndex);
			ACFromItems += 1 + charLevel / 3 + PerkValue(PERK_LIKE_A_ROCK, playerIndex);
			damageFromEnemies -= 1 + charLevel / 5 + PerkValue(PERK_FEEL_NO_PAIN, playerIndex);
			player.stunThreshold[CT_0_STUN] += 1 + charLevel / 5 + PerkValue(PERK_UNBREAKABLE, playerIndex);
			if (HasTrait(playerIndex, TraitId::Psychotic)) {
				addsDamage += charLevel + 5;
				damageFromEnemies -= (charLevel * charLevel / 250) + charLevel / 6 + 2;
				lifeRegen += 15 + (charLevel / 2);
				accuracy += 10 + (charLevel / 2);
				ACFromItems += 10 + (4 * charLevel / 5);
			}
			break;
		case PFC_EXECUTIONER:
			//spellsIncreasedLevels -= 10 + charLevel / 5;
			addsDamage += 10 + (5 * charLevel / 2);
			damageFromEnemies -= 3 * charLevel / 7 + 4;
			accuracy += charLevel / 3;
			//ACFromItems -= 2 * charLevel / 3;
			lifeRegen += charLevel + 10;
			break;
		case PFC_THRAEX:
			//spellsIncreasedLevels -= 2;
			addsDamage += 2 * charLevel;
			damageFromEnemies -= 2 * charLevel / 9;
			accuracy += 5 * charLevel / 4;
			//ACFromItems -= charLevel / 3;
			break;
		case PFC_MURMILLO:
			//spellsIncreasedLevels -= 4;
			addsDamage += 7 * charLevel / 4;
			damageFromEnemies -= 2 * charLevel / 5;
			//accuracy -= charLevel / 4 - 2;
			ACFromItems += 3 * charLevel / 4;
			break;
		case PFC_DIMACHAERUS:
			//spellsIncreasedLevels -= 3;
			addsDamage += charLevel / 2;
			//damageFromEnemies -= 2 * charLevel / 7;
			accuracy += charLevel / 2;
			//ACFromItems -= charLevel / 2;
			break;
		case PFC_SECUTOR:
			allSpellLevel += 2;
			addsDamage += 2 * charLevel;
			damageFromEnemies -= 3 * charLevel / 10 - 2;
			//accuracy -= charLevel  / 2;
			ACFromItems += charLevel / 10;
			break;
		case PFC_DRUID:
			addsDamage += 5 + 3 * charLevel;
			damageFromEnemies -= 3 * charLevel / 21 + 1;
			accuracy += 10 + charLevel / 2;
			ACFromItems += 5 + charLevel / 10;
			player.stunThreshold[CT_0_STUN] += 3 + charLevel / 15;
			break;
		}
		for( int r = 1; r < ET_COUNT; ++r){ // except physical
		    player.resist[ r ] += resistAll;
        } 
		if ( player.fullClassId == PFC_ARCHER || player.fullClassId == PFC_SCOUT || HasTrait(playerIndex, TraitId::Ranger)) { // triple shot under fury - > only for archer and scout classes!
			player.effectFlag[EA_MULTISHOT] = true;
		}
		else if ( player.fullClassId == PFC_SECUTOR ) {
			hitRecovery = max(PerkValue(PERK_QUICK_RECOVERY, playerIndex), hitRecovery);
		}
		// barbarian improves fhr and as under fury
		else if( player.fullClassId == PFC_SAVAGE && HasTrait(playerIndex, TraitId::Barbarian) ){
			if(      charLevel >= 40 ){ hitRecovery = max(3, hitRecovery); attackSpeed = max(4, attackSpeed); }
			else if( charLevel >= 30 ){ hitRecovery = max(2, hitRecovery); attackSpeed = max(2, attackSpeed); }
			else if( charLevel >= 20 ){ hitRecovery = max(1, hitRecovery); attackSpeed = max(1, attackSpeed); }
		}
	}
	// moved from PM_DoGotHit
	hitRecovery += HasTrait(playerIndex, TraitId::Mamluk) * 2 + PerkValue(PERK_HIT_ME_AGAIN, playerIndex);

	if( player.activeBuffFlag & BF_3_FURY_LETARGY ){
		player.effectFlag[EA_MULTISHOT] = false;
	}

	int quenchDamage = player.effectActionValue[EA_QUENCH_DAMAGE].minVal;
	minDamage += quenchDamage;
	maxDamage += quenchDamage;
	ACFromItems += player.effectActionValue[EA_QUENCH_ARMOR].minVal;
	damagePercent += player.effectActionPercent[EA_QUENCH_DAMAGE].minVal;
	summonDamagePercent += player.effectActionPercent[EA_QUENCH_SUMMON_DAMAGE].minVal;
	int quenchSpellPercent = player.effectActionPercent[EA_QUENCH_SPELL_DAMAGE].minVal;
	for( int i = 0; i < PS_COUNT; ++i ){
		if( Spells[i].Element != ET_NONE && i != PS_36_BONE_SPIRIT ) player.spellPercent[i] += quenchSpellPercent;
	}

	player.MinDamageFromItem = minDamage;
	player.MaxDamageFromItem = maxDamage;
	player.ACFromClass = armorClass;
	player.ItemsAddDamagePercents = damagePercent;
	player.ItemsAddToHitPercents = accuracy;
	// if( ACFromItems == -25 ) __debugbreak();
	player.ACFromItems = ACFromItems;
	player.ItemsAddDamage = addsDamage;

	player.DamageFromEnemy = damageFromEnemies;
	//lightRadiusDiv10 -= 6 + PerkValue(PERK_VEIL_OF_SHADOW, playerIndex);
	LimitToRange(lightRadiusDiv10, 3, 15);//Light radius limit - old 2,15 / new 3,15
	if( player.LightRadius != lightRadiusDiv10 && playerIndex == CurrentPlayerIndex ){
		ChangeLightRadius(player.LightIndex, lightRadiusDiv10);
		// original diablo has minimal visibility == 10
		ChangeVisionRadius(player.lightRadiusIdentificator, lightRadiusDiv10);
		player.LightRadius = lightRadiusDiv10;
	}
	int skilledTraitBenefit = HasTrait(playerIndex, TraitId::Skilled) ? (player.CharLevel + (player.CharLevel * player.CharLevel / 150)) : 0;
	int coinbound_penalty = HasTrait(playerIndex, TraitId::Coinbound) ? (player.CharLevel / 5) : 0;
	player.CurStrength = statAdds[ST_STRENGTH] 
		+ player.BaseStrength 
		+ PerkValue(PERK_GAIN_STRENGTH, playerIndex) 
		+ PerkValue(SYNERGY_STAR_ALIGNMENT, playerIndex) 
		- PerkValue(SYNERGY_INFERNAL_BARGAIN, playerIndex, 2)
		+ skilledTraitBenefit
		- coinbound_penalty;
	player.CurDexterity = statAdds[ST_DEXTERITY] 
		+ player.BaseDexterity 
		+ PerkValue(PERK_GAIN_DEXTERITY, playerIndex) 
		+ PerkValue(SYNERGY_STAR_ALIGNMENT, playerIndex)
		- PerkValue(SYNERGY_INFERNAL_BARGAIN, playerIndex, 2)
		+ skilledTraitBenefit
		- coinbound_penalty;
	player.CurMagic = statAdds[ST_MAGIC] 
		+ player.BaseMagic 
		+ PerkValue(PERK_GAIN_MAGIC, playerIndex) 
		+ PerkValue(SYNERGY_STAR_ALIGNMENT, playerIndex)
		- PerkValue(SYNERGY_INFERNAL_BARGAIN, playerIndex, 2)
		+ skilledTraitBenefit
		- coinbound_penalty;
	player.CurVitality = statAdds[ST_VITALITY] 
		+ player.BaseVitality 
		+ PerkValue(PERK_GAIN_VITALITY, playerIndex) 
		+ PerkValue(SYNERGY_STAR_ALIGNMENT, playerIndex)
		- PerkValue(SYNERGY_INFERNAL_BARGAIN, playerIndex, 2)
		+ skilledTraitBenefit
		- coinbound_penalty;
	player.CurStrength += player.BaseStrength * statPercent[ST_STRENGTH] / 100;
	player.CurDexterity += player.BaseDexterity * statPercent[ST_DEXTERITY] / 100;
	player.CurMagic += player.BaseMagic * statPercent[ST_MAGIC] / 100;
	player.CurVitality += player.BaseVitality * statPercent[ST_VITALITY] / 100;
	LimitToMin(player.CurStrength, 0);
	LimitToMin(player.CurDexterity, 0);
	LimitToMin(player.CurMagic, 0);
	LimitToMin(player.CurVitality, 0);
	
	statAdds[ST_STRENGTH] = player.CurStrength - player.BaseStrength;
	statAdds[ST_DEXTERITY] = player.CurDexterity - player.BaseDexterity;
	statAdds[ST_MAGIC] = player.CurMagic - player.BaseMagic;
	statAdds[ST_VITALITY] = player.CurVitality - player.BaseVitality;

	// ------- character base damage ------------------------------------------- (#player base damage)
	int damageDivider;
	__int64 mainAttribLevel;
	int baseDamage;
	if( classID == PC_2_MAGE ){
		mainAttribLevel = player.CharLevel * player.CurStrength;
		damageDivider = HasTrait(playerIndex, TraitId::Mamluk) ? 60 : 200;
		if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) { // only Mamluk can wield bow anyway..
			damageDivider *= 3;
		}
		baseDamage = (int)(mainAttribLevel / damageDivider);
		player.BaseDamage = baseDamage + (HasTrait(playerIndex, TraitId::Mamluk) ? (player.CharLevel / 2) : 0 );
		if (GameMode == GM_CLASSIC) {
			if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
				damageDivider = 200;
			}
			else {
				damageDivider = 100;
			}
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
		}
		goto LABEL_73;
	}

	else if (player.fullClassId == PFC_WARRIOR) { // 240
		mainAttribLevel = player.CharLevel * player.CurStrength;
		if (GameMode == GM_CLASSIC) {
			if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
				damageDivider = 200;
			}
			else {
				damageDivider = 100;
			}
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
		else {
			if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
				damageDivider = 80; // 150 + 25
				baseDamage = (int)(mainAttribLevel / damageDivider);
				int ranger_boost = HasTrait(playerIndex, TraitId::Ranger) ? player.CharLevel : (player.CharLevel / 2);
				player.BaseDamage = HasTrait(playerIndex, TraitId::GrimDeal) ? baseDamage : baseDamage + ranger_boost;
				goto LABEL_73;
			}
			else {
				damageDivider = 71; // 169 + 25 + 1
				baseDamage = (int)(mainAttribLevel / damageDivider);
				player.BaseDamage = baseDamage + (player.CharLevel / 2) + 1;
				if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_5_SHIELD || player.OnBodySlots[IS_RightHand].ItemCode == IC_5_SHIELD) {
					player.BaseDamage += PerkValue(PERK_COUNTERBALANCE, playerIndex);
				}
				goto LABEL_73;
			}
		}
	}

	else if (player.fullClassId == PFC_INQUISITOR) { // 245
		mainAttribLevel = player.CharLevel * player.CurStrength;
		if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
			damageDivider = 100; // 122 + 25
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2);
			goto LABEL_73;
		}
		else {
			damageDivider = 75; // 163 + 26 + 1
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (13 * player.CharLevel / 25) + 1;
			goto LABEL_73;
		}
	}

	else if (player.fullClassId == PFC_GUARDIAN) { // 250
		mainAttribLevel = player.CharLevel * player.CurStrength;
		if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
			damageDivider = 75; // 166 + 25 + trait
			baseDamage = (int)(mainAttribLevel / damageDivider);
			int xbow_training_dmg = HasTrait(playerIndex, TraitId::CrossbowTraining) ? (3 * player.CurDexterity / 2) : 0;
			player.BaseDamage = baseDamage + (player.CharLevel / 2) + xbow_training_dmg + PerkValue(PERK_CROSSBOW_MASTERY, playerIndex);
			goto LABEL_73;
		}
		else if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_23_PISTOL || player.OnBodySlots[IS_RightHand].ItemCode == IC_23_PISTOL) {
			player.BaseDamage = player.CharLevel * player.CurDexterity / 80 + (player.CharLevel - 1); //0;
			goto LABEL_73;
		}
		else {
			damageDivider = HasTrait(playerIndex, TraitId::Pistoleer) ? 189 : 63; // 198 + 25 + 2
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2) + 2;
			goto LABEL_73;
		}
	}

	else if (player.fullClassId == PFC_TEMPLAR) { // 220
		mainAttribLevel = player.CharLevel * player.CurStrength;
		if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
			damageDivider = 100; // 110 + 30
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (3 * player.CharLevel / 5);
			goto LABEL_73;
		}
		else if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_22_MALLET || player.OnBodySlots[IS_RightHand].ItemCode == IC_22_MALLET) {
			damageDivider = 200; // 55 + 25
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2);
			goto LABEL_73;
		}
		else {
			damageDivider = 68; // 161 + 25
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2);
			goto LABEL_73;
		}
	}

	else if (player.fullClassId == PFC_ARCHER) {
		if (GameMode == GM_CLASSIC) {
			mainAttribLevel = player.CharLevel * (player.CurStrength + player.CurDexterity);
			damageDivider = 200;
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
		else {
			mainAttribLevel = player.CharLevel * player.CurStrength; // 120
			if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
				damageDivider = 125;// 37500 / 125 = 300
				baseDamage = (int)(mainAttribLevel / damageDivider);
				baseDamage += player.CharLevel / 2;
				if (HasTrait(playerIndex, TraitId::GrimDeal)) {
					baseDamage -= (player.CharLevel / 3) * baseDamage / 100;
				}
				player.BaseDamage = baseDamage;
				goto LABEL_73;
			}
			else {
				damageDivider = 1600;
				baseDamage = (int)(mainAttribLevel / damageDivider);
				player.BaseDamage = baseDamage;
				goto LABEL_73;
			}
		}
	}

	else if (player.fullClassId == PFC_SCOUT) {
		mainAttribLevel = player.CharLevel * player.CurStrength; //120
		if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
			damageDivider = 80; // 37500 / 80 = 468
			baseDamage = (int)(mainAttribLevel / damageDivider);
			baseDamage += player.CharLevel / 2;
			 if (HasTrait(playerIndex, TraitId::GrimDeal)) {
				 baseDamage -= (player.CharLevel / 3) * baseDamage / 100;
			 }
			 player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
		else {
			damageDivider = 1740;
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
	}

	else if (player.fullClassId == PFC_SHARPSHOOTER) { //300
		mainAttribLevel = player.CharLevel * player.CurStrength;
		if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
			damageDivider = 112; // 45000 / 112 = 401
			baseDamage = (int)(mainAttribLevel / damageDivider);
			baseDamage += player.CharLevel / 2;
			if (HasTrait(playerIndex, TraitId::GrimDeal)) {
				baseDamage -= (player.CharLevel / 3) * baseDamage / 100;
			}
			player.BaseDamage = baseDamage; 
			goto LABEL_73;
		}
		else {
			damageDivider = 1760;
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
	}

	else if (player.fullClassId == PFC_TRAPPER) {
		mainAttribLevel = player.CharLevel * (player.CurStrength + player.CurDexterity);
		if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
			damageDivider = 33900;
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
		else {
			damageDivider = 33900;
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
	}

	else if(player.fullClassId == PFC_MONK){
		mainAttribLevel = player.CharLevel * player.CurStrength; // 400 max
		if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_10_STAFF) {
			damageDivider = 65; // 923 + 75
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2);
			goto LABEL_73;
		}
		else if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
			damageDivider = 100; // 600 + 50
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 3);
			goto LABEL_73;
		}
		else {
			damageDivider = 600;
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
	}

	else if (player.fullClassId == PFC_KENSEI) { // 170
		mainAttribLevel = player.CharLevel * player.CurStrength;
		if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
			damageDivider = 115; // 50 + 16
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 3);
			goto LABEL_73;
		}
		else if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD || player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE){
			damageDivider = 50; // 170 + 25
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2);
			goto LABEL_73;
		}
		else {
			damageDivider = 850; //
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
	}

	else if (player.fullClassId == PFC_SHUGOKI) {
		mainAttribLevel = player.CharLevel * player.CurStrength; // 500 max
		if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
			damageDivider = 100; // 750 + 50
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 3);
			goto LABEL_73;
		}
		else if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_2_AXE){
			damageDivider = 45; // 1666 + 75
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2);
			goto LABEL_73;
		}
		else {
			damageDivider = 850; // ?
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
	}

	else if (player.fullClassId == PFC_SHINOBI) {
		mainAttribLevel = player.CharLevel * player.CurStrength; // 150
		if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
			damageDivider = 100; // 75 + 16
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 3);
			goto LABEL_73;
		}
		else if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_21_KNIFE) {
			damageDivider = 150; // 75 + 15
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (3 * player.CharLevel / 10);
			goto LABEL_73;
		}
		else if ((player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD || player.OnBodySlots[IS_RightHand].ItemCode == IC_1_SWORD )
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE || player.OnBodySlots[IS_RightHand].ItemCode == IC_4_MACE )) {
			damageDivider = 50; // 150 + 25
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2);
			goto LABEL_73;
		}
		else {
			damageDivider = 600;
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
	}	

	else if (player.fullClassId == PFC_ROGUE) {
		mainAttribLevel = player.CharLevel * player.CurStrength; // 130
		if ((player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD && player.OnBodySlots[IS_RightHand].ItemCode == IC_4_MACE)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE && player.OnBodySlots[IS_RightHand].ItemCode == IC_1_SWORD)) {
			damageDivider = 100; // 65 + 25
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2);
			goto LABEL_73;
		}
		else if ((player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD && player.OnBodySlots[IS_RightHand].ItemCode == IC_1_SWORD)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE && player.OnBodySlots[IS_RightHand].ItemCode == IC_4_MACE)) {
			damageDivider = 50; // 130 + 25
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2);
			goto LABEL_73;
		}
		else if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_2_AXE) { // axepertise trait
			damageDivider = 33; // 196 + 50
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + player.CharLevel;
			goto LABEL_73;
		}
		else {
			damageDivider = 56000;
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
	}

	else if ( player.fullClassId == PFC_ASSASSIN ) { // 130
		mainAttribLevel = player.CharLevel * player.CurStrength;
		if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_20_CLAW || player.OnBodySlots[IS_RightHand].ItemCode == IC_20_CLAW) {
			damageDivider = 65; // 100 + 25
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2);
			goto LABEL_73;
		}
		else if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_21_KNIFE || player.OnBodySlots[IS_RightHand].ItemCode == IC_21_KNIFE) {
			damageDivider = 130; // 50 + 25
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2);
			goto LABEL_73;
		}
		else {
			damageDivider = 325;
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
	}


	else if (player.fullClassId == PFC_IRON_MAIDEN) { // 160
		mainAttribLevel = player.CharLevel * player.CurStrength;
		if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
			damageDivider = 80; // 100 + 25
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2);
			goto LABEL_73;
		}
		else {
			damageDivider = 80; // 100
			baseDamage = (int)(mainAttribLevel / damageDivider);
			if (HasTrait(playerIndex, TraitId::TwoTowers)) {
				baseDamage += player.CharLevel + 1;
			}
			player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
	}

	else if (player.fullClassId == PFC_BOMBARDIER) {
		mainAttribLevel = player.CharLevel * player.CurStrength;
		damageDivider = 55800;
		baseDamage = (int)(mainAttribLevel / damageDivider);
		player.BaseDamage = baseDamage;
		goto LABEL_73;
	}

	// savages
	else if (player.fullClassId == PFC_SAVAGE) {
		mainAttribLevel = player.CharLevel * player.CurStrength; // 250
		if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
			damageDivider = 83; // 150
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
		else if ((player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD && player.OnBodySlots[IS_LeftHand].EquippedLocation == EL_2_TWO_HANDED)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE && player.OnBodySlots[IS_LeftHand].EquippedLocation == EL_2_TWO_HANDED)) {			
			damageDivider = HasTrait(playerIndex, TraitId::Barbarian) ? 40 :  61; // basic: 1967
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (23 * player.CharLevel / 104);// 33 at clvl-150
			goto LABEL_73;
		}
		else {
			damageDivider = 125; // 100
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
	}

	else if (player.fullClassId == PFC_BERSERKER) {
		mainAttribLevel = player.CharLevel * player.CurStrength; // 250
		if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
			damageDivider = 100; // 125 + 25
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2);
			goto LABEL_73;
		}
		else if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_2_AXE) {
			damageDivider = 84; // 148 + 25
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2);
			goto LABEL_73;
		}
		else {
			damageDivider = 250;
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
	}

	else if (player.fullClassId == PFC_EXECUTIONER) {
		mainAttribLevel = player.CharLevel * player.CurStrength;
		player.ACFromClass += player.CharLevel / 2;
		if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
			damageDivider = 100; // 125 + 25
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2);
			goto LABEL_73;
		}
		else if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_2_AXE) {
			damageDivider = 54; // 250 + 25
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2);
			goto LABEL_73;
		}
		else if ( (player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE && player.OnBodySlots[IS_RightHand].ItemCode == IC_M1_NONE)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_M1_NONE && player.OnBodySlots[IS_RightHand].ItemCode == IC_4_MACE)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_M1_NONE && player.OnBodySlots[IS_RightHand].ItemCode == IC_1_SWORD)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD && player.OnBodySlots[IS_RightHand].ItemCode == IC_M1_NONE) ) {
			damageDivider = 45; // 347 + 50
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + player.CharLevel;
			goto LABEL_73;
		}
		else {
			damageDivider = 34; // 403 + 5?
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + 5;
			if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) {
				player.BaseDamage += 5 + ((player.CharLevel * player.CharLevel) / 20) + (31 * player.CharLevel / 50);
			}
			goto LABEL_73;
		}
	}

	else if (player.fullClassId == PFC_THRAEX) {
		mainAttribLevel = player.CharLevel * player.CurStrength; // 200
		if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
			damageDivider = 80; // 125 + 25
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2);
			goto LABEL_73;
		}
		else if ((player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD && player.OnBodySlots[IS_RightHand].ItemCode == IC_5_SHIELD)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_5_SHIELD && player.OnBodySlots[IS_RightHand].ItemCode == IC_1_SWORD)) {
			damageDivider = 65; // ~150
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2);
			goto LABEL_73;
		}
		else if ((player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE && player.OnBodySlots[IS_RightHand].ItemCode == IC_5_SHIELD)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_5_SHIELD && player.OnBodySlots[IS_RightHand].ItemCode == IC_4_MACE)) {
			damageDivider = 75;
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
		else if ((player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD && player.OnBodySlots[IS_RightHand].ItemCode == IC_M1_NONE)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_M1_NONE && player.OnBodySlots[IS_RightHand].ItemCode == IC_1_SWORD)) {
			damageDivider = 65;
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage + (player.CharLevel / 2);
			goto LABEL_73;
		}
		else if ((player.OnBodySlots[IS_LeftHand].ItemCode == IC_M1_NONE && player.OnBodySlots[IS_RightHand].ItemCode == IC_4_MACE)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE && player.OnBodySlots[IS_RightHand].ItemCode == IC_M1_NONE)) {
			damageDivider = 75;
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
		else {
			damageDivider = 750;
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
			goto LABEL_73;
		}
	}

	else if (player.fullClassId == PFC_MURMILLO) {
		mainAttribLevel = player.CharLevel * player.CurStrength; // 230
		if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
				damageDivider = 115; // 100 + 25
				baseDamage = (int)(mainAttribLevel / damageDivider);
				player.BaseDamage = baseDamage + (player.CharLevel / 2);
				if (HasTrait(playerIndex, TraitId::Crupellarius)) { 
					player.BaseDamage /= 2; 
				}
				goto LABEL_73;
		}
		else if ((player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD && player.OnBodySlots[IS_RightHand].ItemCode == IC_5_SHIELD)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE && player.OnBodySlots[IS_RightHand].ItemCode == IC_5_SHIELD)) {
				damageDivider = 65; // 176 + 25
				baseDamage = (int)(mainAttribLevel / damageDivider);
				player.BaseDamage = baseDamage + (player.CharLevel / 2);
				if (HasTrait(playerIndex, TraitId::Crupellarius)) {
					player.BaseDamage /= 2;
				}
				goto LABEL_73;
		}
		else if ((player.OnBodySlots[IS_LeftHand].ItemCode == IC_5_SHIELD && player.OnBodySlots[IS_RightHand].ItemCode == IC_1_SWORD)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_5_SHIELD && player.OnBodySlots[IS_RightHand].ItemCode == IC_4_MACE)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD && player.OnBodySlots[IS_RightHand].ItemCode == IC_M1_NONE)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE && player.OnBodySlots[IS_RightHand].ItemCode == IC_M1_NONE)) {
				damageDivider = 100;
				baseDamage = (int)(mainAttribLevel / damageDivider);
				player.BaseDamage = baseDamage;
				if (HasTrait(playerIndex, TraitId::Crupellarius)) {
					player.BaseDamage /= 2;
				}
				goto LABEL_73;
		}
		else if ((player.OnBodySlots[IS_LeftHand].ItemCode == IC_M1_NONE && player.OnBodySlots[IS_RightHand].ItemCode == IC_4_MACE) 
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_M1_NONE && player.OnBodySlots[IS_RightHand].ItemCode == IC_1_SWORD)) {
				damageDivider = 145;
				baseDamage = (int)(mainAttribLevel / damageDivider);
				player.BaseDamage = baseDamage;
				if (HasTrait(playerIndex, TraitId::Crupellarius)) {
					player.BaseDamage /= 2;
				}
				goto LABEL_73;
		}
		else {
			damageDivider = 750;
			baseDamage = (int)(mainAttribLevel / damageDivider);
			player.BaseDamage = baseDamage;
			if (HasTrait(playerIndex, TraitId::Crupellarius)) {
				player.BaseDamage /= 2;
			}
			goto LABEL_73;
		}
	}
	
	else if (player.fullClassId == PFC_DIMACHAERUS) {
		mainAttribLevel = player.CharLevel * player.CurStrength; // 190
		if ((player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD && player.OnBodySlots[IS_RightHand].ItemCode == IC_4_MACE)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE && player.OnBodySlots[IS_RightHand].ItemCode == IC_1_SWORD)) {
				damageDivider = 75; // 126 + 25
				baseDamage = (int)(mainAttribLevel / damageDivider);
				player.BaseDamage = baseDamage + (player.CharLevel / 2);
				goto LABEL_73;
		}
		else if ((player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD && player.OnBodySlots[IS_RightHand].ItemCode == IC_1_SWORD)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE && player.OnBodySlots[IS_RightHand].ItemCode == IC_4_MACE)) {
				damageDivider = 59; // 161 + 25
				baseDamage = (int)(mainAttribLevel / damageDivider);
				player.BaseDamage = baseDamage + (player.CharLevel / 2);
				goto LABEL_73;
		}
		else if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
				damageDivider = 76; // 125 + 25
				baseDamage = (int)(mainAttribLevel / damageDivider);
				player.BaseDamage = baseDamage + (player.CharLevel / 2);
				goto LABEL_73;
		}
		else {
				damageDivider = 755;
				baseDamage = (int)(mainAttribLevel / damageDivider);
				player.BaseDamage = baseDamage;
				goto LABEL_73;
		}
	}

	else if (player.fullClassId == PFC_SECUTOR) {
		mainAttribLevel = player.CharLevel * player.CurStrength; // 180
		if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW) {
				damageDivider = 90; // 100 + 25
				baseDamage = (int)(mainAttribLevel / damageDivider);
				player.BaseDamage = baseDamage + (player.CharLevel / 2);
				goto LABEL_73;
		}
		else if ((player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD && player.OnBodySlots[IS_RightHand].ItemCode == IC_5_SHIELD)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE && player.OnBodySlots[IS_RightHand].ItemCode == IC_5_SHIELD)) {
				damageDivider = 52; // 173 + 25
				baseDamage = (int)(mainAttribLevel / damageDivider);
				player.BaseDamage = baseDamage + (player.CharLevel / 2);
				goto LABEL_73;
		}
		else if ((player.OnBodySlots[IS_LeftHand].ItemCode == IC_5_SHIELD && player.OnBodySlots[IS_RightHand].ItemCode == IC_1_SWORD)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_5_SHIELD && player.OnBodySlots[IS_RightHand].ItemCode == IC_4_MACE)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD && player.OnBodySlots[IS_RightHand].ItemCode == IC_M1_NONE)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE && player.OnBodySlots[IS_RightHand].ItemCode == IC_M1_NONE)) {
				damageDivider = 90;
				baseDamage = (int)(mainAttribLevel / damageDivider);
				player.BaseDamage = baseDamage;
				goto LABEL_73;
		}
		else if ((player.OnBodySlots[IS_LeftHand].ItemCode == IC_M1_NONE && player.OnBodySlots[IS_RightHand].ItemCode == IC_4_MACE)
			|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_M1_NONE && player.OnBodySlots[IS_RightHand].ItemCode == IC_1_SWORD)) {
				damageDivider = 135;
				baseDamage = (int)(mainAttribLevel / damageDivider);
				player.BaseDamage = baseDamage;
				goto LABEL_73;
		}
		else {
				damageDivider = 750;
				baseDamage = (int)(mainAttribLevel / damageDivider);
				player.BaseDamage = baseDamage;
				goto LABEL_73;
		}
	}
	else if (player.fullClassId == PFC_DRUID) {
		mainAttribLevel = player.CharLevel * player.CurStrength;
		damageDivider = 80;
		baseDamage = (int)(mainAttribLevel / damageDivider);
		player.BaseDamage = baseDamage + player.CharLevel + 1;
		goto LABEL_73;
	}


	// THIS SAVAGE CHUNK SEEMS TO BE TOTALLY SKIPPED! COULD REQUIRE DELETION
	// ------- additional Savage features-----------------------------------------

	if( player.OnBodySlots[IS_LeftHand].ItemCode == IC_5_SHIELD ){
		player.ACFromClass += player.OnBodySlots[ IS_LeftHand ].ArmorClass / -2;
	}else if( player.OnBodySlots[ IS_RightHand ].ItemCode != IC_5_SHIELD ){
		if( player.OnBodySlots[IS_LeftHand].ItemCode == IC_2_AXE
		 || player.OnBodySlots[ IS_RightHand ].ItemCode == IC_2_AXE ){
			player.BaseDamage += player.CharLevel * player.CurVitality / 65;
		}
	}else if( player.OnBodySlots[ IS_LeftHand ].ItemCode == IC_5_SHIELD ){
		player.ACFromClass += player.OnBodySlots[ IS_LeftHand ].ArmorClass / -2;
	}else if( player.OnBodySlots[ IS_RightHand ].ItemCode == IC_5_SHIELD ){
		player.ACFromClass += player.OnBodySlots[ IS_RightHand ].ArmorClass / -2;
	}
	player.ACFromClass += player.CharLevel / 2;
	
	// --------------- LAST LINE FOR SAVAGES  -------------------------------------


LABEL_73:
	player.AvailableChargesMask = staffSpellsSpeedbook;
	if( player.SpellType == SO_3_EQUIPED_ITEM && !( player.AvailableChargesMask & (1i64 << (player.CurrentSpellIndex - 1))) ){
		player.CurrentSpellIndex = -1;
		player.SpellType = SO_4_NONE;
	}
	int heavyHandedTraitPenalty = HasTrait(playerIndex, TraitId::HeavyHanded) ? (1 + player.CharLevel / 26) : 0;
	int prodigy_trait_penalty = HasTrait(playerIndex, TraitId::Prodigy) ? (player.CharLevel / 18) : 0;
	int mana_flux_trait_boost = HasTrait(playerIndex, TraitId::ManaFlux) ? (1 + 2 * player.CharLevel / 21) : 0;
	player.allSpellExtraLevel = allSpellLevel + PerkValue(PERK_SPELL_MASTERY, playerIndex)
								- heavyHandedTraitPenalty
								- prodigy_trait_penalty
								+ mana_flux_trait_boost;
	player.armorPiercing = armorPiercing;

	int avatar_of_cold_trait = HasTrait(playerIndex, TraitId::AvatarOfCold) ? 25 : 0;
	int natty_rez = HasTrait(playerIndex, TraitId::Stoneform) ? 0 : (player.CurVitality / 10);
	if (GameMode == GM_CLASSIC) natty_rez = 0;
	player.resist[ET_3_ARCAN] += natty_rez + PerkValue(PERK_MAGIC_INSENSITIVITY, playerIndex);
	player.resist[ET_1_FIRE] += natty_rez - avatar_of_cold_trait + PerkValue(PERK_FIRE_INSENSITIVITY, playerIndex);
	player.resist[ET_2_LIGHTNING] += natty_rez + PerkValue(PERK_STATIC_INSENSITIVITY, playerIndex);
	player.resist[ET_6_COLD] += natty_rez + avatar_of_cold_trait + PerkValue(PERK_COLD_INSENSITIVITY, playerIndex);
	player.resist[ET_4_ACID] += natty_rez + PerkValue(PERK_ACID_INSENSITIVITY, playerIndex);
	player.resist[ET_0_PHYSICAL] += PerkValue(SYNERGY_RESIST_CHAOS, playerIndex);
	if (HasTrait(playerIndex, TraitId::FastMetabolism)) {
		player.resist[ET_4_ACID] -= player.CharLevel;
	}
    int all_elemental_resistances = PerkValue(PERK_RESISTANCE, playerIndex);	
	int NK_res_all = player.gameChanger & BIT(GC_9_NIGHT_KIN) ? player.CharLevel : 0;
	all_elemental_resistances += NK_res_all;
	if (HasTrait(playerIndex, TraitId::ChromaticSkin)) {
		all_elemental_resistances += player.CharLevel * 50 / 100;
	}
	if (HasTrait(playerIndex, TraitId::ResoluteGuard)) {
		all_elemental_resistances += player.CharLevel * 60 / 100;
	}
	if (HasTrait(playerIndex, TraitId::Abnegation)) {
		all_elemental_resistances += player.CharLevel / 3;
	}
	all_elemental_resistances += PerkValue(SYNERGY_IMPEDANCE, playerIndex, 0) + PerkValue(SYNERGY_NEGATION, playerIndex);
	for (int i = ET_1_FIRE; i < ET_COUNT; ++i) { player.resist[i] += all_elemental_resistances; }
    if( Dungeon->isQuest && Dungeon == DUN_36_VALLEY_OF_DESPAIR && !(player.gameChanger & BIT(GC_9_NIGHT_KIN)) ){
		int minusRes = 20 + Difficulty * 5;
		for( auto& r : player.resist ) r -= minusRes;
	}
	if (HasTrait(playerIndex, TraitId::Survivor)) {
		for (auto& r : player.resist) r -= player.CharLevel;
	}
	if( player.effectFlag[EA_LOSE_RESISTANCE] ){
		for( auto& r : player.resist ) r = 0;
	}
	if (HasTrait(playerIndex, TraitId::Barbarism)) {
		player.resistPercentLimit = 0.90f;
	}
	else if (HasTrait(playerIndex, TraitId::BlisteredSkin)) {
		player.resistPercentLimit = 0.75f;
	}
	else {
		player.resistPercentLimit = 0.85f;
	}
	player.resistPercentLimit += (0.01f * PerkValue(SYNERGY_IMPEDANCE, playerIndex, 1))
		+ (0.01f * PerkValue(SYNERGY_ELEM_BARRIER, playerIndex));
	// life gains from +VIT items
	int addHP = (PerkValue(SYNERGY_WILL_TO_ENDURE, playerIndex) + 100)* statAdds[ST_VITALITY] / 100;
	if (GameMode == GM_CLASSIC) {
		if (player.fullClassId == PFC_WARRIOR) { addHP *= 2; }
		else if (player.fullClassId == PFC_ARCHER) { addHP += addHP/2; }
	}
	//switch( classID ){
	//case PC_5_SAVAGE:	//addHP = addsVitality + 2 * addsVitality; break;
	//case PC_0_WARRIOR:	//addHP = 2 * addsVitality + (addsVitality >> 1); break;
	//case PC_4_ROGUE:	//addHP = 2 * addsVitality; break;
	//case PC_3_MONK:		//addHP = 2 * addsVitality + (2 * addsVitality >> 2); break;
	//case PC_1_ARCHER:	//addHP = addsVitality + (addsVitality >> 1); break;
	//case PC_2_MAGE:		addHP = addsVitality; break;
	//}
	addsHitPointsBy64 += addHP << 6;
	addsHitPointsBy64 += (
		  PerkValue(PERK_ENDURANCE, playerIndex) 
		+ PerkValue(SYNERGY_REJUVENATION, playerIndex, 0) 
		+ PerkValue(SYNERGY_LONGEVITY, playerIndex, 0)
		+ PerkValue(SYNERGY_INTREPIDITY, playerIndex)
		) << 6;
	// mana gains from +MAG items
	int addMana = (PerkValue(SYNERGY_WILL_TO_ENDURE, playerIndex) + 100) * statAdds[ST_MAGIC] / 100;
	if (GameMode == GM_CLASSIC) {
		if (player.fullClassId == PFC_MAGE) { addMana *= 2; }
		else if (player.fullClassId == PFC_ARCHER) { addMana += addMana / 2; }
	}
	//switch( classID ){
	//case PC_2_MAGE:	addMana = 2 * addsMagic; break;
	//case PC_1_ARCHER:
	//case PC_3_MONK: addMana += addMana >> 1; break;
	//case PC_4_ROGUE: addMana = 2 * addMana + (addMana >> 1); break; // 2,5 to Rogues when Mages have +2 ??? I need to revise it..
	//}
	addsManaBy64 += addMana << 6;
	addsManaBy64 += (
		  PerkValue(PERK_WILLPOWER, playerIndex) 
		+ PerkValue(SYNERGY_MANA_FLOW, playerIndex, 0) 
		+ PerkValue(SYNERGY_AWAKENING, playerIndex, 0)
		+ PerkValue(SYNERGY_SAPIENCE, playerIndex)
		) << 6;
	addsManaBy64 -= (PerkValue(SYNERGY_VALOR, playerIndex, 2) << 6);
	int weirdTraitBenefit = HasTrait(playerIndex, TraitId::Weird) ? ((player.CharLevel * 2) << 6) : 0;
	//player.CurLife = addsHitPointsBy64 + player.BaseLife + weirdTraitBenefit;
	player.MaxCurLife = addsHitPointsBy64 + player.MaxBaseLife + weirdTraitBenefit;
	// действие life процентов
	// если надо учитывать прибавку Vitality c вещей, то раскомментировать ниже (addHP << 6) + 
	int adventurer_trait_penalty = HasTrait(playerIndex, TraitId::Adventurer) ? 20 : 0;
	int vigorous_trait_bonus = HasTrait(playerIndex, TraitId::Vigorous) ? 35 : 0;
	int grim_deal_trait_penalty_lifemana = HasTrait(playerIndex, TraitId::GrimDeal) ? (2 * player.CharLevel / 5) : 0;
	int sisyphean_trait_benefit = HasTrait(playerIndex, TraitId::Sisyphean) ? (2 * player.CharLevel << 6) : 0;
	int barbarian_war_paint = PerkValue(PERK_WAR_PAINT, playerIndex);
	int addLifePercents = ( /*(addHP << 6) +*/ player.MaxBaseLife) 
		* (lifePercent - adventurer_trait_penalty - grim_deal_trait_penalty_lifemana + barbarian_war_paint + vigorous_trait_bonus - PerkValue(SYMP_MALAISE, playerIndex) )
		/ 100;
	//player.CurLife += addLifePercents;
	player.MaxCurLife += addLifePercents + sisyphean_trait_benefit;
	LimitToMin(player.MaxCurLife, 64); 
	if( player.CurLife > player.MaxCurLife ){
		player.CurLife = player.MaxCurLife;
	}
	if( playerIndex == CurrentPlayerIndex && (player.CurLife & ~63) <= 0 ){
		SetPlayerHitPoints(playerIndex, 64);
	}
	int giftedTraitPenalty = HasTrait(playerIndex, TraitId::Gifted) ? ((player.CharLevel * 2) << 6) : 0;
	int necropathy_trait_penalty = HasTrait(playerIndex, TraitId::Necropathy) ? (player.CharLevel << 6) : 0;
	//player.CurMana = addsManaBy64 + player.BaseMana - giftedTraitPenalty;
	player.MaxCurMana = addsManaBy64
					  + player.MaxBaseMana
					  - giftedTraitPenalty
					  - necropathy_trait_penalty;
	// действие мана процентов
	// если надо учитывать прибавку Magic c вещей, то раскомментировать ниже (addMana << 6) + 
	int mana_flux_trait_penalty = HasTrait(playerIndex, TraitId::ManaFlux) ? 20 : 0;
	int addManaPercents = ( /*(addMana << 6) +*/ player.MaxBaseMana) 
		* (manaPercent - adventurer_trait_penalty - mana_flux_trait_penalty - PerkValue(SYMP_PALLOR, playerIndex) - grim_deal_trait_penalty_lifemana) / 100;
	//player.CurMana += addManaPercents;
	player.MaxCurMana += addManaPercents + sisyphean_trait_benefit;
	//LimitToMin(player.MaxCurMana, 64); // doesn't seem  to work !!!
	LimitToMax( player.CurMana, player.MaxCurMana );
	if (!(player.gameChanger & BIT(GC_21_C_NO_EVIL))) {
		player.Infravision = player.effectFlag[EA_INFRAVISION];
	}
	player.goldFind = goldFind;
	player.goldAbsFindMin = goldAbsFindMin;
	player.goldAbsFindMax = goldAbsFindMax;
	player.magicFind = magicFind;
	player.percentStr = statPercent[ST_STRENGTH];
	player.percentDex = statPercent[ST_DEXTERITY];
	player.percentMag = statPercent[ST_MAGIC];
	player.percentVit = statPercent[ST_VITALITY];
	int survivorBenefit = HasTrait(playerIndex, TraitId::Survivor) ? (2 * player.CharLevel) : 0;
	int fast_metabolism_trait_benefit = HasTrait(playerIndex, TraitId::FastMetabolism) ? (player.CharLevel / 10) : 0;
	player.lifeRegen = lifeRegen 
		+ survivorBenefit 
		+ fast_metabolism_trait_benefit;
	int barbarismPenalty = HasTrait(playerIndex, TraitId::Barbarism) ? (player.CharLevel / 5) : 0;
	int dark_pact_trait_penalty = HasTrait(playerIndex, TraitId::DarkPact) ? (player.CharLevel / 2) : 0;
	int insensitivity_trait_penalty = HasTrait(playerIndex, TraitId::Insensitive) ? (player.CharLevel / 9) : 0;
	player.manaRegen = manaRegen
		- barbarismPenalty 
		+ fast_metabolism_trait_benefit
		- dark_pact_trait_penalty
		- insensitivity_trait_penalty;
	player.lifeRegenPercent = lifeRegenPercent - PerkValue(SYMP_FIBROSIS, playerIndex);
	player.manaRegenPercent = manaRegenPercent - PerkValue(SYMP_FIBROSIS, playerIndex);
	player.xpGainMin = xpGainMin;
	player.xpGainMax = xpGainMax;
	player.xpGainPercent = xpGainPercent;
	switch (GameMode) { //starting exp update 03.09.2019
	    case GM_SURVIVAL: player.xpGainPercent += player.CharLevel + 50; break;
		case GM_IRONMAN: 
			if (player.gameChanger & BIT(GC_24_RUTHLESS)) { 
				break; 
			}
			else if (player.CharLevel < 60) {
				player.xpGainPercent += 50 + ((player.CharLevel * player.CharLevel * player.CharLevel) / 15'000) + (player.CharLevel * player.CharLevel / 5);
			}
			else {
				player.xpGainPercent += 585 + 3 * player.CharLevel;
			}
			break;
		case GM_NIGHTMARE: 
			if (player.CharLevel < 60) {
				player.xpGainPercent += 90 + ((player.CharLevel * player.CharLevel * player.CharLevel) / 10'000) + (player.CharLevel * player.CharLevel / 4);
			}
			else {
				player.xpGainPercent += 111 + 15 * player.CharLevel;
			}
			break;
		//case GM_HARD: player.xpGainPercent -= 25; break;
		case GM_SPEEDRUN: 
			if (player.CharLevel < 60) {
			player.xpGainPercent += 70 + ((player.CharLevel * player.CharLevel * player.CharLevel) / 12'500) + (player.CharLevel * player.CharLevel / 4);
			}
			else {
				player.xpGainPercent += 387 + 10 * player.CharLevel;
			}
			break;
	}
	player.resistMeleeMin = meleeAbsorbPercentMin;
    player.resistMeleeMax = meleeAbsorbPercentMax;
	player.resistArrowMin = arrowAbsorbPercentMin;
    player.resistArrowMax = arrowAbsorbPercentMax;
	player.summonHpPercent = summonHpPercent;
	player.summonDamageMin = summonDamageMin;
	player.summonDamageMax = summonDamageMax;
	player.summonDamagePercent = summonDamagePercent;
	player.summonAcPercent = summonAcPercent;
	player.summonToHit = summonToHit;
	player.summonHp = summonHp;
	player.summonAc = summonAc;
	player.blockChance = blockChance;
	int finesseTraitBenefit = HasTrait(playerIndex, TraitId::Finesse) ? (player.CharLevel / 27 + 10) : 0;
	int ironfist_TraitPenalty = HasTrait(playerIndex, TraitId::IronFisted) ? (player.CharLevel / 10 + 1) : 0;
	player.critChance = critChance + finesseTraitBenefit - ironfist_TraitPenalty;
	player.critPercent = critPercent;
	int bloody_mess_trait_benefit = HasTrait(playerIndex, TraitId::BloodyMess) ? ((player.CharLevel / 2) * (player.CharLevel / 2) + 20) : 0;
	player.critDamageMin = critDamageMin + bloody_mess_trait_benefit;
	player.critDamageMax = critDamageMax + bloody_mess_trait_benefit;	
	player.lifePercent = lifePercent;
	player.manaPercent = manaPercent;
	player.attackSpeed = attackSpeed;
	player.castSpeed = castSpeed;
	player.hitRecovery = hitRecovery;
	player.lifeStealPercent = lifeStealPercent;
	player.manaStealPercent = manaStealPercent;

	if (player.activeBuffFlag & BF_7_EXP_BLESS) {
		player.xpGainPercent += 10 + by(Difficulty, 12, 15, 18, 21, 24, 27, 30, 33, 36) + Dungeon->level / 5;
	}
	if (player.activeBuffFlag & BF_8_GOLD_BLESS) {
		player.goldFind += by( Difficulty, 55, 60, 65, 70, 75, 80, 85, 90, 95) + Dungeon->level / 4;
	}
	if (player.activeBuffFlag & BF_9_MAGIC_FIND) {
		player.magicFind += by(Difficulty, 20, 22, 25, 27, 30, 32, 35, 37, 40) + Dungeon->level / 13;
	}
	if (player.activeBuffFlag & BF_10_REGEN_BLESS) {
		player.lifeRegen += (2 + (player.CharLevel / 10));
		player.manaRegen += (2 + (player.CharLevel / 10));
		if (GameMode == GM_CLASSIC) {
			player.lifeRegen -= 1;
			player.manaRegen -= 1;
		}
	}
	if (player.activeBuffFlag & BF_11_SPELL_BLESS) {
		player.allSpellExtraLevel += 1;
	}
	if (player.activeBuffFlag & BF_12_LIGHT_CURSE) {
		player.LightRadius += 2;
	}
	if (player.activeBuffFlag & BF_13_CRIT_BLESS) {
		player.critChance += 5;
	}
	
	if (!(HasTrait(playerIndex, TraitId::Barbarian))) {
		if (is(GameMode, GM_NIGHTMARE, GM_IRONMAN)) {
			if (!(player.gameChanger & BIT(GC_24_RUTHLESS))) {
				player.magicFind += 100;
			}
		}
		else if (GameMode == GM_SURVIVAL) {
			player.magicFind += 50;
		}
		else if (GameMode == GM_SPEEDRUN) {
			player.magicFind += 200;
		}
	}

	int treasure_hunter_trait_penalty = HasTrait(playerIndex, TraitId::TreasureHunter) ? (75 + player.CharLevel / 2) : 0;
	int tough_luck_gf = player.gameChanger & BIT(GC_23_TOUGH_LUCK) ? 75 : 0;
	int coinbound_boost = HasTrait(playerIndex, TraitId::Coinbound) ? player.CharLevel : 0;
	player.goldFind += PerkValue(SYNERGY_LIGHT_FINGERED, playerIndex, 1) - treasure_hunter_trait_penalty - tough_luck_gf + coinbound_boost;
	int gold_fever_perk = PerkValue(PERK_GOLD_FEVER, playerIndex);
	int gold_digger_trait_benefit = HasTrait(playerIndex, TraitId::GoldDigger) ? (player.CharLevel * 2) : 0;
	player.goldAbsFindMin += 
		gold_fever_perk
		+ gold_digger_trait_benefit 
		+ PerkValue(SYNERGY_GOLDEN_SUNS, playerIndex);
	player.goldAbsFindMax += 
		gold_fever_perk 
		+ gold_digger_trait_benefit 
		+ PerkValue(SYNERGY_GOLDEN_SUNS, playerIndex);
	int abnegation_trait_regen_penalty = HasTrait(playerIndex, TraitId::Abnegation) ? (2 * player.CharLevel / 5) : 0;
	player.manaRegen += PerkValue(PERK_FOCUS, playerIndex)
		- abnegation_trait_regen_penalty
		+ PerkValue(SYNERGY_RESTORATION, playerIndex)
		- PerkValue(PERK_ABSOLUTION, playerIndex, 1)
		- PerkValue(SYNERGY_INFERNAL_BARGAIN, playerIndex, 1)
		- PerkValue(SYMP_COUGHING, playerIndex)
		+ PerkValue(SYNERGY_MANA_FLOW, playerIndex, 1)
		+ PerkValue(SYNERGY_AWAKENING, playerIndex, 1);
	int grim_deal_trait_penalty_clvl_2 = HasTrait(playerIndex, TraitId::GrimDeal) ? (2 * player.CharLevel / 5) : 0;
	int blood_and_sand_trait_penalty = HasTrait(playerIndex, TraitId::BloodAndSand) ? (player.CharLevel / 5) : 0;
	//int black_death_life_regen_penalty = player.gameChanger & BIT(GC_22_BLACK_DEATH) ? ((player.CharLevel * player.CharLevel) / 10) + (player.CharLevel / 2) : 0;
	player.lifeRegen += PerkValue(PERK_FORTITUDE, playerIndex)
		+ PerkValue(SYNERGY_RESTORATION, playerIndex)
		+ PerkValue(SYNERGY_LONGEVITY, playerIndex, 1) 
		- PerkValue(SYNERGY_INFERNAL_BARGAIN, playerIndex, 1)
		+ PerkValue(SYNERGY_REJUVENATION, playerIndex, 1)
		- grim_deal_trait_penalty_clvl_2
		- abnegation_trait_regen_penalty
		- blood_and_sand_trait_penalty
		//- black_death_life_regen_penalty;
		- PerkValue(SYMP_NECROSIS, playerIndex)
		- PerkValue(SYMP_INTERNAL_BLEEDING, playerIndex)
		- PerkValue(SYMP_SEPSIS, playerIndex)
		- PerkValue(SYMP_BLOODY_SPUTUM, playerIndex);
	if (player.CurAction == PCA_6_BLOCK || player.CurAction == PCA_7_GOT_HIT) {
		player.lifeRegen += PerkValue(PERK_RECOUP, playerIndex);
	}
	player.blockChance += PerkValue(PERK_TUCK_TAILS, playerIndex);

	// --------------------  STUN  -----------------------
	int scroungerTraitPenalty = HasTrait(playerIndex, TraitId::Scrounger) ? (1 + player.CharLevel / 15) : 0;
	int axepertise_buff = HasTrait(playerIndex, TraitId::Axepertise) ? 2 : 0;
	int barb_buff = HasTrait(playerIndex, TraitId::Barbarian) ? (player.CharLevel / 2) : 0;	
	int unshakable_trait_buff_stun = HasTrait(playerIndex, TraitId::Unshakable) ? (2 * player.CharLevel / 5) : 0;
	int resolute_guard_penalty = HasTrait(playerIndex, TraitId::ResoluteGuard) ? (3 * player.CharLevel / 10) : 0;
	player.stunThreshold[CT_0_STUN] += PerkValue(PERK_UNSTOPPABLE, playerIndex)
		- scroungerTraitPenalty
		- grim_deal_trait_penalty_clvl_2
		+ axepertise_buff
		+ barb_buff
		+ unshakable_trait_buff_stun
		- resolute_guard_penalty
		+ PerkValue(SYNERGY_GLAD_DISCIPLINE, playerIndex, 1);

	// --------------------  CRIT CHANCE  -----------------------
	int NK_crit_chance = player.gameChanger & BIT(GC_9_NIGHT_KIN) ? (player.CharLevel / 5) : 0;	
	player.critChance +=
		PerkValue(PERK_EXPERT_KILLER, playerIndex)
		+ PerkValue(SYNERGY_BETTER_CRITS, playerIndex, 0)
		+ PerkValue(PERK_CRIPPLING_STRIKES, playerIndex)
		+ PerkValue(SYNERGY_EXCEPTIONAL_FERVOUR, playerIndex, 0)
		+ PerkValue(SYNERGY_MORTAL_STRIKE, playerIndex)
		+ PerkValue(SYNERGY_CRIMSON_TIDE, playerIndex, 0)
		+ NK_crit_chance;
	if (player.CurLife < player.MaxCurLife * 40 / 100) {
		player.critChance += PerkValue(SYNERGY_CORNERED_BEAST, playerIndex);
	}
	// ------------------ END CRIT CHANCE ----------------------

	player.critPercent += PerkValue(PERK_SLAYER, playerIndex)
		+ PerkValue(SYNERGY_BETTER_CRITS, playerIndex, 1)
		+ PerkValue(SYNERGY_EXCEPTIONAL_FERVOUR, playerIndex, 1)
		+ PerkValue(SYNERGY_SEEKER_ARROWS, playerIndex)
		+ PerkValue(SYNERGY_DEEP_WOUNDS, playerIndex)
		+ PerkValue(SYNERGY_CRIMSON_TIDE, playerIndex, 1)
		+ PerkValue(SYNERGY_FATAL_IMPACTS, playerIndex, 0);

	//----------basic and extra boosts to mdr and adr---------------
	int thickskinned_trait_benefit = HasTrait(playerIndex, TraitId::ThickSkinned) ? (player.CharLevel / 9) : 0;
	int abneg_trait_benefit = HasTrait(playerIndex, TraitId::Abnegation) ? (player.CharLevel / 3) : 0;
	int arrow_dancing_trait = HasTrait(playerIndex, TraitId::ArrowDancing) ? (player.CharLevel / 2) : 0;
	int absorbArrow = PerkValue(PERK_STEEL_SKIN, playerIndex)
		+ thickskinned_trait_benefit
		+ PerkValue(SYNERGY_EXCEPTIONAL_FERVOUR, playerIndex, 3)
		+ PerkValue(SYNERGY_RIGIDITY, playerIndex)
		+ arrow_dancing_trait
		+ abneg_trait_benefit;
	int absorb_melee = PerkValue(PERK_TOUGHNESS, playerIndex) 
		+ PerkValue(SYNERGY_BUTCHERY, playerIndex)
		+ PerkValue(SYNERGY_EXCEPTIONAL_FERVOUR, playerIndex, 2)
		+ PerkValue(SYNERGY_RIGIDITY, playerIndex)
		+ thickskinned_trait_benefit 
		- arrow_dancing_trait
		+ abneg_trait_benefit;
	// Current VIT-based boost to MDR & ADR
	int MDR_boost = HasTrait(playerIndex, TraitId::Stoneform) ? 0 : (player.CurVitality / 10);
	int ADR_boost = MDR_boost;
	if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) { 
		MDR_boost += (player.CharLevel * player.CharLevel) / 409 + player.CharLevel ;
		ADR_boost += (player.CharLevel * player.CharLevel) / 409 + player.CharLevel ;
	}
	else if (HasTrait(playerIndex, TraitId::Barbarian)) {
		MDR_boost = (player.CharLevel * player.CharLevel) / 300 + player.CharLevel / 2;
		ADR_boost = MDR_boost;
	}
	else if (HasTrait(playerIndex, TraitId::Fechtmeister, TraitId::Mamluk) && is(GameMode, GM_IRONMAN, GM_SPEEDRUN, GM_SURVIVAL)) {
		MDR_boost += (player.CharLevel / 5);
		ADR_boost += (player.CharLevel / 5);
	}	
	player.resistArrowMin += absorbArrow + ADR_boost;
    player.resistArrowMax += absorbArrow + ADR_boost;
	player.resistMeleeMin += absorb_melee + MDR_boost;
    player.resistMeleeMax += absorb_melee + MDR_boost;
	if (GameMode == GM_CLASSIC) {
		player.resistArrowMin = 0;
		player.resistArrowMax = 0;
		player.resistMeleeMin = 0;
		player.resistMeleeMax = 0;
	}

	//----------basic and extra XP changes---------------
	int domesticatedTraitBenefit = HasTrait(playerIndex, TraitId::Domesticated) ? 30 : 0;
	int tough_luck_xp = player.gameChanger & BIT(GC_23_TOUGH_LUCK) ? 75 : 0;
	int priest_trait_xp_penalty = HasTrait(playerIndex, TraitId::HolyAura) ? 20 : 0;
	int blue_blood_trait_xp_boost = HasTrait(playerIndex, TraitId::BlueBlood) ? 10 : 0;
	player.xpGainPercent += PerkValue(PERK_SWIFT_LEARNER, playerIndex)
						+ PerkValue(SYNERGY_PICK_UP_THE_PACE, playerIndex)
						+ domesticatedTraitBenefit 
						- priest_trait_xp_penalty
						+ blue_blood_trait_xp_boost
						- tough_luck_xp;
	int forgetfulTraitBenefit = HasTrait(playerIndex, TraitId::Forgetful) ? (player.CharLevel * player.CharLevel + 100) : 0;
	player.xpGainMin += forgetfulTraitBenefit;
	player.xpGainMax += forgetfulTraitBenefit;
	int scroungerTraitBenefit = HasTrait(playerIndex, TraitId::Scrounger) ? 50 : 0;	
	//int barb_mf_penalty = HasTrait(playerIndex, TraitId::Barbarian) ? 75 : 0;
	player.magicFind += PerkValue(PERK_OBSESSION, playerIndex) 
		+ scroungerTraitBenefit
		//- barb_mf_penalty
		- treasure_hunter_trait_penalty
		- tough_luck_gf;
	int finesseTraitPenalty = (HasTrait(playerIndex, TraitId::Finesse) ? (player.BaseDamage / 2) : 0);
	int ironFist_TraitBenefit = (HasTrait(playerIndex, TraitId::IronFisted) ? player.CharLevel : 0);
	int oldfashioned_trait_benefit_damage = HasTrait(playerIndex, TraitId::OldFashioned) ? (player.CharLevel * player.CharLevel / 20) : 0;
	int armadillo_trait_penalty = HasTrait(playerIndex, TraitId::Armadillo) ? (player.CharLevel - 1) : 0;
	int boardingTechniqueDamage = player.CurDexterity * PerkValue(SYNERGY_BOARDING_TECHNIQUE, playerIndex) / 100;
	int inexorability_melee_damage = PerkValue(SYNERGY_INEXORABILITY, playerIndex, 1) == 0 ? 0 : (player.CharLevel / PerkValue(SYNERGY_INEXORABILITY, playerIndex, 1));
	int strong_heart = 0;
	if (PerkValue(SYNERGY_STRONG_HEART, playerIndex) > 0) {
		strong_heart = (player.MaxCurLife / 64) / PerkValue(SYNERGY_STRONG_HEART, playerIndex);
	}
	if ( 
			( 
				(	player.OnBodySlots[IS_LeftHand].ItemCode == IC_3_BOW || player.OnBodySlots[IS_LeftHand].ItemCode == IC_21_KNIFE )
				|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_22_MALLET && player.OnBodySlots[IS_LeftHand].IsReqMet )
				|| (player.OnBodySlots[IS_LeftHand].ItemCode == IC_23_PISTOL && player.OnBodySlots[IS_LeftHand].IsReqMet)
			)
		|| 
			(
				(    player.OnBodySlots[IS_RightHand].ItemCode == IC_3_BOW || player.OnBodySlots[IS_RightHand].ItemCode == IC_21_KNIFE )
				|| ( player.OnBodySlots[IS_RightHand].ItemCode == IC_22_MALLET && player.OnBodySlots[IS_RightHand].IsReqMet )
				|| ( player.OnBodySlots[IS_RightHand].ItemCode == IC_23_PISTOL && player.OnBodySlots[IS_RightHand].IsReqMet )
			)
		) {// ranged
		player.BaseDamage += PerkValue(PERK_OVERDRAW, playerIndex) 
			+ PerkValue(PERK_HEAVY_DRAW, playerIndex)
			+ PerkValue(PERK_ABSOLUTION, playerIndex, 0)
			+ PerkValue(PERK_TEMPEST, playerIndex)
			+ PerkValue(SYNERGY_MESSENGER_OF_DEATH, playerIndex, 1)
			- finesseTraitPenalty 
			+ oldfashioned_trait_benefit_damage
			+ ironFist_TraitBenefit
			+ PerkValue(SYNERGY_VERSATILITY, playerIndex)
			+ PerkValue(SYNERGY_ACUITY, playerIndex, 1)
			+ PerkValue(SYNERGY_GUNDOWN, playerIndex, 1)
			+ PerkValue(SYNERGY_ONSET, playerIndex, 1);
	}
	else {// melee
		player.BaseDamage += (HasTrait(playerIndex, TraitId::Rabid) ? player.BaseDamage : 0)
			+ PerkValue(PERK_STRONG_ARM, playerIndex) 
			+ PerkValue(PERK_SHARPENED_SWORD, playerIndex)
			+ PerkValue(SYNERGY_MESSENGER_OF_DEATH, playerIndex)
			+ PerkValue(SYNERGY_ACUITY, playerIndex, 1) 
			+ (HasTrait(playerIndex, TraitId::HeavyHanded) ? (4 * player.CharLevel / 5) : 0)
			- finesseTraitPenalty
			+ oldfashioned_trait_benefit_damage
			+ ironFist_TraitBenefit
			- armadillo_trait_penalty
			+ boardingTechniqueDamage
			+ inexorability_melee_damage
			+ PerkValue(PERK_DEADLY_PURPOSE, playerIndex)
			+ PerkValue(PERK_MIGHT_N_MAGIC, playerIndex, 0)
			+ PerkValue(PERK_VANDALISM, playerIndex)
			+ PerkValue(SYNERGY_HEAVY_HITTER, playerIndex, 1)
			+ PerkValue(SYNERGY_VALOR, playerIndex, 0)
			+ strong_heart
			+ ( PerkValue(SYNERGY_AGGRESSION, playerIndex, 1) * player.BaseDamage / 100 );
	}
	int oldfashioned_trait_benefit_ac = HasTrait(playerIndex, TraitId::OldFashioned) ? ((player.CharLevel * player.CharLevel / 45) + player.CharLevel) : 0;
	int gold_digger_trait_penalty = HasTrait(playerIndex, TraitId::GoldDigger) ? (player.CharLevel * 6 / 5) : 0;
	int grim_deal_penalty_ac = HasTrait(playerIndex, TraitId::GrimDeal) ? (2 * player.CharLevel / 5) : 0;
	int fechmeister_trait_benefit = HasTrait(playerIndex, TraitId::Fechtmeister) ? (player.CharLevel/2) : 0;
	int monkey_grip_trait_penalty = HasTrait(playerIndex, TraitId::MonkeyGrip) ? (10 + player.CharLevel) : 0;
	int NK_ac = player.gameChanger & BIT(GC_9_NIGHT_KIN) ? (player.CharLevel * player.CharLevel / 30 + 3 * player.CharLevel) : 0;
	int insensitive_trait_boost = HasTrait(playerIndex, TraitId::Insensitive) ? (50 * player.CharLevel / 100) : 0;
	int unshakable_trait_penalty_ac = HasTrait(playerIndex, TraitId::Unshakable) ? player.CharLevel : 0;
	int mamluk_ac = HasTrait(playerIndex, TraitId::Mamluk) ? player.CharLevel : 0;
	int dimachaerus_extra_ac = 0;
		if ((is(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SURVIVAL)) && player.fullClassId == PFC_DIMACHAERUS) {
			dimachaerus_extra_ac = 2 * player.CharLevel / 3;
		}
	int kamikazeTraitPenalty = HasTrait(playerIndex, TraitId::Kamikaze) ? (player.CharLevel / 3) : 0;
	int stoneformTraitBonus = HasTrait(playerIndex, TraitId::Stoneform) ? player.CharLevel : 0;
	int level_bonus = player.CharLevel > 50 ? (player.CharLevel - 50) : 0;
	if(HasTrait(playerIndex, TraitId::Crupellarius)){ level_bonus = 3 * player.CharLevel / 2; }
	if (is(player.fullClassId, PFC_SAVAGE, PFC_EXECUTIONER, PFC_BERSERKER)) {
		level_bonus += level_bonus / 2;
	}
	else if (player.fullClassId == PFC_ROGUE) {
		level_bonus *= 2;
	}
	int inexorability_AC = PerkValue(SYNERGY_INEXORABILITY, playerIndex, 2) == 0 ? 0 : (player.CharLevel / PerkValue(SYNERGY_INEXORABILITY, playerIndex, 2));
	player.ACFromClass += PerkValue(PERK_DODGER, playerIndex)
		+ PerkValue(SYNERGY_GRACE, playerIndex)
		+ PerkValue(SYNERGY_ACUITY, playerIndex, 2)
		- PerkValue(SYNERGY_REBOUND, playerIndex, 2)
		+ oldfashioned_trait_benefit_ac 
		- gold_digger_trait_penalty
		- grim_deal_penalty_ac
		- monkey_grip_trait_penalty
		+ fechmeister_trait_benefit
		+ NK_ac
		+ insensitive_trait_boost
		- unshakable_trait_penalty_ac
		+ inexorability_AC
		- PerkValue(SYNERGY_GLAD_DISCIPLINE, playerIndex, 2)
		- PerkValue(SYMP_FEVER_AND_CHILLS, playerIndex)
		+ mamluk_ac
		+ dimachaerus_extra_ac
		+ level_bonus
		- kamikazeTraitPenalty
		+ stoneformTraitBonus;
	int thickskinnedTraitPenalty = HasTrait(playerIndex, TraitId::ThickSkinned) ? (2 * player.CharLevel / 3) : 0;
	int kamikazeTraitBenefit = HasTrait(playerIndex, TraitId::Kamikaze) ? (player.CharLevel) : 0;
	int piercing_shot_trait_penalty = HasTrait(playerIndex, TraitId::PiercingShot) ? (player.CharLevel * 2) : 0;
	int puncturing_stab_trait_penalty = HasTrait(playerIndex, TraitId::PuncturingStab) ? player.CharLevel : 0;
	int zealot_trait_penalty = HasTrait(playerIndex, TraitId::Zealot) ? (5 + player.CharLevel) : 0;
	int NK_accu = player.gameChanger & BIT(GC_9_NIGHT_KIN) ? (5 + player.CharLevel) : 0;
	int nastyDispositionBonus = HasTrait(playerIndex, TraitId::NastyDisposition) ? (10 + player.CharLevel/3) : 0;
	int inexorability_accuracy = PerkValue(SYNERGY_INEXORABILITY, playerIndex, 0) == 0 ? 0 : (player.CharLevel / PerkValue(SYNERGY_INEXORABILITY, playerIndex, 0));
	player.ItemsAddToHitPercents += PerkValue(PERK_EAGLE_EYE, playerIndex)
				+ PerkValue(SYNERGY_ACUITY, playerIndex, 0) 
				- thickskinnedTraitPenalty
				+ kamikazeTraitBenefit
				+ NK_accu
				- piercing_shot_trait_penalty 
				- monkey_grip_trait_penalty
				- zealot_trait_penalty
				- puncturing_stab_trait_penalty
				+ nastyDispositionBonus
				+ PerkValue(PERK_GUIDED_HAND, playerIndex)
				+ PerkValue(PERK_DEADLY_PURPOSE, playerIndex)
				+ inexorability_accuracy
				+ PerkValue(SYNERGY_ONSET, playerIndex, 0) 
				+ PerkValue(SYNERGY_VALOR, playerIndex, 1)
				+ PerkValue(SYNERGY_RETRIBUTION, playerIndex, 1)
				+ PerkValue(SYNERGY_STRENGTH_N_HONOR, playerIndex, 1)
				+ PerkValue(SYNERGY_GUNDOWN, playerIndex, 0)
				- PerkValue(SYNERGY_HEAVY_HITTER, playerIndex, 0)
				+ PerkValue(SYNERGY_FATAL_IMPACTS, playerIndex, 1);
	int ratelhideTraitBenefit = HasTrait(playerIndex, TraitId::RatelHide) ? (player.CharLevel / 7) : 0;
	int abneg_Trait_dfe_Benefit = HasTrait(playerIndex, TraitId::Abnegation) ? 2 : 0;
	int oldhabit_trait_penalty = HasTrait(playerIndex, TraitId::OldHabit) ? (player.CharLevel / 8) : 0;
	int grim_deal_trait_penalty_dfe = HasTrait(playerIndex, TraitId::GrimDeal) ? (2 * player.CharLevel / 5) : 0;
	int blood_oath_trait_penalty_dfe = HasTrait(playerIndex, TraitId::BloodOath) ? (1 + player.CharLevel / 16) : 0;
	short exec_dfe = 0;
	if (player.fullClassId == PFC_EXECUTIONER) {
		if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) { exec_dfe = player.CharLevel * player.CharLevel / 100 + player.CharLevel / 5; }
		else {											exec_dfe = player.CharLevel * player.CharLevel / 1500 + player.CharLevel / 15; }
	}
	player.DamageFromEnemy += PerkValue(PERK_IRONCLAD, playerIndex)
				+ PerkValue(SYNERGY_BOLSTER, playerIndex)
				- ratelhideTraitBenefit
				- abneg_Trait_dfe_Benefit
				+ oldhabit_trait_penalty
				+ grim_deal_trait_penalty_dfe
				- exec_dfe
				+ blood_oath_trait_penalty_dfe
				+ PerkValue(SYMP_CYSTS, playerIndex)
				+ PerkValue(SYMP_DARK_PUSTULES, playerIndex)
				+ PerkValue(SYMP_BUBOES, playerIndex);
	player.AdvancedUser = 0;
	player.AdvancedUser += PerkValue(PERK_ADVANCED_USER, playerIndex);
	
	int engineer = HasTrait(playerIndex, TraitId::Engineer) ? (15 + player.CharLevel / 2) : 0;
	int nastyDispositionDamageBonus = HasTrait(playerIndex, TraitId::NastyDisposition) ? (25 + player.CharLevel / 2) : 0;
	player.ItemsAddDamagePercents += engineer 
		+ nastyDispositionDamageBonus
		+ PerkValue(PERK_HIT_HARDER, playerIndex)
		+ PerkValue(SYNERGY_GLAD_DISCIPLINE, playerIndex, 0)
		+ PerkValue(SYNERGY_RETRIBUTION, playerIndex, 0)
		+ PerkValue(SYNERGY_STRENGTH_N_HONOR, playerIndex, 0);

	//blocking
	player.CanBlock = 0;
	if( classID == PC_3_MONK ){
		if( player.OnBodySlots[IS_LeftHand].ItemCode == IC_10_STAFF && player.OnBodySlots[IS_LeftHand].IsReqMet ){
			player.CanBlock = 1;
			player.effectFlag[EA_FAST_BLOCK] = 1;
		}
		if( player.OnBodySlots[IS_RightHand].ItemCode == IC_10_STAFF && player.OnBodySlots[IS_LeftHand].IsReqMet ){
			player.CanBlock = 1;
			player.effectFlag[EA_FAST_BLOCK] = 1;
		}
		if( player.OnBodySlots[IS_LeftHand].ItemCode == IC_M1_NONE && player.OnBodySlots[IS_RightHand].ItemCode == IC_M1_NONE ){
			player.CanBlock = 1;
		}
		if( player.OnBodySlots[IS_LeftHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_RightHand].ItemCode == IC_M1_NONE
		 && player.OnBodySlots[IS_LeftHand].EquippedLocation != EL_2_TWO_HANDED ){
			player.CanBlock = 1;
		}
		if( player.OnBodySlots[IS_RightHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_LeftHand].ItemCode == IC_M1_NONE
		 && player.OnBodySlots[IS_RightHand].EquippedLocation != EL_2_TWO_HANDED ){
			player.CanBlock = 1;
		}
	}
	if (classID == PC_5_SAVAGE) {
		if ((player.OnBodySlots[IS_RightHand].TypeID == ITEM_1_WEAPON 
			&& player.OnBodySlots[IS_RightHand].EquippedLocation == EL_2_TWO_HANDED && 
			player.OnBodySlots[IS_RightHand].ItemCode != IC_3_BOW)
			|| (player.OnBodySlots[IS_LeftHand].TypeID == ITEM_1_WEAPON 
			&& player.OnBodySlots[IS_LeftHand].EquippedLocation == EL_2_TWO_HANDED && 
			player.OnBodySlots[IS_LeftHand].ItemCode != IC_3_BOW) 
			&& (player.OnBodySlots[IS_LeftHand].ItemCode != IC_M1_NONE 
			|| player.OnBodySlots[IS_RightHand].ItemCode != IC_M1_NONE)) {
			player.CanBlock = 1;
		}
	}
	if (player.fullClassId == PFC_DIMACHAERUS // this here is basis for supporting "Gemini" perk: + to block chance when wielding two identical type melee weapons
		&& ((player.OnBodySlots[IS_RightHand].ItemCode == IC_1_SWORD && player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD)
		|| (player.OnBodySlots[IS_RightHand].ItemCode == IC_4_MACE && player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE)) ) {
		player.CanBlock = 1;
	}
	//block update 05.03.2018
	bool checkFor2H = false;
	if ((player.OnBodySlots[IS_RightHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_RightHand].EquippedLocation == EL_2_TWO_HANDED)
		|| (player.OnBodySlots[IS_LeftHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_LeftHand].EquippedLocation == EL_2_TWO_HANDED))
		checkFor2H = true; //check for 2H weapon

	if (player.fullClassId == PFC_GUARDIAN || player.fullClassId == PFC_KENSEI) {
		if ((checkFor2H && ( player.OnBodySlots[IS_RightHand].ItemCode == IC_1_SWORD || player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD))
			|| (checkFor2H && ( player.OnBodySlots[IS_RightHand].ItemCode == IC_4_MACE || player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE)))
			player.CanBlock = 1;
	}
	if (player.fullClassId == PFC_SHUGOKI) {
		if ((checkFor2H && (player.OnBodySlots[IS_RightHand].ItemCode == IC_2_AXE || player.OnBodySlots[IS_LeftHand].ItemCode == IC_2_AXE )))
			player.CanBlock = 1;
	}

	
	
	player.IsBowInHand = 0;
    player.IsFlaskInHand = 0;
	int weaponItemCode = 0;
	if( player.OnBodySlots[IS_LeftHand].ItemCode != IC_M1_NONE
	 && player.OnBodySlots[IS_LeftHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_LeftHand].IsReqMet ){
        weaponItemCode = player.OnBodySlots[IS_LeftHand].ItemCode;
	}
	if( player.OnBodySlots[IS_RightHand].ItemCode != IC_M1_NONE
	 && player.OnBodySlots[IS_RightHand].TypeID == ITEM_1_WEAPON && player.OnBodySlots[IS_RightHand].IsReqMet ){ // не щит ?
        weaponItemCode = player.OnBodySlots[IS_RightHand].ItemCode;
	}
 
    int appearance = PAW_0_NONE;
	bool hideShield = false;
    	
    if( player.fullClassId == PFC_DEMONOLOGIST ){
		hideShield = true; // always empty hands animation
    }else if( player.fullClassId == PFC_BEASTMASTER ){
        if( weaponItemCode == IC_1_SWORD ) appearance = PAW_2_SWORD;
		hideShield = true; // sword only animation or empty hands animation
	}else if( is(player.fullClassId, PFC_MAGE, PFC_ELEMENTALIST, PFC_WARLOCK) && GameMode != GM_CLASSIC && ! HasTrait(playerIndex, TraitId::Mamluk) ){ // Mamluk go to common anim
        switch( weaponItemCode ){
		case IC_10_STAFF:
		case IC_4_MACE  : appearance = PAW_6_MACE;  break; // mace only animation
		case IC_1_SWORD : appearance = PAW_2_SWORD; break;
		case IC_3_BOW   : appearance = PAW_4_BOW;   break;
		}
		/*if (GameMode != GM_CLASSIC)*/ hideShield = true; // magic sphere and books is not a shield, TODO: check for separate item code needs
	}else if( player.fullClassId == PFC_NECROMANCER ){
        if( is(weaponItemCode , IC_10_STAFF, IC_4_MACE)) appearance = PAW_6_MACE;
    }else{
        switch( weaponItemCode ){
        case IC_1_SWORD  : appearance = PAW_2_SWORD;                             break;
        case IC_2_AXE    : appearance = PAW_5_AXE;                               break;
        case IC_3_BOW    : appearance = PAW_4_BOW;     player.IsBowInHand = 1;   break;
        case IC_4_MACE   : appearance = PAW_6_MACE;                              break;
        case IC_10_STAFF : appearance = PAW_8_STAFF;                             break;
		case IC_18_FLASK :
		case IC_19_TRAP  : appearance = PAW_1_SHIELD;  player.IsFlaskInHand = 1; break;
        case IC_20_CLAW  : appearance = PAW_1_SHIELD;                            break;
		case IC_21_KNIFE : appearance = PAW_1_SHIELD;  player.IsBowInHand = 1;   break;
		case IC_22_MALLET: appearance = PAW_6_MACE;    player.IsBowInHand = 1;   break;
		case IC_23_PISTOL: appearance = PAW_12_PISTOL; player.IsBowInHand = 1;   break;
        }
    }
	// shied + weapon check
	if( ! hideShield && (player.OnBodySlots[IS_LeftHand ].ItemCode == IC_5_SHIELD && player.OnBodySlots[IS_LeftHand ].IsReqMet
	 || player.OnBodySlots[IS_RightHand].ItemCode == IC_5_SHIELD && player.OnBodySlots[IS_RightHand].IsReqMet) ){
	    player.CanBlock = 1;
	    if( WeaponAnims[appearance].canPairShied ) ++appearance;
	}
	// двуручки и одноручки в единственном числе
	if( is(classID, PC_4_ROGUE, PC_5_SAVAGE) && is(appearance, PAW_2_SWORD, PAW_6_MACE)
	 && ( player.OnBodySlots[IS_LeftHand].ItemCode == IC_M1_NONE || player.OnBodySlots[IS_RightHand].ItemCode == IC_M1_NONE
	 || ! player.OnBodySlots[IS_RightHand].IsReqMet || ! player.OnBodySlots[IS_LeftHand].IsReqMet ) ){
	    appearance = appearance == PAW_2_SWORD ? PAW_10_SWORD_ONE : PAW_11_MACE_ONE;
	}
	if (GameMode == GM_CLASSIC) {
		if (player.fullClassId == PFC_MAGE) {
			switch (weaponItemCode) {
			case IC_10_STAFF:	appearance = PAW_8_STAFF;  break;
			case IC_4_MACE: appearance = PAW_6_MACE;  break; // mace only animation
			case IC_1_SWORD: appearance = PAW_2_SWORD; break;
			case IC_3_BOW: appearance = PAW_4_BOW;   player.IsBowInHand = 1; break;
			case IC_2_AXE: appearance = PAW_5_AXE;   break;
			}
			if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_1_SWORD && player.OnBodySlots[IS_RightHand].ItemCode == IC_5_SHIELD
				|| player.OnBodySlots[IS_RightHand].ItemCode == IC_1_SWORD && player.OnBodySlots[IS_LeftHand].ItemCode == IC_5_SHIELD) {
				appearance = PAW_3_SWORD_SHIELD; player.CanBlock = 1;
			}
			else if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_4_MACE && player.OnBodySlots[IS_RightHand].ItemCode == IC_5_SHIELD
				|| player.OnBodySlots[IS_RightHand].ItemCode == IC_4_MACE && player.OnBodySlots[IS_LeftHand].ItemCode == IC_5_SHIELD) {
				appearance = PAW_7_MACE_SHIELD; player.CanBlock = 1;
			}
		}
		else if (player.fullClassId == PFC_ARCHER) {
			if (player.OnBodySlots[IS_LeftHand].ItemCode == IC_5_SHIELD || player.OnBodySlots[IS_RightHand].ItemCode == IC_5_SHIELD) {
				player.CanBlock = 1;
			}

		}
	}
	
	if( player.OnBodySlots[IS_Torso].ItemCode == IC_9_HEAVY_ARMOR && player.OnBodySlots[IS_Torso].IsReqMet ){
		switch( player.fullClassId ){
		case PFC_MONK:
		case PFC_KENSEI:
		case PFC_SHUGOKI:	player.ACFromClass += player.CharLevel;		break;
		case PFC_ROGUE:
		case PFC_ASSASSIN:	player.ACFromClass += player.CharLevel / 2;	break;
		case PFC_SAVAGE:
		case PFC_BERSERKER:	player.ACFromClass += player.CharLevel / 4;	break;
		}
		appearance += 2 << 4; // heavy armor
	}else if( player.OnBodySlots[IS_Torso].ItemCode == IC_8_MID_ARMOR && player.OnBodySlots[IS_Torso].IsReqMet ){
		switch (player.fullClassId) {
		case PFC_MONK:
		case PFC_KENSEI:
		case PFC_SHUGOKI:	player.ACFromClass += player.CharLevel;		break;
		case PFC_ROGUE:
		case PFC_ASSASSIN:	player.ACFromClass += player.CharLevel / 2;	break;
		case PFC_SAVAGE:
		case PFC_BERSERKER:	player.ACFromClass += player.CharLevel / 4;	break;
		}
		appearance += 1 << 4; // medium armor
	}else if (is(player.fullClassId, PFC_MONK, PFC_KENSEI, PFC_SHUGOKI)) {
		player.ACFromClass += player.CharLevel;
	}
	// проверка на подвисание при смене вида
	//static int ii = 0;
	//if( ii < 6 ){ 
	//	if( ii == 5 ){
	//		player.CurAction = PCA_4_ATTACK;
	//	}
	//	++ii;
	//}
	if( player.Appearance != appearance && canLoadAnimFlag && player.CurAction < PCA_10_NEW_LEVEL && player.CurAction != PCA_8_DEATH ){
		player.Appearance = appearance;
		player.LoadPlayerAnimationMask = PAF_0_NO;
		LoadPlayerGFX(playerIndex, PAF_1_STAY);
		SetPlrAnims(playerIndex);
		if( player.CurAction > PCA_3_WALK_HORISONTAL ){
			StartStand(playerIndex, player.dir);
			ClearPlrPVars(playerIndex);
		}else if( player.CurAction != PCA_0_STAND ){
			ContinueWalk( playerIndex );
		}else{
			ContinueStay( playerIndex );
		}
	}else{
		player.Appearance = appearance; // стартовый расчет до загрузки файлов анимации либо работа с инвентарем
	}
	
	/*for (int item = 0; item < MissileAmount; item++) { // old disabled cycle, disabled while fixing monster MS pierce
		Missile& cast = Missiles[MissileIndexes[item]];
		if (cast.BaseMissileIndex == MI_13_MANA_SHIELD && cast.CasterIndex == playerIndex) { //Dragon Mana shield
			cast.FirstServiseValue = player.CurLife;	// здесь запоминается повышеная фури жизнь
			cast.SecondServiseValue = player.BaseLife;
			break;										// Здесь возникает глюк с шилдом :) не расчитано на несколько манащитов одновременно
		}
	}*/
	// Store new hp values in MS if it is active
	if (manaShieldIndex != -1) {
		Missile& cast = Missiles[manaShieldIndex];
		cast.value[0] = player.CurLife;
		cast.value[1] = player.BaseLife;
	}
	if( player.OnBodySlots[IS_Neck].ItemCode == -1 || player.OnBodySlots[IS_Neck].baseItemIndex != BI_41_CIRCLET ){
		bool stripGold = MaxGoldInCell == DoubleMaxGoldInCell / 2;
		MaxGoldInCell = DoubleMaxGoldInCell / 2;
		if( !stripGold ){
			StripTopGold(playerIndex);
		}
	}else{
		MaxGoldInCell = DoubleMaxGoldInCell;
	}
	if (GameMode != GM_CLASSIC){
		for (int i = 0; i < countof(Player::AuraIndex); i++)
		{
			switch (Auras[player.AuraIndex[i]].auraEffect) {
			case AUEF_1_HP_BLEED: player.lifeRegen -= player.AurasValues[i]; break;
			case AUEF_2_MP_BLEED: player.manaRegen -= player.AurasValues[i]; break;
			case AUEF_3_AC_REDUCE: player.ACFromItems -= player.AurasValues[i]; break;
			case AUEF_4_TO_HIT_REDUCE: player.ItemsAddToHitPercents -= player.AurasValues[i]; break;
			case AUEF_5_LIGHTNING_RES_REDUCE: player.resist[ET_2_LIGHTNING] -= player.AurasValues[i]; break;
			case AUEF_6_FIRE_RES_REDUCE: player.resist[ET_1_FIRE] -= player.AurasValues[i]; break;
			case AUEF_7_COLD_RES_REDUCE: player.resist[ET_6_COLD] -= player.AurasValues[i]; break;
			case AUEF_8_ACID_RES_REDUCE: player.resist[ET_4_ACID] -= player.AurasValues[i]; break;
			case AUEF_9_ARCAN_RES_REDUCE: player.resist[ET_3_ARCAN] -= player.AurasValues[i]; break;
			case AUEF_10_MELEE_RES_REDUCE: player.resistMeleeMin -= player.AurasValues[i]; player.resistMeleeMax -= player.AurasValues[i]; break;
			case AUEF_11_RANGED_RES_REDUCE: player.resistArrowMin -= player.AurasValues[i]; player.resistArrowMax -= player.AurasValues[i]; break;
			case AUEF_12_STUNTRSH_REDUCE: player.stunThreshold[CT_0_STUN] -= player.AurasValues[i]; break;
			case AUEF_13_PLAYER_DAMGE_REDUCE: player.ItemsAddDamage -= player.AurasValues[i]; break;
			case AUEF_14_PLAYER_DFE_REDUCE: player.DamageFromEnemy += player.AurasValues[i]; break;
			}
		}
	}
	UpdateScaleResist(playerIndex);
	CalcResistPercentMelee(playerIndex);
	CalcResistPercentArrow(playerIndex);
	for( int i = 0; i < countof(player.resist); ++i) {
		player.resistPercent[i] = player.scaleResist * player.resist[i];
		LimitToRange(player.resistPercent[i], 0.0f, player.resistPercentLimit); //Limiting resistances here
	}
	SetCols(playerIndex);
}

//----- (th3) -------------------------------------------------------------
void SetCols(int playerIndex)
{
	Players[playerIndex].invCols = (HasTrait(playerIndex, TraitId::LitheBuild)) ? 5 : InvCols;
}

//----- (0042463E) --------------------------------------------------------
void __fastcall UpdateRelictsState(Player& player)
{
	player.AvailableRelictMask = 0;

	for( int i = 0, ie = player.InvItemCount; i < ie; ++i ){
		Item& item = player.InventorySlots[i];
		int magicCode = item.MagicCode;
		if( item.ItemCode != IC_M1_NONE
			&& ( magicCode == MC_21_RELIC_NEED_NO_TARGET || magicCode == MC_22_RELIC_NEED_TARGET )
			&& item.IsReqMet ){
				player.AvailableRelictMask |= BIT(item.SpellIndex);
		}
	}

	for( int i = 0, ie = 8; i < ie; ++i ){
		Item& item = player.BeltInventory[i];
		int magicCode = item.MagicCode;
		if( item.ItemCode != IC_M1_NONE
			&& ( magicCode == MC_21_RELIC_NEED_NO_TARGET || magicCode == MC_22_RELIC_NEED_TARGET )
			&& item.IsReqMet ){
				player.AvailableRelictMask |= BIT(item.SpellIndex);
		}
	}

	if( player.SpellType == SO_2_RELIC ){
		if( !(player.AvailableRelictMask & BIT(player.CurrentSpellIndex)) ){
			player.CurrentSpellIndex = PS_M1_NONE;
			player.SpellType = SO_4_NONE;
		}
	}
}

//----- (0042476F) --------------------------------------------------------
void __fastcall SetAvailableChargesMask(int playerIndex)
{
	Player& player = Players[playerIndex];
	player.AvailableChargesMask = 0;

	for (int i = 0; i < IS_Inventory; ++i)
	{
		const Item& item = player.OnBodySlots[i];
		if (item.ItemCode != IC_M1_NONE
			&& item.IsReqMet
			&& item.CurCharges > 0)
			player.AvailableChargesMask |= 1i64 << (item.SpellIndex - 1);
	}
}

struct StatEffects
{
	short adds[ST_count];
	short percent[ST_count];
	void operator+=(StatEffects& a){ for(int i = 0; i < ST_count; ++i){ adds[i] += a.adds[i]; percent[i] += a.percent[i]; } }
	void operator-=(StatEffects& a){ for(int i = 0; i < ST_count; ++i){ adds[i] -= a.adds[i]; percent[i] -= a.percent[i]; } }
};
//----- (th3) -------------------------------------------------------------
void Item::getStatEffects(StatEffects& se, Player& player)
{
	for(auto& e: effect){
		switch(e.id){
		case AE_STAT            : se.adds[e.type]    += e.minVal; break;
		case AE_ALL_STAT        : each(se.adds)      += e.minVal; break;
		case AE_ALL_STAT_PERCENT: each(se.percent)   += e.minVal; break;
		case AE_STAT_PERCENT    : se.percent[e.type] += e.minVal; break;
		case AE_CLVL_STAT_ALL   : each(se.adds)      += e.minVal + player.CharLevel * e.maxVal / 100; break;
		case AE_CLVL_STAT       : se.adds[e.type]    += e.minVal + player.CharLevel * e.maxVal / 100; break;
		}
	}
}

//----- (004247CC) --------------------------------------------------------
// TODO: move code of this func to CalcCharParams for deduplicate and sync param calculation (new effects, set items, etc)
void __fastcall RecalcReqMetForOnBodyItems( int playerIndex )
{
	Player& player = Players[playerIndex];

	// Ensure that full class id is set, else, CheckBaseItemClassReq might fail. 
	// This isn't the ideal spot for this...but I don't know where it should go in this project...
	// Ideally, this would only be called once at a new game start, since the character won't be 
	// switching character classes during the game. Perhaps someone who knows the code base better
	// can put this where it needs to go.

	player.fullClassId = GetPlayerFullClass(player.ClassID, player.subclassID, player.specializationID);

	StatEffects statEffects {};
	auto isItemBroken = []( const Item& item )
		{
			const BaseItem& baseItem = BaseItems[item.baseItemIndex];
			if( baseItem.ItemType == ITEM_1_WEAPON || baseItem.ItemType == ITEM_2_ARMOR ){
				if( item.CurDurability <= 0 ){
					return true;
				}
			}
			return false;
		};

	std::vector<Item*> equippedItems;
	for( Item& item : player.OnBodySlots ) equippedItems.push_back( &item );
	SetItem_GetActiveSetItemEffects( playerIndex, equippedItems );

	for( Item* itemPtr : equippedItems ){
		Item& item = *itemPtr;
		if( item.ItemCode != IC_M1_NONE
			&& player.CharLevel >= item.RequiredLevel
			&& CheckBaseItemClassReq( playerIndex, item.baseItemIndex )
			&& !isItemBroken( item ) )
		{
			item.IsReqMet = true;
			if( item.Identified ){
				StatEffects se {}; item.getStatEffects(se, player);
				statEffects += se;
			}
		}
		else
		{
			item.IsReqMet = false;
		}
	}
	int needLoop;
	do{
		needLoop = 0;
		for( Item* itemPtr : equippedItems ){
			Item& item = *itemPtr;
			if( item.ItemCode == IC_M1_NONE ){
				continue;
			}
			if( !item.IsReqMet ){
				continue;
			}
			int advancedUser = PerkValue(PERK_ADVANCED_USER, playerIndex);
			int itemReqMet = 1;
			int skilledTraitBenefit = HasTrait(playerIndex, TraitId::Skilled) ? (player.CharLevel + (player.CharLevel * player.CharLevel / 150)) : 0;
			if( statEffects.adds[ST_STRENGTH]
				+ advancedUser 
				+ PerkValue(PERK_GAIN_STRENGTH, playerIndex)
				+ PerkValue(SYNERGY_STAR_ALIGNMENT, playerIndex)
				- PerkValue(SYNERGY_INFERNAL_BARGAIN, playerIndex, 2) 
				+ skilledTraitBenefit
				+ player.BaseStrength 
				+ player.BaseStrength * statEffects.percent[ST_STRENGTH] / 100 < item.RequiredStrength ){
				itemReqMet = 0;
			}
			if( statEffects.adds[ST_MAGIC]
				+ advancedUser 
				+ PerkValue(PERK_GAIN_MAGIC, playerIndex) 
				+ PerkValue(SYNERGY_STAR_ALIGNMENT, playerIndex)
				- PerkValue(SYNERGY_INFERNAL_BARGAIN, playerIndex, 2)
				+ skilledTraitBenefit
				+ player.BaseMagic
				+ player.BaseMagic * statEffects.percent[ST_MAGIC] / 100 < item.RequiredMagic ){
				itemReqMet = 0;
			}
			if( statEffects.adds[ST_DEXTERITY]
				+ advancedUser 
				+ PerkValue(PERK_GAIN_DEXTERITY, playerIndex) 
				+ PerkValue(SYNERGY_STAR_ALIGNMENT, playerIndex)
				- PerkValue(SYNERGY_INFERNAL_BARGAIN, playerIndex, 2)
				+ skilledTraitBenefit
				+ player.BaseDexterity
				+ player.BaseDexterity * statEffects.percent[ST_DEXTERITY] / 100 < item.RequiredDexterity  ){
				itemReqMet = 0;
			}
			if( statEffects.adds[ST_VITALITY]
				+ advancedUser 
				+ PerkValue(PERK_GAIN_VITALITY, playerIndex) 
				+ PerkValue(SYNERGY_STAR_ALIGNMENT, playerIndex)
				- PerkValue(SYNERGY_INFERNAL_BARGAIN, playerIndex, 2)
				+ skilledTraitBenefit
				+ player.BaseVitality
				+ player.BaseVitality * statEffects.percent[ST_VITALITY] / 100 < item.RequiredVitality ){
				itemReqMet = 0;
			}
			if( player.CharLevel < item.RequiredLevel ){
				itemReqMet = 0;
			}

			if( itemReqMet == 0 ){
				needLoop = 1;
				item.IsReqMet = 0;// IsReqMet
				if( item.ItemCode != IC_0_OTHER ){
					StatEffects se {}; item.getStatEffects(se, player);
					statEffects -= se;
				}
			}
		}
	}while( needLoop );
}

//----- (004248C6) --------------------------------------------------------
void __fastcall CheckAllItemsReq(int playerIndex)
{
	Player* player;			// ecx@1
	Item* item;				// edi@1
	int itemIndex;			// esi@1
	//int itemCount;			// ebp@2
	int slotNum;			// ebp@4
	
	player = &Players[playerIndex];
	for( itemIndex = 0; itemIndex < player->InvItemCount; ++itemIndex ){
		item = &player->InventorySlots[itemIndex];
		item->IsReqMet = CheckItemReq( playerIndex, item );
	}
	for( slotNum = 0; slotNum < IS_8_Belt_Count; ++slotNum ){
		item = &player->BeltInventory[ slotNum ];
		if( item->ItemCode != IC_M1_NONE ){
			item->IsReqMet = CheckItemReq( playerIndex, item );
		}
	}
}

//----- (th2) -------------------------------------------------------------
uint GetRequiredClassMask( const Item* item )
{
	int mask = BaseItems[item->baseItemIndex].RequiredClassMask;
	if( item->SpellIndex ){ mask &= Spells[item->SpellIndex].RequiredClassMask; }
	return mask;
}

//----- (th2) -------------------------------------------------------------
bool CheckBaseItemClassMaskReq( uint playerIndex, int baseItemIndex )
{
	const Player& player = Players[playerIndex];
	const BaseItem& baseItem = BaseItems[baseItemIndex];
	
	uint playerFullClassMask = (1u << static_cast<uint>( player.fullClassId ) );
	return (playerFullClassMask & baseItem.RequiredClassMask) != 0u;
}

//----- (th2) -------------------------------------------------------------
bool CheckBaseItemExilesReq( uint playerIndex, int baseItemIndex )
{
	const Player& player = Players[playerIndex];
	const BaseItem& baseItem = BaseItems[baseItemIndex];
	
	uint playerFullClassMask = (1u << static_cast<uint>( player.fullClassId ) );
	
	bool forbidden = (playerFullClassMask & IPCM_ANY_EXILE) != 0 && baseItem.ForbiddenForExiles;
	
	return !forbidden;
}

//----- (th2) -------------------------------------------------------------
bool CheckBaseItemClassReq( uint playerIndex, int baseItemIndex )
{
	return CheckBaseItemClassMaskReq( playerIndex, baseItemIndex )
		&& CheckBaseItemExilesReq( playerIndex, baseItemIndex );
}

//----- (0042492F) --------------------------------------------------------
int __fastcall CheckItemReq(uint playerIndex, Item* item)
{
	Player& player = Players[playerIndex];
	
    if( item->ItemCode == IC_M1_NONE )
    {
        return false;
    }
	
	if( item->SpellIndex )
    {
    	const Spell& spell = Spells[item->SpellIndex];
    	uint playerFullClassMask = (1u << static_cast<uint>( player.fullClassId ) );
    	if( (playerFullClassMask & spell.RequiredClassMask) == 0u )
    	{
    		return false;
		}
	}

	if( HasTrait(playerIndex, TraitId::Barbarian) && (item->MagicLevel || item->socketsAmount
	 || item->MagicCode == MC_24_BOOKS && not(item->SpellIndex, PS_2_HEALING, PS_7_TOWN_PORTAL, PS_22_FURY) ) ){
		return false;
	}

	if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) {
		if (item->MagicCode == MC_24_BOOKS && is(item->SpellIndex, PS_2_HEALING, PS_31_HOLY_BOLT, PS_39_HOLY_NOVA)) {
			return false;	// night kin does not read healing, holy bolt and holy nova
		}
		const BaseItem& baseItem = BaseItems[item->baseItemIndex];
		if (is(baseItem.ItemCode, IC_12_RING, IC_13_AMULET, 
			IC_15_GLOVE, IC_16_BOOTS, IC_17_BELT, 
			IC_1_SWORD, IC_2_AXE, IC_3_BOW, IC_4_MACE, 
			IC_7_HELM, IC_6_LIGHT_ARMOR, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR)){
			return false;
		}
		if (is(item->MagicCode, MC_18_POTION_OF_REJUVENATION, MC_19_POTION_OF_FULL_REJUVENATION)) {
			return false;			
		}
	}

	if (player.fullClassId == PFC_WARLOCK) {
		if (item->MagicCode == MC_24_BOOKS && is(item->SpellIndex, PS_31_HOLY_BOLT, PS_39_HOLY_NOVA)) {
			return false;
		}
	}

	if (HasTrait(playerIndex, TraitId::Paladin)) {
		if (item->MagicCode == MC_24_BOOKS && item->SpellIndex == PS_36_BONE_SPIRIT) {
			return false;
		}
	}

	if (HasTrait(playerIndex, TraitId::Juggernaut, TraitId::Feral)) {
		if (item->MagicCode == MC_24_BOOKS && item->SpellIndex == PS_23_TELEPORT) {
			return false;
		}
	}

	if (HasTrait(playerIndex, TraitId::WildSorcery) || HasTrait(playerIndex, TraitId::Rabid)) {
		if (is(item->MagicCode, 
			MC_3_POTION_OF_HEALING, 
			MC_6_POTION_OF_MANA, 
			MC_2_POTION_OF_FULL_HEALING, 
			MC_7_POTION_OF_FULL_MANA, 
			MC_18_POTION_OF_REJUVENATION, 
			MC_19_POTION_OF_FULL_REJUVENATION)) {
			return false;
		}
	}

	if (GameMode == GM_COLISEUM) {
		if (is(item->MagicCode,
			MC_3_POTION_OF_HEALING,
			MC_6_POTION_OF_MANA,
			MC_2_POTION_OF_FULL_HEALING,
			MC_7_POTION_OF_FULL_MANA,
			MC_18_POTION_OF_REJUVENATION,
			MC_19_POTION_OF_FULL_REJUVENATION)

			|| is(item->baseItemIndex, BI_214_RELICT_OF_HEALING, BI_215_RELICT_OF_MANA)
			
			) {
			return false;
		}
	}

	if (HasTrait(playerIndex, TraitId::OldFashioned)) {
		if (is(item->ItemCode, IC_6_LIGHT_ARMOR, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR)) {
			return false;
		}
	}

	if (player.fullClassId == PFC_ELEMENTALIST && (!HasTrait(playerIndex, TraitId::Mamluk))) {
		if (item->ItemCode == IC_3_BOW) {
			return false;
		}
	}

	if (HasTrait(playerIndex, TraitId::Pistoleer)) {
		if (is(item->ItemCode, IC_4_MACE, IC_1_SWORD, IC_3_BOW)) {
			return false;
		}
	}

	if (!HasTrait(playerIndex, TraitId::Pistoleer)) {
		if (is(item->ItemCode,  IC_23_PISTOL)) {
			return false;
		}
	}

	if (HasTrait(playerIndex, TraitId::Fechtmeister, TraitId::Ranger, TraitId::Mamluk)) {
		if (is(item->ItemCode, IC_5_SHIELD)) {
			return false;
		}
	}

	if (HasTrait(playerIndex, TraitId::Black_Witchery)) {
		if (is(item->ItemCode, IC_21_KNIFE)) {
			return false;
		}
	}

	if (HasTrait(playerIndex, TraitId::Paladin) || player.fullClassId == PFC_DRUID) {
		if (is(item->ItemCode, IC_22_MALLET, IC_3_BOW)) {
			return false;
		}
	}

	if (HasTrait(playerIndex, TraitId::TwoTowers) || HasTrait(playerIndex, TraitId::Axepertise)) {
		if (is(item->ItemCode, IC_1_SWORD, IC_4_MACE)) {
			return false;
		}
	}

	if( !CheckBaseItemClassReq( playerIndex, item->baseItemIndex ) )
	{
		return false;
	}
	
	// Broken items check
	const BaseItem& baseItem = BaseItems[item->baseItemIndex];
	if( baseItem.ItemType == ITEM_1_WEAPON || baseItem.ItemType == ITEM_2_ARMOR ){
	    if( item->CurDurability <= 0 ){
	        return false;
	    }
	}
	
	int advancedUser = PerkValue(PERK_ADVANCED_USER, playerIndex);
	if (is(item->MagicCode, MC_24_BOOKS, MC_21_RELIC_NEED_NO_TARGET, MC_22_RELIC_NEED_TARGET, MC_47_RUNE_OF_FIRE, MC_48_RUNE_OF_LIGHTNING, MC_49_RUNE_OF_SHOCK, MC_50_RUNE_OF_BLAZE, MC_51_RUNE_OF_STONE))
		advancedUser = 0;
	if( player.CurVitality + advancedUser >= item->RequiredVitality
	 &&	player.CurMagic + advancedUser >= item->RequiredMagic
	 &&	player.CurStrength + advancedUser >= item->RequiredStrength
	 &&	player.CurDexterity + advancedUser >= item->RequiredDexterity
     && player.CharLevel    >= item->RequiredLevel ){
		return true;
	}else{
		return false;
	}
}

//----- (th2) -------------------------------------------------------------
void __fastcall RecalcBookReq(Item& item, int playerIndex)
{
	if( item.ItemCode == IC_0_OTHER && item.MagicCode == MC_24_BOOKS ){
		item.RequiredMagic = Spells[item.SpellIndex].RequiredMagic;
		for (int spellLevel = 0; spellLevel < Players[playerIndex].SpellLevels[item.SpellIndex]; spellLevel++) {
			uint reqMag = item.RequiredMagic + 20;
			item.RequiredMagic = reqMag;
		}
		item.RequiredLevel = GetBookLevelReq(item.SpellIndex, playerIndex);
		item.IsReqMet = CheckItemReq(playerIndex, &item);
	}
}

//----- (00424965) --------------------------------------------------------
void __fastcall RecalcBooksReq(int playerIndex)
{
	Player& player = Players[playerIndex];

	// Проверка требований магии у книг в магазине Адрии и установка флага рекМет
	if( Dungeon == DUN_0_TOWN ){
		for( int itemIndex = 1; WitchAdriaItems[itemIndex].ItemCode != IC_M1_NONE && itemIndex < PT_25_WitchItemsCount; ++itemIndex ){
			CalculateRequiredMagicForBooks(itemIndex);
		}
	}
	// Проверка требований магии у книг в инвентаре и установка флага рекМет
	for( int itemIndex = 0; itemIndex < player.InvItemCount; itemIndex++ ){
		RecalcBookReq(player.InventorySlots[itemIndex], playerIndex);
	}
}

//----- (00424ABF) --------------------------------------------------------
void __fastcall RecalcPlayer( int playerIndex, int canLoadAnimFlag, bool forceItemRecalc /*= true*/ )
{
    // Prevent random generator from damage
    auto randUsingCounter = RandUsingCounter;
    auto randomSeed = RandomSeed;
    auto lastSeed = LastSeed;

	CheckAllItemsReq(playerIndex);
	// здесь проверяются требования к надетым вещам (база + эти вещи)
	// возможно надо перенести это в CheckAllItemsReq и переделать на сравнение с текущими атрибутами
	// чтобы в квестовых подземелья эти вещи тоже "краснели"
	RecalcReqMetForOnBodyItems(playerIndex);
	SetItem_RecalculateSetInfo(playerIndex);
	InitDoublePrice();
	CalcCharParams(playerIndex, canLoadAnimFlag);
	CheckAllItemsReq(playerIndex);
	if( playerIndex == CurrentPlayerIndex ){
		RecalcBooksReq(playerIndex);
		UpdateRelictsState(Players[playerIndex]);
		SetAvailableChargesMask(playerIndex);
		if( Dungeon == DUN_0_TOWN ){ // TODO: warning, current player used instead of playerIndex
			CheckAllItemsReqMets();
			if( forceItemRecalc || ! ItemUpdateTimer ){
				StashPanel_RecalcItemsReq();
				VisualTrade_RecalcItemsReq();
			}
            Craft_RecalcItemsReq();
		}
	}
	
	RandUsingCounter = randUsingCounter;
    RandomSeed = randomSeed;
    LastSeed = lastSeed;
}

//----- (th3) -------------------------------------------------------------
void __fastcall SetItemGrade(Item& item)
{
	if( item.MagicCode == MC_GEM ){
		Gem& gem = Gems[item.baseItemIndex - BI_1238_GEM_RUBY_1];
		item.subType = (char)gem.type;
		item.grade = gem.grade;
	}else if( uint(item.MagicCode - MC_31_OIL_OF_REPAIR_I) < MC_35_OIL_OF_REPAIR_V - MC_31_OIL_OF_REPAIR_I ){
		item.subType = IST_OIL_REPAIR;
		item.grade = item.MagicCode - MC_31_OIL_OF_REPAIR_I + 1;
	}else if( uint(item.MagicCode - MC_38_OIL_OF_REPAIR_VI) < MC_39_OIL_OF_REPAIR_VII - MC_38_OIL_OF_REPAIR_VI ){
		item.subType = IST_OIL_REPAIR;
		item.grade = item.MagicCode - MC_38_OIL_OF_REPAIR_VI + 6;
	}
}

// TODO: combine with GetItemAttrs (004257B4)
// TODO: convert to Item.SetBase(baseItemIndex) after clean up
//----- (00424B17) Clear and set base item params -------------------------
void __fastcall SetPlayerHandItem(Item& item, int baseItemIndex)
{
	BaseItem& baseItem = BaseItems[baseItemIndex];
	memset(&item, 0, sizeof(Item));
	item.saveVersion = CurSaveVersion;
	item.ItemCode = (ITEM_CODE)baseItem.ItemCode;
	item.GraphicValue = baseItem.GraphicValue;
	strcpy(item.Name, (const char*)baseItem.NamePtr);
	strcpy(item.FullMagicalItemName, (const char*)baseItem.NamePtr);
	item.EquippedLocation = baseItem.EquippedLocation;
	item.TypeID = baseItem.ItemType;
    item.throwing = (char)baseItem.throwing;
	item.MinDamage = baseItem.MinDamageLow;
	item.MaxDamage = baseItem.MaxDamageLow;
	item.ArmorClass = baseItem.MinimumArmor; // нет рандомизации, функция для специальных вещей, типа золота
    item.damageFromEnemies = baseItem.MinDamageFromEnemy;
    item.minMeleeAbsorbPercent = baseItem.MinMeleeAbsorbPercentLow;
    item.maxMeleeAbsorbPercent = baseItem.MaxMeleeAbsorbPercentLow;
    item.minArrowAbsorbPercent = baseItem.MinArrowAbsorbPercentLow;
    item.maxArrowAbsorbPercent = baseItem.MaxArrowAbsorbPercentLow;
    item.minThorns = baseItem.MinThornsLow;
    item.maxThorns = baseItem.MaxThornsLow;
	item.MagicCode = baseItem.MagicCode;
	if( baseItem.ItemCode == IC_18_FLASK ){
		item.overTime = FlasksInfo[baseItem.FlaskOrTrapCode].MaxOverTime;
	}
	item.SpellIndex = baseItem.SpellNumber;
	if( baseItem.MagicCode == MC_23_STAFF ){
		item.CurCharges = 18;// Почему зарядов конкретно 18?
	}
	item.BaseCharges = item.CurCharges;
	item.CurDurability = baseItem.DurabilityMin;
	item.BaseDurability = baseItem.DurabilityMin;
	item.RequiredStrength = baseItem.RequiredStrengthMax;
	item.RequiredMagic = baseItem.RequiredMagicMax;
	item.RequiredDexterity = baseItem.RequiredDexterityMax;
	item.RequiredVitality = baseItem.RequiredVitalityMax;
	item.RequiredLevel = baseItem.RequiredLevelMax;
	item.basePrice = baseItem.MinPrice;
	each(item.enchantIndexes) = -1;
	each(item.effect).id = AE_NO;
	item.quenchAffix = QA_NO;
	item.quenchLevel = 0;
	item.MagicLevel = ML_0_USUAL;
	item.Price = baseItem.MinPrice;
	item.baseItemIndex = baseItemIndex;

	SetItemGrade(item);
	AdjustItemLook(item);

	// Prevent random generator from damage
    auto randUsingCounter = RandUsingCounter;
    auto randomSeed = RandomSeed;
    auto lastSeed = LastSeed;
	for( int i = 0; i < baseItem.EffectCount; ++i ){
        const int effectIndex = 20 + i;
        Item_ApplyAffix(item, baseItem.Effects[i], 0, 0, 1, effectIndex, 0, 0, true);
		item.effect[effectIndex].origin = AO_BASE;
    }
    RandUsingCounter = randUsingCounter;
    RandomSeed = randomSeed;
    LastSeed = lastSeed;
}

//----- (00424C1B) --------------------------------------------------------
void Item::SetSeed(i64 seed, int genVersion)
{
	this->seed = (int)seed; // TODO: expand seed
	this->genVersion = genVersion;
}

i64 Item::MakeSeed()
{
	i64 seed = GetRndSeed();
	SetSeed(seed);
	return seed;
}

//----- (00424C27) --------------------------------------------------------
void __fastcall Item_MakeNonCollisionSeed(int playerIndex, Item& item)
{
	Player& player = Players[playerIndex];
	bool seedHasNoCollision;
	int seed;
	do{
		seedHasNoCollision = true;
		seed = GetRndSeed();

		for( int itemIndexIndex = 0; itemIndexIndex < ItemsCount; itemIndexIndex++ ){
			int itemIndex = ItemActive[itemIndexIndex];
			Item& item = Items[itemIndex];
			if( item.seed == seed ){
				seedHasNoCollision = false;
			}
		}

		if( playerIndex == CurrentPlayerIndex ){
			for( int i = 0; i < player.InvItemCount; ++i ){
				Item& item = player.InventorySlots[i];
				if( item.seed == seed ){
					seedHasNoCollision = false;
				}
			}
		}

	}while( !seedHasNoCollision );
	item.SetSeed(seed, CurItemGenVersion);
}

// в оригинале используется ровно одын раз - в StripTopGold. 
// В тх2 я для снижения дублирования кода все конструкции проверки диапазонов золота и выставления картинки
// были перенаправленны сюда. Но надо помнить, что в оригинале везде проверки по месту
//----- (00424CB0) --------------------------------------------------------
void __fastcall SetGraphicToGold( Item& gold )
{
	if( gold.amount <= Gold_1000_LowLimit ){
		gold.GraphicValue = Gold_4_SmallPic; // маленькая кучка золота
	}else if( gold.amount < Gold_2500_HiLimit ){
		gold.GraphicValue = Gold_5_MediumPic; // средняя кучка золота
	}else{
		gold.GraphicValue = Gold_6_BigPic; // большая кучка золота
	}
}

// преждевременная оптимизация
// Создание стартовых вещей новому персонажу в зависимости от класса
//----- (00424CE5) --------------------------------------------------------
void __fastcall CreateInitialItems(int playerIndex)
{
	Player& player = Players[playerIndex];
	// очистка слотов тела
	for( int onBodySlotIndex = 0; onBodySlotIndex < IS_Inventory; onBodySlotIndex++ ){
		player.OnBodySlots[onBodySlotIndex].ItemCode = IC_M1_NONE;
	}
	// очистка инвентаря
	memset(player.InvUsed, 0, sizeof(player.InvUsed));
	for( int inventorySlotIndex = 0; inventorySlotIndex < IS_70_40_Inv_Count; inventorySlotIndex++ ){  
		player.InventorySlots[inventorySlotIndex].ItemCode = IC_M1_NONE;
	}
	player.InvItemCount = 0;
	// очистка пояса
	for( int beltInventoryIndex = 0; beltInventoryIndex < IS_8_Belt_Count; beltInventoryIndex++ ){
		player.BeltInventory[beltInventoryIndex].ItemCode = IC_M1_NONE;
	}
	// Добавление начальных вещей из массива
	Item& beltSlot1 = player.BeltInventory[0];
	Item& beltSlot2 = player.BeltInventory[1];
	Item& leftHand = player.OnBodySlots[IS_LeftHand];
	Item& rightHand = player.OnBodySlots[IS_RightHand];
	StartItem startItems = FullPlayerClasses[player.fullClassId].StartItems;
	if (GameMode == GM_CLASSIC){ 
		switch(player.fullClassId){   // left hand, right hand, inventory, belt 1, belt 2
		case PFC_WARRIOR: startItems = { 2567, 2580, 2588, BI_2652_POTION_OF_HEAL_CLS, BI_2652_POTION_OF_HEAL_CLS}; break;
		case PFC_ARCHER : startItems = { 2595,   -1,   -1, BI_2652_POTION_OF_HEAL_CLS, BI_2652_POTION_OF_HEAL_CLS}; break;
		case PFC_MAGE   : startItems = { BI_2604_STARTING_STAFF_MAGE_CLS, -1, -1, BI_2653_POTION_OF_MANA_CLS, BI_2653_POTION_OF_MANA_CLS}; break;
		}
	}

	if (player.gameChanger & BIT(GC_23_TOUGH_LUCK)) {
		goto AXEPERTISE_JUMP;
	}
	
	if (HasTrait(playerIndex, TraitId::Axepertise)) {
		FullCreateAndPutItem(leftHand, BI_485_LONG_HATCHET);
		goto AXEPERTISE_JUMP;
	}
	else if (player.fullClassId == PFC_INQUISITOR) {
		FullCreateAndPutItem(leftHand, BI_1360_INQUIS_ENTRY_WEAPON);
		FullCreateAndPutItem(rightHand, startItems.RightHandItemIndex);
		goto AXEPERTISE_JUMP;
	}
	else if (player.fullClassId == PFC_SCOUT) {
		FullCreateAndPutItem(leftHand, BI_1361_SCOUT_ENTRY_WEAPON);
		goto AXEPERTISE_JUMP;
	}
	else if (HasTrait(playerIndex, TraitId::Ranger)) {
		FullCreateAndPutItem(leftHand, BI_1362_RANGER_ENTRY_WEAPON);
		goto AXEPERTISE_JUMP;
	}
	else if (HasTrait(playerIndex, TraitId::Mamluk)) {
		FullCreateAndPutItem(leftHand, BI_1386_LIGHTENED_KILIJ);
		goto AXEPERTISE_JUMP;
	}
	else if (HasTrait(playerIndex, TraitId::Pistoleer)) {
		FullCreateAndPutItem(leftHand, BI_2426_SHORT_PISTOL);
		goto AXEPERTISE_JUMP;
	}

	if (player.gameChanger & BIT(GC_9_NIGHT_KIN)) {
		FullCreateAndPutItem(leftHand, IC_M1_NONE); 
		goto AXEPERTISE_JUMP;
	}
	
	if (!HasTrait(playerIndex, TraitId::TwoTowers)) {
		FullCreateAndPutItem(leftHand, startItems.LeftHandItemIndex);
	}
	else {
		FullCreateAndPutItem(leftHand, startItems.RightHandItemIndex);
	}

	if (!HasTrait(playerIndex, TraitId::Fechtmeister)) {
		FullCreateAndPutItem(rightHand, startItems.RightHandItemIndex);
	}

	if (player.fullClassId == PFC_MAGE) {
		leftHand.CurCharges = 8;
		leftHand.BaseCharges = 8;
	}

	if (!HasTrait(playerIndex, TraitId::TwoTowers)) {
		PutStartItemToInventory(playerIndex, startItems.InventoryItemIndex);
	}	

	AXEPERTISE_JUMP:
	int initialGold = GameMode == GM_CLASSIC ? 100 : (150 + RNG(101));
	if (player.gameChanger & BIT(GC_23_TOUGH_LUCK)) { initialGold = 1 + RNG(10); }
		
	if( HasTrait(playerIndex, TraitId::Rabid, TraitId::WildSorcery, TraitId::Mamluk)/* || HasTrait(playerIndex, TraitId::Axepertise)*/ ){
	    initialGold += 2 * 100; // gold compensation for potions
    }else{
		if (GameMode != GM_COLISEUM) {
			FullCreateAndPutItem(beltSlot1, startItems.BeltSlot1ItemIndex);
			FullCreateAndPutItem(beltSlot2, startItems.BeltSlot2ItemIndex);
		}
    }
    
	if( HasTrait(playerIndex, TraitId::Rudiarius) ){
	    initialGold += 750;
    }
	if( GameMode == GM_CLASSIC && player.fullClassId == PFC_MAGE ){
		leftHand.CurCharges = 34;
		leftHand.BaseCharges = 34;
	}
		
	// добавление золота
	FullCreateAndPutItem(player.ItemOnCursor, 0);
	player.ItemOnCursor.amount = initialGold;
	player.TotalGold = player.ItemOnCursor.amount;
	AutoPutCursorToInventoryOrDrop();
	// какой то пересчёт параметров
	CalcCharParams(playerIndex, 0);
}

//----- (th3) -------------------------------------------------------------
bool __fastcall ItemSpaceOkIntoWall(int row, int col, int playerIndex)
{
	return In112(row, col) && !ItemMap[row][col];
}

//----- (00424F97) --------------------------------------------------------
bool __fastcall ItemSpaceOk(int row, int col, int playerIndex)
{
	// checking for monsters and players is not needed, because we have item highlighting
	if( ! In112(row, col) || ItemMap[row][col] || Dungeon == DUN_0_TOWN && MonsterMap[row][col] /*check for towner*/ ){
		return false;
	}
	Cell c(ObjectsMap, row, col);
	char objectNum = c();
	if( objectNum ){
		int objectIndex = objectNum <= 0 ? -1 - objectNum : objectNum - 1;
		if( Objects[ objectIndex ].isBlockWalking ){
			return false;
		}
	}
	char rightDownType = c(1,1);
	if( rightDownType > 0 ){
		if( Objects[rightDownType-1].selectable ){
			return false;
		}
	}
	if( rightDownType >= 0 || !Objects[ -(rightDownType + 1) ].selectable ){
		char downType = c(1);
		if( downType <= 0 ){
			return TileBlockWalking[ FineMap[row][col] ] == 0;
		}
		char rightType = c(0, 1);
		if( rightType <= 0 || !Objects[downType-1].selectable || !Objects[rightType-1].selectable ){
			return TileBlockWalking[ FineMap[row][col] ] == 0;
		}
	}
	return false;
}

//----- (00425089) --------------------------------------------------------
bool __fastcall GetItemSpaceNear(int& row, int& col, bool (__fastcall *itemSpaceOk)(int,int,int) = ItemSpaceOk, int playerIndex = -1)
{
	IF( 0 ){ // randomize drop placement
	
	int nearSpaceAvailable[3][3];
	// Try surrounding squares
	int c = 0, r = 0;
	for(int j = (col-1); j <= (col+1); j++, c++ ){
		for( int i = (row-1); i <= (row+1); i++, r++ ){
			nearSpaceAvailable[r][c] = itemSpaceOk(i, j, playerIndex);
		}
	}
	bool spaceAvailable = false;
	for( c = 0; c < 3; c++ ){
		for( r = 0; r < 3; r++ ){
			if( nearSpaceAvailable[r][c] ) spaceAvailable = true;
		}
	}
	// Must go here so same number of rnd calls on multiplayer machines
	int rs = RNG(15) + 1;

	// No fit, no good
	if (!spaceAvailable) return false;

	// Place item
	r = 0;
	c = 0;
	while (rs > 0) {
		if (ItemMap[r][c]) rs--;
		if (rs > 0) {
			r++;
			if (r == 3) {
				r = 0;
				c++;
				if (c == 3) c = 0;
			}
		}
	}
	r = r + row - 1;
	c = c + col - 1;
	row = r, col = c;
	return true;
	
	}else{

	for( int i = 0; i < 9; ++i ){
		int r = row + RowDeltaFB[i];
		int c = col + ColDeltaFB[i];
		if( itemSpaceOk(r,c, playerIndex) ){
			row = r, col = c;
			return true;
		}
	}
	return false;

	}
}

//----- (00425184) --------------------------------------------------------
bool __fastcall GetItemSpace(int row, int col, int& itemIndex, bool randomNear /*=true*/, bool (__fastcall *itemSpaceOk)(int,int,int), Item* src /*=0*/)
{
	if( ItemsCount < MaxItems_255 ){
		if( randomNear && GetItemSpaceNear(row, col, itemSpaceOk, CurrentPlayerIndex)
		 || RadialSpaceFound(row, col, CurrentPlayerIndex, itemSpaceOk) ){ // TODO: change to playerIndex
			itemIndex = ItemAvailable[0];
			Item& item = Items[itemIndex];
			if( src ){
				if( src != &item ){
					item = *src;
					src->ItemCode = IC_M1_NONE;
				}
			}else{
				memset(&item, 0, sizeof(item));
			}
			item.MapRow = row;
			item.MapCol = col;
			ItemMap[row][col] = itemIndex + 1;
			ItemAvailable[0] = ItemAvailable[MaxItems_255 - 1 - ItemsCount];
			ItemActive[ItemsCount] = itemIndex;
			++ItemsCount;
			return true;
		}
	}
	return false;
}

//----- (0042521D) --------------------------------------------------------
bool RadialSpaceFound(int& row, int& col, int playerIndex, bool (__fastcall *itemSpaceOk)(int,int,int))
{
	int maxOfs = itemSpaceOk == ItemSpacePathOk ? MaxPathStep_25 : 50;
	for( int cellOfs = 0; cellOfs < maxOfs; cellOfs++ ){
		for( int colOfs = -cellOfs; colOfs <= cellOfs; colOfs++ ){
			int c = col + colOfs;
			for (int rowOfs = -cellOfs; rowOfs <= cellOfs; rowOfs++) {
				int r = row + rowOfs;
				if( itemSpaceOk(r, c, playerIndex) ){ 
					row = r;
					col = c;
					return true;
				}
			}
		}
	}
	return false;
}

//----- (00425287) --------------------------------------------------------
void __fastcall Item_RecalculatePrice(Item& item)
{
	int basePriceMultiplied = item.basePriceMultiplier;
	if( basePriceMultiplied > 0 ){
		basePriceMultiplied *= item.basePrice;
	}else if( basePriceMultiplied < 0 ){
		basePriceMultiplied = item.basePrice / basePriceMultiplied;
	}
	item.Price = item.priceEffect + basePriceMultiplied;
	if( item.Price <= 0 ){
		item.Price = 1;
	}
}

//----- (th3) -------------------------------------------------------------
bool BookSpellSkip(int spellIndex, int playerIndex)// #book availability, #book sorting, #adria books
{
	Player player = Players[playerIndex];
	return spellIndex == PS_7_TOWN_PORTAL && isTownPortalForbidden()
		|| is(spellIndex, PS_32_BONE_SPIRIT_REAL_RESSURECT, PS_34_HEAL_OTHER) && MaxCountOfPlayersInGame == 1
		|| player.fullClassId == PFC_WARLOCK && is(spellIndex, PS_31_HOLY_BOLT, PS_39_HOLY_NOVA)
		|| (GameMode == GM_COLISEUM && (is(spellIndex, PS_32_BONE_SPIRIT_REAL_RESSURECT, PS_2_HEALING, PS_7_TOWN_PORTAL, PS_13_HYDRA, PS_15_FORCE_WAVE, 
			PS_21_GOLEM, PS_42_WARP, PS_9_INFRAVISION, PS_5_IDENTIFY, PS_31_HOLY_BOLT, PS_33_TELEKINES, PS_39_HOLY_NOVA, PS_8_STONE_CURSE, PS_16_REFLECT, 
			PS_24_APOCALYPSE, PS_28_TRAP_DISARM, PS_37_MANA_RECHARGE, PS_38_MAGI, PS_34_HEAL_OTHER)))
		|| HasTrait(CurrentPlayerIndex, TraitId::Paladin) && is(spellIndex, PS_36_BONE_SPIRIT)
		|| (GameMode == GM_CLASSIC && (is(spellIndex, PS_32_BONE_SPIRIT_REAL_RESSURECT, PS_15_FORCE_WAVE, PS_14_BALL_LIGHTNING, PS_42_WARP, PS_39_HOLY_NOVA,
			PS_16_REFLECT, PS_37_MANA_RECHARGE, PS_38_MAGI, PS_34_HEAL_OTHER, PS_17_LIGHTING_RING, PS_18_LIGHTNING_NOVA, PS_19_FLAME_RING, PS_22_FURY,
			PS_41_FIERY_NOVA, PS_43_ARCANE_NOVA)));
}

//----- (th3) -------------------------------------------------------------
bool BookSpellAvailable(int spellIndex, int playerIndex, int qlvl)
{
	Player player = Players[playerIndex];
	return Spells[spellIndex].BookQuality != -1 && qlvl >= Spells[spellIndex].BookQuality
		&& (1u << player.fullClassId) & Spells[spellIndex].RequiredClassMask;
}

//----- (004252D3) --------------------------------------------------------
void __fastcall Item_InitBooks( int itemIndex, int qlvl )
{
	// TODO: change all CurrentPlayerIndex to playerIndex in drop code (add playerIndex param or Item.playerIndex field
	// TODO: remove this playerIndex dependence, specific spell should spawn for specific class/build/mode, but not morph and desync
	int playerIndex = CurrentPlayerIndex;
	Player& player = Players[CurrentPlayerIndex];
	Item& item = Items[itemIndex];
	LimitToMin(qlvl, 1);
	int spellIndex = PS_1_FIREBOLT;
	if( item.genVersion < 1 ){
		for( int spellIndex = 1, randSpellCounter = RNG(PS_COUNT) + 1; randSpellCounter > 0; ++spellIndex ){
			if( BookSpellSkip(spellIndex, playerIndex) ) ++spellIndex;
			if( spellIndex >= PS_COUNT ) spellIndex = 1;
			if( BookSpellAvailable(spellIndex, playerIndex, qlvl) ){
				--randSpellCounter;
				spellIndex = spellIndex;
			}
		}
	}else{
		int availableSpell[PS_COUNT], availableSpells = 0; 
		for( int spellIndex = 1; spellIndex < PS_COUNT; ++spellIndex ){
			// TODO: move playerIndex dependencies to SmartDropDungeonItem()
			if( ! BookSpellSkip(spellIndex, playerIndex) && BookSpellAvailable(spellIndex, playerIndex, qlvl) ){
				availableSpell[ availableSpells++ ] = spellIndex;
			}
		}
		if( availableSpells ){
			spellIndex = availableSpell[ RNG(availableSpells) ];
		}
	}
	Spell& spell = Spells[spellIndex];
	item.SpellIndex = spellIndex;
	item.RequiredMagic = spell.RequiredMagic;
	item.basePrice += spell.BookCost;
	item.Price += spell.BookCost;
	const char* spellName = getSpellName( spellIndex );
	strcat(item.Name, spellName);
	strcat(item.FullMagicalItemName, spellName);
	item.GraphicValue = GameMode == GM_CLASSIC ? 88 : spell.BookPic;
	
	// item looks changing according to class
	// TODO: split summon spells to species for removing player dependency
	switch( spellIndex ){
	case PS_52_LESSER_SUMMON or PS_53_COMMON_SUMMON or PS_54_GREATER_SUMMON: // summon book
		if( is( player.fullClassId, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER) ){
			item.GraphicValue = SummonerSpellBooksIcons[player.fullClassId - PFC_DEMONOLOGIST][spellIndex - PS_52_LESSER_SUMMON];
		}else{
			item.GraphicValue = 2357 + 168;
		}break;
	case PS_13_HYDRA: if( player.fullClassId == PFC_ELEMENTALIST) item.GraphicValue = 2889 + 168; break; // new hydra book for elementalist
	case PS_22_FURY:  // fury book looks different for Magi
		if (player.ClassID == PC_2_MAGE) {
			item.GraphicValue = 2874 + 168;
		}
		else if (player.fullClassId == PFC_KENSEI && MaxCountOfPlayersInGame == 1) {
			item.GraphicValue = 2872/*3307*/ + 168;
		}
		break;
	}

}

//----- (004253D5) --------------------------------------------------------
void __fastcall Item_AddPrefix(Item& item, int MaxAffixQLVL, int Spell_ID, int IsMagicItem)
{
	int affixId = -1;
	vector<int> affixMap;
	affixMap.reserve(1000);
	Affix* affixes = CurAffixesMagic();
    BaseItem& baseItem = BaseItems[ item.baseItemIndex ];
	Affix* pref = 0;//&Affixes[185]; // Инициация нулевым маркером
	if( !RNG(10) || IsMagicItem ){
		while( affixes[affixId + 1].effect.id != AE_NO ){
			pref = &affixes[++affixId];
			if( (pref->item & IA_STAFF) && (pref->itemSpecialization & baseItem.Specialization) && pref->quality <= MaxAffixQLVL ){
				if( !IsMagicItem || pref->NotCursed ){
					affixMap.push_back(affixId);
					if( pref->DoubleChance )
						affixMap.push_back(affixId);
				}
			}
		}
		if( affixMap.size() ){
			affixId = affixMap[RNG(affixMap.size())];
			pref = &affixes[affixId];
			sprints(item.FullMagicalItemName, "%s %s", pref->Name, item.FullMagicalItemName);
			item.MagicLevel = ML_1_MAGIC;
			Item_ApplyAffix(item, pref->effect, pref->MinGoldValue, pref->MaxGoldValue, pref->Multiplier, 0, pref->minReqClvl, pref->maxReqClvl, false );
		}
	}
	if( !CheckLetterWidth(item.FullMagicalItemName) ){
		strcpy(item.FullMagicalItemName, BaseItems[item.baseItemIndex].Name2);
		if( affixId != -1 )
			sprints(item.FullMagicalItemName, "%s %s", pref->Name, item.FullMagicalItemName);
		sprints(item.FullMagicalItemName, "%s of %s", item.FullMagicalItemName, getSpellName( item.SpellIndex ));
		// Вылет при не сгенерированном перфиксе. Исправил, но теперь вместо названия у простого посоха
		// посох чего то там написано просто staff of //mordor - и это называется исправил?
		if( !item.MagicLevel )
			strcpy(item.Name, item.FullMagicalItemName);
	}
	Item_RecalculatePrice(item);
}

//----- (004255B4) --------------------------------------------------------
void __fastcall Item_AddSpellCastAbility(Item& item, int maxAffixQLVL, int isMagicItem, int rare /*= -1*/ )
{
	BaseItem& baseItem = BaseItems[item.baseItemIndex];
	const Player& player = Players[CurrentPlayerIndex];

	uint playerFullClassMask = (1u << static_cast<uint>(player.fullClassId));

	int maxSpellQlvl = maxAffixQLVL / 2;
	if( maxSpellQlvl == 0 ){
		maxSpellQlvl = 1;
	}
	int spellRange = RNG(PS_COUNT) + 1;
	int curSpell_ID = 1;
	int spellIndex = 0;
	
	
	if (GameMode == GM_CLASSIC) {// mor: a copy-paste of cycle below, made to exclude certain spells from wands in classic
		while (spellRange > 0) {
			int itemQLVL = Spells[curSpell_ID].ItemQuality;
			if (itemQLVL != -1 && itemQLVL <= maxSpellQlvl
				&& Spells[curSpell_ID].RequiredClassMask & playerFullClassMask /*& baseItem.RequiredClassMask*/ //replacing baseitem mask with player mask for more precise spell req
				&& (not(curSpell_ID, PS_32_BONE_SPIRIT_REAL_RESSURECT, PS_15_FORCE_WAVE, PS_21_GOLEM, PS_42_WARP, PS_9_INFRAVISION, PS_5_IDENTIFY, PS_17_LIGHTING_RING, PS_18_LIGHTNING_NOVA,
					PS_33_TELEKINES, PS_39_HOLY_NOVA, PS_16_REFLECT, PS_28_TRAP_DISARM, PS_37_MANA_RECHARGE, PS_38_MAGI, PS_34_HEAL_OTHER, PS_14_BALL_LIGHTNING, PS_19_FLAME_RING,
					PS_22_FURY, PS_41_FIERY_NOVA, PS_43_ARCANE_NOVA))
				) {
				spellIndex = curSpell_ID;
				--spellRange;
			}
			if (++curSpell_ID >= PS_COUNT) { curSpell_ID = 1; } // идем по кругу
		}

	}
	else if (GameMode == GM_COLISEUM) {// mor: a copy-paste of cycle below, made to exclude certain spells from wands in coliseum (couldn't find better solution)
		while (spellRange > 0) {
			int itemQLVL = Spells[curSpell_ID].ItemQuality;
			if (itemQLVL != -1 && itemQLVL <= maxSpellQlvl
				&& Spells[curSpell_ID].RequiredClassMask & playerFullClassMask /*& baseItem.RequiredClassMask*/ //replacing baseitem mask with player mask for more precise spell req
				&& (not(curSpell_ID, PS_32_BONE_SPIRIT_REAL_RESSURECT, PS_2_HEALING, PS_7_TOWN_PORTAL, PS_13_HYDRA, PS_15_FORCE_WAVE, PS_21_GOLEM, PS_42_WARP, PS_9_INFRAVISION, PS_5_IDENTIFY,
					PS_31_HOLY_BOLT, PS_33_TELEKINES, PS_39_HOLY_NOVA, PS_8_STONE_CURSE, PS_16_REFLECT, PS_24_APOCALYPSE, PS_28_TRAP_DISARM, PS_37_MANA_RECHARGE, PS_38_MAGI, PS_34_HEAL_OTHER))
				) {
				spellIndex = curSpell_ID;
				--spellRange;
			}
			if (++curSpell_ID >= PS_COUNT) { curSpell_ID = 1; } // идем по кругу
		}

	}
	else {
		while (spellRange > 0) {
			int itemQLVL = Spells[curSpell_ID].ItemQuality;
			if (itemQLVL != -1 && itemQLVL <= maxSpellQlvl
				&& Spells[curSpell_ID].RequiredClassMask & playerFullClassMask /*& baseItem.RequiredClassMask*/ //replacing baseitem mask with player mask for more precise spell req
				&& (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_NIGHTMARE, GM_SPEEDRUN) ? curSpell_ID != PS_7_TOWN_PORTAL : true) //removing townportal generation on certain game modes
				// тут может происходить морфинг спелла на предмете между SP и MP, так как список доступных спеллов разный
				&& (MaxCountOfPlayersInGame > 1 || not(curSpell_ID, PS_32_BONE_SPIRIT_REAL_RESSURECT, PS_34_HEAL_OTHER))				
				) {
				spellIndex = curSpell_ID;
				--spellRange;
			}
			if (++curSpell_ID >= PS_COUNT) { curSpell_ID = 1; } // идем по кругу
		}
	}
	Spell& spell = Spells[spellIndex];
	item.SpellIndex = spellIndex;
	int charges = RngFromRange(spell.MinItemCharges, spell.MaxItemCharges);
	item.CurCharges = charges;
	item.BaseCharges = charges;
	item.RequiredMagic = spell.RequiredMagic;
	int addToPrice = item.CurCharges * spell.PriceMultiplier / 5;
	item.basePrice += addToPrice;
	item.Price += addToPrice;
	if( rare == -1 ){
		char name[64];
		sprintf(name, "%s of %s", baseItem.Name2, getSpellName(item.SpellIndex));
		strcpy(item.Name, name);
		strcpy( item.FullMagicalItemName, name );
		Item_AddPrefix(item, maxAffixQLVL, item.SpellIndex, isMagicItem );
	}else{
		Item_AddPrefixRare(item, maxAffixQLVL, item.SpellIndex, isMagicItem, rare);
	}
}

//----- (004256FF) --------------------------------------------------------
void __fastcall Item_InitOils( int itemIndex, int qlvl )
{
	int oilIndex;
	if( MaxCountOfPlayersInGame == 1 ){
		LimitToMin(qlvl, 1);
		int goodOilsCount = 0;
		int goodOils[32];
		for( int currentOil = 0; currentOil < 10; currentOil++ ){
			if( OilsQuality[currentOil] <= qlvl ){
				goodOils[goodOilsCount++] = currentOil;
			}
		}
		oilIndex = goodOils[RNG(goodOilsCount)];
	}else{
		oilIndex = (RNG(2) != 0) + 5;// В мульти доступны только 5 и 6 масла. в одной из версий поправлено
	}

	Item& item = Items[itemIndex];

	strcpy(item.Name, OilsName[oilIndex]);
	strcpy(item.FullMagicalItemName, OilsName[oilIndex]);
	item.MagicCode = (MAGIC_CODE)OilsMagicCode[oilIndex];
	int oilPrice = OilPrice[oilIndex];
	item.basePrice = oilPrice;
	item.Price = oilPrice;
}

//----- (007085CE) --------------------------------------------------------
int GetGoldModificator()
{
	return 2 * Dungeon->level - 1;
}

//----- (th3) -------------------------------------------------------------
void AdjustItemLook(Item& item)
{
	auto baseItemIndex = item.baseItemIndex;
	// Prevent random morph by changing pics
	auto randUsingCounter = RandUsingCounter;
	auto randomSeed = RandomSeed;
	auto lastSeed = LastSeed;

	if (item.MagicCode == MC_21_RELIC_NEED_NO_TARGET && item.SpellIndex == PS_22_FURY) {
		if (Players[CurrentPlayerIndex].ClassID == PC_2_MAGE) {
			sprintf(item.FullMagicalItemName, RelixToScrolls ? "Scroll of Concentration" : "Relict of Concentration");
			item.GraphicValue = 3231 + 168;
		}
		else if (Players[CurrentPlayerIndex].fullClassId == PFC_KENSEI && MaxCountOfPlayersInGame == 1) {
			sprintf(item.FullMagicalItemName, RelixToScrolls ? "Scroll of Battle Trance" : "Relict of Battle Trance");
			item.GraphicValue = 3306 + 168;
		}
	}

	/*---------------------------------
	------BASE ITEM LOOKS BY CLASS----------  #item flip, #flip, #mirror, #mirrorImage, #base item appearance
	---------------------------------*/
	if (is(Players[CurrentPlayerIndex].ClassID, PC_1_ARCHER, PC_4_ROGUE)) {// females
		if (baseItemIndex == BI_54_CAPE) { // cape
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 1218 + 3885;
				sprintf(item.Name, "Fur Cape");
				sprintf(item.FullMagicalItemName, "Fur Cape");
			}
		}
		else if (item.GraphicValue == 1310 + 3885) { // greaves of blood
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 37 + 3885) { // compact crossbow
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 808) { // griswold's crusher
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 2648 + 168) { // curved knife
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 193 + 168) { // rune blade
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 1143 + 168) { // light flail
			item.mirrorImage = 1;
		}
		//else if (item.GraphicValue == 503 + 3885) { // Dreadscale Gloves (T2 DEX gloves)
		//	item.mirrorImage = 1;
		//}
		else if (item.GraphicValue == 375 + 3885) { // Iron Figurine reward (bracers)
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 383 + 3885) { // Adventurer's Blade (quest reward from Hive Mother)
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 382 + 3885) { // 'A Simple Task' quest reward (belt)
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 385 + 3885) { // Adventurer's Boots ('Locked Chest' quest reward)
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 372 + 3885) { // lightened gambeson
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 627 + 3885;
				sprintf(item.Name, "Insulated Gambeson");
				sprintf(item.FullMagicalItemName, "Insulated Gambeson");
			}
			else {
				item.mirrorImage = 1;
			}
		}
		else if (item.GraphicValue == 1254 + 168) { // Crude Leather
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 629 + 3885;
				//sprintf(item.Name, "Insulated Gambeson");
				//sprintf(item.FullMagicalItemName, "Insulated Gambeson");
			}
		}
		else if (item.GraphicValue == 1465 + 168) { // hourglass gauntlets
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 1199 + 168) { // slim blade
			item.mirrorImage = 1;
		}
		else if (is(item.GraphicValue, 1454 + 168, 1455 + 168, 1456 + 168, 57 + 3885, 58 + 3885)) { // T2 belts
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 1919 + 168) { // iron mace
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == /*1067 + 168*/ 1009 + 3885) { // great flail
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 1058 + 168) { // huge flail
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 727 + 168) { // flanged mace
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 1590 + 168) { // cloth tunic
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 624 + 3885;
				sprintf(item.Name, "Fur Tunic");
				sprintf(item.FullMagicalItemName, "Fur Tunic");
			}
			else {
				item.GraphicValue = 274 + 3885;// 3257 + 168;
			}
		}
		else if (item.GraphicValue == 3308 + 168) { // coif
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 634 + 3885;
				sprintf(item.Name, "Fur Coif");
				sprintf(item.FullMagicalItemName, "Fur Coif");
			}
		}
		else if (item.GraphicValue == 3311 + 168) { // cloth armor
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 625 + 3885;
				sprintf(item.Name, "Dark Fur Armor");
				sprintf(item.FullMagicalItemName, "Dark Fur Armor");
			}
			else {
				item.GraphicValue = 3312 + 168;
			}
		}
		else if (item.GraphicValue == 2382 + 168) { // short leather jack
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 626 + 3885;
				sprintf(item.Name, "Insulated Leather Jack");
				sprintf(item.FullMagicalItemName, "Insulated Leather Jack");
			}
		}
		else if (item.GraphicValue == 19) { // bloodraven's bow
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 2608 + 168) { // thick cloak
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 1219 + 3885;
			}
			else {
				item.GraphicValue = 3259 + 168;
			}
		}
		else if (baseItemIndex == BI_185_QUILTED_ARMOR){ // quilted armor
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 1220 + 3885;
				sprintf(item.Name, "Insulated Coat");
				sprintf(item.FullMagicalItemName, "Insulated Coat");
			}
		}
		else if (item.GraphicValue == 1541 + 168) { // cloth gloves
			if (Players[CurrentPlayerIndex].fullClassId == PFC_TRAPPER) {
				item.GraphicValue = 2069 + 168;
				item.mirrorImage = 1;
			}
			else {
				item.GraphicValue = 3260 + 168;
			}
		}
		else if (item.GraphicValue == 1970 + 168) { // light leather gloves
			item.GraphicValue = 3267 + 168;
			sprintf(item.Name, "Leather Bracers");
			sprintf(item.FullMagicalItemName, "Leather Bracers");
		}
		else if (item.GraphicValue == 1497 + 168) { // leather demi gauntlets
			item.GraphicValue = 3268 + 168;
		}
		else if (item.GraphicValue == 2051 + 168) { // rondel protection gloves
			item.GraphicValue = 3269 + 168;
		}
		else if (baseItemIndex == BI_405_LEATHER_GLOVES) { // leather gloves
			item.GraphicValue = 1098 + 3885;//RngFrom(3270 + 168, 1098 + 3885);
		}
		else if (item.GraphicValue == 1556 + 168) { // hardened leather gloves
			item.GraphicValue = 3271 + 168;
		}
		else if (item.GraphicValue == 1544 + 168) { // chainmail gloves
			item.GraphicValue = 3272 + 168;
		}
		else if (item.GraphicValue == 1548 + 168) { // scalemail gauntlets
			item.GraphicValue = 3273 + 168;
		}
		else if (item.GraphicValue == 1532 + 168) { // boots
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 286 + 3885;
				item.mirrorImage = 1;
				sprintf(item.Name, "Winter Boots");
				sprintf(item.FullMagicalItemName, "Winter Boots");
			}
			else if (Players[CurrentPlayerIndex].fullClassId == PFC_SHARPSHOOTER) {
				item.GraphicValue = 3302 + 168;
			}
			else if (Players[CurrentPlayerIndex].fullClassId == PFC_SCOUT) {
				item.GraphicValue = 4 + 3885;
			}
			else if (Players[CurrentPlayerIndex].fullClassId == PFC_TRAPPER) {
				item.GraphicValue = 5 + 3885;
			}
			else {
				item.GraphicValue = 3261 + 168;
			}
		}
		else if (item.GraphicValue == 261 + 3885) { // leather boots
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 291 + 3885;
				item.mirrorImage = 1;
				sprintf(item.Name, "Insulated Boots");
				sprintf(item.FullMagicalItemName, "Insulated Boots");
			}
			else {
				item.mirrorImage = 1;
			}
		}
		else if (item.GraphicValue == 1911 + 168) { // hard leather boots
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 714 + 3885;
				item.mirrorImage = 1;
			}
			else {
				item.mirrorImage = 1;
			}
		}
		else if (item.GraphicValue == 1551 + 168) { // thick leather boots
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 715 + 3885;
				item.mirrorImage = 1;
				sprintf(item.Name, "Polar Boots");
				sprintf(item.FullMagicalItemName, "Polar Boots");
			}
			else {
				item.GraphicValue = 3264 + 168;
			}
		}
		else if (item.GraphicValue == 1537 + 168) { // chainmail boots
			item.GraphicValue = 3265 + 168;
		}
		else if (item.GraphicValue == 1552 + 168) { // splintmail greaves
			item.GraphicValue = 3266 + 168;
		}
		else if (item.GraphicValue == 128 + 168) { // tight bow
			item.GraphicValue = 3292 + 168;
		}
		else if (item.GraphicValue == 1133 + 168) { // composite bow
			item.GraphicValue = 3294 + 168;
		}
		else if (item.GraphicValue == 42 + 168) { // tight war bow
			item.GraphicValue = 3295 + 168;
		}
		else if (item.GraphicValue == 1332 + 168) { // crossbow
			item.GraphicValue = 3296 + 168;
		}
		else if (item.GraphicValue == 3603 + 168) { // wicker belt
			item.GraphicValue = 3604 + 168;
		}
		else if (item.GraphicValue == 3620 + 168) { // thin plate gauntlets
			//item.GraphicValue = 2234 + 168;
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 758 + 168) { // khopesh
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 744 + 168) { // falchion
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 749 + 168) { // light hammer
			item.mirrorImage = 1;
		}
		if (HasTrait(CurrentPlayerIndex, TraitId::Axepertise)) {
			switch (item.GraphicValue) {
			case 1686 + 168:
			case 3396 + 168:
			case 2903 + 168:
			case 1479 + 168:
			case 1090 + 168:
			case 1179 + 168:
			case 1506 + 168:
			case 1509 + 168:
			case 1895 + 168:
			case 129 + 168:
			case 1893 + 168:
			case 1510 + 168:
			case 1513 + 168:
			case 1685 + 168:
			case 891:// anvil unique axe
			case 1508 + 168:	item.mirrorImage = 1; break;
			}
		}
	}
	// ------------------------------------------------------------------------------------------------------
	// men: generalized changes
	else {
		switch (item.GraphicValue) {
		case 2433 + 168: // Lightmail Gloves
		case 503 + 3885: // Dreadscale Gloves
		case 391 + 3885: // Netherworld Bazubands
		case 398 + 3885: // Infernal Gauntlets
		case 1547 + 168: // Scalemail Gloves
		case 1464 + 168: // Articulated Gauntlets
		case 1557 + 168: // Splintmail Bazubands
		case 1070 + 3885: // Abyssal Gauntlets
		case 1071 + 3885: // Ghoststeel Handguards
		case 1075 + 3885: // Great Boots (Mamluk)
		case 1080 + 3885: // Hell-Forged Mail (Mamluk)
		case 1049 + 3885: // Sacred Plate (Guardian & Templar)
		case 1074 + 3885: // Mamluk later greaves
		case 1472 + 3885: // Stygian Plate
			item.mirrorImage = 1; break;
		}
		if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
			switch (item.GraphicValue) {
			case 261 + 3885: // leather boots
				item.GraphicValue = 291 + 3885;
				sprintf(item.Name, "Insulated Boots");
				sprintf(item.FullMagicalItemName, "Insulated Boots");
				break;
			case 1911 + 168: // hard leather boots
				item.GraphicValue = 714 + 3885;
				break;
			case 1551 + 168: // thick leather boots
				item.GraphicValue = 715 + 3885;
				sprintf(item.Name, "Polar Boots");
				sprintf(item.FullMagicalItemName, "Polar Boots");
				break;
			}
		}
	}
	// ------------------------------------------------------------------------------------------------------
	// men: individual changes
	if (Players[CurrentPlayerIndex].fullClassId == PFC_WARRIOR) {
		if (item.GraphicValue == 1532 + 168) { // boots
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 286 + 3885;
				sprintf(item.Name, "Winter Boots");
				sprintf(item.FullMagicalItemName, "Winter Boots");
			}
			else{
				item.GraphicValue = 1 + 3885; // blue
			}
		}
		else if (item.GraphicValue == 712 + 168) { // griswold's bow
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 1556 + 168) { // hardened leather gloves
			item.GraphicValue = 431 + 3885;
		}
	}
	else if (Players[CurrentPlayerIndex].fullClassId == PFC_INQUISITOR) {
		if (item.GraphicValue == 1532 + 168) { // boots
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 286 + 3885;
				sprintf(item.Name, "Winter Boots");
				sprintf(item.FullMagicalItemName, "Winter Boots");
			}
			else {
				item.GraphicValue = 2 + 3885; // beige
			}
		}
		else if (item.GraphicValue == 1490 + 168) { // round shield
			item.GraphicValue = 63 + 3885;
		}
		else if (item.GraphicValue == 1056 + 168) { // wood rondache
			item.GraphicValue = RngFrom(645 + 3885, 1056 + 168);
		}
	}
	else if (Players[CurrentPlayerIndex].fullClassId == PFC_TEMPLAR) {
		if (item.GraphicValue == 759 + 168) { // serrated sword
			item.GraphicValue = 3337 + 168;
			sprintf(item.Name, "Sharpened Sword");
			sprintf(item.FullMagicalItemName, "Sharpened Sword");
		}
		else if (item.GraphicValue == 1942 + 168) { // sallet with camail
			item.GraphicValue = 3348 + 168;
			sprintf(item.Name, "Light Faceplate");
			sprintf(item.FullMagicalItemName, "Light Faceplate");
		}
		else if (item.GraphicValue == 1532 + 168) { // boots
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 286 + 3885;
				sprintf(item.Name, "Winter Boots");
				sprintf(item.FullMagicalItemName, "Winter Boots");
			}
			else {
				item.GraphicValue = 3 + 3885; // crimson
			}
		}
		else if (baseItemIndex == BI_62_RING_MAIL) { // ring mail
			item.GraphicValue = 712 + 3885;
		}
		else if (baseItemIndex == BI_1912_GREAT_FACEPLATE && Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
			item.GraphicValue = 1129 + 3885;
		}
	}
	else if (Players[CurrentPlayerIndex].fullClassId == PFC_MONK) {
		if (item.GraphicValue == 2041 + 168) { // ritual mask
			item.GraphicValue = RngFrom(3714 + 168, 569+3885); 
		}
		if (baseItemIndex == BI_404_LEATHER_DEMI_GAUNTLETS) {
			item.GraphicValue = 1099 + 3885;
			sprintf(item.Name, "Thick Cloth Wraps");
			sprintf(item.FullMagicalItemName, "Thick Cloth Wraps");
		}
	}
	else if (Players[CurrentPlayerIndex].fullClassId == PFC_KENSEI) {
		if (item.GraphicValue == 3170 + 168) { // tanto
			item.GraphicValue = 3338 + 168;
			sprintf(item.Name, "Tsurugi");
			sprintf(item.FullMagicalItemName, "Tsurugi");
		}
		else if (item.GraphicValue == 1926 + 168) { // light kanabo
			item.GraphicValue = 3339 + 168;
			sprintf(item.Name, "Kanabo");
			sprintf(item.FullMagicalItemName, "Kanabo");
		}
		else if (item.GraphicValue == 2041 + 168) { // ritual mask
			item.GraphicValue = 3376 + 168;
			sprintf(item.Name, "Mempo");
			sprintf(item.FullMagicalItemName, "Mempo");
		}
		else if (item.GraphicValue == 1962 + 168) { // steel facemask
			item.GraphicValue = 3625 + 168;
			sprintf(item.Name, "Open Kabuto");
			sprintf(item.FullMagicalItemName, "Open Kabuto");
		}
		else if (item.GraphicValue == 1956 + 168) { // plate mask
			item.GraphicValue = 3624 + 168;
			sprintf(item.Name, "Skull Kabuto");
			sprintf(item.FullMagicalItemName, "Skull Kabuto");
		}
		else if (item.GraphicValue == 1949 + 168) { // hardened steel mask
			item.GraphicValue = 3626 + 168;
		}
		else if (item.GraphicValue == 571 + 3885) { // faceplate mask
			item.GraphicValue = 572 + 3885;
		}
	}
	else if (Players[CurrentPlayerIndex].fullClassId == PFC_SAVAGE) {
		if (HasTrait(CurrentPlayerIndex, TraitId::Barbarian)) { // Barbarian's item looks:
			switch (item.GraphicValue) {
			case 1668 + 168: // siege helm
				item.GraphicValue = 3340 + 168;
				sprintf(item.Name, "Horned Helmet");
				sprintf(item.FullMagicalItemName, "Horned Helmet");
				break;
			case 2026 + 168: // nasal helm
				item.GraphicValue = 3341 + 168;
				sprintf(item.Name, "Horned Cap");
				sprintf(item.FullMagicalItemName, "Horned Cap");
				break;
			case 639 + 168: // barbute
				item.GraphicValue = RngFrom(3342 + 168, 710 + 3885);
				sprintf(item.Name, "Horned Battle Helmet");
				sprintf(item.FullMagicalItemName, "Horned Battle Helmet");
				break;
			case 1947 + 168: // nasal helm with aventail
				item.GraphicValue = 3343 + 168;
				sprintf(item.Name, "Barbaric Basset");
				sprintf(item.FullMagicalItemName, "Barbaric Basset");
				break;
			case 3344 + 168: // padded coif
				item.GraphicValue = 566 + 168;
				sprintf(item.Name, "Nomad Cap");
				sprintf(item.FullMagicalItemName, "Nomad Cap");
				break;
			case 1669 + 168: // burgonet
				item.GraphicValue = 738 + 168;
				sprintf(item.Name, "Winged Helm");
				sprintf(item.FullMagicalItemName, "Winged Helm");
				break;
			case 961 + 168: // darkened burgonet
				item.GraphicValue = 1054 + 168;
				sprintf(item.Name, "Tusked Helm");
				sprintf(item.FullMagicalItemName, "Tusked Helm");
				break;
			case 63 + 168: // gothic sallet
				item.GraphicValue = 1098 + 168;
				sprintf(item.Name, "Taurus Helm");
				sprintf(item.FullMagicalItemName, "Taurus Helm");
				break;
			case 1840 + 168: // fluted close helm
				item.GraphicValue = 1188 + 168;
				sprintf(item.Name, "Brute Helm");
				sprintf(item.FullMagicalItemName, "Brute Helm");
				break;
			case 1839 + 168: // gorgetted burgonet
				item.GraphicValue = 1233 + 168;
				sprintf(item.Name, "Warlord Helm");
				sprintf(item.FullMagicalItemName, "Warlord Helm");
				break;
			case 69 + 168: // hounskull bascinet
				item.GraphicValue = 2974 + 168;
				sprintf(item.Name, "Bloodlord Helm");
				sprintf(item.FullMagicalItemName, "Bloodlord Helm");
				break;
			case 1573 + 168: // mail coif
				item.GraphicValue = 1946 + 168;
				sprintf(item.Name, "Chain Coif");
				sprintf(item.FullMagicalItemName, "Chain Coif");
				break;
			case 1367 + 168: // full helm
				item.GraphicValue = 3672 + 168;
				break;
			case 1649 + 168: // reinforced armet
				item.GraphicValue = 272 + 3885;
				sprintf(item.Name, "Ancestor Helmet");
				sprintf(item.FullMagicalItemName, "Ancestor Helmet");
				break;
			case 1574 + 168: // iron hat
				item.GraphicValue = 683 + 3885;
				break;
			case 1642 + 168: // open bascinet
				item.GraphicValue = 684 + 3885;
				sprintf(item.Name, "Open Helm");
				sprintf(item.FullMagicalItemName, "Open Helm");
				break;
			case 1575 + 168: // cone helmet
				item.GraphicValue = 685 + 3885;
				break;
			case 122 + 168: // Nasal Helm with Aventail
				item.GraphicValue = 686 + 3885;
				sprintf(item.Name, "Nasal Helm");
				sprintf(item.FullMagicalItemName, "Nasal Helm");
				break;
			case 3420 + 168: // sallet with camail
				item.GraphicValue = 708 + 3885;
				sprintf(item.Name, "Orcish Helm");
				sprintf(item.FullMagicalItemName, "Orcish Helm");
				break;
			case 1419 + 168: // battle helm
				item.GraphicValue = 709 + 3885;
				break;
			case 1670 + 168: // close helmet
				item.GraphicValue = 711 + 3885;
				break;
			case 3419 + 168: // spangenhelm
				item.GraphicValue = 1130 + 3885;
				sprintf(item.Name, "Horned Battle Helm");
				sprintf(item.FullMagicalItemName, "Horned Battle Helm");
				break;
			case 1586 + 168: // camail
				item.GraphicValue = 1131 + 3885;
				sprintf(item.Name, "Barbaric Helm");
				sprintf(item.FullMagicalItemName, "Barbaric Helm");
				break;
			case 1944 + 168: // highlander helm
				item.GraphicValue = 1132 + 3885;
				sprintf(item.Name, "Dread Helm");
				sprintf(item.FullMagicalItemName, "Dread Helm");
				break;
			case 2375 + 168: // bycoque helm
				item.GraphicValue = 1133 + 3885;
				sprintf(item.Name, "Ironfang Helm");
				sprintf(item.FullMagicalItemName, "Ironfang Helm");
				break;
			case 270 + 3885: // grim bascinet
				item.GraphicValue = 1134 + 3885;
				sprintf(item.Name, "Barbaric Helm");
				sprintf(item.FullMagicalItemName, "Barbaric Helm");
				break;
			case 389 + 3885: // exceptional armet
				item.GraphicValue = 1135 + 3885;
				sprintf(item.Name, "Exceptional Helm");
				sprintf(item.FullMagicalItemName, "Exceptional Helm");
				break;
			case 452 + 3885: // pointed helm
				item.GraphicValue = 1136 + 3885;
				sprintf(item.Name, "Conqueror Helm");
				sprintf(item.FullMagicalItemName, "Conqueror Helm");
				break;
			case 455 + 3885: // Hell-Forged Helm
				item.GraphicValue = 1137 + 3885;
				break;
			case 448 + 3885: // Darksteel Helm
				item.GraphicValue = 1138 + 3885;
				break;
			case 449 + 3885: // Hardened Steel Armet
				item.GraphicValue = 1139 + 3885;
				sprintf(item.Name, "Barbaric Helm");
				sprintf(item.FullMagicalItemName, "Barbaric Helm");
				break;
			case 460 + 3885: // Gothic Barbute
				item.GraphicValue = 1140 + 3885;
				sprintf(item.Name, "Barbaric Helm");
				sprintf(item.FullMagicalItemName, "Barbaric Helm");
				break;
			case 462 + 3885: // Great Siege Helm
				item.GraphicValue = 1141 + 3885;
				break;
			case 465 + 3885: // Dark Helm
				item.GraphicValue = 1142 + 3885;
				break;
			}
		}
		if (item.GraphicValue == 263 + 3885) { // BANDED leather
			item.GraphicValue = 1110 + 168;
			sprintf(item.Name, "Hardened Leather");
			sprintf(item.FullMagicalItemName, "Hardened Leather");
		}
		else if (item.GraphicValue == 1532 + 168) { // boots
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 286 + 3885;
				sprintf(item.Name, "Winter Boots");
				sprintf(item.FullMagicalItemName, "Winter Boots");
			}
		}
	}
	else if (Players[CurrentPlayerIndex].fullClassId == PFC_GUARDIAN) {
		if (HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
			if (item.GraphicValue == 3344 + 168) { // padded coif
				item.GraphicValue = 256+3885;
				sprintf(item.Name, "Leather Hat");
				sprintf(item.FullMagicalItemName, "Leather Hat");
			}
			else if (item.GraphicValue == 1590 + 168) { // cloth tunic
				item.GraphicValue = 257 + 3885;
				sprintf(item.Name, "Light Tunic");
				sprintf(item.FullMagicalItemName, "Light Tunic");
			}
			else if (item.GraphicValue == 372 + 3885) { // lightened gambeson
				item.GraphicValue = 258 + 3885;
			}
			else if (item.GraphicValue == 1626 + 168) { // sash
				item.GraphicValue = 265 + 3885;
				sprintf(item.Name, "Leather Sash");
				sprintf(item.FullMagicalItemName, "Leather Sash");
			}
			else if (item.GraphicValue == 3311 + 168) { // cloth armor
				item.GraphicValue = 259 + 3885;
				sprintf(item.Name, "Aketon");
				sprintf(item.FullMagicalItemName, "Aketon");
			}
			else if (item.GraphicValue == 1137 + 168) { // cape
				item.GraphicValue = 260 + 3885;
				sprintf(item.Name, "Silk Tunic");
				sprintf(item.FullMagicalItemName, "Silk Tunic");
			}
			else if (item.GraphicValue == 2026 + 168) { // nasal helm
				item.GraphicValue = 281 + 3885;
				sprintf(item.Name, "Gunslinger Hat");
				sprintf(item.FullMagicalItemName, "Gunslinger Hat");
			}
			else if (item.GraphicValue == 1574 + 168) { // iron hat
				item.GraphicValue = 282 + 3885;
				sprintf(item.Name, "Combat Hat");
				sprintf(item.FullMagicalItemName, "Combat Hat");
			}
			else if (item.GraphicValue == 1911 + 168) { // hard leather boots
				if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) { // hard leather boots
					item.GraphicValue = 714 + 3885;
				}
				else { 
					item.GraphicValue = 283 + 3885; 
				}
			}
			else if (item.GraphicValue == 1551 + 168 && Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) { // thick leather boots
				item.GraphicValue = 715 + 3885;
				sprintf(item.Name, "Polar Boots");
				sprintf(item.FullMagicalItemName, "Polar Boots");
			}
			else if (item.GraphicValue == 1959 + 168) { // cervelliere
				item.GraphicValue = 287 + 3885;
				sprintf(item.Name, "Top Hat");
				sprintf(item.FullMagicalItemName, "Top Hat");
			}
			else if (item.GraphicValue == 1532 + 168) { // boots
				if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) { 
					item.GraphicValue = 286 + 3885;
					sprintf(item.Name, "Winter Boots");
					sprintf(item.FullMagicalItemName, "Winter Boots");
				}
				else {
					item.GraphicValue = 284 + 3885; // black
				}
			}
			else if (item.GraphicValue == 1497 + 168) { // leather demi gauntlets
				item.GraphicValue = 285 + 3885; 
				sprintf(item.Name, "Boiled Leather Gloves");
				sprintf(item.FullMagicalItemName, "Boiled Leather Gloves");
			}
		}
		if (item.GraphicValue == 2026 + 168) { // nasal helm
			item.GraphicValue = 1842 + 168;
			sprintf(item.Name, "Kettle Hat");
			sprintf(item.FullMagicalItemName, "Kettle Hat");
		}
		else if (item.GraphicValue == 372 + 3885) { // lightened gambeson
			item.GraphicValue = 273 + 3885;
		}
		else if (item.GraphicValue == 1959 + 168) { // cervelliere
			item.GraphicValue = 271 + 3885;
		}
		else if (item.GraphicValue == 1842 + 168) { // kettle hat
			item.GraphicValue = 3346 + 168;
			sprintf(item.Name, "Kettle Hat with Padded Coif");
			sprintf(item.FullMagicalItemName, "Kettle Hat with Padded Coif");
		}
		else if (item.GraphicValue == 1942 + 168) { // sallet with camail
			item.GraphicValue = 2887 + 168;
			sprintf(item.Name, "Kettle Hat with Aventail");
			sprintf(item.FullMagicalItemName, "Kettle Hat with Aventail");
		}
		else if (item.GraphicValue == 1419 + 168) { // battle helm
			item.GraphicValue = 2885 + 168;
			sprintf(item.Name, "Eisenhut Helm");
			sprintf(item.FullMagicalItemName, "Eisenhut Helm");			
		}
		else if (item.GraphicValue == 63 + 168) { // gothic sallet
			item.GraphicValue = 2881 + 168;
			sprintf(item.Name, "Full Plate Kettle Hat");
			sprintf(item.FullMagicalItemName, "Full Plate Kettle Hat");
		}
		else if (item.GraphicValue == 1649 + 168) { // reinforced armet
			item.GraphicValue = 2027 + 168;
			sprintf(item.Name, "Tower Helmet");
			sprintf(item.FullMagicalItemName, "Tower Helmet");
		}
		else if (item.GraphicValue == 3352 + 168) { // haubergeon
			item.GraphicValue = 3353 + 168;
			sprintf(item.Name, "Sleeveless Brigandine");
			sprintf(item.FullMagicalItemName, "Sleeveless Brigandine");
		}
		else if (item.GraphicValue == 3420 + 168) { // sallet with camail
			item.GraphicValue = 3628 + 168;
			sprintf(item.Name, "Kettlehat with Eyeslits");
			sprintf(item.FullMagicalItemName, "Kettlehat with Eyeslits");
		}
		else if (item.GraphicValue == 1532 + 168) { // boots
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 286 + 3885;
				sprintf(item.Name, "Winter Boots");
				sprintf(item.FullMagicalItemName, "Winter Boots");
			}
		}
	}
	else if (Players[CurrentPlayerIndex].fullClassId == PFC_SHUGOKI) {
		if (item.GraphicValue == 2041 + 168) {// Ritual Mask
			item.GraphicValue = 3563 + 168;
			sprintf(item.Name, "Mempo");
			sprintf(item.FullMagicalItemName, "Mempo");
		}
		else if (item.GraphicValue == 214 + 168) {// Wood Mask
			item.GraphicValue = 3565 + 168;
			sprintf(item.Name, "Oni Mask");
			sprintf(item.FullMagicalItemName, "Oni Mask");
		}
		else if (item.GraphicValue == 213 + 168) {// Death Mask
			item.GraphicValue = 3570 + 168;
			sprintf(item.Name, "Nightmare Mask");
			sprintf(item.FullMagicalItemName, "Nightmare Mask");
		}
		else if (item.GraphicValue == 1010 + 3885) {// Reaper Mask
			item.GraphicValue = 1144 + 3885;
			sprintf(item.Name, "Watcher Mask");
			sprintf(item.FullMagicalItemName, "Watcher Mask");
		}
		else if (item.GraphicValue == 3619 + 168) {// Imp Mask
			item.GraphicValue = 1770 + 168;
			//sprintf(item.FullMagicalItemName, "Imp Mask");
		}
		else if (item.GraphicValue == 1889 + 168) {// Leper Ironmask
			item.GraphicValue = 3569 + 168;
			sprintf(item.Name, "Blood Mask");
			sprintf(item.FullMagicalItemName, "Blood Mask");
		}
		else if (item.GraphicValue == 1885 + 168) {// Dragon Mask
			item.GraphicValue = 3566 + 168;
			sprintf(item.Name, "Ogre Mask");
			sprintf(item.FullMagicalItemName, "Ogre Mask");
		}
		else if (item.GraphicValue == 1962 + 168) {// Steel Facemask
			item.GraphicValue = 3378 + 168;
			sprintf(item.Name, "Deuce Mask");
			sprintf(item.FullMagicalItemName, "Deuce Mask");
		}
		else if (item.GraphicValue == 1956 + 168) {// Plate mask
			item.GraphicValue = 3567 + 168;
			sprintf(item.Name, "Demon Mask");
			sprintf(item.FullMagicalItemName, "Demon Mask");
		}
		else if (item.GraphicValue == 1949 + 168) {// Hardened Steel Mask
			item.GraphicValue = 3568 + 168;
			sprintf(item.Name, "Devil Mask");
			sprintf(item.FullMagicalItemName, "Devil Mask");
		}
		switch (baseItemIndex) {
		case 1798: item.GraphicValue = 1181 + 3885; break;
		case 1799: item.GraphicValue = 1182 + 3885; break;
		}
	}
	else if (Players[CurrentPlayerIndex].fullClassId == PFC_NECROMANCER) {
		if (item.GraphicValue == 2463 + 168) { // Summoner amulet
			item.GraphicValue = 3372 + 168;
			sprintf(item.Name, "Death Pendant");
			sprintf(item.FullMagicalItemName, "Death Pendant");
		}
		else if (item.GraphicValue == 1590 + 168) { // Cloth Tunic
			item.GraphicValue = 3629 + 168;
		}
		else if (item.GraphicValue == 1541 + 168) { // Cloth Gloves
			item.GraphicValue = 3651 + 168;
		}
	}
	else if (Players[CurrentPlayerIndex].fullClassId == PFC_DEMONOLOGIST) {
		if (item.GraphicValue == 2463 + 168) { // Summoner amulet
			item.GraphicValue = 3612 + 168;
		}
		else if (item.GraphicValue == 1590 + 168) { // Cloth Tunic
			item.GraphicValue = 3633 + 168;
		}
		if (item.GraphicValue == 252 + 168) { // Wizard Cape
			item.GraphicValue = 3650 + 168;
		}
	}
	else if (Players[CurrentPlayerIndex].fullClassId == PFC_BEASTMASTER) {
		if (item.GraphicValue == 2463 + 168) { // Summoner amulet
			item.GraphicValue = 3631 + 168;
		}
		else if (item.GraphicValue == 1590 + 168) { // Cloth Tunic
			item.GraphicValue = 3632 + 168;
		}
		else if (item.GraphicValue == 252 + 168) { // Wizard Cape
			item.GraphicValue = 3647 + 168;
		}
	}
	else if (Players[CurrentPlayerIndex].fullClassId == PFC_ELEMENTALIST) {
		if (item.GraphicValue == 252 + 168) { // Wizard Cape
			item.GraphicValue = 3648 + 168;
		}
		else if (item.GraphicValue == 556 + 168) { // Breast Plate (mamluk)
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 1606 + 168) { // Padded leather (mamluk)
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 51 + 168) { // Mamluke's Battle Bow
			item.mirrorImage = 1;
		}
		//else if (item.GraphicValue == 120) { // Mamluke's Great Bow
		//	item.mirrorImage = 1;
		//}
	}
	else if (Players[CurrentPlayerIndex].fullClassId == PFC_WARLOCK) {
		if (item.GraphicValue == 252 + 168) { // Wizard Cape
			item.GraphicValue = 3649 + 168;
		}
	}
	else if (Players[CurrentPlayerIndex].fullClassId == PFC_BERSERKER) {
		switch (item.GraphicValue) {
		case 106: // butchers cleaver			
			item.mirrorImage = 1;
			break;
		case 263 + 3885: // banded leather		
			item.GraphicValue = 107;
			break;
		case 261 + 3885: // leather boots	
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 291 + 3885;
				sprintf(item.Name, "Insulated Boots");
				sprintf(item.FullMagicalItemName, "Insulated Boots");
			}
			else {
				item.GraphicValue = 266 + 3885;
			}
			break;
		case 1911 + 168: 
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) { // hard leather boots
				item.GraphicValue = 714 + 3885;
			} 
			break;
		case 1551 + 168: 
			if(Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) { // thick leather boots
				item.GraphicValue = 715 + 3885;
				sprintf(item.Name, "Polar Boots");
				sprintf(item.FullMagicalItemName, "Polar Boots");
			}
			break;
		case 1626 + 168: // sash
			item.GraphicValue = 265 + 3885;
			sprintf(item.Name, "Leather Sash");
			sprintf(item.FullMagicalItemName, "Leather Sash");
			break;
		}
	}
	else if (Players[CurrentPlayerIndex].fullClassId == PFC_EXECUTIONER) {
		if (item.GraphicValue == 937) { // Arkaine's Valor Mail tier
			item.GraphicValue = 18 + 3885;
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 1005) { // Arkaine's Valor Plate tier
			//item.GraphicValue = 17+3885;
			item.GraphicValue = 467 + 168;
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 106) { // butchers cleaver			
			item.mirrorImage = 1;
		}
		else if (item.GraphicValue == 207 + 3885) { // ceremonial armor
			item.GraphicValue = RngFrom(653 + 3885, 207 + 3885);
		}
		else if (item.GraphicValue == 2536 + 168) { // cleaving knife
			item.GraphicValue = RngFrom(2536 + 168, 646 + 3885);
		}
		else if (item.GraphicValue == 1532 + 168) { // boots
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 286 + 3885;
				sprintf(item.Name, "Winter Boots");
				sprintf(item.FullMagicalItemName, "Winter Boots");
			}
		}
	}
	else if (Players[CurrentPlayerIndex].fullClassId == PFC_MURMILLO) {
		if (item.GraphicValue == 1532 + 168) { // boots
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 286 + 3885;
				sprintf(item.Name, "Winter Boots");
				sprintf(item.FullMagicalItemName, "Winter Boots");
			}
			else {
				item.GraphicValue = 1999 + 168; // blue
			}
		}
		else if (item.GraphicValue == 3399 + 168) { // cetratus
			item.GraphicValue = 3400 + 168;
		}
	}
	else if (is(Players[CurrentPlayerIndex].fullClassId, PFC_THRAEX, PFC_SECUTOR, PFC_DIMACHAERUS)) {
		if (item.GraphicValue == 1532 + 168) { // boots
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 286 + 3885;
				sprintf(item.Name, "Winter Boots");
				sprintf(item.FullMagicalItemName, "Winter Boots");
			}
		}
	}
	else if (is(Players[CurrentPlayerIndex].fullClassId, PFC_DRUID)) {
		switch (item.GraphicValue) {
		case 106: // butchers cleaver			
			item.mirrorImage = 1;
			break;
		}
	}
	/*-------------------------------------------------------------
	------        ITEM LOOKS BY IMAGE (GENERALIZED)      ----------
	-------------------------------------------------------------*/
	if (GameMode == GM_CLASSIC && item.baseItemIndex == 332) {
		item.GraphicValue = 1496 + 3885;
	}
	if (GameMode == GM_CLASSIC && item.baseItemIndex == 328) {
		item.GraphicValue = 88 + 168;
	}
	if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
		if (baseItemIndex == 2494) {
			item.GraphicValue = 459 + 168;
		}
		if (baseItemIndex == 419) {
			item.GraphicValue = 1479 + 3885;
		}
	}
	if (baseItemIndex == BI_68_THICK_LEATHER_ARMOR) { // mor: checking base item (instead of icon) might actually be wiser
		if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE) || is(Players[CurrentPlayerIndex].fullClassId, PFC_SAVAGE, PFC_BERSERKER)) {
			item.GraphicValue = 1090 + 3885;
		}
	}
	if (baseItemIndex == BI_867_TABARD && Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
		item.GraphicValue = 1090 + 3885;
		item.mirrorImage = 1;
	}
	if (item.GraphicValue == 554 + 168) { // Combat Axe (universally wrong (used by Berserker and Executioner) - always needs mirroring)
		item.mirrorImage = 1;
	}
	if (item.GraphicValue == 1137 + 168) { // cape. now has different looks for different mage subclasses
		if (Players[CurrentPlayerIndex].fullClassId == PFC_BEASTMASTER) {
			item.GraphicValue = 1136 + 168;
		}else if (Players[CurrentPlayerIndex].fullClassId == PFC_MAGE) {
			if( HasTrait(CurrentPlayerIndex, TraitId::Psion)){ item.GraphicValue = 3303 + 168; }
			else { item.GraphicValue = 3287 + 168; }
		}else if (Players[CurrentPlayerIndex].fullClassId == PFC_WARLOCK) {
			if (HasTrait(CurrentPlayerIndex, TraitId::Hydramancer)) { item.GraphicValue = 3287 + 168; }
			else { item.GraphicValue = 3288 + 168; }
		}else if (Players[CurrentPlayerIndex].fullClassId == PFC_DEMONOLOGIST) {
			item.GraphicValue = 3289 + 168;
		}else if (Players[CurrentPlayerIndex].fullClassId == PFC_NECROMANCER) {
			item.GraphicValue = 3290 + 168;
		}
	}
	if (item.GraphicValue == 3311 + 168 && Players[CurrentPlayerIndex].ClassID == PC_2_MAGE) { // cloth armor on all mages
		item.GraphicValue = 2600 + 168;
		sprintf(item.Name, "Cloak");
		sprintf(item.FullMagicalItemName, "Cloak");
	}
	if (item.GraphicValue == 261 + 3885 && Players[CurrentPlayerIndex].ClassID == PC_3_MONK) { // leather boots on all monks
		item.GraphicValue = 3356 + 168;
		sprintf(item.Name, "Field Boots");
		sprintf(item.FullMagicalItemName, "Field Boots");
	}
	if (item.GraphicValue == 2812 + 168) { // shortened dagger
		item.GraphicValue = RngFrom(2812 + 168, 619 + 3885);
	}
	if (item.GraphicValue == 2302 + 168) { // shortened knife
		item.GraphicValue = RngFrom(2302 + 168, 620 + 3885);
	}
	if (item.GraphicValue == 1532 + 168){
		if (Players[CurrentPlayerIndex].ClassID == PC_3_MONK) { // boots on all monks			
			item.GraphicValue = 267 + 3885;
		}
		else if (Players[CurrentPlayerIndex].fullClassId == PFC_BERSERKER) {
			if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE)) {
				item.GraphicValue = 286 + 3885;
				sprintf(item.Name, "Winter Boots");
				sprintf(item.FullMagicalItemName, "Winter Boots");
			}
			else {
				item.GraphicValue = 3602 + 168;
			}
		}
	}
	if (item.GraphicValue == 2822 + 168 && Players[CurrentPlayerIndex].fullClassId == PFC_ROGUE) { // concealed mail coif
		item.GraphicValue = 1887 + 168;
		sprintf(item.Name, "Chain Hood");
		sprintf(item.FullMagicalItemName, "Chain Hood");
	}
	if (item.GraphicValue == 53 + 168 && Players[CurrentPlayerIndex].fullClassId == PFC_ASSASSIN) { // lamellar jacket
		item.GraphicValue = 1968 + 168;
	}
	if (item.GraphicValue == 2803 + 168 && Players[CurrentPlayerIndex].fullClassId == PFC_SCOUT) { // THIN PLATE MAIL
		item.GraphicValue = 556 + 168;
		sprintf(item.Name, "Breastplate");
		sprintf(item.FullMagicalItemName, "Breastplate");
	}
	if (item.GraphicValue == 1066 + 168 && Players[CurrentPlayerIndex].ClassID == PC_0_WARRIOR) { // Reinforced Cuirass
		item.GraphicValue = 644 + 168;		
	}
	if (item.GraphicValue == 2389 + 168 && Players[CurrentPlayerIndex].fullClassId == PFC_SHINOBI) { // buckler
		item.GraphicValue = 200 + 168;
		sprintf(item.Name, "Wooden Buckler");
		sprintf(item.FullMagicalItemName, "Wooden Buckler");
	}
	if (item.GraphicValue == 1949 + 168 && Players[CurrentPlayerIndex].fullClassId == PFC_SHINOBI) { // hardened steel mask for Shinobi
		item.GraphicValue = 3377 + 168;
	}
	if (item.GraphicValue == 2390 + 168 && Players[CurrentPlayerIndex].fullClassId == PFC_ASSASSIN) { // small shield
		item.GraphicValue = 3440 + 168;		
	}
	//--------------RINGS--------------
	if (item.GraphicValue == 2606 + 168) { // ring  1
		item.GraphicValue = RngFrom(667 + 3885, 668 + 3885, 669 + 3885, 2606 + 168, 720 + 3885);
	}
	if (item.GraphicValue == 1659 + 168) { // ring  2
		item.GraphicValue = RngFrom(671 + 3885, 672 + 3885, 1659 + 168, 688 + 3885, 1201 + 3885);
	}
	if (item.GraphicValue == 1979 + 168) { // ring  10
		item.GraphicValue = RngFrom(673 + 3885, 675 + 3885, 1979 + 168, 1199 + 3885);
	}
	if (item.GraphicValue == 2605 + 168) { // ring  20
		item.GraphicValue = RngFrom(648 + 3885, 647 + 3885, 2605 + 168, 1200 + 3885);
	}
	if (item.GraphicValue == 2607 + 168) { // ring  30
		if (is(Players[CurrentPlayerIndex].ClassID, PC_0_WARRIOR, PC_1_ARCHER, PC_3_MONK, PC_5_SAVAGE) || HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
			item.GraphicValue = RngFrom(268 + 3885, 670 + 3885, 1202 + 3885);
		}
		else if (Players[CurrentPlayerIndex].fullClassId == PFC_ELEMENTALIST) {
			item.GraphicValue = 269 + 3885;
		}
	}
	if (item.GraphicValue == 1333 + 168) { // ring  40
		item.GraphicValue = RngFrom(676 + 3885, 1333 + 168);
	}
	if (item.GraphicValue == 1360 + 168) { // ring  50 (goldie)
		item.GraphicValue = RngFrom(650 + 3885, 649 + 3885, 1360 + 168);
	}
	if (item.GraphicValue == 740 + 168) { // ring  56
		item.GraphicValue = RngFrom(677 + 3885, 740 + 168);
	}
	if (item.GraphicValue == 1361 + 168) { // ring  61
		item.GraphicValue = RngFrom(678 + 3885, 1361 + 168);
	}
	if (item.GraphicValue == 88 + 3885) { // ring  64 (battle)
		item.GraphicValue = RngFrom(88 + 3885, 687 + 3885);
	}
	if (item.GraphicValue == 89 + 3885) { // ring  104 (battle)
		item.GraphicValue = RngFrom(89 + 3885, 689 + 3885);
	}
	if (item.GraphicValue == 91 + 3885) { // ring  144 (battle)
		item.GraphicValue = RngFrom(91 + 3885, 690 + 3885);
	}

	//--------------AMULETS--------------
	if (item.GraphicValue == 2598 + 168) { // amulet 2
		item.GraphicValue = RngFrom(679 + 3885, 2598 + 168, 705 + 3885, 706 + 3885, 719 + 3885);
	}
	if (item.GraphicValue == 2596 + 168) { // amulet 10
		item.GraphicValue = RngFrom(651 + 3885, 652 + 3885, 2596 + 168);
	}
	if (item.GraphicValue == 2597 + 168) { // amulet 21
		item.GraphicValue = RngFrom(680 + 3885, 2597 + 168);
	}
	if (item.GraphicValue == 1660 + 168) { // amulet 34
		item.GraphicValue = RngFrom(681 + 3885, 1660 + 168);
	}
	if (item.GraphicValue == 1990 + 168) { // amulet 54
		item.GraphicValue = RngFrom(682 + 3885, 1990 + 168);
	}
	if (item.GraphicValue == 93 + 3885) { // amulet  64 (battle)
		item.GraphicValue = RngFrom(93 + 3885, 691 + 3885);
	}
	if (item.GraphicValue == 102 + 3885) { // amulet  104 (battle)
		item.GraphicValue = RngFrom(102 + 3885, 692 + 3885);
	}
	if (item.GraphicValue == 103 + 3885) { // amulet  144 (battle)
		item.GraphicValue = RngFrom(103 + 3885, 692 + 3885, 693 + 3885);
	}

	// MONK BOWS
	if (Players[CurrentPlayerIndex].ClassID == PC_3_MONK) {
		switch (baseItemIndex) {
		case BI_232_LIGHT_BOW or BI_4_SMALL_BOW or BI_233_CRUDE_BOW or BI_143_SHORT_BOW or BI_234_LONG_BOW or BI_144_OAK_BOW 
			or BI_145_REFLEX_BOW or BI_147_SHORT_BATTLE_BOW or BI_148_LONG_BATTLE_BOW or BI_149_SHORT_WAR_BOW 
			or BI_150_LONG_WAR_BOW or BI_164_ELITE_BOW or BI_163_LONG_HEAVY_BOW or BI_180_PRECISION_BOW or BI_209_ETHEREAL_BOW 
			or BI_1982_CRYPTIC_BOW or BI_1983_GRIM_BOW or BI_1984_RUNE_BOW or BI_1985_DARK_BOW or BI_1987_REINFORCED_WARBOW:
			item.mirrorImage = 1; break;
		}
	}
	else if (HasTrait(CurrentPlayerIndex, TraitId::Ranger)) {
		switch (baseItemIndex) {
		case 1362 or 1363 or 1364 or 1365 or 1366 or 1367 or 1368 or 1369 or 1370 or 1371 or 1372:	item.mirrorImage = 1; break;
		}
	}

	// Restore random after pic selection
	RandUsingCounter = randUsingCounter;
	RandomSeed = randomSeed;
	LastSeed = lastSeed;

	//if( IsAltPressed ){
	//	item.mirrorImage = 1;
	//}

	if( (RelixToScrolls || GameMode == GM_CLASSIC) && is(item.MagicCode, MC_21_RELIC_NEED_NO_TARGET, MC_22_RELIC_NEED_TARGET) ){
		item.GraphicValue = 346 + 168;
		if( memcmp(item.Name,                "Relict", 6) == 0 ) memcpy(item.Name,                "Scroll", 6);
		if( memcmp(item.FullMagicalItemName, "Relict", 6) == 0 ) memcpy(item.FullMagicalItemName, "Scroll", 6);
	}
}

// TODO: convert all item adjusting data from ifs to switch/table, too much CPU usage in this func
// TODO: combine with SetPlayerHandItem (00424B17)
//----- (004257B4) Clear and set base item params -------------------------
void __fastcall GetItemAttrs(int itemIndex, int baseItemIndex, int qlvl)
{
	//RandomSeed = 12345678;
	Item& item = Items[itemIndex];
	BaseItem& baseItem = BaseItems[baseItemIndex];
	item.saveVersion = CurSaveVersion;
	item.Identified = 0;
	item.fixedReqClvl = 0;
	item.dropping = 0;
	item.playAnimation = 0;
	item.ItemCode = (ITEM_CODE)baseItem.ItemCode;
	item.GraphicValue = baseItem.GraphicValue;
	strcpy(item.Name, baseItem.NamePtr);
	strcpy(item.FullMagicalItemName, baseItem.NamePtr);
	item.EquippedLocation = baseItem.EquippedLocation;
	item.TypeID = baseItem.ItemType;
	item.throwing = (char)baseItem.throwing;
	item.MinDamage = RngFromRange(baseItem.MinDamageLow, baseItem.MinDamageHigh);
	item.MaxDamage = RngFromRange(baseItem.MaxDamageLow, baseItem.MaxDamageHigh);
	item.ArmorClass = RngFromRange(baseItem.MinimumArmor, baseItem.MaximumArmor);
	item.damageFromEnemies = RngFromRange(baseItem.MinDamageFromEnemy, baseItem.MaxDamageFromEnemy);
	item.minMeleeAbsorbPercent = RngFromRange(baseItem.MinMeleeAbsorbPercentLow, baseItem.MinMeleeAbsorbPercentHigh);
	item.maxMeleeAbsorbPercent = RngFromRange(baseItem.MaxMeleeAbsorbPercentLow, baseItem.MaxMeleeAbsorbPercentHigh);
	item.minArrowAbsorbPercent = RngFromRange(baseItem.MinArrowAbsorbPercentLow, baseItem.MinArrowAbsorbPercentHigh);
	item.maxArrowAbsorbPercent = RngFromRange(baseItem.MaxArrowAbsorbPercentLow, baseItem.MaxArrowAbsorbPercentHigh);
	item.minThorns = RngFromRange(baseItem.MinThornsLow, baseItem.MinThornsHigh);
	item.maxThorns = RngFromRange(baseItem.MaxThornsLow, baseItem.MaxThornsHigh);
	item.MagicCode = baseItem.MagicCode;
	item.SpellIndex = baseItem.SpellNumber;
	item.MagicLevel = ML_0_USUAL;
	auto randomSeed = RandomSeed; // prevent morphing from adding random basic price
	item.Price = RngFromRange(baseItem.MinPrice, baseItem.MaxPrice);
	item.basePrice = item.Price;
	RandomSeed = randomSeed;
	item.priceEffect = 0;
	item.basePriceMultiplier = 0;
	item.CurCharges = 0;
	item.BaseCharges = 0;
	int durability = RngFromRange(baseItem.DurabilityMin, baseItem.DurabilityMax);
	item.CurDurability = durability;
	item.BaseDurability = durability;
	item.RequiredStrength = RngFromRange(baseItem.RequiredStrengthMin, baseItem.RequiredStrengthMax);
	item.RequiredMagic = RngFromRange(baseItem.RequiredMagicMin, baseItem.RequiredMagicMax);
	item.RequiredDexterity = RngFromRange(baseItem.RequiredDexterityMin, baseItem.RequiredDexterityMax);
	item.RequiredVitality = RngFromRange(baseItem.RequiredVitalityMin, baseItem.RequiredVitalityMax);
	item.RequiredLevel = RngFromRange(baseItem.RequiredLevelMin, baseItem.RequiredLevelMax);
	item.baseItemIndex = baseItemIndex;
	item.uniqIndexSeed = 0;
	item.gottenFromLand = 0;
	each(item.enchantIndexes) = -1;
	item.quenchAffix = QA_NO;
	item.quenchLevel = 0;
	item.overTime = 0;
	if (baseItem.ItemCode == IC_18_FLASK) {
		FlaskInfo& flask = FlasksInfo[baseItem.FlaskOrTrapCode];
		item.overTime = flask.MinOverTime || flask.MaxOverTime ? RngFromRange(flask.MinOverTime, flask.MaxOverTime) : 0;
	}
	memzero(item.effect); // в перпективе это поле заменит ВСЕ остальные поля с эффективными характиристиками предмета
	each(item.effect).id = AE_NO;

	item.isShadow = false;
	item.socketsAmount = 0;
	item.socketsBase = 0;
	item.socketsAdded = 0;
	each(item.sockets) = 0;
	item.difficulty = 0;
	item.charFlag = 0;
	item.mirrorImage = 0;
	item.subType = 0;
	item.grade = 0;

	SetItemGrade(item);
	AdjustItemLook(item);

	if( item.MagicCode == MC_24_BOOKS ){
		Item_InitBooks(itemIndex, qlvl);
	}
	if( item.MagicCode == MC_30_OIL_OF_SOMETHING ){
		Item_InitOils(itemIndex, qlvl);
	}
	int goldAmount = GetGoldModificator();
	if( item.ItemCode == IC_11_GOLD ){
		// 004259BF
		switch (Difficulty ){
		case DL_1_PURGATORY:		goldAmount += 46;	break;
		case DL_2_DOOM:				goldAmount += 91;	break;
		case DL_3_CRUCIBLE:			goldAmount += 136;	break;
		case DL_4_TORMENT:			goldAmount += 181;	break;
		case DL_5_AGONY:			goldAmount += 226;	break;
		case DL_6_TERROR:			goldAmount += 271;	break;
		case DL_7_DESTRUCTION:		goldAmount += 316;	break;
		case DL_8_INFERNO:			goldAmount += 361;	break;
		}
		goldAmount += RNG(34);

		// JG for Classic
		//The amount of gold dropped or found on the dungeon floor is determined by the formulas below :
		//Normal difficulty : 5·dlvl to 15·dlvl - 1
		//Nightmare difficulty : 5·(16 + dlvl) to 15·(16 + dlvl) - 1
		//Hell difficulty : 5·(32 + dlvl) to 15·(32 + dlvl) - 1

		if(GameMode == GM_CLASSIC) {
			int dlvl = Dungeon->level;
			switch (Difficulty) {
			case DL_0_HORROR:		goldAmount = RngFromRange(dlvl * 5,			15 * dlvl - 1); break;
			case DL_1_PURGATORY:	goldAmount = RngFromRange((dlvl + 16) * 5,	15 * (dlvl+16) - 1); break;
			case DL_2_DOOM:			goldAmount = RngFromRange((dlvl + 32) * 5,	15 * (dlvl+32) - 1); break;
			}
		}

//		switch (NetPlayerCount ){
//		case 2:	goldAmount += goldAmount / 2;	break;
//		case 3:	goldAmount += goldAmount;	break;
//		case 4:	goldAmount += goldAmount + goldAmount / 2;	break;
//		}
		LimitToMax(goldAmount, 5000);
		item.amount = goldAmount;
		SetGraphicToGold(item);// оптимизация
	}
	
    // Prevent random generator from damage
    auto randUsingCounter = RandUsingCounter;
    randomSeed = RandomSeed;
    auto lastSeed = LastSeed;
    for( int i = 0; i < baseItem.EffectCount; ++i ){
        const int effectIndex = 20 + i;
        Item_ApplyAffix(item, baseItem.Effects[i], 0, 0, 1, effectIndex, 0, 0, true);
		item.effect[effectIndex].origin = AO_BASE;
    }
    RandUsingCounter = randUsingCounter;
    RandomSeed = randomSeed;
    LastSeed = lastSeed;
}

//----- (00425A76) --------------------------------------------------------
int __fastcall RngFromRange( int min, int max )
{
	if( min > max ){ int tmp = min; min = max; max = tmp; }
	return min + RNG(max - min + 1);
}

//----- (00425A76) --------------------------------------------------------
float __fastcall RngFromRangePercent( float min, float max )
{
	int imin = int(min * 10000); // 2 digit after dot precision
	int imax = int(max * 10000);
	if( imin > imax ){ int tmp = imin; imin = imax; imax = tmp; }
	return 0.0001f * (imin + RNG(imax - imin + 1));
}

//----- (00425A87) --------------------------------------------------------
int __fastcall GetEffectPrice( int effectValue, int effectMin, int effectMax, int goldMin, int goldMax )
{
	if( effectMin == effectMax || goldMin == goldMax ){
		return goldMin;
	}else{
		return goldMin + (goldMax - goldMin) * (100 * (effectValue - effectMin) / (effectMax - effectMin)) / 100;
	}
}

//----- (th2) -------------------------------------------------------------
void Item_ApplyAffixIdentified(Item& i, BaseEffect be, int goldMin, int goldMax, int multiplier, int effectIndex, int affixMinReqClvl, int affixMaxReqClvl)
{
	const Player& player = Players[CurrentPlayerIndex]; // TODO: move out all player dependences to smart drop filter
	Effect& e = i.effect[effectIndex];
	e.id = be.id; e.type = be.type;
	e.minVal = e.maxVal = e.chance = 0;
	int priceVal = 0, priceMin = 0, priceMax = 0;
	const AffixEffect& ae = AffixEffect::Table[be.id];

	if( ae.use & AffixEffect::Min ){
		e.minVal = RngFromRange( be.minLow, be.minHigh );
		priceVal = e.minVal;
		priceMin = be.minLow;
		priceMax = be.minHigh;
	};
	if( ae.use & AffixEffect::Max ){
		e.maxVal = RngFromRange( be.maxLow, be.maxHigh );
		priceVal = e.maxVal; // можно переделать на цену по среднему
		priceMin = be.maxLow;
		priceMax = be.maxHigh;
	};
	if( ae.use & AffixEffect::Chance ){
		e.chance = RngFromRange( be.chanceLow, be.chanceHigh );
		if( ! priceMax ){
			priceVal = e.chance;
			priceMin = be.chanceLow;
			priceMax = be.chanceHigh;
		}
	};
	
	switch( be.id ){
	case AE_SPECIAL_AC	        : i.ArmorClass       = e.minVal; break; // особый случай, устанавливает фикс, вместо прибавления
	case AE_UNUSUAL_ITEM_DAMAGE	: i.MinDamage        = be.minLow; i.MaxDamage = be.minHigh; break; // особый случай, устанавливает фикс, вместо прибавления
	case AE_UNUSUAL_DURABILITY	: i.CurDurability    = be.minLow; i.BaseDurability = be.minLow; break; // особый случай, устанавливает фикс, вместо прибавления
	case AE_REQUIRED_CLVL	    : i.RequiredLevel    = (uchar)e.minVal; i.fixedReqClvl = 1; break;  // особый случай, устанавливает фикс, вместо прибавления
	case AE_EXTRA_CHARGES       : i.CurCharges      += e.minVal; i.BaseCharges += e.minVal; break;
	case AE_SPELL	            : i.SpellIndex       = (char) e.type; i.CurCharges = e.minVal; i.BaseCharges = e.minVal; break; // требует переработки в массив или убрать в CalcChar
	case AE_HIGH_DURABILITY	    : addTo(i.BaseDurability * e.minVal / 100, i.BaseDurability, i.CurDurability); break;
	case AE_DECREASED_DURABILITY: i.BaseDurability  -= i.BaseDurability * e.minVal / 100; i.CurDurability = i.BaseDurability; break;
	case AE_INDESTRUCTIBLE_DIS	: i.CurDurability    = i.BaseDurability = ITEM_DUR_INDESTRUBLE; break;
	case AE_LOW_DUR_ADD_DAMAGE	: i.BaseDurability  -= i.BaseDurability * e.minVal / 100; i.CurDurability = i.BaseDurability; break;
	case AE_ONE_HANDED_STAFF	: i.EquippedLocation = EL_1_ONE_HANDED; break;
	case AE_NO_STR_REQ	        : i.RequiredStrength = 0; break;
	case AE_UNIQUE_GRAPHIC	    : i.GraphicValue     = be.minLow; break;
	case AE_SOCKET              : i.socketsBase      = (uchar)e.minVal; break;
	case AE_ATTACK_SPEED or AE_CAST_SPEED or AE_HIT_RECOVERY
	  or AE_MANA_STEAL_PERCENT or AE_LIFE_STEAL_PERCENT: e.minVal = be.minLow; break; // "Max flag" effects, still does not use min low/high value ranges

	// ====== обдуманные =================================

	// transform fire/ligning hit on weapons to one hit and X-Y damage!
	// частично реализовано, возможно надо добавить удаление закла после нанесения им урона

	// ====== еще не обработанные ====================================

	// корс это отдельная магия вообще
	// она используется в тх как способ обезвредить монстра на время, превратив в неуязвимый камень
	// чисто тактическое решение
	// иногда умелый игрок может корсанными монстрами отгородиться от других монстров и расстреливать вторых через корсанных, выиграв время
	// там много применений
	// просто это для тех кто играет тактично а не пытаясь закидать ДПСом мобов
	// в дарке был еще взгляд медузы, который вызывал окаменение на время всех монстров в радиусе зрения игрока.
	// ну если неуязвимый, то да, это на холод не похоже, проклятья как отдельный типа магии тоже интересно
	// к слову такой тип магии можно будет и назвать проклятиями и нечто похожее как у некроманта в д2 сделать.
	// добавив другие ослабления монстрам в этот вид магии
	// типа + Х % урона монстр получает, -Х % урона - наносит, итп
	// кстати да!замечательная идея так если вдуматься
	// у колда вижу может быть или иммун или зеленый свет..так и у пойзона
	// это надо исправить чтобы были отдельно от корса и резист к колду, и иммун.и у пойзона тоже.и резист, и иммун
	// в противном случае монстры от колда и пойзона начнут огребать 400 % урон
	// и нахер никому не нужными станут любые спеллы физики, огня, магии и молнии
	
	// отдельным пунктом, значительным улучшением могло бы быть если бы ты заменил флаговое присвоение резиста - значением.
	// и можно было бы задавать процент сопротивления гибко.хоть от 1 % и до 99 %
	// ладно.раз к вечеру то к вечеру
	// всмысле у монстров ? я уже думал над этим, и хотел тебя спросить, надо или нет
	// да.у монстров.конечно
	// дженерику, бланту, шарпу, и стрелам / болтам, было бы идеально
	// было бы на самом деле просто обалденно, чего уж там
	// потому что ситуация такая.смотри.по мертвым оружие дробящее дает больше урона, а рубящее - меньше.
	// но это подразумевалось что мертвыми будут скелеты.а кости надо крошить а не резать

	// но в тх2 есть и мертвые которые из гнилой плоти.зомби, гротески.
	// и против них явно напрашивается как раз рубящее.

	// а всякие демоны и големы..по любой логике их лучше должно брать дробящее оружие.
	// вазу или камень ведь легче разбить каким - нибудь молотом
	// ну какое то разделение по урону от вида оружия уже есть для мертвяков
	// вот выше я и объяснил чем текущая ситуация не идеаль на
	// ну то есть чтобы не в коде задавать различие, а в таблице монстров и желательно по процентно
	// да
	// понял
	// еще вдогонку.можно сделать отрицательные значения ? чтобы они давали повышение урона
	// ну да, поля добавлю знаковые, будут и отрицательные
	// 50 %
	// отлично.больше вопросов по этой теме тогда нет
	}
	
	// 00426613
	i.priceEffect += GetEffectPrice(priceVal, priceMin, priceMax, goldMin, goldMax);
	i.basePriceMultiplier += multiplier;

	if( ! i.fixedReqClvl ){ // if item does not have fixed clvl
		LimitToMin(i.RequiredLevel, RngFromRange(affixMinReqClvl, affixMaxReqClvl));
	}

	// весь этот блок лимитов был добавлен давно и по ныне неизвестной причиние
	// но пусть пока побудет тут закомменченым в назидание что такие лимиты в этом месте не нужны
	//LimitToMin( i.ResistFire, 0 );
	//LimitToMin( i.ResistLightning, 0 );
	//LimitToMin( i.ResistArcan, 0 );
	//LimitToMin( i.BaseDurability, 1 ); // ставилось 1 вместо положенного 0 у неразрушаемых предметов
	//LimitToMin( i.CurDurability, 1 );
}

//----- (00425AC7) --------------------------------------------------------
void __fastcall Item_ApplyAffix(Item& item, BaseEffect be, int goldMin, int goldMax, int multiplier, int effectIndex, int affixMinReqClvl, int affixMaxReqClvl, bool forceApply )
{
	if( forceApply || item.Identified ){
		Item_ApplyAffixIdentified( item, be, goldMin, goldMax, multiplier, effectIndex, affixMinReqClvl, affixMaxReqClvl );
	}else{
		// We need apply only durability, charges and required based effects to the non identified item
		Item tmpItem = item;
		Item_ApplyAffixIdentified( tmpItem, be, goldMin, goldMax, multiplier, effectIndex, affixMinReqClvl, affixMaxReqClvl);
		item.BaseDurability    = tmpItem.BaseDurability;
		item.CurDurability     = tmpItem.CurDurability;
		item.BaseCharges       = tmpItem.BaseCharges;
		item.CurCharges        = tmpItem.CurCharges;
		item.SpellIndex        = tmpItem.SpellIndex;
		item.RequiredStrength  = tmpItem.RequiredStrength;
		item.RequiredMagic     = tmpItem.RequiredMagic;
		item.RequiredDexterity = tmpItem.RequiredDexterity;
		item.RequiredVitality  = tmpItem.RequiredVitality;
		item.RequiredLevel     = tmpItem.RequiredLevel;
		item.fixedReqClvl      = tmpItem.fixedReqClvl;
	}
}

// magic item prefix & suffix generation 
//----- (004267F0) --------------------------------------------------------
void __fastcall Item_AddAffixes(Item& item, int minQLVL, int maxQLVL, int occuranceMask, int specializationMask, int isGoodMagicItem )
{
	int enablePostfix, disablePrefix;
	uchar prefixExcludedCombinations = 0;
	vector<int> affixMap;
	affixMap.reserve(1000);
	Affix* affixes = CurAffixesMagic();
	Affix* prefix;
	Affix* post;
	disablePrefix = RNG(4);           //	вероятность префикса 0.25
	enablePostfix = RNG(3);           //	вероятность постфикса 0.66(6)
	if( disablePrefix && !enablePostfix ){ //	если не получилось ни префикса не постфикса то что нибудь всё равно включаем
		if( RNG(2) ){
			enablePostfix = 1;
		}else{
			disablePrefix = 0;
		}
	}
	int prefixIndex = -1;
	int postfixIndex = -1;
	if( !isGoodMagicItem && RNG(3) ){
		isGoodMagicItem = 1;
	}
	int prefixId = 0;
	if( !disablePrefix ){
		for( ; affixes[prefixId].effect.id != AE_NO; ++prefixId ){
			prefix = &affixes[prefixId];
			if( (occuranceMask & prefix->item) && (specializationMask & prefix->itemSpecialization) ){
				if( prefix->quality >= minQLVL && prefix->quality <= maxQLVL ){
					if( !isGoodMagicItem || prefix->NotCursed ){
						if( occuranceMask != IA_STAFF /*Staff wo cast*/ || prefix->effect.id != AE_EXTRA_CHARGES ){
							affixMap.push_back(prefixId);
							if( prefix->DoubleChance )
								affixMap.push_back(prefixId);
						}
					}
				}
			}
		}
		if( affixMap.size() ){
			prefixIndex = affixMap[RNG(affixMap.size())];
			//.text:0x004269D8 4A4 8B 84 85 6C FB FF FF                 mov     eax, [ebp+eax*4+AffixMap]
			//prefixIndex = 78; // test new affix
			prefix = &affixes[prefixIndex];
			sprints(item.FullMagicalItemName, "%s %s", prefix->Name, item.FullMagicalItemName);
			item.MagicLevel = ML_1_MAGIC;
			BaseEffect e = prefix->effect;
			Item_ApplyAffix(item, e, prefix->MinGoldValue, prefix->MaxGoldValue, prefix->Multiplier, 0, prefix->minReqClvl, prefix->maxReqClvl, false );
			prefixExcludedCombinations = prefix->ExcludedCombinations;
		}
	}else{
		for( ; affixes[prefixId].effect.id != AE_NO; ++prefixId );
	}
	int firstSuffix = prefixId + 1;
	affixMap.clear();
	if( enablePostfix ){
		for( int suffixId = 0; affixes[suffixId + firstSuffix].effect.id != AE_NO; ++suffixId ){
			post = &affixes[suffixId + firstSuffix];
			//if( (int)post->Name == 1 ) __debugbreak();
			if( (occuranceMask & post->item) && (specializationMask & post->itemSpecialization) ){
				if( post->quality >= minQLVL && post->quality <= maxQLVL ){
					if( (prefixExcludedCombinations | post->ExcludedCombinations ) != (16 | 1) ){
						if( !isGoodMagicItem || post->NotCursed ){
							affixMap.push_back(suffixId);
						}
					}
				}
			}
		}
		if( affixMap.size() ){
			//.text:0x004269C6 4A4 85 D2                                test    edx, edx // mapSize
			postfixIndex = affixMap[RNG(affixMap.size())];
			//.text:0x004268DD 4A4 8B 84 85 6C FB FF FF                 mov     eax, [ ebp + eax * 4 + AffixMap ]
			post = &affixes[postfixIndex + firstSuffix];// original 87
			sprints(item.FullMagicalItemName, "%s of %s", item.FullMagicalItemName, post->Name);
			item.MagicLevel = ML_1_MAGIC; // TODO: move upper func
			BaseEffect e = post->effect;
			Item_ApplyAffix(item, e, post->MinGoldValue, post->MaxGoldValue, post->Multiplier, 1, post->minReqClvl, post->maxReqClvl, false );
		}
	}
	if( !CheckLetterWidth(item.FullMagicalItemName) ){
		if( BaseItems[item.baseItemIndex].Name2 ){
			strcpy(item.FullMagicalItemName, BaseItems[item.baseItemIndex].Name2);
		}else{
			item.Name[0] = 0;
		}
		if( prefixIndex != -1 ){
			sprints(item.FullMagicalItemName, "%s %s", affixes[prefixIndex].Name, item.FullMagicalItemName);
		}
		if( postfixIndex != -1 ){
			sprints(item.FullMagicalItemName, "%s of %s", item.FullMagicalItemName, affixes[postfixIndex + firstSuffix].Name);
		}
	}
	if( prefixIndex != AE_NO || postfixIndex != AE_NO ){
		Item_RecalculatePrice(item);
	}
}

//----- (th2) -------------------------------------------------------------
ITEM_AFFIX __fastcall ItemAffix(Item& item)
{
	bool _1h = item.EquippedLocation == EL_1_ONE_HANDED;
	switch( item.ItemCode ){
	case IC_1_SWORD      :return _1h ? IA_SWORD_1H : IA_SWORD_2H;
	case IC_2_AXE        :return IA_AXE;
	case IC_4_MACE       :return _1h ? IA_MACE_1H : IA_MACE_2H;
	case IC_3_BOW        :return IA_BOW;
	case IC_18_FLASK     :return IA_FLASK;
	case IC_5_SHIELD     :return IA_SHIELD;
	case IC_6_LIGHT_ARMOR:return IA_LIGHT_ARMOR;
	case IC_7_HELM       :return IA_HELM;
	case IC_8_MID_ARMOR  :return IA_MID_ARMOR;
	case IC_9_HEAVY_ARMOR:return IA_HEAVY_ARMOR;
	case IC_17_BELT      :return IA_BELT;
	case IC_15_GLOVE     :return IA_GLOVE;
	case IC_16_BOOTS     :return IA_BOOTS;
	case IC_10_STAFF     :return IA_STAFF;
	case IC_12_RING      :return IA_RING;
	case IC_13_AMULET    :return IA_AMULET;
	case IC_20_CLAW      :return IA_CLAW;
	case IC_19_TRAP      :return IA_TRAP;
	case IC_21_KNIFE     :return IA_KNIFE;
	case IC_22_MALLET    :return IA_MALLET;
	case IC_23_PISTOL    :return IA_PISTOL;
	default              :return IA_ALL;
	}
}

//----- (00426B32) --------------------------------------------------------
void __fastcall ApplyAffixesToItem(int itemIndex, int baseItemIndex, int minQuality, int maxQuality, int isGoodMagicItem, char enableSpellCastAbility)
{
	Item& item = Items[itemIndex];
    BaseItem& baseItem = BaseItems[item.baseItemIndex];
	if( item.TypeID != ITEM_4_GOLD ){
		//if (minQuality > 25) { minQuality = 25; }
        ITEM_AFFIX itemAffix = ItemAffix(item);
		if( itemAffix == IA_ALL ) return;
        if( item.ItemCode == IC_10_STAFF && baseItem.Specialization & IS_MAGIC ){
            if( enableSpellCastAbility && (enableSpellCastAbility != 2 || RNG(3) == 1 ) ){
                Item_AddSpellCastAbility(item, maxQuality, isGoodMagicItem);
            }else{
                Item_AddAffixes(item, minQuality, maxQuality, itemAffix, baseItem.Specialization, isGoodMagicItem);
            }
        }else{
            Item_AddAffixes(item, minQuality, maxQuality, itemAffix, baseItem.Specialization, isGoodMagicItem);
        }
	}
}

//----- (00426BE0) --------------------------------------------------------
void __fastcall SetupItemAnim(int itemIndex)
{
	Item& item = Items[itemIndex];
	int flip = ItemFlipTable[item.GraphicValue];
	item.ItemCELFilePtr = FlipItemCELFiles[flip];
	item.FramesCount = Flip_FramesCount[flip];
	item.AnimWidth = 96;
	item.subOffsetX = 16;
	//item.Identified = 0;
	item.dropping = 0;
	//item.gottenFromLand = 0; возможно это защита от дублирования в курсоре
	Player& player = Players[CurrentPlayerIndex];
	if( player.SomeSoundMuteDelay ){
		item.FrameIndex = item.FramesCount;
		item.playAnimation = 0;
		item.animPhase = 1;
	}else{
		item.FrameIndex = 1;
		item.playAnimation = 1;
		item.animPhase = 0;
	}
}

//----- (00426C7C) --------------------------------------------------------
int __fastcall GetRandomBaseForMonsterDrop( int monsterIndex )
{
	//return 240; // дроп масла для ремонта для сверки
	vla( /*BASE_ITEM*/int, goodItems, GOOD_ITEM_COUNT);
	int goodItemsCount = 0;

	Monster& monster = Monsters[monsterIndex];
	ushort specialDrop = monster.BasePtr->ItemDropSpecials;

	//goto elix; // проверял выпадание эликсиров
	if( specialDrop & 0x8000 ){
		return -1 - (specialDrop & 0xFFF);// дроп уника
	}
	if( specialDrop & 0x4000 || RNG(100) > 40 ){
		return 0;// нет дропа
	}
	if( RNG(100) > 25 ){
		// шанс выпадения золота дополнительно уменьшается в 3 раза
		return RNG(100) < by(Difficulty, 43, 29, 17, 16, 15, 14, 13, 12, 11 );
	}
	
	PLAYER_FULL_CLASS fullClassId = Players[CurrentPlayerIndex].fullClassId;
	
	//elix: // проверял выпадание эликсиров
	int mode = (GameMode == GM_CLASSIC) + 1;
	for( uint baseItemIndex = 0; goodItemsCount < GOOD_ITEM_COUNT && baseItemIndex < count_BaseItems; ++baseItemIndex ){
		BaseItem& baseItem = BaseItems[baseItemIndex];
		if( !(baseItem.mode + 1 & mode) ) continue;
		int spell = baseItem.SpellNumber;
		int chanceToDrop = baseItem.ChanceToDrop;
		int oilQuality = IsOilQuality( baseItemIndex );
		if( oilQuality && chanceToDrop == 2 ){// TODO: плохо сделано. Точно будут лаги
			// mor: (2023-01-16) seems that it doesn't check for base item level after Purgatory here, so I commented out difficulty check in this condition
			if( /*Difficulty >= DL_2_DOOM ||*/ monster.MonsterLevel >= baseItem.itemLevel) { // did not change that yet, just fixed for now
				if( goodItemsCount < GOOD_ITEM_COUNT ){
                    if( MaxCountOfPlayersInGame == 1 && (spell == PS_32_BONE_SPIRIT_REAL_RESSURECT || spell == PS_34_HEAL_OTHER) ){
                        // Forbidden in mp
                    }
					else if( isTownPortalForbidden() && ( spell == PS_7_TOWN_PORTAL ) ){
                        // Forbidden in NM & IM game modes
                    }
					else{
                        goodItems[goodItemsCount++] = /*(BASE_ITEM)*/ baseItemIndex;
                    }
				}
			}
		}

		if( oilQuality && chanceToDrop && monster.MonsterLevel >= baseItem.itemLevel ){
			// if( MaxCountOfPlayersInGame == 1) // MP smart drop
			{
				Player& player = Players[CurrentPlayerIndex];//GetCurrentPlayerOffset();
				// mor: new exclusions ('23-1-23). copied from boss routine. watch it!
				if ((Difficulty >= DL_2_DOOM && Difficulty < DL_8_INFERNO) && monster.MonsterLevel / 8 + Difficulty * 8 + 45 < baseItem.itemLevel) {
					continue; // skip such drops
				}
				else if (Difficulty == DL_1_PURGATORY && baseItem.itemLevel > 63) {
					continue; // skip such drops
				}
				
				if (GameMode == GM_CLASSIC) { goto SKIP_CHECKS; }

				if( ((player.CharLevel / 2) + player.BaseStrength + 25) < baseItem.RequiredStrengthMax // clvl adjustments will be required here when looting is updated
				 || ((player.CharLevel / 2) + player.BaseMagic + 25) < baseItem.RequiredMagicMax
				 || ((player.CharLevel / 2) + player.BaseDexterity + 25) < baseItem.RequiredDexterityMax
				 || ((player.CharLevel / 2) + player.BaseVitality + 25) < baseItem.RequiredVitalityMax ){
					continue;
				}

				if( !CheckBaseItemClassReq( CurrentPlayerIndex, baseItemIndex ) )
				{
					continue;
				}
			}
			//-------------------------------------------------basic monster drop limits (from above and from below)-------------------------------
			// cut from above
			int maxReqParam = 20 + 110 * Difficulty + 5 * Dungeon->level;
			if (maxReqParam < baseItem.RequiredStrengthMax
				|| maxReqParam < baseItem.RequiredMagicMax
				|| maxReqParam < baseItem.RequiredDexterityMax
				|| maxReqParam < baseItem.RequiredVitalityMax) {
				continue;
			}

			int itemCode = baseItem.ItemCode;
			if( Difficulty == DL_0_HORROR || Players[CurrentPlayerIndex].gameChanger & BIT(GC_7_PROHIBITION)){
				int magicCode = baseItem.MagicCode;
				if( magicCode == MC_10_ELIXIR_OF_STRENGTH 
				 || magicCode == MC_11_ELIXIR_OF_MAGIC 
				 || magicCode == MC_12_ELIXIR_OF_DEXTERITY 
				 || magicCode == MC_13_ELIXIR_OF_VITALITY 
				 || magicCode == MC_44_SPECTRAL_ELIXIR ){
					continue;// no elixirs on Horror (or never - with Prohibition GC)
				}
			}
			// cut from below
			else if( Difficulty >= DL_1_PURGATORY ){
				if (is(itemCode, IC_6_LIGHT_ARMOR, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR, IC_7_HELM, IC_5_SHIELD, IC_2_AXE, IC_3_BOW, IC_4_MACE, IC_10_STAFF, IC_20_CLAW, IC_21_KNIFE, IC_22_MALLET,
					IC_23_PISTOL, IC_19_TRAP, IC_18_FLASK, IC_17_BELT, IC_16_BOOTS, IC_15_GLOVE, IC_1_SWORD)) {
					int limiter = Difficulty * 6 + Dungeon->level / 4;
					if (is(GameMode, GM_IRONMAN, GM_NIGHTMARE)) {
						limiter = Difficulty * 8 + Dungeon->level / 3;
					}
						if (baseItem.itemLevel < limiter) {
						continue;
					}
				}
			}
			if (Difficulty > 0 && (!(Players[CurrentPlayerIndex].gameChanger & BIT(GC_7_PROHIBITION))) && baseItem.MagicCode == MC_44_SPECTRAL_ELIXIR) {
				if (RNG(9) >= 1) {
					continue;
				}
			}
			//-----------------------------------------------------------extra conditions----------------------------------------------------------
			if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
				if (itemCode == IC_5_SHIELD) continue;
				if (itemCode == IC_10_STAFF) continue;
				if (itemCode == IC_6_LIGHT_ARMOR && baseItem.RequiredMagicMax > 0) continue;
				if (itemCode == IC_7_HELM && baseItem.RequiredMagicMax > 0) continue;
				if (itemCode == IC_8_MID_ARMOR && baseItem.RequiredMagicMax > 0) continue;
				if (itemCode == IC_9_HEAVY_ARMOR && baseItem.RequiredMagicMax > 0) continue;
			}
			if ( ( HasTrait ( CurrentPlayerIndex, TraitId::TwoTowers ) || HasTrait(CurrentPlayerIndex, TraitId::Axepertise) ) 
				&& ( itemCode == IC_1_SWORD || itemCode == IC_4_MACE ) ) {
				continue;
			}
			if (itemCode == IC_2_AXE && fullClassId == PFC_ROGUE && (!HasTrait(CurrentPlayerIndex, TraitId::Axepertise) ) ) {
				continue;
			}
			if (itemCode == IC_5_SHIELD && HasTrait(CurrentPlayerIndex, TraitId::Fechtmeister, TraitId::Ranger)) {
				continue;
			}
			if (itemCode == IC_3_BOW && baseItem.AnimationDelay > 0 && HasTrait(CurrentPlayerIndex, TraitId::Ranger)) { // xbow for ranger
				continue;
			}
			if (fullClassId == PFC_WARRIOR && itemCode == IC_3_BOW && baseItem.AnimationDelay == 0 && (!(HasTrait(CurrentPlayerIndex, TraitId::Ranger)))) { // bow for warrior-non-ranger
				continue;
			}
			if (HasTrait(CurrentPlayerIndex, TraitId::Ranger) && (is(itemCode, IC_4_MACE, IC_1_SWORD)) ) {
				//if (RNG(100) < 99) {
					continue;
				//}
			}
			if (itemCode == IC_21_KNIFE && HasTrait(CurrentPlayerIndex, TraitId::Black_Witchery)) {
				continue;
			}
			else if (itemCode == IC_21_KNIFE && RNG(100) < 66) { // throwing knives are dropped with 33% chance
				continue;
			}	
			if (itemCode == IC_22_MALLET && (RNG(100) < 66 || HasTrait(CurrentPlayerIndex, TraitId::Paladin))) { // mallets are dropped with 33% chance, and not dropped for Paladin (0% chance)
				continue;
			}
			if (itemCode == IC_23_PISTOL && !HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
				continue;
			}
			if ((HasTrait(CurrentPlayerIndex, TraitId::Pistoleer))
				&& (itemCode == IC_1_SWORD || itemCode == IC_4_MACE || itemCode == IC_3_BOW)) {
				continue;
			}
			if (itemCode == IC_3_BOW) {
				if (fullClassId == PFC_SHINOBI || fullClassId == PFC_TEMPLAR) {// shinobi & templar have 10% to get xbows, because they are better off with their class weapons (shuriken/mallet)
					if (RNG(100) < 90) {
						continue;
					}
				}
			}
			if ((itemCode == IC_6_LIGHT_ARMOR || itemCode == IC_8_MID_ARMOR || itemCode == IC_9_HEAVY_ARMOR) && HasTrait(CurrentPlayerIndex, TraitId::OldFashioned)) {
				continue;
			}
			if ((itemCode == IC_4_MACE || itemCode == IC_1_SWORD)
				&& baseItem.EquippedLocation == EL_1_ONE_HANDED
				&& fullClassId == PFC_GUARDIAN) {
				continue;
			}
			if (itemCode == IC_15_GLOVE) {// gloves
				if ((is(fullClassId, PFC_MAGE, PFC_ELEMENTALIST, PFC_WARLOCK, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER))
					&& (baseItem.RequiredStrengthMax > 0 || baseItem.RequiredDexterityMax > 0) && (!(HasTrait(CurrentPlayerIndex, TraitId::Mamluk))) ) {
					continue;
				}
				// warriors and savages don't use gloves that require more DEX than STR
				if ((is(fullClassId, PFC_WARRIOR, PFC_INQUISITOR, PFC_GUARDIAN, PFC_TEMPLAR, PFC_SAVAGE, PFC_BERSERKER,
					PFC_EXECUTIONER, PFC_THRAEX, PFC_MURMILLO, PFC_DIMACHAERUS, PFC_SECUTOR, PFC_DRUID))
					&& (baseItem.RequiredDexterityMax > baseItem.RequiredStrengthMax)) {
					continue;
				}
				// removal of STR gloves for girls
				if ((is(fullClassId, PFC_ARCHER, PFC_SCOUT, PFC_SHARPSHOOTER, PFC_TRAPPER, PFC_ROGUE, PFC_ASSASSIN, PFC_IRON_MAIDEN, PFC_BOMBARDIER))
					&& (is(baseItem.itemLevel, 12, 16, 24, 32, 38, 47, 57, 62))) {
					continue;
				}
				if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk) && baseItem.RequiredMagicMax > 0) {
					continue;
				}
				if (fullClassId == PFC_IRON_MAIDEN) {
					if (baseItem.MaxThornsHigh < 1 && RNG(100) < 50) {
						continue;
					}
				}
			}
			if (itemCode == IC_16_BOOTS) {
				if ((is(fullClassId, PFC_ARCHER, PFC_SCOUT, PFC_SHARPSHOOTER, PFC_TRAPPER, PFC_ROGUE, PFC_ASSASSIN, PFC_IRON_MAIDEN,
					PFC_BOMBARDIER, PFC_MAGE, PFC_ELEMENTALIST, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER, PFC_WARLOCK))
					&& (is(baseItem.itemLevel, 5, 11, 19, 27, 36, 57, 79, 94, 118, 147)) && (!(HasTrait(CurrentPlayerIndex, TraitId::Mamluk)))) { // boot base is checked by qlvl, and qlvls now can't change
					continue;
				}
				if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk) && baseItem.RequiredMagicMax > 0) {
					continue;
				}
				if (fullClassId == PFC_IRON_MAIDEN) {
					if (baseItem.MaxThornsHigh < 1 && RNG(100) < 50) {
						continue;
					}
				}
			}
			if (itemCode == IC_17_BELT) {// belt
				if ((is(fullClassId, PFC_WARRIOR, PFC_INQUISITOR, PFC_GUARDIAN, PFC_TEMPLAR, PFC_SAVAGE, PFC_BERSERKER,
					PFC_EXECUTIONER, PFC_THRAEX, PFC_MURMILLO, PFC_DIMACHAERUS, PFC_SECUTOR, PFC_DRUID))
					&& (is(baseItem.itemLevel, 7, 19, 31, 41, 49, 56, 76, 96, 116, 136, 156))) {
					continue;
				}
				if ((is(fullClassId, PFC_ARCHER, PFC_SCOUT, PFC_SHARPSHOOTER, PFC_TRAPPER, PFC_ROGUE, PFC_ASSASSIN, PFC_IRON_MAIDEN, PFC_BOMBARDIER))
					// newly added expansion belts are available to Warrior and Savage types, so I am not excluding them here
					&& (is(baseItem.itemLevel, 13, 25, 37, 46, 53 /* 57 & 61 are segregated already*/))) {// belt base is checked by qlvl, and qlvls now can't change
					continue;
				}
			}
			if (itemCode == IC_7_HELM) {
				if ((is(fullClassId, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER))
					&& (is(baseItem.itemLevel, 9, 15, 19, 21, 27, 34, 44, 55))) {
					continue;
				}
				if (fullClassId == PFC_IRON_MAIDEN
					&& baseItem.itemLevel > 2 /* rogue Steel Cap is level 2 and it's allowed */
					&& baseItem.MaxThornsHigh == 0) {
					continue;
				}
			}
			
			// Getting rid of Mamluk's battle items for Elementalist
			if (fullClassId == PFC_ELEMENTALIST && !HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
				if (is(itemCode, IC_6_LIGHT_ARMOR, IC_7_HELM, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR, IC_1_SWORD, IC_3_BOW, IC_16_BOOTS) && baseItem.Specialization == IS_BATTLE) {
					continue;
				}
			}

			if ((is(itemCode, IC_6_LIGHT_ARMOR, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR))
				&& fullClassId == PFC_IRON_MAIDEN
				&& baseItem.MaxThornsHigh == 0) {
				continue;
			}
			if (itemCode == IC_3_BOW && (fullClassId == PFC_TRAPPER || fullClassId == PFC_DRUID || HasTrait(CurrentPlayerIndex, TraitId::Paladin))) { // bows not dropped for trapper and paladin
				continue;
			}
			if (itemCode == IC_12_RING) {
				if (baseItemIndex == 1149 && HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
					continue;
				}
				if (baseItem.itemLevel == 10 && Difficulty != DL_0_HORROR) {
					continue; // ring level 10 is not dropped by monsters outside of horror difficulty (it will have a low quality unique line on it)
				}
				if (baseItem.itemLevel == 40 && Difficulty != DL_2_DOOM) {
					continue; // ring level 40 is dropped by monsters only on doom
				}
				if (is(baseItem.itemLevel, 74, 94, 114) 
					&& baseItem.Specialization == IS_BATTLE 
					&& Players[CurrentPlayerIndex].ClassID == PC_2_MAGE 
					&& !HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
					continue;
				}
				if (is(baseItem.itemLevel, 74, 94, 114)
					&& baseItem.Specialization == IS_MAGIC){
					if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
						continue;
					}
					if (HasTrait(CurrentPlayerIndex, TraitId::Paladin, TraitId::Black_Witchery ) || is(fullClassId, PFC_BOMBARDIER, PFC_ROGUE, PFC_MAGE, PFC_WARLOCK, PFC_ELEMENTALIST)){
					//do nothing, allow loot
					}
					else {
						continue;
					}
				}
				if (baseItem.itemLevel ==  74 && (Difficulty < DL_3_CRUCIBLE || Difficulty > DL_4_TORMENT)) { continue; }
				if (baseItem.itemLevel ==  94 && (Difficulty < DL_5_AGONY || Difficulty > DL_6_TERROR)) {	continue; }
				if (baseItem.itemLevel == 114 && Difficulty < DL_7_DESTRUCTION) { continue; }
			}
			if (itemCode == IC_13_AMULET) {
				if (baseItemIndex == 160 && HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
					continue;
				}
				if (baseItem.Specialization == IS_BATTLE
					&& is(fullClassId, PFC_MAGE, PFC_ELEMENTALIST, PFC_WARLOCK, PFC_NECROMANCER, PFC_DEMONOLOGIST, PFC_BEASTMASTER, PFC_TRAPPER, PFC_BOMBARDIER)
					&& (!(HasTrait(CurrentPlayerIndex, TraitId::Mamluk)))) {
					continue; // battle amulets don't drop for Mage classes and some other
				}
				if (is(baseItem.itemLevel, 74, 94, 114)
					&& baseItem.Specialization == IS_BATTLE
					&& Players[CurrentPlayerIndex].ClassID == PC_2_MAGE
					&& !HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
					continue;
				}
				if (is(baseItem.itemLevel, 74, 94, 114)
					&& baseItem.Specialization == IS_MAGIC) {
					if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
						continue;
					}
					if (HasTrait(CurrentPlayerIndex, TraitId::Paladin, TraitId::Black_Witchery) || is(fullClassId, PFC_BOMBARDIER, PFC_ROGUE, PFC_MAGE, PFC_WARLOCK, PFC_ELEMENTALIST)) {
						//do nothing, allow loot
					}
					else {
						continue;
					}
				}
				if (baseItem.itemLevel ==  74 && (Difficulty < DL_3_CRUCIBLE || Difficulty > DL_4_TORMENT)) { continue; }
				if (baseItem.itemLevel ==  94 && (Difficulty < DL_5_AGONY || Difficulty > DL_6_TERROR)) { continue; }
				if (baseItem.itemLevel == 114 && Difficulty < DL_7_DESTRUCTION) { continue; }
			}
			if (itemCode == IC_1_SWORD && baseItem.itemLevel == 51 && baseItem.RequiredMagicMax > 240 && Difficulty < DL_2_DOOM) {
				continue; // immolation dagger drops only on Doom
			}
			if ((itemCode == IC_1_SWORD || itemCode == IC_4_MACE) && fullClassId == PFC_INQUISITOR /* && baseItem.Name2 != "Scepter"*/) { 
				if (strcmp(baseItem.Name2, "Scepter") != 0) {// if magic name of base item is not scepter... 
					if (RNG(100) < 65) { // 65% chance to skip dropping non-scepter type 1-handed melee weapons from bosses
						continue;
					}
				}
			}
			SKIP_CHECKS:
			// проверял выпадание эликсиров
			//if( baseItem.MagicCode < MC_10_ELIXIR_OF_STRENGTH || baseItem.MagicCode > MC_13_ELIXIR_OF_VITALITY ) continue;
			if (goodItemsCount < GOOD_ITEM_COUNT) {
				if (MaxCountOfPlayersInGame == 1 && (spell == PS_32_BONE_SPIRIT_REAL_RESSURECT || spell == PS_34_HEAL_OTHER)) {
					// Forbidden in mp
				}
				else if (isTownPortalForbidden() && (spell == PS_7_TOWN_PORTAL)) {
					// Forbidden in game modes
				}
				else if (is(fullClassId, PFC_ARCHER, PFC_SCOUT, PFC_SHARPSHOOTER, PFC_TRAPPER) && (spell == PS_9_INFRAVISION)) {
					// Seeing relicts Forbidden if it's available as class skill
				}
				else if (is(fullClassId, PFC_SAVAGE, PFC_BERSERKER, PFC_EXECUTIONER, PFC_THRAEX, PFC_SECUTOR, PFC_MURMILLO, PFC_DIMACHAERUS) && (spell == PS_16_REFLECT)) {
					// Reflect relicts Forbidden if it's available as class skill
				}
				else if ( (is(fullClassId, PFC_ASSASSIN, PFC_ROGUE, PFC_BOMBARDIER) || HasTrait(CurrentPlayerIndex, TraitId::Barbarian) ) && (spell == PS_5_IDENTIFY) ) {
					// Identify relicts Forbidden if it's available as class skill
				}
				else if (is(fullClassId, PFC_MONK, PFC_KENSEI, PFC_SHUGOKI, PFC_SHINOBI) && (spell == PS_33_TELEKINES)) {
					// Telekinesis relicts Forbidden if it's available as class skill
				}
				else if ((is(fullClassId, PFC_WARRIOR, PFC_INQUISITOR, PFC_GUARDIAN, PFC_TEMPLAR, PFC_IRON_MAIDEN) && !HasTrait(CurrentPlayerIndex, TraitId::Paladin))
					&& (spell == PS_26_ITEM_REPAIR)) {
					// Item Repair relicts Forbidden if it's available as class skill
				}
				else if (is(fullClassId, PFC_MAGE, PFC_ELEMENTALIST, PFC_WARLOCK, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER) && (spell == PS_27_STAFF_RECHARGE)) {
					// Item Recharge relicts Forbidden if it's available as class skill
				}
				else if (fullClassId == PFC_WARLOCK && (spell == PS_31_HOLY_BOLT || spell == PS_39_HOLY_NOVA)) {
					// Warlock doesn't have access to Holy Bolt and Holy Nova books drops
				}
				else if ((Dungeon >= DUN_60_HAUNTED_LABYRINTH && Dungeon <= DUN_108_ARCANE_PRISON) && (spell == PS_42_WARP)) {
					// Monsters don't drop Warp relicts in AR dungeons
				}
				else {
					goodItems[goodItemsCount++] = /*(BASE_ITEM)*/ baseItemIndex;
				}
			}
		}
	}
	int result = goodItemsCount ? goodItems[ RNG(goodItemsCount) ] : 0;
	//if( result >= MC_10_ELIXIR_OF_STRENGTH && result <= MC_13_ELIXIR_OF_VITALITY ) __debugbreak();
	return result + 1;
}

//----- (00426D8B) --------------------------------------------------------
int __fastcall GetRandomBaseForHiQualityObjectAndBossDrop( int monsterIndex )
{
	vla( BASE_ITEM, goodItems, GOOD_ITEM_COUNT );
	int goodItemsCount = 0;

	if( monsterIndex != -1 ){
		Monster& monster = Monsters[monsterIndex];
		int specialDrop = monster.BasePtr->ItemDropSpecials;
		if( specialDrop & 0x8000 && MaxCountOfPlayersInGame == 1 ){
			return -1 - (specialDrop & 0xFFF);
		}
	}

	int qlvl = ItemLevelByDungeonLevel() * 2;

	PLAYER_FULL_CLASS fullClassId = Players[CurrentPlayerIndex].fullClassId;
	int mode = (GameMode == GM_CLASSIC) + 1;
	for( uint baseItemIndex = 0; goodItemsCount < GOOD_ITEM_COUNT && baseItemIndex < count_BaseItems; ++baseItemIndex ){
		BaseItem& baseItem = BaseItems[baseItemIndex];
		if( !(baseItem.mode + 1 & mode) ) continue;

		if( !baseItem.ChanceToDrop ){// ChanceToDrop
			continue;
		}
		if( monsterIndex == -1 ){ // monsterIndex == -1 is drop frop barrels and chests
			if( qlvl < baseItem.itemLevel ){
				continue;
			}
			// here could be check for baseitem not to drop frop barrels and chests
			//if( is(baseItemIndex, BI_1_SHORT_KNIFE) ){
			//	continue;
			//}			
		}
		else{
			Monster& monster = Monsters[monsterIndex];
			// base items with level over 63 are not allowed on Purgatory, to maintain same item progression as in TH2
			if (Difficulty == DL_1_PURGATORY && baseItem.itemLevel > 63) {
				continue;
			}
			if (Difficulty >= DL_2_DOOM && Difficulty < DL_8_INFERNO) {
				if (monster.MonsterLevel / 8 + Difficulty * 8 + 45 < baseItem.itemLevel) {
					continue;
				}
			}
			// on diffs Horror, Purgatory and Inferno item availability works according to boss level
			// so, 2-66 Horror. 17-81 Purgatory and 121-186 Inferno (on Inferno it is required so that Uber D can drop his base items of level up to 312)
			else if (monster.MonsterLevel < baseItem.itemLevel) {
				continue;
			}
		}

		int itemCode = baseItem.ItemCode;// itemCode
		if( itemCode == IC_0_OTHER || itemCode == IC_11_GOLD || itemCode == IC_14_EAR ){
			continue;
		}

		if (GameMode == GM_CLASSIC) { goto SKIP_CHECKS_HQ; }
		// if( MaxCountOfPlayersInGame == 1) // MP smart drop
		{
			Player& player = Players[CurrentPlayerIndex];//GetCurrentPlayerOffset();
			if(	   (9 * player.CharLevel / 15 + player.BaseStrength + 10) < baseItem.RequiredStrengthMax
				|| (9 * player.CharLevel / 15 + player.BaseMagic	+ 10) < baseItem.RequiredMagicMax
				|| (9 * player.CharLevel / 15 + player.BaseDexterity + 10) < baseItem.RequiredDexterityMax
				|| (9 * player.CharLevel / 15 + player.BaseVitality + 10) < baseItem.RequiredVitalityMax ){
					continue;
			}

			if( !CheckBaseItemClassReq( CurrentPlayerIndex, baseItemIndex ) )
			{
				continue;
			}
		}
		//------------------------------------------------- boss drop limits (from above and from below)---------------------------------
		// cut from above
		int maxReqParam = 20 + 110 * Difficulty + 5 * Dungeon->level;
		if (maxReqParam < baseItem.RequiredStrengthMax
			|| maxReqParam < baseItem.RequiredMagicMax
			|| maxReqParam < baseItem.RequiredDexterityMax
			|| maxReqParam < baseItem.RequiredVitalityMax) {
			continue;
		}
		// cut from below
		if ((is(itemCode, IC_6_LIGHT_ARMOR, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR, IC_7_HELM, IC_5_SHIELD, IC_2_AXE, IC_3_BOW, IC_4_MACE, IC_10_STAFF, IC_20_CLAW, IC_21_KNIFE, IC_22_MALLET,
			IC_23_PISTOL, IC_19_TRAP, IC_18_FLASK, IC_17_BELT, IC_16_BOOTS, IC_15_GLOVE, IC_1_SWORD)) && Difficulty >= DL_1_PURGATORY) {
			int limiter = Difficulty * 8 + Dungeon->level / 3;
			if (is(GameMode, GM_IRONMAN, GM_NIGHTMARE)) {
				limiter = Difficulty * 10 + 5 * Dungeon->level / 12;
			}
			if (baseItem.itemLevel < limiter) {			
				continue;
			}
		}
		//-----------------------------------------------------------extra conditions----------------------------------------------------------
		if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
			if (itemCode == IC_5_SHIELD) continue;
			if (itemCode == IC_10_STAFF) continue;
			if (itemCode == IC_6_LIGHT_ARMOR && baseItem.RequiredMagicMax > 0) continue;
			if (itemCode == IC_7_HELM && baseItem.RequiredMagicMax > 0) continue;
			if (itemCode == IC_8_MID_ARMOR && baseItem.RequiredMagicMax > 0) continue;
			if (itemCode == IC_9_HEAVY_ARMOR && baseItem.RequiredMagicMax > 0) continue;
		}
		if (baseItem.GraphicValue == 2558 + 168 /*rags picture*/ && Dungeon->level > 5) { // rags are not dropped by bosses after dlvl 5
			continue;
		}
		if ((HasTrait(CurrentPlayerIndex, TraitId::TwoTowers) || HasTrait(CurrentPlayerIndex, TraitId::Axepertise))
			&& (itemCode == IC_1_SWORD || itemCode == IC_4_MACE)) {
			continue;
		}
		if (itemCode == IC_2_AXE && fullClassId == PFC_ROGUE && (!HasTrait(CurrentPlayerIndex, TraitId::Axepertise))) {
			continue;
		}

		if (itemCode == IC_5_SHIELD && HasTrait(CurrentPlayerIndex, TraitId::Fechtmeister, TraitId::Ranger)) {
			continue;
		}

		if (itemCode == IC_3_BOW && baseItem.AnimationDelay > 0 && HasTrait(CurrentPlayerIndex, TraitId::Ranger)) {
			continue;
		}

		if (fullClassId == PFC_WARRIOR && itemCode == IC_3_BOW && baseItem.AnimationDelay == 0 && (!(HasTrait(CurrentPlayerIndex, TraitId::Ranger)))) {
			continue;
		}
		if (HasTrait(CurrentPlayerIndex, TraitId::Ranger) && (is(itemCode, IC_4_MACE, IC_1_SWORD))) {
			//if (RNG(100) < 99) {
				continue;
			//}
		}

		if (itemCode == IC_21_KNIFE && HasTrait(CurrentPlayerIndex, TraitId::Black_Witchery)) {
			continue;
		}
		else if (itemCode == IC_21_KNIFE && RNG(100) < 66) { // throwing knives are dropped with 33% chance
			continue;
		}
		
		if (itemCode == IC_22_MALLET && (RNG(100) < 66 || HasTrait(CurrentPlayerIndex, TraitId::Paladin))) { // mallets are dropped with 33% chance and not dropped for paladin
			continue;
		}
		if (itemCode == IC_23_PISTOL && !HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
			continue;
		}
		if ((HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) 
			&& (itemCode == IC_1_SWORD || itemCode == IC_4_MACE || itemCode == IC_3_BOW)) { 
			continue;
		}

		// ------------------------------- (x)bow conditions -----------------------
		if (itemCode == IC_3_BOW) {
			if (fullClassId == PFC_SHINOBI || fullClassId == PFC_TEMPLAR) {// shinobi & templar have 10% to get xbows, because they are better off with their class weapons (shuriken/mallet)
				if (RNG(100) < 90) {
					continue;
				}
			}
			else if (is(fullClassId, PFC_SHUGOKI, PFC_MONK, PFC_KENSEI)) {
				int bow_chance = 50;
				if (is(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SURVIVAL, GM_SPEEDRUN)) {
					bow_chance = 70;
				}
				if (RNG(100) < bow_chance) {
					continue;
				}
			}
		}
		//------------------------------------------------------------------------

		if ((itemCode == IC_6_LIGHT_ARMOR || itemCode == IC_8_MID_ARMOR || itemCode == IC_9_HEAVY_ARMOR) && HasTrait(CurrentPlayerIndex, TraitId::OldFashioned)) {
			continue;
		}
		if( ( itemCode == IC_4_MACE || itemCode == IC_1_SWORD )
			&& baseItem.EquippedLocation == EL_1_ONE_HANDED 
			&& fullClassId == PFC_GUARDIAN ){
			continue;
		}
		if ( itemCode == IC_15_GLOVE ){
			if ((is(fullClassId, PFC_MAGE, PFC_ELEMENTALIST, PFC_WARLOCK, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER))
				&& (baseItem.RequiredStrengthMax > 0 || baseItem.RequiredDexterityMax > 0) && (!(HasTrait(CurrentPlayerIndex, TraitId::Mamluk)))) {
				continue;
			}
			if( ( is( fullClassId, PFC_WARRIOR, PFC_INQUISITOR, PFC_GUARDIAN, PFC_TEMPLAR, PFC_SAVAGE, PFC_BERSERKER, PFC_EXECUTIONER, PFC_THRAEX, PFC_MURMILLO, PFC_DIMACHAERUS, PFC_SECUTOR, PFC_DRUID) )
				&& ( baseItem.RequiredDexterityMax > baseItem.RequiredStrengthMax ) ){
				continue;
			}
			if ((is(fullClassId, PFC_ARCHER, PFC_SCOUT, PFC_SHARPSHOOTER, PFC_TRAPPER, PFC_ROGUE, PFC_ASSASSIN, PFC_IRON_MAIDEN, PFC_BOMBARDIER))
				&& (is(baseItem.itemLevel, 12, 16, 24, 32, 38, 47, 57, 62))) {
				continue;
			}
			if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk) && baseItem.RequiredMagicMax > 0) {
				continue;
			}
			if (fullClassId == PFC_IRON_MAIDEN) {
				if (baseItem.MaxThornsHigh == 0 && RNG(100) < 50) {
					continue; 
				}
			}
		}
		if (itemCode == IC_16_BOOTS) {
			if ((is(fullClassId, PFC_ARCHER, PFC_SCOUT, PFC_SHARPSHOOTER, PFC_TRAPPER, PFC_ROGUE, PFC_ASSASSIN, PFC_IRON_MAIDEN,
				PFC_BOMBARDIER, PFC_MAGE, PFC_ELEMENTALIST, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER, PFC_WARLOCK))
				&& (is(baseItem.itemLevel, 5, 11, 19, 27, 36, 57, 79, 94, 118, 147)) && (!(HasTrait(CurrentPlayerIndex, TraitId::Mamluk)))) { // boot base is checked by qlvl, and qlvls now can't change
				continue;
			}
			if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk) && baseItem.RequiredMagicMax > 0) {
				continue;
			}
			if (fullClassId == PFC_IRON_MAIDEN) {
				if (baseItem.MaxThornsHigh == 0 && RNG(100) < 50) {
					continue;
				}
			}
		}
		if (itemCode == IC_17_BELT ){
			if ((is(fullClassId, PFC_WARRIOR, PFC_INQUISITOR, PFC_GUARDIAN, PFC_TEMPLAR, PFC_SAVAGE, PFC_BERSERKER, PFC_EXECUTIONER, PFC_THRAEX, PFC_MURMILLO, PFC_DIMACHAERUS, PFC_SECUTOR, PFC_DRUID))
				&& (is(baseItem.itemLevel, 7, 19, 31, 41, 49, 56, 76, 96, 116, 136, 156))) {
				continue;
			}
			if ((is(fullClassId, PFC_ARCHER, PFC_SCOUT, PFC_SHARPSHOOTER, PFC_TRAPPER, PFC_ROGUE, PFC_ASSASSIN, PFC_IRON_MAIDEN, PFC_BOMBARDIER))
				&& (is(baseItem.itemLevel, 13, 25, 37, 46, 53 /* 57 & 61 are segregated already*/))) {// belt base is checked by qlvl, and qlvls now can't change
				continue;
			}
		}
		if (itemCode == IC_7_HELM ){
			if ((is(fullClassId, PFC_DEMONOLOGIST, PFC_NECROMANCER, PFC_BEASTMASTER))
				&& (is(baseItem.itemLevel, 9, 15, 19, 21, 27, 34, 44, 55))) {
				continue;
			}			
			else if (fullClassId == PFC_IRON_MAIDEN	
				&& baseItem.itemLevel > 2 /* rogue Steel Cap is level 2 and it's allowed */ 
				&& baseItem.MaxThornsHigh == 0) {
				continue;
			}
		}

		// Getting rid of Mamluk's battle items for Elementalist
		if (fullClassId == PFC_ELEMENTALIST && !HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
			if (is(itemCode, IC_6_LIGHT_ARMOR, IC_7_HELM, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR, IC_1_SWORD, IC_3_BOW, IC_16_BOOTS) && baseItem.Specialization == IS_BATTLE) {
				continue;
			}
		}

		if ( ( is( itemCode, IC_6_LIGHT_ARMOR, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR ) ) 
			&& fullClassId == PFC_IRON_MAIDEN 
			&& baseItem.MaxThornsHigh == 0 ) {
			continue;
		}
		if (itemCode == IC_3_BOW && (fullClassId == PFC_TRAPPER || fullClassId == PFC_DRUID || HasTrait(CurrentPlayerIndex, TraitId::Paladin)) ) {
			continue;
		}
		// -------- jewelry drop restrictions ----------- >>>>
		if (itemCode == IC_12_RING){
			if (baseItemIndex == 1149 && HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
				continue;
			}
			if (is(baseItem.itemLevel, 1, 2) && Difficulty > DL_1_PURGATORY) {
				continue; // rings not dropped by bosses after purgatory
			}
			if (is(baseItem.itemLevel, 4, 10, 22) && Difficulty > DL_2_DOOM) {
				continue;
			}
			if (baseItem.itemLevel == 40 && (Difficulty < DL_2_DOOM || Difficulty > DL_3_CRUCIBLE)) {
				continue; // ring level 40 is dropped by bosses only on doom & crucible
			}
			if (is(baseItem.itemLevel, 74, 94, 114)
				&& baseItem.Specialization == IS_BATTLE
				&& Players[CurrentPlayerIndex].ClassID == PC_2_MAGE
				&& !HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
				continue;
			}
			if (is(baseItem.itemLevel, 74, 94, 114)
				&& baseItem.Specialization == IS_MAGIC) {
				if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
					continue;
				}
				if (HasTrait(CurrentPlayerIndex, TraitId::Paladin, TraitId::Black_Witchery) || is(fullClassId, PFC_BOMBARDIER, PFC_ROGUE, PFC_MAGE, PFC_WARLOCK, PFC_ELEMENTALIST)) {
					//do nothing, allow loot
				}
				else {
					continue;
				}
			}
			if (baseItem.itemLevel ==  74 && (Difficulty < DL_3_CRUCIBLE || Difficulty > DL_4_TORMENT)) { continue; }
			if (baseItem.itemLevel ==  94 && (Difficulty < DL_5_AGONY || Difficulty > DL_6_TERROR)) { continue; }
			if (baseItem.itemLevel == 114 && Difficulty < DL_7_DESTRUCTION) { continue; }
		}
		if (itemCode == IC_13_AMULET) {
			if (baseItemIndex == 160 && HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
				continue;
			}
			if (baseItem.Specialization == IS_BATTLE
				&& is(fullClassId, PFC_MAGE, PFC_ELEMENTALIST, PFC_WARLOCK, PFC_NECROMANCER, PFC_DEMONOLOGIST, PFC_BEASTMASTER, PFC_TRAPPER, PFC_BOMBARDIER)
				&& (!(HasTrait(CurrentPlayerIndex, TraitId::Mamluk)))) {
				continue; // battle amulets  don't drop for Mage classes and some other
			}
			if (baseItem.itemLevel == 34 && Difficulty == DL_0_HORROR) {
				continue; // amulet that has qlvl 63 unique of Norrec's Perfectionism should not drop until late Purgatory (so, not dropped on Horror)
			}
			if (is(baseItem.itemLevel, 3, 54) && Difficulty > DL_2_DOOM) {
				continue;
			}
			if (baseItem.itemLevel == 34 && Difficulty > DL_3_CRUCIBLE) {
				continue;
			}
			if (baseItem.itemLevel == 2 && Difficulty > DL_1_PURGATORY) {
				continue;
			}
			if (is(baseItem.itemLevel, 74, 94, 114)
				&& baseItem.Specialization == IS_BATTLE
				&& Players[CurrentPlayerIndex].ClassID == PC_2_MAGE
				&& !HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
				continue;
			}
			if (is(baseItem.itemLevel, 74, 94, 114)
				&& baseItem.Specialization == IS_MAGIC) {
				if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
					continue;
				}
				if (HasTrait(CurrentPlayerIndex, TraitId::Paladin, TraitId::Black_Witchery) || is(fullClassId, PFC_BOMBARDIER, PFC_ROGUE, PFC_MAGE, PFC_WARLOCK, PFC_ELEMENTALIST)) {
					//do nothing, allow loot
				}
				else {
					continue;
				}
			}
			if (baseItem.itemLevel ==  74 && (Difficulty < DL_3_CRUCIBLE || Difficulty > DL_4_TORMENT)) { continue; }
			if (baseItem.itemLevel ==  94 && (Difficulty < DL_5_AGONY || Difficulty > DL_6_TERROR)) { continue; }
			if (baseItem.itemLevel == 114 && Difficulty < DL_7_DESTRUCTION) { continue; }
		}
		// ---------------------------------------------- <<<<
		if (itemCode == IC_1_SWORD && baseItem.itemLevel == 51 && baseItem.RequiredMagicMax > 240 && Difficulty < DL_2_DOOM) {
			continue; // immolation dagger drops only on Doom
		}
		if ( is(itemCode, IC_1_SWORD, IC_4_MACE) && fullClassId == PFC_INQUISITOR) {
			if (strcmpi(baseItem.Name2, "Scepter") != 0) {
				if (RNG(100) < 80) { // 80% chance to skip dropping non-scepter type 1-handed melee weapons from bosses
					continue;
				}
			}			
		}

	SKIP_CHECKS_HQ:
		int spell = baseItem.SpellNumber;
		if( goodItemsCount < GOOD_ITEM_COUNT ){
            if( MaxCountOfPlayersInGame == 1 && (spell == PS_32_BONE_SPIRIT_REAL_RESSURECT || spell == PS_34_HEAL_OTHER) ){
                // Forbidden in mp
            }
			else if( isTownPortalForbidden() && ( spell == PS_7_TOWN_PORTAL ) ){
                // Forbidden in game modes
            }
			else if ( HasTrait(CurrentPlayerIndex, TraitId::Paladin) && spell == PS_36_BONE_SPIRIT ) {
				// Paladin doesn't have Bone Spirit book drops
			}
			else if (fullClassId == PFC_WARLOCK && (spell == PS_31_HOLY_BOLT || spell == PS_39_HOLY_NOVA)) {
				// Warlock doesn't have access to Holy Bolt and Holy Nova books drops
			}
			else{
                goodItems[goodItemsCount++] = static_cast<BASE_ITEM>( baseItemIndex );
            }
		}
	}

	return goodItemsCount ? goodItems[RNG(goodItemsCount)] : 0;
}

//----- (00426EA0) --------------------------------------------------------
int GetRandomBaseForLowQualityObjectDrop() // drop from objects
{
	vla( int /*BASE_ITEM*/, goodItems, GOOD_ITEM_COUNT );
	int goodItemsCount = 0;

	if( RNG(100) > 25 ){
		return BI_0_GOLD;
	}
	int qlvl = ItemLevelByDungeonLevel() * 2;

	const Player& player = Players[CurrentPlayerIndex];

	int mode = (GameMode == GM_CLASSIC) + 1;
	for( uint baseItemIndex = 0; baseItemIndex < count_BaseItems; ++baseItemIndex ){
		BaseItem& baseItem = BaseItems[baseItemIndex];
		if( !(baseItem.mode + 1 & mode) ) continue;
		if( baseItem.ChanceToDrop
		 && qlvl >= baseItem.itemLevel
		 && IsOilQuality( baseItemIndex )
		 && goodItemsCount < GOOD_ITEM_COUNT ){

			if (GameMode == GM_CLASSIC) goto SKIP_CHECKS;

			int maxReqParam = 20 + 110 * Difficulty + 5 * Dungeon->level;		
			if (maxReqParam < baseItem.RequiredStrengthMax
				|| maxReqParam < baseItem.RequiredMagicMax
				|| maxReqParam < baseItem.RequiredDexterityMax
				|| maxReqParam < baseItem.RequiredVitalityMax) {
				continue;
			}
			// base item level cut from below. doubtful. needs check
			if ((is(baseItem.ItemCode, IC_6_LIGHT_ARMOR, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR, IC_7_HELM, IC_5_SHIELD, IC_2_AXE, IC_3_BOW, IC_4_MACE, IC_10_STAFF, IC_20_CLAW, IC_21_KNIFE, IC_22_MALLET,
				IC_23_PISTOL, IC_19_TRAP, IC_18_FLASK, IC_17_BELT, IC_16_BOOTS, IC_15_GLOVE, IC_1_SWORD)) && Difficulty >= DL_1_PURGATORY) {
				int limiter = Difficulty * 6 + Dungeon->level / 4;
				if (is(GameMode, GM_IRONMAN, GM_NIGHTMARE)) {
					limiter = Difficulty * 8 + Dungeon->level / 3;
				}
				if (baseItem.itemLevel < limiter) {
					continue;
				}
			}
			if (	(player.CharLevel / 2 + player.BaseStrength + 25) < baseItem.RequiredStrengthMax
				||	(player.CharLevel / 2 + player.BaseMagic + 25) < baseItem.RequiredMagicMax
				||	(player.CharLevel / 2 + player.BaseDexterity + 25) < baseItem.RequiredDexterityMax
				||	(player.CharLevel / 2 + player.BaseVitality + 25) < baseItem.RequiredVitalityMax) {
				continue;
			}

			if ( ( baseItem.ItemCode == IC_4_MACE || baseItem.ItemCode == IC_1_SWORD )
				&& baseItem.EquippedLocation == EL_1_ONE_HANDED
				&& ( player.fullClassId == PFC_GUARDIAN || ( player.fullClassId == PFC_ROGUE && HasTrait ( CurrentPlayerIndex, TraitId::Axepertise ) ) ) ) {
				continue; // guardian and axepert rogue don't get 1H sharps and blunts
			}

			if (player.fullClassId == PFC_GUARDIAN && (!HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) && baseItem.ItemCode == IC_23_PISTOL) {
				continue;
			}

			if (player.fullClassId == PFC_ROGUE) {
				if (!HasTrait(CurrentPlayerIndex, TraitId::Axepertise)) {
					if (baseItem.ItemCode == IC_2_AXE) {
						continue; // rogue without axepertise gets no axe
					}
				}
			}

			if ((player.fullClassId == PFC_TRAPPER || player.fullClassId == PFC_DRUID || HasTrait(CurrentPlayerIndex, TraitId::Paladin)) && baseItem.ItemCode == IC_3_BOW) {
				continue;
			}

			if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
				if (baseItem.ItemCode == IC_10_STAFF) continue;
				if (baseItem.ItemCode == IC_6_LIGHT_ARMOR && baseItem.RequiredMagicMax > 0) continue;
				if (baseItem.ItemCode == IC_7_HELM && baseItem.RequiredMagicMax > 0) continue;
				if (baseItem.ItemCode == IC_8_MID_ARMOR && baseItem.RequiredMagicMax > 0) continue;
				if (baseItem.ItemCode == IC_9_HEAVY_ARMOR && baseItem.RequiredMagicMax > 0) continue;
				if (baseItem.ItemCode == IC_15_GLOVE && baseItem.RequiredMagicMax > 0) continue;
				if (baseItem.ItemCode == IC_16_BOOTS && baseItem.RequiredMagicMax > 0) continue;
			}
			// Getting rid of Mamluk's battle items for Elementalist
			if (player.fullClassId == PFC_ELEMENTALIST && !HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
				if (is(baseItem.ItemCode, IC_6_LIGHT_ARMOR, IC_7_HELM, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR, IC_1_SWORD, IC_3_BOW, IC_16_BOOTS) && baseItem.Specialization == IS_BATTLE) {
					continue;
				}
			}

			if (HasTrait(CurrentPlayerIndex, TraitId::Fechtmeister, TraitId::Ranger, TraitId::Mamluk) && baseItem.ItemCode == IC_5_SHIELD) {
				continue;
			}
			if (HasTrait(CurrentPlayerIndex, TraitId::Paladin) && baseItem.ItemCode == IC_22_MALLET) {
				continue;
			}
			if (HasTrait(CurrentPlayerIndex, TraitId::Pistoleer) && is(baseItem.ItemCode, IC_1_SWORD, IC_3_BOW, IC_4_MACE)) {
				continue;
			}
			if (HasTrait(CurrentPlayerIndex, TraitId::Black_Witchery) && baseItem.ItemCode == IC_21_KNIFE) {
				continue;
			}
			if (HasTrait(CurrentPlayerIndex, TraitId::OldFashioned) && is(baseItem.ItemCode, IC_6_LIGHT_ARMOR, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR)) {
				continue;
			}
			if (HasTrait(CurrentPlayerIndex, TraitId::TwoTowers, TraitId::Ranger) && is(baseItem.ItemCode, IC_1_SWORD, IC_4_MACE)) {
				continue;
			}
			if (baseItem.ItemCode == IC_3_BOW && baseItem.AnimationDelay > 0 && HasTrait(CurrentPlayerIndex, TraitId::Ranger)) { // xbow for Ranger
				continue;
			}
			if (player.fullClassId == PFC_WARRIOR && baseItem.ItemCode == IC_3_BOW && baseItem.AnimationDelay == 0 && (!(HasTrait(CurrentPlayerIndex, TraitId::Ranger)))) { // bows for Warrior
				continue;
			}
			if (player.fullClassId == PFC_IRON_MAIDEN && (is(baseItem.ItemCode, IC_15_GLOVE, IC_16_BOOTS, IC_7_HELM, IC_6_LIGHT_ARMOR, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR))) {
				if (baseItem.MaxThornsHigh == 0 && RNG(100) < 50) {
					continue;
				}
			}
			SKIP_CHECKS:
            int spell = baseItem.SpellNumber;

            if( isTownPortalForbidden() && ( spell == PS_7_TOWN_PORTAL ) ){
                continue;
            }

            if( MaxCountOfPlayersInGame == 1 )
            {
                if( (spell == PS_32_BONE_SPIRIT_REAL_RESSURECT || spell == PS_34_HEAL_OTHER ) )
                {
                    continue;
                }
            }
            {// MP smart drop
                if( !CheckBaseItemClassReq( CurrentPlayerIndex, baseItemIndex ) )
                {
                    continue;
                }
            }

			goodItems[goodItemsCount++] = static_cast<BASE_ITEM>(baseItemIndex);
		}
	}
	return goodItemsCount ? goodItems[RNG(goodItemsCount)] : 0;
}

//----- (00426F43) --------------------------------------------------------
int __fastcall RndTypeItems( int itemCode, int magicCode, int qlvl, int smartDropFlag /*=0*/, bool minQlvlLimit /*=false*/, uchar specialization /*=0*/ )
{
	vla( int /*BASE_ITEM*/, goodItems, GOOD_ITEM_COUNT );
	int goodItemsCount = 0;
	if( !qlvl ) qlvl = 1;
	qlvl *= 2;
	int mode = (GameMode == GM_CLASSIC) + 1;
	for( uint baseItemIndex = 0; baseItemIndex < count_BaseItems; baseItemIndex++ ){
		BaseItem& baseItem = BaseItems[baseItemIndex];
		if( !(baseItem.mode + 1 & mode) ) continue;
        if( baseItem.ChanceToDrop 
         && qlvl >= baseItem.itemLevel && ( ! minQlvlLimit || baseItem.itemLevel >= Difficulty * 6 + Dungeon->level / 4 ) // min qlvl check for rack
         && baseItem.ItemCode == itemCode
         && ( magicCode == -1 || baseItem.MagicCode == magicCode )
		 && ( ! specialization || specialization == baseItem.Specialization )
         && ( /*MaxCountOfPlayersInGame != 1 ||*/ CheckBaseItemClassReq( CurrentPlayerIndex, baseItemIndex ) )
		 && goodItemsCount < GOOD_ITEM_COUNT ){
			if( smartDropFlag & SD_1_TWO_HANDS_ONLY && baseItem.EquippedLocation == EL_1_ONE_HANDED
			 || smartDropFlag & IF_NO_CROSSBOW && !strcmpi(baseItem.Name2, "crossbow")
			 || smartDropFlag & IF_NO_BOW && strcmpi(baseItem.Name2, "crossbow") 
			 || smartDropFlag & IF_CASTER_HARNESS && baseItem.EquippedLocation == EL_3_TORSO && baseItem.RequiredMagicMax > 0
			 || smartDropFlag & IF_NON_THORNS_HARNESS && baseItem.EquippedLocation == EL_3_TORSO && baseItem.MaxThornsHigh == 0
			 || smartDropFlag & IF_MAMLUK_HARNESS && baseItem.EquippedLocation == EL_3_TORSO && baseItem.RequiredMagicMax == 0
				){
				continue;
			}
			goodItems[goodItemsCount++] = static_cast<BASE_ITEM>(baseItemIndex);
        }
	}
	int baseItemIndex = goodItemsCount ? goodItems[RNG(goodItemsCount)] : 0; // if not found base drop gold
	//if( baseItemIndex >= countof( BaseItems ) ) __debugbreak();
	return baseItemIndex;
}

//----- (00426FDD) --------------------------------------------------------
int __fastcall SelectRandomUnique(int itemIndex, int mlvl_quality, int chanceInProcent, int allowDuplicate)
{
	int countOfDropableUniqs = 0;
	int uniqueItemIndex = -1;
	int uniqLine = BaseItems[Items[itemIndex].baseItemIndex].uniqLine;
	vla( int, drop, count_UniqueItems ); // int drop[count_UniqueItems]; // C99 vla still not supported in msvc
	
	// base item data early was writen in next itemOnMap slot
	int uniqIndex = 0;
	int maxQlvl = 0;
	for( size_t i = 0; i < count_UniqueItems; ++i ){
		UniqueItem& uniq = UniqueItems[i];
		if( uniq.uniqLine == uniqLine && mlvl_quality >= uniq.qualityLevel ){
			drop[countOfDropableUniqs++] = i;
			if( uniq.qualityLevel > maxQlvl ){
				maxQlvl = uniq.qualityLevel; 
			}
		}
	}
	for( int i = 0; i < countOfDropableUniqs; ++i ){ // drop only max available qlvl
		if( UniqueItems[drop[i]].qualityLevel < maxQlvl ){ // if change to < maxQlvl - 1, then will be variation bitween 2 max levels
			drop[i--] = drop[--countOfDropableUniqs];
		}
	}
	// для рандомизации без морфинга старых вещей можно использовать флаг D_MAGIC_FIND
	// но если сделать в th1-hd случайный выбор, то будут морфится вещи при переносе обратно в TH1
	// поэтому это оставляется до перехода полностью на TH1-HD
	if( countOfDropableUniqs ){
		return drop[RNG(countOfDropableUniqs)];
	}else{
		// подходящий по базе уник не найден
		// тут можно добавить генерацию случайного уника (rare/epic)
		//return 863; // test unique drop
	}
	return uniqueItemIndex;
}

//----- (004270AE) --------------------------------------------------------
void __fastcall GetUniqueItem(int itemIndex, int uniqueItemIndex) // #unique item appearance, #unique item looks, #mirroring, #female
{
	UniqueItem& uniq = UniqueItems[uniqueItemIndex];
	Item& item = Items[itemIndex];
	for( int i = 0; i < uniq.effectCount; i++ ){
		Item_ApplyAffix(item, uniq.Effect[i], 0, 0, 1, i, 0, 0, false);
	}
	if( uniq.uniqueGraphics >= 0 ) item.GraphicValue = uniq.uniqueGraphics;

	// female classes (Archer / Rogue)
	if (is(Players[CurrentPlayerIndex].ClassID, PC_1_ARCHER, PC_4_ROGUE)) {
		switch (uniqueItemIndex) {
		case 2879:// glittering greed
		case 23 or 24 or 25 or 26 or 27 or 28 or 2055 or 2057 or 2058  //  Boots
			or 2197 or 2198 or 2199 or 2200 or 2201/*empyrean striders*/:
		case 1774 or 2060 or 1775 or 1776 /*holy dogma*/ or 1777 or 2093 or 1778 or 1779 or 4575 // Cloth Gloves
			 or 1780 or 2059 or 1781 or 1782 or 1783 or 2061 or 1784 or 1785:
		case 1505 or 1507 or 1509: // Sash
		case 2271: // Rags (Timeworn Tapestry)
		case 1208 or 1209 or 1210 or 1211 or 1212 or 1213 or 1214 or 1215 or 2043 or 2044: // Scalemail Gauntlets
		case 599: // Cape
		case 2256 or 2255 or 1332 or 1328 or 1333 or 4654: // Chainmail Boots
		case 2125: // The Black Widow (Horn Bow)
		case 82 or 126 or 2068: // Tight Bow
		//case 1931: // Frozen Cave Arch (Tight War Bow)
		case 1034 or 1035 or 1036 or 1037 or 1038 or 1039: // Khopesh
		case 39 or 496 or 498 or 707: // Precision Bow
		case 3349 or 3351: // "The Sleeping Sun", "Apostle of Darkness"
		case 3466 or 3469 or 3470 or 3471 or 3472 or 3473 
			or 3564 or 3565 or 3566 or 3567 or 3568
			or 3534 or 3535 or 3536 or 3537 or 3538: // grimsteel mask unique pics ported from male classes
		case 3532 or 3533: // "Moonlight Cap", "Bloodmoon Sallet"
		case 1344 or 1337 or 1345 or 1339 or 1338 or 1341 or 1340: // "Boots of Walling", "Death Striders", etc.
		case 3780 or 3783 or 3784 or 3785 or 3790 or 3792 or 3789: // some Gothic Faceplate sprites are reused from other male versions
					item.mirrorImage = 1;						break;

					// ---------- - (UNMIRROR) --------------
		case 415 or 416 or 417: // Falchion
		case 490: // Renegade's Wrath 
					item.mirrorImage = ~item.mirrorImage;		break;

					// ----------- ( SPECIFIC PICS ) --------------
		case 2056:	item.GraphicValue = 3608 + 168;				break;
		case 2202:	item.GraphicValue = 3610 + 168;				break;
		}
	}

	// male classes (Warrior / Mage / Monk / Savage)
	else { 
		switch (uniqueItemIndex) {
		case 28:
		{ if (Players[CurrentPlayerIndex].fullClassId == PFC_MONK) { item.GraphicValue = 1176 + 3885; } }
			break;
		case 2202:
		{ if (Players[CurrentPlayerIndex].fullClassId == PFC_MONK) { item.GraphicValue = 570 + 3885; } }
			break; 
		//case 1216 or 1217 or 1218 or 1219 or 1220 or 1221: // Scalemail Gloves
		case 2125 ://2120 or 2121 or 2122 or 2123 or 2124: // Horn Bow
		case 83 or 715 or 2069: // Tight Bow
		case 1983 or 1931: // Tight War Bow
		case 39 or 496 or 498 or 707 or 4572: 
			item.mirrorImage = ~item.mirrorImage; 
			break; // Precision Bow (mirrored as base, extra mirroring didn't work here: ~item.mirrorImage trick is used)
		//case 497 :
		case 157: // Belphegor's Ravager
		case 3354: // "The Sacrificial Moon"
		case 3484 or 3493 or 3502 or 3520:
		case 4212:
		case 4618 or 4619 or 4620 or 4621 or 4622 or 4623 or 4624 or 4625 or 4626:// monk's fleshgrinder
					item.mirrorImage = 1;			break;
		case 120 or 122 or 124: // Arkaine's Valor expanded Mails (for Monks)
					item.GraphicValue = 847 + 168;	break;
		case 121 or 123 or 125: // Arkaine's Valor expanded Plates (for Monks)
					item.GraphicValue = 849 + 168;	break;
		}
	}

	if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
		if (is(uniqueItemIndex, 82, 83, 126, 4572, 715, 2068, 2069)) { // #1399 base uniques, mamluk "battle bow"
			item.mirrorImage = ~item.mirrorImage;
		}
	}
	if (Players[CurrentPlayerIndex].fullClassId == PFC_BEASTMASTER) {
		if (is(uniqueItemIndex, 327, 328, 329, 2461, 2462, 2463, 2464, 2465, 2466)) {
			item.GraphicValue = 2364+168;
		}
	}
	/*
	if (uniqueItemIndex == 2056 && (is(Players[CurrentPlayerIndex].ClassID, PC_1_ARCHER, PC_4_ROGUE))) { // "Stabilizing Toecaps"
		item.GraphicValue = 3608+168;
	}
	if (uniqueItemIndex == 2202 && (is(Players[CurrentPlayerIndex].ClassID, PC_1_ARCHER, PC_4_ROGUE))) { // winterwalk
		item.GraphicValue = 3610 + 168;
	}
	*/
	if( uniq.uniqueSetIndex != -1 ){
		item.MagicLevel = ML_3_SET;
	}else{
		item.MagicLevel = ML_2_UNIQUE;
	}
	strcpy(item.FullMagicalItemName, uniq.Name);
	item.Price = uniq.GoldValue;
	if( item.MagicCode == MC_27_QUEST_UNIQUE || item.dropType & D_UNIQ ){ // Квестовый уник (0x1B - Quest Unique Item) 
		// пробуем убрать фиксирование параметров квестовых уников
		// раньше в seed( random seed ) квестового уника записывался индекс предмета в таблице уников
		// так и не морфятся, и нельзя получить два квестовых уника с одним индексом, так как дубликаты проверяются по seed, а он у них одинаковых
		// побочный эффект - фиксированные параметры, такак рандомный исходный ключ всегда один
		// поскольку сид занимает 4 байта( 32 бита ), а индекс уника можно запихать в 12 бит( 4095 максимум )
		// то я поменял полную заменую сида индексом, на запись индекса в младшие 12 бит сида
		// так он остается случайным
		// полные дубликаты так все равно не пройдут, а квестовые уники с разными параметрами пожалуйста
		item.uniqIndexSeed = uniqueItemIndex; // Защита от дублирования квестовых уников (вроде оригинальная) // здесь фиксируется одиноковый сид для квестовых уников
		if( ! (item.dropType & D_UNIQ) ){ // check for needs
			item.dropType = D_0_SIMPLE;
			item.quality = 63; // TODO: review max qlvl для квестовых уников
		}
	}
	item.UniqueItemIndex = uniqueItemIndex;
	Items[itemIndex].dropType |= D_ALLOW_DUP;
}

// Had have duplicate func (00427544) CreateItem() / Item_CreateQuestUnique() only used for quest (single player) not returning index
//----- (004271F9) --------------------------------------------------------
int __fastcall SpawnUnique(int uniqueItemIndex, int row, int col, bool sendMsg /*= false*/, int baseId /*= -1*/)
{
	int itemIndex = -1;
	if( (uint)uniqueItemIndex < count_UniqueItems && GetItemSpace(row, col, itemIndex) ){
		int qlvl = ItemLevelByDungeonLevel();
		Item& item = Items[itemIndex];
		SetRndSeed( (int)item.MakeSeed() );
		GetItemAttrs(itemIndex, baseId >= 0 ? baseId : UniqueItems[ uniqueItemIndex ].baseId, qlvl);
		item.dropType = D_UNIQ; // new uniq drop type to avoid morphing for non quest magic code
		item.quality = qlvl;
		GetUniqueItem(itemIndex, uniqueItemIndex);
		SetupItemAnim(itemIndex);
		if( NetItem ){
			if( sendMsg ) NetSendCmdDItem(FALSE, itemIndex);
		}
	}
	return itemIndex;
}

//----- (0042728C) --------------------------------------------------------
void __fastcall SetRandomDurability(int itemIndex)
{
	Item& item = Items[itemIndex];
	if( item.CurDurability && item.CurDurability != ITEM_DUR_INDESTRUBLE ){
		item.CurDurability = RNG(item.BaseDurability >> 1) + (item.BaseDurability >> 2) + 1;
	}
}

enum DROP_CHANCE { ChanceMagicBase = 5, ChanceUniqueMob = 1, MagicFindUniqueDiv = 4 };
constexpr int ChanceUniqueBoss[CurItemGenVersion + 1] = { 10, 10, 6, 6, 10, 10, 6, 8 }; // last value is current chance
//----- (004272CE) - Item regeneration from seed info, should not have any player specific filters
void __fastcall SetupAllItems(int itemIndex, int baseItemIndex, i64 seed, int qlvl, int chanceInPercent, int IsGoodMagicItem, int allowDup, int pregen, bool identified, int playerMask, int magicFind, int rare, int genVersion, int uniqIndex, uchar difficulty, uchar magicLevel /*= 0*/ )
{
	Item& item = Items[itemIndex];
	const BaseItem& baseItem = BaseItems[baseItemIndex];

	// TODO: move player specific to smart drop filter function
	// removing equippable item drops for characters with "Clean Hands" GCы
	if( ( (Players[CurrentPlayerIndex].gameChanger & BIT(GC_14_CLEAN_HANDS)) || Players[CurrentPlayerIndex].gameChanger & BIT(GC_9_NIGHT_KIN) ) // << check for th GC
	 && (is(baseItem.EquippedLocation, EL_10_HANDS, EL_11_FEET, EL_1_ONE_HANDED, EL_2_TWO_HANDED, EL_3_TORSO, EL_4_HEAD, EL_5_FINGER, EL_6_NECK, EL_9_WAIST)) // check for item types (equippable on body slots)
	 && (BaseItems[baseItemIndex].MagicCode != MC_27_QUEST_UNIQUE) ){ // and this is for quest uniques (otherwise they morph to gold in inventory within seconds)
		item.ItemCode = IC_11_GOLD;
		baseItemIndex = BI_0_GOLD;
	}
	// magic find now affects whether the white thing will be magic or unique
	// if you want to affect the chance of dropping things instead of gold or nothing, the default is 10% (25% of 40%), you must change GetRandomBaseForMonsterDrop
	if( playerMask && baseItemIndex != BI_0_GOLD && BaseItems[baseItemIndex].ItemCode > IC_0_OTHER /*&& baseItem.MagicCode != MC_27_QUEST_UNIQUE*/){
        magicFind = Players[CurrentPlayerIndex].magicFind; // if playerMask is not zero (as in RecreateItem)
	}
	if( genVersion == -1 ){ genVersion = CurItemGenVersion; }
	item.SetSeed(seed, genVersion);
	SetRndSeed((int)seed); // TODO: expand seed
	GetItemAttrs(itemIndex, baseItemIndex, qlvl / 2);
	item.Identified = identified;
	if( item.ItemCode == IC_11_GOLD ){
		ActGoldFind( item.amount );
	}
	item.dropType = D_0_SIMPLE;
	item.quality = qlvl;
	item.difficulty = difficulty;
	item.magicFindSeed = magicFind;
	if( pregen ){
		item.dropType = D_PREGEN;
		item.quality = qlvl;
	}
	if( IsGoodMagicItem ){
		item.dropType |= D_GOOD;
	}
	if( chanceInPercent == ChanceUniqueBoss[genVersion] ){ // for boss
		item.dropType |= D_BOSS;
	}else if( chanceInPercent == ChanceUniqueMob ){
		item.dropType |= D_MOB;
	}
	if( item.MagicCode == MC_27_QUEST_UNIQUE ){ // Квестовый уник
		if( item.EquippedLocation != EL_7_INVENTORY ){
			// пробуем убрать фиксирование параметров квестовых уников
			GetUniqueItem( itemIndex, uniqIndex );
		}
	}else{
		int itemQuality = -1;
		int magicChance = ChanceMagicBase * 10;
		if( magicFind ){
			magicChance += magicChance * magicFind / 100;
		}
		if( RNG(1000) <= magicChance || RNG(100) <= qlvl ){ // may be qlvl here have to threat as magicChance2 (?)
			itemQuality = qlvl;
		}else if( item.MagicCode == MC_23_STAFF ){
			itemQuality = qlvl;
		}else if( baseItem.IsUncommon ){
			itemQuality = qlvl;
		}else if( IsGoodMagicItem ){
			itemQuality = qlvl;
		}
		if( chanceInPercent == ChanceUniqueBoss[genVersion] ){
			itemQuality = qlvl + 4;
		}
		if (itemQuality != -1) {
			// базовый шанс на дроп уника/рарки в th2 для обычных монстров 0,1%
			//int uniqueChance = chanceInPercent * (th2 ? (chanceInPercent == ChanceUniqueBoss ? 10 : 1) : 1);  old braintwister code, replaced with the one below
			int uniqueChance = chanceInPercent;
			if (chanceInPercent == ChanceUniqueBoss[genVersion]) {
				uniqueChance *= 10;
			}
			if (magicFind) {
				uniqueChance += uniqueChance * magicFind / 100 / MagicFindUniqueDiv;
			}
			int uniqueItemIndex = -1;
			if (uniqueChance >= RNG(1000)) {// unique chance, TODO: move to Smartdrop from recreate, this can avoid further extention of ChanceUniqueBoss
				uniqueItemIndex = SelectRandomUnique(itemIndex, itemQuality, uniqueChance, true); // unique chance
				//if( uniqueItemIndex == -1 && ! rare && magicLevel == 0) __debugbreak();
				if( (uniqueItemIndex == -1 && DropRares && GameMode != GM_CLASSIC && magicLevel != ML_1_MAGIC) || rare ){ // rare flag is for existing items
					InitRare(itemIndex, itemQuality / 2, itemQuality, uniqueChance, IsGoodMagicItem);
					rare = 1;
				}
			}
			if (!rare) {
				if( uniqueItemIndex == -1 || magicLevel == ML_1_MAGIC ){
					#ifdef ALLOW_CURSE_ON_DOOM // патч пока отменен, так как приводил к морфингу вещей
					if( IsGoodMagicItem == 123 ) IsGoodMagicItem = 0; // патч на корсы
					#endif
					ApplyAffixesToItem(itemIndex, baseItemIndex, itemQuality >> 1, itemQuality, IsGoodMagicItem, 2); // Magic item generation 
				}else{
					GetUniqueItem(itemIndex, uniqueItemIndex);
					//item.dropType |= D_ALLOW_DUP;
				}
			}
		}
		
		if( CanGrind() ){ // TODO: remove all player dependencies from item regeneration (move it to drop func), also canGrind may be item flag
		    TryToAddSocketsToItem( item, qlvl, difficulty );
		}
		
		if( item.MagicLevel != ML_2_UNIQUE ){
			SetRandomDurability(itemIndex); // случайное получение текущей прочности ( 25% - 75% от базовой )
		}
	}
	SetupItemAnim(itemIndex);
}

//----- (th2) ------------------------------------------------------------
void __fastcall SetupAllItemsSmartdrop(int itemIndex, int baseItemIndex, i64 seed, int qlvl, int chanceInProcent, int IsGoodMagicItem, int allowDup, int pregen, int playerMask, int magicFind, int rare)
{
	Item& item = Items[itemIndex];
	Player& player = Players[CurrentPlayerIndex]; // TODO: change to playerIndex
	#if 0
	bool IsSummonerInSession = false; // deactivating duke's patch for summoner books in mp
	if( MaxCountOfPlayersInGame > 1 ){
		for( int i = 0; i < NetPlayerCount || !IsSummonerInSession; i++ ){
			IsSummonerInSession = is(Players[i].fullClassId, PFC_DEMONOLOGIST, PFC_BEASTMASTER, PFC_NECROMANCER);
		}
	}
	#endif
	while(true){
		SetupAllItems(itemIndex, baseItemIndex, seed, qlvl, chanceInProcent, IsGoodMagicItem, allowDup, pregen, false, playerMask, magicFind, rare, -1, 0, Difficulty);
		// TODO: move to BookSpellSkip / SmartDropFilter()
		if( not(player.fullClassId, PFC_DEMONOLOGIST, PFC_BEASTMASTER, PFC_NECROMANCER) && item.MagicCode == MC_24_BOOKS && is(item.SpellIndex, PS_52_LESSER_SUMMON, PS_53_COMMON_SUMMON, PS_54_GREATER_SUMMON)
		 #if 0
		 && !IsSummonerInSession // deactivating duke's patch for summoner books in mp
		 && MaxCountOfPlayersInGame == 1 // MP smart drop
		 #endif
		 ){
			seed = GetRndSeed();
			continue;
		}else{
			break;
		}
	}
}

//----- (th2) ------------------------------------------------------------
int MonsterDropLevel( int monsterIndex )
{
	Monster& monster = Monsters[ monsterIndex ];
	int monsterLevel = monster.BasePtr->MonsterBaseLevel;
	// special exceptions for horror bosses
	int bossIndex = monster.newBossId - 1;
	//if( Difficulty == DL_0_HORROR ){
		if( monster.newBossId > 0 ){
			if( bossIndex == UM_9_The_Butcher ){ // Butcher drop level
				monsterLevel = 12 + Difficulty/*16*/;
			} else if( bossIndex == UM_1_Skeleton_King && MaxCountOfPlayersInGame == 1 && Dungeon != DUN_99_ANCIENT_CRYPTS){
				monsterLevel = 20 + Difficulty/*24*/;
			}
		}
	//}
	switch( bossIndex ){
	case UM_587_Hephasto_the_Armorer:	monsterLevel = 57/*61*/; break; // commented values of old, they seem to be monster level and boss level adds +4 to them
	case UM_583_Lich_King:				monsterLevel = 56/*60*/; break;
	case UM_584_Mordessa:				monsterLevel = 55/*59*/; break;
	case UM_586_Wielder_of_Shadowfang:	monsterLevel = 56/*60*/; break; 
	}
	return monsterLevel;
}

// Create any item from a monster (or unqiue if monster gives one up)
//----- (0042742E) --------------------------------------------------------
int __fastcall SpawnItem(int monsterIndex, int row, int col, int sendMsg)
{
	int itemIndex = -1;
	int baseItemIndex = 0;
	int isGoodMagicItem = 0;
	Monster& monster = Monsters[monsterIndex];
	// (20) расширение номера босса 00427444
	if( !monster.newBossId && (!(Monsters[monsterIndex].BasePtr->ItemDropSpecials & 0x8000) || MaxCountOfPlayersInGame == 1) ){
		Quest& blackMushroomQuest = Quests[Q_1_BLACK_MUSHROOM];
		if( blackMushroomQuest.status == QS_2_IN_PROGRESS && blackMushroomQuest.status2 == 5 ){
			baseItemIndex = BI_331_BRAIN;
			blackMushroomQuest.status2 = 6;
		}else{
			baseItemIndex = GetRandomBaseForMonsterDrop(monsterIndex); // baseItemIndex = -833; // вариант теста уника
			if( baseItemIndex == 0 ){
				return -1; // no drop
			}else if( baseItemIndex > 0 ){
				isGoodMagicItem = 0;
				baseItemIndex -= 1; // normal item or magic
			}else{                  // itemIndex < 0, uniq
				return SpawnUnique(-1 - baseItemIndex, row, col, sendMsg);
			}
		}
	}else{ // дроп боссов и квестовых монстров
        bool dropBook = false;
        if( /*MaxCountOfPlayersInGame == 1 && */ ( is( GameModeType(), GAME_MODE_TYPE::NIGHTMARE, GAME_MODE_TYPE::IRON ) ) ) {
			int treasure_hunter_divider_for_book_drops = 0;
			if (Players[CurrentPlayerIndex].ClassID == PC_2_MAGE) {
				if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
					// do nothing: it remains 0 chance for book drop
				}
				else if (HasTrait(CurrentPlayerIndex, TraitId::TreasureHunter)) {
					treasure_hunter_divider_for_book_drops = 5; // with Treasure Hunter trait, chance for getting book from a boss is 20%
				}
				else {
					treasure_hunter_divider_for_book_drops = 3; // ..otherwise 33%
				}
			}
			else if (Players[CurrentPlayerIndex].fullClassId == PFC_ROGUE
				|| (Players[CurrentPlayerIndex].fullClassId == PFC_ASSASSIN && HasTrait( CurrentPlayerIndex, TraitId::Black_Witchery ) ) ) {
				if (HasTrait(CurrentPlayerIndex, TraitId::TreasureHunter)) {
					treasure_hunter_divider_for_book_drops = 8; // with Treasure Hunter trait, chance for getting book from a boss is 12%
				}
				else {
					treasure_hunter_divider_for_book_drops = 5; // ..otherwise 20%
				}
			}
			if (RNG(treasure_hunter_divider_for_book_drops) == 1) {
				dropBook = true;
			}
        }

        if( dropBook ){
            return CreateTypeItem( row, col, 0, 0, MC_24_BOOKS, true, 0 );
        }else{
		    baseItemIndex = GetRandomBaseForHiQualityObjectAndBossDrop(monsterIndex);
		    if( baseItemIndex < 0 ){
			    return SpawnUnique(-1 - baseItemIndex, row, col, sendMsg);
		    }else{ // baseItemIndex >= 0
			    isGoodMagicItem = 1; // нет проклятых вещей с боссов
			    #ifdef ALLOW_CURSE_ON_DOOM // патч пока отменен, так как приводил к морфингу вещей
			    if( Difficulty == 2 ){
				    isGoodMagicItem = 123; // патч на корс
			    }
			    #endif
		    }
        }
	}
	if( GetItemSpace(row, col, itemIndex) ){
		int chance = monster.newBossId == 0 ? ChanceUniqueMob : ChanceUniqueBoss[CurItemGenVersion];
		DropItemWithQlvlFilteredByMagicLevel(itemIndex, baseItemIndex, GetRndSeed(), MonsterDropLevel(monsterIndex), chance, isGoodMagicItem, 0, 0, monsterIndex);
		if( NetItem ){
			if( sendMsg ) NetSendCmdDItem(FALSE, itemIndex);
		}
	}
	return itemIndex;
}

//----- (004275E4) Create any item from an object, etc (chest, barrel, sarc)
int __fastcall CreateRndItem(int row, int col, int isGoodMagicItem, int sendMsg, int delta, int playerMask)
{
	int qlvl = ItemLevelByDungeonLevel();
	int baseItemIndex = isGoodMagicItem ? GetRandomBaseForHiQualityObjectAndBossDrop(-1)
	                                    : GetRandomBaseForLowQualityObjectDrop();
	return SpawnSomethingQlvl(baseItemIndex, row, col, qlvl, sendMsg, isGoodMagicItem, delta, playerMask);
}

//----- (th2) -------------------------------------------------------------
int __fastcall DropItemFromSlain(int row, int col, int isGoodMagicItem, int sendMsg, int maybeIsGenerationTime, int playerMask, int itemCode)
{
	int itemIndex = -1;
	if( GetItemSpace(row, col, itemIndex) ){
		int numberOfRemainingAttempts = 100;
		do{
			int baseItemIndex = GetRandomBaseForHiQualityObjectAndBossDrop(-1);
			SetupAllItems(itemIndex, baseItemIndex, GetRndSeed(), 2 * ItemLevelByDungeonLevel(), 1, isGoodMagicItem, 0, maybeIsGenerationTime, false, playerMask, 0, 0, -1, 0, Difficulty);
		}while( Items[itemIndex].ItemCode != itemCode && --numberOfRemainingAttempts > 0 );
		// if where is no successful attempt, drop last generated
		if( NetItem ){
			if( sendMsg ) NetSendCmdDItem(FALSE, itemIndex);
		}
	}
	return itemIndex;
}

//----- (0042768D) --------------------------------------------------------
void __fastcall SetupAllUseful( int itemIndex, int seed, int qlvl )
{
	Items[itemIndex].SetSeed(seed, CurItemGenVersion);
	SetRndSeed(seed);
	int randFactor = RNG(7);
	int baseItemIndex = GameMode == GM_CLASSIC ? BI_2652_POTION_OF_HEAL_CLS : BI_24_POTION_OF_HEALING;
    bool isTPForbidden = isTownPortalForbidden();
	switch (randFactor ){
	case 0:
		if( qlvl > 1 && !isTPForbidden ){
			baseItemIndex = GameMode == GM_CLASSIC ? BI_2660_RELICT_OF_TP_CLS : BI_27_RELICT_OF_TOWN_PORTAL; //BI(BI_27_RELICT_OF_TOWN_PORTAL);
		}else{
			baseItemIndex = GameMode == GM_CLASSIC ? BI_2652_POTION_OF_HEAL_CLS : BI_24_POTION_OF_HEALING;
		}
		break;
	case 1:		baseItemIndex = GameMode == GM_CLASSIC ? BI_2652_POTION_OF_HEAL_CLS : BI_24_POTION_OF_HEALING;		break;
	case 2:		baseItemIndex = GameMode == GM_CLASSIC ? BI_2652_POTION_OF_HEAL_CLS : BI_24_POTION_OF_HEALING;		break;
	case 3:
		if( qlvl > 1 && !isTPForbidden ){
			baseItemIndex = GameMode == GM_CLASSIC ? BI_2660_RELICT_OF_TP_CLS : BI_27_RELICT_OF_TOWN_PORTAL;
		}else{
			baseItemIndex = GameMode == GM_CLASSIC ? BI_2652_POTION_OF_HEAL_CLS : BI_24_POTION_OF_HEALING;
		}
		break;
	case 4:
	case 5:		baseItemIndex = GameMode == GM_CLASSIC ? BI_2653_POTION_OF_MANA_CLS : BI_25_POTION_OF_MANA;			break;
	case 6:		baseItemIndex = GameMode == GM_CLASSIC ? BI_2652_POTION_OF_HEAL_CLS : BI_24_POTION_OF_HEALING;		break;
	}
	GetItemAttrs(itemIndex, baseItemIndex, qlvl);
	Items[itemIndex].dropType = D_USEFUL;
	Items[itemIndex].quality = qlvl;
	SetupItemAnim(itemIndex);
}

//----- (0042770B) --------------------------------------------------------
int __fastcall CreateRndUseful(int playerIndex, int objectRow, int objectCol, bool sendMsg)
{
	int itemLevel = ItemLevelByDungeonLevel();
	int itemIndex = -1;
	if( GetItemSpace(objectRow, objectCol, itemIndex) ){
		SetupAllUseful(itemIndex, GetRndSeed(), itemLevel );
		if( NetItem ){
			if( sendMsg ) NetSendCmdDItem(FALSE, itemIndex);
		}
	}
	return itemIndex;
}

// Create any item of a specific type
//----- (0042777A) --------------------------------------------------------
int __fastcall CreateTypeItem( int row, int col, int isGoodMagicItem, int itemCode, int magicCode, int sendMsg, int delta, int smartDropFlag /*=0*/, bool minQlvlLimit /*=false*/ )
{
	int qlvl = ItemLevelByDungeonLevel();
	int baseItemIndex = itemCode == IC_11_GOLD ? BI_0_GOLD : RndTypeItems(itemCode, magicCode, qlvl, smartDropFlag, minQlvlLimit);
	return SpawnSomethingQlvl(baseItemIndex, row, col, qlvl, sendMsg, isGoodMagicItem, delta);
}

//----- (00427824) Recreate any item with the proper info -----------------
void __fastcall RecreateItem( int itemIndex, short baseItemIndex, int dropType, i64 seed, int amount, uchar mlIdentified, int genVersion, int quality, int magicFind, int uniqIndex, array<short, EnchantMax_5> enchantIndexes, QUENCH_AFFIX quenchAffix, uchar quenchLevel, uchar socketsAdded, array<short, SocketMax_6> sockets, uchar difficulty, uchar flag)
{
	Item& item = Items[itemIndex];
	bool identified = mlIdentified & 1;
	uchar magicLevel = mlIdentified >> 1;

	LimitToMax(difficulty, DL_COUNT - 1);
	#if 0 // TODO: check for possible init unifying
	item.baseItemIndex = baseItemIndex;
	item.dropType      = dropType;
	item.seed          = (int)seed;
	item.amount        = amount;
	item.Identified    = identified;
	item.genVersion    = genVersion;
	item.quality       = quality;
	item.magicFindSeed = magicFind;
	item.uniqIndexSeed = uniqIndex;
	#endif
	if( ! item.saveVersion ) item.saveVersion = SaveVersion;
	if( item.saveVersion < 25 ){
		if( (dropType & D_OLD_USEFUL ) == D_OLD_USEFUL  ){ dropType &= ~D_OLD_USEFUL; dropType |=  D_USEFUL;  }
		if( (dropType & D_OLD_RARE   ) == D_OLD_RARE    ){ dropType &= ~D_OLD_RARE;   dropType |=  D_RARE;    }
		if( (dropType & D_OLD_ENCHANT) == D_OLD_ENCHANT ){ dropType &= ~D_OLD_ENCHANT; dropType |=  D_ENCHANT | D_ENCHANT_V1; }
		if( item.MagicLevel == 4 /*ML_4_ENCHANT_OLD*/ ) item.MagicLevel = ML_0_USUAL;
	}
	item.saveVersion = CurSaveVersion;
	int extraGen = dropType & D_EXTRA; dropType &= ~D_EXTRA;
	if( baseItemIndex == BI_0_GOLD){
		SetPlayerHandItem(item, baseItemIndex);
		item.SetSeed(seed, genVersion);
		item.dropType = D_0_SIMPLE;
		item.amount = amount;
		SetGraphicToGold(item);
		extraGen = 0;
	}else if( dropType == D_0_SIMPLE ){
        SetRndSeed((int)seed); // TODO: expand seed
		SetPlayerHandItem(item, baseItemIndex);
		item.SetSeed(seed, genVersion);
	}else if( dropType & D_UNIQ ){
		#if ! CHEATS
		baseItemIndex = UniqueItems[ uniqIndex ].baseId;
		#endif
		item.SetSeed(seed, genVersion);
		SetRndSeed((int)seed );
		// TODO: check for SetPlayerHandItem need
		GetItemAttrs(itemIndex, baseItemIndex, quality);
		item.dropType = dropType; // new uniq drop type to avoid morphing for non quest magic code
		item.quality = quality;
		item.Identified = identified;
		GetUniqueItem(itemIndex, uniqIndex);
	}else if( dropType & D_SHOP ){
		RestoreTownersItems(itemIndex, baseItemIndex, dropType, (int)seed, amount, genVersion, quality, magicFind); // TODO: expand seed
	}else if( dropType & D_USEFUL ){
		/*if( IsExeValidating )*/ forceCallforSync = RNG(baseItemIndex); // для синхронизации с генерацией бутылок
		//else SetupAllUseful(itemIndex, seed, qlvl); // выбор базы для бутылок
		SetPlayerHandItem( item, baseItemIndex );
		item.SetSeed(seed, genVersion);
	}else{
		int chanceToUniq  = dropType & D_MOB ? ChanceUniqueMob : 0;
		if( dropType & D_BOSS )	chanceToUniq = ChanceUniqueBoss[genVersion];
		bool good   = dropType & D_GOOD;
		bool dup    = dropType & D_ALLOW_DUP;
		bool pregen = dropType & D_PREGEN;
		bool rare   = dropType & D_RARE;
		SetupAllItems(itemIndex, baseItemIndex, seed, quality, chanceToUniq, good, dup, pregen, identified, 0, magicFind, rare, genVersion, uniqIndex, difficulty, magicLevel);
	}

	// sockets array recreation has been moved here from LoadItem for correct network sync
	item.sockets = sockets;
	int socketsGemmed = SocketMax_6;
	for(; socketsGemmed > 0; --socketsGemmed ){
		if( sockets[socketsGemmed - 1] != 0 ) break;
	}
	LimitToMax(item.socketsBase, SocketMax_6);
	LimitToMax(socketsAdded, SocketMax_6);

	if( extraGen & D_ENCHANT ){
		ApplyEnchants(itemIndex, baseItemIndex, seed, quality, genVersion, enchantIndexes, extraGen & D_ENCHANT_V1, difficulty);
		item.socketsBase = 0;
		item.socketsAdded = socketsAdded;
		item.socketsAmount = socketsAdded;
		item.sockets = sockets;
	}
	if( extraGen & D_QUENCHED ){
		ApplyQuench(itemIndex, baseItemIndex, seed, quality, genVersion, quenchAffix, quenchLevel);
	}

	if( item.genVersion >= 5 || item.MagicLevel >= ML_2_UNIQUE ){
		if( ! (extraGen & D_ENCHANT) && item.MagicLevel != ML_0_USUAL ) socketsAdded = 0; // Allow added sockets only for enchanted (Craft) and base (Transmute)
	}else{
		socketsAdded = std::max( socketsAdded, item.socketsBase );
		item.socketsBase = 0;
	}

	int socketsAmount = item.socketsBase + socketsAdded;
	if( socketsAmount < socketsGemmed ){
		socketsAdded += socketsGemmed - socketsAmount; // prevent from losing gems after affix/formula changed
	}
	socketsAmount = item.socketsBase + socketsAdded;
	LimitToMax(socketsAmount, SocketMax_6);
	item.socketsAmount = socketsAmount;
	item.socketsAdded = socketsAdded;

	if( item.socketsAmount > 0 ){
		ApplyGemEffects(item);
	}
}

//----- (th2) -------------------------------------------------------------
void CreateItemIgnoringDungeonLevel( int row, int col, int isGoodMagicItem, int itemCode, int magicCode, bool sendMsg )
{
	int qlvl = 1000; // expanded from th2 100
	int baseItemIndex = itemCode == IC_11_GOLD ? BI_0_GOLD : RndTypeItems(itemCode, magicCode, qlvl);
	SpawnSomethingQlvl(baseItemIndex, row, col, qlvl, sendMsg);
}

//----- (00427CEE) --------------------------------------------------------
void SpawnMagicRock()
{
	for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
		Object& object = Objects[ObjectActive[objectIndexIndex]];
		if( object.BaseObjectIndex == BO_23_MAGIC_ROCK_TABLE ){
			SpawnSomething(BI_9_MAGIC_ROCK, object.Row, object.Col, 0, 0, ISEL_TOP, 1);
			break;
		}
	}
}

//----- (th3) -------------------------------------------------------------
void RandomArea(int& row, int& col, int randomAreaSize, bool (__fastcall *itemSpaceOk)(int,int,int))
{
	int tries = 0;
	bool failed = 0;
	do{
		if( ++tries > 1000 && randomAreaSize > 1 ){
			--randomAreaSize;
		}
		row = MapDirty_16 + RNG(Map_40 * 2);
		col = MapDirty_16 + RNG(Map_40 * 2);
		failed = 0;
		for( int r = 0; r < randomAreaSize && !failed; r++ ){
			for( int c = 0; c < randomAreaSize && !failed; c++ ){
				failed = !itemSpaceOk(row + r, col + c, 0);
			}
		}
	}while( failed );
}

//----- (th2) -------------------------------------------------------------
int __fastcall SpawnSomethingQlvl(int baseItemIndex, int row, int col, int qlvl, bool sendMsg, int isGoodMagicItem /*= 1*/, bool delta /*= false*/, int playerMask /*= 0*/ )
{
	int itemIndex = -1;
	if( GetItemSpace(row, col, itemIndex) ){
		SetupAllItemsSmartdrop(itemIndex, baseItemIndex, GetRndSeed(), 2 * qlvl, 1, isGoodMagicItem, 0, delta, playerMask);
		if( NetItem ){
			if (sendMsg) NetSendCmdDItem(FALSE, itemIndex);
			if (delta) DeltaAddItem(itemIndex);
		}
	}
	return itemIndex;
}

// was duplicated by PutQuestItemOnMap (00427DC6) / SpawnQuestItem (00427BCE) with different anim phase, selection and identify
//----- (00427DC6) --------------------------------------------------------
int __fastcall SpawnSomething(int baseItemIndex, int row, int col, bool specQuest /*= 0*/, int randomArea /*= 0*/, int selFlag /*= 0*/, char dropping /*= 0*/, int amount /*= 0*/, bool sendMsg /*= 0*/)
{
	int itemIndex = -1;
	// TODO: may be need to change quests logic to not put item into wall before level morph
	auto itemSpaceOk = selFlag && !randomArea || selFlag == ISEL_TOP ? ItemSpaceOkIntoWall : ItemSpaceOk;
	if( randomArea ) RandomArea(row, col, randomArea); // drop quest item at random location in dungeon
	if( GetItemSpace(row, col, itemIndex, true, itemSpaceOk) ){
		Item& item = Items[itemIndex];
		int qlvl = ItemLevelByDungeonLevel();
		i64 seed = item.MakeSeed();
		SetRndSeed((int)seed);
		GetItemAttrs(itemIndex, baseItemIndex, qlvl);
		item.SetSeed(seed);
		item.dropType = D_0_SIMPLE;
		item.quality = qlvl;
		if( amount ) item.amount = amount;
		if( baseItemIndex == BI_0_GOLD ){
			SetGraphicToGold( item) ;
			item.IsReqMet = true;
		}
		SetupItemAnim(itemIndex);
		item.dropping = dropping;
		if( specQuest ){
			item.animPhase = 2;
			item.dropping = 1;
			item.FrameIndex = 1;
			item.playAnimation = 1;
			item.Identified = 1;
		}
		if( selFlag != ISEL_NONE ){ // selflag indicates creation of post-flippy item
			item.animPhase = selFlag;
			item.FrameIndex = item.baseItemIndex == BI_9_MAGIC_ROCK ? 11 : item.FramesCount;
			item.playAnimation = 0;
		}
		if( NetItem ){
			if( sendMsg ) NetSendCmdDItem(FALSE, itemIndex);
			// TODO: check for delta needing
		}
	}
	return itemIndex;
}

//----- (00427E8B) --------------------------------------------------------
void __fastcall RespawnItem(int itemIndex, int flipFlag)
{
	Item& item = Items[itemIndex];
	item.ItemCELFilePtr = FlipItemCELFiles[ItemFlipTable[item.GraphicValue]];
	item.FramesCount = Flip_FramesCount[ItemFlipTable[item.GraphicValue]];
	item.AnimWidth = 96;
	item.subOffsetX = 16;
	item.dropping = 0;
	item.gottenFromLand = 0;
	if( flipFlag ){
		item.FrameIndex = 1;
		item.playAnimation = 1;
		item.animPhase = 0;
	}else{
		item.FrameIndex = item.FramesCount;
		item.playAnimation = 0;
		item.animPhase = 1;
	}
	if( item.GraphicValue == 76 ){ // ITEM_ROCK
		int tableRow = -1, tableCol = -1;
		for( int objectIndexIndex = 0; objectIndexIndex < ObjectsCount; objectIndexIndex++ ){
			Object& object = Objects[ObjectActive[objectIndexIndex]];
			if( object.BaseObjectIndex == BO_23_MAGIC_ROCK_TABLE ){
				tableRow = object.Row;
				tableCol = object.Col;
				break;
			}
		}
		if( tableRow >= 0 && tableRow == item.MapRow && tableCol == item.MapCol ){
			item.animPhase = ISEL_TOP;
			item.FrameIndex = 11;
			item.playAnimation = 0;
			item.dropping = 1;
		}else{
			item.animPhase = 1;
		}
		PlayLocalSound(Flip_DropSound[ItemFlipTable[item.GraphicValue]], item.MapRow, item.MapCol);
	}
	if( item.GraphicValue == 126 ) // ITEM_INNSIGN
		item.animPhase = 1;
	if( item.GraphicValue == 140 ) // ITEM_ANVIL
		item.animPhase = 1;
}

//----- (00427F68) --------------------------------------------------------
void __fastcall DeleteItem( int itemIndex, int freeIndex )
{
	ItemAvailable[MaxItems_255 - ItemsCount] = itemIndex;
	ItemsCount--;
	if( ItemsCount > 0 && freeIndex != ItemsCount ){
		ItemActive[freeIndex] = ItemActive[ItemsCount];
	}
}

//----- (00427F9C) --------------------------------------------------------
void ClearMapFromInvalidItems()
{
	if( MaxCountOfPlayersInGame == 1 ){
		return;
	}
	for( int scanedMapRow = 16; scanedMapRow < 96; scanedMapRow++ ){
		int itemNum = ItemMap[ scanedMapRow ][ ScanedMapCol ];
		if( itemNum ){
			Item& item = Items[itemNum - 1];
			if( item.MapRow != scanedMapRow || item.MapCol != ScanedMapCol ){
				ItemMap[ scanedMapRow ][ ScanedMapCol ] = 0;
			}
		}
	}
	++ScanedMapCol;
	if( ScanedMapCol >= 96 ){
		ScanedMapCol = 16;
	}
}

//----- (00427FF9) --------------------------------------------------------
void ProcessItems()
{
	for( int itemIndexIndex = 0; itemIndexIndex < ItemsCount; itemIndexIndex++ ){
		int itemIndex = ItemActive[itemIndexIndex];
		Item& item = Items[itemIndex];

		if( item.playAnimation ){
			item.FrameIndex++;
			if( item.GraphicValue == 76 ){
				if( item.animPhase == 1 ){
					if( item.FrameIndex == 11 ){
						item.FrameIndex = 1;
					}
				}
				if( item.animPhase == 2 ){
					if( item.FrameIndex == 21 ){
						item.FrameIndex = 11;
					}
				}
			}else{
				if( item.FrameIndex == item.FramesCount >> 1 ){// на середине анимации играем звук
					PlayLocalSound(Flip_DropSound[ItemFlipTable[item.GraphicValue]], item.MapRow, item.MapCol);
				}
				if( item.FrameIndex >= item.FramesCount ){
					item.FrameIndex = item.FramesCount;
					item.playAnimation = 0;
					item.animPhase = 1;
				}
			}
		}
	}
	ClearMapFromInvalidItems();
}

//----- (004280D9) --------------------------------------------------------
void FreeUniqueItemCELFilesPtrs()
{
	for( int flipItemIndex = 0; flipItemIndex < FL_68_COUNT; flipItemIndex++ ){
		FreeMemZero(FlipItemCELFiles[flipItemIndex]);
	}
}

//----- (004280F6) --------------------------------------------------------
void __fastcall SetUniqueItemCELFile(int itemIndex)
{
	// баг при загрузке сингл сейва старой версии, если его пересохраняли в городе, в подземелье остаются структуры старых размеров
	// нужно таки добавить версию в Item и смотреть на загрузке каждого итема
	Item* NewItem = &Items[itemIndex];
	NewItem->ItemCELFilePtr = FlipItemCELFiles[ItemFlipTable[NewItem->GraphicValue]];
}

//----- (th3) -------------------------------------------------------------
TEXT_COLOR ItemColor(const Item& item)
{
	if( item.dropType & D_ENCHANT ) return C_10_Enchanted;
	if(IsQuestItem(item.baseItemIndex)) return C_11_Quest;
	switch( item.MagicLevel ){
	default /*USUAL*/: return item.socketsAmount > 0 ? C_7_Grey : C_0_White; break;
	case ML_1_MAGIC  : return C_1_Blue; break;
	case ML_2_UNIQUE : return item.dropType & D_RARE ? C_8_Pink : C_3_Gold; break;
	case ML_3_SET    : return C_4_Orange; break;
	}
}

//----- (00428117) --------------------------------------------------------
void __fastcall Item_ShowInfo(int itemIndex)
{
	Item& item = Items[itemIndex];
	if( item.ItemCode == IC_11_GOLD ){ //	если это золото
		sprintf(InfoPanelHeader, "%i gold %s", item.amount, formatGoldAmount(item.amount));
	}else{
		const bool identified = item.Identified || item.MagicLevel == ML_0_USUAL;
		strcpy(InfoPanelHeader, identified ? item.FullMagicalItemName : item.Name);
		CurFontColor = ItemColor(item);
	}
}

//----- (0042818A) --------------------------------------------------------
void __fastcall IdentifyItemWithCursor(uint playerIndex, uint onBodySlotIndex)
{
	Player& player = Players[playerIndex];
	Item& item = player.OnBodySlots[onBodySlotIndex];
	if( item.MagicLevel && !item.Identified && playerIndex == CurrentPlayerIndex ){// в th1 нет этих проверок, но возможно они нужны
		item.Identified = 1;
		UpdateItem( item );
		PlayLocalSound(S_1291, player.Row, player.Col);
		RecalcPlayer(playerIndex, 1);
		if( FindItemInInventoryOrBeltBySeed(playerIndex, Cur.UsedItemSeed, Cur.UsedItemBase) ){
			if( is(item.TypeID, ITEM_1_WEAPON, ITEM_2_ARMOR) || is(item.ItemCode, IC_12_RING, IC_13_AMULET) ){
				ClearPlayerItemBySeed(playerIndex, Cur.UsedItemSeed, Cur.UsedItemBase);
			}
		}
		//MinusManaOrChargeOrRelicByPriceOfSSpell (playerIndex, PS_5_IDENTIFY); // в th1 нет этого вызова
	}
	if( playerIndex == CurrentPlayerIndex ){// странная проверка. Единственное место откуда вызывается передает как playerIndex - CurrentPlayerIndex
		SetCursorGraphics(CM_1_NORMAL_HAND);
	}
}

//----- (004281D0) --------------------------------------------------------
void __fastcall RepairItemWithCursor(uint playerIndex, uint onBodySlotIndex)
{
	Player& player = Players[playerIndex];
	Item& item = player.OnBodySlots[onBodySlotIndex];
	if( item.CurDurability < item.BaseDurability && playerIndex == CurrentPlayerIndex ){ // в th1 этих проверок нет, но похоже они нужны
		PlayLocalSound(91, player.Row, player.Col);
		RepairItemWithBreaking(item, playerIndex);
		RecalcPlayer(playerIndex, 1);
		//MinusManaOrChargeOrRelicByPriceOfSSpell (playerIndex, PS_26_ITEM_REPAIR); // в th1 нет этого вызова
		if( playerIndex == CurrentPlayerIndex ){
			ClearPlayerItemBySeed(CurrentPlayerIndex, Cur.UsedItemSeed, Cur.UsedItemBase);
		}
	}
	if( playerIndex == CurrentPlayerIndex ){
		SetCursorGraphics(CM_1_NORMAL_HAND);
	}
}

//----- (0042822A) --------------------------------------------------------
void __fastcall RepairItemWithBreaking(Item& item, uint playerIndex)
{
	if( item.CurDurability != item.BaseDurability && item.BaseDurability > 1){
		int reduction_factor = 1;
		if( RNG(100) < PerkValue(SYNERGY_CAREFUL_MENDING, playerIndex) ){
			reduction_factor = 0;
		}
		int newBaseDarability = item.BaseDurability - reduction_factor;
		if( newBaseDarability <= 0 ){
			item.ItemCode = IC_M1_NONE;
		}else{
			item.BaseDurability = newBaseDarability;
			item.CurDurability = newBaseDarability;
		}
	}
}

//----- (004282A2) --------------------------------------------------------
void __fastcall RechargeStaffWithCursor(int playerIndex, int itemCellIndex)
{
	Player& player = Players[playerIndex];
	Item& item = player.OnBodySlots[itemCellIndex];
	if( item.SpellIndex && ( item.CurCharges < item.BaseCharges && playerIndex == CurrentPlayerIndex ) ){ // в th1 этих проверок нет, но похоже они нужны
		if (player.SpellType == SO_0_SKILL && (item.ItemCode == IC_12_RING || item.ItemCode == IC_13_AMULET)) {
			PlayGlobalSound(S_531_SOR_14+2);
			return;
		}
		int rechargeFactor = player.CharLevel / Spells[item.SpellIndex].BookQuality;
		RechargeStaffWithBreaking(item, RNG(rechargeFactor) + 1);
		PlayLocalSound(1295, player.Row, player.Col);
		RecalcPlayer(playerIndex, 1);
		//MinusManaOrChargeOrRelicByPriceOfSSpell (playerIndex, PS_27_STAFF_RECHARGE); // в th1 нет этого вызова
	}
	if( playerIndex == CurrentPlayerIndex ){
		SetCursorGraphics(CM_1_NORMAL_HAND);
	}
}

//----- (00428316) --------------------------------------------------------
void __fastcall RechargeStaffWithBreaking(Item& item, int chargesPerOneDur)
{
	if( chargesPerOneDur > 1 ){
		chargesPerOneDur--;
	}
	int chargesToMax = item.BaseCharges - item.CurCharges;
	int durabilityLeft = item.CurDurability - 1;
	int durabilityNeed = chargesToMax / chargesPerOneDur;
	if( durabilityNeed * chargesPerOneDur < chargesToMax ){// если округлилось до меньшего
		durabilityNeed++;// Округляем до большего
	}
	if( durabilityLeft >= durabilityNeed ){
		item.CurDurability -= durabilityNeed;
		item.CurCharges = item.BaseCharges;
	}else{
		item.CurDurability = 1;
		item.CurCharges += chargesPerOneDur * durabilityLeft;
	}
}

//----- (0042835D) --------------------------------------------------------
void __fastcall UseOilWithCursor(int playerIndex, int itemCellIndex)
{
	if( itemCellIndex < IS_50_47_Belt_Start
	 && itemCellIndex != IS_LeftRing
	 && itemCellIndex != IS_RightRing
	 && itemCellIndex != IS_Neck ){
		Player& player = Players[playerIndex];
		Item& item = player.OnBodySlots[itemCellIndex];
		if( playerIndex == CurrentPlayerIndex ){
		    if( FindItemInInventoryOrBeltBySeed(playerIndex, Cur.UsedItemSeed, Cur.UsedItemBase) ){
				if( ApplyOilToItem(item, player) ){
					PlayLocalSound(1294, player.Row, player.Col);
					ClearPlayerItemBySeed(playerIndex, Cur.UsedItemSeed, Cur.UsedItemBase);
					RecalcPlayer(playerIndex, 1);
				}
				SetCursorGraphics(CM_1_NORMAL_HAND);
			}
		}
	}
}

//----- (th3) -------------------------------------------------------------
int MaxDurability(Item& item)
{
	return item.EquippedLocation == EL_3_TORSO ? 600 : 300;
}

//----- (004283B5) --------------------------------------------------------
bool __fastcall ApplyOilToItem(Item& item, Player& player)
{
	if( item.TypeID == ITEM_3_JEWELRY_POTION_RELICT || item.TypeID == ITEM_4_GOLD || item.TypeID == ITEM_5_NOVELTY ){
		return false;
	}
	int maxDurability = 0;
	switch( player.OilEffectOnCursor ){
	case MC_31_OIL_OF_REPAIR_I:
		if( item.CurDurability < item.BaseDurability ){
			int repairValue = RNG(10) + 1;
			int newCurDur = item.CurDurability + repairValue;
			if( newCurDur > item.BaseDurability ){
				newCurDur = item.BaseDurability;
			}
			item.CurDurability = newCurDur;
			return true;
		}
		return false;
	case MC_32_OIL_OF_REPAIR_II:
		if( item.CurDurability < item.BaseDurability ){
			int repairValue = RNG(10) + 6;
			int newCurDur = item.CurDurability + repairValue;
			if( newCurDur > item.BaseDurability ){
				newCurDur = item.BaseDurability;
			}
			item.CurDurability = newCurDur;
			return true;
		}
		return false;
	case MC_33_OIL_OF_REPAIR_III:
		if( item.CurDurability < item.BaseDurability ){
			int repairValue = RNG(10) + 11;
			int newCurDur = item.CurDurability + repairValue;
			if( newCurDur > item.BaseDurability ){
				newCurDur = item.BaseDurability;
			}
			item.CurDurability = newCurDur;
			return true;
		}
		return false;
	case MC_34_OIL_OF_REPAIR_IV:
		if( item.CurDurability < item.BaseDurability ){
			int repairValue = RNG(10) + 16;
			int newCurDur = item.CurDurability + repairValue;
			if( newCurDur > item.BaseDurability ){
				newCurDur = item.BaseDurability;
			}
			item.CurDurability = newCurDur;
			return true;
		}
		return false;
	case MC_35_OIL_OF_REPAIR_V:
		if( item.CurDurability < item.BaseDurability ){
			int repairValue = RNG(10) + 21;
			int newCurDur = item.CurDurability + repairValue;
			if( newCurDur > item.BaseDurability ){
				newCurDur = item.BaseDurability;
			}
			item.CurDurability = newCurDur;
			return true;
		}
		return false;
	case MC_36_BLACKSMITH_OIL:
		maxDurability = MaxDurability(item) / 3; // 100 for regular items, and 200 for harness. its better balanced that way
		if( item.BaseDurability < maxDurability ){
			item.BaseDurability += RNG(3) + 1;// [1-3] randomly
			return true;
		}
		return false;
	case MC_37_OIL_OF_FORTITUDE:
		maxDurability = MaxDurability(item); // was 200
		if( item.BaseDurability < maxDurability ){
			int addedDurPoints = 1;// RNG(3) + 2;// it should slowdown later, not get faster
			item.CurDurability += addedDurPoints;
			LimitToMax( item.CurDurability, maxDurability );
			item.BaseDurability += addedDurPoints;
			LimitToMax( item.BaseDurability, maxDurability );
			return true;
		}
		return false;
	case MC_38_OIL_OF_REPAIR_VI:
		if( item.CurDurability < item.BaseDurability ){
			int repairValue = RNG(10) + 26;
			int newCurDur = item.CurDurability + repairValue;
			if( newCurDur > item.BaseDurability ){
				newCurDur = item.BaseDurability;
			}
			item.CurDurability = newCurDur;
			return true;
		}
		return false;
	case MC_39_OIL_OF_REPAIR_VII:
		if( item.CurDurability < item.BaseDurability ){
			int repairValue = RNG(15) + 31;
			int newCurDur = item.CurDurability + repairValue;
			if( newCurDur > item.BaseDurability ){
				newCurDur = item.BaseDurability;
			}
			item.CurDurability = newCurDur;
			return true;
		}
		return false;
	case MC_40_OIL_OF_HARDENING:
		maxDurability = MaxDurability(item) / 2; // 150 for regular items, and 300 for harness
		if( item.BaseDurability < maxDurability ){
			item.BaseDurability += RNG(2) + 1;// [1-2] randomly, suits well for mid tier dur booster;
			LimitToMax( item.BaseDurability, maxDurability );
			return true;
		}
		return false;
	default:
		return true;
	}
	return true; // по идее до сюда не должны дойти. Просто страховка
}

// TODO: remove affixEffect param, add playerIndex or use only Effect& e
//----- (0042889B) --------------------------------------------------------
bool __fastcall PrintItemEffectToStringBuffer( int affixEffect, Item* itemPtr, int effectIndex )
{
	Player& player = Players[CurrentPlayerIndex];
	Item& item = *itemPtr;
	InfoPanelBuffer[0] = 0;
	if( (uint) affixEffect < (uint) AE_COUNT && AffixEffect::Table[affixEffect].info ){
		AffixEffect::Table[affixEffect].info(item, item.effect[effectIndex], player);
		return true;
	}
	return false;
}

//----- (004298A3) --------------------------------------------------------
bool __fastcall UsePotionOrScroll(int playerIndex, Item* item) // UseItem
{
    bool used = true;
	switch( item->MagicCode ){
	case MC_2_POTION_OF_FULL_HEALING:
	    used = UseFullHealingPotion(playerIndex);
	    break;
	case MC_3_POTION_OF_HEALING:
        used = UseHealingPotion(playerIndex);
        break;
	case MC_6_POTION_OF_MANA:
	    used = UseManaPotion(playerIndex);
	    break;
	case MC_7_POTION_OF_FULL_MANA:
	    used = UseFullManaPotion(playerIndex);
	    break;
	case MC_10_ELIXIR_OF_STRENGTH:
	    used = ModifyPlayerStrength(playerIndex, 1);
	    break;
	case MC_11_ELIXIR_OF_MAGIC:
	    if( ModifyPlayerMagic(playerIndex, 1) ){
			if (HasTrait(playerIndex, TraitId::Rabid) || HasTrait(playerIndex, TraitId::WildSorcery)) {
			}
			else{
				UseFullManaPotion(playerIndex); 
			}
        }
		else{
            used = false;
        }
	    break;
	case MC_12_ELIXIR_OF_DEXTERITY:
	    used = ModifyPlayerDexterity(playerIndex, 1);
	    break;
	case MC_13_ELIXIR_OF_VITALITY:
	    if( ModifyPlayerVitality(playerIndex, 1) ){
			if (HasTrait(playerIndex, TraitId::Rabid) || HasTrait(playerIndex, TraitId::WildSorcery)) {
			}
			else{
				UseFullHealingPotion(playerIndex);
			}
        }
		else{
            used = false;
        }
	    break;
	case MC_18_POTION_OF_REJUVENATION:
		used = false;
		if (UseHealingPotion(playerIndex)) {
			used = true;
		}
		if (UseManaPotion(playerIndex)) {
			used = true;
		}
		break;
	case MC_19_POTION_OF_FULL_REJUVENATION:
		used = false;
		if (UseFullHealingPotion(playerIndex)) {
			used = true;
		}
		if (UseFullManaPotion(playerIndex)) {
			used = true;
		}
		break;
	case MC_21_RELIC_NEED_NO_TARGET:
	case MC_22_RELIC_NEED_TARGET:
		if( item->SpellIndex == PS_5_IDENTIFY ){
			UseIdentifyRelic(playerIndex, item->MagicCode, item->seed, item->baseItemIndex);
			used = false;
			break;
		}
		used = UseItemWithMagicCode_21_22_relic(playerIndex, item->MagicCode, item->SpellIndex, item->seed, item->baseItemIndex);
		if( item->SpellIndex == PS_26_ITEM_REPAIR ) used = false;
	    break;
	case MC_47_RUNE_OF_FIRE: // TODO: may be unify with UseItemWithMagicCode_21_22_relic up
	case MC_48_RUNE_OF_LIGHTNING:
	case MC_49_RUNE_OF_SHOCK:
	case MC_50_RUNE_OF_BLAZE:
	case MC_51_RUNE_OF_STONE:
	    UseItemWithMagicCode_47_51_rune(playerIndex, item->MagicCode, item->seed, item->baseItemIndex);
	    used = false;
	    break;
	case MC_24_BOOKS:
	    UseItemWithMagicCode_24_book(playerIndex, item->MagicCode, item->SpellIndex);
	    break;
	case MC_31_OIL_OF_REPAIR_I:
	case MC_32_OIL_OF_REPAIR_II:
	case MC_33_OIL_OF_REPAIR_III:
	case MC_34_OIL_OF_REPAIR_IV:
	case MC_35_OIL_OF_REPAIR_V:
	case MC_36_BLACKSMITH_OIL:
	case MC_37_OIL_OF_FORTITUDE:
	case MC_38_OIL_OF_REPAIR_VI:
	case MC_39_OIL_OF_REPAIR_VII:
	case MC_40_OIL_OF_HARDENING:
	    UseItemWithMagicCode_31_40_oil(playerIndex, item->MagicCode, item->seed, item->baseItemIndex);
	    used = false;
	    break;
	case MC_42_MAP_OF_THE_STARS:
	    used = false;
	    OpenPopupScreen();
	    break;
	case MC_44_SPECTRAL_ELIXIR:
		used = false;
	    if (ModifyPlayerDexterity(playerIndex, GameMode == GM_CLASSIC ? 3 : 1)) {
			used = true;
		}
		if (ModifyPlayerStrength(playerIndex, GameMode == GM_CLASSIC ? 3 : 1)) {
			used = true;
		}
		if( ModifyPlayerVitality(playerIndex, GameMode == GM_CLASSIC ? 3 : 1) ){
	        UseFullHealingPotion(playerIndex);
            used = true;
        }
        if( ModifyPlayerMagic(playerIndex, GameMode == GM_CLASSIC ? 3 : 1) ){
	        UseFullManaPotion(playerIndex);
	        used = true;
        }
	    break;
	case MC_54_RECONSTRUCTED_NOTE:
	    StartSpeech(SP_322);
		IsINVPanelVisible = false;
		used = false;
	    break;
	}
	
	return used;
}

//----- (0042A0B3) --------------------------------------------------------Now is chunk of 007069B5
bool IsValidBaseForGriswoldBasicalItemsOld(int baseItemIndex)
{
	BaseItem& item = BaseItems[baseItemIndex];
	int itemCode = item.ItemCode;
	if( item.IsUncommon ){
	    return false;
    }
	switch (itemCode ){
	case IC_0_OTHER:
	case IC_11_GOLD:
	case IC_14_EAR:/* Такого ни разу не было*/
	case IC_12_RING:
	case IC_13_AMULET:				return false;
	case IC_10_STAFF:
		if( item.SpellNumber ){
			return false;
		}
		return true;
	default:						return true;
	}
}

//----- (007069B5) --------------------------------------------------------
bool __fastcall IsValidBaseForGriswoldBasicalItems(int baseItemIndex)
{
	if( baseItemIndex == BI_35_BLACKSMITH_OIL ){
		return true;
	}
	return IsValidBaseForGriswoldBasicalItemsOld(baseItemIndex);
}

volatile int getBaseItemIndexCount = 0;
//----- (0042A0F2) --------------------------------------------------------
int __fastcall GetRandomBaseForGriswoldBasicalItems(int maximumQualityLevel)
{
	vla( int, goodItems, GOOD_ITEM_COUNT );
	++getBaseItemIndexCount;
	int goodItemsCount = 0;
	int mode = (GameMode == GM_CLASSIC) + 1;
	for( uint baseItemIndex = 1; baseItemIndex < count_BaseItems; ++baseItemIndex ){
		BaseItem& baseItem = BaseItems[baseItemIndex];
		if( !(baseItem.mode + 1 & mode) ) continue;
		if( baseItem.ChanceToDrop 
		 && IsValidBaseForGriswoldBasicalItems(baseItemIndex) 
		 && IsBaseValidForSaleToPlayer(baseItemIndex, PD_2_BasicalItems)
		 && maximumQualityLevel >= baseItem.itemLevel 
		 && goodItemsCount < GOOD_ITEM_COUNT ){
			goodItems[goodItemsCount++] = baseItemIndex;
			if( baseItem.ChanceToDrop == 2 ){
				goodItems[goodItemsCount++] = baseItemIndex;
			}
		}
	}
	return (goodItemsCount ? goodItems[RNG(goodItemsCount)] : 0) + 1;
}

//----- (0042A17A) --------------------------------------------------------
void __fastcall SwapItems(Item* firstItem, Item* secondItem)
{
    Item tempItem;
    memcpy(&tempItem, firstItem, sizeof(Item)); 
    memcpy(firstItem, secondItem, sizeof(Item));
    memcpy(secondItem, &tempItem, sizeof(Item));

	/*Item tempItem = *firstItem;
	*firstItem = *secondItem;
	*secondItem = tempItem;*/
}

// New sorting algoritm
//----- (0042A1AE) --------------------------------------------------------
void BasicalItemsSorting()
{
	int itemsCount = 0;
	for( itemsCount = 0; itemsCount < 25; itemsCount++ ){
		Item& item = BasicalItemsArray[itemsCount];
		if( item.ItemCode == IC_M1_NONE ){
			break;
		}
	}
	LimitToMax(itemsCount, 25);
	// unstable selection sort http://ru.wikipedia.org/wiki/Сортировка_выбором
	for( int firstItemIndex = 0; firstItemIndex < itemsCount - 1; ++firstItemIndex ){
		Item& firstItem = BasicalItemsArray[firstItemIndex];
		int minItemIndex = firstItemIndex;// это номер вещи с наименьшим индексом
		for( int secondItemIndex = firstItemIndex + 1; secondItemIndex < itemsCount; ++secondItemIndex ){
			Item& secondItem = BasicalItemsArray[secondItemIndex];
			Item& minItem = BasicalItemsArray[minItemIndex];
			 // в th2 предлагалось сортировать по ItemCode
			if( secondItem.baseItemIndex < minItem.baseItemIndex ){
				minItemIndex = secondItemIndex;
			}
		}
		Item& minItem = BasicalItemsArray[minItemIndex];
		if( firstItem.baseItemIndex != minItem.baseItemIndex ){
			SwapItems(&firstItem, &minItem);
		}
	}
}

//----- (0042A211) --------------------------------------------------------
void __fastcall BasicalItemsGeneration(int qlvl)
{
	Item savedTempItem;
	Item& tempItem = Items[MaxItems_255];

	memcpy(&savedTempItem, &tempItem, sizeof(Item));
	int itemsCount = RNG(15) + 10;
	int currentItemIndex = 0;
	while( currentItemIndex < itemsCount ){
		Item& item = BasicalItemsArray[currentItemIndex];
		tempItem.MakeSeed();
		SetRndSeed(tempItem.seed);
		GetItemAttrs(MaxItems_255, GetRandomBaseForGriswoldBasicalItems(qlvl) - 1, qlvl);
		if( tempItem.Price <= 200000 ){
			memcpy(&item, &tempItem, sizeof(Item));
			item.dropType = D_GRISWOLD_BASIC;
			item.quality = qlvl;
			item.Identified = true;
			item.IsReqMet = CheckItemReq(CurrentPlayerIndex, &item);
			currentItemIndex++;
		}
	} 

	for( currentItemIndex = itemsCount; currentItemIndex < PT_BasicItemsCount; currentItemIndex++ ){
		Item& item = BasicalItemsArray[currentItemIndex];
		item.ItemCode = IC_M1_NONE;
	}

	BasicalItemsSorting();
	memcpy(&tempItem, &savedTempItem, sizeof(Item));
	
	VisualTrade_RefreshPage(VTP_BlacksmithBasic);
}

//----- (0042A2F5) --------------------------------------------------------
int __fastcall IsValidBaseForGriswoldMagicalItems( int baseItemIndex )
{
	BaseItem& baseItem = BaseItems[baseItemIndex];
	char itemCode = baseItem.ItemCode;
	if( itemCode == IC_0_OTHER || itemCode == IC_11_GOLD || itemCode == 14 ){
		return 0;
	}
	return 1;
}

//----- (0042A330) --------------------------------------------------------
int __fastcall GetRandomBaseItemIndexForGriswoldMagicalItem( int minQualityLevel, int maxQualityLevel )
{
	vla( int, goodItems, GOOD_ITEM_COUNT );
	int goodItemsCount = 0;
	int mode = (GameMode == GM_CLASSIC) + 1;
	for( uint baseItemIndex = 1; baseItemIndex < count_BaseItems; ++baseItemIndex ){
		BaseItem& baseItem = BaseItems[baseItemIndex];
		if( !(baseItem.mode + 1 & mode) ) continue;
		if( baseItem.ChanceToDrop 
		 && IsValidBaseForGriswoldMagicalItems(baseItemIndex) 
		 && IsBaseValidForSaleToPlayer( baseItemIndex, PD_18_MagicalItems )
		 && (baseItem.itemLevel >= minQualityLevel && baseItem.itemLevel <= maxQualityLevel)
		 && goodItemsCount < GOOD_ITEM_COUNT ){
			goodItems[goodItemsCount++] = baseItemIndex;
		}
	}
	return (goodItemsCount ? goodItems[RNG(goodItemsCount)] : 0)+ 1;
}

//----- (0042A3AD) --------------------------------------------------------
void __fastcall MagicItemsGeneration( int playerIndex )
{
	Player& player = Players[playerIndex];

	if( GriswoldMagicItemCount < PT_MagicItemsCount ){
		for( int i = 0; i < PT_MagicItemsCount; ++i ){
			Item& item = MagicItemsStoreArray[i];
			if( item.ItemCode == IC_M1_NONE ){
				GenerateNewMagicItemForSale(i, GriswoldQlvlRandomizationArray[i] + GriswoldItemsLevelParam, playerIndex, 0);
			}
		}
		GriswoldMagicItemCount = PT_MagicItemsCount;
	}
	if( GriswoldItemsLevelParam < player.CharLevel ){
		int tryesCount = 250;
		do{
			MagicItemsStoreArray[0] = MagicItemsStoreArray[3];
			MagicItemsStoreArray[1] = MagicItemsStoreArray[4];
			MagicItemsStoreArray[2] = MagicItemsStoreArray[5];
			MagicItemsStoreArray[3] = MagicItemsStoreArray[6];
			MagicItemsStoreArray[4] = MagicItemsStoreArray[7];
			MagicItemsStoreArray[5] = MagicItemsStoreArray[8];
			MagicItemsStoreArray[6] = MagicItemsStoreArray[9];
			MagicItemsStoreArray[7] = MagicItemsStoreArray[10];
			MagicItemsStoreArray[8] = MagicItemsStoreArray[11];
			MagicItemsStoreArray[9] = MagicItemsStoreArray[12];
			++GriswoldItemsLevelParam;
			GenerateNewMagicItemForSale( 10, GriswoldQlvlRandomizationArray[ 10 ] + GriswoldItemsLevelParam, playerIndex, 0 );
			MagicItemsStoreArray[11] = MagicItemsStoreArray[13];
			GenerateNewMagicItemForSale( 12, GriswoldQlvlRandomizationArray[ 12 ] + GriswoldItemsLevelParam, playerIndex, 0 );
			MagicItemsStoreArray[13] = MagicItemsStoreArray[14];
			GenerateNewMagicItemForSale( 14, GriswoldQlvlRandomizationArray[ 14 ] + GriswoldItemsLevelParam, playerIndex, 0 );
		}while( GriswoldItemsLevelParam < player.CharLevel && tryesCount--);
	}
	VisualTrade_RefreshPage(VTP_BlacksmithPremium);
}

//----- (0042A53C) --------------------------------------------------------
void __fastcall GenerateNewMagicItemForSale( int itemIndex, int qlvl, int playerIndex, char someFlag )
{
	Item& item = Items[MaxItems_255];
	Player& player = Players[playerIndex];
	FullPlayerClass& fullPlayerClass = FullPlayerClasses[player.fullClassId];
	Item tempItem;
	memcpy(&tempItem, &item, sizeof(Item));

	int maxStr = fullPlayerClass.MaxStat.Strength;
	int maxDex = fullPlayerClass.MaxStat.Dexterity;
	int maxMag = fullPlayerClass.MaxStat.Magic;
	int maxVit = fullPlayerClass.MaxStat.Vitality;

	if( maxStr < player.CurStrength ){
		maxStr = player.CurStrength;
	}
	maxStr = ftol(maxStr * 1.2);
	if( maxDex < player.CurDexterity ){
		maxDex = player.CurDexterity;
	}
	maxDex = ftol(maxDex * 1.2);
	if( maxMag < player.CurMagic ){
		maxMag = player.CurMagic;
	}
	maxMag = ftol(maxMag * 1.2);
	if (maxVit < player.CurVitality) {
		maxVit = player.CurVitality;
	}
	maxVit = ftol(maxVit * 1.2);
	//int baseQlvl = qlvl; // level gets preserved to be used for base item level later <<- this was supposed to work but produces crashes
	//LimitToRange(qlvl, 1, 40);
	int challenge_Mode_Bonus = 6 /*default*/;
	if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_NIGHTMARE)){ 
		challenge_Mode_Bonus = 7; 
	}
	if (player.gameChanger & BIT(GC_14_CLEAN_HANDS ) || GameMode == GM_COLISEUM ) {
		challenge_Mode_Bonus = 10;// in this mode player can't farm, can only buy items, so they need no penalty to quality of goods from vendors
	}
	LimitToRange(qlvl, 1, 10 + (challenge_Mode_Bonus * player.CharLevel / 10) );// this is Griswold shop affix level limitation

	int tryesCount = 0;
	int extras = PerkValue(SYNERGY_TRADER, playerIndex) + PerkValue(PERK_SHREWD_BUYER, playerIndex);// trader: max +30, shrewdbuyer max +25
	extras += player.griswoldLevel; // +25 max (never really clashes with Shrewd Buyer)
	do{
		item.MakeSeed();
		SetRndSeed(item.seed);
		int baseItemIndex = GetRandomBaseItemIndexForGriswoldMagicalItem(qlvl / 4, qlvl) - 1; // this is Griswold shop base item level limitation
		GetItemAttrs(MaxItems_255, baseItemIndex, qlvl);
		item.Identified = true;
		ApplyAffixesToItem(MaxItems_255, baseItemIndex, qlvl / 2, qlvl + extras, 1, someFlag);
		int maxPrice = 0;
		switch( item.ItemCode ){
		case IC_6_LIGHT_ARMOR:
		case IC_8_MID_ARMOR:
		case IC_9_HEAVY_ARMOR:	maxPrice = GetMaxPriceofPlayerArmors	(playerIndex);	break;
		case IC_5_SHIELD:		maxPrice = GetMaxPriceofPlayerShields	(playerIndex);	break;
		case IC_2_AXE:			maxPrice = GetMaxPriceofPlayerAxes		(playerIndex);	break;
		case IC_3_BOW:			maxPrice = GetMaxPriceofPlayerBows		(playerIndex);	break;
		case IC_4_MACE:			maxPrice = GetMaxPriceofPlayerMaces		(playerIndex);	break;
		case IC_1_SWORD:		maxPrice = GetMaxPriceofPlayerSwords	(playerIndex);	break; // <----------- here
		case IC_7_HELM:			maxPrice = GetMaxPriceofPlayerHelms		(playerIndex);	break;
		case IC_10_STAFF:		maxPrice = GetMaxPriceofPlayerStaffs	(playerIndex);	break;
		case IC_12_RING:		maxPrice = MaxItemPrice                 (player, IC_12_RING);	break;
		case IC_13_AMULET:		maxPrice = GetMaxPriceofPlayerAmulets	(playerIndex);	break;
		default:																		break;
		}
		signed __int64 notUsed = ftol(maxPrice * 0.8);
		++tryesCount;
	}while( (item.Price > 800000
	 || item.RequiredStrength > maxStr
	 || item.RequiredMagic > maxMag
	 || item.RequiredDexterity > maxDex
	 || item.RequiredVitality > maxVit)
	 && tryesCount < 150 );

	Item& shopItem = MagicItemsStoreArray[itemIndex];
	memcpy(&shopItem, &item, sizeof(Item));
	shopItem.dropType = D_GRISWOLD_MAGIC;
	shopItem.quality = qlvl;
	shopItem.magicFindSeed = extras;
	shopItem.IsReqMet = CheckItemReq(CurrentPlayerIndex, &shopItem);
	memcpy(&item, &tempItem, sizeof(Item));
}

//----- (0042A7BF) --------------------------------------------------------
int __fastcall IsValidBaseForAdriaItems(int baseItemIndex)
{
	BaseItem& item = BaseItems[baseItemIndex];
	int itemCode = item.ItemCode;
	int magicCode = item.MagicCode;
	int spellNumber = item.SpellNumber;
	if (itemCode == IC_0_OTHER || itemCode == IC_10_STAFF) { // conditions for OTHER items and STAVES (NOT books)
		if (magicCode == MC_6_POTION_OF_MANA
			) { // зелья маны ибо уже есть
			return false;
		}
		else if (GameMode == GM_COLISEUM &&
			(	   magicCode == MC_7_POTION_OF_FULL_MANA
				|| magicCode == MC_18_POTION_OF_REJUVENATION
				|| magicCode == MC_19_POTION_OF_FULL_REJUVENATION)) {
			return false;
		}
		else if (spellNumber == PS_7_TOWN_PORTAL) { // тоже есть
			return false;
		}
		else if (magicCode == MC_2_POTION_OF_FULL_HEALING || magicCode == MC_3_POTION_OF_HEALING) {// зелья лечения ибо не хилер
			return false;
		}
		else if (magicCode >= MC_30_OIL_OF_SOMETHING && magicCode <= MC_40_OIL_OF_HARDENING) { // масла
			return false;
		}
		else if ((spellNumber == PS_32_BONE_SPIRIT_REAL_RESSURECT || spellNumber == PS_34_HEAL_OTHER) && MaxCountOfPlayersInGame == 1) {// заклинания бессмысленные в сингле
			return false;
		}
		else if (isTownPortalForbidden() && (spellNumber == PS_7_TOWN_PORTAL)) {
			return false;
		}
		else if 
			(GameMode == GM_COLISEUM &&
				(
				is(spellNumber,
				PS_2_HEALING, PS_7_TOWN_PORTAL, PS_13_HYDRA, PS_15_FORCE_WAVE, PS_21_GOLEM, PS_42_WARP, PS_9_INFRAVISION, PS_5_IDENTIFY,
				PS_31_HOLY_BOLT, PS_33_TELEKINES, PS_39_HOLY_NOVA, PS_8_STONE_CURSE, PS_16_REFLECT, PS_24_APOCALYPSE, PS_28_TRAP_DISARM, PS_37_MANA_RECHARGE, PS_38_MAGI)
				|| is(item.baseId, BI_300_MORGANITE, BI_299_HELIODOR, BI_298_TOURMALINE, BI_297_CUPRITE, BI_296_QUARTZ, BI_301_AQUAMARINE)
				)
			) {
			return false;
		}
		else if
			(GameMode == GM_CLASSIC &&
				(
					is(spellNumber,
						PS_32_BONE_SPIRIT_REAL_RESSURECT, PS_15_FORCE_WAVE, PS_21_GOLEM, PS_42_WARP, PS_9_INFRAVISION, PS_5_IDENTIFY, PS_17_LIGHTING_RING, PS_18_LIGHTNING_NOVA,
						PS_33_TELEKINES, PS_39_HOLY_NOVA, PS_16_REFLECT, PS_28_TRAP_DISARM, PS_37_MANA_RECHARGE, PS_38_MAGI, PS_34_HEAL_OTHER, PS_14_BALL_LIGHTNING, PS_19_FLAME_RING,
						PS_22_FURY, PS_41_FIERY_NOVA, PS_43_ARCANE_NOVA)
					/*|| is(item.baseId, BI_300_MORGANITE, BI_299_HELIODOR, BI_298_TOURMALINE, BI_297_CUPRITE, BI_296_QUARTZ, BI_301_AQUAMARINE)*/
					)
				) {
			return false;
		}
		return true;
	}else{
		return false;
	}
}

//----- (0042A82A) --------------------------------------------------------
int __fastcall GetRandomBaseForAdriaItems(int maxQlvl)
{
	vla( int, goodItems, GOOD_ITEM_COUNT );
	int goodItemsCount = 0;

	int mode = (GameMode == GM_CLASSIC) + 1;
	for( uint baseItemIndex = 1; baseItemIndex < count_BaseItems; baseItemIndex++ ){
		BaseItem& baseItem = BaseItems[baseItemIndex];
		if( !(baseItem.mode + 1 & mode) ) continue;
		if( baseItem.ChanceToDrop 
		 && IsValidBaseForAdriaItems(baseItemIndex)
		 && IsBaseValidForSaleToPlayer(baseItemIndex, PD_6_WitchBuy)
		 && maxQlvl >= baseItem.itemLevel 
		 && goodItemsCount < GOOD_ITEM_COUNT ){
			goodItems[ goodItemsCount++ ] = baseItemIndex;
		}
	}
	return (goodItemsCount ? goodItems[ RNG(goodItemsCount) ] : 0) + 1;
}

// New sorting algoritm
//----- (0042A89F) --------------------------------------------------------
void AdriaItemsSorting()
{
	int itemsCount = 0;
	const int FORCED_ADRIA_ITEMS_COUNT = 3;
	for( itemsCount = FORCED_ADRIA_ITEMS_COUNT; itemsCount < 25; itemsCount++ ){
		Item& item = WitchAdriaItems[itemsCount];
		if( item.ItemCode == IC_M1_NONE ){
			break;
		}
	}
	LimitToMax(itemsCount, 25);
	// unstable selection sort  http://ru.wikipedia.org/wiki/Сортировка_выбором
	for( int firstItemIndex = FORCED_ADRIA_ITEMS_COUNT; firstItemIndex < itemsCount - 1; ++firstItemIndex ){
		Item& firstItem = WitchAdriaItems[firstItemIndex];
		int minItemIndex = firstItemIndex;// это номер вещи с наименьшим индексом
		for( int secondItemIndex = firstItemIndex + 1; secondItemIndex < itemsCount; ++secondItemIndex ){
			Item& secondItem = WitchAdriaItems[secondItemIndex];
			Item& minItem = WitchAdriaItems[minItemIndex];
			if( secondItem.MagicCode < minItem.MagicCode ){
				minItemIndex = secondItemIndex;
			}
		}
		Item& minItem = WitchAdriaItems[minItemIndex];
		if( firstItem.MagicCode != minItem.MagicCode ){
			SwapItems(&firstItem, &minItem);
		}
	}
}

//----- (0042A906) --------------------------------------------------------
void __fastcall CalculateRequiredMagicForBooks(int currentAdriaItemIndex)
{
	Item& adriaItem = WitchAdriaItems[currentAdriaItemIndex];
	Player& player = Players[CurrentPlayerIndex];
	if( adriaItem.MagicCode != MC_24_BOOKS ){
		return;
	}
	adriaItem.RequiredLevel = GetBookLevelReq( adriaItem.SpellIndex, CurrentPlayerIndex );
	adriaItem.RequiredMagic = Spells[adriaItem.SpellIndex].RequiredMagic;
	int playerSpellLevel = player.SpellLevels[adriaItem.SpellIndex];
	unsigned int adriaReqMag = adriaItem.RequiredMagic;
	for( int i = 0; i < playerSpellLevel; ++i ){
		adriaReqMag += 20;
	}
	adriaItem.RequiredMagic = adriaReqMag;
}

//----- (0042A99E) --------------------------------------------------------
void __fastcall WitchAdriaItemsGeneration(int qlvl)
{
	Player& player = Players[CurrentPlayerIndex];

	int itemsCount = RNG(15) + 10;
	int booksCount = RNG(4);

	Item& tempItem = Items[MaxItems_255];

	if (GameMode != GM_COLISEUM) {
		Item& adriaItem0 = WitchAdriaItems[0];
		Item& adriaItem1 = WitchAdriaItems[1];
		Item& adriaItem2 = WitchAdriaItems[2];

		if (GameMode != GM_CLASSIC){
			GetItemAttrs(MaxItems_255, BI_215_RELICT_OF_MANA, 1);
			adriaItem0 = tempItem;
			adriaItem0.MakeSeed();
			adriaItem0.dropType = D_0_SIMPLE;
			adriaItem0.quality = qlvl;
			adriaItem0.IsReqMet = true;
		}
		GetItemAttrs(MaxItems_255, GameMode == GM_CLASSIC ? BI_2653_POTION_OF_MANA_CLS : BI_25_POTION_OF_MANA, 1);
		adriaItem1 = tempItem;
		adriaItem1.MakeSeed();
		adriaItem1.dropType = D_0_SIMPLE;
		adriaItem1.quality = qlvl;
		adriaItem1.IsReqMet = true;

		int adriaItem2Base = GameMode == GM_CLASSIC ? BI_2660_RELICT_OF_TP_CLS : BI_27_RELICT_OF_TOWN_PORTAL;
		if (isTownPortalForbidden()) {
			adriaItem2Base = BI_187_RELICT_OF_IDENTIFY;
		}
		GetItemAttrs(MaxItems_255, adriaItem2Base, BI_1_SHORT_KNIFE ); // what is this "1"? (same in th3)
		adriaItem2 = tempItem;
		adriaItem2.MakeSeed();
		adriaItem2.dropType = D_0_SIMPLE;
		adriaItem2.quality = qlvl;
		adriaItem2.IsReqMet = true;
	}
	int bookStart = BI_114_BOOK_OF_, bookEnd = BI_117_BOOK_OF_;
	if (GameMode == GM_CLASSIC) { bookStart = BI_2644_BOOK_OF_START_CLS; bookEnd = BI_2647_BOOK_OF_END_CLS; }
	int currentAdriaItemIndex = 3;
	for( int baseItemIndex = bookStart; baseItemIndex <= bookEnd && currentAdriaItemIndex - 3 < booksCount; ++baseItemIndex ){
		BaseItem& baseItem = BaseItems[baseItemIndex];
		// в th1 тут ошибка, неправильная ссылка на baseItem.QualityLevel
		// и еще одна где-то рядом, исправить
		if( IsValidBaseForAdriaItems(baseItemIndex) && baseItem.itemLevel <= qlvl ){
			Item& adriaItem = WitchAdriaItems[currentAdriaItemIndex];
			tempItem.MakeSeed();
			SetRndSeed(tempItem.seed);
			forceCallforSync = RNG(1); // синхронизация с GetRandomBaseForAdriaItems ниже
			GetItemAttrs(MaxItems_255, baseItemIndex, qlvl);
			if (IsItemFiltered(player, tempItem)) {//No summon books in store for non summoners (guaranteed books)
				baseItemIndex--;
			}
			else {
				memcpy(&adriaItem, &tempItem, sizeof(Item));
				adriaItem.dropType = D_ADRIA;
				adriaItem.quality = qlvl;
				adriaItem.Identified = true;
				CalculateRequiredMagicForBooks(currentAdriaItemIndex);
				adriaItem.IsReqMet = CheckItemReq(CurrentPlayerIndex, &adriaItem);
				currentAdriaItemIndex++;
			}
		}
	}
	while( currentAdriaItemIndex < itemsCount ){
		Item& adriaItem = WitchAdriaItems[currentAdriaItemIndex];
		tempItem.MakeSeed();
		SetRndSeed(tempItem.seed);
		int baseItemIndex = GetRandomBaseForAdriaItems(qlvl) - 1;
		GetItemAttrs(MaxItems_255, baseItemIndex, qlvl);
		tempItem.Identified = true;
		if (IsItemFiltered(player, tempItem)) //No summon books in store for non summoners (random items)
			continue;
		int qlvl_1 = -1;
		if( RNG(100) <= 5 ){
			qlvl_1 = 2 * qlvl;
		}
		if( qlvl_1 == -1 ){
			if( tempItem.MagicCode == MC_23_STAFF ){
				qlvl_1 = 2 * qlvl;
			}
			if( qlvl_1 == -1 ){
				goto noApply;
			}
		}
		ApplyAffixesToItem(MaxItems_255, baseItemIndex, qlvl_1 >> 1, qlvl_1, 1, 1);
		noApply:
		if( tempItem.Price <= 200000 ){
			adriaItem = tempItem;
			adriaItem.dropType = D_ADRIA;
			adriaItem.quality = qlvl;
			CalculateRequiredMagicForBooks(currentAdriaItemIndex);
			adriaItem.IsReqMet = CheckItemReq(CurrentPlayerIndex, &adriaItem);
			++currentAdriaItemIndex;
		}
	}
	for( currentAdriaItemIndex = itemsCount; currentAdriaItemIndex < PT_25_WitchItemsCount; ++currentAdriaItemIndex ){
		Item& adriaItem = WitchAdriaItems[currentAdriaItemIndex];
		adriaItem.ItemCode = IC_M1_NONE;
	}
	AdriaItemsSorting();
    VisualTrade_RefreshPage(VTP_WitchBasic);
}

//----- (0042AC1F) --------------------------------------------------------
int __fastcall GetRandomBaseItemIndexForWirtItem( int qlvl )
{
	vla( uint, equipableItems, GOOD_ITEM_COUNT );
	int goodItemsCount = 0;
	int mode = (GameMode == GM_CLASSIC) + 1;
	for( uint baseItemIndex = 1; baseItemIndex < count_BaseItems; ++baseItemIndex ){
		BaseItem& baseItem = BaseItems[baseItemIndex];
		if( !(baseItem.mode + 1 & mode) ) continue;
		if( baseItem.ChanceToDrop 
		 && IsValidBaseForGriswoldMagicalItems(baseItemIndex)
		 && IsBaseValidForSaleToPlayer( baseItemIndex, PD_13_WirtBuy )
		 && qlvl >= baseItem.itemLevel 
		 && goodItemsCount < GOOD_ITEM_COUNT ){
			equipableItems[goodItemsCount++] = baseItemIndex;
		}
	}
	return (goodItemsCount ? equipableItems[RNG(goodItemsCount)] : 0) + 1;
}

//----- (0042AC94) --------------------------------------------------------
void __fastcall WirtItemGeneration( int qlvl )
{
	Item& item = Items[MaxItems_255];
	Player& player = Players[CurrentPlayerIndex];
	FullPlayerClass& fullPlayerClass = FullPlayerClasses[player.fullClassId];

	int maxStr = fullPlayerClass.MaxStat.Strength;
	int maxDex = fullPlayerClass.MaxStat.Dexterity;
	int maxMag = fullPlayerClass.MaxStat.Magic;
	int maxVit = fullPlayerClass.MaxStat.Vitality;

	if( maxStr < player.CurStrength){
		maxStr = player.CurStrength;
	}
	maxStr = ftol((double)maxStr * 1.2);
	if( maxDex < player.CurDexterity ){
		maxDex = player.CurDexterity;
	}
	maxDex = ftol((double)maxDex * 1.2);
	if( maxMag < player.CurMagic ){
		maxMag = player.CurMagic;
	}
	maxMag = ftol((double)maxMag * 1.2);
	if( maxVit < player.CurVitality ){
		maxVit = player.CurVitality;
	}
	maxVit = ftol((double)maxVit * 1.2);
	// don't forget to adjust same stuff in "RestoreWirtItem" function
	int challenge_Mode_Bonus2 = 7 /*default*/;
	if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_NIGHTMARE)) {
		challenge_Mode_Bonus2 = 8;
	}
	if (player.gameChanger & BIT(GC_14_CLEAN_HANDS) || GameMode == GM_COLISEUM ) {
		challenge_Mode_Bonus2 = 10;// in this mode player can't farm, can only buy items, so they need no penalty to quality of goods from vendors
	}
	LimitToRange(qlvl, 1, 15 + (challenge_Mode_Bonus2 * player.CharLevel / 10) ); // Wirt item level

	int extras_wirt = PerkValue(SYNERGY_TRADER, CurrentPlayerIndex) + PerkValue(PERK_SHREWD_BUYER, CurrentPlayerIndex);// trader: max +30, shrewdbuyer max +25
	if( WirtItemHalfQlvl < qlvl >> 1 || WirtItem.ItemCode == IC_M1_NONE ){
		int tryesCount = 0;
		do{
			// TODO: change this block to RestoreWirtItem/InitWirtItem()
			item.MakeSeed();
			SetRndSeed(item.seed);
			int baseItemIndex = GetRandomBaseItemIndexForWirtItem(qlvl) - 1;
			GetItemAttrs(MaxItems_255, baseItemIndex, qlvl);
			item.Identified = true;
			item.magicFindSeed = extras_wirt;
			ApplyAffixesToItem(MaxItems_255, baseItemIndex, 2 * qlvl / 3, qlvl + extras_wirt, 1, 1); // 5th parameter here could be used to sell cursed items to Rogue if she develops some Black Market syn..

			int maxPrice = 0;
			switch( item.ItemCode ){
			case IC_6_LIGHT_ARMOR:
			case IC_8_MID_ARMOR:
			case IC_9_HEAVY_ARMOR:		maxPrice = GetMaxPriceofPlayerArmors(CurrentPlayerIndex);		break;
			case IC_5_SHIELD:			maxPrice = GetMaxPriceofPlayerShields(CurrentPlayerIndex);		break;
			case IC_2_AXE:				maxPrice = GetMaxPriceofPlayerAxes(CurrentPlayerIndex);			break;
			case IC_3_BOW:				maxPrice = GetMaxPriceofPlayerBows(CurrentPlayerIndex);			break;
			case IC_4_MACE:				maxPrice = GetMaxPriceofPlayerMaces(CurrentPlayerIndex);		break;
			case IC_1_SWORD:			maxPrice = GetMaxPriceofPlayerSwords(CurrentPlayerIndex);		break;
			case IC_7_HELM:				maxPrice = GetMaxPriceofPlayerHelms(CurrentPlayerIndex);		break;
			case IC_10_STAFF:			maxPrice = GetMaxPriceofPlayerStaffs(CurrentPlayerIndex);		break;
			case IC_12_RING:			maxPrice = MaxItemPrice(player, IC_12_RING);	                break;
			case IC_13_AMULET:			maxPrice = GetMaxPriceofPlayerAmulets(CurrentPlayerIndex);		break;
			default:																					break;
			}
			volatile signed __int64 notUsed = ftol(maxPrice * 0.8);
			++tryesCount;
			if( item.Price <= 900000 
				&& item.RequiredStrength <= maxStr
				&& item.RequiredMagic <= maxMag 
				&& item.RequiredDexterity <= maxDex
				&& item.RequiredVitality <= maxVit){
					break;
			}
		}while( tryesCount < 250 );
		memcpy(&WirtItem, &item, sizeof(Item));
		WirtItem.dropType = D_WIRT;
		WirtItem.quality = qlvl;
		WirtItem.IsReqMet = CheckItemReq(CurrentPlayerIndex, &WirtItem);
		WirtItemHalfQlvl = qlvl >> 1;
	}
	
	VisualTrade_RefreshPage(VTP_GambleBasic);
}

//----- (0042AFEF) --------------------------------------------------------
bool __fastcall IsValidBaseForPepinItems( int baseItemIndex )
{
	Player& player = Players[CurrentPlayerIndex];
	PlayerStat& maxStat = FullPlayerClasses[player.fullClassId].MaxStat;
	BaseItem& baseItem = BaseItems[baseItemIndex];
	int magicCode = baseItem.MagicCode;
	if( baseItem.ItemCode != IC_0_OTHER ){
		return false;
	}
	if(GameMode != GM_COLISEUM && magicCode == MC_21_RELIC_NEED_NO_TARGET && baseItem.SpellNumber == PS_2_HEALING ){
		return true;
	}
	if( magicCode == MC_22_RELIC_NEED_TARGET && baseItem.SpellNumber == PS_32_BONE_SPIRIT_REAL_RESSURECT && MaxCountOfPlayersInGame != 1 ){
		return false;
	}
	if( magicCode == MC_22_RELIC_NEED_TARGET && baseItem.SpellNumber == PS_34_HEAL_OTHER && MaxCountOfPlayersInGame != 1 )	{
		return true;
	}
    if( magicCode == MC_22_RELIC_NEED_TARGET && baseItem.SpellNumber == PS_7_TOWN_PORTAL && isTownPortalForbidden() ){
        return false;
    }
	if( MaxCountOfPlayersInGame == 1 || GameMode == GM_COLISEUM || GameMode == GM_CLASSIC){
		if( magicCode == MC_10_ELIXIR_OF_STRENGTH && player.BaseStrength < maxStat.Strength 
			|| magicCode == MC_11_ELIXIR_OF_MAGIC && player.BaseMagic < maxStat.Magic 
			|| magicCode == MC_12_ELIXIR_OF_DEXTERITY && player.BaseDexterity < maxStat.Dexterity 
			|| magicCode == MC_13_ELIXIR_OF_VITALITY && player.BaseVitality < maxStat.Vitality ){
				return true;
		}
	}
	if( /*GameMode != GM_COLISEUM &&*/
		(magicCode == MC_2_POTION_OF_FULL_HEALING 
		|| magicCode == MC_18_POTION_OF_REJUVENATION 
		|| magicCode == MC_19_POTION_OF_FULL_REJUVENATION ))	{
			return true;
	}
	if(magicCode == MC_3_POTION_OF_HEALING 
		|| magicCode == MC_6_POTION_OF_MANA 
		|| magicCode == MC_7_POTION_OF_FULL_MANA ){
			return false;
	}
	return false;
}

//----- (0042B124) --------------------------------------------------------
int __fastcall GetRandomBaseForPepinItems( int maximumQualityLevel )
{
	vla( int, goodItems, GOOD_ITEM_COUNT );
	int goodItemsCount = 0;

	int mode = (GameMode == GM_CLASSIC) + 1;
	for( uint baseItemIndex = 1; baseItemIndex < count_BaseItems && goodItemsCount < GOOD_ITEM_COUNT; baseItemIndex++ ){
		BaseItem& baseItem = BaseItems[baseItemIndex];
		if( !(baseItem.mode + 1 & mode) ) continue;
		if( baseItem.ChanceToDrop 
		 && IsValidBaseForPepinItems(baseItemIndex)
		 && IsBaseValidForSaleToPlayer(baseItemIndex, PD_16_HealerBuy)
		 && maximumQualityLevel >= baseItem.itemLevel 
		 && goodItemsCount < 512 ){
			goodItems[goodItemsCount++] = baseItemIndex;
		}
	}
	return (goodItemsCount ? goodItems[RNG(goodItemsCount)] : 0) + 1;
}

// New sorting algoritm
//----- (0042B199) --------------------------------------------------------
void PepinItemsSorting()
{
	int itemsCount = 0;
	const int FORCED_PEPIN_ITEMS_COUNT = 2;
	for( itemsCount = FORCED_PEPIN_ITEMS_COUNT; itemsCount < 20; itemsCount++ ){
		Item& item = PepinItems[itemsCount];
		if( item.ItemCode == IC_M1_NONE ){
			break;
		}
	}
	LimitToMax(itemsCount, 20);
	// unstable selection sort  http://ru.wikipedia.org/wiki/Сортировка_выбором
	for( int firstItemIndex = FORCED_PEPIN_ITEMS_COUNT; firstItemIndex < itemsCount - 1; firstItemIndex++ ){// Сортировка с выбором
		Item& firstItem = PepinItems[firstItemIndex];
		int minItemIndex = firstItemIndex;// это номер вещи с наименьшим индексом
		for( int secondItemIndex = firstItemIndex + 1; secondItemIndex < itemsCount; secondItemIndex++ ){
			Item& secondItem = PepinItems[secondItemIndex];
			Item& minItem = PepinItems[minItemIndex];
			if( secondItem.MagicCode < minItem.MagicCode ){
				minItemIndex = secondItemIndex;
			}
		}
		Item& minItem = PepinItems[minItemIndex];
		if( firstItem.MagicCode != minItem.MagicCode ){
			SwapItems(&firstItem, &minItem);
		}
	}
}

//----- (0042B200) --------------------------------------------------------
void __fastcall PepinItemsGeneration( int qlvl )
{
	Item& tempItem = Items[MaxItems_255];
	//if (GameMode != GM_COLISEUM) {
	Item& healerItem0 = PepinItems[0];
	Item& healerItem1 = PepinItems[1];
	Item& healerItem2 = PepinItems[2];

	GetItemAttrs(MaxItems_255, GameMode == GM_CLASSIC ? BI_2658_SCROLL_OF_HEAL_CLS : BI_214_RELICT_OF_HEALING, 1);
	memcpy(&healerItem0, &tempItem, sizeof(Item));
	healerItem0.MakeSeed();
	healerItem0.dropType = 0;
	healerItem0.quality = qlvl;
	healerItem0.IsReqMet = true;

	GetItemAttrs(MaxItems_255, GameMode == GM_CLASSIC ? BI_2652_POTION_OF_HEAL_CLS : BI_24_POTION_OF_HEALING, 1);
	memcpy(&healerItem1, &tempItem, sizeof(Item));
	healerItem1.MakeSeed();
	healerItem1.dropType = 0;
	healerItem1.quality = qlvl;
	healerItem1.IsReqMet = true;

	int currentPepinItemIndex;
	if (MaxCountOfPlayersInGame == 1) {
		currentPepinItemIndex = PT_2_Healer_Singleplayer_ForcedItemsCount;
	}
	else {
		//Item_InitBaseItem(MaxItems_255, BI_24_POTION_OF_HEALING, 1);
		GetItemAttrs(MaxItems_255, GameMode == GM_CLASSIC ? BI_2654_POTION_OF_FULL_HEAL_CLS : BI_29_POTION_OF_FULL_HEALING, 1); // fixes pepin full healing potion morphing?
		memcpy(&healerItem2, &tempItem, sizeof(Item));
		healerItem2.MakeSeed();
		healerItem2.dropType = 0;
		healerItem2.quality = qlvl;
		healerItem2.IsReqMet = true;
		currentPepinItemIndex = PT_3_Healer_Multiplayer_ForcedItemsCount;
	}

	int itemsCount = RNG(4) + 4;
	while( currentPepinItemIndex < itemsCount ){
		Item& pepinItem = PepinItems[currentPepinItemIndex];
		tempItem.MakeSeed();
		SetRndSeed(tempItem.seed);
		GetItemAttrs(MaxItems_255, GetRandomBaseForPepinItems(qlvl) - 1, qlvl);
		memcpy(&pepinItem, &tempItem, sizeof(Item));
		pepinItem.dropType = D_PEPIN;
		pepinItem.quality = qlvl;
		pepinItem.Identified = true;
		pepinItem.IsReqMet = CheckItemReq(CurrentPlayerIndex, &pepinItem);
		currentPepinItemIndex++;
	}

	for( int currentPepinItemIndex = itemsCount; currentPepinItemIndex < PT_HealerItemsCount; currentPepinItemIndex++ ){
		Item& pepinItem = PepinItems[currentPepinItemIndex];
		pepinItem.ItemCode = IC_M1_NONE;
	}
	PepinItemsSorting();
	
	VisualTrade_RefreshPage(VTP_HealerBasic);
}

//----- (0042B34B) --------------------------------------------------------
void PutGoldBaseData()
{
	Item& tempItem = Items[MaxItems_255];
	GetItemAttrs(MaxItems_255, BI_0_GOLD, 1);
	memcpy(&ItemGoldBuffer, &tempItem, sizeof(Item));
	ItemGoldBuffer.IsReqMet = true;
}

//----- (0042B374) --------------------------------------------------------
void __fastcall RestoreGriswoldBasicalItem( int itemIndex, int baseItemIndex, int qlvl, int seed, int genVersion )
{
	Item& item = Items[itemIndex];
	item.SetSeed(seed, genVersion);
	item.dropType = D_GRISWOLD_BASIC;
	item.quality = qlvl;
	SetRndSeed(seed);
	if( false ) /*baseItemIndex = */forceCallforSync = RNG(baseItemIndex); // код в одной из версий TH
	else /*baseItemIndex = */ forceCallforSync = GetRandomBaseForGriswoldBasicalItems( qlvl ) - 1; // результат не учитывается (?) но нужно для синхронизации с генерацией
	GetItemAttrs(itemIndex, baseItemIndex, qlvl);
	item.Identified = true;
}

//----- (0042B3C5) --------------------------------------------------------
void __fastcall RestoreGriswoldMagicalItem( int itemIndex, int baseItemIndex, int qlvl, int seed, int genVersion, int extras )
{
	Item& item = Items[itemIndex];
	item.SetSeed(seed, genVersion);
	item.dropType = D_GRISWOLD_MAGIC;
	item.quality = qlvl;
	item.magicFindSeed = extras;
	int challenge_Mode_Bonus = 6 /*default*/;
	const auto currentPlayerGameMode = Players[CurrentPlayerIndex].gameMode;
//	if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_NIGHTMARE)) {
	if (is(currentPlayerGameMode, GM_IRONMAN, GM_SURVIVAL, GM_NIGHTMARE)) {// On character selection screen GameMode is not initialized so wrong items can be generated there.  CurrentPlayerIndex should be fine in MP (same game mode for all players)
		challenge_Mode_Bonus = 7;
	}
//	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_14_CLEAN_HANDS) || GameMode == GM_COLISEUM ) {
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_14_CLEAN_HANDS) || currentPlayerGameMode == GM_COLISEUM) {
		challenge_Mode_Bonus = 10;// in this mode player can't farm, can only buy items, so they need no penalty to quality of goods from vendors
	}
	LimitToRange(qlvl, 1, 10 + (challenge_Mode_Bonus * Players[CurrentPlayerIndex].CharLevel / 10));// this is Griswold shop affix level limitation
	SetRndSeed(seed);
	if( false ) /*baseItemIndex = */forceCallforSync = RNG(baseItemIndex);// код в одной из версий TH
	else /*baseItemIndex =*/ forceCallforSync = GetRandomBaseItemIndexForGriswoldMagicalItem(qlvl / 4, qlvl) - 1; // результат не учитывается (?) но нужно для синхронизации с генерацией
	GetItemAttrs(itemIndex, baseItemIndex, qlvl); // TODO: check unifying with SetPlayerHandItem (which zeroing item), GetItemAttrs called after setting params
	item.Identified = true;
	ApplyAffixesToItem(itemIndex, baseItemIndex, qlvl / 2, qlvl + extras, 1, 0);
	item.Identified = true;
}

//----- (0042B432) --------------------------------------------------------
void __fastcall RestoreWirtItem( int itemIndex, int baseItemIndex, int qlvl, int seed, int genVersion, int extras )
{
	Item& item = Items[itemIndex];
	item.SetSeed(seed, genVersion);
	item.dropType = D_WIRT;
	item.quality = qlvl;
	item.magicFindSeed = extras;
	int challenge_Mode_Bonus2 = 7 /*default*/;
	const auto currentPlayerGameMode = Players[CurrentPlayerIndex].gameMode;
//	if (is(GameMode, GM_IRONMAN, GM_SURVIVAL, GM_NIGHTMARE)) {
	if (is(currentPlayerGameMode, GM_IRONMAN, GM_SURVIVAL, GM_NIGHTMARE)) {
		challenge_Mode_Bonus2 = 8;
	}
//	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_14_CLEAN_HANDS) || GameMode == GM_COLISEUM ) {
	if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_14_CLEAN_HANDS) || currentPlayerGameMode == GM_COLISEUM) {
		challenge_Mode_Bonus2 = 10;// in this mode player can't farm, can only buy items, so they need no penalty to quality of goods from vendors
	}
	LimitToRange(qlvl, 1, 15 + (challenge_Mode_Bonus2 * Players[CurrentPlayerIndex].CharLevel / 10)); // Wirt item level
	SetRndSeed(seed);
	if( false ) /*baseItemIndex = */forceCallforSync = RNG(baseItemIndex);// код в одной из версий TH
	else /*baseItemIndex =*/ forceCallforSync = GetRandomBaseItemIndexForWirtItem(qlvl) - 1; // результат не учитывается (?) но нужно для синхронизации с генерацией
	GetItemAttrs(itemIndex, baseItemIndex, qlvl);
	item.Identified = true;
	ApplyAffixesToItem(itemIndex, baseItemIndex, 2 * qlvl / 3, qlvl + extras, 1, 1); // another Wirt item level limit
}

//----- (0042B49D) --------------------------------------------------------
void __fastcall RestoreAdriaItem( int itemIndex, int baseItemIndex, int qlvl, int seed, int genVersion )
{
	Item& item = Items[itemIndex];

	item.SetSeed(seed, genVersion);
	item.dropType = D_ADRIA;
	item.quality = qlvl;
	if( is( baseItemIndex, BI_25_POTION_OF_MANA
	    , BI_27_RELICT_OF_TOWN_PORTAL
	    , BI_187_RELICT_OF_IDENTIFY
	    , BI_192_HOLY_WATER
	    , BI_193_FULL_HOLY_WATER
	    , BI_333_HOLY_WATER )
	     ){
		SetRndSeed(seed);
		GetItemAttrs(itemIndex, baseItemIndex, qlvl);
	}else if( baseItemIndex >= BI_114_BOOK_OF_ && baseItemIndex <= BI_117_BOOK_OF_
        || baseItemIndex >= BI_225_BOOK_OF_ && baseItemIndex <= BI_228_BOOK_OF_ ){
		SetRndSeed(seed);
		forceCallforSync = RNG(1); // синхронизация с GetRandomBaseForAdriaItems ниже
		GetItemAttrs(itemIndex, baseItemIndex, qlvl);
	}else{
		SetRndSeed(seed);
		forceCallforSync = GetRandomBaseForAdriaItems(qlvl) - 1; // результат не учитывается (?) но нужно для синхронизации с генерацией
		//int calcedBaseItemIndex = GetRandomBaseForAdriaItems(qlvl) - 1;
		GetItemAttrs(itemIndex, baseItemIndex /*calcedBaseItemIndex*/, qlvl);
		item.Identified = true;
		if( RNG(100) <= 5 || item.MagicCode == MC_23_STAFF ){
			ApplyAffixesToItem(itemIndex, baseItemIndex /*calcedBaseItemIndex*/, qlvl, qlvl * 2, 1, 1);
		}
	}
}

//----- (0042B585) --------------------------------------------------------
void __fastcall RestorePepinItem( int itemIndex, int baseItemIndex, int qlvl, int seed, int genVersion )
{
	Item& item = Items[itemIndex];
	int calcedBaseItemIndex = baseItemIndex;
	item.SetSeed(seed, genVersion);
	item.dropType = D_PEPIN;
	item.quality = qlvl;
	if( baseItemIndex != BI_24_POTION_OF_HEALING 
	 && baseItemIndex != BI_29_POTION_OF_FULL_HEALING 
	 && baseItemIndex != BI_34_KABUTO_HELM ){
			SetRndSeed(seed);
			calcedBaseItemIndex = GetRandomBaseForPepinItems(qlvl) - 1;
	}
	GetItemAttrs(itemIndex, calcedBaseItemIndex, qlvl);
	item.Identified = true;
}

//----- (0042B5EC) --------------------------------------------------------
void __fastcall RestoreTownersItems( int itemIndex, int baseItemIndex, int dropType, int seed, int qualityLevel, int genVersion, int quality, int extras )
{
	switch(dropType & D_SHOP){
	case D_GRISWOLD_BASIC: RestoreGriswoldBasicalItem(itemIndex, baseItemIndex, quality, seed, genVersion);         break;
	case D_GRISWOLD_MAGIC: RestoreGriswoldMagicalItem(itemIndex, baseItemIndex, quality, seed, genVersion, extras);	break;
	case D_WIRT          : RestoreWirtItem           (itemIndex, baseItemIndex, quality, seed, genVersion, extras);	break;
	case D_ADRIA         : RestoreAdriaItem          (itemIndex, baseItemIndex, quality, seed, genVersion);			break;
	case D_PEPIN         : RestorePepinItem          (itemIndex, baseItemIndex, quality, seed, genVersion);			break;
	}
}

//----- (0042B653) --------------------------------------------------------
void CheckAllItemsReqMets()
{
	for( auto& item: BasicalItemsArray    ) if( item.ItemCode != IC_M1_NONE ) UpdateItem( item );
    for( auto& item: MagicItemsStoreArray ) if( item.ItemCode != IC_M1_NONE ) UpdateItem( item );
    for( auto& item: WitchAdriaItems      ) if( item.ItemCode != IC_M1_NONE ) UpdateItem( item );
    for( auto& item: PepinItems           ) if( item.ItemCode != IC_M1_NONE ) UpdateItem( item );
	UpdateItem( WirtItem );
}

//----- (0042B6F4) --------------------------------------------------------
int SomeActionWithDropItemsInAloneRooms()
{
	int itemIndex = ItemActive[ItemsCount - 1];
	Item& item = Items[itemIndex];
	item.FrameIndex = item.FramesCount;
	item.playAnimation = 0;
	LOBYTE_IDA(item.animPhase) = 1;
	return itemIndex;
}

//----- (0042B723) --------------------------------------------------------
int __fastcall CreateSpellBook(int row, int col, int spellIndex, int sendMsg, int delta)
{
	int itemIndex = -1;
	int qlvl = spellIndex == -123 ? 63 : Spells[spellIndex].BookQuality + 1; // TODO: review max qlvl
	if( qlvl >= 1 && GetItemSpace(row, col, itemIndex) ){
		int baseItemIndex = RndTypeItems(0, MC_24_BOOKS, qlvl);
		Item& item = Items[itemIndex];
		int attempts = 1000;
		do{
			SetupAllItems(itemIndex, baseItemIndex, GetRndSeed(), 2 * qlvl, 1, 1, 0, delta, false, 0, 0, 0, -1, 0, Difficulty);
			if( item.MagicCode == MC_24_BOOKS && is(spellIndex, item.SpellIndex, -123) ) break;
		}while( --attempts );
		if( NetItem ){
			if (sendMsg) NetSendCmdDItem(FALSE, itemIndex);
			if (delta) DeltaAddItem(itemIndex);
		}
	}
	return itemIndex;
}

//----- (00715400) --------------------------------------------------------
int __fastcall CreatePepinRing(int row, int col, int itemQlvl, int sendMsg, int delta)
{
	int itemIndex = -1;
	if( GetItemSpace(row, col, itemIndex) ){
		int baseItemIndex = RndTypeItems(IC_12_RING, MC_25_RINGS, itemQlvl);
		SetupAllItems(itemIndex, baseItemIndex, GetRndSeed(), 2 * itemQlvl, 1, 1, 0, delta, false, 0, 0, 0, -1, 0, Difficulty);
		if( NetItem ){
			if (sendMsg) NetSendCmdDItem(FALSE, itemIndex);
			if (delta) DeltaAddItem(itemIndex);
		}
	}
	return itemIndex;
}

//----- (0042B9A3) --------------------------------------------------------
int __fastcall CreateMagicItemFormSlainHero( int row, int col, int itemCode, int itemGfxId, int sendMsg, int delta )
{
	int itemIndex = -1;
	int magicCode = itemCode == IC_10_STAFF ? MC_23_STAFF : MC_0_NONE;
	int itemLevel = ItemLevelByDungeonLevel();
	if( GetItemSpace(row, col, itemIndex) ){
		int baseItemIndex = RndTypeItems( itemCode, magicCode, itemLevel );
		for( int attempts = 0; attempts < 100; ++attempts ){ // пока что ввел ограничение на 100 попыток, чтобы гарантировано не висло
			SetupAllItemsSmartdrop( itemIndex, baseItemIndex, GetRndSeed(), 2 * itemLevel, 1, 1, 0, delta );
			// потом возможно переделать на сравнение с baseitem.SubType или .Material (заполнение списка и случайный выбор, без "бесконечного" цикла)
			if( Items[ itemIndex ].GraphicValue == itemGfxId ) break;
			baseItemIndex = RndTypeItems( itemCode, magicCode, itemLevel );
		}
		if( NetItem ){
			if (sendMsg) NetSendCmdDItem(FALSE, itemIndex);
			if (delta) DeltaAddItem(itemIndex);
		}
	}
	return itemIndex;
}

//----- (0042BB35) --------------------------------------------------------
void DeleteGetRecord( int itemIndex )
{
	ItemGetCount--;
	if( ItemGetCount ){
		ItemGets[itemIndex] = ItemGets[ItemGetCount];
	}
}

//----- (0042BA92) --------------------------------------------------------
bool CheckGetRecord( const LastItemInfo& info )
{
	i64 dwCurr = GetTickCountTh();
	for( int i = 0; i < ItemGetCount; i++ ){
		if( (dwCurr - ItemGets[i].timeStamp) > RESEND_TIME ) {
			DeleteGetRecord( i );
			i--;
			continue;
		}
		if( info == ItemGets[i] ){
			return FALSE;
		}
	}
	return TRUE;
}

//----- (0042BB7B) --------------------------------------------------------
void AddGetRecord( const LastItemInfo& info )
{
	if( ItemGetCount < MaxItems_255 ){
		ItemGets[ItemGetCount].timeStamp = GetTickCountTh();
		ItemGets[ItemGetCount] = info;
		ItemGetCount++;
	}
}

//----- (0042BBC2) --------------------------------------------------------
void RemoveGetRecord( const LastItemInfo& info )
{
	i64 dwCurr = GetTickCountTh();
	for( int i = 0; i < ItemGetCount; i++ ){
		if( (dwCurr - ItemGets[i].timeStamp) > RESEND_TIME ){
			DeleteGetRecord( i );
			i--;
			continue;
		}
		if( info == ItemGets[i] ){
			DeleteGetRecord( i );
			return;
		}
	}
}

//----- (th2) -------------------------------------------------------------
bool IsQuestItem( int baseItemIndex )
{
	//if (GameMode == GM_CLASSIC /*&& baseItemIndex  < 109*/ ){
	//	baseItemIndex = BI(baseItemIndex);
	//}
	//else {
		switch (baseItemIndex)
		{
		case BI_9_MAGIC_ROCK:
		case BI_16_ANVIL_OF_FURY:
		case BI_19_FUNGAL_TOME:  // Fungal Tome
		case BI_22_CRYPT_MAP:  // Treasure Map
		case BI_33_STAFF_OF_LAZARUS:  // Staff of Lazarus
		case BI_289_BLOOD_STONE:
		case BI_327_TAVERN_SIGN:
		case BI_328_GOLDEN_ELIXIR:
		case BI_330_BLACK_MUSHROOM:
		case BI_331_BRAIN:
		case BI_332_SPECTRAL_ELIXIR: // Spectral Elixir (quest)
		case BI_334_THEODORE:
		case 335: // Torn Note 1
		case 336: // Torn Note 2
		case 337: // Torn Note 3
		case 338: // Reconstructed Note
		case BI_399_SHADOWFANG:
		case BI_1204_THE_PROOF:
		case BI_2456_CHEST_KEY:
		case BI_2457_IRON_FIGURINE:
		case BI_2459_REAGENTS:
		case BI_2460_THE_RIGHT_GOODS:
		case BI_2461_MUNDANE_INVENTORY:
		case BI_2472_THE_LOCKED_CHEST:
		case BI_2474_TOOL_CHEST:
		case BI_2475_CRYSTAL_EYEPIECE:
		case BI_2476_THE_STRANGE_BOOK:
		case BI_2510_DUSTY_CRATE:
		case BI_2511_MT_CONTAINERS:
		case BI_2512_FULL_CONTAINERS:
			return true;
		}
	//}
    return false;
}

//----- (th2) -------------------------------------------------------------
bool IsObsoleteItem( int baseItemIndex )
{
    switch( baseItemIndex )
    {
        case 23:  // Heart
        case 329: // Amber
        case 339: // Executioner's Oil
        case 398: // Relict of Resurrect
            return true;
        default:
            break;
    }
    return false;
}

//----- (th2) -------------------------------------------------------------
bool IsValueStoneItem( int baseItemIndex )
{
    switch( baseItemIndex )
    {
        case 296: // Diamond
        case 297: // Ruby
        case 298: // Sapphire
        case 299: // Topaz
        case 300: // Amethyst
        case 301: // Opal
        case 302: // Hell Sapphire
        case 303: // Hell Ruby
        case 304: // Hell Diamond
            return true;
        default:
            break;
    }
    return false;
}

//----- (th2) -------------------------------------------------------------
bool IsItemForbiddenToStore( int baseItemIndex )
{
    if( IsQuestItem( baseItemIndex ) ){
        return true;
    }
    if( IsObsoleteItem( baseItemIndex ) ){
        return true;
    }
    //if( IsValueStoneItem( baseItemIndex ) ){
    //    return true;
    //}

    /*switch( baseItemIndex )
    {
        // droppable elixirs - allowed for stashing now
        case 20:  // Spectral Elixir
        case 87:  // Elixir of Strength
        case 88:  // Elixir of Magic
        case 89:  // Elixir of Dexterity
        case 90:  // Elixir of Vitality
            return true;
        default:
            break;
    }*/
    return false;
}

// ==============================
// Set items section
// ==============================

// Mask of equipped items for given UniqSet
using SetItemsEquipMask = std::bitset<IS_Inventory>;

struct SetInfo
{
	SetItemsEquipMask equippedSetItemMasks;
	Item fakeItemToApplySetBonuses;
};

using PlayerCacheAboutSet = std::map<int, SetInfo>;

PlayerCacheAboutSet PlayerCacheAboutSetData[PlayersMax_4];

//----- (th2) -------------------------------------------------------------
void SetItem_Init()
{
	// Each uniq listed in sets info should be marked as unique from this set
	for( size_t setIndex = 0; setIndex < count_UniqueSet; ++setIndex ){
		const UniqueSet& set = UniqueSets[setIndex];
		for( int i = 0, ie = set.UniquesCount; i != ie; ++i )
		{
			int uniqueItemIndex = set.UniquesList[i];
			if( uniqueItemIndex < 0 || uniqueItemIndex >= int(count_UniqueItems) )
			{
				TerminateWithError("SetItem_Init: illegal uniqueItemIndex %%d in set #%d", uniqueItemIndex, setIndex);
			}
			const UniqueItem& uniq = UniqueItems[uniqueItemIndex];
			if( uniq.uniqueSetIndex != setIndex )
			{
				TerminateWithError("SetItem_Init: illegal set index #%d for unique #%d. Should be #%d", uniq.uniqueSetIndex, uniqueItemIndex, setIndex);
			}
		}
	}

	// Each uniq marked as unique from a set should be listed in given set's list
	for( size_t uniqueItemIndex = 0; uniqueItemIndex < count_UniqueItems; ++uniqueItemIndex ){
		const UniqueItem& uniq = UniqueItems[uniqueItemIndex];
		int setIndex = uniq.uniqueSetIndex;
		if( setIndex >= 0 )
		{
			if( setIndex >= int(count_UniqueSet) )
			{
				TerminateWithError("SetItem_Init: illegal setIndex #%d in unique #%d", setIndex, uniqueItemIndex);
			}

			const UniqueSet& set = UniqueSets[setIndex];
			bool found = false;

			for( int i = 0, ie = set.UniquesCount; i != ie; ++i )
			{
				if( uniqueItemIndex == set.UniquesList[i] )
				{
					found = true;
					break;
				}
			}

			if( !found )
			{
				TerminateWithError("SetItem_Init: unique #%d is not listed in set uniques list for set #%d", uniqueItemIndex, setIndex);
			}
		}
	}

	// Init FakeItemToShowEffect for each set
	for( size_t setIndex = 0; setIndex < count_UniqueSet; ++setIndex ){
		UniqueSet& set = UniqueSets[setIndex];
		Item& item = set.FakeItemToShowEffect;
		memset(&item, 0, sizeof( item ));
		item.Identified = true;
		for( int i = 0, ie = set.EffectCount; i != ie; ++i )
		{
			const BaseEffect& effect = set.Effects[i].Effect;
			Item_ApplyAffix(item, effect, 0, 0, 1, i, 0, 0, false);
		}
	}
}

//----- (th2) -------------------------------------------------------------
void SetItem_ForEachPlayerEquippedItem( std::function<void( Item* item)> cb )
{
	Player& player = Players[CurrentPlayerIndex];

	for( auto& item : player.OnBodySlots ){
		cb( &item );
	}

	for( auto& swapItem : WeaponSwapItems ){
		Item item;
		LoadItem( &swapItem, &item );
		cb( &item );
		SaveItem( &swapItem, &item );
	}
}

//----- (th2) -------------------------------------------------------------
bool SetItem_IsSetUniqueItem( Item* item )
{
	if( item->ItemCode == IC_M1_NONE )
	{
		return false;
	}

	if( !item->Identified )
	{
		return false;
	}

	return item->MagicLevel == ML_3_SET;
}

//----- (th2) -------------------------------------------------------------
int SetItem_FindUniqueItemOrderInSet( int uniqueItemIndex )
{
	if( uniqueItemIndex >= int(count_UniqueItems) )
	{
		TerminateWithError("SetItem_FindUniqueItemOrderInSet: illegal uniqueItemIndex %d", uniqueItemIndex);
	}
	const UniqueItem& uniq = UniqueItems[uniqueItemIndex];
	int uniqueSetIndex = uniq.uniqueSetIndex;
	if( uniqueSetIndex < 0 )
	{
		TerminateWithError("SetItem_FindUniqueItemOrderInSet: unique #%d is not a set unique. uniqueSetIndex %d", uniqueItemIndex, uniqueSetIndex);
	}
	const UniqueSet& set = UniqueSets[uniqueSetIndex];
	for( int i = 0, ie = set.UniquesCount; i != ie; ++i )
	{
		if( uniqueItemIndex == set.UniquesList[i] )
		{
			return i;
		}
	}

	TerminateWithError("SetItem_FindUniqueItemOrderInSet: unique #%d is not in a set #%d", uniqueItemIndex, uniqueSetIndex);
	return -1;
}

//----- (th2) -------------------------------------------------------------
bool SetItem_IsItemFromSetEquipped( int playerIndex, int uniqueItemIndex )
{
	if( playerIndex >= PlayersMax_4 )
	{
		TerminateWithError("SetItem_IsItemFromSetEquipped: illegal playerIndex %d", playerIndex);
	}
	if( uniqueItemIndex >= int(count_UniqueItems) )
	{
		TerminateWithError("SetItem_IsItemFromSetEquipped: illegal uniqueItemIndex %d", uniqueItemIndex);
	}
	const PlayerCacheAboutSet& cache = PlayerCacheAboutSetData[playerIndex];
	const UniqueItem& uniq = UniqueItems[uniqueItemIndex];
	int uniqueSetIndex = uniq.uniqueSetIndex;

	auto it = cache.find( uniqueSetIndex );
	if( it == cache.cend() )
	{
		return false; // no items from set equipped
	}

	int order = SetItem_FindUniqueItemOrderInSet( uniqueItemIndex );
	return it->second.equippedSetItemMasks.test( order );
}

//----- (th2) -------------------------------------------------------------
int SetItem_GetCountOfEquippedItemsFromSet( int playerIndex, int uniqueSetIndex )
{
	if( playerIndex >= PlayersMax_4 )
	{
		TerminateWithError("SetItem_GetCountOfEquippedItemsFromSet: illegal playerIndex %d", playerIndex);
	}
	if( uniqueSetIndex >= int(count_UniqueSet) )
	{
		TerminateWithError("SetItem_GetCountOfEquippedItemsFromSet: illegal uniqueSetIndex %d", uniqueSetIndex);
	}

	const PlayerCacheAboutSet& cache = PlayerCacheAboutSetData[playerIndex];
	auto it = cache.find( uniqueSetIndex );
	if( it == cache.cend() )
	{
		return 0; // no items from set equipped
	}
	return it->second.equippedSetItemMasks.count();
}

//----- (th2) -------------------------------------------------------------
void SetItem_RecalculateSetInfo( int playerIndex )
{
	if( playerIndex >= PlayersMax_4 )
	{
		TerminateWithError("SetItem_RecalculateSetInfo: illegal playerIndex %d", playerIndex);
	}
	Player& player = Players[playerIndex];
	PlayerCacheAboutSet& cache = PlayerCacheAboutSetData[playerIndex];

	// Clear cache
	cache.clear();

	// Register each equiped set item in cache
	SetItem_ForEachPlayerEquippedItem(
		[&cache]( Item* item )
		{
			if( SetItem_IsSetUniqueItem( item ) && item->IsReqMet )
			{
				int order = SetItem_FindUniqueItemOrderInSet( item->UniqueItemIndex );
				const UniqueItem& uniq = UniqueItems[item->UniqueItemIndex];
				cache[uniq.uniqueSetIndex].equippedSetItemMasks.set( order );
			}
		} );

	// Init fakeItemToApplySetBonuses for each equipped set
	for( auto& c : cache )
	{
		int setIndex = c.first;
		SetInfo& setInfo = c.second;
		UniqueSet& set = UniqueSets[setIndex];
		Item& item = setInfo.fakeItemToApplySetBonuses;
		memset(&item, 0, sizeof( item ));
		item.ItemCode = IC_M1_NONE;
		int equippedCount = setInfo.equippedSetItemMasks.count();
		for( int i = 0, ie = set.EffectCount; i != ie; ++i ){
			const auto& e = set.Effects[i];
			if( equippedCount >= e.Threshold ){
				item.ItemCode = IC_0_OTHER;
				item.IsReqMet = true;
				item.Identified = true;
				const BaseEffect& effect = set.Effects[i].Effect;
				Item_ApplyAffix(item, effect, 0, 0, 1, i, 0, 0, false);
			}
		}
	}
}

//----- (th2) -------------------------------------------------------------
void SetItem_GetActiveSetItemEffects( uint playerIndex, std::vector<Item*>& result )
{
	if( playerIndex >= PlayersMax_4 ) TerminateWithError("SetItem_GetActiveSetItemEffects: illegal playerIndex %d", playerIndex);
	PlayerCacheAboutSet& cache = PlayerCacheAboutSetData[playerIndex];
	for( auto& c : cache ){
		if( Item& item = c.second.fakeItemToApplySetBonuses; item.ItemCode != IC_M1_NONE ){
			result.push_back( &item );
		}
	}
}

// ==============================
// End of set items section
// ==============================
