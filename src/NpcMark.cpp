#include "stdafx.h"

// ============================================================================================
// (new by th4) NPC MARKS -- the "!" over a towner (or a Dark Campaign NPC) who has something to say
// (config NpcSign, default on), in the look of Diablo II's: a see-through, softly glowing yellow speech
// bubble with a dark "!" in it.
// ============================================================================================
//
// WHO GETS ONE: the callers decide, this file only draws. TownMap.cpp DrawDynamicObjectsTown -- a towner
// with a speech waiting (IsTownerHasActiveSpeech); Draw.cpp, the monster part of the dungeon cell -- the
// Dark Campaign's Lazarus (mission 13), the Butcher (25) and the armorer (15). Each passes NpcMark_Note the
// centre of where the old 32x32 CEL (Data_ExclamCEL, "Data\exclam32x32bleak.CEL") was drawn -- tile x + 32,
// tile y - 103 -- so the mark stays where it always was.
//
// WHY IT IS NO LONGER DRAWN IN THE TILE LOOP: the world is drawn back to front, cell by cell, and the mark
// went on with its NPC's cell -- so every roof, wall or tree of a cell drawn LATER (in front) painted over it.
// Now the loop only notes it, and the mark goes on after the whole world.
//
// TWO WAYS TO DRAW IT:
//   TRUE COLOUR, the normal case: NpcMark.hlsl (the shape, colours and "!" are all there -- no art file),
//     drawn by dx_utility.cpp NpcMark_Present inside PostFxPresent, after the world effects and before the
//     frost / retro modes / PostFx.hlsl (so CRT and the retro looks lie on top of it, as on everything). It
//     goes over the FINISHED frame, so the UI has to be kept on top of it -- THE COVER below.
//   8-BIT, the old CEL, drawn in DrawIsoLand right after the tile loop (NpcMark_Flush, next to the overhead
//     HP bars), so it too is on top of every tile. Used where no shader can run (DrawMode 0 "surface", DDraw,
//     or the shader failed to create: NpcMark_GpuFailed) and in CLASSIC (Mordor: Classic keeps the original
//     look -- the old mark, just no longer hidden by roofs).
//
// THE COVER (true colour only): panels, text, the cursor... are drawn into the 8-bit frame AFTER the world,
// and the mark after all of that, on the GPU. So right after the world is finished (NpcMark_SnapshotWorld,
// Draw.cpp next to HeatHaze_SnapshotWorld) each mark's box is copied from the 8-bit frame (and LightSurface
// under Enriched Light); at present time (NpcMark_Cover) a pixel of the box is UI where the finished frame
// differs from that copy, where a panel rectangle covers it (dx_utility.cpp UI RECTS: a panel's black over
// black floor would otherwise pass as world), or where UI surrounds it on 3 sides (a UI pixel that happens
// to equal the world under it: a glyph hole) -- the same three rules as DynamicZoom.cpp DynZoom_BuildUiMask,
// on a few hundred pixels instead of the whole screen. The shader leaves those pixels alone.
//   DrawRedScreen (pause, death, Terminator vision -- Panel.cpp) re-colours the whole 8-bit frame, which would
// make every pixel "differ"; NpcMark_OnRedScreen re-colours the copies the same way and gives the shader the
// two colours the red screen turns the bubble and the "!" into.
//
// WHERE ON SCREEN: noted in pre-zoom WorkingSurface px; NpcMark_SnapshotWorld applies Alt+Z's 2x (the CPU
// stretch, ( x - border ) * 2, as Flames.cpp) and the Perspective warp -- that is the DRAWN frame; the pass
// applies THE VIEW (dx_utility.cpp View_Point: the wheel zoom). So the mark grows with Alt+Z and the wheel
// zoom like its NPC, as the old CEL did. Its cover box goes where the mark SHOWS (DynZoom_WorldToScreen),
// because the UI is never zoomed.
//
// PER FRAME:
//   Draw.cpp DrawIsoLand start ............ NpcMark_BeginFrame()     forget last frame's notes
//   TownMap.cpp / Draw.cpp ................ NpcMark_Note()           a mark at this centre
//   Draw.cpp DrawIsoLand, after the loop .. NpcMark_Flush()          the 8-bit CEL when no shader / Classic
//   Draw.cpp after the world .............. NpcMark_SnapshotWorld()  positions + the cover boxes' world pixels
//   Panel.cpp DrawRedScreen ............... NpcMark_OnRedScreen()
//   dx_utility.cpp PostFxPresent .......... NpcMark_List(), NpcMark_Cover() -> NpcMark_Present
// The list lives until the next drawn frame, so the presents of a palette fade (they re-convert the same
// 8-bit frame) still show the marks, faded with it; it is dropped when the fade reaches black.
//
// PERFORMANCE: nothing at all without a mark on screen. With marks: per mark a box copy and a compare of a
// few hundred to a few thousand pixels, a small texture upload, one small alpha-blended quad.
//
// KNOWN-GOOD CHECK: town, a towner with something to say standing in front of a house -- the bubble shows
// whole over the roof behind him; open the inventory over it -- the panel stays on top; pause -- the bubble
// turns red with everything else; Alt+Z or the wheel zoom -- it grows with the world. Classic: the old 8-bit
// mark, also over the roof.
// ============================================================================================

namespace {

// How far the mark reaches from its centre, 1x game px, its haze included -- NpcMark.hlsl's shape must fit
// inside (BUBBLE 14 x 9.5, the tail's tip 14.5 down, the haze GLOW_W 2.0 fades out ~8 px past the edge).
// They size the quad (dx_utility.cpp NpcMark_Present) and the cover box.
const int NPC_MARK_LEFT = 22, NPC_MARK_RIGHT = 22, NPC_MARK_UP = 18, NPC_MARK_DOWN = 23;
const int BOX_MARGIN = 2;		// the cover box is this much bigger: the box and the quad round differently
const int MAX_MARKS = 16;		// dx_utility.cpp NpcMark_Present asks for at most this many

struct Note { int x, y; };		// pre-zoom WorkingSurface px: the mark's centre
Note Notes[ MAX_MARKS ];
int NoteN = 0;

struct Mark {
	int x, y;					// the DRAWN frame: screen game px, Alt+Z and the Perspective warp applied
	int bx, by, bw, bh;			// the cover box: screen game px, where the mark SHOWS (the wheel zoom applied)
	int snap;					// its world pixels start at Snap[ snap ] (and SnapL[ snap ]), bw x bh
};
Mark Marks[ MAX_MARKS ];
int MarkN = 0;
int MarkW = 0, MarkH = 0;		// the ScreenWidth / ScreenHeight they were made for
float MarkScale = 1.0f;			// Alt+Z: 2 when the CPU drew the world doubled
std::vector<uchar> Snap, SnapL, Raw;
bool HasLight = false;
bool GpuFailed = false;

// the red screen: the palette entries the bubble and the "!" stand for, carried through every XLAT
bool Red = false;
int LensIdx = -1, InkIdx = -1;
const uchar LENS_RGB[ 3 ] = { 250, 235, 158 };	// NpcMark.hlsl LENS x 255
const uchar INK_RGB[ 3 ]  = { 33, 23, 3 };		// NpcMark.hlsl INK x 255

bool UseGpu()
{
	return !DDraw && DrawMode == 1 && GameMode != GM_CLASSIC && !GpuFailed;
}

int CeilPos( float v ){ const int i = (int)v; return (float)i < v ? i + 1 : i; }	// v >= 0

int NearestIndex( const uchar* rgb )
{
	int best = 0, bestD = 0x7FFFFFFF;
	for( int i = 0; i < 256; ++i ){
		const int dr = ScreenPalette[ i ].peRed - rgb[ 0 ], dg = ScreenPalette[ i ].peGreen - rgb[ 1 ], db = ScreenPalette[ i ].peBlue - rgb[ 2 ];
		const int d = 2 * dr * dr + 4 * dg * dg + 3 * db * db;
		if( d < bestD ){ bestD = d; best = i; }
	}
	return best;
}

} // namespace

// (th4) Draw.cpp DrawIsoLand start: forget last frame's notes.
void NpcMark_BeginFrame()
{
	NoteN = 0;
}

// (th4) TownMap.cpp DrawDynamicObjectsTown / Draw.cpp (the Dark Campaign NPCs): an NPC with something to say.
// x, y = the mark's centre, pre-zoom WorkingSurface px (the cell's tile x + 32, tile y - 103). A cell can be
// drawn twice in a frame (the arch repaint), so the same centre counts once.
void NpcMark_Note( int x, int y )
{
	for( int i = 0; i < NoteN; ++i ) if( Notes[ i ].x == x && Notes[ i ].y == y ) return;
	if( NoteN < MAX_MARKS ) Notes[ NoteN++ ] = { x, y };
}

// (th4) Draw.cpp DrawIsoLand, right after the tile loop (next to FlushOverheadHpBars, before Alt+Z's stretch):
// where the true-colour mark cannot be drawn, the old 8-bit CEL goes on here -- on top of every tile.
void NpcMark_Flush()
{
	if( !NoteN || UseGpu() ) return;
	for( int i = 0; i < NoteN; ++i ) Surface_DrawCEL( Notes[ i ].x - 16, Notes[ i ].y + 15, Data_ExclamCEL, 1, 32 );
	NoteN = 0;
}

// (th4) Draw.cpp, right after HeatHaze_SnapshotWorld (the world is finished, no UI yet): where the marks are,
// and the world pixels of their cover boxes.
void NpcMark_SnapshotWorld()
{
	MarkN = 0; Red = false; LensIdx = InkIdx = -1;
	if( !NoteN || !UseGpu() || !SoftSrcSurfacePtr ) return;
	const int z = IsZoomDisable ? 1 : 2;
	const int W = ScreenWidth, H = ScreenHeight, P = WorkingWidth;
	size_t need = 0;
	for( int i = 0; i < NoteN; ++i ){
		int x = ( Notes[ i ].x - Screen_LeftBorder ) * z, y = ( Notes[ i ].y - Screen_TopBorder ) * z;
		Perspective_WorldToScreen( x, y );	// follow the Perspective warp, if on
		// where it shows: the wheel zoom (a no-op without it), its factor read from a second point
		int sx = x, sy = y, ex = x + 1000, ey = y;
		DynZoom_WorldToScreen( sx, sy ); DynZoom_WorldToScreen( ex, ey );
		const float s = z * ( ex - sx ) / 1000.0f;
		int x0 = sx - CeilPos( NPC_MARK_LEFT * s ) - BOX_MARGIN, x1 = sx + CeilPos( NPC_MARK_RIGHT * s ) + BOX_MARGIN;
		int y0 = sy - CeilPos( NPC_MARK_UP * s ) - BOX_MARGIN, y1 = sy + CeilPos( NPC_MARK_DOWN * s ) + BOX_MARGIN;
		if( x0 < 0 ) x0 = 0; if( y0 < 0 ) y0 = 0; if( x1 > W ) x1 = W; if( y1 > H ) y1 = H;
		if( x0 >= x1 || y0 >= y1 ) continue;	// off screen
		Marks[ MarkN ] = { x, y, x0, y0, x1 - x0, y1 - y0, (int)need };
		need += (size_t)( x1 - x0 ) * ( y1 - y0 );
		++MarkN;
	}
	if( !MarkN ) return;
	HasLight = g_A8L8Lighting && LightSurface;
	if( Snap.size() < need ) Snap.resize( need );
	if( HasLight && SnapL.size() < need ) SnapL.resize( need );
	const size_t top = (size_t)P * Screen_TopBorder + Screen_LeftBorder;
	for( int i = 0; i < MarkN; ++i ){
		const Mark& m = Marks[ i ];
		for( int r = 0; r < m.bh; ++r ){
			const size_t o = top + (size_t)( m.by + r ) * P + m.bx;
			memcpy( &Snap[ m.snap + r * m.bw ], SoftSrcSurfacePtr + o, m.bw );
			if( HasLight ) memcpy( &SnapL[ m.snap + r * m.bw ], LightSurface + o, m.bw );
		}
	}
	MarkW = W; MarkH = H; MarkScale = (float)z;
}

// (th4) Panel.cpp DrawRedScreen, after it re-coloured the frame through xlat (indices below minIndex kept):
// the same to the copies, so only real UI keys as cover; and the bubble and the "!" go red the same way.
void NpcMark_OnRedScreen( const uchar* xlat, int minIndex )
{
	if( !MarkN || !xlat ) return;
	for( int i = 0; i < MarkN; ++i ){
		uchar* p = &Snap[ Marks[ i ].snap ];
		for( int k = Marks[ i ].bw * Marks[ i ].bh; k > 0; --k, ++p ) if( *p >= minIndex ) *p = xlat[ *p ];
	}
	if( LensIdx < 0 ){ LensIdx = NearestIndex( LENS_RGB ); InkIdx = NearestIndex( INK_RGB ); }
	if( LensIdx >= minIndex ) LensIdx = xlat[ LensIdx ];
	if( InkIdx >= minIndex ) InkIdx = xlat[ InkIdx ];
	Red = true;
}

// (th4) dx_utility.cpp NpcMark_Present: the marks of the last drawn frame. pos = centre in the DRAWN frame
// (screen game px; THE VIEW still to apply), box = the cover box (screen game px: x, y, w, h), reach = the
// mark's extent from its centre (1x px: left, up, right, down), scale = Alt+Z's factor; under the red screen
// red = true and redLens / redInk = its colours (0..1). Returns how many (0 = nothing to draw now).
int NpcMark_List( float (*pos)[2], int (*box)[4], int maxMarks, float* reach, float& scale, bool& red, float* redLens, float* redInk )
{
	if( !gbRunGame || MarkW != ScreenWidth || MarkH != ScreenHeight ) return 0;
	if( PaletteFadeLevel <= 0 ){ MarkN = 0; return 0; }	// faded to black: whatever comes next draws its own world
	const int n = MarkN < maxMarks ? MarkN : maxMarks;
	for( int i = 0; i < n; ++i ){
		const Mark& m = Marks[ i ];
		pos[ i ][ 0 ] = (float)m.x; pos[ i ][ 1 ] = (float)m.y;
		box[ i ][ 0 ] = m.bx; box[ i ][ 1 ] = m.by; box[ i ][ 2 ] = m.bw; box[ i ][ 3 ] = m.bh;
	}
	reach[ 0 ] = (float)NPC_MARK_LEFT; reach[ 1 ] = (float)NPC_MARK_UP; reach[ 2 ] = (float)NPC_MARK_RIGHT; reach[ 3 ] = (float)NPC_MARK_DOWN;
	scale = MarkScale;
	red = Red && LensIdx >= 0;
	if( red ){
		const PALETTEENTRY& l = ScreenPalette[ LensIdx ];
		const PALETTEENTRY& k = ScreenPalette[ InkIdx ];
		redLens[ 0 ] = l.peRed / 255.0f; redLens[ 1 ] = l.peGreen / 255.0f; redLens[ 2 ] = l.peBlue / 255.0f;
		redInk[ 0 ] = k.peRed / 255.0f; redInk[ 1 ] = k.peGreen / 255.0f; redInk[ 2 ] = k.peBlue / 255.0f;
	}
	return n;
}

// (th4) dx_utility.cpp NpcMark_Present: the COVER of mark i -- 255 where the UI was drawn over it, 0 where the
// world shows -- its box (bw x bh) written to dst with the given pitch. rects = the UI RECTS (screen game px,
// x1 / y1 exclusive). Same rules as DynamicZoom.cpp DynZoom_BuildUiMask (see the header).
void NpcMark_Cover( int i, uchar* dst, int pitch, const int (*rects)[4], int rectN )
{
	if( i < 0 || i >= MarkN || !SoftSrcSurfacePtr ) return;
	const Mark& m = Marks[ i ];
	const int w = m.bw, h = m.bh, P = WorkingWidth;
	if( Raw.size() < (size_t)w * h ) Raw.resize( (size_t)w * h );
	const size_t top = (size_t)P * Screen_TopBorder + Screen_LeftBorder;
	const bool light = HasLight && LightSurface;
	// 1) raw: the finished frame differs from the world copy, or a panel covers the pixel
	for( int r = 0; r < h; ++r ){
		const size_t o = top + (size_t)( m.by + r ) * P + m.bx;
		const uchar* fin = SoftSrcSurfacePtr + o; const uchar* snap = &Snap[ m.snap + r * w ];
		const uchar* finL = light ? LightSurface + o : 0; const uchar* snapL = light ? &SnapL[ m.snap + r * w ] : 0;
		uchar* d = &Raw[ (size_t)r * w ];
		for( int x = 0; x < w; ++x ) d[ x ] = ( fin[ x ] != snap[ x ] || ( light && finL[ x ] != snapL[ x ] ) ) ? 255 : 0;
		const int y = m.by + r;
		for( int k = 0; k < rectN; ++k ){
			if( y < rects[ k ][ 1 ] || y >= rects[ k ][ 3 ] ) continue;
			int a = rects[ k ][ 0 ] - m.bx, b = rects[ k ][ 2 ] - m.bx;
			if( a < 0 ) a = 0; if( b > w ) b = w;
			if( a < b ) memset( d + a, 255, b - a );
		}
	}
	// 2) a world pixel with UI on 3 of its 4 sides is UI too (judged on the raw mask; the box edge is left raw)
	for( int r = 0; r < h; ++r ){
		const uchar* s = &Raw[ (size_t)r * w ];
		uchar* d = dst + (size_t)r * pitch;
		memcpy( d, s, w );
		if( r == 0 || r == h - 1 ) continue;
		for( int x = 1; x < w - 1; ++x ){
			if( s[ x ] ) continue;
			const int nb = ( s[ x - 1 ] != 0 ) + ( s[ x + 1 ] != 0 ) + ( s[ x - w ] != 0 ) + ( s[ x + w ] != 0 );
			if( nb >= 3 ) d[ x ] = 255;
		}
	}
}

// (th4) dx_utility.cpp NpcMark_Present: NpcMark.hlsl could not be created -- the 8-bit mark from now on.
void NpcMark_GpuFailed()
{
	GpuFailed = true;
}
