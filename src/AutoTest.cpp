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
//   main   Warrior, normal mode: the shipped config, Cain's transmute window, F9/F12
//          quick save and load, then stashes items in the shared stash and lists them
//          in autotest-shared.txt as the Warrior has them; ends with a dungeon
//          screenshot to check pixel light by eye
//   other  Mage, Classic mode: opens the shared stash. Every item that comes out
//          different from the Warrior's list must be locked and stay in the stash;
//          every item that comes out the same must not be
//   owner  the Warrior again: every listed item must still be exactly as stashed

extern StashTab StashTabs[Stash_TotalTabs];

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

// Copies of the item with this seed the hero has anywhere: inventory, body, belt,
// cursor and Cain's transmute slots
int CountSeed( int seed )
{
	Player& p = Hero();
	int n = 0;
	auto count = [&]( const Item& it ){ if( it.ItemCode != IC_M1_NONE && it.seed == seed ) ++n; };
	for( int i = 0; i < p.InvItemCount; ++i ) count( p.InventorySlots[i] );
	for( const Item& it: p.OnBodySlots ) count( it );
	for( const Item& it: p.BeltInventory ) count( it );
	count( p.ItemOnCursor );
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

// Where the shared stash has the item with this seed, as tab * Stash_MaxSlot + index
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
