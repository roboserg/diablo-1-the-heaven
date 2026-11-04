#include "stdafx.h"

Light LightList[Lights_128_Count];
// (th4) Stage 3: colored-light tint side table (parallel to LightList).
// White = no tint. LightList is raw-serialized to saves, so the tint cannot
// live in the Light struct — it is a runtime-only side channel.
// (th4) Rev 2.46: enlarged to Lights_128_Count (save persists at most 32).
LightTint LightTintList[Lights_128_Count];

// PPL animation remap table.
// For Hell dungeons: points to LightTable row 0 (the blood-fade cycling row).
//   LightTable cycling (AnimateHellPalette) advances this mapping each frame →
//   linecpyPerPixel writes an animated blood-range index to WorkingSurface while
//   the GPU correctly applies brightness(lv). Both PPL and non-PPL animate identically.
// For all other dungeons: points to s_PPLIdentityRemap (sv → sv), preserving the
//   original raw-index behaviour. AnimateCavePalette/Crypt/Abyss cycle ScreenPalette
//   instead, which remains unaffected by this table.
static uchar s_PPLIdentityRemap[256];
namespace {
    struct PPLIdentityInit {
        PPLIdentityInit() { for (int i = 0; i < 256; i++) s_PPLIdentityRemap[i] = (uchar)i; }
    } _pplIdInit;
}
uchar* g_PPLAnimRemap = s_PPLIdentityRemap;

// Palette entries [CRYPT_BRIGHT_PAL_FIRST, CRYPT_BRIGHT_PAL_END) are the animated
// water/lava range in crypt and abyss.  MakeLightTable maps them to themselves in
// every intermediate LightTable row ("always bright regardless of distance").
// The same range is used for g_PPLBrightnessMask in the A8L8 path.
// JKE comment: "16 is the number of rotated colors."
constexpr int CRYPT_BRIGHT_PAL_FIRST = 1;
constexpr int CRYPT_BRIGHT_PAL_END   = 16; // exclusive: entries 1..15

//----- (0042BC67) --------------------------------------------------------
void __fastcall RotateOffset(int& x, int& y, int& rx, int& ry, int& ox, int& oy, int& ax, int& ay)
{
	int tmp;
	ax = ay = 0;
	tmp = rx; rx = 7 - ry; ry = tmp; // rotate rx,ry by 90 deg. clockwise
	tmp = ox; ox = 7 - oy; oy = tmp; // rotate origin by 90 deg. clockwise
	x = rx - ox; y = ry - oy;        // set x,y to rx,ry shifted relative to origin
	if( x < 0 ){ x += 8; ax = 1; }   // bring them back into 0..7 range
	if( y < 0 ){ y += 8; ay = 1; }
}

//----- (0042BCDE) --------------------------------------------------------
void __fastcall DoLighting(int row, int col, int radius, int lightIndex)
{
	if( ! To112(row, col) ) return; // (rt) memory corruption fix (applied in th1 1.216)
	if( (uint) radius > 15 ) radius = 15;

	int xoff = 0, yoff = 0; // offset into tile, where tile is broken
	if( lightIndex >= 0 ){
		xoff = LightList[lightIndex].deltaRow;
		yoff = LightList[lightIndex].deltaCol;
		if( xoff < 0 ){ xoff += 8; row--; }
		if( yoff < 0 ){ yoff += 8; col--; }
	}
	// into 8 x 8 grid for increased distance resolution.
	int rxoff = xoff; // rotated offsets
	int ryoff = yoff;
	int orgx = 0, orgy = 0;
	int addx = 0, addy = 0;

	// clipping vars: offsets from row, col; absolute vals
	int maxleft  = row - 15 < 0           ?           1 + row : 15;
	int maxright = row + 15 > FineMap_112 ? FineMap_112 - row : 15;
	int maxup    = col - 15 < 0           ?           1 + col : 15;
	int maxdown  = col + 15 > FineMap_112 ? FineMap_112 - col : 15;
	
	if( Dungeon->graphType < DT_5_CRYPT ){
		LightMap[row][col] = 0;
	}else{
		if( LightMap[row][col] > LightLevels[radius][0] ){
			LightMap[row][col] = LightLevels[radius][0];
		}
	}
	int y, x;
	auto LightQuadrant = [&]( int max1, int max2, auto d1, auto d2 ){
		for( y = 0; y < max1; y++ ){
			for( x = 1; x < max2; x++ ){
				int dist = LightDist[xoff][yoff][x + addx][y + addy];
				if( dist < 128 ){
					int r = row + d1(); if( (uint)r >= FineMap_112 ) continue;
					int c = col + d2(); if( (uint)c >= FineMap_112 ) continue;
					int lightVal = LightLevels[radius][dist];
					if( lightVal < LightMap[r][c] ){
						LightMap[r][c] = lightVal;
					} 
				}
			}
		}
	};

	#define E(expr) [&]{return expr;}
	LightQuadrant( maxup,   maxright, E( x), E( y) );                 //   |x
	RotateOffset( xoff, yoff, rxoff, ryoff, orgx, orgy, addx, addy ); //   | 

	LightQuadrant( maxdown, maxright, E( y), E(-x) );                 //   | 
	RotateOffset( xoff, yoff, rxoff, ryoff, orgx, orgy, addx, addy ); //   |x

	LightQuadrant( maxdown,  maxleft, E(-x), E(-y) );                 //   | 
	RotateOffset( xoff, yoff, rxoff, ryoff, orgx, orgy, addx, addy ); //  x| 

	LightQuadrant( maxup,    maxleft, E(-y), E( x) );                 //  x| 
																	  //   | 
}

//----- (0042C05C) --------------------------------------------------------
void __fastcall DoUnLight( int row, int col, int radius )
{
	int colMin = col - (radius + 1);
	int colMax = radius + 1 + col;
	int rowMin = row - (radius + 1);
	int rowMax = radius + 1 + row;
	if( colMin < 0 ) colMin = 0;
	if( colMax > FineMap_112 ) colMax = FineMap_112;
	if( rowMin < 0 ) rowMin = 0;
	if( rowMax > FineMap_112 ) rowMax = FineMap_112;
	for( int c = colMin; c < colMax; ++c ){
		for( int r = rowMin; r < rowMax; ++r ){
			LightMap[ r ][ c ] = StaticLightMap[ r ][ c ];
		}
	}
}

//----- (0042C0C7) --------------------------------------------------------
void __fastcall DoUnVision(int row, int col, int lightRadius)
{
	int lightRadiusPlusOne = lightRadius + 1;
	int maxCol = col + lightRadiusPlusOne;
	int minCol = col - lightRadiusPlusOne;
	int minRow = row - lightRadiusPlusOne;
	int maxRow = row + lightRadiusPlusOne;
	
	LimitToMin(minRow, 0);
	LimitToMin(minCol, 0);
	LimitToMax(maxRow, FineMap_112);
	LimitToMax(maxCol, FineMap_112);

	for( int curRow = minRow; curRow < maxRow; curRow++ ){
		for( int curCol = minCol; curCol < maxCol; curCol++ ){
			FlagMap[ curRow ][ curCol ] &= ~(CF_2_VISIBLE_BY_PLAYER | CF_64_VISIBLE_BY_CURSOR);
		}
	}
}

//---- new ----------------------------------------------------------------
void FillIceDarkenPalette()
{
	if( Dungeon->isQuest && Dungeon == DUN_39_FROZEN_LOCH 
		// mor: this didnt work. i tried to make nonquest cave maps in ice age lose lava light
		/*|| (!Dungeon->isQuest && Dungeon->genType == DT_3_CAVE && Players[CurrentPlayerIndex].gameChanger & BIT(GC_15_ICE_AGE))*/){
		//for( int l = 1; l < 15; ++l ){
		//	for( int i = 1; i < 32; ++i ){
		//		PaletteShadowTable[ l * 256 + i ] = i;
		//	}
		//}
		//return;
		LoadPalette( "levels\\l3data\\glacial03.pal" );
		PALETTEENTRY* filePalette = FilePalette;
		for( int l = 1; l < 15; ++l ){
			PALETTEENTRY p16 = filePalette[ 16 ];
			if( false /*EasyClick*/ ){
				//double k = log( (2.71828182845904523536 - 1.) * ((15.-l)/15.) + 1. );
				//double k = log(1.0 + 3.5*((15.-l)/15.)) / log(4.5);
				//double k = log(1.0 + 5.5*((15.-l)/15.)) / log(6.5);
				double k = log(1.0 + 8.0*((15.-l)/15.)) / log(9.0);
				p16.peRed   = (uchar)((double) p16.peRed   * k);
				p16.peGreen = (uchar)((double) p16.peGreen * k);
				p16.peBlue  = (uchar)((double) p16.peBlue  * k);
			}else{
				p16.peRed   = (int) p16.peRed * (15 - l) / 15;
				p16.peGreen = (int) p16.peGreen * (15 - l) / 15;
				p16.peBlue  = (int) p16.peBlue * (15 - l) / 15;
			}
			for( int i = 1; i < 32; ++i ){
				PALETTEENTRY p;
				if( i == 16 ){
					p = p16;
				} else{
					int peRed = (int) p16.peRed - ((int) filePalette[ 16 ].peRed - filePalette[ i ].peRed);
					int peGreen = (int) p16.peGreen - ((int) filePalette[ 16 ].peGreen - filePalette[ i ].peGreen);
					int peBlue = (int) p16.peBlue - ((int) filePalette[ 16 ].peBlue - filePalette[ i ].peBlue);
					p.peRed = peRed < 0 ? 0 : peRed;
					p.peGreen = peGreen < 0 ? 0 : peGreen;
					p.peBlue = peBlue < 0 ? 0 : peBlue;
				}
				int minIndex = 0;
				int dr = abs( (int) p.peRed - filePalette[ 0 ].peRed );
				int dg = abs( (int) p.peGreen - filePalette[ 0 ].peGreen );
				int db = abs( (int) p.peBlue - filePalette[ 0 ].peBlue );
				int minDif = (int) sqrt( 2 * dr * dr + 4 * dg * dg + 3 * db * db );
				for( int c = 1; c < 256; ++c ){
					int dr = abs( (int) p.peRed - filePalette[ c ].peRed );
					int dg = abs( (int) p.peGreen - filePalette[ c ].peGreen );
					int db = abs( (int) p.peBlue - filePalette[ c ].peBlue );
					int dif = (int) sqrt( 2 * dr * dr + 4 * dg * dg + 3 * db * db );
					if( dif < minDif ){
						minDif = dif;
						minIndex = c;
					}
				}
				LightTable[ l * 256 + i ] = minIndex;
			}
		}
	}
}

//----- A8L8 per-pixel light field: LightLevels builder + falloff LUT -----------
// BuildLightLevelsTable is the SINGLE source of truth for the per-radius
// LightLevels table (lv 0..15 per 1/8-tile distance) — extracted verbatim from
// MakeLightTable so the production table (MakeLightTable) and the Debug
// self-check reference (RunA8L8LightBrightLutSelfCheck) are built by the SAME
// code; the reference can no longer drift from production.
//
// g_A8L8LightBright[rad][i] = brightness 0..255 (255 = fully lit) at linear
// distance d = i/64 tile units from the light center, i = 0..1024
// (rad 15 max distance = 16 tiles), consumed ONLY by the per-pixel field (V2):
//  - non-crypt, OriginalLightLevels on or off: lv = f(2-f)*15  =>  B = (1-f)^2,
//    continuous to B = 0 at the radius edge (C^1 — no hard cutoff).  A
//    gammaBlend knob (0..1, config "EnrichedLightEnhancedGamma" percent,
//    default 100)
//    generalizes both non-crypt curves to B = 255(1-f)(1-gammaBlend*f):
//    gammaBlend=1.0 keeps the exact original bytes; lower values keep a
//    NONZERO slope at the edge (255(1-gammaBlend)/dMax per unit), which
//    spreads the 8-bit LUT steps evenly instead of piling them into wide
//    edge plateaus (the contour-ring banding).  Crypt/abyss curves are
//    unaffected.
//  - improved crypt/abyss: lv = (blend*f + (1-blend)*f^2)*15,
//    blend = min(1, rad*1.25/15), radius-limited to 8(rad+1).
//  - OriginalLightLevels crypt/abyss (Rev 25 hybrid): the smooth (1-f)^2 main
//    body is kept (Rev 15's accepted curve — bright center, no per-tile
//    plateaus), but V1's far-field TAIL is recovered as a SMOOTH quadratic
//    fade anchored at V1's per-radius far-end value (the literal row's
//    lvTable[15-rad][127]; B_floor = 17..68 for rad ≤ 13, 0 for rad 15/14):
//    B_tail(i) = B_floor·(1 − (i/1024)²), reaching 0 at i=1024 (16 tiles =
//    V1's dist<128 boundary).  The join is a log-sum-exp smooth-max (K=3):
//    C¹ on (0,1024) — the slope always lies between the two curves' slopes
//    (hence monotone), lifts plain max() by at most K·ln2 = 2 brightness at
//    the exact crossing and matches it elsewhere; i=1024 is forced to 0 so
//    the 16-tile boundary stays exactly black (the final 1/64-tile step drops
//    ≤ ~2 brightness, so the curve is NOT C¹ at the boundary itself — visually
//    negligible, below the banding threshold).  Rad 15/14 (B_floor = 0) keep
//    the pure (1-f)^2 curve.  NOTE: the first Rev 25 iteration reproduced the
//    literal flat tail shelf and was REJECTED in playtest — per-pixel
//    rendering turned the flat per-radius plateaus (B=34 for 8.4 tiles at
//    rad 9, etc.) into contour discs of distinct brightness around every
//    light.  The smooth tail keeps the "not pitch black" character with no
//    flat region at all.
static void BuildLightLevelsTable( uchar lv[16][128], int graphType, bool a8l8, double gammaBlend = 1.0 )
{
	if( OriginalLightLevels ){ // original light levels
		for( int rad = 0; rad < 16; ++rad ){ // Doron's new smooth light tables
			for( int d = 0; d < 128; ++d ){
				if( d <= 8 * (rad + 1) ){
					if (a8l8) {
						// A8L8: gamma-2 correction to match the perceptual falloff of the
						// LightTable path.  Original formula is linear (lv = factor*15); the
						// shader applies brightness=(1-lv/15) also linearly, so the same lv
						// looks brighter with A8L8 than with LightTable palette-snap (~gamma-2).
						// Remapping: f*(2-f)*15 = (1-(1-f)^2)*15 steepens the curve so lv
						// rises faster with distance, halving the apparent light radius.
						const double factor = (double)d / ((double)(rad + 1) * 8.0);
						// gammaBlend shapes the falloff: 1.0 = f(2-f)*15 => (1-f)^2
						// (the original A8L8 quadratic), 0.0 = linear.  The ==1.0
						// path keeps the exact pre-knob expression so the table
						// stays byte-identical for the default configuration.
						const double inner = ( gammaBlend == 1.0 )
							? ( 2.0 - factor )
							: ( 1.0 + gammaBlend * ( 1.0 - factor ) );
						const double scaled = factor * inner * 15.0;
						// Cap at 14: lv=15 (full dark) is reserved for the distance > radius branch.
						// At factor=1 the formula gives exactly 15.
						lv[rad][d] = (uchar)std::min(scaled + 0.5, 15.0);
					} else {
						// Original linear falloff (A8L8=OFF, unchanged).
						lv[rad][d] = ftol(double(d) * 15.0 / (double(rad + 1) * 8.0) + 0.5);
					}
				}else{
					lv[rad][d] = 15;
				}
			}
		}
		if( graphType >= DT_5_CRYPT ){
			for( int rad = 0; rad < 16; rad++ ){
				double radiusMod = (sqrt((double)(16 - rad))) / 128.0;
				radiusMod *= radiusMod;
				for( int d = 0; d < 128; d++ ){ //here
					// Compute the original crypt lv using the existing formula.
					// uchar underflow (15 - value > 15) wraps to large value → clamped to 0
					// (fully bright) by the ">15" guards — intentional original design.
					uchar l = 15 - (uchar)(radiusMod * (double)((128 - d) * (128 - d)));
					if( l > 15 ) l = 0;
					l = l - (uchar)((15 - rad) / 2);
					if( l > 15 ) l = 0;
					if (a8l8) {
						// A8L8: apply gamma-2 correction to the computed crypt lv.
						//
						// The original formula 2*lv - lv²/15 was derived for linear-falloff
						// input (non-crypt standard formula).  The crypt formula already
						// produces a quadratic-like falloff that clusters lv near the dark
						// end.  Applying the same aggressive gamma remap on top of that
						// nonlinear distribution compresses all mid-range values into the
						// cap at 14, making the entire lit area render at ~6.67 % brightness.
						//
						// FIX: use a sqrt-based gamma correction (milder than quadratic).
						//   scaled = 15·sqrt(lv/15)  maps [0,15]→[0,15]:
						//     0→0   7→10.3   12→13.4   15→15
						// This preserves gamma compensation but with a gentler curve
						// appropriate for the crypt formula's already-nonlinear input.
						// Cap at 14 so lv=15 is reserved for the non-crypt
						// distance > radius branch and the distance-table out-of-range fallback.
						const double scaled = 15.0 * sqrt((double)l / 15.0);
						l = (uchar)std::min(scaled + 0.5, 15.0);
					}
					lv[15 - rad][d] = l;
				}
			}
		}
	}else{ // improved light levels
		const float maxDarkness = 15.0f;
		const float maxBrightness = 0.0f;
		for( unsigned radius = 0; radius < 16; radius++) {
			const unsigned maxDistance = (radius + 1) * 8;
			for (unsigned distance = 0; distance < 128; distance++) {
				if (distance > maxDistance) {
					lv[radius][distance] = 15;
				} else {
					const float factor = static_cast<float>(distance) / static_cast<float>(maxDistance);
					float scaled;
					if( is(graphType, DT_5_CRYPT, DT_6_ABYSS) ){
						if (a8l8) {
							// A8L8 fix: corrected quadratic with over-exposure for crypt/abyss.
							//
							// ORIGINAL BUG: scaled = factor² * brightness + (maxDarkness - brightness)
							// At distance=0 (factor=0): scaled = maxDarkness - brightness = 15 - radius*1.25.
							// For radius < 12 this is > 0; at radius=0 scaled = 15 = lmax.
							// In A8L8 mode lv >= lmax triggers {d[i]=0; dstL[i]=lmax} → pure black.
							// (A8L8=OFF was not affected: lv=15 still goes through LightTable which
							//  returns a dark palette color rather than index 0.)
							//
							// FIX: blend from quadratic (radius=0, steep) to linear (radius≥12, gentle).
							//   lv=0 at distance=0 for ALL radii (center always fully bright).
							//   lv=15 at distance=maxDistance (edge always fully dark).
							//   Larger radius → more linear → wider bright zone (over-exposure preserved).
							const float blend = std::min(1.0f, static_cast<float>(radius) * 1.25f / maxDarkness);
							scaled = (blend * factor + (1.0f - blend) * factor * factor) * maxDarkness;
						} else {
							// A8L8=OFF: preserve original crypt/abyss formula for backward compatibility.
							// (lv=15 at center for small-radius lights is visible as "very dark" via
							//  LightTable, not pitch black — no behavioral bug in that path.)
							const float brightness = static_cast<float>(radius) * 1.25F;
							scaled = factor * factor * brightness + (maxDarkness - brightness);
							scaled = std::max(maxBrightness, scaled);
						}
					} else {
						if (a8l8) {
							// A8L8: compensate for linear GPU brightness vs LightTable ~gamma-2 darkening.
							// Shader: brightness = saturate(1 - lv/15)  (linear in lv).
							// LightTable: palette-snap darkening acts like gamma≈2 in perceptual space,
							// making mid-range lv (≈7) look darker (~25%) than linear (53%).
							// Users perceived the same LightLevels as a much larger light radius.
							// Fix without changing the shader: apply the inverse-gamma-2 mapping to factor
							// before computing lv.  f*(2-f) = 1-(1-f)² maps [0,1]→[0,1] with a steeper
							// initial slope, causing lv to rise faster with distance.
							// gammaBlend: same knob as the OriginalLightLevels branch;
							// the ==1.0 path keeps the exact pre-knob expression.
							const float inner = ( gammaBlend == 1.0 )
								? ( 2.0f - factor )
								: ( 1.0f + (float)gammaBlend * ( 1.0f - factor ) );
							scaled = factor * inner * maxDarkness;
						// Cap at 14: factor=1.0 (exactly at maxDistance) gives 15.  Only the distance > maxDistance branch sets lv=15.
						if (scaled > 15.0f) scaled = 15.0f;
						} else {
							// A8L8=OFF: original linear falloff (unchanged).
							scaled = factor * maxDarkness;
						}
					}
					lv[radius][distance] = static_cast<uint8_t>(scaled + 0.5f); // round up
				}
			}
		}

	}
}

static void BuildA8L8LightBrightLut( int graphType, const uchar lvTable[16][128], double gammaBlend = 1.0 )
{
	const int nDist64 = countof( g_A8L8LightBright[0] );
	for( int rad = 0; rad < 16; rad++ ){
		uchar* dst = g_A8L8LightBright[rad];
		const int dMax = 64 * ( rad + 1 ); // 1/64-tile units
		if( OriginalLightLevels && graphType >= DT_5_CRYPT ){
			// Crypt/abyss + OriginalLightLevels hybrid (see header): smooth
			// (1-f)^2 main body + a smooth far-field tail anchored at V1's
			// per-radius far-end value, joined by a log-sum-exp smooth-max so
			// the curve is C¹ on (0,1024) — no slope kink at the join; the
			// forced-black i=1024 boundary is a final ≤~2 brightness step and
			// is not part of the C¹ join.  The literal flat tail shelf was
			// rejected in playtest (contour discs at per-pixel fidelity); this
			// quadratic fade keeps the "not pitch black" character without any
			// flat region.
			constexpr double kSmoothJoin = 3.0;
			const double bFloor = 255.0 * ( 1.0 - (double)lvTable[15 - rad][127] / 15.0 );
			for( int i = 0; i < nDist64; i++ ){
				double bMain = 0.0;
				if( i <= dMax ){
					const double f = (double)i / (double)dMax;
					bMain = 255.0 * ( 1.0 - f ) * ( 1.0 - f );
				}
				double b;
				if( bFloor <= 0.0 || i >= 1024 ){
					// rad 15/14: V1's far end is pitch black -> plain curve.
					// i=1024 forced to 0: the 16-tile boundary stays black.
					b = bMain;
				}else{
					const double f16 = (double)i / 1024.0;
					const double bTail = bFloor * ( 1.0 - f16 * f16 );
					// log-sum-exp smooth-max: hi + K·ln(1+e^(lo-hi)/K).  The
					// result is C¹ and its slope stays between the two curves'
					// slopes, so the row stays monotone non-increasing.
					const double hi = bMain > bTail ? bMain : bTail;
					const double lo = bMain > bTail ? bTail : bMain;
					b = hi + kSmoothJoin * log1p( exp( ( lo - hi ) / kSmoothJoin ) );
				}
				dst[i] = (uchar)( b + 0.5 );
			}
		}else if( OriginalLightLevels ){
			// Non-crypt + OriginalLightLevels: B = 255(1-f)(1-gammaBlend*f).
			// gammaBlend=1.0 -> 255(1-f)^2 (the original quadratic; byte-identical
			// because 1.0*f == f); gammaBlend=0.0 -> 255(1-f) linear.  Lower values
			// keep a NONZERO slope at the edge (255(1-gammaBlend)/dMax per unit),
			// which spreads the 8-bit LUT steps evenly instead of piling them into
			// wide edge plateaus (the visible contour rings).
			for( int i = 0; i < nDist64; i++ ){
				if( i > dMax ){
					dst[i] = 0;
					continue;
				}
				const double f = (double)i / (double)dMax;
				const double oneMinusF = 1.0 - f;
				dst[i] = (uchar)( 255.0 * oneMinusF * ( 1.0 - gammaBlend * f ) + 0.5 );
			}
		}else{
			if( graphType >= DT_5_CRYPT ){
				const double blend = rad * 1.25 / 15.0 > 1.0 ? 1.0 : rad * 1.25 / 15.0;
				for( int i = 0; i < nDist64; i++ ){
					if( i > dMax ){
						dst[i] = 0;
						continue;
					}
					const double f = (double)i / (double)dMax;
					const double g = blend * f + ( 1.0 - blend ) * f * f;
					dst[i] = (uchar)( 255.0 * ( 1.0 - g ) + 0.5 );
				}
			}else{
				// Non-crypt improved: same gammaBlend knob as the OriginalLightLevels
				// branch (gammaBlend=1.0 -> 255(1-f)^2, byte-identical).
				for( int i = 0; i < nDist64; i++ ){
					if( i > dMax ){
						dst[i] = 0;
						continue;
					}
					const double f = (double)i / (double)dMax;
					const double oneMinusF = 1.0 - f;
					dst[i] = (uchar)( 255.0 * oneMinusF * ( 1.0 - gammaBlend * f ) + 0.5 );
				}
			}
		}
	}
}

#ifdef _DEBUG

// Debug-time self-check: compares the LUT against a LightLevels table at tile
// centers — for distance d in 1/8-tile steps the current system samples
// LightLevels[rad][d] (lv 0..15), target brightness = round(255*(1 - lv/15)),
// LUT entry = g_A8L8LightBright[rad][8*d].
// Expected max deviation is 9 (rounding of the lv <-> brightness conversion);
// anything above 10 indicates a formula mismatch.  The crypt/abyss +
// OriginalLightLevels combo is skipped by the callers (the render LUT deviates
// from the literal-tail LightLevels by design — see the LUT header).
static int VerifyA8L8LightBrightLut( int graphType, const uchar lvTable[16][128], int* worstRad, int* worstD )
{
	int maxDev = 0;
	*worstRad = -1;
	*worstD = -1;
	for( int rad = 0; rad < 16; rad++ ){
		// d range with a light contribution: everything is radius-limited to
		// 8*(rad+1) in the render LUT.
		const int dEnd = std::min( 8 * ( rad + 1 ), 127 );
		for( int d = 0; d <= dEnd; d++ ){
			const int target = (int)( 255.0 * ( 1.0 - (double)lvTable[rad][d] / 15.0 ) + 0.5 );
			const int got = g_A8L8LightBright[rad][8 * d];
			const int dev = got > target ? got - target : target - got;
			if( dev > maxDev ){
				maxDev = dev;
				*worstRad = rad;
				*worstD = d;
			}
		}
	}
	return maxDev;
}

// Reference LightLevels for the CLI self-check harness = the SAME shared builder
// MakeLightTable uses (A8L8 branches — the LUT's only consumer is the A8L8
// path).  No duplicated formula copy to keep in sync.
//
// Hybrid verifier for crypt/abyss + OriginalLightLevels (the combo where the
// render LUT deviates from the literal LightLevels by design): checks every LUT
// row against the hybrid construction — (1-f)^2 main body joined to a smooth
// quadratic tail (anchored at the literal row's far-end value) by the same
// log-sum-exp smooth-max — and that each row stays monotone non-increasing
// (V2's binary-search support radius relies on it).
static int VerifyA8L8LightBrightLutHybrid( int graphType, const uchar lvTable[16][128], int* worstRad, int* worstD, int* nonMonotoneCount )
{
	constexpr double kSmoothJoin = 3.0;
	int maxDev = 0;
	*worstRad = -1;
	*worstD = -1;
	*nonMonotoneCount = 0;
	for( int rad = 0; rad < 16; rad++ ){
		const uchar* lut = g_A8L8LightBright[rad];
		const uchar* lvRow = lvTable[15 - rad];
		const int dMax8 = 8 * ( rad + 1 );
		const double bFloor = 255.0 * ( 1.0 - (double)lvRow[127] / 15.0 );
		for( int d = 0; d < 128; d++ ){
			double bMain = 0.0;
			if( d <= dMax8 ){
				const double f = (double)d / (double)dMax8;
				bMain = 255.0 * ( 1.0 - f ) * ( 1.0 - f );
			}
			double expect;
			if( bFloor <= 0.0 ){
				expect = bMain;
			}else{
				const double f16 = ( 8.0 * (double)d ) / 1024.0;
				const double bTail = bFloor * ( 1.0 - f16 * f16 );
				const double hi = bMain > bTail ? bMain : bTail;
				const double lo = bMain > bTail ? bTail : bMain;
				expect = hi + kSmoothJoin * log1p( exp( ( lo - hi ) / kSmoothJoin ) );
			}
			const int expectI = (int)( expect + 0.5 );
			const int got = lut[8 * d];
			const int dev = got > expectI ? got - expectI : expectI - got;
			if( dev > maxDev ){
				maxDev = dev;
				*worstRad = rad;
				*worstD = d;
			}
		}
		for( int i = 0; i < 1024; i++ ){
			if( lut[i] < lut[i + 1] ) ( *nonMonotoneCount )++;
		}
	}
	return maxDev;
}

// CLI self-check harness: "TH4.exe -lightlutcheck" (see WinMain). Runs before any
// game init: for every graphType x OriginalLightLevels combo it builds a reference
// LightLevels table, builds the LUT, compares both at tile centers, and writes
// "A8L8LightBright_selfcheck.txt". Also reports the largest adjacent LUT step per
// combo (banding indicator for Phase 2).
void RunA8L8LightBrightLutSelfCheck()
{
	FILE* f = fopen( "A8L8LightBright_selfcheck.txt", "w" );
	if( !f ) return;
	const bool savedOriginal = OriginalLightLevels;
	// gammaBlend sweep: 1.0 = original (1-f)^2, 0.0 = linear.  The crypt/abyss
	// + OriginalLightLevels hybrid branch ignores gammaBlend, so it is checked
	// once (gammaBlend=1.0).
	const double gammaSweep[] = { 1.0, 0.75, 0.5, 0.0 };
	int combosFailed = 0;
	for( int orig = 0; orig <= 1; orig++ ){
		OriginalLightLevels = orig != 0;
		for( int gt = 0; gt <= 6; gt++ ){
			for( int gi = 0; gi < 4; gi++ ){
				const double gammaBlend = gammaSweep[gi];
				if( orig && gt >= 5 && gi != 0 ) continue; // hybrid ignores gammaBlend
				uchar refLv[16][128];
				BuildLightLevelsTable( refLv, gt, true, gammaBlend ); // A8L8 branches — the LUT's only consumer
				BuildA8L8LightBrightLut( gt, refLv, gammaBlend );
				if( orig && gt >= 5 ){
				// crypt/abyss + OriginalLightLevels: hybrid curve (smooth
				// (1-f)^2 body + recovered V1 tail) — deviates from the
				// literal LightLevels by design, so verify the hybrid
				// construction instead of V1 equality.
				int worstRad, worstD, nonMonotone;
				const int maxDev = VerifyA8L8LightBrightLutHybrid( gt, refLv, &worstRad, &worstD, &nonMonotone );
				const bool ok = maxDev <= 10 && nonMonotone == 0;
				if( !ok ) combosFailed++;
				fprintf( f, "graphType=%d OriginalLightLevels=%d gammaBlend=%.2f : hybrid maxDev=%d (rad=%d d=%d) nonMonotone=%d %s\n",
					gt, orig, gammaBlend, maxDev, worstRad, worstD, nonMonotone, ok ? "OK" : "FAILED" );
				continue;
			}
			int worstRad, worstD;
			const int maxDev = VerifyA8L8LightBrightLut( gt, refLv, &worstRad, &worstD );

			int maxStep = 0;
			int stepRad = -1;
			for( int rad = 0; rad < 16; rad++ ){
				for( int i = 0; i < 1024; i++ ){
					const int step = (int)g_A8L8LightBright[rad][i + 1] - (int)g_A8L8LightBright[rad][i];
					const int absStep = step < 0 ? -step : step;
					if( absStep > maxStep ){
						maxStep = absStep;
						stepRad = rad;
					}
				}
			}
			const bool ok = maxDev <= 10;
			if( !ok ) combosFailed++;
			fprintf( f, "graphType=%d OriginalLightLevels=%d gammaBlend=%.2f : maxDev=%d (rad=%d d=%d) %s ; maxAdjacentStep=%d (rad=%d)\n",
				gt, orig, gammaBlend, maxDev, worstRad, worstD, ok ? "OK" : "FAILED", maxStep, stepRad );
				}
		}
	}
	OriginalLightLevels = savedOriginal;
	fprintf( f, "RESULT: %s\n", combosFailed == 0 ? "ALL COMBOS OK (maxDev <= 10)" : "FAILED" );
	fclose( f );
}

#endif // _DEBUG

//----- (0042C3ED) --------------------------------------------------------
void MakeLightTable()
{
	uchar* tbl = LightTable; // Palettes for 16 light values
	int maxLightLevel = Light4Levels ? 3 : 15;

	for( int palIndex = 0, i = 0; i < maxLightLevel; i++, palIndex += Light4Levels ? 5 : 1 ){
		*tbl++ = 0;
		for( int j = 0; j < 8; j++ ){
			uchar col = 16 * j + palIndex;
			uchar max = 16 * j + 15;
			for( int k = 0; k < 16; k++ ){
				if( k != 0 || j != 0 ) *tbl++ = col;
				if( col < max ){
					col++;
				}else{
					max = 0;
					col = 0;
				}
			}
		}
		for( int j = 16; j < 20; j++ ){
			uchar col = 8 * j + (palIndex >> 1);
			uchar max = 8 * j + 7;
			for( int k = 0; k < 8; k++ ){
				*tbl++ = col;
				if( col < max ){
					col++;
				}else{
					max = 0;
					col = 0;
				}
			}
		}
		for( int j = 10; j < 16; j++ ){
			uchar col = 16 * j + palIndex;
			uchar max = 16 * j + 15;
			for( int k = 0; k < 16; k++ ){
				*tbl++ = col;
				if( col < max ){
					col++;
				}else{
					max = 0;
					col = 0;
				}
				if( col == 255 ){
					max = 0;
					col = 0;
				}
			}
		}
		
	}
	for( int i = 0; i < 256; i++ ) *tbl++ = 0;

	// Init fading for blood
	if( Dungeon->graphType == DT_4_HELL ){
		uchar bloodFadeHold[16];
		tbl = LightTable;
		for( int i = 0; i < maxLightLevel; i++ ){
			int l1 = maxLightLevel - i;
			int l2 = l1;
			int div = maxLightLevel / l1;
			int rem = maxLightLevel % l1;
			int cnt = 0;
			bloodFadeHold[0] = 0;
			uchar col = 1;
			for( int j = 1; j < 16; j++ ){
				bloodFadeHold[j] = col;
				l2 += rem;
				if( l2 > l1 && j < 15 ){
					j++;
					l2 -= l1;
					bloodFadeHold[j] = col;
				}
				cnt++;
				if( cnt == div ){
					col++;
					cnt = 0;
				}
			}
			*tbl++ = 0;
			for( int j =  1; j <= 15; j++ ) *tbl++ = bloodFadeHold[j];
			for( int j = 15; j >   0; j-- ) *tbl++ = bloodFadeHold[j];
			*tbl++ = 1;
			tbl += 224;
		}
		*tbl++ = 0;
		for( int j = 0; j < 31; j++ ){
			*tbl++ = 1;
		}
		tbl += 224;
	}

	if( Dungeon->graphType >= DT_5_CRYPT ){ // Make the lava fully bright ?
		tbl = LightTable;
		for( int i = 0; i < maxLightLevel; i++ ){
			*tbl++ = 0;
			for( int j = CRYPT_BRIGHT_PAL_FIRST; j < CRYPT_BRIGHT_PAL_END; j++ ){
				*tbl++ = j; // identity: always-bright
			}
			tbl += 256 - CRYPT_BRIGHT_PAL_END;
		}
		*tbl++ = 0;
		for( int j = CRYPT_BRIGHT_PAL_FIRST; j < CRYPT_BRIGHT_PAL_END; j++ ){
			*tbl++ = 1; // darkest row: all lava entries collapse to index 1
		}
		tbl += 256 - CRYPT_BRIGHT_PAL_END;
	}

	// Load infra vision table
	uchar* trn = (uchar*)LoadFile("PlrGFX\\Infra.TRN");
	for( int i = 0; i < 256; i++ ) *tbl++ = trn[i];
	FreeMem(trn);

	// Load stone curse table
	trn = (uchar*)LoadFile("PlrGFX\\Stone.TRN");
	for( int i = 0; i < 256; i++ ) *tbl++ = trn[i];
	FreeMem(trn);

	// Create pause table
	constexpr uchar pauseBandStart = 224; // Pause band starting color
	constexpr uchar pauseBandEnd = pauseBandStart + 15; // Pause band ending color
	for( int i = 0; i < 8; i++ ){
		for( uchar col = pauseBandStart + 2; col < pauseBandEnd; col++ ){
			if( i != 0 || col != pauseBandStart + 2 ){
				*tbl++ = col;
			}else{
				*tbl++ = 0;
			}
		}
		*tbl++ = 0;
		*tbl++ = 0;
		*tbl++ = 0;
	}
	for( int i = 0; i < 4; i++ ){
		uchar col = pauseBandStart;
		for( int j = pauseBandStart; j < pauseBandEnd; j += 2 ){
			*tbl++ = col;
			col += 2;
		}
	}
	for( int i = 0; i < 6; i++ ){
		for( uchar col = pauseBandStart; col < pauseBandEnd; col++ ){
			*tbl++ = col;
		}
		*tbl++ = 0;
	}
		
	// Per-radius LightLevels table (lv 0..15 per 1/8-tile distance).  Built by
	// the shared builder above — the single source of truth also used by the
	// Debug self-check reference, so production and reference cannot drift.
	BuildLightLevelsTable( LightLevels, Dungeon->graphType, g_A8L8Lighting );

	// Distance Table
	for( int xOfs = 0; xOfs < 8; xOfs++ ){
		for( int yOfs = 0; yOfs < 8; yOfs++ ){
			for( int y = 0; y < 16; y++ ){
				for( int x = 0; x < 16; x++ ){
					int a = x * 8 - yOfs;
					int b = y * 8 - xOfs;
					double ftmp = (uchar)sqrt((double)( a * a + b * b ));
					ftmp += ftmp < 0 ? -0.5 : 0.5; // round
					LightDist[xOfs][yOfs][x][y] = (uchar)ftmp;
				}
			}
		}
	}
	FillIceDarkenPalette();

	// Point the PPL animation remap at LightTable row 0 for Hell dungeon so that
	// linecpyPerPixel writes an animated (cycling) blood-range index to WorkingSurface.
	// LightTable cycling in AnimateHellPalette keeps this row advancing each frame.
	// For all other dungeons use the identity table; animation goes through ScreenPalette.
	g_PPLAnimRemap = (Dungeon->graphType == DT_4_HELL) ? LightTable : s_PPLIdentityRemap;

	// Build the A8L8+PPL brightness mask.  Default: all entries use the actual per-pixel
	// light level (0xFF = no override).  For crypt/abyss, palette entries 1-15 are the
	// animated lava/water range; the LightTable maps them to themselves in every row
	// (always-bright).  In A8L8 mode the GPU would darken them via LightSurface, so we
	// force their LightSurface byte to 0 (full-bright) at any intermediate light level.
	memset(g_PPLBrightnessMask, 0xFF, 256);
	if (Dungeon->graphType >= DT_5_CRYPT) {
		for (int j = CRYPT_BRIGHT_PAL_FIRST; j < CRYPT_BRIGHT_PAL_END; j++) g_PPLBrightnessMask[j] = 0x00;
	}
	PPL_NoteRemapAndMask( g_PPLAnimRemap == s_PPLIdentityRemap );	// (th4) linecpyPerPixel A8L8 fast path (Draw.cpp)

	// Phase 1 (§3.1 of the A8L8 per-pixel field plan): build the falloff LUT from
	// the A8L8 branch formulas.  The LUT is consumed ONLY by the per-pixel field
	// (V2), so build it only when that path is active — the old A8L8 path and
	// non-A8L8 modes stay untouched.
	if (g_A8L8Lighting && A8L8PixelField) {
		// Render-only falloff shape knob (config "EnrichedLightEnhancedGamma",
		// percent, default 100): 100 = 255(1-f)^2 (original), 0 = 255(1-f)
		// linear.
		// Clamped to [0,1]: outside that range the curve is no longer monotone
		// (which would break the V2 binary-search support radius and the
		// nearest-light envelope).  Only the render LUT uses it — the coarse
		// LightLevels/LightMap stay at gammaBlend=1.0.
		double gammaBlend = (double)A8L8GammaBlendPct / 100.0;
		if( gammaBlend < 0.0 ) gammaBlend = 0.0;
		if( gammaBlend > 1.0 ) gammaBlend = 1.0;
		BuildA8L8LightBrightLut( Dungeon->graphType, LightLevels, gammaBlend );
#ifdef _DEBUG
		// Debug-time self-check: LUT vs a reference LightLevels built with the
		// SAME gammaBlend (production LightLevels stays at 1.0), at tile
		// centers.  Crypt/abyss + OriginalLightLevels verifies the hybrid
		// construction (the render LUT deviates from the literal LightLevels
		// by design there); every other combo verifies full equality.
		uchar refLv[16][128];
		BuildLightLevelsTable( refLv, Dungeon->graphType, true, gammaBlend );
		int worstRad, worstD;
		int maxDev;
		int nonMonotone = 0;
		if( OriginalLightLevels && Dungeon->graphType >= DT_5_CRYPT ){
			maxDev = VerifyA8L8LightBrightLutHybrid( Dungeon->graphType, refLv, &worstRad, &worstD, &nonMonotone );
		}else{
			maxDev = VerifyA8L8LightBrightLut( Dungeon->graphType, refLv, &worstRad, &worstD );
		}
		if( FILE* f = fopen( "A8L8LightBright_selfcheck.txt", "a" ) ){
			fprintf( f, "in-level (graphType=%d OriginalLightLevels=%d gammaBlend=%.2f): maxDev=%d (rad=%d d=%d) nonMonotone=%d %s\n",
				(int)Dungeon->graphType, OriginalLightLevels ? 1 : 0, gammaBlend, maxDev, worstRad, worstD, nonMonotone,
				( maxDev <= 10 && nonMonotone == 0 ) ? "OK" : "FAILED" );
			fclose( f );
		}
#endif
	}
}

// (th4) Rev 2.46 Option C + Rev 2.50: SINGLE winner rule for the colored
// static bake — first `==` match in ObjectActive order against the FINAL
// StaticLightMap (the tile must still hold TINT_WHITE). Used both right after
// fresh generation and on every load path (RebuildStaticTintMap), so a level's
// StaticTintMap is byte-identical freshly generated and after any save/load
// round trip. The Rev 2.46 incremental bake (strict `<` vs the PRE-write
// LightMap) was a DIFFERENT rule: it disagreed with this replay at tiles
// where a colored emitter ties an untinted static contributor (lava/abyss
// bakes) or where one is strictly beaten by the other — fresh levels and
// reloaded levels could tint different tiles. Removed; DoLighting stays the
// only generation-time bake.
// Static lights only: no offsets (the DoLighting lightIndex == -1 path runs
// with xoff == yoff == 0), so LightDist[0][0] and no rotations.
// Returns true when at least one tile was colored — callers latch
// g_StaticTintActive on this so a light that wins nothing never keeps the
// idle gate on by itself.
bool __fastcall DoLightingTintEquals( int row, int col, int radius, uchar tintR, uchar tintG, uchar tintB )
{
	if( ! To112(row, col) ) return false;
	if( (uint) radius > 15 ) radius = 15;
	bool wrote = false;

	const uint colorWord = (uint)tintB | ((uint)tintG << 8) | ((uint)tintR << 16);

	// Center tile — the value path writes the center NON-strictly for non-crypt
	// levels (`LightMap[row][col] = 0` unconditionally) and as a strict min for
	// crypt+. To keep the COLOR winner aligned (first writer wins, `== final
	// value`), the center uses `<=` + first-writer (white check).
	if( LightLevels[radius][0] <= (uchar)StaticLightMap[row][col] && StaticTintMap[row][col] == TINT_WHITE ){
		StaticTintMap[row][col] = colorWord;
		wrote = true;
	}

	int maxleft  = row - 15 < 0           ?           1 + row : 15;
	int maxright = row + 15 > FineMap_112 ? FineMap_112 - row : 15;
	int maxup    = col - 15 < 0           ?           1 + col : 15;
	int maxdown  = col + 15 > FineMap_112 ? FineMap_112 - col : 15;

	int y, x; // quadrant loop vars (captured by the E() lambdas below, same as DoLighting)
	// (th4) GOTCHA fixed 2026-09-06: the loops MUST write the OUTER y/x above,
	// not declare shadowing `int y`/`int x` of their own — the E() lambdas
	// capture the outer variables by reference, so a shadowing declaration
	// left them uninitialized (UB) and d1()/d2() returned garbage. DoLighting
	// already uses the outer vars — keep this mirrored to it.
	auto TintQuadrant = [&]( int max1, int max2, auto d1, auto d2 ){
		for( y = 0; y < max1; y++ ){
			for( x = 1; x < max2; x++ ){
				int dist = LightDist[0][0][x][y];
				if( dist < 128 ){
					int r = row + d1(); if( (uint)r >= FineMap_112 ) continue;
					int c = col + d2(); if( (uint)c >= FineMap_112 ) continue;
					int lightVal = LightLevels[radius][dist];
					if( lightVal == (uchar)StaticLightMap[r][c] && StaticTintMap[r][c] == TINT_WHITE ){
						StaticTintMap[r][c] = colorWord;
						wrote = true;
					}
				}
			}
		}
	};

	#define E(expr) [&]{return expr;}
	TintQuadrant( maxup,    maxright, E( x), E( y) );
	TintQuadrant( maxdown,  maxright, E( y), E(-x) );
	TintQuadrant( maxdown,  maxleft,  E(-x), E(-y) );
	TintQuadrant( maxup,    maxleft,  E(-y), E( x) );
	#undef E
	return wrote;
}

//----- (0042C840) --------------------------------------------------------
void InitLightMax()
{
	LightMax = Light4Levels ? 3 : 15;
}

//----- (0042C856) --------------------------------------------------------
void InitLighting()
{
	LightsCount = 0;
	NeedLightRecalc = 0;
	FullLighting = 0;
	for( int i = 0; i < Lights_128_Count; i++ ){
		LightIndexes[i] = i;
		// (th4) Stage 3: default tint is white (no tint).
		LightTintList[i].r = 255;
		LightTintList[i].g = 255;
		LightTintList[i].b = 255;
	}
}

//----- (0042C874) --------------------------------------------------------
int __fastcall AddLight(int casterRow, int casterCol, int radius)
{
	int lightIndex = -1;
	if( !FullLighting && LightsCount < Lights_128_Count ){
		lightIndex = LightIndexes[LightsCount];
		NeedLightRecalc = 1;
		++LightsCount;
		LightList[lightIndex].row = casterRow;
		LightList[lightIndex].col = casterCol;
		LightList[lightIndex].radius = radius;
		LightList[lightIndex].deltaRow = 0;
		LightList[lightIndex].deltaCol = 0;
		LightList[lightIndex].off = 0;
		LightList[lightIndex].unflag = 0;
		// (th4) Stage 3: every reused slot starts white (no tint); colored
		// lights override this at their AddLight call site.
		LightTintList[lightIndex].r = 255;
		LightTintList[lightIndex].g = 255;
		LightTintList[lightIndex].b = 255;
	}
	return lightIndex;
}

//----- (0042C8E3) --------------------------------------------------------
int __fastcall LightOff(int lightIndex)
{
	int result = 0;
	if( !FullLighting && lightIndex != -1 ){
		result = 1;
		LightList[lightIndex].off = 1;
		NeedLightRecalc = 1;
	}
	return result;
}

//----- (0042C903) --------------------------------------------------------
void __fastcall ChangeLightRadius(int lightIndex, int radius)
{
	if( !FullLighting && lightIndex != -1 ){
		Light& light = LightList[lightIndex];
		light.unflag = 1;
		light.prevRow = light.row;
		light.prevCol = light.col;
		NeedLightRecalc = 1;
		light.prevRadius = light.radius;
		light.radius = radius;
	}
}

//----- (0042C956) --------------------------------------------------------
void __fastcall ChangeLightPos(int lightIndex, int row, int col)
{
	if( !FullLighting && lightIndex != -1 ){
		Light& light = LightList[lightIndex];
		light.unflag = 1;
		NeedLightRecalc = 1;
		light.prevRow = light.row;
		light.prevCol = light.col;
		light.prevRadius = light.radius;
		light.row = row;
		light.col = col;
	}
}

// For increased lighting effect, we specify a position (-8..8,-8..8)
// within each dungeon grid coordinate.  This is deltaRow and deltaCol.
//----- (0042C9B5) --------------------------------------------------------
void __fastcall ChangeLightOffset(int lightIndex, int deltaRow, int deltaCol)
{
	if( !FullLighting && lightIndex != -1 ){
		Light& light = LightList[lightIndex];
		light.unflag = 1;
		light.prevRow = light.row;
		light.prevCol = light.col;
		light.prevRadius = light.radius;
		light.deltaRow = deltaRow;
		light.deltaCol = deltaCol;
		NeedLightRecalc = 1;
	}
}

//----- (0042CA10) --------------------------------------------------------
void __fastcall ChangeLight( int lightIndex, int row, int col, int radius )
{
	Light& light = LightList[lightIndex];
	if( FullLighting || lightIndex == -1 ){
		return;
	}
	light.unflag = 1;
	light.prevRow = light.row;
	light.prevCol = light.col;
	light.prevRadius = light.radius;
	light.row = row;
	light.col = col;
	light.radius = radius;
	NeedLightRecalc = 1;
}

//----- (0042CA7C) --------------------------------------------------------
void ProcessLightList()
{
	if( !FullLighting && NeedLightRecalc ){
		for( int i = 0; i < LightsCount; ++i ){
			int l = LightIndexes[ i ];
			if( LightList[ l ].off ){
				DoUnLight( LightList[ l ].row, LightList[ l ].col, LightList[ l ].radius );
			}
			if( LightList[ l ].unflag ){
				DoUnLight( LightList[ l ].prevRow, LightList[ l ].prevCol, LightList[ l ].prevRadius );
				LightList[ l ].unflag = 0;
			}
		}
		for( int l = 0; l < LightsCount; l++ ){ //  (rt) compute the lighting from moving objects, including the hero
			Light& light = LightList[LightIndexes[l]];
			if( !light.off ){
				DoLighting(light.row, light.col, light.radius, LightIndexes[l]);
			}
		}
		int l = 0;
		while( l < LightsCount ){ // remove off light
			if( LightList[LightIndexes[l]].off ){
				--LightsCount;
				int removedLight = LightIndexes[LightsCount];
				LightIndexes[LightsCount] = LightIndexes[l];
				LightIndexes[l] = removedLight;
			}else{
				l++;
			}
		}
		NeedLightRecalc = 0;
	}
}

//----- (0042CB86) --------------------------------------------------------
void SavePreLighting()
{
	memcpy(StaticLightMap, LightMap, 112*112);
}

//----- (0042CB9E) --------------------------------------------------------
void InitVision()
{
	VisionCount = 0;
	IsDoVision = 0;
	VisionId = 1;
	for( int i = 0; i < TransValue; ++i ) TransList[i] = 0;
}

//----- (0042CBD9) --------------------------------------------------------
int __fastcall AddVision(unsigned int row, int col, int lightRadius, int isCurrentPlayer)
{
	int result;
	if( VisionCount >= 32 ){
		result = lightRadius;
	}else{
		IsDoVision = 1;
		Light& playerVisibility = VisionList[VisionCount];
		result = VisionId++;
		playerVisibility.row = row;
		playerVisibility.col = col;
		playerVisibility.radius = lightRadius;
		playerVisibility.id = result;
		playerVisibility.off = 0;
		playerVisibility.unflag = 0;
		playerVisibility.flag = !(!isCurrentPlayer);
		VisionCount++;
	}
	return result;
}

//----- (0042CC4B) --------------------------------------------------------
void __fastcall ChangeVisionRadius(unsigned int playerIdentificator, unsigned int lightRadius)
{
	for( int i = 0; i < VisionCount; i++ ){
		Light& playerVisibility = VisionList[i];
		if( playerVisibility.id == playerIdentificator ){ // 12
			playerVisibility.unflag = 1;// 20
			playerVisibility.prevRow = playerVisibility.row;// 28 - 0
			playerVisibility.prevCol = playerVisibility.col;// 32 - 4
			playerVisibility.prevRadius = playerVisibility.radius;// 36 - 8
			playerVisibility.radius = lightRadius;// 8
			IsDoVision = 1;
		}
	}
}

//----- (0042CC8C) --------------------------------------------------------
void __fastcall ChangeVision(uint id, uint row, uint col)
{
	for( int i = 0; i < VisionCount; i++ ){
		Light& playerVisibility = VisionList[i];
		if( playerVisibility.id == id ){ // 12
			playerVisibility.unflag = 1;// 20
			playerVisibility.prevRow = playerVisibility.row;// 28 - 0
			playerVisibility.prevCol = playerVisibility.col;// 32 - 4
			playerVisibility.prevRadius = playerVisibility.radius;// 36 - 8
			playerVisibility.row = row;
			playerVisibility.col = col;
			IsDoVision = 1;
		}
	}
}

//----- (0042CCD6) --------------------------------------------------------
void ProcessVisionList()
{
	int loIndex;           // ebp@1
	int hiIndex;           // edx@2
	//int v2;           // edi@2
	//int* v3;          // esi@3
	//int v4;           // ecx@10
	//char* v5;         // edi@10
	//int v6;           // edi@13
	//int* v7;          // esi@14
	int someFlag;           // ecx@18
	const void* hiStruct;    // eax@19
	Light* loStruct;    // ebx@19
	//char v11;         // si@10
	//unsigned int v12; // ecx@10
	loIndex = 0;
	if( IsDoVision ){
		
		for( int i = 0; i < VisionCount; i++ ){
			Light& playerVisibility = VisionList[i];
			if( playerVisibility.off ){ // 16
				DoUnVision(playerVisibility.row, playerVisibility.col, playerVisibility.radius);
			}

			if( playerVisibility.unflag ){ // 20
				DoUnVision(playerVisibility.prevRow, playerVisibility.prevCol, playerVisibility.prevRadius);
				playerVisibility.unflag = 0;
			}
		}
		if( (char)TransValue > 0 ){// (rt) Unclear construct; to be examined in action later
			memset(TransList, 0, TransValue);
		}
		for( int i = 0; i < VisionCount; i++ ){
			Light& playerVisibility = VisionList[i];
			if( !playerVisibility.off ){ // 16
				DoVision(playerVisibility.row, playerVisibility.col, playerVisibility.radius, playerVisibility.flag & 1, playerVisibility.flag & 1);// 0 4 8 48 48
			}
		}
		hiIndex = VisionCount;
		do{
			someFlag = 0;
			if( hiIndex > 0 ){
				loStruct = VisionList;
				hiStruct = &VisionList[hiIndex];
				do{
					if( loStruct->off ){
						hiIndex--;
						hiStruct = (char*)hiStruct - 52;
						if( hiIndex > 0 && loIndex != hiIndex ){
							memcpy(loStruct, hiStruct, 52);
						}
						someFlag = 1;
					}
					++loIndex;
					++loStruct;
				}while( loIndex < hiIndex );
				loIndex = 0;
			}
		}while( someFlag != 0 );
		VisionCount = hiIndex;
	}
	IsDoVision = 0;
}

//----- (0042CDD7) --------------------------------------------------------
void AnimateHellPalette() // BloodCycle()
{
	// Cycle the blood
//	uchar* tbl = LightTable;
//	int lightLevels = Light4Levels ? 4 : 16;
//	for( int i = 0; i < lightLevels; i++ ){
//		tbl++;
//		uchar h = *tbl;
//		for( int k = 0; k < 30; k++ ){
//			*tbl = *(tbl+1);
//			*tbl++;
//		}
//		*tbl = h;
//		tbl += 225;
//	}

	// Cycle the blood-fade LightTable rows (entries 1..31 in each row).
	// This drives both PPL and non-PPL blood animation correctly:
	//
	//   Non-PPL: linecpy writes LightTable[lv*256+sv] (a darkened index k).
	//     LightTable cycling → k advances each frame → ScreenPalette[k] shows the
	//     correct animated blood colour at the correct brightness level.  No
	//     oscillation because ScreenPalette itself is not cycled.
	//
	//   PPL:  linecpyPerPixel writes g_PPLAnimRemap[sv] = LightTable[sv] (row 0)
	//     to WorkingSurface, with the actual light level lv in LightSurface.
	//     LightTable cycling → LightTable[sv] advances each frame → WorkingSurface
	//     holds an animated index → GPU shows ScreenPalette[index]*brightness(lv).
	//
	// This is the original Hellfire blood-cycle mechanism, now extended to cover PPL.
	uchar* tbl = LightTable;
	int lightLevels = Light4Levels ? 4 : 16;
	for( int i = 0; i < lightLevels; i++ ){
		tbl++; // skip entry 0 (always 0)
		uchar h = *tbl; // save entry 1
		for( int k = 0; k < 30; k++ ){
			*tbl = *(tbl + 1); // shift entry k+2 → k+1
			tbl++;
		}
		*tbl = h; // entry 31 = old entry 1
		tbl += 225; // advance past the remaining 224 non-blood entries to next row
	}
}
