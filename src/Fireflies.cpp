#include "stdafx.h"

// ============================================================================================
// (new by th4) FIREFLIES -- blinking yellow-green lights drifting over Tristram on CLEAR nights.
// ============================================================================================
//
// WHEN: the night weather roll (Weather.cpp Weather_Tonight) says CLEAR, it is the FIRST HALF of
// the night -- from dusk to the middle of the dark (palette.cpp DayNight_NightPosQ8 < 128); the
// number of fireflies rises with the dusk (DayNight_NightQ8) and thins out over FADE_OUT_POS..127
// -- and the level is the normal town -- not Classic (no night), not Ice Age, not Ravenholm.
// Fireflies_Active() is the one place that decides. Mist and storm nights have their own effects,
// so a night is never both.
//
// WHICH SYSTEM THIS IS. Like Embers.cpp: single palette pixels written into the 8-bit frame on the
// CPU right after DrawIsoLand (Draw.cpp) -- post-zoom, before Perspective_WarpWorld and the
// post-effect snapshot, under the UI. Under Enriched Light a lit firefly pixel gets LightSurface 0,
// so it glows in the dark. Colours are the nearest entries to SHADE_* in 128..254 of
// ScreenPalette, the half the town night blend leaves alone.
//
// HOW (stateless): the ground is cut into CELL_W x CELL_H cells in WORLD pixels, fixed to the map
// through the same tile (0,0) anchor the water ripple uses (dx_utility.cpp PostFxPresent c9), so
// fireflies belong to places and scroll with the town. Each cell may host one firefly per hashed
// period (CHANCE_PCT of them do): it wanders slowly on two sine paths around a hashed spot, and
// blinks -- a glow that swells and fades a few times, dark in between. A lit firefly is a bright
// centre pixel with a dim cross around it when it is at its brightest. Pause-aware clock.
//
// KNOWN-GOOD CHECK: normal town on a clear night: a scattering of small yellow-green lights,
// blinking slowly and drifting, more of them as the dusk deepens, thinning out and gone by the
// middle of the night (none in the second half, none by day); none on misty or
// stormy nights, none in Ice Age or Classic; pause freezes them.
// ============================================================================================

namespace {

const int CELL_W      = 112;	// world px (1x)
const int CELL_H      = 56;
const int CHANCE_PCT  = 45;		// share of cells that host a firefly in a given period
const int PERIOD_MIN  = 6000;	// ms a firefly lives
const int PERIOD_MAX  = 11000;
const int WANDER_X    = 22;		// px (1x) around its spot
const int WANDER_Y    = 12;
const int LIFT        = 24;		// px above the ground spot
const int FADE_OUT_POS = 96;		// night position (0..255) where they start to thin out; gone at 128
// Shades, dim + bright per kind: [0,1] firefly; [2..7] the ground lights of a GLOW night --
// cyan, violet, blue.
const int SHADE_COUNT = 8;
const int SHADE_RGB[ SHADE_COUNT ][ 3 ] = {
	{ 110, 150,  40 }, { 225, 255, 120 },	// firefly
	{  20, 120, 130 }, {  90, 240, 230 },	// cyan
	{  80,  50, 150 }, { 180, 120, 255 },	// violet
	{  25,  60, 150 }, {  90, 160, 255 },	// blue
};
// GLOW-night ground lights: CLUSTERS of small glowing plants on the grass, one colour per cluster,
// breathing slowly; each lit one also feeds the soft halo (dx_utility.cpp BioGlow_Add).
const int GL_CELL_W   = 72;		// world px (1x) per cluster cell
const int GL_CELL_H   = 36;
const int GL_CHANCE   = 35;		// % of cells with a cluster (x the night strength)
const int GL_MIN      = 3;		// lights per cluster
const int GL_MAX      = 7;
const int GL_SPREAD_X = 12;		// cluster size, 1x px
const int GL_SPREAD_Y = 6;

uchar Shade[ SHADE_COUNT ];
PALETTEENTRY ShadePal[ 256 ];
bool ShadeReady = false;
int SinTab[ 256 ];
bool SinReady = false;

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
	for( int s = 0; s < SHADE_COUNT; ++s ){
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

// ms, frozen while paused (GetTickCountTh counts 1/MAX_FPS s, enums.h)
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

// (th4) The one place that decides (see the header).
bool Fireflies_Active()
{
	if( GameMode == GM_CLASSIC || Dungeon != DUN_0_TOWN || Dungeon->isQuest ) return false;
	if( Players[ CurrentPlayerIndex ].gameChanger & BIT( GC_15_ICE_AGE ) ) return false;
	const int pos = DayNight_NightPosQ8();
	return Weather_Tonight() == WEATHER_CLEAR && DayNight_NightQ8() > 0 && pos >= 0 && pos < 128;
}

// (th4) GLOW nights (Weather.cpp WEATHER_GLOW, half of the still nights): all night, clusters of
// small cyan / violet / blue lights glow on the grass (GlowLights below) and the river glows
// (TownWater.cpp GlowRiver); dx_utility.cpp BIOGLOW adds the soft halo. Same town / Ice Age /
// Classic rules as the fireflies.
bool GlowNight_Active()
{
	if( GameMode == GM_CLASSIC || Dungeon != DUN_0_TOWN || Dungeon->isQuest ) return false;
	if( Players[ CurrentPlayerIndex ].gameChanger & BIT( GC_15_ICE_AGE ) ) return false;
	return Weather_Tonight() == WEATHER_GLOW && DayNight_NightQ8() > 0;
}

// (th4) BLOOD MOON nights (Weather.cpp WEATHER_BLOOD, 1%): the whole night, a deep red night tint,
// a dark crimson river (TownWater.cpp GlowRiver marks the water, as on glow nights) and a red halo
// along it -- all drawn by the BIOGLOW pass (dx_utility.cpp) with its blood-moon colours. No leaves,
// ground lights or fireflies. Same town / Ice Age / Classic rules as the other night types.
bool BloodNight_Active()
{
	if( GameMode == GM_CLASSIC || Dungeon != DUN_0_TOWN || Dungeon->isQuest ) return false;
	if( Players[ CurrentPlayerIndex ].gameChanger & BIT( GC_15_ICE_AGE ) ) return false;
	return Weather_Tonight() == WEATHER_BLOOD && DayNight_NightQ8() > 0;
}

static void GlowLights( long long ms, int z );

// (th4) Draw.cpp, right after DrawIsoLand (post-zoom), next to Snowfall / Weather / Embers -- and
// after TownWater_Render, so glowing river pixels are already >= 128 and never host a ground light.
void Fireflies_Render( int zoomFactor )
{
	const long long ms = Clock();	// runs every frame so the clock never jumps
	if( GlowNight_Active() && SoftSrcSurfacePtr ){ GlowLights( ms, zoomFactor < 1 ? 1 : zoomFactor ); return; }
	if( !Fireflies_Active() || !SoftSrcSurfacePtr ) return;
	int night = DayNight_NightQ8();	// 0..256: fewer fireflies early in the dusk
	const int pos = DayNight_NightPosQ8();
	if( pos > FADE_OUT_POS ) night = night * ( 128 - pos ) / ( 128 - FADE_OUT_POS );	// thin out toward midnight
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
	const int wx0 = FloorDiv( -ax, z ), wy0 = FloorDiv( -ay, z );
	const int gx0 = FloorDiv( wx0, CELL_W ) - 1, gx1 = FloorDiv( wx0 + W / z, CELL_W ) + 1;
	const int gy0 = FloorDiv( wy0, CELL_H ) - 1, gy1 = FloorDiv( wy0 + H / z, CELL_H ) + 1;

	auto plot = [&]( int sx, int sy, uchar c ){
		for( int dy = 0; dy < z; ++dy ){
			const int y = sy + dy; if( y < 0 || y >= H ) continue;
			for( int dx = 0; dx < z; ++dx ){
				const int x = sx + dx; if( x < 0 || x >= W ) continue;
				frame[ y * P + x ] = c;
				if( light ) light[ y * P + x ] = 0;	// it glows
			}
		}
	};

	for( int gy = gy0; gy <= gy1; ++gy ){
		for( int gx = gx0; gx <= gx1; ++gx ){
			const uint hs = Hash( (uint)gx * 73856093U ^ (uint)gy * 19349663U ^ 0x5bd1e995U );
			const int period = PERIOD_MIN + (int)( hs % (uint)( PERIOD_MAX - PERIOD_MIN + 1 ) );
			const long long local = ms + ( hs >> 8 ) % period;
			const uint cycle = (uint)( local / period );
			const int age = (int)( local % period );
			const uint he = Hash( hs ^ ( cycle * 2654435761U ) ), he2 = Hash( he ), he3 = Hash( he2 );
			if( (int)( he % 100 ) >= CHANCE_PCT * night / 256 ) continue;	// empty this time
			// blink: brightness 0..1024 from a sine at 0.4..0.8 Hz, lit only in its upper part;
			// the whole life fades in and out over its first / last 15%
			const int bph = (int)( ( (long long)age * ( 100 + (int)( he3 % 100 ) ) / 1000 + ( he3 >> 8 ) ) & 255 );
			int glow = SinTab[ bph ] - 350;	// -1374..674
			if( glow <= 0 ) continue;
			const int edge = age * 100 / period;
			if( edge < 15 ) glow = glow * edge / 15; else if( edge > 85 ) glow = glow * ( 100 - edge ) / 15;
			// wander around the spot
			const int px = (int)( ( (long long)age * 40 / 1000 + ( he2 & 255 ) ) & 255 );
			const int py = (int)( ( (long long)age * 29 / 1000 + ( ( he2 >> 8 ) & 255 ) ) & 255 );
			const int wx = gx * CELL_W + (int)( ( he2 >> 16 ) % CELL_W ) + SinTab[ px ] * WANDER_X / 1024;
			const int wy = gy * CELL_H + (int)( ( he3 >> 20 ) % CELL_H ) + SinTab[ py ] * WANDER_Y / 1024 - LIFT;
			const int sx = ax + wx * z, sy = ay + wy * z;
			if( sx < -z || sx >= W + z || sy < -z || sy >= H + z ) continue;
			// (th4) FIREFLY HALO (dx_utility.cpp BIOGLOW): a soft light around each lit firefly, by its glow
			BioGlow_Add( sx, sy, 8 + glow / 25 );
			if( glow > 420 ){	// brightest: dim cross around the core
				plot( sx - z, sy, Shade[ 0 ] ); plot( sx + z, sy, Shade[ 0 ] );
				plot( sx, sy - z, Shade[ 0 ] ); plot( sx, sy + z, Shade[ 0 ] );
			}
			plot( sx, sy, Shade[ glow > 150 ? 1 : 0 ] );
		}
	}
}

// (th4) The ground lights of a GLOW night: world-anchored cells (like the fireflies), each maybe
// hosting a cluster of GL_MIN..GL_MAX lights around a hashed spot. A light sits still, breathes
// slowly (its own rate and phase), is drawn only on a terrain pixel (1..127: grass, earth -- not
// the glowing river, not a sprite), glows full bright under Enriched Light, and at its brightest
// gets a dim cross. Every lit light adds to the halo map.
static void GlowLights( long long ms, int z )
{
	int night = DayNight_NightQ8();
	if( !SinReady ) BuildSin();
	if( !ShadeReady || memcmp( ShadePal, ScreenPalette, sizeof( ShadePal ) ) ) PickShades();
	const int W = ScreenWidth, H = ScreenHeight, P = WorkingWidth;
	uchar* frame = SoftSrcSurfacePtr + P * Screen_TopBorder + Screen_LeftBorder;
	uchar* light = ( g_A8L8Lighting && PixelLight && LightSurface ) ? LightSurface + P * Screen_TopBorder + Screen_LeftBorder : 0;
	const int row = -PlayerRowPos * z, col = -PlayerColPos * z;
	const int ax = 32 * ( row - col ) + ( 200 * z * ( Scroll.xOfs + PlayerShiftY ) / 100 >> 1 ) + Xofs - Screen_LeftBorder;
	const int ay = 16 * ( row + col ) + ( 200 * z * ( Scroll.yOfs + PlayerShiftX ) / 100 >> 1 ) + Yofs - Screen_TopBorder;
	const int wx0 = FloorDiv( -ax, z ), wy0 = FloorDiv( -ay, z );
	const int gx0 = FloorDiv( wx0, GL_CELL_W ) - 1, gx1 = FloorDiv( wx0 + W / z, GL_CELL_W ) + 1;
	const int gy0 = FloorDiv( wy0, GL_CELL_H ) - 1, gy1 = FloorDiv( wy0 + H / z, GL_CELL_H ) + 1;
	auto plot = [&]( int sx, int sy, uchar c ){
		for( int dy = 0; dy < z; ++dy ){
			const int y = sy + dy; if( y < 0 || y >= H ) continue;
			for( int dx = 0; dx < z; ++dx ){
				const int x = sx + dx; if( x < 0 || x >= W ) continue;
				frame[ y * P + x ] = c;
				if( light ) light[ y * P + x ] = 0;
			}
		}
	};
	for( int gy = gy0; gy <= gy1; ++gy ){
		for( int gx = gx0; gx <= gx1; ++gx ){
			const uint hc = Hash( (uint)gx * 83492791U ^ (uint)gy * 2971215073U ^ 0x27d4eb2dU );
			if( (int)( hc % 100 ) >= GL_CHANCE * night / 256 ) continue;
			const int cx = gx * GL_CELL_W + (int)( ( hc >> 8 ) % GL_CELL_W );
			const int cy = gy * GL_CELL_H + (int)( ( hc >> 16 ) % GL_CELL_H );
			const int base = 2 + 2 * (int)( ( hc >> 24 ) % 3 );	// cluster colour: cyan / violet / blue
			const int n = GL_MIN + (int)( Hash( hc ) % ( GL_MAX - GL_MIN + 1 ) );
			for( int i = 0; i < n; ++i ){
				const uint hl = Hash( hc ^ (uint)( i + 1 ) * 0x9E3779B9U );
				const int wx = cx + (int)( hl % ( 2 * GL_SPREAD_X + 1 ) ) - GL_SPREAD_X;
				const int wy = cy + (int)( ( hl >> 8 ) % ( 2 * GL_SPREAD_Y + 1 ) ) - GL_SPREAD_Y;
				const int sx = ax + wx * z, sy = ay + wy * z;
				if( sx < 0 || sx >= W || sy < 0 || sy >= H ) continue;
				const uchar under = frame[ sy * P + sx ];
				if( under == 0 || under >= 128 ) continue;	// only on the ground
				const int ph = (int)( ( ms * ( 25 + (int)( ( hl >> 16 ) % 30 ) ) / 1000 + ( hl >> 24 ) ) & 255 );	// 0.1..0.2 Hz
				const int glow = ( SinTab[ ph ] + 1024 ) * night / 256;	// 0..2048
				if( glow < 500 ) continue;
				if( glow > 1600 ){
					plot( sx - z, sy, Shade[ base ] ); plot( sx + z, sy, Shade[ base ] );
					plot( sx, sy - z, Shade[ base ] ); plot( sx, sy + z, Shade[ base ] );
				}
				plot( sx, sy, Shade[ base + ( glow > 1000 ? 1 : 0 ) ] );
				BioGlow_Add( sx, sy, glow > 1000 ? 12 : 6 );
			}
		}
	}
}
