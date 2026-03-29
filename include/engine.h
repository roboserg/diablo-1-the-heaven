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
					IF( draw & R_MIRROR ){
						uchar* d = dstLineStart + widLeft - 1;
						for( int i = 0; i < len; ++i, --d ) if( cel[i] ) *(d - WorkingWidth) = *(d - 1) = *(d + 1) = *(d + WorkingWidth) = outlineColor;
					}else{
						uchar* d = dst;
						for( int i = 0; i < len; ++i, ++d ) if( cel[i] ) *(d - WorkingWidth) = *(d - 1) = *(d + 1) = *(d + WorkingWidth) = outlineColor;
					}
				}ELIF( draw & R_MIRROR ){
					uchar* d = dstLineStart + widLeft - 1;
					IF( draw & R_PAL ){
						IF( draw & R_TRANS ){
							for( int i = 0; i < len; ++i, --d, ++srcX ){ if( bool t = srcX % 2; srcY % 2 ? t : !t ) *d = pal[(uchar)cel[i]]; }
						}else{
							for( int i = 0; i < len; ++i, --d ) *d = pal[(uchar)cel[i]];
						}
					}else{
						for( int i = 0; i < len; ++i, --d ) *d = cel[i];
					}
				}else{
					IF( draw & R_PAL ){
						IF( draw & R_TRANS ){
							for( int i = 0; i < len; ++i, ++srcX ){ if( bool t = srcX % 2; srcY % 2 ? t : !t ) dst[i] = pal[(uchar)cel[i]]; }
						}else{
							for( int i = 0; i < len; ++i ) dst[i] = pal[(uchar)cel[i]];
						}
					}else{
						memcpy( dst, cel, len );
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
enum Game_Param: ushort { CurSaveVersion = 27, CurItemGenVersion = 7 }; // при релизе архив версий можно будет очиситить и запретить загружать старые версии, либо сделать релизный идентификатор сейвов 'HEL3'
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
