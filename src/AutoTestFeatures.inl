// The autotest's features phase. AutoTest.cpp includes this inside its anonymous namespace, so it
// has that file's Steps, Check, Shot and item helpers.

//----- phase: features ---------------------------------------------------

vector<int> SoundsPlayed; // what PlayLocalSound was asked to play, see AutoTest_NoteSound
double      TicksPerSecondAt1x = 0, TicksPerSecondAt2x = 0;

int CountSound( int soundIndex ) { return (int)count( SoundsPlayed.begin(), SoundsPlayed.end(), soundIndex ); }

short& BaseStat( int button )
{
	Player& p = Hero();
	return button == 0 ? p.BaseStrength : button == 1 ? p.BaseMagic : button == 2 ? p.BaseDexterity : p.BaseVitality;
}

int MaxStat( int button )
{
	const PlayerStat m = GetPlayerMaxStat( CurrentPlayerIndex );
	return button == 0 ? m.Strength : button == 1 ? m.Magic : button == 2 ? m.Dexterity : m.Vitality;
}

// Presses and releases a stat's + button with these points to spend and this room under the
// stat's cap (room -1: the stat as it is), and checks how many points went in
void AddStatClick( const char* name, int button, bool shift, int points, int room, int expected )
{
	static int before;
	Steps.push_back( { "click a stat's +", [=]{
		Hero().AvailableLvlPoints = points;
		if( room >= 0 ) BaseStat( button ) = (short)( MaxStat( button ) - room );
		before = BaseStat( button );
		CursorX = CharPanelRect.Left + StatUpButtons[button].X + 1;
		CursorY = CharPanelRect.Top  + StatUpButtons[button].Y + 1;
		IsShiftPressed = shift ? 1 : 0;
		CharPanel_MouseDown();
		CharPanel_MouseClick();
		IsShiftPressed = 0;
		return 3;
	} } );
	Steps.push_back( { name, [=]{
		const int gained = BaseStat( button ) - before, left = Hero().AvailableLvlPoints;
		Check( name, gained == expected && left == points - expected,
			Fmt( "%d points, room %d: the stat gained %d, %d points left", points, room, gained, left ) );
		return Next;
	} } );
}

struct DropPick { int base = 0, seed = 0; };

DropPick FindDrop( bool (*match)( const Item& ) )
{
	for( int base: FindBases( CanBeMagic, 2000 ) ){
		for( int seed = 1; seed <= 200; ++seed ){
			if( match( DropItem( base, seed, 30 ) ) ) return { base, seed };
		}
	}
	return {};
}

bool IsPlainDrop( const Item& it )    { return it.MagicLevel == ML_0_USUAL; }
bool IsMagicDrop( const Item& it )    { return it.MagicLevel == ML_1_MAGIC; }
bool IsRareDrop( const Item& it )     { return it.MagicLevel == ML_2_UNIQUE && (it.dropType & D_RARE); }
bool IsSocketedDrop( const Item& it ) { return it.MagicLevel == ML_0_USUAL && it.socketsAmount > 0; }

// On the ground next to the hero, the way TryToDropSigil drops an item
int PutDrop( const DropPick& d )
{
	int itemIndex = -1;
	if( ! d.base || ! GetItemSpace( Hero().Row, Hero().Col, itemIndex ) ) return -1;
	SetupAllItemsSmartdrop( itemIndex, d.base, d.seed, 30, 1, 0, 0, 0, 1 );
	return itemIndex;
}

int PutUnique( bool set )
{
	for( size_t i = 1; i < count_UniqueItems; ++i ){
		const auto& u = UniqueItems[i];
		if( u.baseId > 0 && (u.uniqueSetIndex != -1) == set ) return SpawnUnique( (int)i, Hero().Row, Hero().Col );
	}
	return -1;
}

// Makes an item land next to the hero and checks the sounds it played
void AddDropSound( const char* name, std::function<int()> drop, int extraSounds )
{
	static int itemIndex;
	Steps.push_back( { "an item drops", [drop]{
		SoundsPlayed.clear();
		itemIndex = drop();
		return Next;
	} } );
	Steps.push_back( { name, [=]{
		if( itemIndex >= 0 && Items[itemIndex].playAnimation ) return Retry;
		string detail = "no item";
		if( itemIndex >= 0 ){
			detail = Fmt( "%s, magic level %d, drop type %d: %d sounds, %d extra", Items[itemIndex].Name, (int)Items[itemIndex].MagicLevel,
				(int)Items[itemIndex].dropType, (int)SoundsPlayed.size(), CountSound( S_36_I_FLIPMAG ) );
		}
		Check( name, itemIndex >= 0 && ! SoundsPlayed.empty() && CountSound( S_36_I_FLIPMAG ) == extraSounds, detail );
		return Next;
	} } );
}

void AddMeasureSpeed( double* ticksPerSecond )
{
	Steps.push_back( { "measure the game's speed", [ticksPerSecond]{
		static DWORD start;
		static int ticks = 0;
		if( ticks == 0 ) start = GetTickCount();
		if( ++ticks <= 60 ) return Retry;
		const DWORD ms = GetTickCount() - start;
		*ticksPerSecond = 60 * 1000.0 / ( ms ? ms : 1 );
		ticks = 0;
		return Next;
	} } );
}

// The monsters GetNumberOfMonstersInDungeon counts
vector<int> MonstersLeft()
{
	vector<int> left;
	for( int i = SummonMonsters_Count; i < MonstersCount; ++i ){
		const Monster& m = Monsters[MonsterIndexes[i]];
		if( ! m.speechIndex && ! (m.flag & MF_15_KILLED) && m.Xp > 0 && (m.Col > 1 || m.Row > 1) ) left.push_back( MonsterIndexes[i] );
	}
	return left;
}

bool MonstersDying()
{
	for( int i = SummonMonsters_Count; i < MonstersCount; ++i ){
		const Monster& m = Monsters[MonsterIndexes[i]];
		if( (m.flag & MF_15_KILLED) || m.CurAction == A_6_DEATH ) return true;
	}
	return false;
}

// Goes to a level and lets it settle
void AddFeaturesGoTo( DUNGEON level, uint message )
{
	Steps.push_back( { "go to the level", [=]{
		if( (DUNGEON)Dungeon != level ) StartNewLvl( CurrentPlayerIndex, message, level );
		return Next;
	} } );
	Steps.push_back( { "on the level", [level, settle = 20]() mutable {
		if( (DUNGEON)Dungeon != level ) return Retry;
		return settle-- > 0 ? Retry : Next;
	} } );
}

void BuildFeatures()
{
	AddStartSteps( GM_NORMAL );

	// Shift-click on a stat's + spends 5 points, not more than there are or than fit
	AddStatClick( "Shift-click on a stat's + spends 5 points", 0, true, 13, -1, 5 );
	AddStatClick( "a plain click on a stat's + spends 1 point", 0, false, 8, -1, 1 );
	AddStatClick( "Shift-click spends only what fits under the stat's cap, the rest stays", 1, true, 7, 3, 3 );
	AddStatClick( "Shift-click with 2 points left spends 2", 2, true, 2, -1, 2 );

	// The socket count in the ground label
	Steps.push_back( { "socket count in the ground label", []{
		const int i = PutDrop( FindDrop( IsSocketedDrop ) );
		if( ! Check( "a socketed item drops", i >= 0 && Items[i].socketsAmount > 0 ) ) return Next;
		const Item& item = Items[i];
		char label[256];
		const string count = Fmt( " [%d]", (int)item.socketsAmount );
		const auto hasCount = [&]{ const size_t n = strlen( label ); return n > count.size() && count == label + n - count.size(); };
		GroundItemLabel( item, label );
		Check( "an item's label on the ground ends with its socket count", hasCount(), label );
		ShowSocketsOnGround = false;
		GroundItemLabel( item, label );
		Check( "with ShowSocketsOnGround off the label has no count", ! hasCount(), label );
		ShowSocketsOnGround = true;
		Item unique = item;
		unique.MagicLevel = ML_2_UNIQUE;
		GroundItemLabel( unique, label );
		Check( "a unique item's label shows its socket count in normal mode", hasCount(), label );
		const auto mode = GameMode;
		GameMode = GM_IRONMAN;
		GroundItemLabel( unique, label );
		Check( "in Ironman it doesn't, as in the item's tooltip", ! hasCount(), label );
		GameMode = mode;
		AltHighlight = true;
		return 10;
	} } );
	Steps.push_back( { "screenshot", []{ Shot( "ground-labels" ); AltHighlight = false; return Next; } } );

	// The extra sound of a unique or set item that drops
	AddDropSound( "a plain item that drops plays no extra sound", []{ return PutDrop( FindDrop( IsPlainDrop ) ); }, 0 );
	AddDropSound( "a magic item that drops plays no extra sound", []{ return PutDrop( FindDrop( IsMagicDrop ) ); }, 0 );
	AddDropSound( "a rare item that drops plays no extra sound", []{ return PutDrop( FindDrop( IsRareDrop ) ); }, 0 );
	AddDropSound( "a unique item that drops plays the extra sound once", []{ return PutUnique( false ); }, 1 );
	AddDropSound( "a set item that drops plays the extra sound once", []{ return PutUnique( true ); }, 1 );
	AddDropSound( "a unique item the hero puts on the ground plays no extra sound", []{
		const int onGround = PutUnique( false );
		if( onGround < 0 ) return -1;
		Hero().ItemOnCursor = Items[onGround];
		const int put = InvPutItem( CurrentPlayerIndex, Hero().Row, Hero().Col ); // takes the copy on the ground away as a duplicate
		Hero().ItemOnCursor.ItemCode = IC_M1_NONE;
		return put;
	}, 0 );
	AddDropSound( "with UniqueDropSound off a unique item plays no extra sound", []{
		UniqueDropSound = false;
		return PutUnique( false );
	}, 0 );
	Steps.push_back( { "UniqueDropSound back on", []{ UniqueDropSound = true; return Next; } } );

	// 2x speed
	AddMeasureSpeed( &TicksPerSecondAt1x );
	Steps.push_back( { "the speed key", []{
		Check( "the speed button has frames for four speeds", *(int*)CtrlPan_TimeButtonCEL == 8 );
		double seen[4];
		for( double& speed: seen ){ KeyPressHandler( VK_OEM_PERIOD ); speed = TimeSpeed; }
		Check( "the speed key goes 1.25x, 1.5x, 2x and back to 1x", seen[0] == 1.25 && seen[1] == 1.5 && seen[2] == 2.0 && seen[3] == 1.0,
			Fmt( "%.2f %.2f %.2f %.2f", seen[0], seen[1], seen[2], seen[3] ) );
		for( int i = 0; i < 3; ++i ) KeyPressHandler( VK_OEM_PERIOD );
		return 10;
	} } );
	Steps.push_back( { "screenshot", []{ Shot( "speed-button-2x" ); return Next; } } );
	AddMeasureSpeed( &TicksPerSecondAt2x );
	Steps.push_back( { "the game runs twice as fast", []{
		const double ratio = TicksPerSecondAt2x / TicksPerSecondAt1x;
		Check( "at 2x the game runs twice as fast", ratio > 1.6 && ratio < 2.4,
			Fmt( "%.1f ticks per second at 1x, %.1f at 2x", TicksPerSecondAt1x, TicksPerSecondAt2x ) );
		TimeReset();
		return Next;
	} } );

	// The sound of a cleared level
	AddFeaturesGoTo( DUN_1_CHURCH_1, WM_1026_NEXT_DUNGEON );
	Steps.push_back( { "kill all the monsters but one", []{
		SoundsPlayed.clear();
		const vector<int> left = MonstersLeft();
		Check( "the level has monsters", left.size() >= 2, Fmt( "%d", (int)left.size() ) );
		for( size_t i = 1; i < left.size(); ++i ) MonsterKilledByPlayer( left[i], CurrentPlayerIndex, true );
		return Next;
	} } );
	Steps.push_back( { "one monster left", []{
		if( MonstersDying() ) return Retry;
		Check( "with a monster left the level's clear sound doesn't play", MonstersLeft().size() == 1 && CountSound( S_14_QUESTDON ) == 0,
			Fmt( "%d left, played %d times", (int)MonstersLeft().size(), CountSound( S_14_QUESTDON ) ) );
		MonsterKilledByPlayer( MonstersLeft().at( 0 ), CurrentPlayerIndex, true );
		return Next;
	} } );
	Steps.push_back( { "all monsters dead", []{
		if( MonstersDying() ) return Retry;
		Check( "the last monster's death plays the level's clear sound once", GetNumberOfMonstersInDungeon() == 0 && CountSound( S_14_QUESTDON ) == 1,
			Fmt( "%d left, played %d times", GetNumberOfMonstersInDungeon(), CountSound( S_14_QUESTDON ) ) );
		return Next;
	} } );
	AddFeaturesGoTo( DUN_2_CHURCH_2, WM_1026_NEXT_DUNGEON );
	// What DeltaLoadLevel does on entering a level in multiplayer: the monsters killed earlier are
	// flagged during the load, and go through DelMonster on the first tick
	Steps.push_back( { "a level cleared earlier, entered as in multiplayer", []{
		SoundsPlayed.clear();
		LevelClearedSound_LevelLoaded();
		for( int m: MonstersLeft() ){
			ClearMonsterOnMap( m );
			Monsters[m].flag |= MF_15_KILLED;
		}
		return 5;
	} } );
	Steps.push_back( { "back up", []{
		Check( "entering a level cleared earlier in multiplayer plays no clear sound", GetNumberOfMonstersInDungeon() == 0 && CountSound( S_14_QUESTDON ) == 0,
			Fmt( "%d left, played %d times", GetNumberOfMonstersInDungeon(), CountSound( S_14_QUESTDON ) ) );
		SoundsPlayed.clear();
		return Next;
	} } );
	AddFeaturesGoTo( DUN_1_CHURCH_1, WM_1027_PREV_DUNGEON );
	Steps.push_back( { "the cleared level stays quiet", []{
		Check( "entering a level cleared earlier plays no clear sound", CountSound( S_14_QUESTDON ) == 0, Fmt( "played %d times", CountSound( S_14_QUESTDON ) ) );
		return Next;
	} } );
}
