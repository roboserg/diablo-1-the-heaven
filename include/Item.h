#ifndef _item_h_
#define _item_h_
#else
#error reinclude item.h
#endif

struct VisualTradeStorage
{
	Item		items[VisualTrade_MaxSlot];
	int			itemCount;
	union{
		short   usedFlags[VisualTrade_MaxSlot];
		short   usedFlagsAlt[VisualTrade_SlotsY][VisualTrade_SlotsX];
	};
};

struct StashTab
{
	Item		items[Stash_MaxSlot];
	int			itemCount;
	union{
		short	usedFlags[Stash_MaxSlot];
		short	usedFlagsAlt[Stash_SlotsY][Stash_SlotsX];
	};
};

// TODO: reuse for all inventory-like panels (inventory, stash, trade, transmute)
template<int R, int C, typename Usize = short>
struct CellSlots
{
	enum Params { Rows = R, Cols = C, Max = Rows * Cols };
	Item		items[Max];
	int			count;
	union{
		Usize   used[Max];
		Usize   used2d[Rows][Cols];
	};
	DisplayObject rect;

//----- (th3) -------------------------------------------------------------
void Clear(int itemIndex)
{
	if( count <= 0 ) return;

	for(auto& usedIndex : used){
		if( abs(usedIndex) == itemIndex + 1 ){
			usedIndex = 0;
		}
	}
	--count;
	if( count > 0 && count != itemIndex ){// Если вещи есть и удаляемая вещь не последняя
		items[itemIndex] = items[count]; // перенос крайнего в массиве предмета на индекс удалённого

		for(auto& used: used){ // TODO: check negative index for
			if( used == count + 1 ){
				used = itemIndex + 1;
			}
			if( used == -(count + 1) ){
				used = -(itemIndex + 1);
			}
		}
	}
}

//----- (th3) -------------------------------------------------------------
void ClearAll()
{
	count = 0;
	memzero(used);
	memzero(items);
}

};

// TODO: modern version, can be used instead of fixed template CellsSlots to avoid template funcs dublication and static memory corruption
struct CellSlots_dynamic
{
	const int Rows, Cols, Max;

	int			  count; // TODO: remove, use items.size instead
	vector<Item>  items;
	vector<short> used;
	short&        used2d(int row, int col){ return used[row * Cols + col]; }

	DisplayObject rect;
	
	CellSlots_dynamic(int rows, int cols):
		Rows(rows),
		Cols(cols),
		Max(rows * cols),
		count(0),
		items(Max),
		used(Max)
	{}
	void Clear(int itemIndex);
};
