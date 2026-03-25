#include "stdafx.h"

/*
Transmute item service at Cain. Something akin to Horadric Cube, but it costs gold. Available in modes where you can get gems & crafting.
Recipes:
1) make white item magic: 100 x CLVL gold cost, adds affix(es) of qlvl (CLVL/2  to  CLVL x 1.2 + 5) (non-cursed). 25% to roll 2 affixes, no extra ingredients
2) re-roll magic item's affix(es). same thing but you put in a magic item instead of white. 100 x CLVL gold
3) 2 potions of healing, 150 gold cost = full healing potion
4) 2 potions of mana, 150 gold cost = full mana potion
5) 2 potions of holy, 200 gold cost = full holy potion
6) some sort of elixir transmutation (although it's a limited use (til you hit MSL))
7) 2 similar grade repair oils = next grade repair oil (gold cost: 10 x grade x grade)
8) [?] magic / rare / enchanted (if without sockets) item + diamond (any grade) = affixes removed, item turns white, quenching lost too. gold cost: 100 + (CLVL x 10)
9) to be continued... (looking for ideas btw)

для сервиса трансмута должно открываться то же окно что и для опознания
в этом окне вместо 2-х кнопок опознания (внизу слева) должна быть одна.
В тултипе кнопки, если в окне нет предметов, написано: "Place item(s) to transmute" (подходящие рецептурно вещи выдадут иную надпись, об этом позже),
id кнопки = 3 (не нажато), 4 (нажато). Из того же списка кнопок что и используются в торговле. (tradewindowbutn.cel)
если нажатие на кнопку трансмута не выдает эффекта, то используется звук 1218
если трансмутация свершилась, проигрывается звук 1808 (добавил его в ветке - обновлять дату не придется)
--------------------------------------------------------------------------------------------------------------------------------
Сообщения для тултипа, если айтемы в окне подходят для рецепта:
один белый предмет в окне. и у игрока достаточно золота по формуле рецепта:"transmute into a random magic item"
один магический (именно синий. не рарный. не уник. не сокетный. не энчант. не сетовый) предмет"re-roll affixes"
2 хилки:"transmute into potion of full healing"
2 маны:"transmute into potion of full mana"
2 холика:"transmute into full holy water potion"

Сообщения для инфо- окна.
Если ничего нет на станке, то выводится сообщение "Place item(s) to transmute"
Если на станке предметы, которые нелья затрансмутить, то пишется "transmute impossible"
В прочих случаях выводится то, что написано в условиях сообщений для тултипа (подходящие рецепты. + в нижней строке так же указана стоимость в золоте)
В любом случае, на кнопке написано просто "Transmute". (если трансмут невозможен, то красным цветом).

добавил в дату вот такой интерфейс
находится по адресу data\\transmute\\tmpanel.cel
есть вторая версия: data\\transmute\\tmpanel_v2.cel
*/

bool IsTransmutePanelVisible;
DisplayObject TransmutePanel;
DisplayObject TransmuteButton;
DisplayObject TransmuteLeaveButton;
char TransmuteString[256];
bool IsTransmutePressed;
char* TransmutePanelCEL = nullptr;

CellSlots<5, 10> TransmuteSlots;

enum TRANSMUTE_RECIPE
{
	TR_NO             = 0,
	TR_RING          , // 2 magic rings of the same specialization + Heliodor = 1 magic amulet. price: CLVL * 50. qlvl of affixes : like in recipe # 1.
	TR_AMULET        , // 2 magic amulets of the same specialization + Heliodor = 1 magic ring. Resulting item must be of the same Specialization too.
	TR_MAGIC_REROLL  , // re-roll magic item's affix(es). same thing but you put in a magic item instead of white. 100 x CLVL gold
	TR_FULL_HEALING  , // 2 potions of healing, 150 gold cost = full healing potion
	TR_FULL_MANA     , // 2 potions of mana, 150 gold cost = full mana potion
	TR_FULL_HOLY     , // 2 potions of holy, 200 gold cost = full holy potion
	TR_ELIXIR        , // spectral elixir (not the quest one!) + 1 tourmaline = 4 random single stat elixirs. gold cost: 1000 
	TR_UPGRADE_OIL    , // 2 similar grade repair oils = next grade repair oil (gold cost: 10 x grade x grade)
	TR_MAGIC_TO_WHITE, // magic / rare / enchanted (if without sockets) item + diamond (any grade) = affixes removed, item turns white. gold cost: 100 + (CLVL x 10)
	TR_ADD_3_SOCKET  , // equippable (non-socketed && non-jewelry) base item + 1 aquamarine = that base item with 1 socket. 3 aquamarines = 2 sockets,
	TR_ADD_2_SOCKET  , // 5 aquamarines = 3 sockets (available if CanGrind() is true). info reads: bore (a socket / 2 sockets / 3 sockets) in this item
	TR_ADD_1_SOCKET  ,
	TR_WHITE_TO_MAGIC, // make white item magic: 100 x CLVL gold cost, adds affix(es) of qlvl (CLVL/2  to  CLVL x 1.2 + 5) (non-cursed). 25% to roll 2 affixes, no extra ingredients
	TR_GEM           , // 2 (same grade and type) gems give 1 same grade gem of another random type. price is: (gem grade  * gem grade  * gem grade * 100). info reads: make a different type gem of the same grade
	TR_BOOK          , // 2 books + Morganite give 1 other random book: qlvls of books are summed, and divided by 2. this is level X. the result: you get other type of book, of level in range from X-3 to X. Price is: 100 + CLVL x 10

	TR_count
};

// TODO: may be change this block to vector<AvailableTransmute> AvailableTransmutes
//       for output all available recipes list and user selection (D3-like)
bitset<TR_count> AvailableRecipes;
TRANSMUTE_RECIPE AvailableRecipe;
int AvailableTransmuteCount;

int TransmuteFullPrice;
bool TransmuteDifferentAtOnce = false;
int RecipeListPos = 1, RecipeListNext = 0;
vector<int> RecipeListPrev;

typedef int (*ItemReq)(Item& item, Player& player, vector<int>& transmuteIndexes);
#define IR(code) [](Item& item, Player& player, vector<int>& transmuteIndexes)->int{ code }

typedef bool (*ItemEffect)(vector<int>& itemIndexes, Player& player);

struct Recipe
{
	TRANSMUTE_RECIPE    id;
	vector<const char*> info;
	vector<ItemReq>     reqs;
	ItemReq             price;
	ItemEffect          effect;
	bool                once;
	i64					gcForbidMask;
};

template<TRANSMUTE_RECIPE> bool Do(vector<int>& itemIndexes, Player& player);

Recipe Recipes[TR_count] = {
	{ TR_NO            , { "Transmute impossible" }, {}, 0, 0 },
	{ TR_RING          , { "2 magic rings of same", "specialization + Heliodor", "To new magic amulet" },
	                     { IR( return is(item.ItemCode, IC_12_RING) && item.MagicLevel == ML_1_MAGIC; ),
						   IR( return item.baseItemIndex == BI_299_HELIODOR; ),
						   IR( Item& _2 = TransmuteSlots.items[transmuteIndexes[0]];
						       return is(item.ItemCode, IC_12_RING) && item.MagicLevel == ML_1_MAGIC
						              && BaseItems[item.baseItemIndex].Specialization == BaseItems[_2.baseItemIndex].Specialization; ) },
						 IR( return 50 * player.CharLevel; ),
						 Do<TR_RING>, false, BIT(GC_14_CLEAN_HANDS) },
	{ TR_AMULET        , { "2 magic amulets of same", "specialization + Heliodor", "To new magic ring" },
	                     { IR( return is(item.ItemCode, IC_13_AMULET) && item.MagicLevel == ML_1_MAGIC; ),
						   IR( return item.baseItemIndex == BI_299_HELIODOR; ),
						   IR( Item& _2 = TransmuteSlots.items[transmuteIndexes[0]];
						       return is(item.ItemCode, IC_13_AMULET) && item.MagicLevel == ML_1_MAGIC
						              && BaseItems[item.baseItemIndex].Specialization == BaseItems[_2.baseItemIndex].Specialization; ) },
						 IR( return 50 * player.CharLevel; ),
						 Do<TR_AMULET>, false, BIT(GC_14_CLEAN_HANDS) },
	{ TR_MAGIC_REROLL  , { "Magic ring or amulet", "To new magic item" },
	                     { IR( return is(item.ItemCode, IC_12_RING, IC_13_AMULET) && item.MagicLevel == ML_1_MAGIC; ) },
						 IR( return 100 * player.CharLevel; ),
						 Do<TR_MAGIC_REROLL>, true, BIT(GC_14_CLEAN_HANDS) },
	{ TR_FULL_HEALING  , { "2 healing potions", "To full healing potion" },
	                     { IR( return item.MagicCode == MC_3_POTION_OF_HEALING; ),
						   IR( return item.MagicCode == MC_3_POTION_OF_HEALING; ) },
						 IR( return 150; ),
						 Do<TR_FULL_HEALING>, false },
	{ TR_FULL_MANA     , { "2 mana potions", "To full mana potion" },
	                     { IR( return item.MagicCode == MC_6_POTION_OF_MANA; ),
						   IR( return item.MagicCode == MC_6_POTION_OF_MANA; ) },
						 IR( return 150; ),
						 Do<TR_FULL_MANA>, false },
	{ TR_FULL_HOLY     , { "2 holy potions", "To full holy potion" },
	                     { IR( return item.MagicCode == MC_18_POTION_OF_REJUVENATION; ),
						   IR( return item.MagicCode == MC_18_POTION_OF_REJUVENATION; ) },
						 IR( return 200; ),
						 Do<TR_FULL_HOLY>, false },
	{ TR_ELIXIR        , { "Spectral elixir + tourmaline", "To 4 random stat elixirs" },
	                     { IR( return item.MagicCode == MC_44_SPECTRAL_ELIXIR; ),
                           IR( return item.baseItemIndex == BI_298_TOURMALINE; ) },
						 IR( return 1000; ),
						 Do<TR_ELIXIR>, false },
	{ TR_UPGRADE_OIL   , { "2 repair oils of same grade", "to repair oil of next grade" },
	                     { IR( return item.subType == IST_OIL_REPAIR && item.grade <= 6; ),
						   IR( return item.subType == IST_OIL_REPAIR && item.grade == TransmuteSlots.items[transmuteIndexes[0]].grade; ) },
						 IR( return 10 * item.grade * item.grade; ),
						 Do<TR_UPGRADE_OIL>, true },
	{ TR_MAGIC_TO_WHITE, { "Magic, Rare or Enchanted item", "To base item" },
	                     { IR( return not(item.ItemCode, IC_12_RING, IC_13_AMULET, IC_0_OTHER)
							          && (item.MagicLevel == ML_1_MAGIC || item.dropType & (D_ENCHANT|D_ENCHANT_V1|D_RARE)); ) },
						 IR( return 100 + player.CharLevel * 10; ),
						 Do<TR_MAGIC_TO_WHITE>, false },
	{ TR_ADD_3_SOCKET  , { "Base item + 5 aquamarines", "To 3 socketed item" },
	                     { IR( return not(item.ItemCode, IC_12_RING, IC_13_AMULET, IC_0_OTHER) && CanGrind() 
						               && item.MagicLevel == ML_0_USUAL && !(item.dropType & (D_ENCHANT|D_ENCHANT_V1)); ),
						   IR( return item.baseItemIndex == BI_301_AQUAMARINE; ),
						   IR( return item.baseItemIndex == BI_301_AQUAMARINE; ),
						   IR( return item.baseItemIndex == BI_301_AQUAMARINE; ),
						   IR( return item.baseItemIndex == BI_301_AQUAMARINE; ),
						   IR( return item.baseItemIndex == BI_301_AQUAMARINE; ) },
						 IR( return 600; ),
						 Do<TR_ADD_3_SOCKET>, false },
	{ TR_ADD_2_SOCKET  , { "Base item + 3 aquamarines", "To 2 socketed item" },
	                     { IR( return not(item.ItemCode, IC_12_RING, IC_13_AMULET, IC_0_OTHER) && CanGrind() 
						               && item.MagicLevel == ML_0_USUAL && !(item.dropType & (D_ENCHANT|D_ENCHANT_V1)); ),
						   IR( return item.baseItemIndex == BI_301_AQUAMARINE; ),
						   IR( return item.baseItemIndex == BI_301_AQUAMARINE; ),
						   IR( return item.baseItemIndex == BI_301_AQUAMARINE; ) },
						 IR( return 300; ),
						 Do<TR_ADD_2_SOCKET>, false },
	{ TR_ADD_1_SOCKET  , { "Base item + aquamarine", "To 1 socketed item" },
	                     { IR( return not(item.ItemCode, IC_12_RING, IC_13_AMULET, IC_0_OTHER) && CanGrind() 
						               && item.MagicLevel == ML_0_USUAL && !(item.dropType & (D_ENCHANT|D_ENCHANT_V1)); ),
						   IR( return item.baseItemIndex == BI_301_AQUAMARINE; ) },
						 IR( return 100; ),
						 Do<TR_ADD_1_SOCKET>, false },
	{ TR_WHITE_TO_MAGIC, { "Base item", "To magic item" },
	                     { IR( return not(item.ItemCode, IC_12_RING, IC_13_AMULET, IC_0_OTHER) && item.socketsAmount == 0
							           && item.MagicLevel == ML_0_USUAL && !(item.dropType & (D_ENCHANT|D_ENCHANT_V1)); ) },
						 IR( return 100 * player.CharLevel; ),
						 Do<TR_WHITE_TO_MAGIC>, false, BIT(GC_14_CLEAN_HANDS) },
	{ TR_GEM           , { "2 same type & grade gems", "To a different random gem", "of the same grade" },
	                     { IR( return item.MagicCode == MC_GEM; ),
						   IR( Item& _2 = TransmuteSlots.items[transmuteIndexes[0]];
						       return item.MagicCode == MC_GEM
								   && item.subType == _2.subType && item.grade == _2.grade; ) },
						 IR( return item.grade * item.grade * item.grade * 100; ),
						 Do<TR_GEM>, true },
	{ TR_BOOK          , { "2 books + Morganite", "To other random book", "of average level" },
	                     { IR( return item.MagicCode == MC_24_BOOKS; ),
						   IR( return item.MagicCode == MC_24_BOOKS; ),
						   IR( return item.baseItemIndex == BI_300_MORGANITE; ) },
						 IR( return 100 + player.CharLevel * 10; ),
						 Do<TR_BOOK>, true },
};


//----- (th3) -------------------------------------------------------------
void TransmuteRecipeListDown()
{
	if( RecipeListPos < TR_count - 1 && RecipeListNext < TR_count ){
		RecipeListPrev.push_back(RecipeListPos);
		RecipeListPos = RecipeListNext;
	}
}

//----- (th3) -------------------------------------------------------------
void TransmuteRecipeListUp()
{
	if( RecipeListPos > 1 ){
		RecipeListPos = RecipeListPrev.back();
		RecipeListPrev.pop_back();
	}
}

//----- (th3) -------------------------------------------------------------
void Transmute_DrawRecipeInfo()
{
	Player& player = Players[CurrentPlayerIndex];
	int x = TransmutePanel.Left + 20;
	int xend = TransmutePanel.Right - 20;
	int y = TransmutePanel.Top + 210;
	int yend = TransmutePanel.Down - 40;
	
	DrawText(x, y, xend, "Transmute", C_3_Gold);
	if( ! TransmuteSlots.count ){
		if( TransmuteShowRecipeList ){
			y += 17 + 8;
			for( int i = RecipeListPos; i < TR_count && y + 17 * (int)Recipes[i].info.size() < yend; RecipeListNext = ++i ){
				auto& info = Recipes[i].info;
				if( info.size() ){
					sprintf(TransmuteString, "\x87%i.\x87 %s", i, info[0]);
					DrawTextColored(x, y, xend, TransmuteString, C_0_White);
					y += 17;
					for( uint s = 1; s < info.size(); ++s ){
						DrawText(x, y, xend, info[s], C_0_White);
						y += 17;
					}
					y += 8;
				}
			}
		}else{
			y += 17 * 2;
			DrawText(x, y, xend, "Place item(s) to transmute", C_0_White);
		}
	}else{
		y += 17 * 2;
		for(auto& s: Recipes[AvailableRecipe].info){
			DrawText(x, y, xend, s, C_0_White);
			y += 17;
		}
		if( AvailableRecipe ){
			y += 17;
			sprintf(TransmuteString, "Cost: %i", TransmuteFullPrice);
			DrawText(x, y, xend, TransmuteString, TransmuteFullPrice > player.TotalGold ? C_2_Red : C_1_Blue);
		}
	}
}

//----- (th3) -------------------------------------------------------------
void Transmute_UpdateAvailableRecipes()
{
	AvailableRecipes.reset();
	AvailableRecipe = TR_NO;
	AvailableTransmuteCount = 0;
	TransmuteFullPrice = 0;

	Player& player = Players[CurrentPlayerIndex];
	vector<int> itemIndexes;      itemIndexes     .reserve(TransmuteSlots.count);
	vector<int> afterItemIndexes; afterItemIndexes.reserve(TransmuteSlots.count);
	vector<int> transmuteIndexes; transmuteIndexes.reserve(TransmuteSlots.count);

	for( int i = 0; i < TransmuteSlots.count; ++i) itemIndexes.push_back(i);

	for(int recipeIndex = 1; recipeIndex < TR_count && itemIndexes.size(); ++recipeIndex){
		Recipe& recipe = Recipes[recipeIndex];
		if( recipe.effect && recipe.reqs.size() > 0 && ! (recipe.gcForbidMask & player.gameChanger) ){
			transmuteIndexes.clear();
			afterItemIndexes = itemIndexes;
			vector<ItemReq> reqs = recipe.reqs;
			bool reqFound;
			bool transmuteReady;
			do{
				reqFound = false;
				transmuteReady = false;
				for( uint a = 0; a < afterItemIndexes.size(); ++a ){
					int& i = afterItemIndexes[a];
					Item& item = TransmuteSlots.items[i];

					if( reqs[0] && reqs[0](item, player, transmuteIndexes) ){

						transmuteIndexes.push_back(i);
						i = afterItemIndexes.back();
						afterItemIndexes.pop_back();
						reqs.erase(reqs.begin());
						reqFound = true;

						if( ! reqs.size() ){
							reqs = recipe.reqs;
							transmuteReady = true;

							TransmuteFullPrice += BuyPrice(recipe.price(item, player, transmuteIndexes));

							// delete matched recipe ingredients (recipe effect emulation)
							for(auto t: transmuteIndexes) itemIndexes.erase(find(itemIndexes.begin(), itemIndexes.end(), t));
							afterItemIndexes = itemIndexes;

							AvailableRecipe = recipe.id;
							AvailableRecipes.set(AvailableRecipe - 1);
							AvailableTransmuteCount++;

							transmuteIndexes.clear();
						}
						break;
					}
				}
			}while( (transmuteReady || reqFound ) && afterItemIndexes.size() > 0 );
		}
	}
	if( AvailableRecipes.count() != 1 && ! TransmuteDifferentAtOnce ){
		AvailableRecipe = TR_NO;
	}
	if( itemIndexes.size() && ! TransmuteAllowExtra || AvailableTransmuteCount > 1 && ( ! TransmuteAllowMultiple || Recipes[AvailableRecipe].once) ){
		AvailableRecipes.reset();
		AvailableRecipe = TR_NO;
	}
}

//----- (th3) -------------------------------------------------------------
void Transmute_Start()
{
	Player& player = Players[CurrentPlayerIndex];
	bool anyTransmuted = false;
	if( AvailableRecipes.count() == 1 || TransmuteDifferentAtOnce ){
		vector<int> itemIndexes;      itemIndexes     .reserve(TransmuteSlots.count);
		vector<int> afterItemIndexes; afterItemIndexes.reserve(TransmuteSlots.count);
		vector<int> transmuteIndexes; transmuteIndexes.reserve(TransmuteSlots.count);

		for( int i = 0; i < TransmuteSlots.count; ++i)itemIndexes.push_back(i);
		
		int transmuteSound = S_1811_TRANSMUTE_SFX;
		int recipeBegin = TransmuteDifferentAtOnce ?        1 : AvailableRecipe;
		int recipeEnd   = TransmuteDifferentAtOnce ? TR_count : AvailableRecipe + 1;
		for(int recipeIndex = recipeBegin; recipeIndex < recipeEnd && itemIndexes.size(); ++recipeIndex){
			Recipe& recipe = Recipes[recipeIndex];
			if( recipe.effect && recipe.reqs.size() > 0 && ! (recipe.gcForbidMask & player.gameChanger) ){
				transmuteIndexes.clear();
				afterItemIndexes = itemIndexes;
				vector<ItemReq> reqs = recipe.reqs;
				bool reqFound;
				bool transmuteReady;
				do{
					reqFound = false;
					transmuteReady = false;
					for( uint a = 0; a < afterItemIndexes.size(); ++a ){
						int& i = afterItemIndexes[a];
						Item& item = TransmuteSlots.items[i];
						
						if( reqs[0] && reqs[0](item, player, transmuteIndexes) ){

							transmuteIndexes.push_back(i);
							i = afterItemIndexes.back();
							afterItemIndexes.pop_back();
							reqs.erase(reqs.begin());
							reqFound = true;

							if( ! reqs.size() ){
								reqs = recipe.reqs;
								transmuteReady = true;

								itemIndexes = afterItemIndexes;

								int price = BuyPrice(recipe.price(item, player, transmuteIndexes));
								if( price <= player.TotalGold ){
									if( ! recipe.effect(transmuteIndexes, player) ) goto once;

									anyTransmuted = true;
									PlayGlobalSound(transmuteSound);
									ClearGoldByInventoryAsPrice(price);

									if( recipe.once ) goto once;

									itemIndexes.clear();
									for( int i = 0; i < TransmuteSlots.count; ++i) itemIndexes.push_back(i);
									afterItemIndexes = itemIndexes;
								}
								transmuteIndexes.clear();
							}
							break;
						}
					}
				}while( (transmuteReady || reqFound ) && afterItemIndexes.size() > 0 );
			}
		}
		once:
		Transmute_UpdateAvailableRecipes();
	}
	if( ! anyTransmuted ){
		PlayGlobalSound(S_1218);
	}
}

//----- (th3) -------------------------------------------------------------
bool Do<TR_RING>(vector<int>& itemIndexes, Player& player)
{
	Item& item  = TransmuteSlots.items[itemIndexes[0]];
	int newBase = RndTypeItems(IC_13_AMULET, MC_26_AMULETS, 1000, 0, false, BaseItems[item.baseItemIndex].Specialization);
	if( ! newBase ) return false;
	item.baseItemIndex = newBase;
	item.dropType = D_GOOD | D_BOSS | item.dropType & D_QUENCHED;
	item.quality = player.CharLevel;
	item.SetSeed(Rng64());
	item.Identified = 1;
	UpdateItem(item, false);
	sort(itemIndexes.begin() + 1, itemIndexes.end(), greater<>());
	TransmuteSlots.Clear(itemIndexes[1]);
	TransmuteSlots.Clear(itemIndexes[2]);
	return true;
}

//----- (th3) -------------------------------------------------------------
bool Do<TR_AMULET>(vector<int>& itemIndexes, Player& player)
{
	Item& item  = TransmuteSlots.items[itemIndexes[0]];
	int newBase = RndTypeItems(IC_12_RING, MC_25_RINGS, 1000, 0, false, BaseItems[item.baseItemIndex].Specialization);
	if( ! newBase ) return false;
	item.baseItemIndex = newBase;
	item.dropType = D_GOOD | D_BOSS | item.dropType & D_QUENCHED;
	item.quality = player.CharLevel;
	item.SetSeed(Rng64());
	item.Identified = 1;
	UpdateItem(item, false);
	sort(itemIndexes.begin() + 1, itemIndexes.end(), greater<>());
	TransmuteSlots.Clear(itemIndexes[1]);
	TransmuteSlots.Clear(itemIndexes[2]);
	return true;
}

//----- (th3) -------------------------------------------------------------
bool Do<TR_WHITE_TO_MAGIC>(vector<int>& itemIndexes, Player& player)
{
	Item& item = TransmuteSlots.items[itemIndexes[0]];
	item.dropType = D_GOOD | D_BOSS | item.dropType & D_QUENCHED;
	item.MagicLevel = ML_1_MAGIC;
	item.quality = player.CharLevel; // TODO: check diz out
	item.SetSeed(Rng64());
	item.Identified = 1;
	UpdateItem(item, false);
	return true;
}

//----- (th3) -------------------------------------------------------------
bool Do<TR_MAGIC_TO_WHITE>(vector<int>& itemIndexes, Player& player)
{
	Item& item = TransmuteSlots.items[itemIndexes[0]];
	item.dropType = D_0_SIMPLE | item.dropType & D_QUENCHED;
	item.MagicLevel = ML_0_USUAL;
	item.SetSeed(Rng64());
	item.Identified = 1;
	UpdateItem(item, false);
	return true;
}

//----- (th3) -------------------------------------------------------------
bool Do<TR_MAGIC_REROLL>(vector<int>& itemIndexes, Player& player)
{
	Item& item = TransmuteSlots.items[itemIndexes[0]];
	item.dropType = D_GOOD | D_BOSS | item.dropType & D_QUENCHED;
	item.MagicLevel = ML_1_MAGIC;
	item.quality = player.CharLevel;
	item.SetSeed(Rng64());
	item.Identified = 1;
	UpdateItem(item, false);
	return true;
}

//----- (th3) -------------------------------------------------------------
bool Do<TR_FULL_HEALING>(vector<int>& itemIndexes, Player& player)
{
	Item& item = TransmuteSlots.items[itemIndexes[0]];
	int newBase = RndTypeItems(IC_0_OTHER, MC_2_POTION_OF_FULL_HEALING, 1000);
	if( ! newBase ) return false;
	item.baseItemIndex = newBase;
	item.dropType = D_0_SIMPLE;
	UpdateItem(item);
	TransmuteSlots.Clear(itemIndexes[1]);
	return true;
}

//----- (th3) -------------------------------------------------------------
bool Do<TR_FULL_MANA>(vector<int>& itemIndexes, Player& player)
{
	Item& item = TransmuteSlots.items[itemIndexes[0]];
	int newBase = RndTypeItems(IC_0_OTHER, MC_7_POTION_OF_FULL_MANA, 1000);
	if( ! newBase ) return false;
	item.baseItemIndex = newBase;
	item.dropType = D_0_SIMPLE;
	UpdateItem(item);
	TransmuteSlots.Clear(itemIndexes[1]);
	return true;
}

//----- (th3) -------------------------------------------------------------
bool Do<TR_FULL_HOLY>(vector<int>& itemIndexes, Player& player)
{
	Item& item = TransmuteSlots.items[itemIndexes[0]];
	int newBase = RndTypeItems(IC_0_OTHER, MC_19_POTION_OF_FULL_REJUVENATION, 1000);
	if( ! newBase ) return false;
	item.baseItemIndex = newBase;
	item.dropType = D_0_SIMPLE;
	UpdateItem(item);
	TransmuteSlots.Clear(itemIndexes[1]);
	return true;
}

//----- (th3) -------------------------------------------------------------
bool Do<TR_ELIXIR>(vector<int>& itemIndexes, Player& player)
{
	Item& cur = player.ItemOnCursor;
	Item tmp = cur;
	Item& item = TransmuteSlots.items[itemIndexes[0]];
	int mc[4];
	for( int i = 0; i < 4; ++i ) mc[i] = MC_10_ELIXIR_OF_STRENGTH + Rng64(4);
	item.baseItemIndex = RndTypeItems(IC_0_OTHER, mc[0], 1000); // TODO: remove RNG for main seed sequence (or add option to generation funcs/struct)
	UpdateItem(item);
	cur = item;
	TransmuteSlots.Clear(itemIndexes[1]);
	for( int i = 1; i < 4; ++i ){
		cur.baseItemIndex = RndTypeItems(IC_0_OTHER, mc[i], 1000);
		cur.SetSeed(Rng64());
		UpdateItem(cur);
		if( ! Transmute_AutoPutCursorItem(CurrentPlayerIndex, true) ){ // TODO: change to player ?
			InvPutItem(CurrentPlayerIndex);
		}
	};
	cur = tmp;
	return true;
}

//----- (th3) -------------------------------------------------------------
bool Do<TR_ADD_1_SOCKET>(vector<int>& itemIndexes, Player& player)
{
	Item& item = TransmuteSlots.items[itemIndexes[0]];
	item.socketsAdded = 1;
	UpdateItem(item);
	TransmuteSlots.Clear(itemIndexes[1]);
	return true;
}

//----- (th3) -------------------------------------------------------------
bool Do<TR_ADD_2_SOCKET>(vector<int>& itemIndexes, Player& player)
{
	Item& item = TransmuteSlots.items[itemIndexes[0]];
	item.socketsAdded = 2;
	UpdateItem(item);
	sort(itemIndexes.begin() + 1, itemIndexes.end(), greater<>());
	TransmuteSlots.Clear(itemIndexes[1]);
	TransmuteSlots.Clear(itemIndexes[2]);
	TransmuteSlots.Clear(itemIndexes[3]);
	return true;
}

//----- (th3) -------------------------------------------------------------
bool Do<TR_ADD_3_SOCKET>(vector<int>& itemIndexes, Player& player)
{
	Item& item = TransmuteSlots.items[itemIndexes[0]];
	item.socketsAdded = 3;
	UpdateItem(item);
	sort(itemIndexes.begin() + 1, itemIndexes.end(), greater<>());
	TransmuteSlots.Clear(itemIndexes[1]);
	TransmuteSlots.Clear(itemIndexes[2]);
	TransmuteSlots.Clear(itemIndexes[3]);
	TransmuteSlots.Clear(itemIndexes[4]);
	TransmuteSlots.Clear(itemIndexes[5]);
	return true;
}

int OilUpgrade[] = { MC_31_OIL_OF_REPAIR_I, MC_32_OIL_OF_REPAIR_II, MC_33_OIL_OF_REPAIR_III,
	MC_34_OIL_OF_REPAIR_IV, MC_35_OIL_OF_REPAIR_V, MC_38_OIL_OF_REPAIR_VI, MC_39_OIL_OF_REPAIR_VII
};
//----- (th3) -------------------------------------------------------------
bool Do<TR_UPGRADE_OIL>(vector<int>& itemIndexes, Player& player)
{
	Item& item = TransmuteSlots.items[itemIndexes[0]];
	int newBase = RndTypeItems(IC_0_OTHER, OilUpgrade[item.grade], 1000);
	if( ! newBase ) return false;
	item.baseItemIndex = newBase;
	item.dropType = D_0_SIMPLE;
	UpdateItem(item);
	TransmuteSlots.Clear(itemIndexes[1]);
	return true;
}

//----- (th3) -------------------------------------------------------------
bool Do<TR_GEM>(vector<int>& itemIndexes, Player& player)
{
	Item& item = TransmuteSlots.items[itemIndexes[0]];
	constexpr int count = int(GEM::count) - 1;
	char gems[count];
	char gem = 0;
	for(auto& g: gems){
		if( item.subType == gem ) gem++;
		g = gem++;
	}
	int newBase = BI_1238_GEM_RUBY_1 + gems[Rng64(count)] * int(GEM::maxGrade) + (item.grade - 1);
	if( ! newBase ) return false;
	item.baseItemIndex = newBase;
	UpdateItem(item);
	TransmuteSlots.Clear(itemIndexes[1]);
	return true;
}

//----- (th3) -------------------------------------------------------------
bool Do<TR_BOOK>(vector<int>& itemIndexes, Player& player)
{
	Item& book1 = TransmuteSlots.items[itemIndexes[0]];
	Item& book2 = TransmuteSlots.items[itemIndexes[1]];
	int maxQlvl = (int)round(double(book1.quality/2 + book2.quality/2) / 2); // SetupAllItems divide qlvl by 2 for InitBooks in GetItemAttrs
	int minQlvl = maxQlvl - 3; // X - 3 to X
	LimitToMin(maxQlvl, 1);
	LimitToMin(minQlvl, 1);
	Item newBook = book1;
	int tries = 1000;
	for(; tries > 0; --tries ){
		int quality = minQlvl + Rng64(maxQlvl - minQlvl + 1);
		int newBase = RndTypeItems(IC_0_OTHER, MC_24_BOOKS, quality);
		if( ! newBase ) continue;
		newBook.baseItemIndex = newBase;
		newBook.quality = quality * 2; // SetupAllItems divide qlvl by 2 for InitBooks in GetItemAttrs
		newBook.SetSeed(Rng64());
		UpdateItem(newBook);
		if( not(newBook.SpellIndex, book1.SpellIndex, book2.SpellIndex) ) break; // success
	}
	if( ! tries ) return false;
	book1 = newBook;
	sort(itemIndexes.begin() + 1, itemIndexes.end(), greater<>());
	TransmuteSlots.Clear(itemIndexes[1]);
	TransmuteSlots.Clear(itemIndexes[2]);
	return true;
}

//----- (th3) -------------------------------------------------------------
bool Transmute_Allowed( const Item& item )
{
	return not(item.ItemCode, IC_11_GOLD) && ! IsQuestItem( item.baseItemIndex ) && item.socketsAmount == 0;
}

//----- (th3) -------------------------------------------------------------
void SetupTransmutePanel()
{
	SetupPanelRect( TransmutePanel, LeftPanel, GUI_PanelWidth, GUI_PanelHeight);
	SetupDisplayObject(TransmuteLeaveButton, TransmutePanel.Left + 278, TransmutePanel.Top + 450 - 32, 33, 32);
	SetupDisplayObject(TransmuteButton, TransmutePanel.Left + 12, TransmutePanel.Top + 450 - 32, 33, 32);
	SetupDisplayObject(TransmuteSlots.rect, TransmutePanel.Left + 17, TransmutePanel.Top + 34, (CELL_SIZE + 1) * TransmuteSlots.Cols, (CELL_SIZE + 1) * TransmuteSlots.Rows);
	RecipeListPrev.reserve(10);
}

//----- (th2) -------------------------------------------------------------
void Transmute_RecalcItemsReq()
{
	for( Item& item: TransmuteSlots.items ){
		UpdateItem( item );
		RecalcBookReq( item, CurrentPlayerIndex );
	}
}

//----- (th3) -------------------------------------------------------------
void __fastcall Transmute_Open()
{
    IsTransmutePanelVisible = true;
    IsINVPanelVisible = true;
    CurrentDialogIndex = PD_0_None;
    PlayGlobalSound(S_75_I_TITLEMOV);
	Transmute_RecalcItemsReq();
}

//----- (th3) -------------------------------------------------------------
void Transmute_RetriveAllItems()
{
	Player& player = Players[CurrentPlayerIndex];
	if( Cur.GraphicsID == CM_1_NORMAL_HAND ){
		for(int itemIndex = TransmuteSlots.count - 1; itemIndex >= 0; --itemIndex){
			player.ItemOnCursor = TransmuteSlots.items[itemIndex];
			if( AutoPutCursorToInventoryOrDrop(false) ){
				TransmuteSlots.Clear(itemIndex);
				PlayGlobalSound( S_56_I_INVGRAB );
			}else if( InvPutItem(CurrentPlayerIndex) != -1 ){
				TransmuteSlots.Clear(itemIndex);
			}else{
				SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
				TransmuteSlots.Clear(itemIndex);
				break;
			}
		}
	}
}

//----- (th3) -------------------------------------------------------------
void __fastcall Transmute_Close()
{
    if( IsTransmutePanelVisible ){
        if( ! TransmuteKeepItems ) Transmute_RetriveAllItems();
		IsTransmutePanelVisible = false;
        IsINVPanelVisible = false;
    }
}
//----- (th3) -------------------------------------------------------------
int TransmuteButtonPicNumber(bool transmute)
{
	if( transmute ){
		return CursorIntoDisplayObject(TransmuteButton) && IsTransmutePressed ? BT_TRANS_PRESS : BT_TRANS;
	}else{
		return CursorIntoDisplayObject(TransmuteLeaveButton) && IsTransmutePressed ? N_ENCH_PRESS : N_ENCH;
	}
}

//----- (th3) -------------------------------------------------------------
void __fastcall Transmute_DrawItems()
{
	extern bool InventoryBackgroundOldStyle;
	extern bool InventoryTurnOffRedColorItems;

	for( int row = 0; row < TransmuteSlots.Rows; ++row ){
		for( int col = 0; col < TransmuteSlots.Cols; ++col ){
			if (TransmuteSlots.used2d[row][col]) {
				if (InventoryBackgroundOldStyle) {
					ShadowItemSlot(TransmuteSlots.rect.Left + Screen_LeftBorder + col * (CELL_SIZE + 1), TransmuteSlots.rect.Down + Screen_TopBorder - (TransmuteSlots.Rows - 1 - row) * (CELL_SIZE + 1), CELL_SIZE, CELL_SIZE);
				} else {
					const char curItemIndex = abs(TransmuteSlots.used2d[row][col]) - 1;
					if (curItemIndex >= 0 && curItemIndex < Stash_MaxSlot) {
						const Item& curItem = TransmuteSlots.items[curItemIndex];
						ShadowItemSlot(TransmuteSlots.rect.Left + Screen_LeftBorder + col * (CELL_SIZE + 1), TransmuteSlots.rect.Down + Screen_TopBorder - (TransmuteSlots.Rows - 1 - row) * (CELL_SIZE + 1), CELL_SIZE, CELL_SIZE, GetItemBackgroundColorType(curItem));
					}
				}
			}
		}
	}

	if (Cur.GraphicsID >= CM_12_ITEMS_PICS_START && Players[CurrentPlayerIndex].ItemOnCursor.ItemCode != IC_M1_NONE) {
		ChangeItemBacklight( TransmuteSlots.rect.Left + Screen_LeftBorder, TransmuteSlots.rect.Top + Screen_TopBorder, TransmuteSlots.Cols, TransmuteSlots.Rows, -1, -1, 2);
	}

	for( int row = 0; row < TransmuteSlots.Rows; ++row ){
		for( int col = 0; col < TransmuteSlots.Cols; ++col ){
			int invItemIndex = TransmuteSlots.used2d[row][col];
			if( invItemIndex <= 0 ){
				continue;
			}
			const Item& inventorySlot = TransmuteSlots.items[invItemIndex - 1];
			int graphicValue = inventorySlot.GraphicValue + CM_12_ITEMS_PICS_START;
			char itemWidthTable = ItemWidthTable[graphicValue];

			int x = TransmuteSlots.rect.Left + Screen_LeftBorder + col * (CELL_SIZE + 1);
			int y = TransmuteSlots.rect.Down + Screen_TopBorder - (TransmuteSlots.Rows - 1 - row) * (CELL_SIZE + 1);
			if( ItemHeightTable[graphicValue] >= CELL_SIZE * 3 ) y -= 3;
			else if( ItemHeightTable[graphicValue] >= CELL_SIZE * 2 ) y -= 1;

			char* itemsCELArray = GetItemSprites(graphicValue);
			bool itemUnderCursor = Cur.InventoryCellID == invItemIndex - 1 + Stash_SlotIDStart;
			// Выбираем цвет обводки
			int colorOfBorder = -1;
			if(itemUnderCursor){
				colorOfBorder = 197;
				if( inventorySlot.MagicLevel ){
					colorOfBorder = 181;
				}
				if( !inventorySlot.IsReqMet ){
					colorOfBorder = 229;
				}
			}
			CEL_Draw(x, y, itemsCELArray, graphicValue, itemWidthTable, 0, 8, (int)!(inventorySlot.IsReqMet || InventoryTurnOffRedColorItems /*|| itemUnderCursor*/) | inventorySlot.mirrorImage << 1, colorOfBorder);
		}
	}
}

//----- (th3) -------------------------------------------------------------
void __fastcall Transmute_Draw()
{
	Surface_DrawCEL(TransmutePanel.Left + Screen_LeftBorder, TransmutePanel.Down + Screen_TopBorder, TransmutePanelCEL , 1, TransmutePanel.Width);
	Surface_DrawCEL(TransmuteButton.Left + Screen_LeftBorder, TransmuteButton.Down + Screen_TopBorder, VisualTradeBtnsCEL, TransmuteButtonPicNumber(1), 33); // transmute button
	Surface_DrawCEL(TransmuteLeaveButton.Left + Screen_LeftBorder, TransmuteLeaveButton.Down + Screen_TopBorder, VisualTradeBtnsCEL, TransmuteButtonPicNumber(0), 33); // leave button

	Transmute_DrawItems();
	Transmute_DrawRecipeInfo();

	if( 0 && CursorIntoDisplayObject(TransmuteSlots.rect) ){ // item cells position debug
		DrawRect(TransmuteSlots.rect, 18);
		int i = GetCellIndex(TransmuteSlots.rect, CursorX, CursorY);
		if( i >= 0 ){
			DisplayObject cell;
			auto [row, col] = div(i, TransmuteSlots.Cols);
			SetupDisplayObject(cell, TransmuteSlots.rect.Left + (CELL_SIZE + 1) * col, TransmuteSlots.rect.Top + (CELL_SIZE + 1) * row, CELL_SIZE + 1, CELL_SIZE + 1);
			DrawRect(cell, 18);
		}
	}
}

//----- (th3) -------------------------------------------------------------
int GetSlotMouseAbove_Transmute()
{
	int cellIndex = GetCellIndex(TransmuteSlots.rect, CursorX, CursorY);
	if( cellIndex == -1 ){
		return -1;
	}
	Item* item = nullptr;

	int itemNumber = abs(TransmuteSlots.used[cellIndex]);
	if( ! itemNumber ){
		return -1;
	}
	int itemIndex = itemNumber - 1;
	int inventoryCellIndex = itemIndex + Stash_SlotIDStart; // TODO: check why same in stash and trade
	item = &TransmuteSlots.items[itemIndex];

	if( item->ItemCode == IC_M1_NONE ){
		return -1;
	}
	if( item->ItemCode == IC_11_GOLD ){
		char* currency = formatGoldAmount(item->amount);
		char infoPanelString[260];
		sprintf(infoPanelString, "%i gold %s", item->amount, currency);
		return inventoryCellIndex;
	}
	return inventoryCellIndex;
}

//----- (th3) -------------------------------------------------------------
void __fastcall Transmute_MouseMove()
{
	MaybeNeedDrawInfoPanel = 0;
	InfoPanel_ClearBody();

	if( CursorIntoDisplayObject(TransmuteSlots.rect) ){
		Cur.InventoryCellID = GetSlotMouseAbove_Transmute();
		Tooltip_ShowItemInfo(Cur.InventoryCellID);
	}else if( CursorIntoDisplayObject(TransmuteLeaveButton)) {
		Tooltip_SetOffsetTop();
		Tooltip_Clear();
		Tooltip_AddLine("Exit");
	}else if( CursorIntoDisplayObject(TransmuteButton)) {
		Tooltip_SetOffsetTop();
		Tooltip_Clear();
		Tooltip_AddLine("Transmute");
		Tooltip_AddLine("Hotkey: T");
	}
}

//----- (th3) -------------------------------------------------------------
bool Transmute_MoveItemToInventory(int itemIndex)
{
	Player& player = Players[CurrentPlayerIndex];
	if( (uint) itemIndex >= (uint) TransmuteSlots.count ) return false;

	player.ItemOnCursor = TransmuteSlots.items[itemIndex];
	PlayGlobalSound( S_56_I_INVGRAB );
	if( AutoPutCursorToInventoryOrDrop( false ) ){
		TransmuteSlots.Clear(itemIndex);
	}else{
		SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
		TransmuteSlots.Clear(itemIndex);
	}
	Transmute_UpdateAvailableRecipes();
	return true;
}

//----- (th3) -------------------------------------------------------------
void __fastcall Transmute_TakeCursorItem(int playerIndex, int mX, int mY)
{
	Player& player = Players[playerIndex];

	if( player.CurAction > PCA_3_WALK_HORISONTAL )return;

	IsGoldSplitPanelVisible = false;

	int cellIndex = GetCellIndex(TransmuteSlots.rect, CursorX, CursorY);
	if( cellIndex == -1 ){
		return;
	}

	if( IsShiftPressed ){
		int itemIndex = TransmuteSlots.used[cellIndex];
		if( itemIndex != 0 ){
			Transmute_MoveItemToInventory( abs(itemIndex) - 1 );
			return;
		}
	}

	// тут чистая копия саба клеар инвентори слот. Только до рекалк плеер
	int takeItemIndex;

	player.ItemOnCursor.ItemCode = IC_M1_NONE;

	if( TransmuteSlots.used[cellIndex] ){
		takeItemIndex = abs(TransmuteSlots.used[cellIndex]);
		for(auto& invUsed : TransmuteSlots.used){
			if( abs(invUsed) == takeItemIndex ){
				invUsed = 0;
			}
		}
		int invItemIndex = takeItemIndex - 1;
		player.ItemOnCursor = TransmuteSlots.items[takeItemIndex - 1];
		TransmuteSlots.count--;
		if( TransmuteSlots.count > 0 && TransmuteSlots.count != (takeItemIndex - 1) ){
			memcpy(&TransmuteSlots.items[takeItemIndex - 1], &TransmuteSlots.items[TransmuteSlots.count], sizeof(Item));
			for(auto& invUsed : TransmuteSlots.used){
				if( invUsed == (TransmuteSlots.count + 1) ){
					invUsed = invItemIndex + 1;
				}
				if( invUsed == -(TransmuteSlots.count + 1) ){
					invUsed = -(invItemIndex + 1);
				}
			}
		}
	}

	if( player.ItemOnCursor.ItemCode != IC_M1_NONE ){ //  если что то подцепили на курсор
		if( player.ItemOnCursor.ItemCode == IC_11_GOLD ){
			player.TotalGold = CalcTotalGold(playerIndex);
		}
		RecalcPlayer(playerIndex, 1);
		CheckItemStats(playerIndex);
		if( playerIndex == CurrentPlayerIndex ){
			PlayGlobalSound(56);
			SetCursorGraphics(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
			FixItemCursor(-1);
		}
	}
	Transmute_UpdateAvailableRecipes();
}

//----- (th3) -------------------------------------------------------------
bool __fastcall Transmute_PutItemInStorage( int playerIndex, int aCellID, int aWidth, int aHeight, int isNotRoomTest)
{
	int countFreeInvSlots = 0;
	for(short usedFlag : TransmuteSlots.used){
		if( !usedFlag ){
			countFreeInvSlots++;
		}
	}

	if( countFreeInvSlots < aWidth * aHeight ){
		return false;
	}

	Player& player = Players[playerIndex];
	bool haveRoomForItem = true;
	int currentInvYPosition;
	int currentInvXPosition;
	currentInvYPosition = aCellID / TransmuteSlots.Cols;
	if( currentInvYPosition < 0 ){
		currentInvYPosition = 0;
	}

	for( int i = 0; haveRoomForItem; i++ ){// Вычисление есть ли место для вещи
		if( currentInvYPosition >= TransmuteSlots.Rows ){
			haveRoomForItem = false;
			return haveRoomForItem;// На нет и суда нет
		}
		currentInvXPosition = aCellID % TransmuteSlots.Cols;
		if( currentInvXPosition < 0 ){// Если отрицательный - обнуляем
			currentInvXPosition = 0;
		}
		for( int j = 0; j < aWidth; j++ ){// проходим по клеткам aWidth количество раз
			if( !haveRoomForItem ){// То есть по сути чтобы цикл вернул 1 нужно чтобы клетки инвентаря от текущей по х размер ширины были пусты.
				return haveRoomForItem;// На нет и суда нет
			}
			int inventorySlotsIndex = abs(TransmuteSlots.used[currentInvXPosition + currentInvYPosition * TransmuteSlots.Cols]);
			if( inventorySlotsIndex ){
				int graphicsIndex = TransmuteSlots.items[inventorySlotsIndex - 1].GraphicValue + CM_12_ITEMS_PICS_START;
				haveRoomForItem = false;
			}else{
				haveRoomForItem = true;
			}
			currentInvXPosition++;
		}
		currentInvYPosition++;
		if( i == aHeight - 1 ){//если есть место для вещи
			break; // При выходе haveRoomForItem будет 1, что с ним делать следующая конструкция сама решить
		}
	}
	if( !haveRoomForItem || !isNotRoomTest ){
		return haveRoomForItem;
	}
	// Собственно впихивание вещи. haveRoomForItem скорее всего будет равно 1, ибо в случаях когда не равно в предыдущем цикле сразу выход
	TransmuteSlots.items[TransmuteSlots.count] = player.ItemOnCursor;
	TransmuteSlots.count++;
	currentInvYPosition = aCellID / TransmuteSlots.Cols;

	if( currentInvYPosition < 0 ){
		currentInvYPosition = 0;
	}

	for( int i = 0; i < aHeight; i++ ){
		currentInvXPosition = aCellID % TransmuteSlots.Cols;
		if( currentInvXPosition < 0 ){
			currentInvXPosition = 0;
		}
		for( int j = 0; j < aWidth; j++ ){// проходим по клеткам aWidth количество раз
			if( j || i != aHeight - 1 ){// Обозначаем клетки как занятые вещью
				TransmuteSlots.used[currentInvXPosition + currentInvYPosition * TransmuteSlots.Cols] = -TransmuteSlots.count; // Если одна из остальных клеток
			}else{
				TransmuteSlots.used[currentInvXPosition + currentInvYPosition * TransmuteSlots.Cols] = TransmuteSlots.count; // Если клетка с картинкой
			}
			currentInvXPosition++;
		}
		currentInvYPosition++;
	}
	return haveRoomForItem;
}

//----- (th3) -------------------------------------------------------------
void __fastcall Transmute_PutCursorItem(int playerIndex, int mX, int mY)
{
	Player& player = Players[playerIndex];

	Item tempItem {};

	SetupItemSizes(player.ItemOnCursor.GraphicValue + CM_12_ITEMS_PICS_START);
	int yPos = mY + (Cur.ItemHeight / 2);
	int xPos = mX + (Cur.ItemWidth / 2);

	bool isHasGotToTheSlot = false;

	if( !(ItemWidthCells & 1) ){ // even
		xPos -= CELL_SIZE / 2; // Отступ влево на полклетки для вещей с чётной шириной. Чтобы ставились на клетки под вещьюа не правее
	}
	if( !(ItemHeightCells & 1) ){ // even
		yPos -= CELL_SIZE / 2; // Отступ вверх для вещей с чётной высотой. Для тех же целей
	}

	int cellIndex = GetCellIndex(TransmuteSlots.rect, xPos, yPos);

	if( cellIndex == -1 ){
		return;
	}

	int newCursorGraphics = 1;
	int itemForReplacing = 0;
	int currentInvYPosition;
	int currentInvXPosition;
	currentInvYPosition = cellIndex / TransmuteSlots.Cols - (ItemHeightCells - 1) / 2;
	if( currentInvYPosition < 0 ){
		currentInvYPosition = 0;
	}
	for( int i = 0; i < ItemHeightCells; i++ ){
		if( currentInvYPosition >= TransmuteSlots.Rows ){
			return;
		}
		currentInvXPosition = cellIndex % TransmuteSlots.Cols - (ItemWidthCells - 1) / 2;
		if( currentInvXPosition < 0 ){
			currentInvXPosition = 0;
		}
		for( int j = 0; j < ItemWidthCells; j++ ){
			if( currentInvXPosition >= TransmuteSlots.Cols ){
				return;
			}
			bool haveRoomForItem = !TransmuteSlots.used[currentInvXPosition + currentInvYPosition * TransmuteSlots.Cols];
			if( !haveRoomForItem ){ // Запоминаем какая вещь нам попалась под нашу вещь, если их несколько - выходим
				if( !itemForReplacing ){
					itemForReplacing = TransmuteSlots.used[currentInvXPosition + currentInvYPosition * TransmuteSlots.Cols];
					if( itemForReplacing < 0 ){
						itemForReplacing = -itemForReplacing;
					}
				}
				else if( itemForReplacing != TransmuteSlots.used[currentInvXPosition + currentInvYPosition * TransmuteSlots.Cols] && itemForReplacing != -TransmuteSlots.used[currentInvXPosition + currentInvYPosition * TransmuteSlots.Cols] ){
					return;
				}
			}
			currentInvXPosition++;
		}
		currentInvYPosition++;
	}

	if( playerIndex == CurrentPlayerIndex ){ // Если кладём текущему игроку - играем соответствующий звук
		PlayGlobalSound(Flip_InventorySound[ItemFlipTable[player.ItemOnCursor.GraphicValue]]);
	}

	int currentY;
	if( !itemForReplacing ){
		//если нечем заменять
		currentY = cellIndex / TransmuteSlots.Cols - ((ItemHeightCells - 1) / 2);
		if( currentY < 0 ){
			currentY = 0;
		}
		cellIndex = cellIndex % TransmuteSlots.Cols + TransmuteSlots.Cols * currentY;
		Transmute_PutItemInStorage( playerIndex, cellIndex, ItemWidth / CELL_SIZE, ItemHeight / CELL_SIZE, 1 );
	}
	else if( itemForReplacing ){
		//если есть чем заменять
		tempItem = TransmuteSlots.items[itemForReplacing - 1]; // Сохранили в темп заменяемую вещь
		TransmuteSlots.Clear(itemForReplacing - 1); // Удаляемая вещь становится под номером равным количеству вещей
		currentY = cellIndex / TransmuteSlots.Cols - ((ItemHeightCells - 1) / 2);
		if( currentY < 0 ){
			currentY = 0;
		}
		cellIndex = cellIndex % TransmuteSlots.Cols + TransmuteSlots.Cols * currentY;
		Transmute_PutItemInStorage( playerIndex, cellIndex, ItemWidth / CELL_SIZE, ItemHeight / CELL_SIZE, 1 ); // Сюда вещь копируется с курсора
		newCursorGraphics = SwapItemsAndGetFirstItemGraphicsValue(&tempItem, &player.ItemOnCursor); // Поменяли вещи
	}
	Transmute_UpdateAvailableRecipes();

	RecalcPlayer(playerIndex, 1);
	if( playerIndex == CurrentPlayerIndex ){
		if( newCursorGraphics == 1 ){
			// PlayGlobalSound( S_56_I_INVGRAB );
			FixItemCursor(1);
		}
		SetCursorGraphics(newCursorGraphics);
	}
}

//----- (th3) -------------------------------------------------------------
bool __fastcall Transmute_AutoPutCursorItem(int playerIndex, bool force /*= false*/) // TODO: check AutoPutCursorToInventory sync possibility
{
	Item& cur = Players[playerIndex].ItemOnCursor;
	if( ! Transmute_Allowed(cur) && ! force ) return false;

	SetupItemSizes(cur.GraphicValue + CM_12_ITEMS_PICS_START);
	vector<uchar> successionArray; // Сюда копируется приоритет места инвентаря для каждой позиции.
	successionArray.reserve(TransmuteSlots.Max * 2);

	GetSuccessionArray(TransmuteSlots.Cols, TransmuteSlots.Rows, ItemWidthCells, ItemHeightCells, successionArray);

	for( auto cellId: successionArray ){
		if( Transmute_PutItemInStorage(playerIndex, cellId, ItemWidthCells, ItemHeightCells, 1) ){
			Transmute_UpdateAvailableRecipes();
			return true;
		}
	}

	return false;
}

//----- (th3) -------------------------------------------------------------
void Transmute_LButtonDown()
{
	if( CursorIntoDisplayObject(TransmuteSlots.rect) ){
		if( Cur.GraphicsID < CM_12_ITEMS_PICS_START ){
			Transmute_TakeCursorItem(CurrentPlayerIndex, CursorX, CursorY);
		}else{
			if( Transmute_Allowed(Players[CurrentPlayerIndex].ItemOnCursor) ){
				Transmute_PutCursorItem(CurrentPlayerIndex, CursorX, CursorY);
			}
		}
	}else{
		if( CursorIntoDisplayObject(TransmuteLeaveButton) ){
			PlayGlobalSound(S_75_I_TITLEMOV);
			IsTransmutePressed = true;
		}
		if( CursorIntoDisplayObject(TransmuteButton) ){ // && TransmuteButtonPicNumber(1) != ENCH_INACTIVE ){ // inactive ?
			PlayGlobalSound(S_75_I_TITLEMOV);
			IsTransmutePressed = true;
		}
	}
}

//----- (th3) -------------------------------------------------------------
void __fastcall Transmute_MouseClick()
{
	Player& player = Players[CurrentPlayerIndex];
	if( CursorIntoDisplayObject(TransmuteLeaveButton) ){
		Transmute_Close();
	}else{
		if( CursorIntoDisplayObject(TransmuteButton) ){ // && TransmuteButtonPicNumber(1) != ENCH_INACTIVE ){
			Transmute_Start();
		}
	}
	IsTransmutePressed = false;
}

//----- (th2) -------------------------------------------------------------
void __fastcall Transmute_WriteToSave()
{
	int blockSize = sizeof(SaveBuffer_Transmute);
	uchar* buf = SaveBuffer_Transmute;
	CurSaveData = (char*)buf;

	PutNextHtonl2(SaveSignature);
	PutNextHtonl(CurSaveVersion);

	PutNextHtonl(TransmuteSlots.count);

	memcpy(CurSaveData, TransmuteSlots.used, sizeof(TransmuteSlots.used));
	CurSaveData += sizeof(TransmuteSlots.used);

	for( int itemIndex = 0; itemIndex < TransmuteSlots.count; ++itemIndex ){
		const Item& item = TransmuteSlots.items[itemIndex];
		SaveItem( (LastItemInfo*)CurSaveData, &item );
		CurSaveData += sizeof(LastItemInfo);
	}

	int actualSize = (uchar*)CurSaveData - buf;
	blockSize = CalcEncodeDstBytes(actualSize);
	EncodeFile( buf, actualSize, blockSize, CryptKey );
	MpqAddFile( SaveFile_Transmute, buf, blockSize );

	CurSaveData = nullptr;
}

//----- (th3) -------------------------------------------------------------
void Transmute_Load()
{
	auto loadItem = LoadItem<ItemInfo64>; auto itemSize = sizeof(ItemInfo64); 

	TransmuteSlots.count = GetNextHtonl();

	memcpy(TransmuteSlots.used, CurSaveData, sizeof(TransmuteSlots.used));
	CurSaveData += sizeof(TransmuteSlots.used);

	for( int itemIndex = 0; itemIndex < TransmuteSlots.count; ++itemIndex ){
		loadItem( (LastItemInfo*)CurSaveData, &TransmuteSlots.items[itemIndex], true );
		CurSaveData += itemSize;
	}
	Transmute_UpdateAvailableRecipes();
}

//----- (th3) -------------------------------------------------------------
bool __fastcall Transmute_ReadFromSave( HANDLE archiveHandle )
{
	int isSuccess = false;
	TransmuteSlots.ClearAll();

	HANDLE transmuteHandle;
	if( !SFileOpenFileEx( archiveHandle, SaveFile_Transmute, 0, &transmuteHandle ) ){
		return false;
	}

	uchar* buf = 0;
	DWORD fileSize = SFileGetFileSize( transmuteHandle, 0 );
	if( fileSize ){
		buf = (uchar*) AllocMem( fileSize );
		uint bytesReaded;
		if( SFileReadFile( transmuteHandle, buf, fileSize, (DWORD*)&bytesReaded, 0 ) ){
			int readSize = DecodeFile( buf, fileSize, CryptKey );
			if( readSize > 0 ){
				CurSaveData = (char*)buf;
				int transmuteSignature = GetNextHtonl2();
				if( transmuteSignature == SaveSignature ){
					SaveVersion = GetNextHtonl();
					Transmute_Load();
					isSuccess = true;
				}else{
					//TerminateWithError("Invalid transmute file");
				}
				CurSaveData = nullptr;
			}
		}
		FreeMem( buf );
	}
	SFileCloseFile( transmuteHandle );
	return isSuccess;
}

//----- (th3) -------------------------------------------------------------
Item* __fastcall TransmuteSlots_GetItemBySlotId( int slotId )
{
	if( slotId < Stash_SlotIDStart ) slotId = Stash_SlotIDStart; // assert
	return &TransmuteSlots.items[ slotId - Stash_SlotIDStart ];
}

//----- (th3) -------------------------------------------------------------
bool CanTransmute()
{
	#if CHEATS
	return true;
	#endif
	return GameMode != GM_COLISEUM && GameMode != GM_CLASSIC
		&& !HasTrait(CurrentPlayerIndex, TraitId::Barbarian)
		&& (!(Players[CurrentPlayerIndex].gameChanger & BIT(GC_9_NIGHT_KIN)))
        && (!(Players[CurrentPlayerIndex].gameChanger & BIT(GC_26_ASCETICISM)));
}
