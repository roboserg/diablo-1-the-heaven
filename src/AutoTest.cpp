#include "stdafx.h"

//----- (heaven) ----------------------------------------------------------
// Autotest: end-to-end checks that run the real game unattended.
//
//   TheHeaven.exe -autotest <phase> [-show]
//
// scripts\autotest.ps1 runs the phases in order in a scratch copy of the game
// (build\autotest), so the player's own saves, shared stash and settings are never
// touched. Each run creates or loads its own test hero straight away (no menus),
// runs its steps from the game loop, writes autotest-<phase>.json and screenshots
// next to the exe, and quits. The window opens off-screen, without focus and with
// sound off, so the PC stays usable; -show puts it on screen to watch.
//
// Phases, in the order the script runs them:
//   main   Warrior, normal mode: the shipped config, magic items rebuilt by the game's
//          5-second item check (autotest-rebuilds.csv), penalties on unique and set items,
//          Cain's transmute window, F9/F12
//          quick save and load, Griswold's personal stash next to the shared stash,
//          items held or in Adria's and Griswold's windows during a load or an autosave;
//          then stashes items in the shared stash and lists them in autotest-shared.txt
//          as the Warrior has them; takes a dungeon screenshot to check pixel light by
//          eye, and quits with a gem in Griswold's window
//   other  Mage, Classic mode: opens the shared stash. Every item that comes out
//          different from the Warrior's list must be locked and stay in the stash;
//          every item that comes out the same must not be
//   owner  the Warrior again: the gem main quit with is in the inventory, and every
//          listed item must still be exactly as stashed

extern StashTab StashTabs[Stash_TotalTabs];
bool AllowEnchant( Item& item );                                 // Enchanting.cpp
void Enchant_PlaceItemIntoCraftSlot();                           // Enchanting.cpp
bool Craft_Gems_IsItemAllowedToBeConsumable( Item* itemPtr );    // Crafting.cpp

namespace {

struct Phase
{
	const char*  name;
	const char*  hero;
	PLAYER_CLASS classId;
	GAME_MODE    gameMode;
	void       (*build)();
};

struct Step
{
	const char*          name;
	std::function<int()> run; // returns Next, Retry (run again next tick), or ticks to wait before the next step
};
constexpr int Next = 0, Retry = -1;
constexpr int StepTimeoutTicks = 30 * ENGINE_FPS;

struct CheckResult
{
	string name;
	bool   pass;
	string detail;
};

bool                Active   = false;
bool                Show     = false;
bool                Finished = false;
const Phase*        CurPhase = nullptr;
vector<Step>        Steps;
size_t              StepIndex  = 0;
int                 WaitTicks  = 20; // let the game settle after loading
int                 RetryTicks = 0;
vector<CheckResult> Checks;
vector<string>      Shots;

const char* ManifestFile = "autotest-shared.txt";
const char* HeroWarrior  = "AtWarrior";
const char* HeroMage     = "AtMage";

string Fmt( const char* fmt, ... )
{
	char buf[1024];
	va_list args;
	va_start( args, fmt );
	vsnprintf( buf, sizeof buf, fmt, args );
	va_end( args );
	return buf;
}

// Progress log, autotest-<phase>.log: shows how far a run got when it has no report
void Log( const char* fmt, ... )
{
	char buf[1024];
	va_list args;
	va_start( args, fmt );
	vsnprintf( buf, sizeof buf, fmt, args );
	va_end( args );
	FILE* f = fopen( Fmt( "autotest-%s.log", CurPhase ? CurPhase->name : "start" ).c_str(), "a" );
	if( ! f ) return;
	fprintf( f, "%s\n", buf );
	fclose( f );
}

bool Check( const char* name, bool pass, const string& detail = "" )
{
	Log( "  %s  %s  %s", pass ? "PASS" : "FAIL", name, detail.c_str() );
	Checks.push_back( { name, pass, detail } );
	return pass;
}

Player& Hero() { return Players[CurrentPlayerIndex]; }

//----- items -------------------------------------------------------------

// An item as a monster would drop it, in the form a save and reload gives it
Item MakeItem( int baseItemIndex, int seed, bool magic, int itemLevel = 30 )
{
	int randomSeed = RandomSeed, lastSeed = LastSeed;
	uint randUsingCounter = RandUsingCounter;
	SetupAllItems( MaxItems_255, baseItemIndex, seed, itemLevel, 0, magic, 0, 0, true, 0, 0, 0, -1, 0, 0 );
	LastItemInfo info;
	SaveItem( &info, &Items[MaxItems_255] );
	Item item;
	LoadItem( &info, &item, true );
	RandomSeed = randomSeed; LastSeed = lastSeed; RandUsingCounter = randUsingCounter;
	return item;
}

// Base items that can drop and be stashed, filtered by match
vector<int> FindBases( bool (*match)( const BaseItem& ), size_t max )
{
	vector<int> found;
	for( size_t i = 1; i < count_BaseItems && found.size() < max; ++i ){
		const BaseItem& b = BaseItems[i];
		if( b.baseId < 0 || b.ChanceToDrop <= 0 || IsItemForbiddenToStore( (int)i ) ) continue;
		if( match( b ) ) found.push_back( (int)i );
	}
	return found;
}

bool IsRing( const BaseItem& b )  { return b.EquippedLocation == EL_5_FINGER; }
bool IsStaff( const BaseItem& b ) { return b.MagicCode == MC_23_STAFF; }
bool IsBook( const BaseItem& b )  { return b.MagicCode == MC_24_BOOKS; }
bool IsArmor( const BaseItem& b ) { return b.EquippedLocation == EL_3_TORSO || b.EquippedLocation == EL_4_HEAD; }

// What the player sees of an item: its names and everything the generator decided
string Describe( const Item& item )
{
	string s = Fmt( "%s / %s | base %d ml %d spell %d charges %d/%d dmg %d-%d ac %d dur %d/%d sockets %d req %d/%d/%d/%d |",
		item.Name, item.FullMagicalItemName, item.baseItemIndex, (int)item.MagicLevel, (int)item.SpellIndex,
		item.CurCharges, item.BaseCharges, item.MinDamage, item.MaxDamage, item.ArmorClass,
		item.CurDurability, item.BaseDurability, (int)item.socketsAmount,
		item.RequiredStrength, item.RequiredMagic, item.RequiredDexterity, item.RequiredVitality );
	for( const Effect& e: item.effect ){
		if( e.id == AE_NO ) continue;
		s += Fmt( " %d:%d:%d..%d:%d", (int)e.id, e.type, e.minVal, e.maxVal, e.chance );
	}
	return s;
}

bool Held() { return Cur.GraphicsID >= CM_12_ITEMS_PICS_START; }

// Copies of the item with this seed the hero has anywhere: inventory, body, belt,
// cursor and Cain's transmute slots. The cursor counts only while it shows the item:
// after a load ItemOnCursor can keep a copy the player doesn't have.
int CountSeed( int seed )
{
	Player& p = Hero();
	int n = 0;
	auto count = [&]( const Item& it ){ if( it.ItemCode != IC_M1_NONE && it.seed == seed ) ++n; };
	for( int i = 0; i < p.InvItemCount; ++i ) count( p.InventorySlots[i] );
	for( const Item& it: p.OnBodySlots ) count( it );
	for( const Item& it: p.BeltInventory ) count( it );
	if( Held() ) count( p.ItemOnCursor );
	for( int i = 0; i < TransmuteSlots.count; ++i ) count( TransmuteSlots.items[i] );
	return n;
}

int FindInInventory( int seed )
{
	Player& p = Hero();
	for( int i = 0; i < p.InvItemCount; ++i ){
		if( p.InventorySlots[i].ItemCode != IC_M1_NONE && p.InventorySlots[i].seed == seed ) return i;
	}
	return -1;
}

void Hold( const Item& item )
{
	Hero().ItemOnCursor = item;
	SetCursorGraphics( item.GraphicValue + CM_12_ITEMS_PICS_START );
}

void EmptyHand()
{
	Hero().ItemOnCursor.ItemCode = IC_M1_NONE;
	SetCursorGraphics( CM_1_NORMAL_HAND );
}

bool PutHeldInInventory() { return AutoPutCursorToInventoryOrDrop( false ); }

bool TakeFromInventory( int seed )
{
	int i = FindInInventory( seed );
	if( i < 0 ) return false;
	Item item = Hero().InventorySlots[i];
	ClearInventorySlot( CurrentPlayerIndex, i );
	Hold( item );
	return true;
}

bool PutHeldInTransmute()
{
	bool put = Transmute_AutoPutCursorItem( CurrentPlayerIndex, true );
	EmptyHand();
	return put;
}

// Where the open stash (personal or shared) has the item with this seed, as tab * Stash_MaxSlot + index
vector<int> FindInStash( int seed )
{
	vector<int> found;
	for( int t = 0; t < Stash_TotalTabs; ++t ){
		for( int i = 0; i < StashTabs[t].itemCount; ++i ){
			if( StashTabs[t].items[i].seed == seed ) found.push_back( t * Stash_MaxSlot + i );
		}
	}
	return found;
}

int CountInStash( int seed ) { return (int)FindInStash( seed ).size(); }

// The item with this seed from the open stash into the inventory
bool TakeFromStash( int seed )
{
	vector<int> at = FindInStash( seed );
	if( at.empty() ) return false;
	StashCurrentTab = at[0] / Stash_MaxSlot;
	return Stash_MoveItemToInventory( at[0] % Stash_MaxSlot );
}

// The item with this seed from the inventory into the open stash
bool StashFromInventory( int seed )
{
	if( ! TakeFromInventory( seed ) ) return false;
	if( AddItemToStashFromCursor() ){ EmptyHand(); return true; }
	PutHeldInInventory();
	return false;
}

// A new item straight into the open stash
bool StashNew( const Item& item )
{
	Hold( item );
	bool put = AddItemToStashFromCursor();
	EmptyHand();
	return put;
}

// A gem Griswold's gem combiner takes; ItemCode is IC_M1_NONE if there is none.
// Gems don't drop (FindBases skips them): they come from shops and crafting.
Item MakeCraftGem( int seed )
{
	for( size_t i = 1; i < count_BaseItems; ++i ){
		if( BaseItems[i].baseId < 0 || BaseItems[i].MagicCode != MC_GEM ) continue;
		Item gem = MakeItem( (int)i, seed, false, 0 );
		if( gem.ItemCode != IC_M1_NONE && Craft_Gems_IsItemAllowedToBeConsumable( &gem ) ) return gem;
	}
	Item none = {};
	none.ItemCode = IC_M1_NONE;
	return none;
}

void QuickSave()    { KeyPressHandler( VK_120_F9_KEY ); }
void LoadFromMenu() { LoadGameMenuHandler( 0, 0 ); } // ESC, Load Game; F12 runs the same

//----- item rebuilds (bug 3) ---------------------------------------------
// The game keeps only a magic item's seed and a few fields, and rebuilds the rest after
// every load and, for the inventory and body, every 5 seconds (ValidateCurPlayer ->
// UpdateAllItems -> UpdateItem)

bool CanBeMagic( const BaseItem& b )
{
	return b.MagicCode != MC_27_QUEST_UNIQUE && is( b.EquippedLocation, EL_1_ONE_HANDED, EL_2_TWO_HANDED,
		EL_3_TORSO, EL_4_HEAD, EL_5_FINGER, EL_6_NECK, EL_9_WAIST, EL_10_HANDS, EL_11_FEET );
}

// An item as a monster drops it, on the ground, before the game has rebuilt it
Item DropItem( int baseItemIndex, int seed, int qlvl )
{
	int randomSeed = RandomSeed, lastSeed = LastSeed;
	uint randUsingCounter = RandUsingCounter;
	SetupAllItemsSmartdrop( MaxItems_255, baseItemIndex, seed, qlvl, 1 /*ChanceUniqueMob*/, 0, 0, 0, 1 );
	Item item = Items[MaxItems_255];
	RandomSeed = randomSeed; LastSeed = lastSeed; RandUsingCounter = randUsingCounter;
	return item;
}

// What the game's 5-second item check does to an inventory item
void RebuildLikeTheGame( Item& item )
{
	int randomSeed = RandomSeed, lastSeed = LastSeed;
	uint randUsingCounter = RandUsingCounter;
	UpdateItem( item );
	RandomSeed = randomSeed; LastSeed = lastSeed; RandUsingCounter = randUsingCounter;
}

struct DropInfo { int base, seed, qlvl; };

//----- unique and set item penalties (bug 4) ------------------------------

// A unique or set item and the same base item without its affixes, with the same base rolls
Item MakeUnique( int uniq, bool affixes )
{
	int randomSeed = RandomSeed, lastSeed = LastSeed;
	uint randUsingCounter = RandUsingCounter;
	const UniqueItem& u = UniqueItems[uniq];
	SetRndSeed( 9001 + uniq );
	GetItemAttrs( MaxItems_255, u.baseId, u.qualityLevel );
	Items[MaxItems_255].Identified = true; // an unidentified item gets no effects (Item_ApplyAffix)
	if( affixes ) GetUniqueItem( MaxItems_255, uniq );
	Item item = Items[MaxItems_255];
	item.Identified = true;
	item.IsReqMet = true;
	RandomSeed = randomSeed; LastSeed = lastSeed; RandUsingCounter = randUsingCounter;
	return item;
}

// The hero's values that unique penalties made worse. Bigger is better for all of them.
vector<int> HeroValues( const Item& item )
{
	Player& p = Hero();
	Item saved = p.OnBodySlots[IS_LeftRing];
	p.OnBodySlots[IS_LeftRing] = item;
	CalcCharParams( CurrentPlayerIndex, 0 );
	vector<int> v = { -p.DamageFromEnemy, p.stunThreshold[CT_0_STUN], p.resistMeleeMin, p.resistMeleeMax,
		p.resistArrowMin, p.resistArrowMax, p.MaxCurLife, p.MaxCurMana, p.CurStrength, p.CurMagic,
		p.CurDexterity, p.CurVitality, p.ACFromItems };
	for( short s: p.spiciesDfe ) v.push_back( -s );
	for( short r: p.resist ) v.push_back( r );
	for( short e: p.elementPercent ) v.push_back( e );
	p.OnBodySlots[IS_LeftRing] = saved;
	CalcCharParams( CurrentPlayerIndex, 0 );
	return v;
}

const char* HeroValueName( size_t i )
{
	static const char* names[] = { "damage from enemies", "stun threshold", "melee absorb min", "melee absorb max",
		"ranged absorb min", "ranged absorb max", "life", "mana", "strength", "magic", "dexterity", "vitality", "armour" };
	if( i < countof( names ) ) return names[i];
	i -= countof( names );
	if( i < 3 ) return "damage from a monster species";
	i -= 3;
	return i < ET_CAP ? "a resistance" : "elemental damage %";
}

// Uniques whose penalty is the point of the item, left as they are: they trade life for mana
// or the other way, or lose resistances or regeneration for a big bonus (Grace of Death)
bool IsTradeOff( const Item& item )
{
	for( const Effect& e: item.effect ){
		if( e.id == AE_MANA_MOVED_TO_HEALTH || e.id == AE_HEALTH_MOVED_TO_MANA ) return true;
		if( e.id == AE_FLAG && (e.type == EA_LOSE_RESISTANCE || e.type == EA_LOSE_REGENERATION) ) return true;
	}
	return false;
}

// Effects a unique or set item must not hurt the player with, and the harmful sign. This is
// the test's own list, from the effects in the unique data and the bug reports
bool IsPenalty( const Effect& e )
{
	int v = e.minVal != 0 ? e.minVal : e.chance;
	switch( e.id ){
	case AE_DFE: case AE_SPICIES_DFE: case AE_CLVL_DFE:
		return v > 0 || e.maxVal > 0 && e.id == AE_CLVL_DFE;
	case AE_STUN_RESIST: case AE_CLVL_STUN_RES: case AE_ABSORB_MELEE_DAMAGE: case AE_ABSORB_RANGED_DAMAGE:
	case AE_ELEMENT_PERCENT: case AE_STAT: case AE_ALL_STAT: case AE_HIT_POINTS: case AE_MANA: case AE_RESIST:
	case AE_RESIST_All: case AE_AC: case AE_SPECIAL_AC: case AE_LIFE_PERCENT: case AE_MANA_PERCENT:
		return v < 0;
	default:
		return false;
	}
}

//----- files -------------------------------------------------------------

i64 FileTime( const char* path )
{
	WIN32_FILE_ATTRIBUTE_DATA d;
	if( ! GetFileAttributesExA( path, GetFileExInfoStandard, &d ) ) return 0;
	return (i64(d.ftLastWriteTime.dwHighDateTime) << 32) | d.ftLastWriteTime.dwLowDateTime;
}

i64 SaveFileTime()
{
	char path[MAX_PATH];
	GetSaveArchivePath( path, MAX_PATH, AutoLoadSlot, 0 );
	return FileTime( path );
}

i64 SharedStashFileSize()
{
	char path[MAX_PATH];
	GetSharedStashArchivePath( path );
	WIN32_FILE_ATTRIBUTE_DATA d;
	if( ! GetFileAttributesExA( path, GetFileExInfoStandard, &d ) ) return 0;
	return (i64(d.nFileSizeHigh) << 32) | d.nFileSizeLow;
}

void DeleteFiles( const char* pattern )
{
	WIN32_FIND_DATAA f;
	HANDLE h = FindFirstFileA( pattern, &f );
	if( h == INVALID_HANDLE_VALUE ) return;
	do DeleteFileA( f.cFileName ); while( FindNextFileA( h, &f ) );
	FindClose( h );
}

// The game's own in-game screenshot (it applies the lighting), renamed for the report
void Shot( const char* name )
{
	DeleteFiles( "THscrn*.gif" );
	SaveScreenShot();
	WIN32_FIND_DATAA f;
	HANDLE h = FindFirstFileA( "THscrn*.gif", &f );
	if( h == INVALID_HANDLE_VALUE ){
		Check( Fmt( "screenshot %s saved", name ).c_str(), false );
		return;
	}
	FindClose( h );
	string file = Fmt( "autotest-%s-%s.gif", CurPhase->name, name );
	if( MoveFileExA( f.cFileName, file.c_str(), MOVEFILE_REPLACE_EXISTING ) ) Shots.push_back( file );
}

map<int, string> ReadManifest()
{
	map<int, string> items;
	FILE* f = fopen( ManifestFile, "r" );
	if( ! f ) return items;
	char line[2048];
	while( fgets( line, sizeof line, f ) ){
		char* tab = strchr( line, '\t' );
		if( ! tab ) continue;
		*tab = 0;
		string desc = tab + 1;
		while( ! desc.empty() && (desc.back() == '\n' || desc.back() == '\r') ) desc.pop_back();
		items[atoi( line )] = desc;
	}
	fclose( f );
	return items;
}

string JsonText( const string& s )
{
	string out = "\"";
	for( uchar c: s ){
		if( c == '"' || c == '\\' ) { out += '\\'; out += c; }
		else if( c < 0x20 ) out += Fmt( "\\u%04x", c );
		else if( c >= 0x80 ) out += '?'; // the game's names are not UTF-8
		else out += c;
	}
	return out + "\"";
}

void WriteReport()
{
	int failed = 0;
	for( auto& c: Checks ) if( ! c.pass ) ++failed;
	string file = Fmt( "autotest-%s.json", CurPhase->name );
	FILE* f = fopen( file.c_str(), "w" );
	if( ! f ) return;
	fprintf( f, "{\n  \"phase\": %s,\n  \"version\": %s,\n  \"passed\": %d,\n  \"failed\": %d,\n  \"checks\": [\n",
		JsonText( CurPhase->name ).c_str(), JsonText( TheHeavenVersion ).c_str(), (int)Checks.size() - failed, failed );
	for( size_t i = 0; i < Checks.size(); ++i ){
		fprintf( f, "    { \"name\": %s, \"pass\": %s, \"detail\": %s }%s\n", JsonText( Checks[i].name ).c_str(),
			Checks[i].pass ? "true" : "false", JsonText( Checks[i].detail ).c_str(), i + 1 < Checks.size() ? "," : "" );
	}
	fprintf( f, "  ],\n  \"screenshots\": [" );
	for( size_t i = 0; i < Shots.size(); ++i ) fprintf( f, "%s%s", i ? ", " : "", JsonText( Shots[i] ).c_str() );
	fprintf( f, "]\n}\n" );
	fclose( f );
}

void Finish()
{
	Log( "finished" );
	WriteReport();
	Finished = true;
	gbRunGame = 0;
	gbRunGameResult = 0; // leave the game and quit instead of returning to the menu
}

//----- steps shared by phases --------------------------------------------

void AddStartSteps( GAME_MODE expectedMode )
{
	Steps.push_back( { "start", [expectedMode]{
		Check( "the game starts and loads the test hero", ! _stricmp( Hero().playerName, CurPhase->hero ) && GameMode == expectedMode,
			Fmt( "hero %s, game mode %d, %s", Hero().playerName, (int)GameMode, TheHeavenVersion ) );
		return Next;
	} } );
}

// Shared state between steps of one run
map<int, string> Manifest;
int              TestSeed = 0;
int              PersonalSeed = 0, HeldSeed = 0, EnchantSeed = 0, GemSeed = 0;
i64              SaveTimeBefore = 0;
vector<DropInfo> RebuildWatch; // magic items to watch in the inventory: those that kept changing first
constexpr int    QuitGemSeed = 6051; // main quits with this gem in Griswold's window; owner looks for it

int OpenSharedStash()
{
	StashPanel_OpenShared();
	Check( "the shared stash opens", IsSharedStashOpen && IsStashPanelVisible );
	return 10; // let it draw before a screenshot
}

int CloseSharedStash()
{
	StashPanel_Close();
	Check( "closing the shared stash writes th4shared.stv", ! IsSharedStashOpen && SharedStashFileSize() > 0,
		Fmt( "%lld bytes", SharedStashFileSize() ) );
	return Next;
}

//----- phase: main -------------------------------------------------------

void BuildMain()
{
	AddStartSteps( GM_NORMAL );

	Steps.push_back( { "config", []{
		Check( "pixel light is on with the shipped config.ini (issue #5)", PixelLight );
		Check( "multi-threaded pixel light is on with the shipped config.ini", MultiThreadPixelLight );
		Shot( "town" );
		return Next;
	} } );

	Steps.push_back( { "magic items through the game's 5-second item check", []{
		// Bug 3 (kishak95, 4 Apr): affixes on an item in the inventory kept disappearing and
		// coming back, "Not Identified" too, on an item that was never in the shared stash.
		// Every rebuild of an item must give the same item. autotest-rebuilds.csv lists
		// each magic item as it dropped and after each of 4 checks
		vector<int> bases = FindBases( IsStaff, 12 ), all = FindBases( CanBeMagic, 2000 );
		for( size_t i = 0, step = max<size_t>( 1, all.size() / 48 ); i < all.size() && bases.size() < 60; i += step ){
			if( find( bases.begin(), bases.end(), all[i] ) == bases.end() ) bases.push_back( all[i] );
		}
		FILE* f = fopen( "autotest-rebuilds.csv", "w" );
		if( f ) fprintf( f, "base,qlvl,seed,first check changes it,keeps changing,as dropped,check 1,check 2,check 3,check 4\n" );
		int magic = 0, changedOnce = 0, keepsChanging = 0;
		string first;
		vector<DropInfo> once, rest;
		for( int base: bases ) for( int qlvl: { 10, 30, 60 } ) for( int k = 0; k < 20; ++k ){
			int seed = 70001 + k * 7919 + base * 31;
			Item it = DropItem( base, seed, qlvl );
			if( it.MagicLevel != ML_1_MAGIC ) continue;
			++magic;
			string d[5];
			d[0] = Describe( it );
			for( int c = 1; c < 5; ++c ){ RebuildLikeTheGame( it ); d[c] = Describe( it ); }
			bool changes = d[1] != d[0];
			bool keeps   = d[2] != d[1] || d[3] != d[2] || d[4] != d[3];
			if( changes ) ++changedOnce;
			if( keeps ){
				++keepsChanging;
				RebuildWatch.push_back( { base, seed, qlvl } );
				if( first.empty() ) first = Fmt( "First: base %d seed %d qlvl %d: check 1: %s  check 2: %s", base, seed, qlvl, d[1].c_str(), d[2].c_str() );
			}else{
				( changes ? once : rest ).push_back( { base, seed, qlvl } );
			}
			if( f ){
				fprintf( f, "%d,%d,%d,%s,%s", base, qlvl, seed, changes ? "yes" : "no", keeps ? "yes" : "no" );
				for( const string& s: d ){ string q = s; replace( q.begin(), q.end(), '"', '\'' ); fprintf( f, ",\"%s\"", q.c_str() ); }
				fprintf( f, "\n" );
			}
		}
		if( f ) fclose( f );
		RebuildWatch.insert( RebuildWatch.end(), once.begin(), once.end() );
		RebuildWatch.insert( RebuildWatch.end(), rest.begin(), rest.end() );
		Check( "the test makes magic items as monsters drop them", magic >= 200, Fmt( "%d magic items from %d bases", magic, (int)bases.size() ) );
		Check( "no magic item keeps changing in the game's 5-second item check (bug 3)", keepsChanging == 0,
			Fmt( "%d of %d keep changing; %d change at the first check. ", keepsChanging, magic, changedOnce ) + first );
		return Next;
	} } );

	Steps.push_back( { "magic items in the inventory for 11 seconds", []{
		// The same in the running game: its own 5-second check, two or three times
		static vector<int> seeds, changes;
		static vector<string> last;
		static int ticks = 0;
		if( ticks == 0 ){
			for( const DropInfo& d: RebuildWatch ){
				if( seeds.size() >= 10 ) break;
				Item it = DropItem( d.base, d.seed, d.qlvl );
				Hold( it );
				if( ! PutHeldInInventory() ){ EmptyHand(); continue; }
				int i = FindInInventory( it.seed );
				seeds.push_back( it.seed );
				last.push_back( i >= 0 ? Describe( Hero().InventorySlots[i] ) : "gone" );
				changes.push_back( 0 );
			}
			if( ! Check( "magic items to watch go into the inventory", ! seeds.empty() ) ) return Next;
		}
		for( size_t k = 0; k < seeds.size(); ++k ){
			int i = FindInInventory( seeds[k] );
			string now = i >= 0 ? Describe( Hero().InventorySlots[i] ) : "gone";
			if( now != last[k] ){ ++changes[k]; last[k] = now; }
		}
		if( ++ticks < 11 * ENGINE_FPS ) return Retry;
		int twice = (int)count_if( changes.begin(), changes.end(), []( int n ){ return n >= 2; } );
		string counts;
		for( int n: changes ) counts += Fmt( " %d", n );
		Check( "in the running game, no magic item in the inventory changes more than once in 11 seconds (bug 3)", twice == 0,
			Fmt( "%d of %d changed 2 times or more. Changes per item:", twice, (int)seeds.size() ) + counts );
		for( int seed: seeds ) if( TakeFromInventory( seed ) ) EmptyHand();
		return Next;
	} } );

	Steps.push_back( { "unique and set item penalties", []{
		// Bug 4 (Rageweaver, Gorvex, 5 Apr): harmful penalties on uniques and sets are meant to be
		// gone (0172f90), but -stun threshold and +damage from enemies still showed. Every
		// unique and set item, worn by the hero, must leave each of the hero's values at least as
		// good as the same base item without its affixes, and its tooltip must not list a penalty
		int checked = 0, worse = 0, shown = 0, skipped = 0, negativeAc = 0;
		string firstWorse, firstShown, firstAc;
		for( size_t u = 0; u < count_UniqueItems; ++u ){
			const UniqueItem& uq = UniqueItems[u];
			if( uq.baseId <= 0 || (size_t)uq.baseId >= count_BaseItems || BaseItems[uq.baseId].baseId < 0 || uq.effectCount <= 0 ) continue;
			Item item = MakeUnique( (int)u, true ), plain = MakeUnique( (int)u, false );
			if( item.MagicLevel < ML_2_UNIQUE ) continue;
			if( item.ArmorClass < 0 ){
				++negativeAc;
				if( firstAc.empty() ) firstAc = Fmt( "%s (unique %d): %d", item.FullMagicalItemName, (int)u, item.ArmorClass );
			}
			if( IsTradeOff( item ) ){ ++skipped; continue; }
			++checked;
			vector<int> withIt = HeroValues( item ), base = HeroValues( plain );
			for( size_t i = 0; i < withIt.size(); ++i ){
				if( withIt[i] < base[i] ){
					++worse;
					int sign = i == 0 || i >= 13 && i < 16 ? -1 : 1; // damage taken: smaller is better
					string what = Fmt( "%s (unique %d) makes %s worse: %d instead of %d", item.FullMagicalItemName, (int)u, HeroValueName( i ), sign * withIt[i], sign * base[i] );
					if( worse <= 40 ) Log( "    worse: %s", what.c_str() );
					if( firstWorse.empty() ) firstWorse = what;
					break;
				}
			}
			// the tooltip, as when hovering the item in the inventory
			Item savedInv = Hero().InventorySlots[0];
			int invCount = Hero().InvItemCount;
			Hero().InventorySlots[0] = item;
			if( invCount < 1 ) Hero().InvItemCount = 1;
			Tooltip_ShowItemInfo( IS_Inventory + 0 );
			Hero().InventorySlots[0] = savedInv;
			Hero().InvItemCount = invCount;
			for( int k = 0; k < (int)countof( item.effect ); ++k ){
				const Effect& e = item.effect[k];
				if( e.id == AE_NO || ! IsPenalty( e ) || ! PrintItemEffectToStringBuffer( e.id, &item, k ) ) continue;
				string text = InfoPanelBuffer;
				for( int line = 0; line < Tooltip_LineCount(); ++line ){
					const char* shownText = Tooltip_LineText( line ); // a long line may be wrapped: its start is enough
					size_t n = strlen( shownText );
					if( n >= 10 && text.compare( 0, n, shownText ) == 0 || text == shownText ){
						++shown;
						if( shown <= 40 ) Log( "    shown: %s (unique %d) \"%s\"", item.FullMagicalItemName, (int)u, text.c_str() ); if( firstShown.empty() ) firstShown = Fmt( "%s (unique %d) shows \"%s\"", item.FullMagicalItemName, (int)u, text.c_str() );
						break;
					}
				}
			}
		}
		Tooltip_Clear();
		Check( "unique and set items to check exist", checked > 500, Fmt( "%d checked; %d trade-offs left as they are (life for mana, lose resistances or regeneration)", checked, skipped ) );
		Check( "no unique or set item has negative armour (bug 4)", negativeAc == 0, Fmt( "%d. First: ", negativeAc ) + firstAc );
		Check( "no unique or set item makes the hero worse in any value (bug 4)", worse == 0, Fmt( "%d of %d. First: ", worse, checked ) + firstWorse );
		Check( "no unique or set item tooltip shows a penalty (bug 4)", shown == 0, Fmt( "%d penalty lines. First: ", shown ) + firstShown );
		return Next;
	} } );

	Steps.push_back( { "transmute close while holding an item", []{
		vector<int> rings = FindBases( IsRing, 4 );
		if( ! Check( "found ring bases for the test items", rings.size() >= 2 ) ) return Next;
		Transmute_Open();
		vector<int> seeds;
		for( int k = 0; k < 3; ++k ){
			Item it = MakeItem( rings[k % rings.size()], 101 + k, true );
			Hold( it );
			if( PutHeldInTransmute() ) seeds.push_back( it.seed );
		}
		Check( "3 items go into Cain's transmute slots", TransmuteSlots.count == 3, Fmt( "%d in the slots", TransmuteSlots.count ) );
		Item held = MakeItem( rings[1], 111, true );
		Hold( held );
		Transmute_Close();
		int back = 0;
		for( int seed: seeds ) if( FindInInventory( seed ) >= 0 && CountSeed( seed ) == 1 ) ++back;
		Check( "closing Cain's window while holding an item returns the slot items (issue #3)",
			TransmuteSlots.count == 0 && back == (int)seeds.size(),
			Fmt( "%d of %d back in the inventory, %d left in the slots", back, (int)seeds.size(), TransmuteSlots.count ) );
		Check( "the held item stays on the cursor",
			Hero().ItemOnCursor.ItemCode != IC_M1_NONE && Hero().ItemOnCursor.seed == held.seed
			&& Cur.GraphicsID == held.GraphicValue + CM_12_ITEMS_PICS_START );
		PutHeldInInventory();
		return Next;
	} } );

	Steps.push_back( { "F9 while holding an item", []{
		Item it = MakeItem( FindBases( IsRing, 1 ).at( 0 ), 121, true );
		Hold( it );
		i64 before = SaveFileTime();
		KeyPressHandler( VK_120_F9_KEY );
		Check( "F9 doesn't save while an item is held (it wouldn't be in the save)",
			SaveFileTime() == before && Hero().ItemOnCursor.seed == it.seed && Hero().ItemOnCursor.ItemCode != IC_M1_NONE );
		PutHeldInInventory();
		return Next;
	} } );

	Steps.push_back( { "F9 with Cain's window open", []{
		Item it = MakeItem( FindBases( IsRing, 1 ).at( 0 ), 131, true );
		TestSeed = it.seed;
		Transmute_Open();
		Hold( it );
		PutHeldInTransmute();
		i64 before = SaveFileTime();
		KeyPressHandler( VK_120_F9_KEY );
		Check( "F9 closes Cain's window, moves its items to the inventory and saves (issue #3)",
			! IsTransmutePanelVisible && TransmuteSlots.count == 0 && FindInInventory( TestSeed ) >= 0
			&& CountSeed( TestSeed ) == 1 && SaveFileTime() != before,
			Fmt( "window %s, %d in the slots, %d copies, save %s", IsTransmutePanelVisible ? "open" : "closed",
				TransmuteSlots.count, CountSeed( TestSeed ), SaveFileTime() != before ? "written" : "not written" ) );
		return Next;
	} } );

	Steps.push_back( { "F12 with the item back in a slot", []{
		// The save has the item in the inventory. Before the fix, an item sitting in a slot
		// when loading came back twice: once from the save, once from the slot.
		TakeFromInventory( TestSeed );
		Transmute_Open();
		PutHeldInTransmute();
		KeyPressHandler( VK_123_F12_KEY );
		return 20;
	} } );

	Steps.push_back( { "F12 result", []{
		Check( "F12 brings the item back once, not twice (issue #3)",
			CountSeed( TestSeed ) == 1 && TransmuteSlots.count == 0,
			Fmt( "%d copies, %d in the slots", CountSeed( TestSeed ), TransmuteSlots.count ) );
		return Next;
	} } );

	// ---- Griswold's personal stash and Gillian's shared stash (bug 1) ----

	Steps.push_back( { "personal stash item and a visit to the shared stash", []{
		// Rageweaver: put an item in Griswold's stash, open the shared stash, open Griswold's
		// again: the item is gone. The shared stash replaced the personal one in memory, and
		// leaving it reloaded the personal stash from the last save
		vector<int> rings = FindBases( IsRing, 4 );
		Item it = MakeItem( rings.at( 0 ), 6001, true );
		PersonalSeed = it.seed;
		StashPanel_OpenPersonal();
		bool put = StashNew( it );
		StashPanel_Close();
		StashPanel_OpenShared();
		StashPanel_Close();
		StashPanel_OpenPersonal();
		int inStash = CountInStash( PersonalSeed );
		StashPanel_Close();
		Check( "an item put in Griswold's stash is still there after a visit to Gillian's shared stash (bug 1)",
			put && inStash == 1 && CountSeed( PersonalSeed ) == 0,
			Fmt( "%d in Griswold's stash, %d on the hero", inStash, CountSeed( PersonalSeed ) ) );
		return Next;
	} } );

	Steps.push_back( { "personal stash item moved to the shared stash", []{
		// Rageweaver: put an item in Griswold's stash, take it out, put it in the shared
		// stash: it is in Griswold's stash again. The last save still had it there
		vector<int> rings = FindBases( IsRing, 4 );
		Item it = MakeItem( rings.at( 1 % rings.size() ), 6011, true );
		int seed = it.seed;
		StashPanel_OpenPersonal();
		StashNew( it );
		StashPanel_Close();
		QuickSave(); // the save has the ring in Griswold's stash
		StashPanel_OpenPersonal();
		bool out = TakeFromStash( seed );
		StashPanel_Close();
		StashPanel_OpenShared();
		bool in = StashFromInventory( seed );
		StashPanel_Close();
		StashPanel_OpenPersonal();
		int personal = CountInStash( seed );
		StashPanel_Close();
		StashPanel_OpenShared();
		int shared = CountInStash( seed );
		TakeFromStash( seed ); // the morph tests below expect only their own items in the shared stash
		StashPanel_Close();
		Check( "an item moved from Griswold's stash to the shared stash is in the shared stash only (bug 1)",
			out && in && personal == 0 && shared == 1, Fmt( "%d in Griswold's stash, %d in the shared stash", personal, shared ) );
		return Next;
	} } );

	Steps.push_back( { "personal stash through quick save and load", []{
		QuickSave();
		KeyPressHandler( VK_123_F12_KEY );
		return 20;
	} } );

	Steps.push_back( { "personal stash through quick save and load result", []{
		StashPanel_OpenPersonal();
		int inStash = CountInStash( PersonalSeed );
		StashPanel_Close();
		Check( "Griswold's stash keeps the item through F9 and F12 after the shared stash was open", inStash == 1,
			Fmt( "%d in Griswold's stash", inStash ) );
		return Next;
	} } );

	// ---- items lost or doubled by a load (bug 2 in docs/OPEN_BUGS.md) ----

	Steps.push_back( { "shared stash item held while loading", []{
		// Marcototech: take an item from the stash, keep it on the cursor, ESC, Load Game:
		// the item is gone. The shared stash file was written without it at once, and closing
		// the stash for the load saved the game with it on the cursor, where loading doesn't
		// give it back
		Item it = MakeItem( FindBases( IsRing, 4 ).at( 0 ), 6021, true );
		HeldSeed = it.seed;
		StashPanel_OpenShared();
		StashNew( it );
		StashPanel_Close(); // saves: the ring is in the shared stash, not on the hero
		StashPanel_OpenShared();
		TakeFromStash( HeldSeed );
		TakeFromInventory( HeldSeed ); // on the cursor
		LoadFromMenu();
		return 20;
	} } );

	Steps.push_back( { "shared stash item held while loading result", []{
		int onHero = CountSeed( HeldSeed );
		StashPanel_OpenShared();
		int shared = CountInStash( HeldSeed );
		TakeFromStash( HeldSeed );
		StashPanel_Close();
		Check( "an item taken from the shared stash and held while loading a save is back in the stash, once (bug 2)",
			shared == 1 && onHero == 0, Fmt( "after the load: %d in the shared stash, %d on the hero", shared, onHero ) );
		return Next;
	} } );

	Steps.push_back( { "Adria's window while loading", []{
		// GitHub #3: enchant at Adria and load: two copies with F12, gone with Load Game
		EnchantSeed = 0;
		for( int base: FindBases( IsArmor, 40 ) ){
			Item it = MakeItem( base, 6031, false, 0 );
			if( it.MagicLevel != 0 || ! AllowEnchant( it ) ) continue;
			Hold( it );
			if( PutHeldInInventory() ) EnchantSeed = it.seed;
			break;
		}
		if( ! Check( "found an item Adria can enchant", EnchantSeed != 0 ) ) return Next;
		QuickSave(); // the save has it in the inventory
		Enchant_Open();
		TakeFromInventory( EnchantSeed );
		Enchant_PlaceItemIntoCraftSlot();
		bool inWindow = EnchantItem.ItemCode != IC_M1_NONE && EnchantItem.seed == EnchantSeed && ! Held();
		Check( "the item goes into Adria's enchant window", inWindow );
		LoadFromMenu();
		return 20;
	} } );

	Steps.push_back( { "Adria's window while loading result", []{
		if( ! EnchantSeed ) return Next;
		Check( "an item in Adria's window while loading a save comes back once (bug 2)",
			CountSeed( EnchantSeed ) == 1 && ! IsEnchantPanelVisible,
			Fmt( "%d copies, window %s", CountSeed( EnchantSeed ), IsEnchantPanelVisible ? "open" : "closed" ) );
		return Next;
	} } );

	Steps.push_back( { "Griswold's window during an autosave", []{
		// GitHub #3: a gem put in Griswold's crafting window disappeared. With the Auto Save
		// game changer (and in multiplayer) the game saved once a minute, also while a
		// window held items, and that save didn't have them
		Item gem = MakeCraftGem( 6041 );
		if( ! Check( "found a gem Griswold's gem combiner takes", gem.ItemCode != IC_M1_NONE ) ) return Next;
		GemSeed = gem.seed;
		Hold( gem );
		PutHeldInInventory();
		QuickSave(); // the save has the gem in the inventory
		Craft_Open();
		TakeFromInventory( GemSeed );
		bool inWindow = Craft_Gems_AutoPutCursorItem();
		Check( "the gem goes into Griswold's gem combiner", inWindow );
		Hero().gameChanger |= BIT( GC_1_AUTO_SAVE );
		AutoSaveTickCount = GetTickCountTh() - 61000_ms; // the last autosave was over a minute ago: one is due
		SaveTimeBefore = SaveFileTime();
		return 5; // the game loop gets the chance to autosave
	} } );

	Steps.push_back( { "Griswold's window during an autosave: close it", []{
		if( ! GemSeed ) return Next;
		Check( "the autosave waits while Griswold's window holds the gem", SaveFileTime() == SaveTimeBefore,
			SaveFileTime() == SaveTimeBefore ? "" : "it saved without the gem" );
		Craft_Close(); // the gem goes back to the inventory
		return 5;
	} } );

	Steps.push_back( { "Griswold's window during an autosave: load", []{
		Hero().gameChanger &= ~BIT( GC_1_AUTO_SAVE );
		if( ! GemSeed ) return Next;
		Check( "the autosave runs once the window is closed", SaveFileTime() != SaveTimeBefore );
		LoadFromMenu(); // loads the autosave
		return 20;
	} } );

	Steps.push_back( { "Griswold's window during an autosave result", []{
		// The autosave writes only the hero file, while loading reads the game file F9 wrote:
		// the gem was lost when the next game started from the hero file (see the quit check
		// in the owner phase). Here: the load brings it back once, not twice.
		Hero().gameChanger &= ~BIT( GC_1_AUTO_SAVE );
		if( ! GemSeed ) return Next;
		Check( "the gem from Griswold's window is there once after loading",
			CountSeed( GemSeed ) == 1, Fmt( "%d copies", CountSeed( GemSeed ) ) );
		return Next;
	} } );

	Steps.push_back( { "open shared stash", OpenSharedStash } );

	Steps.push_back( { "stash items", []{
		FILE* f = fopen( ManifestFile, "w" );
		auto stash = [f]( const Item& it ){
			if( it.ItemCode == IC_M1_NONE || it.ItemCode == IC_11_GOLD ) return false;
			Hold( it );
			bool put = AddItemToStashFromCursor();
			if( put && f ) fprintf( f, "%d\t%s\n", it.seed, Describe( it ).c_str() );
			EmptyHand();
			return put;
		};
		// Plain armour first, so it lands on the first tab: nothing about it depends on
		// the class or the game mode, so it must come out the same for the Mage, unlocked
		int plain = 0;
		vector<int> armour = FindBases( IsArmor, 40 );
		for( size_t k = 0; k < armour.size() && plain < 4; ++k ){
			Item it = MakeItem( armour[k], 4000 + int(k), false, 0 );
			if( it.MagicLevel == 0 && stash( it ) ) ++plain;
		}
		// Staves and books get class-dependent spells, and Classic mode uses another
		// magic affix table: the kinds of items that morph
		vector<int> bases;
		for( auto match: { IsStaff, IsBook, IsRing, IsArmor } ){
			vector<int> found = FindBases( match, 40 );
			for( size_t k = 0; k < found.size() && k < 4 * 7; k += 7 ) bases.push_back( found[k] ); // 4 bases of each kind, spread out
		}
		int magic = 0;
		for( size_t k = 0; k < bases.size(); ++k ){
			for( int copy = 0; copy < 2; ++copy ){
				if( stash( MakeItem( bases[k], 5000 + int(k) * 10 + copy, true ) ) ) ++magic;
			}
		}
		if( f ) fclose( f );
		Check( "the Warrior stashes plain armour and magic staves, books, rings and armour in the shared stash",
			plain >= 2 && magic >= 10, Fmt( "%d plain and %d magic items stashed", plain, magic ) );
		return 10;
	} } );

	Steps.push_back( { "shared stash as owner", []{
		int locked = 0, total = 0;
		for( int i = 0; i < StashTabs[0].itemCount; ++i, ++total ) if( SharedStash_IsLocked( StashTabs[0].items[i] ) ) ++locked;
		Check( "nothing is locked for the Warrior who stashed it", locked == 0, Fmt( "%d of %d locked", locked, total ) );
		Shot( "shared-stash" );
		return Next;
	} } );

	Steps.push_back( { "close shared stash", CloseSharedStash } );

	Steps.push_back( { "go to the dungeon", []{
		StartNewLvl( CurrentPlayerIndex, WM_1026_NEXT_DUNGEON, DUN_1_CHURCH_1 );
		return Next;
	} } );

	Steps.push_back( { "dungeon screenshot", []{
		if( (DUNGEON)Dungeon != DUN_1_CHURCH_1 ) return (int)Retry;
		static int settle = 40;
		if( settle-- > 0 ) return (int)Retry;
		Shot( "dungeon-pixel-light" );
		Check( "the hero reaches dungeon level 1 (screenshot to check pixel light by eye)", true );
		return Next;
	} } );

	Steps.push_back( { "quit with a gem in Griswold's window", []{
		// Quitting saves with the Auto Save game changer and in multiplayer. The owner phase
		// checks that the gem is in that save
		Item gem = MakeCraftGem( QuitGemSeed );
		Craft_Open();
		Hold( gem );
		Check( "the gem goes into Griswold's gem combiner before quitting", gem.ItemCode != IC_M1_NONE && Craft_Gems_AutoPutCursorItem() );
		Hero().gameChanger |= BIT( GC_1_AUTO_SAVE );
		return Next; // then the run finishes and the game quits
	} } );
}

//----- phase: other ------------------------------------------------------

void BuildOther()
{
	AddStartSteps( GM_CLASSIC );

	Steps.push_back( { "read the Warrior's list", []{
		Manifest = ReadManifest();
		Check( "the Warrior's list of stashed items is there", ! Manifest.empty(), Fmt( "%d items", (int)Manifest.size() ) );
		return Next;
	} } );

	Steps.push_back( { "open shared stash", OpenSharedStash } );

	// The game's 5-second item check rebuilds the open stash tab too (StashPanel_RecalcItemsReq):
	// locked items must stay locked through it, and be saved back as the Warrior left them
	Steps.push_back( { "keep the shared stash open for 11 seconds", []{ return 11 * ENGINE_FPS; } } );

	Steps.push_back( { "compare", []{
		int morphed = 0, morphedLocked = 0, same = 0, sameLocked = 0;
		string example, wrong;
		for( int t = 0; t < Stash_TotalTabs; ++t ){
			for( int i = 0; i < StashTabs[t].itemCount; ++i ){
				const Item& item = StashTabs[t].items[i];
				auto listed = Manifest.find( item.seed );
				if( listed == Manifest.end() ) continue;
				bool differs = Describe( item ) != listed->second;
				bool locked = SharedStash_IsLocked( item );
				if( differs ){
					++morphed;
					if( locked ) ++morphedLocked;
					else if( wrong.empty() ) wrong = "not locked: " + Describe( item ) + "  was: " + listed->second;
					if( example.empty() ) example = "Warrior's: " + listed->second + "  Mage's: " + Describe( item );
				}else{
					++same;
					if( locked ){ ++sameLocked; if( wrong.empty() ) wrong = "locked but the same: " + Describe( item ); }
				}
			}
		}
		Check( "items that would come out different for a Classic Mage really exist in the test", morphed > 0,
			Fmt( "%d of %d items differ. ", morphed, morphed + same ) + example );
		Check( "every item that would come out different is locked", morphedLocked == morphed,
			Fmt( "%d of %d locked. ", morphedLocked, morphed ) + wrong );
		Check( "items that come out the same (plain armour) are not locked", same > 0 && sameLocked == 0,
			Fmt( "%d of %d locked", sameLocked, same ) );
		return Next;
	} } );

	Steps.push_back( { "take out a locked item", []{
		StashTab& tab = StashTabs[StashCurrentTab];
		for( int i = 0; i < tab.itemCount; ++i ){
			if( ! SharedStash_IsLocked( tab.items[i] ) ) continue;
			int seed = tab.items[i].seed, count = tab.itemCount;
			bool taken = Stash_MoveItemToInventory( i );
			Check( "a locked item can't be taken out", ! taken && tab.itemCount == count && FindInStash( seed ).size() == 1
				&& FindInInventory( seed ) < 0 );
			return Next;
		}
		Check( "a locked item can't be taken out", false, "no locked item on the first tab" );
		return Next;
	} } );

	Steps.push_back( { "take out and put back an unlocked item", []{
		StashTab& tab = StashTabs[StashCurrentTab];
		for( int i = 0; i < tab.itemCount; ++i ){
			if( SharedStash_IsLocked( tab.items[i] ) ) continue;
			int seed = tab.items[i].seed;
			bool taken = Stash_MoveItemToInventory( i );
			Check( "an item that doesn't morph can be taken out", taken && FindInInventory( seed ) >= 0 && FindInStash( seed ).empty() );
			bool back = TakeFromInventory( seed ) && AddItemToStashFromCursor();
			EmptyHand();
			Check( "and stashed again", back && FindInStash( seed ).size() == 1 );
			return 10;
		}
		Check( "an item that doesn't morph can be taken out", false, "every item on the first tab is locked" );
		return 10;
	} } );

	Steps.push_back( { "screenshot", []{ Shot( "shared-stash-locked" ); return Next; } } );
	Steps.push_back( { "close shared stash", CloseSharedStash } );
}

//----- phase: owner ------------------------------------------------------

void BuildOwner()
{
	AddStartSteps( GM_NORMAL );

	Steps.push_back( { "the gem main quit with", []{
		Hero().gameChanger &= ~BIT( GC_1_AUTO_SAVE ); // main turned it on to make quitting save
		int seed = MakeCraftGem( QuitGemSeed ).seed;
		Check( "a gem left in Griswold's crafting window when the game quits is in the inventory (bug 2)",
			CountSeed( seed ) == 1, Fmt( "%d copies", CountSeed( seed ) ) );
		return Next;
	} } );

	Steps.push_back( { "read the list", []{
		Manifest = ReadManifest();
		Check( "the list of stashed items is there", ! Manifest.empty(), Fmt( "%d items", (int)Manifest.size() ) );
		return Next;
	} } );

	Steps.push_back( { "open shared stash", OpenSharedStash } );

	Steps.push_back( { "compare", []{
		int missing = 0, changed = 0, locked = 0;
		string first;
		for( auto& [seed, desc]: Manifest ){
			vector<int> at = FindInStash( seed );
			if( at.size() != 1 ){ ++missing; if( first.empty() ) first = Fmt( "seed %d found %d times", seed, (int)at.size() ); continue; }
			const Item& item = StashTabs[at[0] / Stash_MaxSlot].items[at[0] % Stash_MaxSlot];
			if( Describe( item ) != desc ){ ++changed; if( first.empty() ) first = "now: " + Describe( item ) + "  was: " + desc; }
			if( SharedStash_IsLocked( item ) ) ++locked;
		}
		Check( "every item the Warrior stashed is still there, once", missing == 0, Fmt( "%d missing or doubled. ", missing ) + first );
		Check( "every item is exactly as the Warrior stashed it, after the Mage opened the stash", changed == 0,
			Fmt( "%d changed. ", changed ) + first );
		Check( "nothing is locked for the Warrior", locked == 0, Fmt( "%d locked", locked ) );
		return Next;
	} } );

	Steps.push_back( { "take one out", []{
		StashTab& tab = StashTabs[StashCurrentTab];
		if( tab.itemCount == 0 ){ Check( "the Warrior can take an item out", false, "the first tab is empty" ); return Next; }
		int seed = tab.items[0].seed;
		bool taken = Stash_MoveItemToInventory( 0 );
		int at = FindInInventory( seed );
		Check( "the Warrior can take an item out, unchanged", taken && at >= 0 && Describe( Hero().InventorySlots[at] ) == Manifest[seed] );
		return Next;
	} } );

	Steps.push_back( { "close shared stash", CloseSharedStash } );
}

const Phase Phases[] = {
	{ "main",  HeroWarrior, PC_0_WARRIOR, GM_NORMAL,  BuildMain  },
	{ "other", HeroMage,    PC_2_MAGE,    GM_CLASSIC, BuildOther },
	{ "owner", HeroWarrior, PC_0_WARRIOR, GM_NORMAL,  BuildOwner },
};

} // namespace

//----- entry points ------------------------------------------------------

// From WinMain, before the config is read: "-autotest <phase> [-show]"
void AutoTest_ParseCommandLine( const char* cmdLine )
{
	const char* arg = strstr( cmdLine, "-autotest" );
	if( ! arg ) return;
	char name[32] = "";
	sscanf( arg + strlen( "-autotest" ), " %31s", name );
	for( const Phase& p: Phases ){
		if( ! _stricmp( p.name, name ) ) CurPhase = &p;
	}
	if( ! CurPhase ){
		MessageBoxA( NULL, "Usage: TheHeaven.exe -autotest main|other|owner [-show]", "Autotest", MB_OK | MB_ICONERROR );
		ExitProcess( 2 );
	}
	Active = true;
	Show = strstr( cmdLine, "-show" ) != nullptr;
	DeleteFileA( Fmt( "autotest-%s.log", CurPhase->name ).c_str() );
	Log( "autotest %s%s", CurPhase->name, Show ? " (shown)" : "" );
}

bool AutoTest_Active() { return Active; }
bool AutoTest_Show()   { return Show; }

// After the config is read: load the test hero at once, quietly, in a window
void AutoTest_OverrideConfig()
{
	if( ! Active ) return;
	LoadOnStart    = true;
	LoadMulti      = false;
	AutoLoadSlot   = 0;
	NewGameOnStart = true;
	IsStartVideo   = false;
	AllSound       = false;
	IsFullScreen   = false;
	BigWin         = false;
	BackgroundDraw = true; // keep drawing (and screenshots) while the window isn't active
	EasyClick      = 0;
	XinputEnabled  = false; // it would read the player's gamepad and send its keys to the app they are using
	Log( "config read" );
}

// From SinglePlayerGame before the hero list is read: creates the phase's hero in slot 0
void AutoTest_PrepareHero()
{
	GameMode = CurPhase->gameMode;
	GameModeFilter = CurPhase->gameMode == GM_CLASSIC ? (int)B(GM_CLASSIC) : -1;
	LoadCharSlots( nullptr );
	if( _stricmp( CharNameSlots[AutoLoadSlot], CurPhase->hero ) ){
		CharSaveInfo info = {};
		strcpy( info.Name, CurPhase->hero );
		info.Level    = 1;
		info.Class    = CurPhase->classId;
		info.GameMode = CurPhase->gameMode;
		int created = UiCreateHero( &info );
		Log( "created hero %s: %s", CurPhase->hero, created ? "ok" : "FAILED" );
	}else{
		Log( "hero %s found in slot %d", CurPhase->hero, AutoLoadSlot );
	}
}

// From the game loop, once per game tick
void AutoTest_Tick()
{
	if( ! Active || Finished || gbRunGame != 1 ) return;
	if( Steps.empty() ){
		Log( "in the game" );
		CurPhase->build();
	}
	IsShiftPressed = 0; IsCtrlPressed = false; ForceMove = false; // the PC's own keyboard doesn't count
	if( WaitTicks > 0 ){ --WaitTicks; return; }
	if( StepIndex >= Steps.size() ){ Finish(); return; }
	if( RetryTicks == 0 ) Log( "step: %s", Steps[StepIndex].name );
	int result = Steps[StepIndex].run();
	if( result == Retry ){
		if( ++RetryTicks > StepTimeoutTicks ){
			Check( Fmt( "step \"%s\" finishes", Steps[StepIndex].name ).c_str(), false, "timed out" );
			Finish();
		}
		return;
	}
	RetryTicks = 0;
	WaitTicks = result;
	++StepIndex;
}
