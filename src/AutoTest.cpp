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
//          5-second item check (autotest-rebuilds.csv), penalties and -light radius on
//          unique and set items,
//          the doubled stat cap, perk resets with Ostentation and with Here and Now, volume
//          sliders, unarmed damage,
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
//   oldwarrior, oldmage, oldarcher, oldclassic, oldveteran  heroes saved by v0.2.0
//          (tests\saves\v0.2.0): a Warrior, a Mage, an Archer, a Classic Mage and a level 120
//          Savage with many perks and gems. The hero and every item in the
//          inventory, Griswold's stash and the shared stash must be as v0.2.0 listed them in
//          oldsave-manifest.txt, and again after a save and load. The script runs them first,
//          each in its own save folders
//   ql*    F9 and F12 in each game mode, with a new hero and its own save folders: they must do
//          nothing wherever the menu's Save Game and Load Game are unavailable, and work where
//          they are available, also on quest floors, in a battle and while dying
//   features  Warrior, normal mode: Shift-click on a stat's +, the socket count in ground labels, the
//          extra sound of unique and set item drops, the 2x speed button and the sound of a cleared
//          level, BattleLockdown in a fight and in a quest area, and SigilDropRate. The window has
//          sound off, so it reads what PlayLocalSound was asked to play
//   oldconfig  a config.ini from before the options above: they read as their defaults
//   missingfile  asks for a file no archive has, like a TH4data.mor from an older TH4 would:
//          the error must name that file and TH4data.mor (bug 13)
//   nomor  the script removes TH4data.mor first: the game must stop with an error that names
//          it (bug 13). It runs last. AutoTest_Error reads the errors' text; nothing shows

extern StashTab StashTabs[Stash_TotalTabs];
extern Perk Perks[PERKS_COUNT];
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
const char* HeroNoData   = "AtNoData";
const char* HeroFeatures = "AtFeatures"; // the script deletes its saves after the phase
const char* MissingFile  = "autotest\\no archive has this.cel";
// Made by v0.2.0, see tests\saves\v0.2.0\README.md
const char* HeroOldWarrior = "OldWarrior";
const char* HeroOldMage    = "OldMage";
const char* HeroOldArcher  = "OldArcher";
const char* HeroOldClassic = "OldClassic";
const char* HeroOldVeteran = "OldVeteran";
const char* HeroQuick    = "AtQuick";    // the ql* phases; the script deletes the saves after each

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

// A plain item as a monster drops it: rebuilt by the full generator on every load, so the
// loading character's rules apply (sockets only for characters who can grind, gold for
// Night Kin and Clean Hands)
Item MakeMonsterDrop( int baseItemIndex, int seed, int itemLevel = 30 )
{
	int randomSeed = RandomSeed, lastSeed = LastSeed;
	uint randUsingCounter = RandUsingCounter;
	SetupAllItems( MaxItems_255, baseItemIndex, seed, itemLevel, 1 /*ChanceUniqueMob*/, 0, 0, 0, true, 0, 0, 0, -1, 0, 0 );
	LastItemInfo info;
	SaveItem( &info, &Items[MaxItems_255] );
	Item item;
	LoadItem( &info, &item, true );
	RandomSeed = randomSeed; LastSeed = lastSeed; RandUsingCounter = randUsingCounter;
	return item;
}

// Describe() without the names: some names depend on the class (a Warrior's Relict of
// Healing is a Mage's Scroll of Healing), the item doesn't
string WithoutNames( const string& described )
{
	size_t at = described.find( " | base " );
	return at == string::npos ? described : described.substr( at );
}

// Sockets the item had, from its Describe() line
int DescribedSockets( const string& described )
{
	size_t at = described.find( " sockets " );
	return at == string::npos ? 0 : atoi( described.c_str() + at + 9 );
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
	bool gemmed = false;
	for( short gem: item.sockets ) if( gem ) gemmed = true;
	if( gemmed ){
		s += " gems";
		for( short gem: item.sockets ) if( gem ) s += Fmt( " %d", (int)gem );
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

// Bug 9: how much of the pause menu title's brightness reaches the screen, from 0 to 1. Draws
// the title again at full brightness over a blanked copy of its band to find its pixels,
// compares them with what the frame shows (lit the way the screenshot lights it), then puts
// the band back
double PauseTitleBrightness()
{
	const char* title = "DIABLO 1 THE HEAVEN";
	const int x0 = Screen_LeftBorder + (ScreenWidth - GetTextWidthBig( title )) / 2;
	const int y0 = Screen_TopBorder + 40 + (ScreenHeight - GUI_Height) / 2;
	const int left = x0 - 8, width = GetTextWidthBig( title ) + 16, top = y0 - 60, rows = 70;
	vector<uchar> frame( width * rows ), light( width * rows ), glyph( width * rows );
	auto at = [&]( int r ){ return (uchar*)&WorkingSurface[left + YOffsetHashTable[top + r]]; };
	auto lightAt = [&]( int r ){ return LightSurface + (at( r ) - SoftSrcSurfacePtr); };
	for( int r = 0; r < rows; ++r ){
		memcpy( &frame[r * width], at( r ), width );
		if( LightSurface ) memcpy( &light[r * width], lightAt( r ), width );
		memset( at( r ), 0, width );
	}
	int dark = DarkLevel;
	DarkLevel = 0;
	Font_DrawText( x0, y0, (char*)title );
	DarkLevel = dark;
	for( int r = 0; r < rows; ++r ){
		memcpy( &glyph[r * width], at( r ), width );
		memcpy( at( r ), &frame[r * width], width );
		if( LightSurface ) memcpy( lightAt( r ), &light[r * width], width );
	}
	auto lum = []( uchar i ){ return ScreenPalette[i].peRed + ScreenPalette[i].peGreen + ScreenPalette[i].peBlue; };
	double shown = 0, full = 0;
	for( int i = 0; i < width * rows; ++i ){
		if( ! glyph[i] ) continue;
		full += lum( glyph[i] );
		shown += lum( frame[i] ) * (LightSurface ? 1.0 - light[i] / 255.0 : 1.0);
	}
	return full ? shown / full : 0;
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
set<int>         UnlockedForMage;
int              TestSeed = 0;
int              SavedStats[4] = {}, StatSumAtCap = 0, ElixirsDrunk = 0;
int              StatSumBeforeReset = 0, PerksAfterReset = 0;
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

	Steps.push_back( { "unique -light radius", []{
		// Darkbone115 (30 Mar, 15 Aug): -light radius on uniques helps archers and mages pull
		// fewer monsters, so players want it kept. Every unique or set item with -light radius,
		// worn by the hero, must make the light radius smaller than the same base item without
		// its affixes, and its tooltip must show the line
		Player& p = Hero();
		int withLight = 0, smaller = 0, shown = 0;
		string firstNot, firstHidden;
		for( size_t u = 0; u < count_UniqueItems; ++u ){
			const UniqueItem& uq = UniqueItems[u];
			if( uq.baseId <= 0 || (size_t)uq.baseId >= count_BaseItems || BaseItems[uq.baseId].baseId < 0 || uq.effectCount <= 0 ) continue;
			Item item = MakeUnique( (int)u, true ), plain = MakeUnique( (int)u, false );
			if( item.MagicLevel < ML_2_UNIQUE ) continue;
			int minus = 0, plus = 0, k = -1;
			for( int i = 0; i < (int)countof( item.effect ); ++i ){
				if( item.effect[i].id == AE_MINUS_LIGHT_RADIUS ){ minus += item.effect[i].minVal; k = i; }
				if( item.effect[i].id == AE_PLUS_LIGHT_RADIUS ) plus += item.effect[i].minVal;
			}
			if( minus <= plus ) continue;
			++withLight;
			Item saved = p.OnBodySlots[IS_LeftRing];
			p.OnBodySlots[IS_LeftRing] = plain;
			CalcCharParams( CurrentPlayerIndex, 0 );
			int radiusPlain = p.LightRadius;
			p.OnBodySlots[IS_LeftRing] = item;
			CalcCharParams( CurrentPlayerIndex, 0 );
			int radiusWorn = p.LightRadius;
			p.OnBodySlots[IS_LeftRing] = saved;
			CalcCharParams( CurrentPlayerIndex, 0 );
			if( radiusWorn < radiusPlain ) ++smaller;
			else if( firstNot.empty() ) firstNot = Fmt( "%s (unique %d): light radius %d, %d without its affixes", item.FullMagicalItemName, (int)u, radiusWorn, radiusPlain );
			Item savedInv = p.InventorySlots[0];
			int invCount = p.InvItemCount;
			p.InventorySlots[0] = item;
			if( invCount < 1 ) p.InvItemCount = 1;
			Tooltip_ShowItemInfo( IS_Inventory + 0 );
			p.InventorySlots[0] = savedInv;
			p.InvItemCount = invCount;
			bool found = false;
			if( PrintItemEffectToStringBuffer( AE_MINUS_LIGHT_RADIUS, &item, k ) ){
				string text = InfoPanelBuffer;
				for( int line = 0; line < Tooltip_LineCount() && ! found; ++line ) found = text == Tooltip_LineText( line );
			}
			if( found ) ++shown;
			else if( firstHidden.empty() ) firstHidden = Fmt( "%s (unique %d)", item.FullMagicalItemName, (int)u );
		}
		Tooltip_Clear();
		RecalcPlayer( CurrentPlayerIndex, true );
		if( ! Check( "uniques with -light radius exist", withLight > 0 ) ) return Next;
		Check( "uniques with -light radius make the light radius smaller (Darkbone115)", smaller == withLight,
			Fmt( "%d of %d. First that doesn't: ", smaller, withLight ) + firstNot );
		Check( "their tooltip shows -light radius", shown == withLight, Fmt( "%d of %d. First that doesn't: ", shown, withLight ) + firstHidden );
		return Next;
	} } );

	Steps.push_back( { "stat cap: Spectral Elixirs up to the cap", []{
		// Bug 5 (Rageweaver, 4 Apr): the per-level stat cap is doubled (cccf003), but a Spectral
		// Elixir drunk past the old cap raised the stats for a second, then they dropped back,
		// and capped stats never turned gold. Start from the lowest stats, drink elixirs until
		// the game refuses them, then let the game run for 2 seconds
		Player& p = Hero();
		short* base[4] = { &p.BaseStrength, &p.BaseMagic, &p.BaseDexterity, &p.BaseVitality };
		for( int s = 0; s < 4; ++s ){ SavedStats[s] = *base[s]; *base[s] = 15; }
		RecalcPlayer( CurrentPlayerIndex, true );
		Item elixir {};
		elixir.MagicCode = MC_44_SPECTRAL_ELIXIR;
		ElixirsDrunk = 0;
		while( ElixirsDrunk < 1000 && UsePotionOrScroll( CurrentPlayerIndex, &elixir ) ) ++ElixirsDrunk;
		StatSumAtCap = CalcCharBaseStatSum( p );
		return 2 * ENGINE_FPS;
	} } );

	Steps.push_back( { "stat cap: 2 seconds later", []{
		Player& p = Hero();
		int clvl = p.CharLevel;
		int doubled = ((10 * clvl) + 90 + (2 * clvl / 3)) * 2; // the changelog's formula; this hero has no traits
		int now = CalcCharBaseStatSum( p );
		Check( "Spectral Elixirs raise the stats to the doubled per-level cap (bug 5)", StatSumAtCap >= doubled,
			Fmt( "level %d: stat sum %d after %d elixirs, doubled cap %d", clvl, StatSumAtCap, ElixirsDrunk, doubled ) );
		Check( "the stats stay at the cap while the game runs (bug 5)", now >= doubled && now >= StatSumAtCap - 4,
			Fmt( "stat sum %d, %d right after the elixirs (an elixir's last +5 may go up to 4 past the cap)", now, StatSumAtCap ) );
		Check( "stats at the cap are drawn gold (bug 5)", CannotIncreaseStat( CurrentPlayerIndex ) == 1 );
		short* base[4] = { &p.BaseStrength, &p.BaseMagic, &p.BaseDexterity, &p.BaseVitality };
		for( int s = 0; s < 4; ++s ) *base[s] = SavedStats[s];
		RecalcPlayer( CurrentPlayerIndex, true );
		return Next;
	} } );

	Steps.push_back( { "perk reset with Ostentation", []{
		// Godroll locks Raise the Bar, Thaumaturgy and Ostentation against a perk reset:
		// they raise attribute limits, and lowering a limit cuts base stats above it without
		// refunding them. The hero takes Ostentation and another perk, drinks elixirs up to the
		// raised cap, then resets its perks at Griswold's for gold
		Player& p = Hero();
		short* base[4] = { &p.BaseStrength, &p.BaseMagic, &p.BaseDexterity, &p.BaseVitality };
		for( int s = 0; s < 4; ++s ){ SavedStats[s] = *base[s]; *base[s] = 15; }
		p.perk[SYNERGY_OSTENTATION] = 1;
		p.perk[PERK_SWIFT_LEARNER] = 1;
		RecalcPlayer( CurrentPlayerIndex, true );
		Item elixir {};
		elixir.MagicCode = MC_44_SPECTRAL_ELIXIR;
		for( int n = 0; n < 1000 && UsePotionOrScroll( CurrentPlayerIndex, &elixir ); ++n );
		StatSumBeforeReset = CalcCharBaseStatSum( p );
		VisualTrade_AddGoldToInventory( 1000 * p.CharLevel );
		VisualTrade_ResetPerks();
		PerksAfterReset = 0;
		for( int k = 0; k < PERKS_SYNERGY_MAX_750; ++k ) if( p.perk[k] ) ++PerksAfterReset;
		return 2 * ENGINE_FPS;
	} } );

	Steps.push_back( { "perk reset: 2 seconds later", []{
		Player& p = Hero();
		int now = CalcCharBaseStatSum( p );
		Check( "a perk reset doesn't take away base stats above the cap Ostentation raised", now >= StatSumBeforeReset - 4,
			Fmt( "stat sum %d before the reset, %d after", StatSumBeforeReset, now ) );
		Check( "the reset keeps Ostentation then, and resets the other perks", p.perk[SYNERGY_OSTENTATION] == 1 && p.perk[PERK_SWIFT_LEARNER] == 0,
			Fmt( "Ostentation %d, Swift Learner %d, %d perks left", (int)p.perk[SYNERGY_OSTENTATION], (int)p.perk[PERK_SWIFT_LEARNER], PerksAfterReset ) );
		p.perk[SYNERGY_OSTENTATION] = 0;
		short* base[4] = { &p.BaseStrength, &p.BaseMagic, &p.BaseDexterity, &p.BaseVitality };
		for( int s = 0; s < 4; ++s ) *base[s] = SavedStats[s];
		RecalcPlayer( CurrentPlayerIndex, true );
		// below the cap without it, nothing is lost: then the reset takes Ostentation too
		p.perk[SYNERGY_OSTENTATION] = 1;
		VisualTrade_AddGoldToInventory( 1000 * p.CharLevel );
		VisualTrade_ResetPerks();
		Check( "below the cap without Ostentation, the reset takes it too", p.perk[SYNERGY_OSTENTATION] == 0 );
		p.perk[SYNERGY_OSTENTATION] = 0;
		RecalcPlayer( CurrentPlayerIndex, true );
		return Next;
	} } );

	Steps.push_back( { "perk reset with Here and Now", []{
		// Here and Now grants a full level when it's taken. If a perk reset gave its point back,
		// it could be taken again for another level, as often as the reset is paid for. The hero
		// takes Swift Learner 5 (its requirement) and Here and Now, then resets its perks.
		// The hero is put back as it was afterwards
		Player& p = Hero();
		static Player saved;
		saved = p;
		p.perk[PERK_SWIFT_LEARNER] = 5;
		int level = p.CharLevel;
		SpendPerkPoint( PERK_HERE_AND_NOW );
		int levelTaken = p.CharLevel, held = p.perk[PERK_HERE_AND_NOW];
		VisualTrade_AddGoldToInventory( 1000 * p.CharLevel );
		VisualTrade_ResetPerks();
		int heldAfter = p.perk[PERK_HERE_AND_NOW], swiftAfter = p.perk[PERK_SWIFT_LEARNER];
		int spentAfter = getPlayerSumOfPerks( PERK_TYPE_COMMON );
		p = saved;
		getAvailablePerksList();
		RecalcPlayer( CurrentPlayerIndex, true );
		if( ! Check( "Here and Now gives a level", levelTaken == level + 1 && held == 1,
			Fmt( "level %d, then %d with Here and Now %d", level, levelTaken, held ) ) ) return Next;
		Check( "a perk reset keeps Here and Now, so its level can't be bought again", heldAfter == 1 && spentAfter == 1 && swiftAfter == 0,
			Fmt( "after the reset: Here and Now %d, Swift Learner %d, %d perk points still spent", heldAfter, swiftAfter, spentAfter ) );
		return Next;
	} } );

	Steps.push_back( { "perk reset with perks whose benefit stays (bug 12)", []{
		// Educated, Higher Education and Overwhelm add stat points at every level-up, and Heavy
		// Drinker makes unique monsters drop elixirs. A reset that refunded them let a player take
		// them while leveling, keep the points and elixirs, and swap them for other perks later.
		// Ways the fix can fail: the reset still refunds one of them, or lowers its rank; it keeps
		// their requirements too (Swift Learner, Obsession, Advanced User), which must still reset;
		// a kept perk stops working once its requirements are gone; its points come back as unspent.
		// Overwhelm exists only in Ironman, Nightmare and Speedrun, so the step plays Nightmare.
		// The hero is put back as it was afterwards
		Player& p = Hero();
		static Player saved;
		saved = p;
		const GAME_MODE mode = GameMode;
		GameMode = GM_NIGHTMARE;
		const int kept[] = { PERK_EDUCATED, PERK_HEAVY_DRINKER, SYNERGY_HIGHER_EDUCATION, SYNERGY_OVERWHELM };
		const int ranks[] = { 5, 5, 1, 1 };
		const int reset[] = { PERK_SWIFT_LEARNER, PERK_OBSESSION, PERK_ADVANCED_USER };
		for( int k = 0; k < PERKS_SYNERGY_MAX_750; ++k ) p.perk[k] = 0;
		p.perk[PERK_SWIFT_LEARNER] = 5; p.perk[PERK_OBSESSION] = 5; p.perk[PERK_ADVANCED_USER] = 1;
		for( int k = 0; k < 4; ++k ) p.perk[kept[k]] = ranks[k];
		auto benefit = []{ return Fmt( "%d stat points per level, %d%% elixir chance",
			PerkValue( PERK_EDUCATED, CurrentPlayerIndex ) + PerkValue( SYNERGY_HIGHER_EDUCATION, CurrentPlayerIndex )
			+ PerkValue( SYNERGY_OVERWHELM, CurrentPlayerIndex ), PerkValue( PERK_HEAVY_DRINKER, CurrentPlayerIndex ) ); };
		string before = benefit();
		VisualTrade_AddGoldToInventory( 1000 * p.CharLevel );
		VisualTrade_ResetPerks();
		string after = benefit(), keptNow, resetNow;
		bool allKept = true, allReset = true;
		for( int k = 0; k < 4; ++k ){
			allKept &= p.perk[kept[k]] == ranks[k];
			keptNow += Fmt( " %s %d", Perks[kept[k]].name, (int)p.perk[kept[k]] );
		}
		for( int r: reset ){
			allReset &= p.perk[r] == 0;
			resetNow += Fmt( " %s %d", Perks[r].name, (int)p.perk[r] );
		}
		int spent = getPlayerSumOfPerks( PERK_TYPE_COMMON ), spentSynergy = getPlayerSumOfPerks( PERK_TYPE_SYNERGY );
		GameMode = mode;
		p = saved;
		getAvailablePerksList();
		RecalcPlayer( CurrentPlayerIndex, true );
		Check( "a perk reset keeps Educated, Higher Education, Overwhelm and Heavy Drinker (bug 12)", allKept && spent == 10 && spentSynergy == 2,
			Fmt( "after the reset:%s; %d perk and %d synergy points still spent (10 and 2 kept)", keptNow.c_str(), spent, spentSynergy ) );
		Check( "the reset still resets their requirements", allReset, "after the reset:" + resetNow );
		Check( "the kept perks still work without their requirements", before == after, "before the reset " + before + ", after " + after );
		return Next;
	} } );

	Steps.push_back( { "Infernal Bargain with every trait (bug 14)", []{
		// Infernal Bargain gets "X more perk points" for lower regeneration and stats. Ways this
		// can fail: a trait with its own perk point formula (Skilled, Lord of the Rings, Grim
		// Deal) gets the penalties but no points; a fix adds the points twice for heroes without
		// those traits; the points don't follow the synergy's rank. The hero has no perks here,
		// so the points left are the points given. It is put back as it was afterwards
		Player& p = Hero();
		static Player saved;
		saved = p;
		struct { const char* name; TraitId trait; } traits[] = { { "no trait", TraitId::NoTrait }, { "Skilled", TraitId::Skilled },
			{ "Lord of the Rings", TraitId::LordOfTheRings }, { "Grim Deal", TraitId::GrimDeal } };
		for( int k = 0; k < PERKS_SYNERGY_MAX_750; ++k ) p.perk[k] = 0;
		p.traits[0] = p.traits[1] = TraitId::NoTrait;
		string wrong, all;
		int ranks[] = { 1, Perks[SYNERGY_INFERNAL_BARGAIN].levels };
		for( auto& t: traits ){
			p.traits[0] = t.trait;
			p.perk[SYNERGY_INFERNAL_BARGAIN] = 0;
			int base = getPlayerPerksPointsLeft( PERK_TYPE_COMMON );
			for( int rank: ranks ){
				p.perk[SYNERGY_INFERNAL_BARGAIN] = rank;
				int want = base + PerkValue( SYNERGY_INFERNAL_BARGAIN, CurrentPlayerIndex, 0 );
				int got = getPlayerPerksPointsLeft( PERK_TYPE_COMMON );
				string line = Fmt( "  %s, rank %d: %d points, %d without it, should be %d", t.name, rank, got, base, want );
				all += line;
				if( got != want ) wrong += line;
			}
		}
		p = saved;
		getAvailablePerksList();
		RecalcPlayer( CurrentPlayerIndex, true );
		Check( "Infernal Bargain gives its perk points with every trait (bug 14)", wrong.empty(), wrong.empty() ? all : wrong );
		return Next;
	} } );

	Steps.push_back( { "volume sliders through a restart", []{
		// Rageweaver (antigrind, 3 Nov): sound and music go up one bar every time the game
		// restarts. For each bar of the sound slider: the volume it sets is saved on quit, the
		// next start reads it back, and the slider shows it again. It must be the same bar.
		// The autotest runs without sound, so the registry isn't touched and the sliders are
		// switched on here just for the check
		bool allocated = IsDirectSoundAllocated;
		IsDirectSoundAllocated = true;
		Menu menu = OptionsMenu[OP_SOUND];
		SetVolumeMenu( SoundMenu, &menu, VOL_MIN );
		int bars = (menu.Flags >> 12) & 0xFFF, moved = 0;
		string first;
		for( int bar = 0; bar <= bars; ++bar ){
			menu.Flags = (menu.Flags & ~0xFFFu) | bar;
			int saved = Menu_GetVolume( &menu );
			int loaded = LimitVolume( saved );
			SetVolumeMenu( SoundMenu, &menu, loaded );
			int shown = menu.Flags & 0xFFF;
			if( shown != bar && ++moved == 1 ) first = Fmt( "bar %d: volume %d saved, %d read back, shown at bar %d", bar, saved, loaded, shown );
		}
		IsDirectSoundAllocated = allocated;
		Check( "the volume sliders stay on their bar through a restart", bars > 2 && moved == 0,
			Fmt( "%d of %d bars move. ", moved, bars + 1 ) + first );
		return Next;
	} } );

	Steps.push_back( { "unarmed damage with Plate Harness", []{
		// Bug 10 (Gorvex): unarmed damage loses its base when an item changes base damage, like
		// Plate Harness (-5): a Shugoki loses about 37 damage. A level 75 Monk with empty hands,
		// without armour and then in Plate Harness: the harness must change the damage by its own -5
		Player& p = Hero();
		int harnessBase = 0;
		for( int i = 1; i < (int)count_BaseItems && ! harnessBase; ++i ) if( BaseItems[i].NamePtr && ! strcmp( BaseItems[i].NamePtr, "Plate Harness" ) ) harnessBase = i;
		if( ! Check( "Plate Harness exists", harnessBase > 0 ) ) return Next;
		Item harness = MakeItem( harnessBase, 7100, false, 0 );
		harness.IsReqMet = true;
		PLAYER_CLASS classId = p.ClassID;
		int level = p.CharLevel;
		Item left = p.OnBodySlots[IS_LeftHand], right = p.OnBodySlots[IS_RightHand], torso = p.OnBodySlots[IS_Torso];
		p.ClassID = PC_3_MONK;
		p.CharLevel = 75;
		p.OnBodySlots[IS_LeftHand].ItemCode = p.OnBodySlots[IS_RightHand].ItemCode = p.OnBodySlots[IS_Torso].ItemCode = IC_M1_NONE;
		CalcCharParams( CurrentPlayerIndex, 0 );
		int minBare = p.MinDamageFromItem, maxBare = p.MaxDamageFromItem;
		p.OnBodySlots[IS_Torso] = harness;
		CalcCharParams( CurrentPlayerIndex, 0 );
		int minHarness = p.MinDamageFromItem, maxHarness = p.MaxDamageFromItem;
		p.ClassID = classId; p.CharLevel = level;
		p.OnBodySlots[IS_LeftHand] = left; p.OnBodySlots[IS_RightHand] = right; p.OnBodySlots[IS_Torso] = torso;
		RecalcPlayer( CurrentPlayerIndex, true );
		Check( "unarmed, Plate Harness changes the damage only by its own base damage (bug 10)",
			minHarness == minBare + harness.MinDamage && maxHarness == maxBare + harness.MaxDamage,
			Fmt( "level 75 Monk: %d-%d without armour, %d-%d in Plate Harness (%d-%d)", minBare, maxBare, minHarness, maxHarness, harness.MinDamage, harness.MaxDamage ) );
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
		// StArchon's cases (godroll), on the second tab (the game fills only the open tab):
		// white monster drops roll their sockets again on every load, and only for a character
		// who can grind; Night Kin and Clean Hands turn them into gold
		StashCurrentTab = 1;
		int socketed = 0, unsocketed = 0;
		for( int seed = 6000; seed < 7000 && (socketed < 3 || unsocketed < 3); ++seed ){
			Item it = MakeMonsterDrop( armour[seed % armour.size()], seed );
			if( it.MagicLevel != ML_0_USUAL ) continue;
			int& count = it.socketsAmount > 0 ? socketed : unsocketed;
			if( count < 3 && stash( it ) ) ++count;
		}
		// Shop items are rebuilt with the loading character's shop rules: Griswold's premium
		// items and Wirt's for its level, Pepin's from its shop pool
		int shop = 0;
		for( int i = 0; i < 3; ++i ) if( stash( MagicItemsStoreArray[i] ) ) ++shop;
		if( stash( WirtItem ) ) ++shop;
		for( int i = 2; i < 5; ++i ) if( stash( PepinItems[i] ) ) ++shop; // the first two are always the same potions
		for( int i = 0; i < 2; ++i ) if( stash( BasicalItemsArray[i] ) ) ++shop;
		StashCurrentTab = 0;
		if( f ) fclose( f );
		Check( "the Warrior stashes plain armour and magic staves, books, rings and armour in the shared stash",
			plain >= 2 && magic >= 10, Fmt( "%d plain and %d magic items stashed", plain, magic ) );
		Check( "the Warrior stashes white monster drops, with and without sockets, and shop items",
			socketed > 0 && unsocketed > 0 && shop > 0, Fmt( "%d socketed, %d without sockets, %d from shops", socketed, unsocketed, shop ) );
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

	// Bug 9 (issue #3): the pause menu title "can't be seen" in some zones. The menu over one
	// level of each zone: the title must reach the screen at full brightness, and a screenshot
	struct Zone { DUNGEON level; const char* name; };
	for( Zone z: { Zone{ DUN_1_CHURCH_1, "church" }, Zone{ DUN_5_CATACOMB_1, "catacombs" }, Zone{ DUN_9_CAVE_1, "caves" }, Zone{ DUN_13_HELL_1, "hell" } } ){
		if( z.level != DUN_1_CHURCH_1 ) Steps.push_back( { "go to the next zone", [z]{
			StartNewLvl( CurrentPlayerIndex, WM_1026_NEXT_DUNGEON, z.level );
			return Next;
		} } );
		Steps.push_back( { "pause menu screenshot", [z]{
			static int ticks = 0;
			if( (DUNGEON)Dungeon != z.level ) return Retry;
			if( ticks == 0 ) SelectMainMenu();
			if( ++ticks < 20 ) return Retry;
			ticks = 0;
			Shot( Fmt( "pause-menu-%s", z.name ).c_str() );
			bool open = IsGameMenuActive();
			double bright = PauseTitleBrightness();
			Menu_SetupMenuWindow( 0, 0 );
			Check( Fmt( "the pause menu opens in the %s", z.name ).c_str(), open );
			Check( Fmt( "the pause menu title is at full brightness in the %s (bug 9)", z.name ).c_str(), bright > 0.95,
				Fmt( "%.0f%% of its brightness reaches the screen", bright * 100 ) );
			return Next;
		} } );
	}

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
		int shopItems = 0, shopDiffer = 0, shopLocked = 0, socketedWhite = 0, socketedLocked = 0;
		string example, wrong;
		for( int t = 0; t < Stash_TotalTabs; ++t ){
			for( int i = 0; i < StashTabs[t].itemCount; ++i ){
				const Item& item = StashTabs[t].items[i];
				auto listed = Manifest.find( item.seed );
				if( listed == Manifest.end() ) continue;
				bool differs = WithoutNames( Describe( item ) ) != WithoutNames( listed->second );
				bool locked = SharedStash_IsLocked( item );
				if( ! locked ) UnlockedForMage.insert( item.seed );
				if( item.dropType & D_SHOP ){ ++shopItems; if( differs ) ++shopDiffer; if( differs && locked ) ++shopLocked; }
				if( item.MagicLevel == ML_0_USUAL && DescribedSockets( listed->second ) > 0 ){
					++socketedWhite;
					if( differs && locked ) ++socketedLocked;
				}
				if( differs ){
					++morphed;
					if( locked ) ++morphedLocked;
					else if( wrong.empty() ) wrong = "not locked: " + Describe( item ) + "  was: " + listed->second;
					if( example.empty() ) example = "Warrior's: " + listed->second + "  Mage's: " + Describe( item );
				}else if( ! (item.dropType & D_SHOP) ){
					// Not shop items: their stock is random, and one can swap an affix for another with
					// the same effects and another name, which isn't in this comparison but is rightly locked
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
		Check( "white items whose sockets the Mage can't have (no grinding in Classic) are locked", socketedWhite > 0 && socketedLocked == socketedWhite,
			Fmt( "%d of %d locked", socketedLocked, socketedWhite ) );
		Check( "shop items that would come out different are locked", shopItems > 0 && shopLocked == shopDiffer,
			Fmt( "%d shop items, %d would come out different, %d of them locked", shopItems, shopDiffer, shopLocked ) );
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

	// StArchon (godroll): Night Kin and Clean Hands turn equipment into gold when they load
	// it, and it stays gold once they save. The owner phase then checks the Warrior gets
	// every item back as it was
	Steps.push_back( { "open the shared stash as a Night Kin", []{
		Hero().gameChanger |= BIT( GC_9_NIGHT_KIN );
		return OpenSharedStash();
	} } );

	Steps.push_back( { "compare as a Night Kin", []{
		int gold = 0, goldLocked = 0, differs = 0, differsLocked = 0;
		string wrong;
		for( int t = 0; t < Stash_TotalTabs; ++t ){
			for( int i = 0; i < StashTabs[t].itemCount; ++i ){
				const Item& item = StashTabs[t].items[i];
				auto listed = Manifest.find( item.seed );
				if( listed == Manifest.end() || WithoutNames( Describe( item ) ) == WithoutNames( listed->second ) ) continue;
				bool locked = SharedStash_IsLocked( item );
				++differs;
				if( locked ) ++differsLocked;
				else if( wrong.empty() ) wrong = "not locked: " + Describe( item ) + "  was: " + listed->second;
				if( item.ItemCode == IC_11_GOLD && UnlockedForMage.count( item.seed ) ){ ++gold; if( locked ) ++goldLocked; }
			}
		}
		Check( "for a Night Kin, equipment the Mage could take turns into gold, and is locked", gold > 0 && goldLocked == gold,
			Fmt( "%d of %d locked", goldLocked, gold ) );
		Check( "for a Night Kin, every item that would come out different is locked", differsLocked == differs,
			Fmt( "%d of %d locked. ", differsLocked, differs ) + wrong );
		return Next;
	} } );

	Steps.push_back( { "close the shared stash as a Night Kin", []{
		int result = CloseSharedStash();
		Hero().gameChanger &= ~BIT( GC_9_NIGHT_KIN );
		return result;
	} } );
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
		Check( "every item is exactly as the Warrior stashed it, after the Mage opened the stash, also as a Night Kin", changed == 0,
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
		Check( "the Warrior can take an item out, unchanged", taken && at >= 0 && Describe( Hero().InventorySlots[at] ) == Manifest[seed],
			Fmt( "taken %d, in the inventory at %d: ", (int)taken, at ) + (at >= 0 ? Describe( Hero().InventorySlots[at] ) : string()) + "  was: " + Manifest[seed] );
		return Next;
	} } );

	Steps.push_back( { "close shared stash", CloseSharedStash } );
}

//----- phases: heroes saved by v0.2.0 --------------------------------------
// Players upgrade with their v0.2.0 characters, Griswold's stash and the shared stash. Ways this
// can fail, for each hero, on load and again after a save and load in this version:
// - the game doesn't find the hero and makes a new one: a Classic save has its own folder, and
//   TH4 v0.774 renamed Classic saves from th3cls_ to th4cls_
// - TH4 moved perks or spells between v0.2.0's base and this one: a rank lands on another perk, a
//   spell level on another spell. That's why the list has their names as well as their numbers
// - a perk's rank or a stat above a limit of this version gets cut
// - books and staves come out with another spell for a Mage or an Archer
// - Classic rebuilds an item with the wrong affix table
// - sockets are rolled again, or a gem vanishes or turns into another gem
// - the rare, unique and magic items of a high level, high difficulty come out different
// - the test fools itself: a save that doesn't hold what it's for, so the lists are checked too

struct OldItem { string where; int seed; string described; };
vector<OldItem> OldItems;
string          OldHero, OldPerks, OldSpells;

void ReadOldManifest()
{
	FILE* f = fopen( "oldsave-manifest.txt", "r" );
	if( ! f ) return;
	char line[4096];
	while( fgets( line, sizeof line, f ) ){
		string s = line;
		while( ! s.empty() && (s.back() == '\n' || s.back() == '\r') ) s.pop_back();
		size_t a = s.find( '\t' ), b = a == string::npos ? a : s.find( '\t', a + 1 );
		if( b == string::npos ) continue;
		OldItem it{ s.substr( 0, a ), atoi( s.c_str() + a + 1 ), s.substr( b + 1 ) };
		if( it.where == "hero" ) OldHero = it.described;
		else if( it.where == "perks" ) OldPerks = it.described;
		else if( it.where == "spells" ) OldSpells = it.described;
		else OldItems.push_back( it );
	}
	fclose( f );
}

// The hero line v0.2.0 wrote, from this hero
string HeroLine()
{
	Player& p = Hero();
	return Fmt( "%s class %d mode %d level %d str %d mag %d dex %d vit %d available %d xp %lld", p.playerName, (int)p.ClassID,
		(int)GameMode, (int)p.CharLevel, p.BaseStrength, p.BaseMagic, p.BaseDexterity, p.BaseVitality, p.AvailableLvlPoints, (long long)p.Xp );
}

// The manifest's items in one place (inventory, personal or shared) that are missing or come
// out different; ok counts the ones that are exactly as v0.2.0 had them
string HeroPerks()
{
	string s;
	for( int k = 0; k < PERKS_COUNT; ++k ) if( Hero().perk[k] ) s += Fmt( " %d:%d:%s", k, (int)Hero().perk[k], Perks[k].name );
	return s.empty() ? "none" : s.substr( 1 );
}

string HeroSpells()
{
	string s;
	for( int k = 1; k < PS_COUNT && k < 128; ++k ) if( Hero().SpellLevels[k] ) s += Fmt( " %d:%d:%s", k, (int)Hero().SpellLevels[k], Spells[k].Name );
	return s.empty() ? "none" : s.substr( 1 );
}

// TH4 v0.774 renamed these affixes, with the same effect
string WithNewNames( string described )
{
	const string renames[][2] = { { " of Vampires", " of the Vampire" }, { "/ crimson ", "/ Crimson " } };
	for( auto& rename: renames ){
		for( size_t at; (at = described.find( rename[0] )) != string::npos; ) described.replace( at, rename[0].size(), rename[1] );
	}
	return described;
}

// TH4 v0.774 added rows to the Classic affix table, so these Classic magic items roll another
// affix after the upgrade (bug 16, docs\OPEN_BUGS.md). They must still be there as the same base
// item, with the same durability and requirements. Any other item that changes fails the check
const set<int> RerolledInClassic = { 1102, 12002, 12003, 22002, 22003 };

// The part of a Describe() line about the base item: from " | base " to the effects
string BasePart( const string& described )
{
	size_t from = described.find( " | base " ), to = from == string::npos ? from : described.find( " |", from + 3 );
	return to == string::npos ? "" : described.substr( from, to - from );
}

string CompareOld( const char* where, int& ok, int& total )
{
	bool stash = strcmp( where, "inventory" ) != 0;
	if( ! strcmp( where, "personal" ) ) StashPanel_OpenPersonal();
	if( ! strcmp( where, "shared" ) ) StashPanel_OpenShared();
	string wrong;
	for( const OldItem& it: OldItems ){
		if( it.where != where ) continue;
		++total;
		const Item* found = nullptr;
		if( stash ){
			vector<int> at = FindInStash( it.seed );
			if( ! at.empty() ) found = &StashTabs[at[0] / Stash_MaxSlot].items[at[0] % Stash_MaxSlot];
		}else{
			int at = FindInInventory( it.seed );
			if( at >= 0 ) found = &Hero().InventorySlots[at];
		}
		if( found && Describe( *found ) == WithNewNames( it.described ) ){ ++ok; continue; }
		if( found && ! strcmp( CurPhase->name, "oldclassic" ) && RerolledInClassic.count( it.seed ) && BasePart( Describe( *found ) ) == BasePart( it.described ) ){
			++ok;
			wrong += Fmt( "  known reroll [%d] %s  was: %s", it.seed, Describe( *found ).c_str(), it.described.c_str() );
			continue;
		}
		wrong += Fmt( "  [%d] %s  was: %s", it.seed, found ? Describe( *found ).c_str() : "missing", it.described.c_str() );
	}
	if( stash ) StashPanel_Close();
	return wrong;
}

void CheckOldItems( const char* when )
{
	for( const char* where: { "inventory", "personal", "shared" } ){
		int ok = 0, total = 0;
		string wrong = CompareOld( where, ok, total );
		const char* place = ! strcmp( where, "inventory" ) ? "the inventory" : ! strcmp( where, "personal" ) ? "Griswold's stash" : "the shared stash";
		Check( Fmt( "v0.2.0 save, %s: every item in %s is as v0.2.0 had it", when, place ).c_str(), total > 0 && ok == total,
			Fmt( "%d of %d the same", ok, total ) + wrong );
	}
}

void CheckOldHero( const char* when )
{
	Check( Fmt( "v0.2.0 save, %s: the hero has the level, stats and points v0.2.0 saved", when ).c_str(), ! OldHero.empty() && HeroLine() == OldHero,
		HeroLine() + "  was: " + OldHero );
	// The Warrior's list is older than these lines; every other list must have them
	if( ! strcmp( CurPhase->name, "oldwarrior" ) ) return;
	Check( Fmt( "v0.2.0 save, %s: every perk has the rank v0.2.0 saved", when ).c_str(), ! OldPerks.empty() && HeroPerks() == OldPerks,
		HeroPerks() + "  was: " + OldPerks );
	Check( Fmt( "v0.2.0 save, %s: every spell has the level v0.2.0 saved", when ).c_str(), ! OldSpells.empty() && HeroSpells() == OldSpells,
		HeroSpells() + "  was: " + OldSpells );
}

void BuildOldSave()
{
	ReadOldManifest();
	AddStartSteps( CurPhase->gameMode );
	Steps.push_back( { "the save holds what it's for", []{
		int gemmedPersonal = 0, gemmedShared = 0, books = 0, spellStaves = 0;
		for( const OldItem& it: OldItems ){
			bool gemmed = it.described.find( " gems " ) != string::npos;
			if( gemmed && it.where == "personal" ) ++gemmedPersonal;
			if( gemmed && it.where == "shared" ) ++gemmedShared;
			if( it.described.find( " spell 0 " ) == string::npos ) ( it.described.find( "Book" ) != string::npos ? books : spellStaves )++;
		}
		string counts = Fmt( "%d gemmed in Griswold's stash, %d in the shared stash, %d books, %d staves with a spell. Perks: %s. Spells: %s",
			gemmedPersonal, gemmedShared, books, spellStaves, OldPerks.c_str(), OldSpells.c_str() );
		if( ! strcmp( CurPhase->name, "oldveteran" ) ){
			int perks = (int)count( OldPerks.begin(), OldPerks.end(), ' ' );
			Check( "v0.2.0 save: the veteran has many perks, and gems in both stashes", perks >= 20 && gemmedPersonal >= 2 && gemmedShared >= 2, counts );
		}else if( strcmp( CurPhase->name, "oldwarrior" ) ){
			Check( "v0.2.0 save: the hero has books and staves whose spell depends on his class", books >= 3 && spellStaves >= 3, counts );
		}
		return Next;
	} } );
	Steps.push_back( { "old hero", []{ CheckOldHero( "loaded" ); return Next; } } );
	Steps.push_back( { "old items", []{ CheckOldItems( "loaded" ); return Next; } } );
	Steps.push_back( { "save and load in this version", []{
		QuickSave();
		KeyPressHandler( VK_123_F12_KEY );
		return 20;
	} } );
	Steps.push_back( { "old items after a save and load", []{
		CheckOldHero( "saved and loaded again" );
		CheckOldItems( "saved and loaded again" );
		return Next;
	} } );
}

//----- phase: nomor ------------------------------------------------------
// The game must never get this far without TH4data.mor: AutoTest_Error ends the run

void BuildNoMor()
{
	Steps.push_back( { "in the game without TH4data.mor", []{
		Check( "without TH4data.mor, the game stops with an error that names it (bug 13)", false, "it got into the game without any error" );
		return Next;
	} } );
}

//----- phase: missingfile -----------------------------------------------
// File_Open with a name no archive has goes the same way as a file an older TH4data.mor lacks.
// AutoTest_Error ends the run at the dialog

void BuildMissingFile()
{
	Steps.push_back( { "open a file no archive has", []{
		HANDLE file;
		File_Open( MissingFile, &file, 0 );
		Check( "a file missing from the archives gives an error that names it and TH4data.mor (bug 13)", false, "no error" );
		return Next;
	} } );
}

//----- phases: ql* (F9 quick save and F12 quick load) --------------------
// F9 and F12 must do nothing wherever the menu's Save Game and Load Game are unavailable, and work
// where they are available. Each phase plays a real hero of that game mode. Ways this can fail:
// - no saved game: F12 reads a game file that isn't there, and the game ends with a fatal error
// - Ironman, Outbreak, Nightmare and Speedrun forbid loading, even with a save: F12 loads anyway
// - Hardcore forbids it in a battle and while dying: F12 takes back the battle or the death
// - Auto Save and Asceticism play with the menu that has neither item: F9 and F12 work anyway
// - F9 ignores the menu's rules: Ironman's Save & Quit only, monsters left in Survival and Outbreak,
//   quest floors, Nightmare and Speedrun after Uber Diablo
// - the fix blocks too much: F12 no longer loads a normal save, or a save while dying in Normal,
//   where the menu allows it; F9 no longer saves in town, on a normal level, on an Easy quest floor
//   or in the Dark Campaign, where every mission is a quest floor
// - the test fools itself: a menu left open eats the key, which looks like "did nothing", so every
//   press comes with the menu closed; and qlnormal shows that the key does work where it may
// F12's marker is an item the save doesn't have: a load takes it away. F9's is the save file's time.

constexpr int MenuSave = 0, MenuLoad = 3; // items of SPMainMenu
int MarkerSeed = 8000;

void AddMarker()
{
	Item it = MakeItem( FindBases( IsRing, 1 ).at( 0 ), ++MarkerSeed, true );
	MarkerSeed = it.seed;
	Hold( it );
	PutHeldInInventory();
}

bool HasMarker() { return FindInInventory( MarkerSeed ) >= 0; }

// A menu item as the player finds it: opens the menu, reads the item, closes the menu
bool MenuOffers( int item )
{
	SelectMainMenu();
	bool offered = CurrentMenuWindowPtr == SPMainMenu && (SPMainMenu[item].Flags & MENU_ENABLED);
	Menu_SetupMenuWindow( 0, 0 );
	return offered;
}

// A save as the mode's own rules make it: Ironman's Save & Quit, or after Uber Diablo in Nightmare
// and Speedrun, whose saving is off until then
void MakeSave()
{
	bool noSave = EnforceNoSave;
	EnforceNoSave = false;
	SaveGame();
	EnforceNoSave = noSave;
	Check( "the hero has a saved game", gbValidSaveFile );
}

// A monster that has noticed the hero: the game counts it as a battle
void StartBattle()
{
	for( int i = SummonMonsters_Count; i < MonstersCount; ++i ){
		Monster& monster = Monsters[MonsterIndexes[i]];
		if( ! monster.speechIndex ){ monster.ActivationCounter = 255; break; }
	}
	Check( "a monster has noticed the hero", IsSomeMonstersActivated() );
}

// So what stops F9 on this level can't be a monster that noticed the hero as it arrived
void EndBattle()
{
	for( int i = SummonMonsters_Count; i < MonstersCount; ++i ) Monsters[MonsterIndexes[i]].ActivationCounter = 0;
	Check( "no monster has noticed the hero", ! IsSomeMonstersActivated() );
}

// Asks the menu, presses F9, and the save file's time shows whether it saved
void AddF9( const char* when, bool mustSave, void (*setup)() = nullptr )
{
	Steps.push_back( { when, [=]{
		if( setup ) setup();
		Check( Fmt( "the menu's Save Game %s is %s", when, mustSave ? "available" : "not available" ).c_str(), MenuOffers( MenuSave ) == mustSave );
		i64 before = SaveFileTime();
		KeyPressHandler( VK_120_F9_KEY );
		bool saved = SaveFileTime() != before;
		Check( Fmt( mustSave ? "F9 %s saves" : "F9 %s does nothing", when ).c_str(), saved == mustSave, saved ? "the save file was written" : "the save file wasn't written" );
		return Next;
	} } );
}

// Asks the menu, presses F12, and 20 ticks later the marker shows whether it loaded
void AddF12( const char* when, bool mustLoad, void (*setup)() = nullptr )
{
	Steps.push_back( { when, [=]{
		AddMarker();
		if( setup ) setup();
		Check( Fmt( "the menu's Load Game %s is %s", when, mustLoad ? "available" : "not available" ).c_str(), MenuOffers( MenuLoad ) == mustLoad );
		KeyPressHandler( VK_123_F12_KEY );
		return 20;
	} } );
	Steps.push_back( { when, [=]{
		Check( Fmt( mustLoad ? "F12 %s loads the save" : "F12 %s does nothing", when ).c_str(), HasMarker() != mustLoad,
			Fmt( "the marker item the save doesn't have is %s", HasMarker() ? "still there: no load" : "gone: it loaded" ) );
		return Next;
	} } );
}

void AddGoTo( DUNGEON level, uint message )
{
	Steps.push_back( { "change level", [=]{
		if( (DUNGEON)Dungeon != level ) StartNewLvl( CurrentPlayerIndex, message, level );
		return Next;
	} } );
	Steps.push_back( { "on the new level", [=]{ return (DUNGEON)Dungeon != level ? Retry : 20; } } );
}

void AddGoToChurch()     { AddGoTo( DUN_1_CHURCH_1, WM_1026_NEXT_DUNGEON ); }
void AddGoToQuestFloor() { AddGoTo( DUN_25_KING_LEORICS_TOMB, WM_1029_GOTO_QUEST_DUNGEON ); }

void AddKeysWhileDying( bool mustLoad )
{
	Steps.push_back( { "the hero dies", []{
		AddMarker();
		TryToDie( CurrentPlayerIndex, 0 );
		return 5;
	} } );
	Steps.push_back( { "F9 and F12 during the death animation", [=]{
		Check( "the hero is in the death animation", Hero().CurAction == PCA_8_DEATH && ! IsPlayerDead,
			Fmt( "action %d, dead %d", (int)Hero().CurAction, (int)IsPlayerDead ) );
		i64 before = SaveFileTime();
		KeyPressHandler( VK_120_F9_KEY );
		Check( "F9 during the death animation does nothing", SaveFileTime() == before );
		Check( Fmt( "the menu's Load Game during the death animation is %s", mustLoad ? "available" : "not available" ).c_str(), MenuOffers( MenuLoad ) == mustLoad );
		KeyPressHandler( VK_123_F12_KEY );
		return 20;
	} } );
	Steps.push_back( { "F12 during the death animation: result", [=]{
		bool dying = Hero().CurAction == PCA_8_DEATH || IsPlayerDead;
		Check( mustLoad ? "F12 during the death animation loads the save, as the menu's Load Game does" : "F12 during the death animation does nothing: the hero stays dead",
			HasMarker() != mustLoad && dying != mustLoad, Fmt( "marker %s, hero %s", HasMarker() ? "there" : "gone", dying ? "dying" : "alive" ) );
		return Next;
	} } );
}

// A new hero: nothing saved yet, or never saved in Nightmare and Speedrun
void BuildQuickLoadNoSave()
{
	AddStartSteps( CurPhase->gameMode );
	Steps.push_back( { "no saved game", []{
		Check( "the new hero has no saved game", ! gbValidSaveFile );
		return Next;
	} } );
	AddF12( "with no saved game", false );
	Steps.push_back( { "screenshot", []{ Shot( "after-f12" ); return Next; } } );
}

// The modes whose menu never loads. Outbreak's saves with no monsters left, the others' never
void BuildQuickLoadForbidden()
{
	const GAME_MODE mode = CurPhase->gameMode;
	AddStartSteps( mode );
	AddF9( "in town", mode == GM_OUTBREAK );
	Steps.push_back( { "a save", []{ MakeSave(); return Next; } } );
	AddF12( "with a save, in a mode that forbids loading", false );
	Steps.push_back( { "screenshot", []{ Shot( "after-f12" ); return Next; } } );
	if( is( mode, GM_NIGHTMARE, GM_SPEEDRUN ) ){
		// Their menu then has Save & Quit, not Save Game, but opening it lifts EnforceNoSave
		AddF9( "after Uber Diablo", false, []{ UberDiabloKilled = true; } );
		Steps.push_back( { "before Uber Diablo again", []{ UberDiabloKilled = false; EnforceNoSave = true; return Next; } } );
	}
	AddGoToChurch();
	if( mode == GM_OUTBREAK ) AddF9( "with monsters left on the level", false, EndBattle );
	AddKeysWhileDying( false );
}

// Hardcore's menu saves and loads only with no battle going on and a living hero
void BuildQuickLoadHardcore()
{
	AddStartSteps( GM_HARDCORE );
	AddF9( "with no battle going on", true );
	AddF12( "with no battle going on", true );
	AddGoToChurch();
	AddF9( "during a battle", false, StartBattle );
	AddF12( "during a battle", false, StartBattle );
	AddKeysWhileDying( false );
}

// Survival's menu loads like Normal's, but saves only with no monsters left
void BuildQuickSaveSurvival()
{
	AddStartSteps( GM_SURVIVAL );
	AddF9( "in town", true );
	AddF12( "with a save", true );
	AddGoToChurch();
	AddF9( "with monsters left on the level", false, EndBattle );
}

// The modes that save on quest floors and, as these never count a battle, during one too.
// The Dark Campaign starts on one; a hero there has no monster to notice it
void BuildQuickSaveAlways()
{
	AddStartSteps( CurPhase->gameMode );
	if( CurPhase->gameMode == GM_EASY ) AddGoToQuestFloor();
	AddF9( "on a quest floor", true );
	AddF12( "with a save", true );
	if( CurPhase->gameMode == GM_EASY ) AddF9( "during a battle", true, StartBattle );
	Steps.push_back( { "screenshot", []{ Shot( "quest-floor" ); return Next; } } );
}

void BuildQuickLoadNormal()
{
	AddStartSteps( GM_NORMAL );
	AddF9( "in town", true );
	AddF12( "with a save", true );
	AddF9( "with the Auto Save game changer", false, []{ Hero().gameChanger |= BIT( GC_1_AUTO_SAVE ); } );
	AddF12( "with the Auto Save game changer", false, []{ Hero().gameChanger |= BIT( GC_1_AUTO_SAVE ); } );
	Steps.push_back( { "next game changer", []{ Hero().gameChanger &= ~BIT( GC_1_AUTO_SAVE ); return Next; } } );
	AddF9( "with the Asceticism game changer", false, []{ Hero().gameChanger |= BIT( GC_26_ASCETICISM ); } );
	AddF12( "with the Asceticism game changer", false, []{ Hero().gameChanger |= BIT( GC_26_ASCETICISM ); } );
	Steps.push_back( { "no game changer", []{ Hero().gameChanger &= ~BIT( GC_26_ASCETICISM ); return Next; } } );
	AddGoToChurch();
	AddF9( "on a normal level with monsters left", true, EndBattle );
	AddF9( "during a battle", false, StartBattle );
	Steps.push_back( { "the battle ends", []{ EndBattle(); return Next; } } );
	Steps.push_back( { "a save in the dungeon", []{ MakeSave(); return Next; } } );
	AddKeysWhileDying( true );
	AddGoToQuestFloor();
	AddF9( "on a quest floor", false, EndBattle );
	AddF12( "on a quest floor", true );
	Steps.push_back( { "the hero dies for good", []{
		static bool killed = false;
		if( ! killed ){ TryToDie( CurrentPlayerIndex, 0 ); killed = true; }
		return IsPlayerDead ? Next : Retry;
	} } );
	Steps.push_back( { "F12 on a dead hero", []{
		AddMarker();
		KeyPressHandler( VK_123_F12_KEY ); // the menu opened by itself: it takes the key
		Menu_SetupMenuWindow( 0, 0 );
		KeyPressHandler( VK_123_F12_KEY ); // with the menu closed the key is a chat message
		return 20;
	} } );
	Steps.push_back( { "F12 on a dead hero: result", []{
		Check( "F12 on a dead hero does nothing: the menu's Load Game is the way back", HasMarker() && IsPlayerDead );
		return Next;
	} } );
}

#include "AutoTestFeatures.inl"

const Phase Phases[] = {
	{ "main",  HeroWarrior, PC_0_WARRIOR, GM_NORMAL,  BuildMain  },
	{ "other", HeroMage,    PC_2_MAGE,    GM_CLASSIC, BuildOther },
	{ "owner", HeroWarrior, PC_0_WARRIOR, GM_NORMAL,  BuildOwner },
	{ "oldwarrior", HeroOldWarrior, PC_0_WARRIOR, GM_NORMAL,  BuildOldSave },
	{ "oldmage",    HeroOldMage,    PC_2_MAGE,    GM_NORMAL,  BuildOldSave },
	{ "oldarcher",  HeroOldArcher,  PC_1_ARCHER,  GM_NORMAL,  BuildOldSave },
	{ "oldclassic", HeroOldClassic, PC_2_MAGE,    GM_CLASSIC, BuildOldSave },
	{ "oldveteran", HeroOldVeteran, PC_5_SAVAGE,  GM_NORMAL,  BuildOldSave },
	{ "qlnormal",       HeroQuick, PC_0_WARRIOR, GM_NORMAL,    BuildQuickLoadNormal },
	{ "qlhardcore",     HeroQuick, PC_0_WARRIOR, GM_HARDCORE,  BuildQuickLoadHardcore },
	{ "qlironman",      HeroQuick, PC_0_WARRIOR, GM_IRONMAN,   BuildQuickLoadForbidden },
	{ "qloutbreak",     HeroQuick, PC_0_WARRIOR, GM_OUTBREAK,  BuildQuickLoadForbidden },
	{ "qlnightmare",    HeroQuick, PC_0_WARRIOR, GM_NIGHTMARE, BuildQuickLoadForbidden },
	{ "qlspeedrun",     HeroQuick, PC_0_WARRIOR, GM_SPEEDRUN,  BuildQuickLoadForbidden },
	{ "qlsurvival",     HeroQuick, PC_0_WARRIOR, GM_SURVIVAL,  BuildQuickSaveSurvival },
	{ "qleasy",         HeroQuick, PC_0_WARRIOR, GM_EASY,      BuildQuickSaveAlways },
	{ "qldark",         HeroQuick, PC_0_WARRIOR, GM_DARK_CAMPAIGN, BuildQuickSaveAlways },
	{ "qlnormalnew",    HeroQuick, PC_0_WARRIOR, GM_NORMAL,    BuildQuickLoadNoSave },
	{ "qlnightmarenew", HeroQuick, PC_0_WARRIOR, GM_NIGHTMARE, BuildQuickLoadNoSave },
	{ "qlspeedrunnew",  HeroQuick, PC_0_WARRIOR, GM_SPEEDRUN,  BuildQuickLoadNoSave },
	{ "features", HeroFeatures, PC_0_WARRIOR, GM_NORMAL, BuildFeatures },
	{ "oldconfig", HeroFeatures, PC_0_WARRIOR, GM_NORMAL, BuildOldConfig },
	{ "missingfile", HeroNoData, PC_0_WARRIOR, GM_NORMAL, BuildMissingFile },
	{ "nomor", HeroNoData,  PC_0_WARRIOR, GM_NORMAL,  BuildNoMor },
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
		MessageBoxA( NULL, "Usage: TheHeaven.exe -autotest main|other|owner|oldwarrior|oldmage|oldarcher|oldclassic|oldveteran|ql...|missingfile|nomor [-show]", "Autotest", MB_OK | MB_ICONERROR );
		ExitProcess( 2 );
	}
	Active = true;
	Show = strstr( cmdLine, "-show" ) != nullptr;
	DeleteFileA( Fmt( "autotest-%s.log", CurPhase->name ).c_str() );
	Log( "autotest %s%s", CurPhase->name, Show ? " (shown)" : "" );
}

bool AutoTest_Active() { return Active; }
bool AutoTest_Show()   { return Show; }

// From the error dialogs and message boxes (Error.cpp). They would show on the PC's screen and
// wait for a click, so the autotest reads the text the player would get, records it and quits.
// dialogId 0 means text is the message itself; otherwise it's what the dialog shows as the problem.
// In the nomor and missingfile phases this error is the point (bug 13); elsewhere any error fails.
// Ways the bug 13 errors can fail: naming DIABDAT.MPQ or nothing instead of the file that's missing
// (the dialogs' fixed text names TH4data.mor anyway, so the problem field is what counts); text cut
// off by its box; a dialog on the PC's screen; the game carrying on after it
void AutoTest_Error( int dialogId, const char* text )
{
	if( ! Active ) return;
	string problem = text ? text : "", shown = problem, cutOff;
	if( dialogId ){
		shown.clear();
		// Hidden: the dialog templates aren't WS_VISIBLE, and only DialogBox would show them
		HWND dlg = CreateDialogParamA( HInstance, (LPCSTR)(INT_PTR)dialogId, NULL, DialogFunc, (LPARAM)problem.c_str() );
		for( HWND c = dlg ? GetWindow( dlg, GW_CHILD ) : NULL; c; c = GetWindow( c, GW_HWNDNEXT ) ){
			char buf[1024] = "";
			GetWindowTextA( c, buf, sizeof buf );
			if( ! *buf || ! strcmp( buf, "OK" ) || ! strcmp( buf, "Quit" ) ) continue;
			shown += string( shown.empty() ? "" : " / " ) + buf;
			RECT box, need;
			GetClientRect( c, &box );
			need = box;
			HDC dc = GetDC( c );
			HGDIOBJ oldFont = SelectObject( dc, (HGDIOBJ)SendMessageA( c, WM_GETFONT, 0, 0 ) );
			DrawTextA( dc, buf, -1, &need, DT_CALCRECT | DT_WORDBREAK );
			SelectObject( dc, oldFont );
			ReleaseDC( c, dc );
			if( need.bottom > box.bottom ) cutOff += Fmt( " CUT OFF: needs %d pixels, has %d: %s", (int)need.bottom, (int)box.bottom, buf );
		}
		if( dlg ) DestroyWindow( dlg );
		shown = Fmt( "dialog %d: ", dialogId ) + shown + cutOff;
	}
	auto has = []( const string& s, const char* what ){ return s.find( what ) != string::npos; };
	if( ! strcmp( CurPhase->name, "nomor" ) ){
		Check( "without TH4data.mor, the game stops with an error that names it (bug 13)",
			has( problem, "TH4data.mor" ) && has( problem, "v0.774" ) && cutOff.empty(), shown );
	}else if( ! strcmp( CurPhase->name, "missingfile" ) ){
		Check( "a file missing from the archives gives an error that names it and TH4data.mor (bug 13)",
			problem == MissingFile && has( shown, "TH4data.mor" ) && cutOff.empty(), shown );
	}else{
		Check( "the game runs without an error message", false, shown );
	}
	Log( "finished" );
	WriteReport();
	ExitProcess( 0 );
}

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

// From PlayLocalSound: the features phase checks what was played
void AutoTest_NoteSound( int soundIndex )
{
	SoundsPlayed.push_back( soundIndex );
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
