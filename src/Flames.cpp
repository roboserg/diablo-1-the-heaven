#include "stdafx.h"

// ============================================================================================
// (new by th4) FLAMES -- sparks rising from torches, braziers and burning crosses, and a soft
// warm halo around every flame of a light-emitting object.
// ============================================================================================
//
// WHICH FLAMES: every object drawn with a sprite listed in FlameSprites[] below -- one row per
// object SPRITE FILE (the names of dataseg.cpp FloorObjectsNames*, found through
// BaseObjects[].SpriteIndex the way Object.cpp LoadOneObjectSprite does), so the braziers
// BO_0/8/10/15..19, the skull fire BO_3, the candles BO_9/65/87, the burning crosses BO_26/91 and
// the wall torches BO_44..47 are all covered by their art. Never in Classic (Flames_NoteObject).
// Crypt candles use L5Light (a lamp with no visible flame) and the flame trap uses Flame1: neither
// has a row, so neither glows. A new flame sprite needs a row (measure it -- see the numbers'
// description at the table).
// Sibling systems: DustMotes.cpp (specks in the light of the same objects), Embers.cpp (sparks
// from cave lava -- the spark code here is the same idea).
//
// TWO HALVES:
//   SPARKS -- single palette pixels written into the 8-bit frame on the CPU right after DrawIsoLand
//     (Draw.cpp, next to Embers_Render): post-zoom, before Perspective_WarpWorld and the post-effect
//     snapshot, under the UI. Every lighting and draw mode shows them; under Enriched Light they are
//     drawn at full brightness (LightSurface 0): they glow. Stateless like the embers: each sprite
//     row has a few spark slots, each slot fires once per hashed period, the spark rises from the
//     top of the flame, sways, cools from yellow to dark red and flickers out. Candles get none.
//   HALO -- a soft round glow, true colour: dx_utility.cpp FlameGlow_Present + FlameGlow.hlsl (the
//     ATMOSPHERE block), one small square per halo, screen blend, world pixels only (world/UI
//     mask). This file only hands over the list (Flames_Halos). No halo in "drawmode surface"
//     (no shaders there); the sparks still show.
//
// VISIBLE OR NOT: the object is drawn even where a wall in front hides it (a wall torch on the far
// side of a wall), and a monster can stand in front of it. So after the whole world is drawn, the
// flame box of each noted object is read back from the 8-bit frame and the pixels of FLAME colour
// are counted (warm: red >= 150, red >= green >= blue, red - blue >= 70; the candle's flame is
// white wax and a pale tip, so its rows count bright near-white pixels instead -- both tested on
// FilePalette, the level palette as loaded, whose flame entries 145..155 / 208..214 sit in the upper
// half no palette cycle touches). Visibility = count / (VIS_PART x the sprite's median count),
// smoothed over VIS_MS; the halo is scaled by it and sparks start only above SPARK_VIS. A flame
// darkened by the classic light tables also stops counting -- right, since then it does not glow.
//
// PER FRAME:
//   Draw.cpp DrawIsoLand start ........ Flames_BeginFrame()   forget last frame's notes
//   Draw.cpp DrawObjectSprite ......... Flames_NoteObject()   the object and where it was drawn
//   Draw.cpp after DrawIsoLand ........ Flames_Render()       count, sparks, build the halo list
//   dx_utility.cpp PostFxPresent ...... Flames_Halos()        FlameGlow_Present draws the halos
//
// KNOWN-GOOD CHECK: catacombs (level 5..8), a wall torch on screen: a soft orange glow around the
// flame that breathes a little, a spark leaving its top every second or so and fading out on the
// way up; walk so the torch goes behind a wall and the glow goes with it. Church: braziers and
// candle stands glow (candles: small pale glow, no sparks). Pause: sparks and flicker freeze.
// ============================================================================================

namespace {

enum { TEST_WARM = 0, TEST_WHITE = 1 };

// One row per flame SPRITE. All positions in sprite pixels: x from the left edge of the frame,
// y in rows ABOVE its bottom row (CEL rows are drawn bottom-up from the y the game passes, so
// sprite row r lands on screen line y - r). Measured from the extracted CEL files with the level
// palette (church l1.pal / catacomb l2.pal), all frames:
//   cx, cy          halo centre: the centroid of the flame pixels (candle: its flame tip)
//   x0..x1, y0..y1  the flame box counted for visibility (2nd..98th percentile of flame pixels)
//   median          flame pixels in that box on a typical frame (the median over the frames)
//   test            which pixels count (TEST_WARM / TEST_WHITE, see the header)
//   halo            halo radius, 1x px
//   strength        halo brightness at its centre, 0..1 (screen blend)
//   r, g, b         halo colour (the LightTint of the same objects, Object.cpp GetObjectLightTint,
//                   pushed a little deeper)
//   slots           spark slots (0 = no sparks); each fires about every 0.9..2.6 s
//   sx, sy, sw      spark start: centre x, height and half-width on the flame's upper part
struct FlameSprite {
	const char* sprite;
	int cx, cy, x0, x1, y0, y1, median, test, halo;
	float strength, r, g, b;
	int slots, sx, sy, sw;
};
const FlameSprite FlameSprites[] = {
	//  sprite       cx   cy   x0   x1   y0   y1  median test         halo  str    r     g     b    slots sx   sy  sw
	{ "L1Braz",      33,  81,  26,  40,  73,  93,  103, TEST_WARM,   34, 0.30f, 1.0f, 0.56f, 0.20f, 3,  33,  87,  4 },
	{ "SkulFire",    47,  23,  38,  56,  15,  28,  138, TEST_WARM,   30, 0.28f, 1.0f, 0.56f, 0.20f, 2,  47,  27,  3 },
	{ "Burncros",    97,  53,  66, 126,  30,  96, 1197, TEST_WARM,   72, 0.32f, 1.0f, 0.42f, 0.14f, 6,  96,  78, 18 },
	{ "WTorch1",     58,  82,  53,  62,  70,  95,  171, TEST_WARM,   28, 0.28f, 1.0f, 0.56f, 0.20f, 2,  58,  90,  2 },
	{ "WTorch2",     37,  83,  32,  41,  71,  95,  144, TEST_WARM,   28, 0.28f, 1.0f, 0.56f, 0.20f, 2,  37,  90,  2 },
	{ "WTorch3",     45,  85,  40,  50,  74,  98,  175, TEST_WARM,   28, 0.28f, 1.0f, 0.56f, 0.20f, 2,  45,  93,  2 },
	{ "WTorch4",     37,  84,  32,  42,  72,  98,  196, TEST_WARM,   28, 0.28f, 1.0f, 0.56f, 0.20f, 2,  37,  92,  2 },
	{ "Candle2",     47,  65,  43,  50,  58,  65,   26, TEST_WHITE,  13, 0.22f, 1.0f, 0.80f, 0.50f, 0,   0,   0,  0 },
};
const int SPRITE_COUNT = sizeof( FlameSprites ) / sizeof( FlameSprites[ 0 ] );

const float VIS_PART  = 0.4f;	// this share of the median count = fully visible
const int   VIS_MS    = 80;		// visibility smoothing, ms (the sprites' own flicker stays readable)
const float SPARK_VIS = 0.6f;	// sparks only from a flame at least this visible
const int PERIOD_MIN_MS = 900, PERIOD_MAX_MS = 2600;	// a spark slot fires once per period
const int LIFE_MIN_MS = 450, LIFE_MAX_MS = 1150;		// how long a spark lives (capped by its period)
const int RISE_MIN = 22, RISE_MAX = 48;					// px per second, 1x
const int DRIFT_MAX = 8;								// sideways drift, +- px per second
const int SWAY_PX = 2;
const int SHADE_RGB[ 3 ][ 3 ] = { { 255, 235, 130 }, { 255, 150, 40 }, { 185, 50, 20 } };	// hot, warm, cooling (Embers.cpp)

struct Note { int object, sprite, x, y; };		// x, y = pre-zoom WorkingSurface bottom-left of the sprite
Note Notes[ MaxObjects_127 ];
int NoteN = 0;
bool Noted[ MaxObjects_127 ];
struct Seen { float vis; long long ms; };
Seen SeenState[ MaxObjects_127 ];				// per object index; stale after 250 ms off screen (a new level too)

const int MAX_HALOS = 32;						// FlameGlow.hlsl's constant arrays
float HaloPos[ MAX_HALOS ][ 4 ], HaloCol[ MAX_HALOS ][ 4 ];
int HaloN = 0;

uchar Cls[ 256 ];								// per palette entry: bit 0 warm, bit 1 white
PALETTEENTRY ClsPal[ 256 ];
bool ClsReady = false;
uchar Shade[ 3 ];
PALETTEENTRY ShadePal[ 256 ];
bool ShadeReady = false;
int SinTab[ 256 ];
bool SinReady = false;

struct Spark { int x, y; uchar col; };
std::vector<Spark> Pending;

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

void BuildCls()
{
	for( int i = 0; i < 256; ++i ){
		const int r = FilePalette[ i ].peRed, g = FilePalette[ i ].peGreen, b = FilePalette[ i ].peBlue;
		uchar c = 0;
		if( r >= 150 && r >= g && g >= b && r - b >= 70 ) c |= 1 << TEST_WARM;
		if( r >= 170 && g >= 170 && b >= 170 && r >= b ) c |= 1 << TEST_WHITE;
		Cls[ i ] = c;
	}
	memcpy( ClsPal, FilePalette, sizeof( ClsPal ) );
	ClsReady = true;
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

// Flame clock in ms, frozen while paused (GetTickCountTh counts 1/MAX_FPS s, enums.h).
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

// The object's sprite file name, as Object.cpp LoadOneObjectSprite picks it.
const char* SpriteName( const Object& o )
{
	if( (uint)o.BaseObjectIndex >= countof( BaseObjects ) ) return 0;
	const int si = BaseObjects[ o.BaseObjectIndex ].SpriteIndex;
	if( si < 0 || si >= 57 ) return 0;
	switch( Dungeon->graphType ){
	case DT_5_CRYPT: return FloorObjectsNamesCrypt[ si ];
	case DT_6_ABYSS: return FloorObjectsNamesAbyss[ si ];
	default:         return FloorObjectsNamesOriginal[ si ];
	}
}

} // namespace

// (th4) Draw.cpp DrawIsoLand start: forget last frame's notes.
void Flames_BeginFrame()
{
	for( int i = 0; i < NoteN; ++i ) Noted[ Notes[ i ].object ] = false;
	NoteN = 0;
}

// (th4) Draw.cpp DrawObjectSprite, after the object was drawn: x, y = where (pre-zoom WorkingSurface
// px, the bottom-left corner of the sprite, the point CEL_Draw / DrawItemOnMap get). An object can be
// drawn more than once a frame; the first note counts.
void Flames_NoteObject( int objectIndex, int x, int y )
{
	if( GameMode == GM_CLASSIC ) return;	// never in Classic: nothing noted = no sparks, no halos (Mordor's call)
	if( (uint)objectIndex >= MaxObjects_127 || Noted[ objectIndex ] || NoteN >= MaxObjects_127 ) return;
	const char* name = SpriteName( Objects[ objectIndex ] );
	if( !name ) return;
	for( int s = 0; s < SPRITE_COUNT; ++s ){
		if( _stricmp( name, FlameSprites[ s ].sprite ) ) continue;
		Noted[ objectIndex ] = true;
		Notes[ NoteN++ ] = { objectIndex, s, x, y };
		return;
	}
}

// (th4) Draw.cpp, right after DrawIsoLand (post-zoom), next to Embers_Render. zoomFactor: 2 when
// DrawIsoLand stretched the world, else 1.
void Flames_Render( int zoomFactor )
{
	const long long ms = Clock();	// runs every frame so the clock never jumps
	HaloN = 0;
	if( !NoteN || !SoftSrcSurfacePtr ) return;
	const int z = zoomFactor < 1 ? 1 : zoomFactor;
	if( !SinReady ) BuildSin();
	if( !ClsReady || memcmp( ClsPal, FilePalette, sizeof( ClsPal ) ) ) BuildCls();
	if( !ShadeReady || memcmp( ShadePal, ScreenPalette, sizeof( ShadePal ) ) ) PickShades();
	const int W = ScreenWidth, H = ScreenHeight, P = WorkingWidth;
	uchar* frame = SoftSrcSurfacePtr + P * Screen_TopBorder + Screen_LeftBorder;
	uchar* light = ( g_A8L8Lighting && PixelLight && LightSurface ) ? LightSurface + P * Screen_TopBorder + Screen_LeftBorder : 0;

	Pending.clear();
	for( int i = 0; i < NoteN; ++i ){
		const Note& n = Notes[ i ];
		const FlameSprite& S = FlameSprites[ n.sprite ];
		const Object& o = Objects[ n.object ];
		// bottom-left of the sprite on screen (post-zoom game px, relative to the screen area)
		const int bx = ( n.x - Screen_LeftBorder ) * z, by = ( n.y - Screen_TopBorder ) * z;
		// 1) how much of the flame is on screen: flame pixels in its box, one test per game px
		int count = 0;
		const uchar want = (uchar)( 1 << S.test );
		for( int fy = S.y0; fy <= S.y1; ++fy ){
			const int sy = by - fy * z;
			if( sy < 0 || sy >= H ) continue;
			const uchar* row = frame + sy * P;
			for( int fx = S.x0; fx <= S.x1; ++fx ){
				const int sx = bx + fx * z;
				if( sx >= 0 && sx < W && ( Cls[ row[ sx ] ] & want ) ) ++count;
			}
		}
		float target = count / ( VIS_PART * S.median );
		if( target > 1.0f ) target = 1.0f;
		Seen& s = SeenState[ n.object ];
		const long long dt = ms - s.ms;
		if( s.ms == 0 || dt < 0 || dt > 250 ) s.vis = target;		// first seen (or back on screen): no fade-in
		else if( dt > 0 ) s.vis += ( target - s.vis ) * (float)dt / (float)( dt + VIS_MS );
		s.ms = ms ? ms : 1;
		const uint seed = Hash( (uint)( o.Row * 112 + o.Col ) * 2654435761U + 17 );
		// 2) the halo: centre, radius, brightness (the sprite's own flicker through the count, plus two
		// slow waves so a still flame still breathes)
		if( s.vis > 0.05f && HaloN < MAX_HALOS ){
			int hx = bx + S.cx * z, hy = by - S.cy * z;
			Perspective_WorldToScreen( hx, hy );	// follow the Perspective warp, if on
			float anim = (float)count / S.median;
			if( anim > 1.5f ) anim = 1.5f;
			const int p1 = (int)( ( ms * 590 / 1000 + ( seed & 255 ) ) & 255 );		// ~2.3 Hz
			const int p2 = (int)( ( ms * 1510 / 1000 + ( ( seed >> 8 ) & 255 ) ) & 255 );	// ~5.9 Hz
			const float flick = 0.90f + 0.06f * SinTab[ p1 ] / 1024.0f + 0.04f * SinTab[ p2 ] / 1024.0f;
			const float k = S.strength * s.vis * ( 0.8f + 0.2f * anim ) * flick;
			HaloPos[ HaloN ][ 0 ] = (float)hx; HaloPos[ HaloN ][ 1 ] = (float)hy;
			HaloPos[ HaloN ][ 2 ] = (float)( S.halo * z ); HaloPos[ HaloN ][ 3 ] = k;
			HaloCol[ HaloN ][ 0 ] = S.r; HaloCol[ HaloN ][ 1 ] = S.g; HaloCol[ HaloN ][ 2 ] = S.b; HaloCol[ HaloN ][ 3 ] = 0.0f;
			++HaloN;
		}
		// 3) sparks (collected first: drawing them now could add flame pixels to the next box)
		if( s.vis < SPARK_VIS ) continue;
		for( int k = 0; k < S.slots; ++k ){
			const uint hs = Hash( seed ^ (uint)k * 83492791U );
			const int period = PERIOD_MIN_MS + (int)( hs % (uint)( PERIOD_MAX_MS - PERIOD_MIN_MS + 1 ) );
			const long long local = ms + ( hs >> 8 ) % period;
			const uint cycle = (uint)( local / period );
			const int age = (int)( local % period );
			const uint he = Hash( hs ^ ( cycle * 2654435761U ) ), he2 = Hash( he ), he3 = Hash( he2 );
			int life = LIFE_MIN_MS + (int)( he % (uint)( LIFE_MAX_MS - LIFE_MIN_MS + 1 ) );
			if( life > period ) life = period;
			if( age >= life ) continue;
			const int rise = RISE_MIN + (int)( he3 % (uint)( RISE_MAX - RISE_MIN + 1 ) );
			const int drift = (int)( ( he3 >> 8 ) % (uint)( 2 * DRIFT_MAX + 1 ) ) - DRIFT_MAX;
			const int ph = (int)( ( (long long)age * 200 / 1000 + ( he3 >> 16 ) ) & 255 );	// ~0.8 Hz sway
			const int x1 = S.sx + (int)( he2 % (uint)( 2 * S.sw + 1 ) ) - S.sw + age * drift / 1000 + SinTab[ ph ] * SWAY_PX / 1024;
			const int y1 = S.sy + age * rise / 1000;		// rows above the sprite's bottom: up
			const int f = age * 100 / life;
			if( f > 75 && ( Hash( he3 ^ (uint)( age / 50 ) ) & 1 ) ) continue;	// flickering out
			Pending.push_back( { bx + x1 * z, by - y1 * z, Shade[ f < 30 ? 0 : ( f < 65 ? 1 : 2 ) ] } );
		}
	}
	for( const Spark& e : Pending ){
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

// (th4) dx_utility.cpp FlameGlow_Present / Atmos_NeedMask: the halos of the last drawn frame.
// pos: xy centre (screen game px, after the zoom and the Perspective warp), z radius (game px),
// w brightness; col: rgb. Returns how many (at most maxHalos).
int Flames_Halos( float* pos, float* col, int maxHalos )
{
	if( !gbRunGame ) return 0;
	const int n = HaloN < maxHalos ? HaloN : maxHalos;
	if( pos ) memcpy( pos, HaloPos, n * 4 * sizeof( float ) );
	if( col ) memcpy( col, HaloCol, n * 4 * sizeof( float ) );
	return n;
}
