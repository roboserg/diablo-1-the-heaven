#include "stdafx.h"

// ============================================================================================
// (new by th4) DUST MOTES -- faint specks floating in the light of torches, candles and braziers
// in the church and the catacombs.
// ============================================================================================
//
// WHERE: levels drawn with the church or catacomb tileset (Dungeon->graphType DT_1_CHURCH /
// DT_2_CATACOMB, quest maps on those tilesets included; never in Classic), around every LIGHT-EMITTING OBJECT: the
// object types Object.cpp gives a light with PutSomeLightSorce (~1587-1645) -- wall torches
// BO_44..47 (radius 8), braziers BO_0/8, torch BO_3, candles BO_9/65, burning crosses BO_26/91
// (5), floor torch BO_87 (3); kEmitters below mirrors that switch. These lights are NOT in
// LightList: they are baked into the light map once at level creation (PutSomeLightSorce ->
// DoLighting(..., -1)), so the object list is the only place to find them. A new light-emitting
// object type must be added to kEmitters too. DustMotes_Active() decides the levels;
// DustMotes_BeginFrame() picks the emitters.
//
// WHICH SYSTEM THIS IS. Like Snowfall.cpp and TownWater.cpp: single palette pixels written into
// the 8-bit frame on the CPU, right after DrawIsoLand (Draw.cpp) -- post-zoom, before
// Perspective_WarpWorld and the post-effect snapshot, under the UI. Every lighting mode works;
// under Enriched Light a mote is lit a little brighter than the air around it (MOTE_LIGHT_LIFT),
// as dust catches the light.
//
// PER FRAME:
//   Draw.cpp DrawIsoLand start ........ DustMotes_BeginFrame()  mark the object-light cells
//   Draw.cpp DrawSceneDungeonTop ...... DustMotes_NoteCell()    screen position of each marked
//                                                                cell as it is drawn (exact, so no
//                                                                map-to-screen formula is needed)
//   Draw.cpp after DrawIsoLand ........ DustMotes_Render()      draw the motes (x zoom)
//
// HOW: stateless, like the snow -- each mote's position and brightness is a function of its light
// cell, its number and a pause-aware clock. A light gets MOTES_BASE + radius motes spread over an
// ellipse (CLOUD_RX x CLOUD_RY, 1x px) centred CLOUD_LIFT px above its floor tile (torches hang on
// walls, flames stand on candles). Each mote wanders a few pixels on two slow sine paths and
// fades in and out over a few seconds (dim -> bright -> dim -> gone), so the cloud shimmers
// instead of sitting still. Colours: the nearest entries to SHADE_* in the upper palette half
// (128..254), which dungeon palettes share and no palette cycle touches.
//
// KNOWN-GOOD CHECK: catacombs level 5, a wall torch on screen: a dozen or so warm specks hang
// around the flame and the wall below it, drifting slowly and twinkling on and off; none around
// the hero; none in the caves, hell or town. Pause: they freeze.
// ============================================================================================

namespace {

const int MOTES_BASE      = 6;		// motes per light, plus the light's radius
const int CLOUD_RX        = 44;		// cloud half-width, 1x px
const int CLOUD_RY        = 34;		// cloud half-height, 1x px
const int CLOUD_LIFT      = 36;		// cloud centre above the floor tile's centre, 1x px
const int WANDER_X        = 7;		// sway, 1x px
const int WANDER_Y        = 5;
const int MOTE_LIGHT_LIFT = 55;		// Enriched Light: mote light = air light x this % (lower = brighter)
const int SHADE_RGB[ 2 ][ 3 ] = { { 190, 150, 100 }, { 255, 228, 175 } };	// dim, bright

// Light-emitting object types and their light radius -- the PutSomeLightSorce cases in Object.cpp
// (the object creation switch, ~1587-1645). Keep the two in step.
struct Emitter { int type, radius; };
const Emitter kEmitters[] = {
	{ BO_44_WTORCH4, 8 }, { BO_45_WTORCH3, 8 }, { BO_46_WTORCH1, 8 }, { BO_47_WTORCH2, 8 },
	{ BO_0_L1BRAZ, 5 }, { BO_3_TORCH, 5 }, { BO_8_L1BRAZ, 5 }, { BO_9_CANDLE2, 5 }, { BO_65_CANDLE2, 5 },
	{ BO_26_BURNCROS, 5 }, { BO_91_BURNCROS, 5 },
	{ BO_87_ON_FLOOR_TORCH, 3 },
};

const int MAX_CELLS = 256;
struct LitCell { int x, y, radius, seed; };		// pre-zoom screen position of the cell's diamond (dst)
LitCell Cells[ MAX_CELLS ];
int CellCount = 0;
uchar Mark[ FineMap_112 ][ FineMap_112 ];		// light radius on object-light cells this frame, else 0
struct Marked { uchar r, c; };
Marked MarkedList[ MaxObjects_127 ];
int MarkedCount = 0;

uchar Shade[ 2 ];
PALETTEENTRY ShadePal[ 256 ];
bool ShadeReady = false;
int SinTab[ 256 ];								// sin * 1024, 256 steps per turn; no <math.h> here
bool SinReady = false;

uint Hash( uint x )
{
	x ^= x >> 16; x *= 0x7feb352dU; x ^= x >> 15; x *= 0x846ca68bU; x ^= x >> 16;
	return x;
}

void BuildSin()
{
	// Bhaskara I approximation, error < 0.2% -- same table as Snowfall.cpp
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
	for( int s = 0; s < 2; ++s ){
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

// Mote clock in ms, frozen while paused (GetTickCountTh counts 1/MAX_FPS s, enums.h).
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

void Plot( uchar* frame, uchar* light, int W, int H, int P, int z, int x1, int y1, uchar col )
{
	const int sx0 = x1 * z, sy0 = y1 * z;
	for( int dy = 0; dy < z; ++dy ){
		const int sy = sy0 + dy;
		if( sy < 0 || sy >= H ) continue;
		for( int dx = 0; dx < z; ++dx ){
			const int sx = sx0 + dx;
			if( sx < 0 || sx >= W ) continue;
			const int p = sy * P + sx;
			frame[ p ] = col;
			if( light ) light[ p ] = (uchar)( light[ p ] * MOTE_LIGHT_LIFT / 100 );
		}
	}
}

} // namespace

// (th4) The one place that decides the levels (see the header).
bool DustMotes_Active()
{
	if( Dungeon == DUN_0_TOWN || GameMode == GM_CLASSIC ) return false;	// never in Classic (Mordor's call)
	return Dungeon->graphType == DT_1_CHURCH || Dungeon->graphType == DT_2_CATACOMB;
}

// (th4) Draw.cpp DrawIsoLand start: forget last frame's cells and mark the emitter cells.
void DustMotes_BeginFrame()
{
	for( int i = 0; i < MarkedCount; ++i ) Mark[ MarkedList[ i ].r ][ MarkedList[ i ].c ] = 0;
	MarkedCount = 0;
	CellCount = 0;
	if( !DustMotes_Active() ) return;
	for( int i = 0; i < ObjectsCount && MarkedCount < MaxObjects_127; ++i ){
		const Object& o = Objects[ ObjectActive[ i ] ];
		int radius = 0;
		for( const Emitter& e : kEmitters ) if( o.BaseObjectIndex == e.type ){ radius = e.radius; break; }
		if( !radius ) continue;
		if( (uint)o.Row >= FineMap_112 || (uint)o.Col >= FineMap_112 || Mark[ o.Row ][ o.Col ] ) continue;
		Mark[ o.Row ][ o.Col ] = (uchar)radius;
		MarkedList[ MarkedCount++ ] = { (uchar)o.Row, (uchar)o.Col };
	}
}

// (th4) Draw.cpp DrawSceneDungeonTop, once per drawn cell: x, y = the cell's diamond dst
// (pre-zoom WorkingSurface coordinates, the same point DrawTileFill uses).
void DustMotes_NoteCell( int row, int col, int x, int y )
{
	if( !MarkedCount || (uint)row >= FineMap_112 || (uint)col >= FineMap_112 ) return;
	const int r = Mark[ row ][ col ];
	if( !r || CellCount >= MAX_CELLS ) return;
	Cells[ CellCount++ ] = { x, y, r, row * FineMap_112 + col };
}

// (th4) Draw.cpp, right after DrawIsoLand (post-zoom), next to Snowfall_Render / Weather_Render.
void DustMotes_Render( int zoomFactor )
{
	const long long ms = Clock();	// runs every frame so the clock never jumps
	if( !CellCount || !SoftSrcSurfacePtr ) return;
	const int z = zoomFactor < 1 ? 1 : zoomFactor;
	if( !SinReady ) BuildSin();
	if( !ShadeReady || memcmp( ShadePal, ScreenPalette, sizeof( ShadePal ) ) ) PickShades();
	const int W = ScreenWidth, H = ScreenHeight, P = WorkingWidth;
	uchar* frame = SoftSrcSurfacePtr + P * Screen_TopBorder + Screen_LeftBorder;
	uchar* light = ( g_A8L8Lighting && PixelLight && LightSurface ) ? LightSurface + P * Screen_TopBorder + Screen_LeftBorder : 0;

	for( int c = 0; c < CellCount; ++c ){
		const LitCell& L = Cells[ c ];
		// cloud centre, 1x px relative to the screen area: floor-tile centre, lifted
		const int cx = L.x + 32 - Screen_LeftBorder;
		const int cy = L.y - 15 - CLOUD_LIFT - Screen_TopBorder;
		const int n = MOTES_BASE + L.radius;
		for( int i = 0; i < n; ++i ){
			const uint h1 = Hash( (uint)( L.seed * 131 + i * 7 + 1 ) ), h2 = Hash( h1 ), h3 = Hash( h2 ), h4 = Hash( h3 );
			// base spot, uniform-ish in the ellipse (square with the corners folded in)
			int bx = (int)( h1 % ( 2 * CLOUD_RX + 1 ) ) - CLOUD_RX;
			int by = (int)( h2 % ( 2 * CLOUD_RY + 1 ) ) - CLOUD_RY;
			if( bx * bx * CLOUD_RY * CLOUD_RY + by * by * CLOUD_RX * CLOUD_RX > CLOUD_RX * CLOUD_RX * CLOUD_RY * CLOUD_RY ){ bx /= 2; by /= 2; }
			// wander: two slow sines (periods about 6..14 s)
			const int fx = 18 + (int)( h3 % 25 ), fy = 18 + (int)( ( h3 >> 8 ) % 25 );	// steps of 256 per 1000 s
			const int px = (int)( ( ms * fx / 1000 + ( h4 & 255 ) ) & 255 );
			const int py = (int)( ( ms * fy / 1000 + ( ( h4 >> 8 ) & 255 ) ) & 255 );
			const int x1 = cx + bx + SinTab[ px ] * WANDER_X / 1024;
			const int y1 = cy + by + SinTab[ py ] * WANDER_Y / 1024;
			// twinkle: period about 3..7 s; visible in the upper part of the wave, bright at its top
			const int ft = 36 + (int)( ( h4 >> 16 ) % 50 );
			const int pt = (int)( ( ms * ft / 1000 + ( ( h3 >> 16 ) & 255 ) ) & 255 );
			const int tw = SinTab[ pt ];
			if( tw < 300 ) continue;
			Plot( frame, light, W, H, P, z, x1, y1, Shade[ tw > 800 ? 1 : 0 ] );
		}
	}
}
