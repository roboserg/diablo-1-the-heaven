#ifndef _engine_h_
#define _engine_h_
#else
#error reinclude engine.h
#endif

using filesystem::rename;
using filesystem::exists;
using filesystem::copy_file;
using filesystem::copy_options;

#pragma warning(disable: 4102)

template<typename T, size_t N> __forceinline void* memzero(T (&dst)[N]){ return memset( dst, 0, sizeof(dst)); }
template<typename T, size_t N, size_t M> __forceinline void* memzero(T (&dst)[N][M]){ return memset( dst, 0, sizeof(dst)); }

// milliseconds to engine time interval
constexpr i64 operator"" _ms( u64 n ){ return n * MAX_FPS / 1000; }
// engine time interval to milliseconds
__forceinline int ms( i64 n ){ return int(n / (MAX_FPS / 1000)); }
// if game clock and the real clock differ by more than CatchUpDelta
// then reset the game clock to be equal to the real clock.  This ensures
// that the game will attempt to maintain a normal frame rate, but if it
// falls too far outside normal bounds, it will not struggle indefinitely.
constexpr i64 CatchUpDelta_500 = 500_ms;
constexpr i64 MIN_REPLAY_THRESHOLD = 80_ms;
enum { MAX_PROGRESS = 534 };
inline const char UTF8_BOM[] = "\xEF\xBB\xBF";

inline void SleepTh(int ms   ){ if( ms   > 0 ) Sleep(ms); }
inline void SleepTh(i64 tick ){ if( tick > 0 ) Sleep(ms(tick)); }

// (th4) A8L8 ALT-highlight outline brightness floor (LightSurface units:
// 0 = full bright, 255 = full dark).  Outline ring pixels darker than this are
// lifted to this value so the yellow ALT ring stays visible in Thief-mode pitch
// darkness, without full-brightening the object body underneath.
//const uchar kAltHighlightBright = 127;
const uchar kAltHighlightBright = 0;

// refactor and merge all of .cel rle drawing function, fix decompilation copy/paste potential bugs
//----- (00418BE0) -------------------------------------------------------- interface
template<int draw = R_DIRECT> void __fastcall RleDraw(uchar* dst, char* src, int celSize, int width, uchar outlineColor = 0)
{
	if( dst && src ){
		char* cel = src;
		// https://github.com/savagesteel/d1-file-formats/blob/master/PC-Mac/CEL.md
		if( is(*(short*)cel, 10, 12) ){ // optional header for pixels offset
			cel += *cel;
		}
		char* end = &src[celSize];
		int widLeft = 0, len = 0;
		[[maybe_unused]] uchar* pal;
		[[maybe_unused]] int srcY;
		[[maybe_unused]] int srcX;
		// Approach B (A8L8): precompute constant offset from any WorkingSurface
		// pointer to the corresponding LightSurface position.
		// Adding lightOfs to a dst-derived pointer gives the LightSurface address.
		// Cleared to 0 so the shader does not further darken pre-darkened pixels.
//		const bool hasLight = LightSurface && SoftSrcSurfacePtr &&
		const bool hasLight = g_A8L8Lighting && LightSurface && SoftSrcSurfacePtr &&
			(uintptr_t)dst >= (uintptr_t)SoftSrcSurfacePtr;
		const intptr_t lightOfs = hasLight ? (intptr_t)LightSurface - (intptr_t)SoftSrcSurfacePtr : 0;
		// (th4) Stage 2: R_OVERLAY_CLEAR pass (cursor only) — zero the overlay pair
		// under every pixel this CEL writes. The first term is a template constant,
		// so every other instantiation compiles the whole block out.
		const bool hasOverlay = (draw & R_OVERLAY_CLEAR) && g_ARGBLighting && EnrichedLightUltraAlpha != 0
			&& AlphaSurface && SpriteLayerSurface && SoftSrcSurfacePtr
			&& (uintptr_t)dst >= (uintptr_t)SoftSrcSurfacePtr;
		// (th4) Stage 3: R_TINT_CLEAR pass — write TINT_WHITE into LightTintSurface
		// exactly under the pixels this CEL writes (menu art, dialog boxes, menu
		// glyphs). UI ink drawn over the playfield must never be multiplied by the
		// world's colored light; transparent CEL cells are skip-runs, so the tint
		// keeps showing through the holes. Idle-gated: with no colored light active
		// the s3 upload is skipped entirely, so the pass is free to no-op.
		const bool hasTintClear = (draw & R_TINT_CLEAR) && g_ARGBLighting && g_LightTintActive
			&& LightTintSurface && SoftSrcSurfacePtr
			&& (uintptr_t)dst >= (uintptr_t)SoftSrcSurfacePtr;
		// (th4) Stage 3: the ring pixels of ALT/hover outlines must never be
		// tinted either — the Stage-3 analog of the ring-light lift below.
		// The outline color sits in WorkingSurface and its max brightness comes
		// from the lifted LightSurface byte; the tint word is a separate channel
		// (LightTintSurface) the lift never touched, so it still holds the
		// world's winning light color. Reset it wherever a ring pixel is
		// detected (the same `== outlineColor` test the lift uses). The
		// template constant compiles this block out of every other RleDraw
		// instantiation; runtime-gated on an active colored light.
		const bool hasRingTint = (draw & R_LIGHT_OUTLINE) && g_ARGBLighting && g_LightTintActive
			&& LightTintSurface && SoftSrcSurfacePtr
			&& (uintptr_t)dst >= (uintptr_t)SoftSrcSurfacePtr;
		#ifdef HELLFIRE_ORIGINAL
		int palTab = Light4Levels ? 1024 : 4096;
		if (LIGHT_TYPE == LIGHT_STONE){
			palTab += 256;
		}else if (LIGHT_TYPE >= LIGHT_U){
			palTab += ((LIGHT_TYPE - LIGHT_U) << 8) + 768;
		}
		pal = LightTable + palTab;
		#endif
		IF( draw & R_DARK ) pal = &LightTable[256 * DarkLevel]; // max 15
		IF( draw & R_RED  ) pal = &LightTable[256 * 16]; // pal 16
		IF( draw & R_TRANS ){ srcY = 0; srcX = 0; }
		do{
			if( (size_t)dst < (size_t)WorkingSurfaceDrawBegin ){ return; }
			uchar* dstLineStart = dst;
			widLeft = width;
			do{
				while( 1 ){
					len = *cel++;
					if( len > 0 ){
						break;
					}
					len = -len;
					dst += len;
					IF( draw & R_TRANS ){ srcX += len; }
					widLeft -= len;
					if( widLeft <= 0 ){
						goto next_line;
					}
				}
				IF( draw & R_CLIP ) if( dst >= WorkingSurfaceDrawEnd ) goto skip_draw;
				IF( draw & R_OUTLINE ){
					IF(draw & R_MIRROR) {
						uchar* d = dstLineStart + widLeft - 1;
						for (int i = 0; i < len; ++i, --d) if (cel[i]) *(d - WorkingWidth) = *(d - 1) = *(d + 1) = *(d + WorkingWidth) = outlineColor;
					}
					else {
						uchar* d = dst;
						for (int i = 0; i < len; ++i, ++d) if (cel[i]) *(d - WorkingWidth) = *(d - 1) = *(d + 1) = *(d + WorkingWidth) = outlineColor;
					}
					// If NOT commented out, ALT highlight in A8L8 mode will make object full bright even deep in the shadows
//					// Outline writes to neighbor positions; clear their LightSurface bytes too.  This only works for legacy A8L8 behavior without screen space PPL
//					if( hasLight ){
//						IF( draw & R_MIRROR ){
//							uchar* d = dstLineStart + widLeft - 1;
//							for( int i = 0; i < len; ++i, --d ) if( cel[i] ){
//								*(uchar*)((intptr_t)(d - WorkingWidth) + lightOfs) = 0;
//								*(uchar*)((intptr_t)(d - 1)            + lightOfs) = 0;
//								*(uchar*)((intptr_t)(d + 1)            + lightOfs) = 0;
//								*(uchar*)((intptr_t)(d + WorkingWidth) + lightOfs) = 0;
//							}
//						}else{
//							uchar* d = dst;
//							for( int i = 0; i < len; ++i, ++d ) if( cel[i] ){
//								*(uchar*)((intptr_t)(d - WorkingWidth) + lightOfs) = 0;
//								*(uchar*)((intptr_t)(d - 1)            + lightOfs) = 0;
//								*(uchar*)((intptr_t)(d + 1)            + lightOfs) = 0;
//								*(uchar*)((intptr_t)(d + WorkingWidth) + lightOfs) = 0;
//							}
//						}
//					}
					// Outline writes to neighbor positions only in WorkingSurface.
					// Do NOT modify LightSurface — the outline is a pure overlay, and
					// modifying LightSurface would change the brightness of the object
					// underneath, which is not pre-branch behavior.
				}ELIF( draw & R_LIGHT_OUTLINE ){
					// (th4) Light-only pass run AFTER the body draw (callers run it last).
					// The outline COLOR was written earlier and the body has overwritten
					// every interior outline-colored pixel, so any pixel that still equals
					// outlineColor is a true ring pixel. Lift ONLY those: the object body
					// keeps its ambient light, so a highlighted object behind an arch can
					// no longer imprint a body glow on the foreground arch (the thin ring
					// halo is removed by the arch's pre-fill light restore).
					if( hasLight ){
						IF( draw & R_MIRROR ){
							uchar* d = dstLineStart + widLeft - 1;
							for( int i = 0; i < len; ++i, --d ) if( cel[i] ){
								uchar* lp = (uchar*)((intptr_t)(d - WorkingWidth) + lightOfs);
								if( *(d - WorkingWidth) == outlineColor ){
									if( *lp > kAltHighlightBright ) *lp = kAltHighlightBright;
									if( hasRingTint ) LightTintSurface[(d - WorkingWidth) - SoftSrcSurfacePtr] = TINT_WHITE;
								}
								lp = (uchar*)((intptr_t)(d - 1) + lightOfs);
								if( *(d - 1) == outlineColor ){
									if( *lp > kAltHighlightBright ) *lp = kAltHighlightBright;
									if( hasRingTint ) LightTintSurface[(d - 1) - SoftSrcSurfacePtr] = TINT_WHITE;
								}
								lp = (uchar*)((intptr_t)(d + 1) + lightOfs);
								if( *(d + 1) == outlineColor ){
									if( *lp > kAltHighlightBright ) *lp = kAltHighlightBright;
									if( hasRingTint ) LightTintSurface[(d + 1) - SoftSrcSurfacePtr] = TINT_WHITE;
								}
								lp = (uchar*)((intptr_t)(d + WorkingWidth) + lightOfs);
								if( *(d + WorkingWidth) == outlineColor ){
									if( *lp > kAltHighlightBright ) *lp = kAltHighlightBright;
									if( hasRingTint ) LightTintSurface[(d + WorkingWidth) - SoftSrcSurfacePtr] = TINT_WHITE;
								}
							}
						}else{
							uchar* d = dst;
							for( int i = 0; i < len; ++i, ++d ) if( cel[i] ){
								uchar* lp = (uchar*)((intptr_t)(d - WorkingWidth) + lightOfs);
								if( *(d - WorkingWidth) == outlineColor ){
									if( *lp > kAltHighlightBright ) *lp = kAltHighlightBright;
									if( hasRingTint ) LightTintSurface[(d - WorkingWidth) - SoftSrcSurfacePtr] = TINT_WHITE;
								}
								lp = (uchar*)((intptr_t)(d - 1) + lightOfs);
								if( *(d - 1) == outlineColor ){
									if( *lp > kAltHighlightBright ) *lp = kAltHighlightBright;
									if( hasRingTint ) LightTintSurface[(d - 1) - SoftSrcSurfacePtr] = TINT_WHITE;
								}
								lp = (uchar*)((intptr_t)(d + 1) + lightOfs);
								if( *(d + 1) == outlineColor ){
									if( *lp > kAltHighlightBright ) *lp = kAltHighlightBright;
									if( hasRingTint ) LightTintSurface[(d + 1) - SoftSrcSurfacePtr] = TINT_WHITE;
								}
								lp = (uchar*)((intptr_t)(d + WorkingWidth) + lightOfs);
								if( *(d + WorkingWidth) == outlineColor ){
									if( *lp > kAltHighlightBright ) *lp = kAltHighlightBright;
									if( hasRingTint ) LightTintSurface[(d + WorkingWidth) - SoftSrcSurfacePtr] = TINT_WHITE;
								}
							}
						}
					}
				}ELIF( draw & R_OVERLAY_CLEAR ){
					// (th4) Stage 2: clear the alpha overlay exactly under the pixels this
					// CEL writes — used ONLY by the cursor pass (RenderCursor). The cursor
					// CEL's transparent cells are skip-runs, so this zeroes just the cursor
					// silhouette and leaves the ghost visible around it; a bounding-rect
					// clear would instead show a wall-colored square.
					if (hasOverlay) {
						IF( draw & R_MIRROR ){
							uchar* d = dstLineStart + widLeft - 1;
							for( int i = 0; i < len; ++i, --d ){
								AlphaSurface[d - SoftSrcSurfacePtr] = 0;
								SpriteLayerSurface[d - SoftSrcSurfacePtr] = 0;
							}
						}else{
							for( int i = 0; i < len; ++i ){
								AlphaSurface[(dst + i) - SoftSrcSurfacePtr] = 0;
								SpriteLayerSurface[(dst + i) - SoftSrcSurfacePtr] = 0;
							}
						}
					}
				}ELIF( draw & R_TINT_CLEAR ){
					// (th4) Stage 3: clear the colored-light tint exactly under the pixels
					// this CEL writes — the Stage-3 analog of the R_OVERLAY_CLEAR pass.
					// Used by Surface_DrawCEL and Font_DrawChar so menu art and menu
					// glyphs never inherit the world's winning light color, while the
					// transparent cells keep the tint visible around the ink.
					if (hasTintClear) {
						IF( draw & R_MIRROR ){
							uchar* d = dstLineStart + widLeft - 1;
							for( int i = 0; i < len; ++i, --d ){
								LightTintSurface[d - SoftSrcSurfacePtr] = TINT_WHITE;
							}
						}else{
							for( int i = 0; i < len; ++i ){
								LightTintSurface[(dst + i) - SoftSrcSurfacePtr] = TINT_WHITE;
							}
						}
					}
				}ELIF( draw & R_MIRROR ){
					uchar* d = dstLineStart + widLeft - 1;
					IF( draw & R_PAL ){
						IF( draw & R_TRANS ){
//							if (g_A8L8Lighting && PixelLight && (draw & R_DARK)) {
//								// A8L8: skip gamma-2 LightTable, use linear LightSurface
//								uchar lf = (uchar)(DarkLevel * 17 > 255 ? 255 : DarkLevel * 17);
//								for( int i = 0; i < len; ++i, --d, ++srcX ){
//									if( bool t = srcX % 2; srcY % 2 ? t : !t ){
//										*d = (uchar)cel[i];
//										if(hasLight) *(uchar*)((intptr_t)d + lightOfs) = lf;
//									}
//								}
//							} else {
//								for( int i = 0; i < len; ++i, --d, ++srcX ){
//									if( bool t = srcX % 2; srcY % 2 ? t : !t ){
//										*d = pal[(uchar)cel[i]];
//										if(hasLight) *(uchar*)((intptr_t)d + lightOfs) = 0;
//									}
//								}
//							}
							if (g_A8L8Lighting && PixelLight && g_InDungeonRender && (draw & R_DARK)) {
								// Screen-space PPL: skip LightSurface write, pre-fill handles it
								for (int i = 0; i < len; ++i, --d, ++srcX) {
									if (bool t = srcX % 2; srcY % 2 ? t : !t) {
										*d = (uchar)cel[i];
									}
								}
							}
							else {
								for (int i = 0; i < len; ++i, --d, ++srcX) {
									if (bool t = srcX % 2; srcY % 2 ? t : !t) {
										*d = pal[(uchar)cel[i]];
										if (hasLight && !g_SkipSpriteLightClear) *(uchar*)((intptr_t)d + lightOfs) = 0;
									}
								}
							}
						}else{
							// Commented out is the legacy A8L8 logic, inferior approach that was superseded when FillScreenSpaceLighting was introduced.
							// Its per-sprite DarkLevel*17 LightSurface writes would degrade the bilinear pre-fill quality and create lighting discontinuities.
//							if (g_A8L8Lighting && PixelLight && (draw & R_DARK)) {
//								// A8L8: skip gamma-2 LightTable, use linear LightSurface
//								for( int i = 0; i < len; ++i, --d ) *d = cel[i];
//								if(hasLight) {
//									uchar lf = (uchar)(DarkLevel * 17 > 255 ? 255 : DarkLevel * 17);
//									memset((uchar*)((intptr_t)(dstLineStart + widLeft - len) + lightOfs), lf, len);
//								}
//							} else {
//								for( int i = 0; i < len; ++i, --d ) *d = pal[(uchar)cel[i]];
//								if(hasLight) memset((uchar*)((intptr_t)(dstLineStart + widLeft - len) + lightOfs), 0, len);
//							}
							if (g_A8L8Lighting && PixelLight && g_InDungeonRender && (draw & R_DARK)) {
								// Screen-space PPL: skip LightSurface write, pre-fill handles it
								for( int i = 0; i < len; ++i, --d ) *d = cel[i];
							} else {
								for( int i = 0; i < len; ++i, --d ) *d = pal[(uchar)cel[i]];
								if(hasLight && !g_SkipSpriteLightClear) memset((uchar*)((intptr_t)(dstLineStart + widLeft - len) + lightOfs), 0, len);
							}
							// (th4) track wall occlusion: SET for arches, CLEAR for normal sprites.
							// R_MIRROR path: pixels are written at mirrored positions via 'd',
							// so use (d+1) as the leftmost mirrored pixel, not 'dst'.
							if (WallOcclusionSurface) {
								memset(WallOcclusionSurface + ((d + 1) - SoftSrcSurfacePtr), g_IsDrawingOpaqueWall ? WOB_WALL : (g_IsDrawingOpaqueObject ? WOB_OBJECT : 0), len);
							}
						}
					}else{
//						for( int i = 0; i < len; ++i, --d ) *d = cel[i];
//						if(hasLight) memset((uchar*)((intptr_t)(dstLineStart + widLeft - len) + lightOfs), 0, len);
						if (g_A8L8Lighting && PixelLight && g_InDungeonRender && !g_ForceSpriteLightClear) {
							// Screen-space PPL: skip LightSurface write, pre-fill handles it
							for( int i = 0; i < len; ++i, --d ) *d = cel[i];
						} else {
							for( int i = 0; i < len; ++i, --d ) *d = cel[i];
							if (hasLight && !g_SkipSpriteLightClear) {
								memset((uchar*)((intptr_t)(dstLineStart + widLeft - len) + lightOfs), 0, len);
							}
						}
						// (th4) track wall occlusion for all sprites rendered via this path.
						// R_MIRROR path: pixels are written at mirrored positions via 'd',
						// so use (d+1) as the leftmost mirrored pixel, not 'dst'.
						if (WallOcclusionSurface) {
							memset(WallOcclusionSurface + ((d + 1) - SoftSrcSurfacePtr), g_IsDrawingOpaqueWall ? WOB_WALL : (g_IsDrawingOpaqueObject ? WOB_OBJECT : 0), len);
						}
					}
				}else{
					IF( draw & R_PAL ){
						IF( draw & R_TRANS ){
//							if (g_A8L8Lighting && PixelLight && (draw & R_DARK)) {
//								// A8L8: skip gamma-2 LightTable, use linear LightSurface
//								uchar lf = (uchar)(DarkLevel * 17 > 255 ? 255 : DarkLevel * 17);
//								for( int i = 0; i < len; ++i, ++srcX ){
//									if( bool t = srcX % 2; srcY % 2 ? t : !t ){
//										dst[i] = (uchar)cel[i];
//										if(hasLight) *(uchar*)((intptr_t)(dst + i) + lightOfs) = lf;
//									}
//								}
//							} else {
//								for( int i = 0; i < len; ++i, ++srcX ){
//									if( bool t = srcX % 2; srcY % 2 ? t : !t ){
//										dst[i] = pal[(uchar)cel[i]];
//										if(hasLight) *(uchar*)((intptr_t)(dst + i) + lightOfs) = 0;
//									}
//								}
//							}
							if (g_A8L8Lighting && PixelLight && g_InDungeonRender && (draw & R_DARK)) {
								// Screen-space PPL: skip LightSurface write, pre-fill handles it
								for (int i = 0; i < len; ++i, ++srcX) {
									if (bool t = srcX % 2; srcY % 2 ? t : !t) {
										dst[i] = (uchar)cel[i];
									}
								}
							} else {
								for (int i = 0; i < len; ++i, ++srcX) {
									if (bool t = srcX % 2; srcY % 2 ? t : !t) {
										dst[i] = pal[(uchar)cel[i]];
										if (hasLight && !g_SkipSpriteLightClear) *(uchar*)((intptr_t)(dst + i) + lightOfs) = 0;
									}
								}
							}
						}else{
//							if (g_A8L8Lighting && PixelLight && (draw & R_DARK)) {
//								// A8L8: skip gamma-2 LightTable, use linear LightSurface
//								memcpy(dst, cel, len);
//								if(hasLight) {
//									uchar lf = (uchar)(DarkLevel * 17 > 255 ? 255 : DarkLevel * 17);
//									memset((uchar*)((intptr_t)dst + lightOfs), lf, len);
//								}
//							} else {
//								for( int i = 0; i < len; ++i ) dst[i] = pal[(uchar)cel[i]];
//								if(hasLight) memset((uchar*)((intptr_t)dst + lightOfs), 0, len);
//							}
							if (g_A8L8Lighting && PixelLight && g_InDungeonRender && (draw & R_DARK)) {
								// Screen-space PPL: skip LightSurface write, pre-fill handles it
								memcpy(dst, cel, len);
								// (th4) Arch tiles only (g_IsDrawingOpaqueWall): restore the pre-fill
								// ambient light so an ALT-highlighted outline ring behind the arch
								// cannot brighten it through LightSurface. Matches how linecpyPerPixel
								// lights opaque wall tiles (reads LightSurfaceBase).
								if (g_IsDrawingOpaqueWall && hasLight && LightSurfaceBase) {
									memcpy((uchar*)((intptr_t)dst + lightOfs), LightSurfaceBase + (dst - SoftSrcSurfacePtr), len);
								}
							}
							else {
								for (int i = 0; i < len; ++i) dst[i] = pal[(uchar)cel[i]];
								if (hasLight && !g_SkipSpriteLightClear) memset((uchar*)((intptr_t)dst + lightOfs), 0, len);
							}
							// (th4) track wall occlusion: SET for arches, CLEAR for normal sprites
							if (WallOcclusionSurface) {
								memset(WallOcclusionSurface + (dst - SoftSrcSurfacePtr), g_IsDrawingOpaqueWall ? WOB_WALL : (g_IsDrawingOpaqueObject ? WOB_OBJECT : 0), len);
							}
						}
					}else{
//						memcpy( dst, cel, len );
//						if(hasLight) memset((uchar*)((intptr_t)dst + lightOfs), 0, len);
						if (g_A8L8Lighting && PixelLight && g_InDungeonRender && !g_ForceSpriteLightClear) {
							// Screen-space PPL: skip LightSurface write, pre-fill handles it
							memcpy(dst, cel, len);
							// (th4) Arch tiles only (g_IsDrawingOpaqueWall): restore pre-fill
							// ambient light (see the R_DARK branch above).
							if (g_IsDrawingOpaqueWall && hasLight && LightSurfaceBase) {
								memcpy((uchar*)((intptr_t)dst + lightOfs), LightSurfaceBase + (dst - SoftSrcSurfacePtr), len);
							}
						}
						else {
							memcpy(dst, cel, len);
							if (hasLight && !g_SkipSpriteLightClear) {
								memset((uchar*)((intptr_t)dst + lightOfs), 0, len);
							}
						}
						// (th4) track wall occlusion: SET for arches, CLEAR for normal sprites
						if (WallOcclusionSurface) {
							memset(WallOcclusionSurface + (dst - SoftSrcSurfacePtr), g_IsDrawingOpaqueWall ? WOB_WALL : (g_IsDrawingOpaqueObject ? WOB_OBJECT : 0), len);
						}
					}
				}
				skip_draw:
				widLeft -= len;
				cel += len;
				dst += len;
				IF( draw & R_TRANS ){ srcX += len; }
			}while( widLeft > 0 );
		next_line: dst -= width - widLeft + WorkingWidth;
		IF( draw & R_TRANS ){ ++srcY; srcX = 0; }
		}while( cel < end );
	}
}

int RandSound(int range);
int RandSound(int soundMin, int soundMax);

// get non gameplay random from parameters --------------------------------
template<typename... T> __forceinline auto RandFrom(T... v)
{
	common_type_t<T...> val[] = {v ...};
	return val[rand() % countof(val)];
}

// get gameplay random from parameters ------------------------------------
template<typename... T> __forceinline auto RngFrom(T... v)
{
	common_type_t<T...> val[] = {v ...};
	return val[RNG(countof(val))];
}

//----- (th3) -------------------------------------------------------------
template<size_t N, typename I> int PrintBigNum(char (&outBuf)[N], I val, char separator = 0, int digitLimit = 0)
{
	char digitBuf[32]; //18446744073709551615 -> 20 chars
	char formatedBuf[32]; //18'446'744'073'709'551'615 -> 26 chars
	if( digitLimit && val < 0 ) ++digitLimit;
	int tempOffset = countof(digitBuf) - 1;
	char* buf = formatedBuf;
	buf[tempOffset] = 0;
	const char* format = "%llu";
	IS( I, i64 )format = "%lld";
	IS( I, int )format = "%d";
	IS( I, uint)format = "%u";
	int charCount = snprintf(digitBuf, countof(digitBuf), format, val);
	if( digitLimit && charCount > digitLimit && charCount > 3){
		static const char suffix[] = {'K','M','B','T','Q'};
		int suffixIndex = 0;
		--tempOffset;
		while( charCount > digitLimit && charCount > 3 && suffixIndex < countof(suffix)){
			charCount -= 3;
			digitBuf[charCount] = 0;
			buf[tempOffset] = suffix[suffixIndex++];
		}
	}
	--charCount;
	for( int pos = 1; charCount >= 0; --charCount, ++pos ){
		buf[--tempOffset] = digitBuf[charCount];
		if( pos % 3 == 0 && charCount > 0 && separator){
			buf[--tempOffset] = separator; //inject the separator
		}
	}
	buf += tempOffset;
	return sprintf(outBuf, "%s", buf);
}

// versioning struct size
enum Game_Param: ushort { CurSaveVersion = 30, CurItemGenVersion = 7 }; // (rt) on release the version archive can be cleared and loading old versions forbidden, or a release save identifier 'HEL3' introduced
struct StructVersion { ushort num; size_t size; };
template<typename T> constexpr size_t StructSize(ushort version)
{
	size_t size = 0;
	for( auto& v: T::versions ){
		size = v.size;
		if( version >= v.num ) break;
	}
	return size;
}
