#include "stdafx.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

// ============================================================================================
// (new by th4) SNOWFALL -- a BLIZZARD in Tristram and Ravenholm under the Ice Age game changer.
// ============================================================================================
//
// WHEN: always on (no config switch, by design) when the current player has GC_15_ICE_AGE and
// the level is the town (DUN_0_TOWN, not a quest map) or Ravenholm (DUN_44_RAVENHOLM, a quest map
// on the town tileset). Snowfall_Active() is the one place that decides; add a map there.
//
// WHICH SYSTEM THIS IS. Not a shader and not a post effect: flakes are palette pixels written
// into the 8-bit frame on the CPU, the same slot as the town river (TownWater.cpp):
//   Draw.cpp, right after DrawIsoLand (so AFTER the 2x zoom stretch) and BEFORE
//   Perspective_WarpWorld and the world snapshot the post effects take. So flakes lie over the
//   whole scene, never over panels, text or the cursor (the UI is drawn later), are warped by
//   Perspective with the world, and count as world for the ripple / heat-haze mask.
// Every lighting mode works: classic draws the palette entry as is; under Enriched Light the
// flake keeps the light value of the ground under it (brightened a little: SNOW_LIGHT_LIFT), so
// snow is dim at night and bright near torches and the hero.
//
// THE LOOK (Mordor, 2026-10-09: "intense blizzard", after a blizzard video -- he liked the
// amount of snow, how it moves and its pattern): a lot of fine snow driven hard by the wind,
// sideways and down -- and the faster flakes read as short STREAKS along their path. Three depth layers:
// far (fine dim dots, slow -- they lag behind the wind), mid (thin streaks) and near (big bright
// streaks racing past). Each flake also churns a little on its own (CHURN).
//
// HOW. Stateless: each flake's position is a pure function of its index and the time, inside a
// field one screen large that repeats in every direction. The field is fixed to the SCREEN in both
// directions: the hero walking or the view scrolling never moves the snow (Mordor, 2026-10-09:
// "don't make player movement affect it"). An earlier version anchored it to the ground
// vertically, so the snow scrolled with the world when walking north or south -- he did not want
// that. Positions are computed in 1x pixels and drawn as zoom x zoom blocks.
// THE SPELLS (Mordor, 2026-10-10: the gusts and the calm "last not long enough ... I'd like longer periods that can last a
// few minutes, 10-20 seconds minimum ... I need more range in their timings"): the weather takes turns between a CALM
// SNOWFALL (gentle wind, CALM_WIND0..1 of the mean) and a STORM (STORM_WIND0..1 of the mean). WITHIN A SPELL THE SPEED IS
// FIXED -- one wind, one fall speed, from its start to its end (Mordor: "can it stick to one speed for the periods I told
// you about"); it changes ONLY in the ramp between two spells. Each spell's length is random, LOG-UNIFORM in
// CALM_MIN..MAX_MS / STORM_MIN..MAX_MS, so short and long spells both come often; each spell also rolls its own strength;
// the wind builds up or dies down over a random RAMP_MIN..MAX_MS between spells (smoothstep). A new sequence (own RNG,
// never the game's) starts on every entry into Tristram or Ravenholm (SpellsReset), with a random first mood.
// NO SURGES: before the spells, the weather was two waves (+-45% over 7.3 s, +-25% over 2.9 s, the fall +-20% with the
// slow one): a surge every 3..7 s, forever. Keeping those waves inside the spells (the first version of the spells) still
// read as "the speed changes every several seconds" -- they were removed; do not bring them back.
// A flake's sideways travel is the INTEGRAL of the wind speed, summed per frame on the snow clock (SpellsAdvance:
// BaseTravel, in px at 100% wind; BaseFall likewise), so the snow speeds up and slows down smoothly in a ramp and every
// flake of a layer feels the same wind. The fall speed follows the wind's strength a little. A streak's tail points back
// along the layer's CURRENT flight direction, so the streaks lie flatter in a storm and stand up in a calm.
// Each flake's start point, speed factor and churn are worked out once (BuildFlakes) and kept
// until the field size changes -- the frame loop does no hashing.
//
// TIME: flakes move on their own clock (SnowClock), frozen while the game is paused and running
// at SNOW_TIME_PCT of real time.
//
// COLOURS: the three shades are the nearest entries to SHADE_* in 128..254 of ScreenPalette,
// found again whenever the palette changes (day/night, Ice Age, level palette). That half of
// the palette is the shared sprite/UI half, which the town day/night blend (palette.cpp
// DAYNIGHT_TERRAIN_FIRST/LAST = 1..127) leaves alone, so flakes do not turn into ground colours
// at night. A streak's tail is drawn one shade dimmer than its head (two dimmer past TAIL_FADE).
//
// AMOUNT: perMegaPixel per layer -- Mordor's numbers after two play-tests (2026-10-09): the fine far dots x3 then x4
// (168000), the big near streaks halved twice (450). If it is ever too much, the far dots are the first to cut.
// COST: about 120,000 flakes at 1056 x 594, nearly all far dots (a quarter as many with the 2x zoom: the field is in
// 1x pixels). So the frame loop is kept to table look-ups and adds: a flake's travel comes from a per-layer table
// over its 41 possible speed factors (travelTabX / Y), its wobble from per-layer tables over 16 wobble rates (ratePhase, churnX / Y),
// the wrap is two compares -- no division, no 64-bit multiply, no hashing per flake. Under the Ice Age the town
// has no grass, sun rays or cloud shadows, which cost far more.
//
// KNOWN-GOOD CHECK: Ice Age, town: thick snow blowing to the right and down, the near streaks
// racing past faster than the fine far snow; spells of calm snowfall (steep streaks, little wind) and of storm (flat
// streaks, fast) take turns, each from about 10-15 s to several minutes at ONE steady speed, easing into each other. Walk in any direction: the snow does not care -- it
// blows across the screen exactly as when standing still. Pause: the snow stops. Open the inventory: no flake is drawn on it. Leave Ice Age or go into the
// dungeon: no snow.
// ============================================================================================

namespace {

struct Layer {
	int perMegaPixel;	// flakes per 1,000,000 1x-pixels of screen
	int fallPxPerSec;	// mean fall, 1x px per second of SNOW time (SNOW_TIME_PCT), downwards
	int windPct;		// share of the wind this layer moves with: far snow lags, near snow races
	int size;			// head size, 1 or 2 (1x px)
	int streak;			// tail length in 1x px along the flight path (0 = a dot)
	int shade;			// 0 far (dim) .. 2 near (bright)
	int churnPx;		// the flake's own wobble, 1x px
};
const Layer kLayers[] = {
	{ 168000,  70,  60, 1, 0, 0, 3 },	// far: fine dim dots
	{   8500, 120, 100, 1, 2, 1, 4 },	// mid: thin streaks
	{    450, 190, 150, 2, 4, 2, 5 },	// near: big bright streaks
};
const int WIND_PX_PER_SEC = 260;		// mean wind to the right, 1x px per second of snow time (x windPct)
// THE SPELLS (header): lengths in ms of snow time, wind as a share of WIND_PX_PER_SEC
const int CALM_MIN_MS = 30000, CALM_MAX_MS = 205000;		// a calm snowfall: 30 s .. 3 min 25 s (Mordor, 2026-10-11: +15 .. 25 s; was 15 s .. 3 min)
const int STORM_MIN_MS = 25000, STORM_MAX_MS = 325000;		// a storm: 25 s .. 5 min 25 s (Mordor, 2026-10-11: +15 .. 25 s; was 10 s .. 5 min)
const int RAMP_MIN_MS = 8000, RAMP_MAX_MS = 15000;			// the change from one to the other: 8 .. 15 s (Mordor, 2026-10-11: +4 .. 7 s; was 4 .. 8 s)
const float CALM_WIND0 = 0.4f, CALM_WIND1 = 0.6f;			// a calm's wind, rolled per spell
const float STORM_WIND0 = 2.64f, STORM_WIND1 = 3.36f;		// a storm's wind, rolled per spell (Mordor, 2026-10-11: +20 % of his 2.2 .. 2.8; it was 1.3 .. 1.8)
const int TAIL_FADE = 2;				// tail pixels past this many are two shades dimmer than the head
const int SNOW_TIME_PCT = 100;			// the snow clock runs at this % of real time: one knob for fall, wind, gusts and churn
const int SNOW_LIGHT_LIFT = 70;			// Enriched Light: flake light = ground light x this % (lower = brighter)
const int SHADE_RGB[ 3 ][ 3 ] = { { 150, 155, 170 }, { 200, 205, 215 }, { 240, 242, 250 } };
// (th4) ICE AGE NIGHT (Snowfall_NightQ8): by night the shades move toward these -- the far dots to about the moonlit
// snow's brightness (they vanish against the snow and show as faint points over dark houses and trees, as distant flakes
// do at night), the near streaks a little brighter. The sprite half they are picked from never darkens with the town
// night, so at day shades they covered the night screen with bright grain.
const int SHADE_NIGHT_RGB[ 3 ][ 3 ] = { { 125, 138, 172 }, { 150, 164, 198 }, { 190, 204, 234 } };
const int LAYER_COUNT = (int)( sizeof( kLayers ) / sizeof( kLayers[ 0 ] ) );

uchar ShadeIdx[ 3 ];
PALETTEENTRY ShadePal[ 256 ];		// palette the shades were picked from
int ShadeNight = -1;				// (th4) ICE AGE NIGHT: the night (0..256) they were picked for
bool ShadeReady = false;
int SinTab[ 256 ];					// sin(2*pi*i/256) * 1024; no <math.h> in this project (stdafx.h)
bool SinReady = false;

struct Flake { int x0, y0; uchar speed, phX, phY, rateX, rateY; };	// speed 0..40 = 80..120 %, rates 0..15 (CHURN_RATES)
const int SPEEDS = 41, CHURN_RATES = 16;
std::vector<Flake> Flakes[ LAYER_COUNT ];
int FlakesFW = 0, FlakesFH = 0;

uint Hash( uint x )
{
	x ^= x >> 16; x *= 0x7feb352dU; x ^= x >> 15; x *= 0x846ca68bU; x ^= x >> 16;
	return x;
}

// Bhaskara I: sin(t) ~ 16 t (pi - t) / (5 pi^2 - 4 t (pi - t)), t in [0, pi]; error < 0.2%
const double PI = 3.14159265358979;
double SinPi( double t )				// t in [0, pi]
{
	const double p = t * ( PI - t );
	return 16.0 * p / ( 5.0 * PI * PI - 4.0 * p );
}

void BuildSin()
{
	for( int i = 0; i < 128; ++i ){
		SinTab[ i ] = (int)( 1024.0 * SinPi( PI * i / 128.0 ) + 0.5 );
		SinTab[ i + 128 ] = -SinTab[ i ];
	}
	SinReady = true;
}

void PickShades( int night )
{
	ShadeNight = night;
	for( int s = 0; s < 3; ++s ){
		int t[ 3 ];	// the target: day shade .. night shade (ICE AGE NIGHT)
		for( int c = 0; c < 3; ++c ) t[ c ] = SHADE_RGB[ s ][ c ] + ( SHADE_NIGHT_RGB[ s ][ c ] - SHADE_RGB[ s ][ c ] ) * night / 256;
		int best = 255, bestD = 0x7FFFFFFF;
		for( int i = 128; i < 255; ++i ){
			const int dr = ScreenPalette[ i ].peRed - t[ 0 ], dg = ScreenPalette[ i ].peGreen - t[ 1 ], db = ScreenPalette[ i ].peBlue - t[ 2 ];
			const int d = 2 * dr * dr + 4 * dg * dg + 3 * db * db;
			if( d < bestD ){ bestD = d; best = i; }
		}
		ShadeIdx[ s ] = (uchar)best;
	}
	memcpy( ShadePal, ScreenPalette, sizeof( ShadePal ) );
	ShadeReady = true;
}

// each flake's fixed part, once per field size (see HOW in the header)
void BuildFlakes( int FW, int FH )
{
	for( int li = 0; li < LAYER_COUNT; ++li ){
		const int count = (int)( (long long)FW * FH * kLayers[ li ].perMegaPixel / 1000000 );
		Flakes[ li ].resize( count );
		for( int i = 0; i < count; ++i ){
			const uint h1 = Hash( (uint)( i * 3 + li * 7919 + 1 ) ), h2 = Hash( h1 ), h3 = Hash( h2 ), h4 = Hash( h3 );
			Flake& f = Flakes[ li ][ i ];
			f.x0 = (int)( h1 % (uint)FW );
			f.y0 = (int)( h2 % (uint)FH );
			f.speed = (uchar)( h3 % SPEEDS );				// 80..120% of the layer's wind and fall
			f.phX = (uchar)( h4 & 255 ); f.phY = (uchar)( ( h4 >> 8 ) & 255 );
			f.rateX = (uchar)( ( h3 >> 8 ) % CHURN_RATES );	// churn: 300 + 40 x rate, 256 = one wobble -> 1.2..3.5 per second
			f.rateY = (uchar)( ( h4 >> 16 ) % CHURN_RATES );
		}
	}
	FlakesFW = FW; FlakesFH = FH;
}

// THE SPELLS (header). The state is this file's own and local (not saved, not synced): cosmetic only.
uint SpellRng = 1;
bool SpellStorm = false;
uint SpellStart = 0, SpellLen = 1, RampLen = 1;
float SpellWind = 1.0f, PrevWind = 1.0f;
double BaseTravel = 0.0;	// sideways travel so far, 1x px at 100% wind (a layer: x WIND_PX_PER_SEC x windPct)
double BaseFall = 0.0;		// fall so far in "seconds of mean fall" (a layer: x fallPxPerSec)
uint SpellLastMs = 0;
int SpellDun = -1;			// the level the sequence belongs to: a new one on every entry
float WindNow = 1.0f, FallNow = 1.0f;	// this frame's wind (x WIND_PX_PER_SEC) and fall (x fallPxPerSec) factors

float SpellRand()	// 0..1, xorshift: never the game's RNG
{
	SpellRng ^= SpellRng << 13; SpellRng ^= SpellRng >> 17; SpellRng ^= SpellRng << 5;
	return ( SpellRng >> 8 ) / 16777216.0f;
}
uint LogUniformMs( int lo, int hi )	// short and long spells equally often per doubling of length
{
	const double l = log( (double)lo ), h = log( (double)hi );
	return (uint)exp( l + ( h - l ) * SpellRand() );
}
void StartSpell( uint startMs, bool storm )
{
	PrevWind = SpellWind;
	SpellStorm = storm; SpellStart = startMs;
	SpellLen = storm ? LogUniformMs( STORM_MIN_MS, STORM_MAX_MS ) : LogUniformMs( CALM_MIN_MS, CALM_MAX_MS );
	RampLen = RAMP_MIN_MS + (uint)( ( RAMP_MAX_MS - RAMP_MIN_MS ) * SpellRand() );
	SpellWind = storm ? STORM_WIND0 + ( STORM_WIND1 - STORM_WIND0 ) * SpellRand() : CALM_WIND0 + ( CALM_WIND1 - CALM_WIND0 ) * SpellRand();
}
void SpellsReset( uint ms )	// a new random sequence, starting in a random mood with no ramp
{
	SpellRng = (uint)GetTickCount() * 2654435761u | 1u;
	for( int i = 0; i < 4; ++i ) SpellRand();
	StartSpell( ms, SpellRand() < 0.5f );
	PrevWind = SpellWind;
	SpellLastMs = ms;
}
// once per frame: the mood now, and the travel / fall summed over the frame's snow time
void SpellsAdvance( uint ms )
{
	if( SpellDun != (int)(DUNGEON)Dungeon ){ SpellDun = (int)(DUNGEON)Dungeon; SpellsReset( ms ); }
	while( ms - SpellStart >= SpellLen ) StartSpell( SpellStart + SpellLen, !SpellStorm );
	float r = (float)( ms - SpellStart ) / (float)RampLen;
	if( r > 1.0f ) r = 1.0f;
	r = r * r * ( 3.0f - 2.0f * r );
	const float wind = PrevWind + ( SpellWind - PrevWind ) * r;	// steady within a spell (NO SURGES)
	WindNow = wind;
	FallNow = 0.85f + 0.2f * ( wind - 0.5f );
	const uint dt = ms - SpellLastMs;	// the snow clock stands still in a pause
	SpellLastMs = ms;
	if( dt > 0 && dt < 1000 ){ BaseTravel += WindNow * dt / 1000.0; BaseFall += FallNow * dt / 1000.0; }
}

// Snow time in ms. GetTickCountTh() counts 1/MAX_FPS s (MAX_FPS = 10000, enums.h), converted
// here. It keeps running while the game is paused, so the snow keeps
// its own clock that only advances when nothing is paused -- the same pattern as
// AnimOrbs.cpp AnimOrbs_Frame. GamePaused (1 on the frame it is entered, 2 settled, Main.cpp)
// and IsPausedInMenu() (the single-player ESC menu) both freeze it, the same pair Draw.cpp
// DrawIsoLand checks before motion interpolation.
uint SnowClock()
{
	static long long lastTick = 0, snowTicks = 0;
	const long long now = GetTickCountTh();
	long long delta = now - lastTick;
	if( delta < 0 || delta > MAX_FPS ) delta = 0;	// first call, or a stall of over a second
	lastTick = now;
	if( !GamePaused && !IsPausedInMenu() ) snowTicks += delta;
	return (uint)( snowTicks * 1000 / MAX_FPS * SNOW_TIME_PCT / 100 );
}

// (th4) FAR DOTS ON SEVERAL THREADS (PERFORMANCE, Mordor 2026-10-11: no visual change). The far layer is ~114,000 one-pixel
// dots a frame (COST above), all the SAME palette entry and nothing else drawn meanwhile, so the picture does not depend
// on which dot is written first: the layer is cut into DOT_PARTS pieces, drawn at once by DOT_PARTS - 1 helper threads
// and this one, and the mid / near layers are drawn only after ALL of them (they paint over the far dots, as before).
// Not with the Enriched Light buffer: there each dot also darkens the light under it (read, then write), two threads on
// one pixel could lose a step -- that case keeps the single loop. DOT_PARTS 1 = the single loop always.
const int DOT_PARTS = 4;
struct DotJob {
	const Flake* flakes; int count;
	const int* travelTabX; const int* travelTabY; const int* ratePhase; const int* churnX; const int* churnY;
	int FW, FH, margin, W, H, P;
	uchar* frame; uchar c8;
};
// the dots of flakes [ b, e ) of the job: the far layer's loop body in Snowfall_Render, unchanged
void DrawDots( const DotJob& j, int b, int e )
{
	const Flake* const end = j.flakes + e;
	for( const Flake* fp = j.flakes + b; fp < end; ++fp ){
		const Flake& f = *fp;
		int x = f.x0 + j.travelTabX[ f.speed ] + j.churnX[ ( j.ratePhase[ f.rateX ] + f.phX ) & 255 ];
		int y = f.y0 + j.travelTabY[ f.speed ] + j.churnY[ ( j.ratePhase[ f.rateY ] + f.phY ) & 255 ];
		if( x >= j.FW ) x -= j.FW; if( x >= j.FW ) x -= j.FW; if( x < 0 ) x += j.FW;
		if( y >= j.FH ) y -= j.FH; if( y >= j.FH ) y -= j.FH; if( y < 0 ) y += j.FH;
		const int x1 = x - j.margin, y1 = y - j.margin;
		if( (uint)x1 < (uint)j.W && (uint)y1 < (uint)j.H ) j.frame[ y1 * j.P + x1 ] = j.c8;
	}
}
DotJob DotCur;
std::thread DotThreads[ DOT_PARTS - 1 ];
std::mutex DotMtx; std::condition_variable DotCv;
unsigned DotGen = 0;					// under DotMtx: a new job for every helper
std::atomic<int> DotDone( 0 );			// helpers finished with the current job
bool DotQuit = false;					// under DotMtx
bool DotStarted = false, DotBroken = false;	// main thread only
void DotHelper( int part )
{
	unsigned seen = 0;
	for( ;; ){
		{
			std::unique_lock<std::mutex> lk( DotMtx );
			DotCv.wait( lk, [ & ]{ return DotQuit || DotGen != seen; } );
			if( DotQuit ) return;
			seen = DotGen;
		}
		const int n = DotCur.count;
		DrawDots( DotCur, (int)( (long long)n * part / DOT_PARTS ), (int)( (long long)n * ( part + 1 ) / DOT_PARTS ) );
		DotDone.fetch_add( 1 );
	}
}
// at exit: the helpers stopped before what they use is destroyed (declared after it; Draw.cpp PPLPoolGuard's pattern)
struct DotGuard { ~DotGuard(){
	{ std::lock_guard<std::mutex> lk( DotMtx ); DotQuit = true; }
	DotCv.notify_all();
	for( std::thread& t : DotThreads ) if( t.joinable() ) t.join();
} };
DotGuard DotStop;
// the whole job: part 0 here, parts 1 .. DOT_PARTS - 1 on the helpers; back when every dot is in the frame
void DrawDotsAll( const DotJob& j )
{
	if( DOT_PARTS > 1 && !DotStarted ){
		DotStarted = true;
		try{ for( int i = 0; i < DOT_PARTS - 1; ++i ) DotThreads[ i ] = std::thread( DotHelper, i + 1 ); }
		catch( ... ){ DotBroken = true; }
	}
	if( DOT_PARTS <= 1 || DotBroken ){ DrawDots( j, 0, j.count ); return; }
	DotCur = j;
	DotDone.store( 0 );
	{ std::lock_guard<std::mutex> lk( DotMtx ); ++DotGen; }
	DotCv.notify_all();
	DrawDots( j, 0, (int)( (long long)j.count / DOT_PARTS ) );
	while( DotDone.load() < DOT_PARTS - 1 ) std::this_thread::yield();
}

// (th4) SNOWFALL ON THE GPU (Mordor, 2026-10-11: "real snow looks more like rice in motion"; GPU, +50..75 % snow). The
// flakes are drawn by dx_utility.cpp Snowfall_Present -- SnowFlakesVS.hlsl + SnowFlakes.hlsl, one instanced quad per
// flake, in PostFxPresent after the breath, over world pixels only (the world/UI mask) -- at WINDOW resolution, each a
// soft RICE GRAIN: an ellipse lying along the flake's own flight right now (its layer's wind and fall x its
// speed factor, + its churn's motion), as long as its speed over GPU_EXPOSURE (at least its layer's minimum), its tips
// fading like motion blur. SCREEN BLEND (frame + grain x ( 1 - frame )): a grain only ever brightens -- full shade over
// dark walls, roofs, trees and the night, a faint white speck on the bright snow ground (the 8-bit flakes showed there as
// white specks too: the snow pass lit them up). Two tries rejected the same day (Mordor: "snow looks as if it's less of
// it"): 55..90 % see-through grains about 1 px wide (half an old dot's light each), and a LIGHTEN-ONLY (max) blend, which
// hid every flake on the snow. The grains are opaque at the core and the far ones ~1.6 px wide, as the old 1-game-px dot.
// The MOTION is the 8-bit flakes' own (THE SPELLS, the travel, the speed factors, the churn),
// but continuous: Snowfall_Render still runs the clock and the spells every frame, and only stops drawing.
// SNOW UNITS (su): px of a 1080-high window (SU_PER_GAMEPX of the 8-bit flakes' game px), so the snow looks the same at
// every window size. Amount = the layers' perMegaPixel x GPU_AMOUNT_PCT %, and in a STORM GPU_STORM_EXTRA_PCT % more
// (Mordor, 2026-10-11: "+100 % when storm wind blows, not instantly, gradually", then 50 % more: a storm = 3 x the calm's
// amount): each layer also holds that many EXTRA
// flakes, each with its own threshold on Snowfall_Storm (0.1 .. 1, hashed) -- an extra flake fades in over the last 0.1 of
// the storm strength below its threshold, so while the wind builds up (THE SPELLS' ramp) they appear one by one all over
// the screen, and go the same way when it dies down; at 0 they are collapsed in the vertex shader (no pixels, no cost).
// The 8-bit fallback flakes keep one amount. Grains: kGpuGrains -- also how far and how fast
// each layer CHURNS on the GPU: the near flakes' full churn (+-9 su at 1.2 .. 3.5 per s, the grain turning with it) read
// as big flakes wobbling too much (Mordor). Colours: SHADE_RGB ..
// SHADE_NIGHT_RGB with the night, true colour.
// WHEN: Snowfall_GpuWanted -- not Classic (it keeps the 8-bit flakes), DrawMode 1, not DDraw, and only until the GPU
// pass once failed (Snowfall_GpuFailed: no vertex shader 3.0 / instancing / hardware vertex processing, or a resource):
// then the 8-bit flakes again for the session.
const double SU_PER_GAMEPX = 1080.0 / 594.0;	// the 8-bit flakes' numbers are game px of the default 594-high game screen
const int GPU_MARGIN = 30;						// su round the window: grains enter from off screen
const int GPU_AMOUNT_PCT = 160;					// Mordor: +50 .. 75 %
const int GPU_STORM_EXTRA_PCT = 200;			// Mordor: +100 % in a storm, gradually (above); then 50 % more of the storm's amount = 3 x
const float GPU_EXPOSURE = 1.0f / 300.0f;		// s: how long a grain's streak is at its speed
// su, su, su (the soft edge: wider = softer), 0..1; churn: x the layer's churnPx; rate: x its churn rates -- in TENTHS
// (0.1 .. 1.0), or the churn would jump each time the clock wraps (SnowFlakesVS.hlsl c1.z: every rate must turn whole there)
struct GpuGrain { float minLen, halfWidth, soft, alpha, churn, rate; };
const GpuGrain kGpuGrains[ LAYER_COUNT ] = {
	{ 2.8f, 0.80f, 1.0f, 1.0f, 1.0f, 1.0f },	// far: fine grains, the dim shade
	{ 4.2f, 0.95f, 1.0f, 1.0f, 0.8f, 0.8f },	// mid
	{ 6.0f, 1.20f, 1.6f, 1.0f, 0.4f, 0.6f },	// near: bigger and softer (closer to the eye), calmer
};
std::vector<float> GpuFlakes;		// 8 floats a flake: x0, y0 su, speed 0..40, layer + 0.98 x storm threshold (0 = always), churn phase x, y, churn rate x, y
int GpuFW = 0, GpuFH = 0;			// the field, su
uint GpuGen = 0;					// changes whenever GpuFlakes does (dx_utility.cpp rebuilds its buffer)
bool GpuBroken = false;
uint FrameMs = 0;					// this frame's snow clock (Snowfall_Render)

} // namespace

// (th4) SNOWFALL ON THE GPU (above): is this frame's snow drawn by dx_utility.cpp Snowfall_Present?
bool Snowfall_GpuWanted()
{
	return !GpuBroken && GameMode != GM_CLASSIC && DrawMode == 1 && !DDraw && Snowfall_Active();
}
void Snowfall_GpuFailed() { GpuBroken = true; }
// the flakes for a window of winW x winH px, built again when the field changes; returns the count (8 floats each)
int Snowfall_GpuFlakes( int winW, int winH, const float*& data, uint& gen )
{
	if( winW <= 0 || winH <= 0 ) return 0;
	const int FW = (int)( winW * 1080.0 / winH + 0.5 ) + 2 * GPU_MARGIN, FH = 1080 + 2 * GPU_MARGIN;
	if( FW != GpuFW || FH != GpuFH ){
		GpuFlakes.clear();
		for( int li = 0; li < LAYER_COUNT; ++li ){	// far first: the near grains are drawn last, on top
			const int count = (int)( (double)FW * FH * kLayers[ li ].perMegaPixel * GPU_AMOUNT_PCT / 100.0 / ( SU_PER_GAMEPX * SU_PER_GAMEPX ) / 1000000.0 );
			const int all = count + count * GPU_STORM_EXTRA_PCT / 100;	// the first count always, the rest storm only (above)
			for( int i = 0; i < all; ++i ){
				const uint h1 = Hash( (uint)( i * 3 + li * 7919 + 101 ) ), h2 = Hash( h1 ), h3 = Hash( h2 ), h4 = Hash( h3 );
				const float storm = i < count ? 0.0f : 0.1f + 0.9f * ( ( h2 >> 20 ) & 1023 ) / 1023.0f;	// its threshold
				const float f[ 8 ] = { (float)( h1 % (uint)FW ) + ( ( h4 >> 24 ) & 255 ) / 256.0f, (float)( h2 % (uint)FH ) + ( ( h3 >> 24 ) & 255 ) / 256.0f,
				                       (float)( h3 % SPEEDS ), (float)li + 0.98f * storm, (float)( h4 & 255 ), (float)( ( h4 >> 8 ) & 255 ),
				                       (float)( ( h3 >> 8 ) % CHURN_RATES ), (float)( ( h4 >> 16 ) % CHURN_RATES ) };
				GpuFlakes.insert( GpuFlakes.end(), f, f + 8 );
			}
		}
		GpuFW = FW; GpuFH = FH;
		if( !++GpuGen ) GpuGen = 1;
	}
	data = GpuFlakes.data(); gen = GpuGen;
	return (int)( GpuFlakes.size() / 8 );
}
// this frame's vertex shader constants (SnowFlakesVS.hlsl c0 ..): n float4s into c (room for 2 + 4 x LAYER_COUNT)
bool Snowfall_GpuConsts( int winW, int winH, float* c, int& n )
{
	if( !GpuFW || winH <= 0 ) return false;
	const float c0[ 8 ] = { (float)winW, (float)winH, winH / 1080.0f, (float)GpuFW, (float)GpuFH, (float)GPU_MARGIN, (float)( FrameMs % 256000u ), Snowfall_Storm() };
	memcpy( c, c0, sizeof( c0 ) );
	const int night = Snowfall_NightQ8();
	for( int li = 0; li < LAYER_COUNT; ++li ){
		const Layer& L = kLayers[ li ];
		const GpuGrain& gg = kGpuGrains[ li ];
		const double wind = WIND_PX_PER_SEC * L.windPct / 100.0 * SU_PER_GAMEPX, fall = L.fallPxPerSec * SU_PER_GAMEPX;	// su / s at 100 %
		float* o = c + 8 + 16 * li;
		// the travel, wrapped where every speed factor ( 80 + k ) / 100 turns whole: the same positions, no lost precision
		o[ 0 ] = (float)fmod( BaseTravel * wind, 100.0 * GpuFW ); o[ 1 ] = (float)fmod( BaseFall * fall, 100.0 * GpuFH );
		o[ 2 ] = (float)( L.churnPx * SU_PER_GAMEPX * gg.churn ); o[ 3 ] = (float)( L.churnPx * SU_PER_GAMEPX * gg.churn / 2 );
		o[ 4 ] = (float)( wind * WindNow ); o[ 5 ] = (float)( fall * FallNow ); o[ 6 ] = GPU_EXPOSURE; o[ 7 ] = gg.minLen;
		for( int k = 0; k < 3; ++k ) o[ 8 + k ] = ( SHADE_RGB[ L.shade ][ k ] + ( SHADE_NIGHT_RGB[ L.shade ][ k ] - SHADE_RGB[ L.shade ][ k ] ) * night / 256.0f ) / 255.0f;
		o[ 11 ] = gg.alpha;
		o[ 12 ] = gg.halfWidth; o[ 13 ] = gg.soft; o[ 14 ] = gg.rate; o[ 15 ] = 0;
	}
	n = 2 + 4 * LAYER_COUNT;
	return true;
}

// (th4) The one place that decides whether it snows (see the header).
bool Snowfall_Active()
{
	if( !( Players[ CurrentPlayerIndex ].gameChanger & BIT( GC_15_ICE_AGE ) ) ) return false;
	return ( Dungeon == DUN_0_TOWN && !Dungeon->isQuest ) || Dungeon == DUN_44_RAVENHOLM;
}

// (th4) ICE RIVER (dx_utility.cpp): how stormy the weather is right now, 0 = a calm snowfall .. 1 = a storm. Read off the
// wind of THE SPELLS (WindNow, set once per frame by SpellsAdvance), so it eases between the two exactly like the snow.
float Snowfall_Storm()
{
	const float t = ( WindNow - CALM_WIND1 ) / ( STORM_WIND0 - CALM_WIND1 );
	return t <= 0.0f ? 0.0f : ( t >= 1.0f ? 1.0f : t );
}

// (th4) DRIFTING SNOW (dx_utility.cpp SnowDrift_Present): how far the wind has carried the snow so far, px at the mean wind
// (WIND_PX_PER_SEC) -- THE SPELLS' travel, so the drift speeds up and slows down with the storm and stops on pause.
double Snowfall_WindTravel()
{
	return BaseTravel * WIND_PX_PER_SEC;
}

// (th4) ICE AGE NIGHT: how much of night it is where it snows, 0 (day) .. 256 -- the town's own night phase
// (DayNight_NightQ8: in step with the night palette, palette.cpp AnimateTownPalette), always 256 in Ravenholm (it is
// always night there); 0 in Classic. Read by the flakes (PickShades) and the snow ground's night colours (dx_utility.cpp
// SNOW GROUND, ICE AGE NIGHT).
int Snowfall_NightQ8()
{
	if( !Snowfall_Active() || GameMode == GM_CLASSIC ) return 0;
	if( Dungeon == DUN_44_RAVENHOLM ) return 256;
	return DayNight_NightQ8();
}

// (th4) Draw.cpp, right after DrawIsoLand (post-zoom) and before Perspective_WarpWorld.
// zoomFactor: 2 when DrawIsoLand stretched the world, else 1 -- the same value TownWater_Render gets.
void Snowfall_Render( int zoomFactor )
{
	if( !Snowfall_Active() || !SoftSrcSurfacePtr ) return;
	if( Snowfall_GpuWanted() ){	// SNOWFALL ON THE GPU (above): the clock and THE SPELLS go on; the flakes are drawn there
		FrameMs = SnowClock();
		SpellsAdvance( FrameMs );
		return;
	}
	const int z = zoomFactor < 1 ? 1 : zoomFactor;
	if( !SinReady ) BuildSin();
	const int night = Snowfall_NightQ8();	// (th4) ICE AGE NIGHT: night shades, picked again as it changes
	if( !ShadeReady || memcmp( ShadePal, ScreenPalette, sizeof( ShadePal ) ) || abs( night - ShadeNight ) >= 8 ) PickShades( night );

	const int W = ScreenWidth, H = ScreenHeight, P = WorkingWidth;
	uchar* frame = SoftSrcSurfacePtr + P * Screen_TopBorder + Screen_LeftBorder;
	uchar* light = ( g_A8L8Lighting && PixelLight && LightSurface ) ? LightSurface + P * Screen_TopBorder + Screen_LeftBorder : 0;

	// the repeating field, 1x px: one screen plus a margin so flakes (and their tails) enter from off screen
	const int margin = 16;
	const int FW = W / z + 2 * margin, FH = H / z + 2 * margin;
	if( FW != FlakesFW || FH != FlakesFH ) BuildFlakes( FW, FH );
	const uint ms = SnowClock();
	SpellsAdvance( ms );	// THE SPELLS: this frame's wind and the travel so far

	auto put = [&]( int x1, int y1, int n, uchar c8 ){	// n x n 1x px at (x1, y1) as (n*z) x (n*z) screen px
		const int sx0 = x1 * z, sy0 = y1 * z, m = n * z;
		for( int dy = 0; dy < m; ++dy ){
			const int sy = sy0 + dy;
			if( sy < 0 || sy >= H ) continue;
			for( int dx = 0; dx < m; ++dx ){
				const int sx = sx0 + dx;
				if( sx < 0 || sx >= W ) continue;
				const int p = sy * P + sx;
				frame[ p ] = c8;
				if( light ) light[ p ] = (uchar)( light[ p ] * SNOW_LIGHT_LIFT / 100 );
			}
		}
	};

	for( int li = 0; li < LAYER_COUNT; ++li ){
		const Layer& L = kLayers[ li ];
		// this layer's travel so far (THE SPELLS) and its flight direction right now (the streaks)
		const double wind = (double)WIND_PX_PER_SEC * L.windPct / 100.0;
		const double travelX = BaseTravel * wind;					// THE SPELLS (SpellsAdvance)
		const double travelY = BaseFall * L.fallPxPerSec;
		const double vx = wind * WindNow, vy = L.fallPxPerSec * (double)FallNow;
		int tailX[ 8 ], tailY[ 8 ];
		const int tails = L.streak < 8 ? L.streak : 8;
		{
			// unit vector without sqrt: |v| ~ max + 3/8 min (within 7%), enough for a few pixels
			const double ax = vx < 0 ? -vx : vx, ay = vy < 0 ? -vy : vy;
			const double len = ( ax > ay ? ax + 0.375 * ay : ay + 0.375 * ax ) + 1e-6;
			for( int k = 0; k < tails; ++k ){
				const double d = (double)( k + 1 );
				tailX[ k ] = (int)( -vx / len * d + ( vx > 0 ? -0.5 : 0.5 ) );
				tailY[ k ] = (int)( -vy / len * d + ( vy > 0 ? -0.5 : 0.5 ) );
			}
		}
		const uchar head8 = ShadeIdx[ L.shade ];
		const uchar tail8a = ShadeIdx[ L.shade > 0 ? L.shade - 1 : 0 ], tail8b = ShadeIdx[ L.shade > 1 ? L.shade - 2 : 0 ];
		// per-frame tables (COST in the header): travel for each speed factor, already wrapped into the field ...
		int travelTabX[ SPEEDS ], travelTabY[ SPEEDS ];
		for( int k = 0; k < SPEEDS; ++k ){
			long long tx = (long long)( travelX * ( 80 + k ) / 100.0 ) % FW, ty = (long long)( travelY * ( 80 + k ) / 100.0 ) % FH;
			travelTabX[ k ] = (int)( tx < 0 ? tx + FW : tx ); travelTabY[ k ] = (int)( ty < 0 ? ty + FH : ty );
		}
		// ... and the wobble phase for each churn rate, plus the wobble itself per phase
		int ratePhase[ CHURN_RATES ], churnX[ 256 ], churnY[ 256 ];
		for( int k = 0; k < CHURN_RATES; ++k ) ratePhase[ k ] = (int)( (unsigned long long)ms * ( 300 + 40 * k ) / 1000 & 255 );
		for( int k = 0; k < 256; ++k ){ churnX[ k ] = SinTab[ k ] * L.churnPx / 1024; churnY[ k ] = SinTab[ k ] * L.churnPx / 2048; }
		const bool dot = tails == 0 && L.size == 1 && z == 1;	// the far dots: one pixel, written inline
		const std::vector<Flake>& fl = Flakes[ li ];
		if( dot && !light ){	// FAR DOTS ON SEVERAL THREADS (above): the same dots, the loop below split in parts
			const DotJob j = { fl.data(), (int)fl.size(), travelTabX, travelTabY, ratePhase, churnX, churnY, FW, FH, margin, W, H, P, frame, head8 };
			DrawDotsAll( j );
			continue;
		}
		const Flake* const flEnd = fl.data() + fl.size();	// a plain pointer walk, same order: ~120,000 flakes a frame
		for( const Flake* fp = fl.data(); fp < flEnd; ++fp ){	// (a checked iterator step per flake was most of this loop)
			const Flake& f = *fp;
			int x = f.x0 + travelTabX[ f.speed ] + churnX[ ( ratePhase[ f.rateX ] + f.phX ) & 255 ];
			int y = f.y0 + travelTabY[ f.speed ] + churnY[ ( ratePhase[ f.rateY ] + f.phY ) & 255 ];
			if( x >= FW ) x -= FW; if( x >= FW ) x -= FW; if( x < 0 ) x += FW;	// x0, travel in [0, FW), wobble a few px
			if( y >= FH ) y -= FH; if( y >= FH ) y -= FH; if( y < 0 ) y += FH;
			const int x1 = x - margin, y1 = y - margin;
			if( dot ){
				if( (uint)x1 < (uint)W && (uint)y1 < (uint)H ){
					const int p = y1 * P + x1;
					frame[ p ] = head8;
					if( light ) light[ p ] = (uchar)( light[ p ] * SNOW_LIGHT_LIFT / 100 );
				}
				continue;
			}
			for( int k = tails - 1; k >= 0; --k ) put( x1 + tailX[ k ], y1 + tailY[ k ], 1, k >= TAIL_FADE ? tail8b : tail8a );
			put( x1, y1, L.size, head8 );
		}
	}
}

// ============================================================================================
// (th4) SNOW DEPTH -- heroes, NPCs and cows in Ice Age Tristram stand in snow above the knee.
// ============================================================================================
// WHAT: the bottom SNOW_DEPTH pixels of a character are not shown -- the snowy ground stays there instead -- so the legs
// sink into it (Mordor, 2026-10-09: "bury the floor in snow, up above player knee level"). THE CUT is never a screen row
// (Mordor drew it: "that's how you hide legs, NOT in a straight horizontal line"): the snow's surface is the GROUND
// raised by the depth, and the ground runs along the isometric 2 : 1 diagonals, so the cut is a 2 : 1 slanted line through
// the legs -- across the body's width, i.e. perpendicular to its facing on the ground: facing down-left / up-right it
// falls to the right (+1/2), facing up-left / down-right it rises (-1/2); facing straight down / up / left / right it
// is level -- the width runs across the screen, or straight into it (SnowDepthSlope; Mordor: "facing down, the cutoff
// must be horizontal"). Depth is measured at the character's centre column; small fixed bumps keep it from looking
// ruled. A pure function of the column's offset from the centre: it moves with the character, no flicker. No white "collar" rows above the cut: they jittered with the feet and were removed.
// Items on the ground are never buried; nor is anyone whose feet stand on the frozen river's WATER or on ground the SNOW GROUND left bare (TownWater_FeetBare: ice, or no snow to stand in -- the river's
// snowy banks bury the legs like any snow).
// HOW, and why this way: TownMap.cpp DrawDynamicObjectsTown wraps each character's draw (its outline included) in
// SnowDepth_Begin / SnowDepth_End. Begin copies the strip of the 8-bit frame the legs may cover (and the Enriched Light
// buffer under it); End finds the feet -- the lowest row the draw changed -- and copies the strip back from the feet
// up to SNOW_DEPTH rows, so exactly what was there BEFORE this character comes back: the snowy ground, or another
// character or a fence behind it. Nothing is painted as a box or patch: an earlier snowbound attempt drew a wading
// patch per character after the whole scene, and an NPC's patch landed on the hero standing in front of it.
// WHEN: Snowfall_Active (Ice Age, town + Ravenholm). TWO SETS OF CALLS, one per way of drawing a map:
//   the TOWN draws heroes, towners and cows in TownMap.cpp DrawDynamicObjectsTown;
//   RAVENHOLM goes through the dungeon path: heroes at the DrawPlayer call in Draw.cpp DrawMapCell (NOT inside
//   DrawPlayer -- the Enriched Light replay below calls DrawPlayer a second time), monsters in Draw.cpp
//   DrawMonsterInTile around their outline + DrawMonster. Ravenholm's monsters sink like the heroes, except FLIERS
//   (monster.canFly: they hover above the snow). Corpses and dying heroes lie on top, like items.
// THE HP BARS: DrawPlayer / DrawMonster draw the overhead bar INSIDE the wrap when Enriched Light is off
// (QueueOverheadHpBar draws it at once then). The bar is above the head, so it would count as the top of the body and
// bury small monsters deeper: QueueOverheadHpBar calls SnowDepth_Measure first, which fixes feet and height from what
// the body drew so far. A NEW thing drawn inside the wrap above the body needs the same call before it.
// ENRICHED LIGHT (A8L8, EnrichedLightEnhanced / Ultra): after the whole scene, Draw.cpp DrawIsoLand REPLAYS monsters and
// heroes into the light buffer only (g_LightSurfaceOnlyPass); unchecked, that relit the buried legs' shape on the snow.
// So SnowDepth_End records each Ravenholm cut (SnowDepth_End( anim, frame ): key = Begin's x, y + animation + frame),
// and the replays are wrapped in SnowDepth_ReplayBegin / SnowDepth_ReplayEnd, which put the light under the cut back.
// The records live one frame (SnowDepth_FrameStart, where DrawIsoLand empties the replay lists). Standard lighting
// records nothing.
// NO HERO SHADOW: the heroes' sprites carry their shadow painted in (Diablo's CL2s: a solid palette-0 blob left of the
// feet, up to ~35 % of the body's height). In snow it was "deadweight" (Mordor): for HEROES only (shadow = true; never
// NPCs or cows, his call) every pixel the draw set to palette 0 in the lower SHADOW_SHARE % of the body goes back to what
// was there before -- the hero's own dark outline pixels that low are under the snow anyway.
// WHO NOT: Wirt (TownMap.cpp, TI_8_WIRT) never sinks -- Mordor's call; the peg-legged boy stands on top of the snow.
// Nor does Ogden (TI_3_OGDEN): he stands in his open doorway, which has snow since the tavern's snow line was redrawn
// into it (TownWater.cpp kTavernLine), and sunk he lost his legs on his own doorstep (Mordor, 2026-10-10).
// KNOWN-GOOD CHECK: Ice Age town by day: the hero, Griswold, Ogden, the cows -- all sunk in snow to above the knee,
// the snow line curved round the legs; walk past an NPC: neither is cut by the other's snow; step onto the
// frozen river: the legs come back; drop an item: it lies on top of the snow.
namespace {
const int SNOW_DEPTH = 22;		// pre-zoom px hidden: just above the knee (a standing hero is 63..67 px from feet to head,
								// the knee ~18 px up -- measured on the warrior / rogue / monk town stand CL2s)
const int SNOW_FIND  = 48;		// more rows above the frame's bottom searched for the feet
// HOW DEEP: the same SHARE of everyone's height -- a fixed SNOW_DEPTH swallowed half of Wirt, a child 48 px tall (PegKid1.CEL)
// (Mordor: "Wirt's lower body is not displayed"). The height = feet .. the topmost row the draw changed, searched in
// SNOW_TALL rows above the feet; depth = SNOW_DEPTH * height / SNOW_HERO_HEIGHT, never more than SNOW_DEPTH (heroes
// and grown towners keep 22, Wirt gets 16).
const int SNOW_TALL = 120, SNOW_HERO_HEIGHT = 65;
std::vector<uchar> DepthSnap, DepthLightSnap;
int DepthX0 = 0, DepthX1 = 0, DepthY0 = 0, DepthY1 = 0;
bool DepthOn = false;
intptr_t DepthLightOfs = 0;		// LightSurface - SoftSrcSurfacePtr, 0 = no light buffer (Engine.cpp's "constant offset")
int DepthCX = 0;				// the character's centre column (THE CUT)
int DepthSlope = 0;				// THE CUT: +1 = falls to the right by 1 px per 2, -1 = rises, 0 = level
bool DepthShadow = false;		// NO HERO SHADOW (above)
const int SHADOW_SHARE = 45;	// NO HERO SHADOW: how much of the body, from the feet up, may hold shadow pixels
int DepthBX = 0, DepthBY = 0;	// Begin's x, y: the ENRICHED LIGHT replay key
bool DepthMeasured = false;		// THE HP BARS (above): feet and height already fixed
int DepthFeet = -1, DepthTop = -1;
// ENRICHED LIGHT (above): this frame's cuts -- per column the first screen row under the snow
struct DepthCut { int x, y; const char* anim; int frame; int x0, bw, r0, r1; size_t at; };
std::vector<DepthCut> DepthCuts;
std::vector<short> DepthCutRows;
int ReplayAt = -1;				// the record of the replay in progress, -1 = none
std::vector<uchar> ReplaySnap;
}

// (th4) THE CUT's slant for a hero facing dir (DIRECTION, enums.h): across the body's width on the ground. Facing down-left
// or up-right (map axes) the width runs along the other map axis, down to the right on screen: +1; facing up-left or
// down-right: -1; facing straight down / up (width across the screen) or left / right (width into the screen): 0, level.
// Towners and cows: 0 (their facing is not used here).
int SnowDepthSlope( int dir )
{
	switch( dir ){
	case DIR_1_DL: case DIR_5_UR: return 1;
	case DIR_3_UL: case DIR_7_DR: return -1;
	default: return 0;
	}
}

// (th4) TownMap.cpp DrawDynamicObjectsTown, right before a character's draw: x = the frame's left column, y = its
// bottom row, w = its width (pre-zoom WorkingSurface coordinates, as CL2_Draw / CEL_Draw take them).
void SnowDepth_Begin( int x, int y, int w, bool onIce, int slope, bool shadow )
{
	DepthShadow = shadow;
	DepthSlope = slope > 0 ? 1 : slope < 0 ? -1 : 0;
	DepthOn = false;
	DepthMeasured = false;
	DepthBX = x; DepthBY = y;
	if( onIce || !Snowfall_Active() || !WorkingSurface ) return;
	DepthX0 = max( x, (int)Screen_LeftBorder ); DepthX1 = min( x + w, (int)Screen_LeftBorder + ScreenWidth );
	DepthY0 = max( y - SNOW_FIND - SNOW_TALL, (int)Screen_TopBorder ); DepthY1 = min( y + 1, (int)Screen_TopBorder + ScreenHeight );
	if( DepthX0 >= DepthX1 || DepthY0 >= DepthY1 ) return;
	DepthCX = x + w / 2;
	const int bw = DepthX1 - DepthX0, bh = DepthY1 - DepthY0;
	DepthLightOfs = ( g_A8L8Lighting && PixelLight && LightSurface && SoftSrcSurfacePtr ) ? (intptr_t)LightSurface - (intptr_t)SoftSrcSurfacePtr : 0;
	DepthSnap.resize( (size_t)bw * bh );
	if( DepthLightOfs ) DepthLightSnap.resize( (size_t)bw * bh );
	for( int ry = 0; ry < bh; ++ry ){
		const uchar* src = WorkingSurface + YOffsetHashTable[ DepthY0 + ry ] + DepthX0;
		memcpy( &DepthSnap[ (size_t)ry * bw ], src, bw );
		if( DepthLightOfs ) memcpy( &DepthLightSnap[ (size_t)ry * bw ], src + DepthLightOfs, bw );
	}
	DepthOn = true;
}

// (th4) THE HP BARS (SNOW DEPTH above): fix the feet and the height from what the draw changed so far. Draw.cpp
// QueueOverheadHpBar calls it before a bar is drawn inside the wrap; SnowDepth_End calls it if nobody did.
void SnowDepth_Measure()
{
	if( !DepthOn || DepthMeasured ) return;
	DepthMeasured = true;
	const int bw = DepthX1 - DepthX0, bh = DepthY1 - DepthY0;
	auto row = [&]( int ry ){ return WorkingSurface + YOffsetHashTable[ DepthY0 + ry ] + DepthX0; };
	int feet = -1;		// the lowest row the character changed
	for( int ry = bh - 1; ry >= 0 && feet < 0; --ry ) if( memcmp( row( ry ), &DepthSnap[ (size_t)ry * bw ], bw ) ) feet = ry;
	int top = feet;			// HOW DEEP (above): the topmost row the character changed
	if( feet >= 0 ) for( int ry = max( 0, feet - SNOW_TALL ); ry < feet; ++ry ) if( memcmp( row( ry ), &DepthSnap[ (size_t)ry * bw ], bw ) ){ top = ry; break; }
	DepthFeet = feet; DepthTop = top;
}

// (th4) right after it (see SNOW DEPTH above). anim / frame: given by the Ravenholm calls (Draw.cpp) -- with Enriched
// Light the cut is recorded for the replay (ENRICHED LIGHT above); the town passes nothing.
void SnowDepth_End( const char* anim, int frame )
{
	if( !DepthOn ) return;
	SnowDepth_Measure();
	DepthOn = false;
	const int bw = DepthX1 - DepthX0, bh = DepthY1 - DepthY0;
	auto row = [&]( int ry ){ return WorkingSurface + YOffsetHashTable[ DepthY0 + ry ] + DepthX0; };
	const int feet = DepthFeet, top = DepthTop;
	if( feet < 0 ) return;
	const bool keep = anim && DepthLightOfs;	// ENRICHED LIGHT (above)
	DepthCut rec = { DepthBX, DepthBY, anim, frame, DepthX0, bw, DepthY1, DepthY1, DepthCutRows.size() };
	const int depth = min( SNOW_DEPTH, ( SNOW_DEPTH * ( feet - top + 1 ) + SNOW_HERO_HEIGHT / 2 ) / SNOW_HERO_HEIGHT );
	if( DepthShadow ){						// NO HERO SHADOW (above)
		for( int ry = max( 0, feet - ( feet - top + 1 ) * SHADOW_SHARE / 100 ); ry <= feet; ++ry ){
			uchar* dst = row( ry );
			const uchar* was = &DepthSnap[ (size_t)ry * bw ];
			for( int xx = 0; xx < bw; ++xx ){
				if( dst[ xx ] != 0 || was[ xx ] == 0 ) continue;
				dst[ xx ] = was[ xx ];
				if( DepthLightOfs ) dst[ xx + DepthLightOfs ] = DepthLightSnap[ (size_t)ry * bw + xx ];
			}
		}
	}
	for( int xx = 0; xx < bw; ++xx ){		// THE CUT (above): this column's snow line, then everything below it back from the snapshot
		const float t = (float)( DepthX0 + xx - DepthCX );
		const float line = feet + DepthSlope * 0.5f * t - depth + 0.6f * sinf( 0.9f * t ) + 0.4f * sinf( 2.3f * t + 1.0f );
		const int cut = (int)floorf( line + 0.5f ) + 1;	// rows cut .. the bottom of this column are under the snow
		if( keep ){ const int r = DepthY0 + max( cut, 0 ); DepthCutRows.push_back( (short)r ); rec.r0 = min( rec.r0, r ); }
		for( int ry = max( cut, 0 ); ry < bh; ++ry ){
			const size_t k = (size_t)ry * bw + xx;
			uchar* dst = row( ry ) + xx;
			*dst = DepthSnap[ k ];
			if( DepthLightOfs ) dst[ DepthLightOfs ] = DepthLightSnap[ k ];
		}
	}
	if( keep && rec.r0 < rec.r1 ) DepthCuts.push_back( rec );
}

// (th4) ENRICHED LIGHT (SNOW DEPTH above): Draw.cpp DrawIsoLand, where the replay lists are emptied each frame.
void SnowDepth_FrameStart()
{
	DepthCuts.clear(); DepthCutRows.clear(); ReplayAt = -1;
}

// (th4) ENRICHED LIGHT: around one sprite's light-only replay in Draw.cpp DrawIsoLand -- x, y, anim, frame exactly as
// the replay draws it (= what the main pass gave SnowDepth_Begin / SnowDepth_End). No record (not buried, not
// Ravenholm, standard lighting): nothing happens.
void SnowDepth_ReplayBegin( int x, int y, const char* anim, int frame )
{
	ReplayAt = -1;
	if( DepthCuts.empty() || !anim || !WorkingSurface || !( g_A8L8Lighting && PixelLight && LightSurface && SoftSrcSurfacePtr ) ) return;
	for( size_t i = 0; i < DepthCuts.size(); ++i ){
		const DepthCut& c = DepthCuts[ i ];
		if( c.x == x && c.y == y && c.anim == anim && c.frame == frame ){ ReplayAt = (int)i; break; }
	}
	if( ReplayAt < 0 ) return;
	const DepthCut& c = DepthCuts[ ReplayAt ];
	const intptr_t lo = (intptr_t)LightSurface - (intptr_t)SoftSrcSurfacePtr;
	ReplaySnap.resize( (size_t)c.bw * ( c.r1 - c.r0 ) );
	for( int r = c.r0; r < c.r1; ++r ) memcpy( &ReplaySnap[ (size_t)( r - c.r0 ) * c.bw ], WorkingSurface + YOffsetHashTable[ r ] + c.x0 + lo, c.bw );
}

void SnowDepth_ReplayEnd()
{
	if( ReplayAt < 0 ) return;
	const DepthCut& c = DepthCuts[ ReplayAt ];
	ReplayAt = -1;
	const intptr_t lo = (intptr_t)LightSurface - (intptr_t)SoftSrcSurfacePtr;
	for( int xx = 0; xx < c.bw; ++xx )
		for( int r = DepthCutRows[ c.at + xx ]; r < c.r1; ++r )
			WorkingSurface[ YOffsetHashTable[ r ] + c.x0 + xx + lo ] = ReplaySnap[ (size_t)( r - c.r0 ) * c.bw + xx ];
}
