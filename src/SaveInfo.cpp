#include "stdafx.h"

//----- (00454CBF) --------------------------------------------------------
void __fastcall SaveItem(LastItemInfo* itemInfo, const Item* item)
{
	if( item->ItemCode == IC_M1_NONE ){
        itemInfo->baseItemIndex = -1;
    }else{
		*itemInfo = *item;
    }
}

//----- (00454DCF) -------------------------------------------------------- копия для изменения
void __fastcall SavePlayerInfo(LastPlayerInfo* playerInfo, int playerIndex)
{
	LastPlayerInfo& pi = *playerInfo;
	Player& player = Players[ playerIndex ];

	memset( &pi, 0, sizeof(pi) );
	pi.signature             = SaveSignature;
	pi.version               = CurSaveVersion;
	pi.NextAction            = player.destAction;
	pi.MapRow                = player.destParam1;
	pi.MapCol                = player.destParam2;
	pi.dungeon               = player.dungeon;
	pi.Row                   = player.Row;
	pi.Col                   = player.Col;
	pi.XPos3                 = player.TargetRow;
	pi.YPos3                 = player.TargetCol;
	strcpy( pi.CharName,       player.playerName );
	pi.ClassId               = player.ClassID;
	pi.subclassID            = player.subclassID; // subclasses
	pi.specializationID      = player.specializationID; // subclasses
	pi.BaseStrength          = player.BaseStrength;
	pi.BaseMagic             = player.BaseMagic;
	pi.BaseDexterity         = player.BaseDexterity;
	pi.BaseVitality          = player.BaseVitality;
	pi.CharLevel             = player.CharLevel;
	pi.AvailableLvlPoints    = player.AvailableLvlPoints;
	pi.Xp                    = player.Xp;
	pi.TotalGold             = player.TotalGold;
	pi.BaseLife              = player.BaseLife;
	pi.MaxBaseLife           = player.MaxBaseLife;
	pi.BaseMana              = player.BaseMana;
	pi.MaxBaseMana           = player.MaxBaseMana;
	for( int i = 0; i < 128; ++i ) pi.SpellLevels[i] = (uchar)player.SpellLevels[i];
	pi.AvailableSpellMask    = player.AvailableSpellMask;
	//pi.AvailableSpellMask_64_128  = player.AvailableSpellMaskHi;
	for( int i = 0; i < IS_Inventory; ++i ) SaveItem( &pi.OnBodySlots[i], &player.OnBodySlots[i] );
	for( int i = 0; i < 70; ++i ) SaveItem( &pi.InventorySlots[i], &player.InventorySlots[i] );
	for( int i = 0; i < 70; ++i ) pi.InvUsed[i] = player.InvUsed[i];
	pi.InvItemCount          = player.InvItemCount;
	for( int i = 0; i < 70; ++i ) SaveItem( &pi.TalismanSlots[i], &player.TalismanSlots[i] );
	for( int i = 0; i < 70; ++i ) pi.TalUsed[i] = player.TalUsed[i];
	pi.TalItemCount          = player.TalItemCount;
	for( int i = 0; i <  8; ++i ) SaveItem( &pi.BeltInventory[i], &player.BeltInventory[i] );
	pi.UberDialogKilled      = UberDiabloKilled;
	pi.altWeaponIndex        = player.altWeaponIndex;
	pi.CountOfReflectCharges = player.CountOfReflectCharges;
	pi.traits                = player.traits;
	pi.allowedDifficulty     = player.allowedDifficulty;
	pi.Difficulty            = player.Difficulty;
	pi.griswoldLevel         = player.griswoldLevel;
	for( int i= 0; i < PERKS_MAX_800; ++i ) pi.perk[i] = player.perk[i];
	pi.gameChanger           = player.gameChanger;
	pi.CurrentSeedRun        = player.CurrentSeedRun;
	pi.gameMode              = player.gameMode;
}

// обнуление индекса игровых объектов при превышении списка 
// для загрузки записей новый версий на старой версии игры  
// th2, новая функция -----------------------------------------------------
template<typename IndexType, class ArrayType>
void Limit(IndexType& index, ArrayType* array)
{
    IndexType maxIndex = 0;
    while( maxIndex < 4096 && array[maxIndex].EndMarker != -1 ){
        maxIndex++;
    }
    if( index >= maxIndex ){
        index = 0;
    }
}

//----- (00454FD6) --------------------------------------------------------
template<class ItemInfo> void LoadItem(ItemInfo* itemInfo, Item* item, bool keepCharges /*= true*/)
{
	ItemInfo& info = *itemInfo;
	Item& temp = Items[MaxItems_255];
	if( (uint)info.baseItemIndex >= count_BaseItems ){
		item->ItemCode = IC_M1_NONE;
		return;
	}
	if( BaseItems[info.baseItemIndex].baseId < 0 ){
		item->ItemCode = IC_M1_NONE;
		char text[128];
		sprintf( text, "Item with invalid baseId = %i has been deleted", info.baseItemIndex );
		MessageBoxA( NULL, text, "ERROR", MB_TASKMODAL | MB_ICONERROR );
		return;
	}
	IS( ItemInfo, ItemInfo64 ){
		temp.saveVersion = info.saveVersion ? info.saveVersion : SaveVersion;
		RecreateItem(MaxItems_255, info.baseItemIndex, info.dropType, info.seed, info.amount, info.magicLevelAndIdentified, info.genVersion,
			info.quality, info.magicFind, info.uniqIndex, info.enchantIndexes, info.quenchAffix, info.quenchLevel, info.socketsAdded, info.sockets, info.difficulty, info.charFlag);
	}else{
		int magicFind = 0;
		if( (info.dropType & D_OLD_MAGIC_FIND) == D_OLD_MAGIC_FIND ){
			magicFind = info.seed & 511;
			if( info.seed & 512 ) magicFind = -magicFind;
		}
		IS( ItemInfo, ItemInfo42 ){
			RecreateItem(MaxItems_255, info.baseItemIndex, info.dropType & ~63, info.seed, info.amount, info.magicLevelAndIdentified, info.genVersion,
				info.dropType & 63, magicFind, info.seed & (info.genVersion < 3 ? 1023 : 4095), info.enchantIndexes, QA_NO, 0, info.socketsAdded, info.sockets, 0, 0);
		}else IS( ItemInfo, ItemInfo32 ){
			RecreateItem(MaxItems_255, info.baseItemIndex, info.dropType & ~63, info.seed, info.amount, info.magicLevelAndIdentified, info.genVersion,
				info.dropType & 63, magicFind, info.seed & (info.genVersion < 3 ? 1023 : 4095), {-1,-1,-1,-1,-1}, QA_NO, 0, info.socketsAdded, info.sockets, 0, 0);
		}else{
			RecreateItem(MaxItems_255, info.baseItemIndex, info.dropType & ~63, info.seed, info.amount, info.magicLevelAndIdentified, info.genVersion,
				info.dropType & 63, magicFind, info.seed & (info.genVersion < 3 ? 1023 : 4095), {-1,-1,-1,-1,-1}, QA_NO, 0, 0, {0,0,0,0,0,0}, 0, 0);
		}
	}
	temp.CurDurability = info.curDurability;
	temp.BaseDurability = info.baseDurability;
	if( keepCharges ){
		temp.CurCharges = info.curCharges;
		temp.BaseCharges = info.baseCharges;
	}
	*item = temp;
}

//----- (00455083) --------------------------------------------------------
int __fastcall RandomizeGoldsSeed(Player* player)
{
    int invItemIndex;     // ebx@1
    int v3;     // esi@1
    int goldIndex;     // ecx@4
    //int v6;     // eax@6
    v3 = (int)player;
    invItemIndex = 0;
    int invItemCount = player->InvItemCount;
    if( invItemCount > 0 ){
        Item* invItem = player->InventorySlots;
        do{
            if( invItem->baseItemIndex == BI_0_GOLD ){
                goldIndex = 0;
                if( invItemCount > 0 ){
                    do{
                        if( invItemIndex != goldIndex ){
                            if( player->InventorySlots[goldIndex].baseItemIndex == BI_0_GOLD ){
                                if( invItem->seed == player->InventorySlots[goldIndex].seed ){
                                    invItem->MakeSeed();
                                    goldIndex = -1;
                                }
                            }
                        }
                        ++goldIndex;
                    }while( goldIndex < player->InvItemCount );
                }
            }
            invItemCount = player->InvItemCount;
            ++invItemIndex;
            ++invItem;
        }while( invItemIndex < invItemCount );
    }
    return invItemCount;
}

// ----- th2, проверка на фантомные вещи и их удаление
void CheckInvalidInvItems( int playerIndex )
{
	Player& player = Players[playerIndex];
	#ifdef CHECK_BELT
	for( int itemIndex = 0; itemIndex < IS_8_Belt_Count; itemIndex++ ){
		Item& item = player.BeltInventory[ itemIndex ];
		if( item.ItemCode != IC_M1_NONE && false ){
			ClearBeltSlot( playerIndex, itemIndex );
		}
	}
	#endif
	// проверка на недовыброшенные вещи ( player.InvItemCount < real count )
	// некорректный InvItemCount возникает по неустановленной причине
	// надо бы вынести изменения InvItemCount в единую функцию и половить этот баг
	// эту новую проверку ниже не делаю в RecalcPlayer, потому что она может стать дырой для хаков
	if( player.InvItemCount < 0 ) player.InvItemCount = 0;
	check_start:
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		if( abs( player.InvUsed[ i ] ) > IS_70_40_Inv_Count ) player.InvUsed[ i ] = 0;
		if( player.InvUsed[ i ] > 0 && player.InventorySlots[ player.InvUsed[ i ] - 1 ].ItemCode != IC_M1_NONE ){
			if( player.InvUsed[ i ] == player.InvItemCount + 1 ){
				++player.InvItemCount;
				goto check_start;
			}
		}
	}
	for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
		if( player.InvUsed[ i ] > 0 && player.InventorySlots[ player.InvUsed[ i ] - 1 ].ItemCode != IC_M1_NONE ){
			if( player.InvUsed[ i ] > player.InvItemCount ){
				player.InventorySlots[ player.InvUsed[ i ] - 1 ].ItemCode = IC_M1_NONE;
				player.InvUsed[ i ] = 0;
			}
		}
	}
		
	for( int itemIndex = 0; itemIndex < player.InvItemCount; ++itemIndex ){
		Item& item = player.InventorySlots[ itemIndex ];
		if( item.ItemCode != IC_M1_NONE ){
			bool inInv = false;
			int itemNum = itemIndex + 1;
			for( int i = 0; i < IS_70_40_Inv_Count; ++i ){
				if( player.InvUsed[ i ] == itemNum ){
					inInv = true;
					break;
				}
			}
			if( !inInv ){
				ClearInventorySlot( playerIndex, itemIndex );
				--itemIndex;
				continue;
			}
		}
	}
	player.TotalGold = CalcTotalGold( playerIndex );
}

template <typename Pi> bool __fastcall LoadPlayerInfo(Pi* pi, int playerIndex, int isNetPlayer)
{
	Player& player = Players[playerIndex];
	if( playerIndex < PlayersMax_4 ) ClearPlayerPath(playerIndex);

	IS( Pi, PlayerInfo_TH1, OldPlayerInfo5, PlayerInfo_v17, PlayerInfo_v18, PlayerInfo_v21 ){
		player.Row = pi->Row;
		player.Col = pi->Col;
		player.NextRow = pi->Row;
		player.NextCol = pi->Col;
		player.TargetRow = pi->XPos3;
		player.TargetCol = pi->YPos3;
		player.dungeon = pi->dungeon;
		player.destAction = PCMD_M1_NOTHING;
		strcpy(player.playerName, pi->CharName);
		player.ClassID = (PLAYER_CLASS)pi->ClassId;
		player.BaseStrength = pi->BaseStrength;
		player.CurStrength = pi->BaseStrength;
		player.BaseMagic = pi->BaseMagic;
		player.CurMagic = pi->BaseMagic;
		player.BaseDexterity = pi->BaseDexterity;
		player.CurDexterity = pi->BaseDexterity;
		player.BaseVitality = pi->BaseVitality;
		player.CurVitality = pi->BaseVitality;
		player.CharLevel = pi->CharLevel;
		player.AvailableLvlPoints = pi->AvailableLvlPoints;
		player.Xp = pi->Xp;
		player.TotalGold = pi->TotalGold;
		player.MaxBaseLife = pi->MaxBaseLife;
		int baseLife = pi->BaseLife;
		player.BaseLife = baseLife;
		if (!isNetPlayer) {
			baseLife &= 0xFFFFFFC0; // окруление жизни до кратного 4 
			if (baseLife < 64)
				player.BaseLife = 64;
		}
		// player.BaseLife = !isNetPlayer && ((int)(playerInfo->BaseLife) & 0xFFFFFFC0) < 64 ? 64 : playerInfo->BaseLife;
		player.MaxBaseMana = pi->MaxBaseMana;
		player.BaseMana = pi->BaseMana;
		player.AvailableSpellMask = pi->AvailableSpellMask;
		for (int i = 0; i < 37; ++i) player.SpellLevels[i] = pi->BaseSpellLevels[i];
		for (int i = 37; i < 47; ++i) player.SpellLevels[i] = pi->AdvancedSpellLevels[i - 37];
		for (int i = 47; i < 64; ++i) player.SpellLevels[i] = pi->MoreAdvancedSpellLevels[i - 47];
		player.altWeaponIndex = pi->altWeaponIndex;
		std::copy(std::begin(pi->traits), std::end(pi->traits), std::begin(player.traits));
		player.CountOfReflectCharges = pi->CountOfReflectCharges;
		player.allowedDifficulty = pi->allowedDifficulty;
		player.Difficulty = pi->Difficulty;
	}
	IS( Pi, PlayerInfo_TH1 ){
		for (int i = IS_Waist; i < IS_Inventory; ++i) player.OnBodySlots[i].ItemCode = IC_M1_NONE;
		auto itemInfo = pi->InventorySlots;
		auto item = player.InventorySlots;
		for (int i = 0; i < 40; ++i) LoadItem(itemInfo++, item++);
		for (int i = 0; i < 40; ++i) player.InvUsed[i] = pi->InvUsed[i];
		player.InvItemCount = pi->InvItemCount;
		itemInfo = pi->BeltInventory;
		item = player.BeltInventory;
		for (int i = 0; i < 8; ++i) LoadItem(itemInfo++, item++);
	}
	IS( Pi, OldPlayerInfo5 ){
		auto itemInfo = pi->OnbodySlots;
		auto item = player.OnBodySlots;
		for (int i = 0; i < 7; ++i) LoadItem(itemInfo++, item++);
		itemInfo = pi->newbodySlots;
		item = &player.OnBodySlots[7];
		for (int i = 0; i < 3; ++i) LoadItem(itemInfo++, item++);
		if (pi->version >= 12) {
			itemInfo = pi->newInvSlots;
			item = &player.InventorySlots[40];
			for (int i = 0; i < 30; ++i) LoadItem(itemInfo++, item++);
			for (int i = 0; i < 30; ++i) player.InvUsed[i + 40] = pi->newInvUsed[i];
		}else{
			for (int i = 40; i < 70; ++i) {
				player.InventorySlots[i].ItemCode = IC_M1_NONE;
				player.InvUsed[i] = 0;
			}
		}
		itemInfo = pi->InventorySlots;
		item = player.InventorySlots;
		for (int i = 0; i < 40; ++i) LoadItem(itemInfo++, item++);
		for (int i = 0; i < 40; ++i) player.InvUsed[i] = pi->InvUsed[i];
		player.InvItemCount = pi->InvItemCount;
		itemInfo = pi->BeltInventory;
		item = player.BeltInventory;
		for (int i = 0; i < 8; ++i) LoadItem(itemInfo++, item++);
		if (pi->version >= 13) {
			for (int i = 0; i < 100; ++i) player.perk[i] = pi->perk[i];
			player.subclassID = pi->subclassID;// subclasses
			player.specializationID = pi->specializationID;// subclasses
		}
		if (pi->version >= 15) {
			player.gameMode = pi->gameMode;
		}
	}
	IS( Pi, PlayerInfo_v17, PlayerInfo_v18, PlayerInfo_v21 ){
		auto itemInfo = pi->OnbodySlots;
		auto item = player.OnBodySlots;
		for (int i = 0; i < 7; ++i) LoadItem(itemInfo++, item++);
		itemInfo = pi->newbodySlots;
		item = &player.OnBodySlots[7];
		for (int i = 0; i < 3; ++i) LoadItem(itemInfo++, item++);
		itemInfo = pi->newInvSlots;
		item = &player.InventorySlots[40];
		for (int i = 0; i < 30; ++i) LoadItem(itemInfo++, item++);
		for (int i = 0; i < 30; ++i) player.InvUsed[i + 40] = pi->newInvUsed[i];
		for (int i = 0; i < countof(pi->perk); ++i) player.perk[i] = pi->perk[i];
		player.subclassID = pi->subclassID;
		player.specializationID = pi->specializationID;
		player.gameMode = pi->gameMode;
		itemInfo = pi->InventorySlots;
		item = player.InventorySlots;
		for (int i = 0; i < 40; ++i) LoadItem(itemInfo++, item++);
		for (int i = 0; i < 40; ++i) player.InvUsed[i] = pi->InvUsed[i];
		player.InvItemCount = pi->InvItemCount;
		itemInfo = pi->BeltInventory;
		item = player.BeltInventory;
		for (int i = 0; i < 8; ++i) LoadItem(itemInfo++, item++);
		IS( Pi, PlayerInfo_v18, PlayerInfo_v21 ){
			player.gameChanger = pi->gameChanger;
			player.CurrentSeedRun = pi->CurrentSeedRun;
		}
	}
	IS( Pi, PlayerInfo_v22 ){
		player.destAction            = pi->NextAction;
		player.destParam1            = pi->MapRow;
		player.destParam2            = pi->MapCol;
		player.dungeon               = pi->dungeon;
		player.Row                   = pi->Row;
		player.Col                   = pi->Col;
		player.TargetRow             = pi->XPos3;
		player.TargetCol             = pi->YPos3;
		strcpy( player.playerName,     pi->CharName );
		player.ClassID               = (PLAYER_CLASS)pi->ClassId;
		player.subclassID            = pi->subclassID;
		player.specializationID      = pi->specializationID;
		player.BaseStrength          = pi->BaseStrength;
		player.BaseMagic             = pi->BaseMagic;
		player.BaseDexterity         = pi->BaseDexterity;
		player.BaseVitality          = pi->BaseVitality;
		player.CharLevel             = pi->CharLevel;
		player.AvailableLvlPoints    = pi->AvailableLvlPoints;
		player.Xp                    = pi->Xp;
		player.TotalGold             = pi->TotalGold;
		player.BaseLife              = pi->BaseLife;
		player.MaxBaseLife           = pi->MaxBaseLife;
		player.BaseMana              = pi->BaseMana;
		player.MaxBaseMana           = pi->MaxBaseMana;
		for( int i = 0; i < 128; ++i ) player.SpellLevels[i] = pi->SpellLevels[i];
		player.AvailableSpellMask    = pi->AvailableSpellMask;
		//player.AvailableSpellMask_64_128  = pi->AvailableSpellMask_64_128;
		UberDiabloKilled             = pi->UberDialogKilled;
		player.altWeaponIndex        = pi->altWeaponIndex        ;
		player.CountOfReflectCharges = pi->CountOfReflectCharges;
		player.traits                = pi->traits;
		player.allowedDifficulty     = pi->allowedDifficulty;
		player.Difficulty            = pi->Difficulty;
		for( int i = 0; i < PERKS_MAX_800; ++i ) player.perk[i] = pi->perk[i];
		player.gameChanger           = pi->gameChanger;
		player.CurrentSeedRun        = pi->CurrentSeedRun;
		player.gameMode              = pi->gameMode; // LoadItem requires game mode

		player.InvItemCount          = pi->InvItemCount;
		for( int i = 0; i < 10; ++i ) LoadItem( &pi->OnBodySlots[i], &player.OnBodySlots[i] );
		for( int i = 0; i < 70; ++i ) LoadItem( &pi->InventorySlots[i], &player.InventorySlots[i] );
		for( int i = 0; i < 70; ++i ) player.InvUsed[i] = pi->InvUsed[i];
		for( int i = 0; i <  8; ++i ) LoadItem( &pi->BeltInventory[i], &player.BeltInventory[i] );

		for (int i = IS_LeftRing2; i < IS_Inventory; ++i) player.OnBodySlots[i].ItemCode = IC_M1_NONE;
		player.TalItemCount          = 0;
		for( int i = 0; i < 70; ++i ) player.TalismanSlots[i].ItemCode = IC_M1_NONE;
		for( int i = 0; i < 70; ++i ) player.TalUsed[i] = 0;
	}
	IS( Pi, PlayerInfo_v26 ){
		player.destAction            = pi->NextAction;
		player.destParam1            = pi->MapRow;
		player.destParam2            = pi->MapCol;
		player.dungeon               = pi->dungeon;
		player.Row                   = pi->Row;
		player.Col                   = pi->Col;
		player.TargetRow             = pi->XPos3;
		player.TargetCol             = pi->YPos3;
		strcpy( player.playerName,     pi->CharName );
		player.ClassID               = (PLAYER_CLASS)pi->ClassId;
		player.subclassID            = pi->subclassID;
		player.specializationID      = pi->specializationID;
		player.BaseStrength          = pi->BaseStrength;
		player.BaseMagic             = pi->BaseMagic;
		player.BaseDexterity         = pi->BaseDexterity;
		player.BaseVitality          = pi->BaseVitality;
		player.CharLevel             = pi->CharLevel;
		player.AvailableLvlPoints    = pi->AvailableLvlPoints;
		player.Xp                    = pi->Xp;
		player.TotalGold             = pi->TotalGold;
		player.BaseLife              = pi->BaseLife;
		player.MaxBaseLife           = pi->MaxBaseLife;
		player.BaseMana              = pi->BaseMana;
		player.MaxBaseMana           = pi->MaxBaseMana;
		for( int i = 0; i < 128; ++i ) player.SpellLevels[i] = pi->SpellLevels[i];
		player.AvailableSpellMask    = pi->AvailableSpellMask;
		//player.AvailableSpellMask_64_128  = pi->AvailableSpellMask_64_128;
		UberDiabloKilled             = pi->UberDialogKilled;
		player.altWeaponIndex        = pi->altWeaponIndex        ;
		player.CountOfReflectCharges = pi->CountOfReflectCharges;
		player.traits                = pi->traits;
		player.allowedDifficulty     = pi->allowedDifficulty;
		player.Difficulty            = pi->Difficulty;
		player.griswoldLevel         = pi->griswoldLevel;
		for( int i = 0; i < PERKS_MAX_800; ++i ) player.perk[i] = pi->perk[i];
		player.gameChanger           = pi->gameChanger;
		player.CurrentSeedRun        = pi->CurrentSeedRun;
		player.gameMode              = pi->gameMode; // LoadItem requires game mode
		
		player.InvItemCount          = pi->InvItemCount;
		for( int i = 0; i < 17; ++i ) LoadItem( &pi->OnBodySlots[i], &player.OnBodySlots[i] );
		for( int i = 0; i < 70; ++i ) LoadItem( &pi->InventorySlots[i], &player.InventorySlots[i] );
		for( int i = 0; i < 70; ++i ) player.InvUsed[i] = pi->InvUsed[i];
		for( int i = 0; i <  8; ++i ) LoadItem( &pi->BeltInventory[i], &player.BeltInventory[i] );
		player.TalItemCount          = pi->TalItemCount;
		for( int i = 0; i < 70; ++i ) LoadItem( &pi->TalismanSlots[i], &player.TalismanSlots[i] );
		for( int i = 0; i < 70; ++i ) player.TalUsed[i] = pi->TalUsed[i];
	}
	if( playerIndex < PlayersMax_4 ){
		InitPlayer(playerIndex, 1);
		RandomizeGoldsSeed(&player);
		if (playerIndex == CurrentPlayerIndex) for (Item& adriaItem : WitchAdriaItems) adriaItem.ItemCode = IC_M1_NONE;
		RecalcPlayer(playerIndex, 0);
		player.CurLife = player.MaxCurLife;
		player.CurMana = player.MaxCurMana;
		if (player.altWeaponIndex >= 3)
			player.altWeaponIndex = 0;
		player.OpenedTownDoorMask = 0;
		player.DungeonMessageFlags1 = 0;
		player.SomeSoundMuteDelay = 0;
		player.DungeonMessageFlags2 = 0;
	}
	return true;
}

//----- (004550F2) --------------------------------------------------------
template<> bool __fastcall LoadPlayerInfo(uchar* playerInfo, int playerIndex, int isNetPlayer)
{
    auto pi = (PlayerInfo*)playerInfo;
	if(pi->signature == SaveSignature && pi->version > 0 ){
		SaveVersion = pi->version;
		if(pi->version >= 26) LoadPlayerInfo((PlayerInfo_v26*)pi, playerIndex, isNetPlayer); else
		if(pi->version >= 22) LoadPlayerInfo((PlayerInfo_v22*)pi, playerIndex, isNetPlayer); else
		if(pi->version >= 21) LoadPlayerInfo((PlayerInfo_v21*)pi, playerIndex, isNetPlayer); else
		if(pi->version >= 18) LoadPlayerInfo((PlayerInfo_v18*)pi, playerIndex, isNetPlayer); else
		if(pi->version >= 17) LoadPlayerInfo((PlayerInfo_v17*)pi, playerIndex, isNetPlayer); else
		if(pi->version >=  1) LoadPlayerInfo((OldPlayerInfo5*)pi, playerIndex, isNetPlayer); else
		                      LoadPlayerInfo((PlayerInfo_TH1*)pi, playerIndex, isNetPlayer);
		// добавлена проверка на фантомные вещи и их удаление
		if( playerIndex < PlayersMax_4 ) CheckInvalidInvItems(playerIndex);
	}else{
		return false;
	}
	return true;
}
