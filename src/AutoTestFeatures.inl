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

void HealHero() { Hero().CurLife = Hero().MaxCurLife; }

bool OnScreenShows( int messageIndex )
{
	for( uchar i = OnScreenMessage::Head; i != OnScreenMessage::Tail; ++i ){
		if( OnScreenMessage::Queue[i].index == messageIndex ) return true;
	}
	return false;
}

// Stairs as the hero finds them: stands the hero on the level's warp for the game's own check
bool TakeWarp( int message )
{
	Player& p = Hero();
	for( const Warp& warp: Warps ){
		if( warp.GameMessage != message ) continue;
		const int row = p.Row, col = p.Col;
		p.Row = warp.Row;
		p.Col = warp.Col;
		CheckTriggers();
		p.Row = row;
		p.Col = col;
		return true;
	}
	return false;
}

// Stairs to town, the kind a quest area can have: added under the hero where the level has none
bool TakeWarpToTown()
{
	Warps.push_back( { Hero().Row, Hero().Col, WM_1032_RETURN_TO_TOWN, DUN_0_TOWN } );
	const bool taken = TakeWarp( WM_1032_RETURN_TO_TOWN );
	Warps.pop_back();
	return taken;
}

// The hero's town portal, moved under the hero as if it had walked in. Returns the missile
int OpenPortalUnderHero()
{
	const Player& p = Hero();
	const int portal = CastMissile( p.Row, p.Col, p.Row, p.Col, p.dir, MI_10_TOWN_PORTAL, CT_0_PLAYER, CurrentPlayerIndex, 0, 0, 0 );
	if( ! Missiles[portal].IsDeleted ){
		Missiles[portal].Row = p.Row;
		Missiles[portal].Col = p.Col;
	}
	return portal;
}

// Waits for a level change the hero started, and checks it came
void AddLeftLevel( const char* name, DUNGEON from )
{
	Steps.push_back( { name, [=, wait = 100]() mutable {
		if( (DUNGEON)Dungeon == from && wait-- > 0 ) return Retry;
		Check( name, (DUNGEON)Dungeon != from, Fmt( "on level %d", (int)(DUNGEON)Dungeon ) );
		return 20;
	} } );
}

// F9 and the save file's time: the same check as the F9 phases
void CheckF9Saves( const char* name )
{
	const i64 before = SaveFileTime();
	KeyPressHandler( VK_120_F9_KEY );
	Check( name, SaveFileTime() != before );
}

// Rolls for a sigil as a monster's death does, from the same start each time, and takes each one
// that drops off the ground again
int RollSigils( int rate, int rolls )
{
	SigilDropRate = rate;
	SetRndSeed( 12345 );
	int sigils = 0;
	for( int i = 0; i < rolls; ++i ){
		const int before = ItemsCount;
		TryToDropSigil( Hero().Row, Hero().Col );
		if( ItemsCount == before ) continue;
		const int index = ItemActive[ItemsCount - 1];
		if( Items[index].baseItemIndex >= BI_2504_SIGIL_FIRST && Items[index].baseItemIndex <= BI_2508_SIGIL_LAST ) ++sigils;
		ItemMap[Items[index].MapRow][Items[index].MapCol] = 0;
		DeleteItem( index, ItemsCount - 1 );
	}
	return sigils;
}

// A config.ini from before the options: autotest.ps1 takes their lines out
void BuildOldConfig()
{
	AddStartSteps( GM_NORMAL );
	Steps.push_back( { "the options a config.ini doesn't have yet", []{
		Check( "a config.ini without the new options reads as their defaults",
			BattleLockdown && SigilDropRate == 100 && ShowSocketsOnGround && UniqueDropSound && LevelClearedSound,
			Fmt( "BattleLockdown %d, SigilDropRate %d, ShowSocketsOnGround %d, UniqueDropSound %d, LevelClearedSound %d",
				(int)BattleLockdown, SigilDropRate, (int)ShowSocketsOnGround, (int)UniqueDropSound, (int)LevelClearedSound ) );
		return Next;
	} } );
}

void BuildFeatures()
{
	AddStartSteps( GM_NORMAL );

	// autotest.ps1 changes these two in the shipped config.ini for this phase, so a line missing or a
	// default changed there fails here too
	Steps.push_back( { "the options from config.ini", []{
		Check( "BattleLockdown off and SigilDropRate 250 in config.ini are read", ! BattleLockdown && SigilDropRate == 250,
			Fmt( "BattleLockdown %d, SigilDropRate %d", (int)BattleLockdown, SigilDropRate ) );
		BattleLockdown = true;
		SigilDropRate = 100;
		return Next;
	} } );

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
	AddGoTo( DUN_1_CHURCH_1, WM_1026_NEXT_DUNGEON );
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
	AddGoTo( DUN_2_CHURCH_2, WM_1026_NEXT_DUNGEON );
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
	AddGoTo( DUN_1_CHURCH_1, WM_1027_PREV_DUNGEON );
	Steps.push_back( { "the cleared level stays quiet", []{
		Check( "entering a level cleared earlier plays no clear sound", CountSound( S_14_QUESTDON ) == 0, Fmt( "played %d times", CountSound( S_14_QUESTDON ) ) );
		return Next;
	} } );

	// The battle lockdown: a fight on an ordinary level, as the game has it
	AddGoTo( DUN_3_CHURCH_3, WM_1026_NEXT_DUNGEON );
	Steps.push_back( { "a fight, with the lockdown on", []{
		if( Hero().CurAction != PCA_0_STAND ) return Retry;
		HealHero();
		StartBattle();
		Check( "with BattleLockdown on, Save Game is off in a fight", ! MenuOffers( MenuSave ) );
		const i64 before = SaveFileTime();
		KeyPressHandler( VK_120_F9_KEY );
		Check( "with BattleLockdown on, F9 doesn't save in a fight", SaveFileTime() == before );
		ClearOnScreenMessages();
		const bool stairs = TakeWarp( WM_1026_NEXT_DUNGEON );
		Check( "with BattleLockdown on, the stairs refuse in a fight", stairs && OnScreenShows( OM_4_battle_not_over ) );
		Check( "with BattleLockdown on, a quest area's entrance refuses in a fight", BlockMessageToQuestDungeon() == OM_4_battle_not_over );
		return 20;
	} } );
	Steps.push_back( { "the portal, with the lockdown on", []{
		if( Hero().CurAction != PCA_0_STAND ) return Retry;
		HealHero();
		StartBattle();
		ClearOnScreenMessages();
		OpenPortalUnderHero();
		return 10;
	} } );
	Steps.push_back( { "the portal refused", []{
		Check( "with BattleLockdown on, a town portal refuses in a fight", OnScreenShows( OM_4_battle_not_over ) && (DUNGEON)Dungeon == DUN_3_CHURCH_3,
			Fmt( "on level %d", (int)(DUNGEON)Dungeon ) );
		return 20;
	} } );

	// The same fight with the lockdown off: the menu, then a save and a load in the fight
	Steps.push_back( { "a fight, with the lockdown off", []{
		if( Hero().CurAction != PCA_0_STAND ) return Retry;
		HealHero();
		BattleLockdown = false;
		StartBattle();
		Check( "with BattleLockdown off, Save Game is on in a fight", MenuOffers( MenuSave ) );
		Check( "with BattleLockdown off, a quest area's entrance opens in a fight", BlockMessageToQuestDungeon() == OM_0_NO_MESSAGE );
		SelectMainMenu();
		return 3;
	} } );
	Steps.push_back( { "screenshot", []{
		Shot( "pause-menu-in-a-fight" );
		Menu_SetupMenuWindow( 0, 0 );
		return Next;
	} } );
	Steps.push_back( { "save in the fight", []{
		StartBattle();
		CheckF9Saves( "with BattleLockdown off, F9 saves in a fight" );
		AddMarker();
		KeyPressHandler( VK_123_F12_KEY );
		return 20;
	} } );
	Steps.push_back( { "load it", []{
		Check( "and F12 loads that save, with the fight still on", ! HasMarker() && (DUNGEON)Dungeon == DUN_3_CHURCH_3 && IsSomeMonstersActivated(),
			Fmt( "marker %d, level %d, fight %d", (int)HasMarker(), (int)(DUNGEON)Dungeon, IsSomeMonstersActivated() ) );
		return Next;
	} } );

	// Only single player, Normal and Hard
	Steps.push_back( { "the modes it leaves alone", []{
		HealHero();
		StartBattle();
		const GAME_MODE mode = GameMode;
		const bool noSave = EnforceNoSave;
		struct { GAME_MODE mode; const char* name; } modes[] = { { GM_HARDCORE, "Hardcore" }, { GM_IRONMAN, "Ironman" }, { GM_NIGHTMARE, "Nightmare" },
			{ GM_SURVIVAL, "Survival" }, { GM_OUTBREAK, "Outbreak" } };
		for( const auto& m: modes ){
			GameMode = m.mode;
			Check( Fmt( "with BattleLockdown off, %s keeps its lock: no Save Game and no leaving in a fight", m.name ).c_str(),
				! MenuOffers( MenuSave ) && ! IsSaveAndLeavingEnabled() );
			EnforceNoSave = noSave; // Nightmare's menu turns it on
		}
		GameMode = GM_HARD;
		Check( "with BattleLockdown off, Hard has Save Game and leaving in a fight", MenuOffers( MenuSave ) && IsSaveAndLeavingEnabled() );
		GameMode = mode;
		MaxCountOfPlayersInGame = 2;
		const bool offInMultiplayer = IsBattleLockdownOff();
		MaxCountOfPlayersInGame = 1;
		Check( "with BattleLockdown off, multiplayer keeps its own rules", ! offInMultiplayer );
		return Next;
	} } );

	// How often sigils drop. In the same step, so nothing else rolls in between
	Steps.push_back( { "sigils", []{
		const int rolls = 40000; // 1 in 800 at 100%: about 50 sigils
		const int at0 = RollSigils( 0, rolls ), at50 = RollSigils( 50, rolls ), at100 = RollSigils( 100, rolls );
		const int at250 = RollSigils( 250, rolls ), at1000 = RollSigils( 1000, rolls ), at5000 = RollSigils( 5000, rolls );
		const string counts = Fmt( "in %d rolls: 0%% %d, 50%% %d, 100%% %d, 250%% %d, 1000%% %d, 5000%% %d", rolls, at0, at50, at100, at250, at1000, at5000 );
		Check( "SigilDropRate 0 drops no sigils", at0 == 0, counts );
		Check( "SigilDropRate 100 keeps the game's 1 in 800", at100 >= 25 && at100 <= 80, counts );
		Check( "SigilDropRate 50 drops half as many", at50 >= 8 && at50 <= 45, counts );
		Check( "SigilDropRate 250 drops 2.5 times as many", at250 >= 85 && at250 <= 170, counts );
		Check( "SigilDropRate 1000 drops 10 times as many", at1000 >= 400 && at1000 <= 600, counts );
		Check( "SigilDropRate above 1000 counts as 1000", at5000 == at1000, counts );
		const GAME_MODE mode = GameMode;
		GameMode = GM_DARK_CAMPAIGN; // the game's best chance, 1 in 200
		const int inCampaign = RollSigils( 1000, 4000 );
		GameMode = mode;
		SigilDropRate = 100;
		Check( "at 1000% the Dark Campaign's 1 in 200 becomes 1 in 20", inCampaign >= 140 && inCampaign <= 260, Fmt( "%d sigils in 4000 rolls", inCampaign ) );
		return Next;
	} } );

	// Stairs and the portal in the fight, with the lockdown off
	Steps.push_back( { "the stairs in the fight", []{
		if( Hero().CurAction != PCA_0_STAND ) return Retry;
		HealHero();
		StartBattle();
		TakeWarp( WM_1026_NEXT_DUNGEON );
		return Next;
	} } );
	AddLeftLevel( "with BattleLockdown off, the stairs work in a fight", DUN_3_CHURCH_3 );
	Steps.push_back( { "the portal in the fight", []{
		if( Hero().CurAction != PCA_0_STAND ) return Retry;
		HealHero();
		StartBattle();
		OpenPortalUnderHero();
		return Next;
	} } );
	AddLeftLevel( "with BattleLockdown off, a town portal works in a fight", DUN_4_CHURCH_4 );

	// A quest area with monsters left
	Steps.push_back( { "the lockdown on again", []{ BattleLockdown = true; return Next; } } );
	AddGoToQuestFloor();
	Steps.push_back( { "a quest area, with the lockdown on", []{
		if( Hero().CurAction != PCA_0_STAND ) return Retry;
		HealHero();
		Check( "the quest area has monsters left", MonstersCount > SummonMonsters_Count );
		Check( "with BattleLockdown on, Save Game is off in a quest area with monsters left", ! MenuOffers( MenuSave ) );
		Check( "with BattleLockdown on, town portal can't be cast there", ! IsSpellEnabledHere( PS_7_TOWN_PORTAL, CurrentPlayerIndex ) );
		Check( "with BattleLockdown on, a town portal doesn't open there", Missiles[OpenPortalUnderHero()].IsDeleted );
		ClearOnScreenMessages();
		const bool stairs = TakeWarp( WM_1028_RETURN_FROM_QUEST_DUNGEON );
		Check( "with BattleLockdown on, the stairs back refuse there", stairs && OnScreenShows( OM_2_area_not_cleared ) );
		Check( "with BattleLockdown on, the next quest area's entrance refuses there", BlockMessageToQuestDungeon() == OM_2_area_not_cleared );
		return 20;
	} } );
	Steps.push_back( { "stairs to town, with the lockdown on", []{
		if( Hero().CurAction != PCA_0_STAND ) return Retry;
		HealHero();
		ClearOnScreenMessages();
		const bool stairs = TakeWarpToTown();
		Check( "with BattleLockdown on, stairs to town refuse there", stairs && OnScreenShows( OM_2_area_not_cleared ) );
		return 20;
	} } );
	Steps.push_back( { "a quest area, with the lockdown off", []{
		if( Hero().CurAction != PCA_0_STAND ) return Retry;
		HealHero();
		BattleLockdown = false;
		Check( "with BattleLockdown off, Save Game is on in a quest area with monsters left", MenuOffers( MenuSave ) );
		Check( "with BattleLockdown off, the next quest area's entrance opens there", BlockMessageToQuestDungeon() == OM_0_NO_MESSAGE );
		CheckF9Saves( "with BattleLockdown off, F9 saves there" );
		Check( "with BattleLockdown off, town portal can be cast there", IsSpellEnabledHere( PS_7_TOWN_PORTAL, CurrentPlayerIndex ) );
		Check( "with BattleLockdown off, a town portal opens there", ! Missiles[OpenPortalUnderHero()].IsDeleted );
		return Next;
	} } );
	AddLeftLevel( "with BattleLockdown off, that town portal takes the hero out", DUN_25_KING_LEORICS_TOMB );
	AddGoToQuestFloor();
	Steps.push_back( { "the stairs back", []{
		if( Hero().CurAction != PCA_0_STAND ) return Retry;
		HealHero();
		TakeWarp( WM_1028_RETURN_FROM_QUEST_DUNGEON );
		return Next;
	} } );
	AddLeftLevel( "with BattleLockdown off, the stairs back work there", DUN_25_KING_LEORICS_TOMB );
	AddGoToQuestFloor();
	Steps.push_back( { "stairs to town", []{
		if( Hero().CurAction != PCA_0_STAND ) return Retry;
		HealHero();
		TakeWarpToTown();
		return Next;
	} } );
	AddLeftLevel( "with BattleLockdown off, stairs to town work there", DUN_25_KING_LEORICS_TOMB );
	Steps.push_back( { "the lockdown back on", []{ BattleLockdown = true; return Next; } } );
}
