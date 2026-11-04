#include "stdafx.h"

// ============================================================================================
// (new by th4) EMBERS -- glowing sparks rising from the lava in the caves.
// ============================================================================================
//
// WHERE: procedural caves (Dungeon->graphType DT_3_CAVE, not a quest map, not under Ice Age --
// there the lava is frozen, the same rule dx_utility.cpp HeatHaze_LevelHasLava uses) and these
// cave-tileset quest maps with lava: Passage of Fire (35), Lava Pits (73), Crater (74), River of
// Fire (75), Den of Evil (76), Vile Tunnels (77), The Bridge (78), Hell Gate (80). Never in Classic.
// Embers_Active() is the one place that decides; add a map there.
//
// WHERE ON SCREEN: an ember starts only on a LAVA pixel: palette entries 1..31, the animated lava
// range of the cave palettes (the same entries the heat-haze mask treats as lava,
// dx_utility.cpp HeatHaze_BuildMask). On cave quest maps where 1..31 is WATER (Fetid Cavern etc.)
// Embers_Active() is false, so water never sparks.
//
// WHICH SYSTEM THIS IS. Like Snowfall.cpp / DustMotes.cpp: single palette pixels written into the
// 8-bit frame on the CPU right after DrawIsoLand (Draw.cpp) -- post-zoom, before
// Perspective_WarpWorld and the post-effect snapshot, under the UI. Every lighting mode works;
// under Enriched Light an ember is drawn at full brightness (LightSurface 0): it glows.
//
// HOW (stateless, nothing stored): the ground is cut into CELL_W x CELL_H cells in WORLD pixels,
// fixed to the map through the same tile (0,0) anchor the water ripple uses (dx_utility.cpp
// PostFxPresent c9), so embers belong to the lava and scroll with it. Every cell has SLOTS ember
// slots; each slot fires once per hashed period at a hashed spot of its cell. If that spot is
// lava on screen, an ember rises from it for its lifetime, swaying and drifting with the air,
// cooling from yellow to orange to dark red, and flickering out at the end. Spots that are not
// lava simply produce nothing, so the density follows the lava. All spawn spots are tested
// against the frame BEFORE any ember is drawn (two passes), so embers never hide each other's
// spawn point.
// Colours: nearest entries to SHADE_* in 128..254 of ScreenPalette -- the upper half, which the
// cave palette cycle (1..31) never touches, so an ember does not change colour with the lava.
//
// KNOWN-GOOD CHECK: cave level 9 (or Lava Pits), a lava lake on screen: small sparks keep rising
// from it at random spots, yellow at first, turning orange and red, fading after a second or two;
// none over rock or floor; they move with the lava when walking; pause freezes them; none in the
// Fetid Cavern water and none under Ice Age.
// ============================================================================================

namespace {

const int CELL_W        = 40;	// world px (1x); cells are 2:1 like the isometric floor
const int CELL_H        = 20;
const int SLOTS         = 2;		// ember slots per cell
const int PERIOD_MIN_MS = 1600;	// each slot fires once per period (hashed per slot)
const int PERIOD_MAX_MS = 3400;
const int LIFE_MIN_MS   = 1100;	// how long an ember lives (capped by its period)
const int LIFE_MAX_MS   = 2600;
const int RISE_MIN      = 20;	// px per second, 1x
const int RISE_MAX      = 46;
const int DRIFT_PX_SEC  = 4;		// air drift to the right
const int SWAY_PX       = 3;
const int SHADE_RGB[ 3 ][ 3 ] = { { 255, 235, 130 }, { 255, 150, 40 }, { 185, 50, 20 } };	// hot, warm, cooling

uchar Shade[ 3 ];
PALETTEENTRY ShadePal[ 256 ];
bool ShadeReady = false;
int SinTab[ 256 ];
bool SinReady = false;

struct Ember { int x, y; uchar col; };
std::vector<Ember> Pending;

uint Hash( uint x )
{
	x ^= x >> 16; x *= 0x7feb352dU; x ^= x >> 15; x *= 0x846ca68bU; x ^= x >> 16;
	return x;
}

void BuildSin()
{
	// Bhaskara I approximation, error < 0.2% -- same table as Snowfall.cpp; no <math.h> here
	const double pi = 3.14159265358979;
	for( int i = 0; i < 128; ++i ){
		const double t = pi * i / 128.0, p = t * ( pi - t );
		SinTab[ i ] = (int)( 1024.0 * 16.0 * p / ( 5.0 * pi * pi - 4.0 * p ) + 0.5 );
		SinTab[ i + 128 ] = -SinTab[ i ];
	}
	SinReady = true;
}

void PickShades()
{
	for( int s = 0; s < 3; ++s ){
		int best = 255, bestD = 0x7FFFFFFF;
		for( int i = 128; i < 255; ++i ){
			const int dr = ScreenPalette[ i ].peRed - SHADE_RGB[ s ][ 0 ], dg = ScreenPalette[ i ].peGreen - SHADE_RGB[ s ][ 1 ], db = ScreenPalette[ i ].peBlue - SHADE_RGB[ s ][ 2 ];
			const int d = 2 * dr * dr + 4 * dg * dg + 3 * db * db;
			if( d < bestD ){ bestD = d; best = i; }
		}
		Shade[ s ] = (uchar)best;
	}
	memcpy( ShadePal, ScreenPalette, sizeof( ShadePal ) );
	ShadeReady = true;
}

// Ember clock in ms, frozen while paused (GetTickCountTh counts 1/MAX_FPS s, enums.h).
long long Clock()
{
	static long long last = 0, ticks = 0;
	const long long now = GetTickCountTh();
	long long d = now - last;
	last = now;
	if( d < 0 || d > MAX_FPS ) d = 0;
	if( !GamePaused && !IsPausedInMenu() ) ticks += d;
	return ticks * 1000 / MAX_FPS;
}

int FloorDiv( int a, int b ){ return a >= 0 ? a / b : -( ( -a + b - 1 ) / b ); }

} // namespace

// (th4) The one place that decides the maps (see the header).
bool Embers_Active()
{
	if( GameMode == GM_CLASSIC ) return false;	// never in Classic (Mordor's call)
	if( Dungeon->graphType != DT_3_CAVE ) return false;
	if( !Dungeon->isQuest ) return !( Players[ CurrentPlayerIndex ].gameChanger & BIT( GC_15_ICE_AGE ) );
	return is( (DUNGEON)Dungeon, DUN_35_PASSAGE_OF_FIRE, DUN_73_LAVA_PITS, DUN_74_CRATER, DUN_75_RIVER_OF_FIRE,
	           DUN_76_DEN_OF_EVIL, DUN_77_VILE_TUNNELS, DUN_78_THE_BRIDGE, DUN_80_HELL_GATE );
}

// (th4) Draw.cpp, right after DrawIsoLand (post-zoom), next to Snowfall / Weather / DustMotes.
// zoomFactor: 2 when DrawIsoLand stretched the world, else 1.
void Embers_Render( int zoomFactor )
{
	const long long ms = Clock();	// runs every frame so the clock never jumps
	if( !Embers_Active() || !SoftSrcSurfacePtr ) return;
	const int z = zoomFactor < 1 ? 1 : zoomFactor;
	if( !SinReady ) BuildSin();
	if( !ShadeReady || memcmp( ShadePal, ScreenPalette, sizeof( ShadePal ) ) ) PickShades();
	const int W = ScreenWidth, H = ScreenHeight, P = WorkingWidth;
	uchar* frame = SoftSrcSurfacePtr + P * Screen_TopBorder + Screen_LeftBorder;
	uchar* light = ( g_A8L8Lighting && PixelLight && LightSurface ) ? LightSurface + P * Screen_TopBorder + Screen_LeftBorder : 0;

	// world anchor: screen position (post-zoom game px) of tile (0,0), dx_utility.cpp PostFxPresent c9
	const int row = -PlayerRowPos * z, col = -PlayerColPos * z;
	const int ax = 32 * ( row - col ) + ( 200 * z * ( Scroll.xOfs + PlayerShiftY ) / 100 >> 1 ) + Xofs - Screen_LeftBorder;
	const int ay = 16 * ( row + col ) + ( 200 * z * ( Scroll.yOfs + PlayerShiftX ) / 100 >> 1 ) + Yofs - Screen_TopBorder;

	// cells covering the screen (plus the height an ember can rise, below the screen)
	const int wx0 = FloorDiv( -ax, z ), wy0 = FloorDiv( -ay, z );
	const int gx0 = FloorDiv( wx0, CELL_W ) - 1, gx1 = FloorDiv( wx0 + W / z, CELL_W ) + 1;
	const int gy0 = FloorDiv( wy0, CELL_H ) - 1, gy1 = FloorDiv( wy0 + H / z + RISE_MAX * LIFE_MAX_MS / 1000, CELL_H ) + 1;

	Pending.clear();
	for( int gy = gy0; gy <= gy1; ++gy ){
		for( int gx = gx0; gx <= gx1; ++gx ){
			for( int k = 0; k < SLOTS; ++k ){
				const uint hs = Hash( (uint)gx * 73856093U ^ (uint)gy * 19349663U ^ (uint)k * 83492791U );
				const int period = PERIOD_MIN_MS + (int)( hs % (uint)( PERIOD_MAX_MS - PERIOD_MIN_MS + 1 ) );
				const long long local = ms + ( hs >> 8 ) % period;
				const uint cycle = (uint)( local / period );
				const int age = (int)( local % period );
				const uint he = Hash( hs ^ ( cycle * 2654435761U ) ), he2 = Hash( he ), he3 = Hash( he2 );
				int life = LIFE_MIN_MS + (int)( he % (uint)( LIFE_MAX_MS - LIFE_MIN_MS + 1 ) );
				if( life > period ) life = period;
				if( age >= life ) continue;
				// spawn spot, world 1x px -> screen (post-zoom)
				const int wx = gx * CELL_W + (int)( he2 % CELL_W ), wy = gy * CELL_H + (int)( ( he2 >> 12 ) % CELL_H );
				const int sx0 = ax + wx * z, sy0 = ay + wy * z;
				if( sx0 < 0 || sx0 >= W || sy0 < 0 || sy0 >= H ) continue;
				const uchar under = frame[ sy0 * P + sx0 ];
				if( under < 1 || under > 31 ) continue;			// not lava: no ember from here this time
				// flight
				const int rise = RISE_MIN + (int)( he3 % (uint)( RISE_MAX - RISE_MIN + 1 ) );
				const int ph = (int)( ( (long long)age * 180 / 1000 + ( he3 >> 8 ) ) & 255 );	// ~0.7 Hz sway
				const int x1 = wx + age * DRIFT_PX_SEC / 1000 + SinTab[ ph ] * SWAY_PX / 1024;
				const int y1 = wy - age * rise / 1000;
				// cooling and flicker
				const int f = age * 100 / life;
				if( f > 80 && ( Hash( he3 ^ (uint)( age / 60 ) ) & 1 ) ) continue;
				const uchar c8 = Shade[ f < 30 ? 0 : ( f < 65 ? 1 : 2 ) ];
				Pending.push_back( { ax + x1 * z, ay + y1 * z, c8 } );
			}
		}
	}
	for( const Ember& e : Pending ){
		for( int dy = 0; dy < z; ++dy ){
			const int sy = e.y + dy;
			if( sy < 0 || sy >= H ) continue;
			for( int dx = 0; dx < z; ++dx ){
				const int sx = e.x + dx;
				if( sx < 0 || sx >= W ) continue;
				const int p = sy * P + sx;
				frame[ p ] = e.col;
				if( light ) light[ p ] = 0;		// it glows
			}
		}
	}
}
