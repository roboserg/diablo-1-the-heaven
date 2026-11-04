#include "stdafx.h"
#include "ItemOverlay.h"

//----- (th2) -------------------------------------------------------------
int HasSpaceForGold( uint playerIndex, int amount )
{
	Player& player = Players[playerIndex];
	for( int i = 0; i < player.InvItemCount && amount > 0; ++i ){
		Item& item = player.InventorySlots[ i ];
		if( item.ItemCode == IC_11_GOLD ){
			if( item.amount < MaxGoldInCell ){
				amount -= MaxGoldInCell - item.amount;
			}
		}
	}
	if( amount > 0 ){
		for( int y = InvRows - 1, ye = 0; y >= ye; --y ){
			for( int x = player.invCols - 1, xe = 0; x >= xe && amount > 0; --x ){
				if( !player.InvUsedAlt[ y ][ x ] ){
					amount -= MaxGoldInCell;
				}
			}
		}
	}
	return amount <= 0;
}

// (rt) th2, New function. Auto-pickup of gold while the player moves
// It could be tied to some new suffix (not needed - Mor)
// (rt) Also, ideally it should be tied to the event of the player changing cells,
// (rt) so it doesn't grind constantly
//----- (th2) -------------------------------------------------------------
void __fastcall GoldAutoPickup(int playerIndex)
{
	//ex(Players) ex(ItemsOnGround) ex(CurrentPlayerIndex) ex(NetSendCmdGItem)
	Player& player = Players[ playerIndex ];
	bool walking = false;
	if (player.CurAction == PCA_1_WALK_UP || player.CurAction == PCA_2_WALK_DOWN || player.CurAction == PCA_3_WALK_HORISONTAL) {
		walking = true;
	}
	if (!(Dungeon->genType || Dungeon->isQuest) || /*(MaxCountOfPlayersInGame > 1 && NetPlayerCount > 1) ||*/ IsINVPanelVisible /*|| !Scroll.dir*/ || !walking) {
		return;
	}
	int playerRow, playerCol;
	playerRow = player.Row;
	playerCol = player.Col;
	for( int orient = 0; orient < 9; ++orient ){
		if( orient < 8 ){ 
			int orientDif = abs( player.dir - orient);
			if( orientDif >= 3 && orientDif <= 5 ) continue; // (rt) gold is not auto-picked from the 3 cells behind the player
		}
		int row = playerRow + RowDelta[orient];
		int col = playerCol + ColDelta[orient];
		int itemNum = ItemMap[ row ][ col ]; 
		if( itemNum > 0 ){
			int itemIndex = itemNum - 1;
			Item& item = Items[ itemIndex ];
			if( item.ItemCode == IC_11_GOLD && ! item.gottenFromLand ){
				if( ! HasSpaceForGold( playerIndex, item.amount ) ) return;
                NetSendCmdGItem( 1, CMD_39_REQUESTGITEM, CurrentPlayerIndex, CurrentPlayerIndex, itemIndex );
				//track_repeat_walk( false );
				Items[ itemIndex ].gottenFromLand = 1;
				PlayLocalSound( Flip_DropSound[ ItemFlipTable[ item.GraphicValue ] ], row, col );
				//ItemsOnGroundMap[ row ][ col ] = 0;
				break;
			}
		}
	}
}

PlayerClass PlayerClasses[PC_COUNT] = {
	
// 0 Paladin
	{
	"Warrior",			// NamePtr
	{ 25,  5, 15, 20 },	// StartStats
	//MaxStat[PC_0_WARRIOR],	// MaxStats
	10,	// Block
	8,	// SubSteps
	PS_26_ITEM_REPAIR,	// SkillIndex
	80, // LifeBase
	5, // ManaBase
	{ 1, 0, 100 },// BaseDamageMultipliers
	3,// Appearance
	{ BI_394_LIGHT_DAGGER , BI_395_BUCKLER, BI_362_LIGHT_HAMMER, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING }, // StartItems
	{ S_736_WAR_13, S_763_WAR_34, S_756_WAR_27, S_764_WAR_35, S_737_WAR_14}	// Voices
	},
// 1 Scout
	{
	"Archer",			// NamePtr
	{ 15,  5, 30, 15 },	// StartStats
	//MaxStat[PC_1_ARCHER],	// MaxStats
	1,		// Block
	8,	// SubSteps
	PS_9_INFRAVISION,			// SkillIndex
	50,							// LifeBase
	12,							// ManaBase
	{ 1, 1, 200 },				// BaseDamageM
	4,							// Appearance
	{ BI_393_SMALL_BOW, -1, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING }, // StartItems
	{ S_633_ROG_13, S_654_ROG_34, S_647_ROG_27, S_655_ROG_35, S_634_ROG_14}	// Voices
	},
// 2 Mage
	{ "Mage",				// NamePtr
	{ 10,  30, 10, 15 },	// StartStats
	//MaxStat[PC_2_MAGE],	// MaxStats
	1,		// Block
	8,		// SubSteps
	PS_27_STAFF_RECHARGE,		// SkillIndex
	25,							// LifeBase
	60,							// ManaBase
	{ 1, 0, 100 },				// BaseDamageM
	4,							// Appearance
	{ BI_5_WAND_OF_HOLY_BOLTS, -1, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },		// StartItems
	{ S_530_SOR_13, S_551_SOR_34, S_544_SOR_27, S_552_SOR_35, S_531_SOR_14}	// Voices
	},
// 3 Monk
	{ 
	"Monk",				// NamePtr
	{ 15,  5, 25, 20 }, // StartStats
	//MaxStat[PC_3_MONK],	// MaxStats
	1,	// Block
	8,	// SubSteps
	PS_33_TELEKINES,				// SkillIndex
	70,							// LifeBase
	12,							// ManaBase
	{ 1, 1, 150 },				// BaseDamageM
	8,							// Appearance
	{ BI_392_STAFF, -1, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING }, // StartItems
	{ S_850_MONK_13, S_871_MONK_34, S_864_MONK_27, S_872_MONK_35, S_851_MONK_14 } // Voices
	},
// 4 Assassin
	{
	"Rogue",			// NamePtr
	{ 15,  10, 20, 20 },	// StartStats
	//MaxStat[PC_4_ROGUE],	// MaxStats
	5,	// Block
	8,	// SubSteps
	PS_5_IDENTIFY,				// SkillIndex
	60,							// LifeBase
	20,							// ManaBase
	{ 1, 1, 200 },				// BaseDamageM
	3,							// Appearance
	{ BI_385_LIGHT_HAMMER, BI_363_SHARP_KNIFE, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING }, // StartItems
	{ S_633_ROG_13, S_654_ROG_34, S_647_ROG_27, S_655_ROG_35, S_634_ROG_14 } // Voices
	},
// 5 Gladiator
	{
	"Savage",		// NamePtr
	{ 30,  0, 10, 25 },	// StartStats
	//MaxStat[PC_5_SAVAGE],	// MaxStats
	2,	// Block
	8,// SubSteps
	PS_16_REFLECT,				// SkillIndex
	100,						// LifeBase
	0,							// ManaBase
	{ 1, 0, 100 },				// BaseDamageM
	3,							// Appearance
	{ BI_6_HATCHET,-1,-1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING }, // StartItems - new - 1 hatchet
    //{ BI_361_LIGHT_MACE, BI_396_CLUB, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING }, // StartItems - old - 2 weapons
	{ S_736_WAR_13, S_763_WAR_34, S_756_WAR_27, S_764_WAR_35, S_737_WAR_14 } // Voices
	},

	#if 0
	// --------- CLASSIC MODE ---------------------------------------------------

	// 0 Warrior classic
	{
	"Warrior",			// NamePtr
	{ 25,  5, 15, 20 },	// StartStats
	//MaxStat[PC_0_WARRIOR_CLASSIC],	// MaxStats
	10,	// Block
	8,	// SubSteps
	PS_26_ITEM_REPAIR,	// SkillIndex
	80, // LifeBase
	5, // ManaBase
	{ 1, 0, 100 },// BaseDamageMultipliers
	3,// Appearance
	{ BI_394_LIGHT_DAGGER , BI_395_BUCKLER, BI_362_LIGHT_HAMMER, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING }, // StartItems
	{ S_736_WAR_13, S_763_WAR_34, S_756_WAR_27, S_764_WAR_35, S_737_WAR_14}	// Voices
	},
	// 1 Rogue classic
	{
		"Rogue",			// NamePtr
	{ 15,  5, 30, 15 },	// StartStats
	//MaxStat[PC_1_ROGUE_CLASSIC],	// MaxStats
	1,		// Block
	8,	// SubSteps
	PS_9_INFRAVISION,			// SkillIndex
	50,							// LifeBase
	12,							// ManaBase
	{ 1, 1, 200 },				// BaseDamageM
	4,							// Appearance
	{ BI_393_SMALL_BOW, -1, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING }, // StartItems
	{ S_633_ROG_13, S_654_ROG_34, S_647_ROG_27, S_655_ROG_35, S_634_ROG_14}	// Voices
	},
	// 2 Sorcerer classic
	{ "Sorcerer",				// NamePtr
	{ 10,  30, 10, 15 },	// StartStats
	//MaxStat[PC_2_SORCERER_CLASSIC],	// MaxStats
	1,		// Block
	8,		// SubSteps
	PS_27_STAFF_RECHARGE,		// SkillIndex
	25,							// LifeBase
	60,							// ManaBase
	{ 1, 0, 100 },				// BaseDamageM
	4,							// Appearance
	{ BI_5_WAND_OF_HOLY_BOLTS, -1, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },		// StartItems
	{ S_530_SOR_13, S_551_SOR_34, S_544_SOR_27, S_552_SOR_35, S_531_SOR_14}	// Voices
	},
	#endif
};


 // setting subclass attributes: starting/max stats, etc.
FullPlayerClass FullPlayerClasses[PFC_COUNT] = {
	
// 0 Warrior
	// warriors
	{
	"Warrior",
	{25, 5, 15, 20}, // (rt) starting stat values
	{600, 100, 400, 400}, // (rt) max stat values
	{ BI_394_LIGHT_DAGGER, BI_395_BUCKLER, BI_362_LIGHT_HAMMER, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Inquisitor",
	{25, 5, 15, 20}, // (rt) starting stat values
	{550, 175, 375, 400}, // (rt) max stat values
	{ BI_394_LIGHT_DAGGER, BI_395_BUCKLER, BI_362_LIGHT_HAMMER, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Guardian",
	{25, 0, 15, 25}, // (rt) starting stat values
	{750, 50, 200, 500}, // (rt) max stat values
	{ BI_460_ESTOC, -1, BI_473_LONG_MACE, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Templar",
	{25, 5, 15, 20}, // (rt) starting stat values
	{500, 200, 400, 400}, // (rt) max stat values
	{ BI_394_LIGHT_DAGGER, BI_395_BUCKLER, BI_362_LIGHT_HAMMER, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

//archers
	{
	"Archer",
	{15, 5, 25, 20}, // (rt) starting stat values
	{250, 150, 800, 300}, // (rt) max stat values
	{ BI_393_SMALL_BOW, -1, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Scout",
	{15, 5, 30, 15}, // (rt) starting stat values
	{250, 200, 800, 250}, // (rt) max stat values
	{ BI_393_SMALL_BOW, -1, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Sharpshooter",
	{15, 5, 25, 20}, // (rt) starting stat values
	{300, 200, 700, 300}, // (rt) max stat values
	{ BI_393_SMALL_BOW, -1, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Trapper",
	{15, 15, 20, 15}, // (rt) starting stat values
	{200, 400, 550, 350}, // (rt) max stat values
	{ BI_1047_ARROW_TRAP_I, BI_395_BUCKLER, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

//mages
	{
	"Mage",
	{10, 30, 10, 15}, // (rt) starting stat values
	{150, 900, 150, 300}, // (rt) max stat values
	{ BI_5_WAND_OF_HOLY_BOLTS, BI_1115_ORB, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Elementalist",
	{10, 30, 10, 15}, // (rt) starting stat values
	{150, 900, 150, 300}, // (rt) max stat values
	{ BI_1077_SMOKED_SPHERE, BI_1115_ORB, -1, BI_261_POTION_OF_MANA, BI_261_POTION_OF_MANA },
	},

	{
	"Demonologist",
	{10, 30, 10, 15}, // (rt) starting stat values
	{150, 900, 150, 300}, // (rt) max stat values
	{ BI_1074_HATRED_EFFIGY, BI_535_CORPSE_HEAD, -1, BI_192_HOLY_WATER, BI_192_HOLY_WATER },
	},

	{
	"Necromancer",
	{10, 30, 10, 15}, // (rt) starting stat values
	{150, 900, 150, 300}, // (rt) max stat values
	{ BI_1076_BONE_WAND, BI_595_CRANIUM_SHIELD, -1, BI_192_HOLY_WATER, BI_192_HOLY_WATER },
	},

	{
	"Beastmaster",
	{10, 30, 10, 15}, // (rt) starting stat values
	{150, 900, 150, 300}, // (rt) max stat values
	{ BI_1082_CEREMONIAL_DAGGER, BI_1006_QUASIT_HEART, -1, BI_192_HOLY_WATER, BI_192_HOLY_WATER },
	},
	
	{
	"Warlock",
	{10, 30, 10, 15}, // (rt) starting stat values
	{150, 900, 150, 300}, // (rt) max stat values
	{ BI_1075_LIGHT_ROD, BI_814_BOOKLET_OF_BLACK_MAGIC, -1, BI_261_POTION_OF_MANA, BI_261_POTION_OF_MANA },
	},

//MONX
	{
	"Monk",
	{15, 5, 25, 20}, // (rt) starting stat values
	{400, 100, 600, 400}, // (rt) max stat values
	{ BI_392_STAFF, -1, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Kensei",
	{20, 0, 25, 20}, // (rt) starting stat values
	{350, 100, 700, 350}, // (rt) max stat values
	{ BI_931_LIGHT_KANABO, -1, BI_912_TANTO, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Shugoki",
	{20, 0, 25, 20}, // (rt) starting stat values
	{500, 50, 450, 500}, // (rt) max stat values
	{ BI_485_LONG_HATCHET, -1, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Shinobi",
	{20, 0, 25, 20}, // (rt) starting stat values
	{250, 150, 750, 350}, // (rt) max stat values
	{ BI_931_LIGHT_KANABO, BI_1094_ROUNDED_SHIELD, BI_912_TANTO, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

//ROGUES
	{
	"Rogue",
	{15, 10, 20, 20}, // (rt) starting stat values
	{300, 400, 500, 300}, // (rt) max stat values
	{ BI_385_LIGHT_HAMMER, BI_363_SHARP_KNIFE, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Assassin",
	{15, 10, 20, 20}, // (rt) starting stat values
	{350, 200, 600, 350}, // (rt) max stat values
	{ BI_1036_CLAW_I , BI_395_BUCKLER, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Iron maiden",
	{15, 10, 20, 20}, // (rt) starting stat values
	{300, 150, 550, 500}, // (rt) max stat values
	{ BI_385_LIGHT_HAMMER, BI_506_THORNED_BUNDLE, BI_363_SHARP_KNIFE, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Bombardier",
	{15, 15, 20, 15}, // (rt) starting stat values
	{300, 450, 500, 250}, // (rt) max stat values
	{ BI_771_TOXIC_VIALS, BI_1103_LWS, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Gladiatrix",
	{20, 5, 20, 20}, // (rt) starting stat values
	{450, 50, 550, 450}, // (rt) max stat values
	{ BI_485_LONG_HATCHET, -1, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

//SAVAGES
	{
	"Savage",
	{30, 0, 10, 25}, // (rt) starting stat values
	{800, 50, 200, 450}, // (rt) max stat values
	{ BI_473_LONG_MACE, -1, BI_460_ESTOC, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Berserker",
	{30, 0, 10, 25}, // (rt) starting stat values
	{650, 50, 300, 500}, // (rt) max stat values
	{ BI_6_HATCHET, -1, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Executioner",
	{30, 0, 10, 25}, // (rt) starting stat values
	{900, 25, 25, 550}, // (rt) max stat values
	{ BI_6_HATCHET, -1, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Thraex",
	{20, 0, 20, 25}, // (rt) starting stat values
	{550, 150, 400, 400}, // (rt) max stat values // 165  165, 180 160
	{ BI_394_LIGHT_DAGGER , BI_395_BUCKLER, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Murmillo",
	{25, 0, 10, 30}, // (rt) starting stat values
	{700, 50, 250, 500}, // (rt) max stat values
	{ BI_394_LIGHT_DAGGER , BI_395_BUCKLER, BI_362_LIGHT_HAMMER, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Dimachaerus",
	{25, 0, 20, 20}, // (rt) starting stat values
	{450, 100, 550, 400}, // (rt) max stat values
	{ BI_361_LIGHT_MACE, BI_394_LIGHT_DAGGER, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	{
	"Secutor",
	{25, 0, 15, 25}, // (rt) starting stat values
	{550, 100, 400, 450}, // (rt) max stat values
	{ BI_394_LIGHT_DAGGER , BI_395_BUCKLER, BI_362_LIGHT_HAMMER, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},
	
	{
	"Druid",
	{20, 10, 15, 20}, // (rt) starting stat values
	{400, 350, 350, 400}, // (rt) max stat values
	{ 2509 , -1, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},

	#if 0
	// --------- CLASSIC MODE ---------------------------------------------------

	// 0 Warrior classic
	{
	"Warrior",
	{25, 5, 15, 20}, // (rt) starting stat values
	{600, 100, 400, 400}, // (rt) max stat values
	{ BI_394_LIGHT_DAGGER, BI_395_BUCKLER, BI_362_LIGHT_HAMMER, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},
	// 1 Rogue classic
	{
	"Rogue",
	{15, 5, 25, 20}, // (rt) starting stat values
	{250, 150, 800, 300}, // (rt) max stat values
	{ BI_393_SMALL_BOW, -1, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},
	// 2 Sorcerer classic
	{
	"Sorcerer",
	{10, 30, 10, 15}, // (rt) starting stat values
	{150, 900, 150, 300}, // (rt) max stat values
	{ BI_5_WAND_OF_HOLY_BOLTS, BI_1115_ORB, -1, BI_397_POTION_OF_HEALING, BI_397_POTION_OF_HEALING },
	},
	#endif
};

// (rt) new direction function. Unlike the two old ones it's slower, 
// (rt) but without the 15-cell limit from caster to target. Plus it works with both 8 and 16 directions.
// (rt) differs slightly from the original functions at the boundaries between values
// (rt) proposed as a replacement for SelectMissileDirectionFrom16 and SelectMissileDirectionFrom8
//----- (th2) -------------------------------------------------------------
int SelectMissileDirection(int casterRow, int casterCol, int targetRow, int targetCol, int framesCount)
{
	int deltaRow = targetRow - casterRow;
	int deltaCol = targetCol - casterCol;
	float angle = 0;
	if( deltaRow == 0 ){
		angle = deltaCol >= 0 ? 90.0f : 270.0f;
	}else{
		float range = sqrt((float)deltaRow * deltaRow + (float)deltaCol * deltaCol);
		float angleSin = float(deltaCol) / range;
		float angleCos = float(deltaRow) / range;
		float angleTan = angleSin / angleCos;
		float angle = float( atan(angleTan) * 180.0 / 3.14159 );
		if( angleCos <= 0 ){
			angle += 180.0;
		}
		if( angle < 0 ){
			angle += 360.0;
		}
	}
	angle -= 45.0;// (rt) 45-degree correction, because 0 is not at zero but at -45 degrees.
	if( angle < 0 ){
		angle += 360.0;
	}
	int direction = (int)floor(angle / 360.0 * framesCount + 0.5);
	Wrap(direction, framesCount);
	return direction;
}

#pragma region item_optimization

// (rt) New function. Drawing the items on the body
//----- (th2) -------------------------------------------------------------
void DrawOnBodyItems()
{
	Player& player = Players[ CurrentPlayerIndex ];
	for( Slot& slot: Slots ){ // (rt) The number 7 may change if new items are introduced. And items must be numbered in order
		if( slot.base.slotType == SL_INV ) break;
		Item& onBodySlot = player.OnBodySlots[ slot.id ];
		if( onBodySlot.ItemCode == IC_M1_NONE ){// (rt) If there is no item in the slot, go to the next one
			continue;
		}
		ShadowItemSlot( slot.rect.Left, slot.rect.Down, slot.rect.Width, slot.rect.Heigth );// (rt) This colours the area pink
		int graphicValue = onBodySlot.GraphicValue + 12;
		char itemWidthTable = ItemWidthTable[ graphicValue ];
		char* itemsCELArray;

		// (rt) Correction. Needed to place weapons and shields smaller than 2x3 correctly in the matching 2x3 slot
		int onBodySlotXCorrection = (slot.rect.Width - ItemWidthTable[ graphicValue ]) >> 1;
		int onBodySlotYCorrection = (slot.rect.Heigth - ItemHeightTable[ graphicValue ]) >> 1;
		// (rt) These are the item's coordinates with the correction already applied
		int leftItemPosition = slot.rect.Left + onBodySlotXCorrection;
		int downItemPosition = slot.rect.Down - onBodySlotYCorrection;

		const int overlayGfx = graphicValue;	// (th4) GetItemSprites() mutates it
		itemsCELArray = GetItemSprites( graphicValue );

		int colorOfBorder = -1;
		if( Cur.InventoryCellID == slot.id ){// (rt) If the cursor is over this slot
			colorOfBorder = 197;
			if( onBodySlot.MagicLevel ){
				colorOfBorder = 181/*-75*/;
			}
			if( !onBodySlot.IsReqMet ){
				colorOfBorder = 229/*-27*/;
			}
		}
		ItemCEL_Draw( overlayGfx, leftItemPosition, downItemPosition, itemsCELArray, graphicValue, itemWidthTable, 0, 8, (int)!onBodySlot.IsReqMet | onBodySlot.mirrorImage << 1, colorOfBorder );
		ItemOverlay_Queue(overlayGfx, leftItemPosition, downItemPosition, itemWidthTable, ItemHeightTable[overlayGfx], onBodySlot.mirrorImage != 0, colorOfBorder, (int)!onBodySlot.IsReqMet | onBodySlot.mirrorImage << 1);	// (th4)

		if( slot.id == IS_LeftHand && onBodySlot.EquippedLocation == EL_2_TWO_HANDED
		// #ifdef ALLOW_TWO_HANDED_IN_ONE
		 && (player.fullClassId != PFC_SAVAGE || !HasTrait(CurrentPlayerIndex, TraitId::MonkeyGrip) || not(onBodySlot.ItemCode, IC_1_SWORD, IC_4_MACE) )
		 //#endif
		 ){// (rt) If this is a two-handed weapon in slot 4, i.e. the left hand, draw its semi-transparent copy in slot 5, i.e. the right hand 
			ShadowItemSlot( RightHand->rect.Left, RightHand->rect.Down, RightHand->rect.Width, RightHand->rect.Heigth );
			DarkLevel = 0;
			TransparentMode = 1;
			uchar* surfaceDest;
			if( itemWidthTable != 28 ){
				surfaceDest = WorkingSurface + WorkingWidth * GUI_PanelWidth + RightHand->rect.Left;  //246391;
			}else{
				surfaceDest = WorkingSurface + WorkingWidth * GUI_PanelWidth + RightHand->rect.Left + 14; //24ORIGINAL_RESOLUTION_WIDTH5;
			}
			CEL_DrawShadow( surfaceDest, itemsCELArray, graphicValue, itemWidthTable, 0, 8, onBodySlot.mirrorImage << 1 );
			TransparentMode = 0;
		}
	}
}

// (rt) New function for drawing inventory items
//----- (th2) -------------------------------------------------------------
void DrawInventoryItems()// (rt) Similar to the previous one in many ways, but can't be merged yet. Too many differences in representation
{
	Player& player = Players[ CurrentPlayerIndex ];
	// (rt) Inventory
	for( int i = 0; i < IS_70_40_Inv_Count; i++ ){// (rt) Walk all cells and mark the used ones pink
		if( player.InvUsed[ i ] ){
			//ShadowItemSlot(ItemCellPos[25 + i].X + InventoryIndentX, ItemCellPos[25 + i].Y + InventoryIndentY, cellSize, cellSize);
			ShadowItemSlot( Inv->rect.Left + CELL_SIZE * (i % InvCols), Inv->rect.Down - (InvRows - 1 - (int) (i / InvCols)) * CELL_SIZE, CELL_SIZE, CELL_SIZE );
		}
	}

	for( int i = 0; i < IS_70_40_Inv_Count; i++ ){
		int invItemIndex = (char) player.InvUsed[ i ];
		if( invItemIndex <= 0 ){// (rt) If this slot is not marked as having a picture
			continue;
		}
		Item& inventorySlot = player.InventorySlots[ invItemIndex - 1 ];
		int graphicValue = inventorySlot.GraphicValue + 12;
		char itemWidthTable = ItemWidthTable[ graphicValue ];
		char* itemsCELArray;
		// (rt) The coordinates could be taken from an array, but computing them here is handier. 10 - inventory width, 3 - height minus 1 
		int leftItemPosition = Inv->rect.Left + CELL_SIZE * (i % InvCols);
		int downItemPosition = Inv->rect.Down - (InvRows - 1 - (int) (i / InvCols)) * CELL_SIZE;

		const int overlayGfx = graphicValue;	// (th4) GetItemSprites() mutates it
		itemsCELArray = GetItemSprites( graphicValue );

		int colorOfBorder = -1;
		if( Cur.InventoryCellID == invItemIndex - 1 + IS_Inventory ){ // (rt) Pick the outline colour
			colorOfBorder = 197;
			if( inventorySlot.MagicLevel ){
				colorOfBorder = 181/*-75*/;
			}
			if( !inventorySlot.IsReqMet ){
				colorOfBorder = 229/*-27*/;
			}
		}
		ItemCEL_Draw( overlayGfx, leftItemPosition, downItemPosition, itemsCELArray, graphicValue, itemWidthTable, 0, 8, (int)!inventorySlot.IsReqMet | inventorySlot.mirrorImage << 1, colorOfBorder );
		ItemOverlay_Queue(overlayGfx, leftItemPosition, downItemPosition, itemWidthTable, ItemHeightTable[overlayGfx], inventorySlot.mirrorImage != 0, colorOfBorder, (int)!inventorySlot.IsReqMet | inventorySlot.mirrorImage << 1);	// (th4)
	}
}

#pragma endregion item_optimization

//----- (th2) -------------------------------------------------------------
bool __fastcall MoveInOrientOrAboveNew( int monsterIndex, int orientation, uint orientIndent )
{
	if( !orientIndent ){// (rt) If 0, return the check of the main direction
		return Move_in_the_Direction_If_allowed( monsterIndex, orientation );
	}else if( MoveInOrientOrAboveNew( monsterIndex, orientation, orientIndent - 1 ) ){// (rt) If not, check the directions next to the main one
		return true;// (rt) If any of them fits, return it
	}

	int newOrientation;
	if( RNG(2) ){// (rt) Randomly choose which side's turn is checked first
		if(Move_in_the_Direction_If_allowed( monsterIndex, orientation - orientIndent ) ){
			return true;
		}
		newOrientation = orientation + orientIndent;
	}else{
		if(Move_in_the_Direction_If_allowed( monsterIndex, orientation + orientIndent ) ){
			return true;
		}
		newOrientation = orientation - orientIndent;
	}
	if(Move_in_the_Direction_If_allowed( monsterIndex, newOrientation ) ){// (rt) check the turn to the other side if the first chosen one isn't possible
		return true;
	}
	return false;
}

//----- (th2) -------------------------------------------------------------
Item* findThrowableItem( int playerIndex )
{
	Player& player = Players[ playerIndex ];
	Item& leftHand = player.OnBodySlots[ IS_LeftHand ];
	Item& rightHand = player.OnBodySlots[ IS_RightHand ];
	if( leftHand.ItemCode == IC_18_FLASK  ||  leftHand.ItemCode == IC_19_TRAP ){
		return &leftHand;
	} else if( rightHand.ItemCode == IC_18_FLASK  ||  rightHand.ItemCode == IC_19_TRAP ){
		return &rightHand;
	} else{
		return nullptr;
	}

}

//----- (th2) -------------------------------------------------------------
void Ice_Age_Abyss_Jesus_mode()
{	
	int tile = 386;
	do {
		TileBlockWalking[tile++] = 0;
	} while (tile <= 496);
	TileBlockWalking[506] = 0;
	TileBlockWalking[504] = 0;
	TileBlockWalking[503] = 0;
	TileBlockWalking[500] = 0;
	TileBlockWalking[498] = 0;
	tile = 534;
	do {
		TileBlockWalking[tile++] = 0;
	} while (tile <= 537);
}

// potentialy can be used for D1 beta shrine mechanic - reverse character name
//----- (th3) -------------------------------------------------------------
void ReverseNumber(int& revNumber) 
{
	int changer = 0;
	while (revNumber != 0) {
		changer += revNumber % 10;
		revNumber /= 10;
		if (revNumber != 0) {
			changer *= 10;
		}
	}
	revNumber = changer;
}
