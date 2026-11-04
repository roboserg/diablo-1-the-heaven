#include "stdafx.h"

// ============================================================================================
// (new by th4) TOWN NIGHT WEATHER -- a weather roll for every night, and the thunderstorm.
// ============================================================================================
//
// THE ROLL. Every night in the normal town is CLEAR (24%, fireflies), GLOW (24%, bioluminescence:
// glowing river, glowing ground lights, soft blue halo -- see BIOGLOW in dx_utility.cpp), MIST (31%),
// BLOOD (3%, blood moon: crimson river, gold-red leaves, warmer ground, red halo -- same BIOGLOW
// pass) or STORM (18%):
// Weather_Tonight(). Only in Dungeon == DUN_0_TOWN (not quest maps such as Ravenholm), never in
// Classic (no day/night cycle) and never under Ice Age (it snows there instead, Snowfall.cpp).
// The roll is a hash of the seed of dungeon level 1 (DungeonRandSeed[1], generated from the game seed,
// so identical for every player of a multiplayer game; never the town's seed, which is re-rolled on
// every return to town -- see RollForNight) and the NUMBER of the night
// (DayNight_NightIndex, palette.cpp: which day/night cycle it is). So all players get the same
// weather sequence night after night; each night is one weather from dusk to dawn. Note the
// day/night cycle itself runs on each player's own clock (GetTickCountTh), so two players can
// still be at different points of it -- same weather per night number, not the same moment.
// NO CONFIG OPTION changes the roll or switches a weather off (the GroundMist row is disabled
// for that reason, Config.cpp): every player of a game must see the same night. What the options
// CAN change is only how it is drawn -- mist and rain are shaders, so "drawmode surface" (no
// shaders at all) shows lightning, thunder and the rain sound but no mist or rain streaks.
// Who reads the roll:
//   dx_utility.cpp Mist_Active ....... ground mist only on MIST nights
//   this file ........................ rain, lightning and thunder only on STORM nights
//   palette.cpp AnimateTownPalette / DayNight_TownSpriteDark .. Weather_FlashQ8 (lightning)
//   SoundBuffer.cpp PlayAmbient ...... the rain loop (sfx\ambience\01_Rain_Ambience.ogg) replaces the
//                                      night crickets on STORM nights, with fades both ways
//   Weather_WetQ8 (below) ............ WET GROUND: puddles (TownWater.cpp PUDDLES) and the darker,
//                                      wet look (dx_utility.cpp WET GROUND, Wet.hlsl) during a storm
//                                      night and over the day after it
//   Weather_SunQ8 (below) ............ SUNNY DAYS: CHANCE_SUNNY of the days are sunny (dx_utility.cpp
//                                      SUN RAYS through the trees, SunRays.hlsl) -- never the day after
//                                      a storm night (the town is drying, the sky still grey). The sun
//                                      shines for ONE spell of about half the day (Weather_SunQ8); the
//                                      other days are CLOUDY: dx_utility.cpp Clouds_Active draws the cloud
//                                      shadows only when Weather_SunnyDay is false
// Every effect fades in and out with the night itself (DayNight_NightQ8, the eased phase that
// drives the town palette), so a storm starts at dusk and ends at dawn without a hard cut.
//
// RAIN is NOT drawn here: it is a window-resolution shader (Rain.hlsl, driven by dx_utility.cpp
// Rain_Present), so streaks are about one screen pixel wide and semi-transparent instead of
// blocky game pixels. This file only tells it when (Weather_Tonight == STORM), how strong (the
// night), the time (Weather_ClockSec) and the flash (Weather_FlashQ8).
//
// LIGHTNING: every LIGHTNING_GAP_MIN..MAX ms of rain time a strike flashes the town. The flash
// is Weather_FlashQ8(), 0..256: palette.cpp scales the NIGHT strength by (256 - flash) in
// AnimateTownPalette (so the terrain palette, or the Enriched Light night in LightSurface, goes
// back toward day for an instant) and in DayNight_TownSpriteDark (NPCs, cows, objects). The flash
// has a flicker (bright, dim, bright, fade). AnimateTownPalette runs in the game logic loop, so
// the flash steps at that rate; the envelope segments are long enough for it.
// THUNDER: sfx\ambience\Thunder_Sound.wav (S_1946_THUNDER, structs_sounds.cpp) plays
// THUNDER_DELAY_MIN..MAX ms after the flash -- light first, sound later, like a distant strike.
//
// TIME: one clock for the rain shader, strikes and thunder, frozen while the game is paused (GamePaused /
// IsPausedInMenu, as Snowfall.cpp and AnimOrbs.cpp). GetTickCountTh() counts 1/MAX_FPS s
// (MAX_FPS = 10000, enums.h), NOT milliseconds.
//
// KNOWN-GOOD CHECK: normal town. Over a few nights, some are clear, some misty, some stormy --
// never mist and rain together. A storm night: rain fades in at dusk; strikes light the whole
// town for a moment with a flicker, thunder follows a second or so later; nothing at day, nothing
// in Ice Age or Classic, nothing in Ravenholm. Pause: rain, strikes and thunder freeze.
// ============================================================================================

namespace {

// ---- the roll -------------------------------------------------------------------------------
const int CHANCE_CLEAR = 48;		// %, split evenly into CLEAR (fireflies) and GLOW; then MIST, then BLOOD,
const int CHANCE_MIST  = 31;		// then STORM takes the rest (18)
const int CHANCE_BLOOD = 3;		// blood moon (WEATHER_BLOOD)
const int CHANCE_SUNNY = 50;	// % of the DAYS that are sunny (Weather_SunQ8); the day after a storm never is
// TESTING: 1 = every day sunny, 0 = none, -1 = the normal roll (still only where the weather happens, and
// never the day after a storm). Keep -1 for release -- same idea as WEATHER_FORCE above.
const int SUN_FORCE = -1;
// TESTING: set to WEATHER_CLEAR / WEATHER_GLOW / WEATHER_MIST / WEATHER_STORM / WEATHER_BLOOD to get that weather
// EVERY night (still only where a weather can happen). -1 = the normal roll. Keep -1 for release.
// Why it is needed: nights are numbered from the start of the session and the seed belongs to the
// game, so the SAME game always rolls the same sequence of nights -- an unlucky run (say six still
// nights that all came out as fireflies) repeats every time that game is loaded.
const int WEATHER_FORCE = -1;
// ---- lightning and thunder ------------------------------------------------------------------
const int LIGHTNING_GAP_MIN  = 50000;	// ms between strikes (50 .. 210 s of storm time)
const int LIGHTNING_GAP_MAX  = 210000;
const int LIGHTNING_FIRST_MIN = 20000;	// first strike this long after the night got dark enough
const int LIGHTNING_FIRST_MAX = 60000;
const int LIGHTNING_NIGHT_MIN = 160;	// strikes only once the night is this dark (0..256)
const int THUNDER_DELAY_MIN  = 400;		// ms from flash to thunder
const int THUNDER_DELAY_MAX  = 2200;

uint Hash( uint x )
{
	x ^= x >> 16; x *= 0x7feb352dU; x ^= x >> 15; x *= 0x846ca68bU; x ^= x >> 16;
	return x;
}

bool InNormalTown()
{
	return Dungeon == DUN_0_TOWN && !Dungeon->isQuest;
}

// Rain / storm clock, ms, frozen while paused.
long long ClockMs = 0;
void TickClock()
{
	static long long last = 0, ticks = 0;
	const long long now = GetTickCountTh();
	long long d = now - last;
	last = now;
	if( d < 0 || d > MAX_FPS ) d = 0;	// first call or a stall of over a second
	if( !GamePaused && !IsPausedInMenu() ) ticks += d;
	ClockMs = ticks * 1000 / MAX_FPS;
}

// Strike state
long long NextStrikeMs = -1, StrikeMs = -1, ThunderMs = -1;
uint StrikeNo = 0;
int FlashQ8 = 0;

// Flash envelope, 0..256, ms since the strike: bright, dim, bright again, fade out.
int FlashAt( long long t )
{
	if( t < 0 ) return 0;
	if( t < 70 ) return 256;
	if( t < 150 ) return 70;
	if( t < 260 ) return 230;
	if( t < 800 ) return (int)( 230 * ( 800 - t ) / 540 );
	return 0;
}

void UpdateStorm( int night )
{
	if( night < LIGHTNING_NIGHT_MIN ){ FlashQ8 = 0; NextStrikeMs = -1; StrikeMs = -1; return; }
	if( NextStrikeMs < 0 ){	// first dark enough moment: first strike after a short wait
		NextStrikeMs = ClockMs + LIGHTNING_FIRST_MIN + Hash( ++StrikeNo * 2654435761U ) % (uint)( LIGHTNING_FIRST_MAX - LIGHTNING_FIRST_MIN + 1 );
	}
	if( ClockMs >= NextStrikeMs ){
		const uint h = Hash( ++StrikeNo * 40503U + 17 );
		StrikeMs = ClockMs;
		ThunderMs = ClockMs + THUNDER_DELAY_MIN + h % (uint)( THUNDER_DELAY_MAX - THUNDER_DELAY_MIN + 1 );
		NextStrikeMs = ClockMs + LIGHTNING_GAP_MIN + ( h >> 8 ) % (uint)( LIGHTNING_GAP_MAX - LIGHTNING_GAP_MIN + 1 );
	}
	FlashQ8 = StrikeMs >= 0 ? FlashAt( ClockMs - StrikeMs ) : 0;
	if( ThunderMs >= 0 && ClockMs >= ThunderMs ){
		PlayGlobalSound( S_1946_THUNDER );
		ThunderMs = -1;
	}
}

// The roll for night number 'night' (DayNight_NightIndex numbering), without the where-and-when
// conditions -- Weather_Tonight and Weather_WetQ8 check those first.
WEATHER RollForNight( int night )
{
	if( WEATHER_FORCE >= 0 ) return (WEATHER)WEATHER_FORCE;
	if( night < 0 ) return WEATHER_CLEAR;	// before the first night of the session
	// The seed of dungeon level 1 ONLY: saved with the game and the same for every player. NOT the town's
	// seed (DungeonRandSeed[0]): the game re-rolls it every time a hero enters town (LoadingScreen.cpp
	// ~84, msg.cpp delta_leave_sync), which re-rolled every night's weather -- last night could turn into
	// a storm after the fact (puddles with no rain) and tonight's rain could start or stop on a return.
	const uint seed = (uint)DungeonRandSeed[ 1 ] * 0x9E3779B9U;
	const int roll = (int)( Hash( seed ^ ( (uint)night * 0x85EBCA6BU ) ) % 100 );
	if( roll < CHANCE_CLEAR ) return roll < CHANCE_CLEAR / 2 ? WEATHER_CLEAR : WEATHER_GLOW;	// half the still nights glow
	if( roll < CHANCE_CLEAR + CHANCE_MIST ) return WEATHER_MIST;
	if( roll < CHANCE_CLEAR + CHANCE_MIST + CHANCE_BLOOD ) return WEATHER_BLOOD;
	return WEATHER_STORM;
}

// (th4) Is day number 'day' (DayNight_NightIndex numbering: the day of cycle N follows night N - 1)
// sunny? Same seed as the nights, a different salt, so the two rolls are independent.
bool RollSunnyDay( int day )
{
	if( day < 0 ) return false;
	if( RollForNight( day - 1 ) == WEATHER_STORM ) return false;	// the day after a storm: drying, grey
	if( SUN_FORCE >= 0 ) return SUN_FORCE != 0;
	const uint seed = (uint)DungeonRandSeed[ 1 ] * 0x9E3779B9U;
	return (int)( Hash( seed ^ ( (uint)day * 0xC2B2AE35U ) ^ 0x5C0FFEEU ) % 100 ) < CHANCE_SUNNY;
}

bool WeatherHere()
{
	return GameMode != GM_CLASSIC && InNormalTown() && !( Players[ CurrentPlayerIndex ].gameChanger & BIT( GC_15_ICE_AGE ) );
}

// (th4) WET GROUND timing: the town gets wet while it rains and dries over the next day.
// Positions are DayNight_NightPosQ8 / DayNight_DayPosQ8 units (0..255 over dusk + night + dawn, and
// over the day plateau). The rain starts early in the dusk (dx_utility.cpp Rain_Strength: night 8 of
// 256), so wetting starts at WET_START and is complete by WET_FULL, the end of the dusk.
const int WET_START = 12;
const int WET_FULL  = 85;
const int DRY_END   = 200;	// the day after a storm: dry again at this point of the day (~80% of it)

} // namespace

// (th4) Tonight's weather in the normal town (see the header). Day or not, it answers for the
// current night number; callers fade their effect with DayNight_NightQ8().
WEATHER Weather_Tonight()
{
	if( !WeatherHere() ) return WEATHER_CLEAR;
	return RollForNight( DayNight_NightIndex() );
}

// (th4) WET GROUND: how wet the normal town is, 0 (dry) .. 256 (soaked). On a STORM night it gets
// wet as the rain falls (WET_START .. WET_FULL of the night) and stays soaked until dawn; the DAY
// after a storm night it dries out (256 at sunrise, 0 at DRY_END of the day). Every other time: 0.
// The night before is roll number DayNight_NightIndex() - 1, because a cycle starts with its day.
// Same places as the weather itself (WeatherHere: normal town, not Classic, not Ice Age). Pure
// function of the clock and the rolls, so it needs no saving and is the same for every player of a
// game at the same point of the cycle. Read by TownWater.cpp (puddles) and dx_utility.cpp (Wet.hlsl).
int Weather_WetQ8()
{
	if( !WeatherHere() ) return 0;
	const int pos = DayNight_NightPosQ8();
	if( pos >= 0 ){	// dusk, night or dawn: wet only if it rains tonight
		if( RollForNight( DayNight_NightIndex() ) != WEATHER_STORM || pos <= WET_START ) return 0;
		return pos >= WET_FULL ? 256 : ( pos - WET_START ) * 256 / ( WET_FULL - WET_START );
	}
	const int day = DayNight_DayPosQ8();
	if( day < 0 || RollForNight( DayNight_NightIndex() - 1 ) != WEATHER_STORM ) return 0;
	return day >= DRY_END ? 0 : ( DRY_END - day ) * 256 / DRY_END;
}

// (th4) SUNNY DAYS: the day number the weather of "today" belongs to -- the cycle's own day by day and at
// dusk, the COMING day in the second half of the night (towards the dawn), so nothing pops at the cycle
// boundary (the day of cycle N follows night N - 1, palette.cpp DayNight_DayPosQ8).
static int SunDayNow()
{
	return DayNight_NightIndex() + ( DayNight_NightPosQ8() >= 128 ? 1 : 0 );
}

// (th4) SUNNY DAYS: is today a sunny day at all (the whole day, whether the sun is out right now or not)?
// Same places as the weather (WeatherHere: normal town, not Classic, not Ice Age). Read by dx_utility.cpp
// Clouds_Active: a sunny day has no cloud shadows, ALL day (Mordor) -- also while its sunny spell is over.
bool Weather_SunnyDay()
{
	return WeatherHere() && RollSunnyDay( SunDayNow() );
}

// (th4) SUNNY DAYS: how much sun the normal town gets right now, 0 .. 256. The sun does NOT shine all of a
// sunny day (Mordor): each sunny day has ONE SUNNY SPELL, SUN_SPELL_MIN..MAX of the day plateau long
// (DayNight_DayPosQ8 units, 256 = the whole day; about 42..58 %), starting at a point hashed from the
// day -- some days in the morning, some at noon, some in the afternoon -- with a SUN_FADE ramp at both
// ends. Nothing at dusk, night or dawn. Pure function of the clock and the rolls: the same for every
// player of a game. Read by dx_utility.cpp SunRays_Active (SUN RAYS).
// For scale: the whole day / night cycle is 6 minutes (palette.cpp DAYNIGHT_CYCLE_SEC), the day plateau
// 2/5 of it, about 144 s -- so 256 units are ~144 s and SUN_FADE 27 is ~15 s.
int Weather_SunQ8()
{
	if( !Weather_SunnyDay() ) return 0;
	const int pos = DayNight_DayPosQ8();	// 0..255 over the day plateau, -1 at dusk / night / dawn
	if( pos < 0 ) return 0;
	const int SUN_SPELL_MIN = 108, SUN_SPELL_MAX = 148, SUN_FADE = 27;
	const int day = SunDayNow();
	const uint h = Hash( ( (uint)DungeonRandSeed[ 1 ] * 0x9E3779B9U ) ^ ( (uint)day * 0x27D4EB2FU ) ^ 0x50115EE1U );
	const int len = SUN_SPELL_MIN + (int)( h % (uint)( SUN_SPELL_MAX - SUN_SPELL_MIN + 1 ) );
	const int start = (int)( ( h >> 8 ) % (uint)( 256 - len + 1 ) );
	if( pos < start || pos >= start + len ) return 0;
	const int edge = min( pos - start, start + len - 1 - pos );	// distance to the nearer end of the spell
	if( edge >= SUN_FADE ) return 256;
	const int t = edge * 256 / SUN_FADE;					// 0..255: smoothstep, so it eases in and out
	return t * t / 256 * ( 768 - 2 * t ) / 256;	// 256 * x^2 (3 - 2x), x = t / 256
}

// (th4) Lightning flash 0..256 right now; 0 outside a storm night in the normal town.
int Weather_FlashQ8()
{
	if( !InNormalTown() || Weather_Tonight() != WEATHER_STORM ) return 0;
	return FlashQ8;
}

// (th4) Rain / storm clock in seconds, frozen while paused: the time Rain.hlsl animates with.
float Weather_ClockSec()
{
	return (float)( ( ClockMs % 100000000LL ) / 1000.0 );	// wraps every ~28 h, keeps shader float precision
}

// (th4) Draw.cpp, right after DrawIsoLand / TownWater_Render / Snowfall_Render. Advances the storm
// clock, strikes and thunder -- it draws nothing (the rain is Rain.hlsl). zoomFactor is unused
// and kept so the call site matches its neighbours. Must run every drawn frame.
void Weather_Render( int zoomFactor )
{
	(void)zoomFactor;
	TickClock();
	const int night = DayNight_NightQ8();
	if( Weather_Tonight() != WEATHER_STORM || night <= 0 ){ FlashQ8 = 0; return; }
	UpdateStorm( night );
}
