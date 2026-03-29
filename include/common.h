#ifndef _common_h_
#define _common_h_
#else
#error reinclude common.h
#endif

#define IF(...) if constexpr(bool(__VA_ARGS__))
#define ELIF(...) else if constexpr(bool(__VA_ARGS__))
template<class T, class... Ts> struct is_any: bool_constant<(is_same_v<T, Ts> || ...)> {};
#define IS(...) if constexpr( is_any<__VA_ARGS__>::value )

// test intervals
#define InSegment( x, min_x, max_x )   ( (x) >= (min_x) && (x) <= (max_x) )
#define InInterval( x, min_x, max_x )   ( (x) > (min_x) && (x) < (max_x) )

// convert row40 & col40 to row112 & col112
#define Convert40to112( x40 )   ( 2 * (x40) + MapDirty_16 )

template<typename T> __forceinline void FreeMemZero( T& ptr ) { void* oldAddr = ptr; ptr = 0; FreeMem(oldAddr); }
template<typename T> __forceinline void ReleaseZero( T& ptr ) { if( ptr ){ ptr->Release(); ptr = 0; } }

// Если число больше максимума, присваеваем ему максимум
template<class T, class V> __forceinline void LimitToMax( T& n, V max )
{
	if( n > max ) n = max;
}
// Если число меньше минимума, присваеваем ему минимум
template<class T, class V> __forceinline void LimitToMin( T& n, V min )
{
	if( n < min ) n = min;
}
// Ограничиваем число сверху и снизу
template<class T, class Vmin, class Vmax> __forceinline void LimitToRange( T& n, Vmin min, Vmax max)
{
	if( n < min ) n = min;
	else if( n > max ) n = max;
}

// если внутри есть вызов функции не th2-only и это не оптимизация оригинала
// то выносим в таблицу и ставим адрес th2
// Заворачиваем число вокруг диапазона [0; period - 1]
// этой функцией пытались заменить несколько разных операций, с возможным нарушением логики, проверить все вызовы
__forceinline void Wrap(int& n, int period)
{
	if( n >= period ){
		n -= period;
	}else if( n < 0 ){
		n += period;
	}
	// В оригинале нет подобных проверок, более того есть логика в которой явно требуется отрицательный результат
	// а если нужно обрезать любые числа то это можно делать так: (uint&)n %= period - 1 ;
	//#ifdef _DEBUG
	//if( !InSegment(n, 0, period - 1) ){
	//	TerminateWithError("Wrap illegal period (%d) or value (%d)", period, n);
	//}
	//#endif
}

// checking out of map access
template<typename... T> __forceinline bool In40 (T...  coord){ return (... && (uint(coord) < Map_40     )); }
template<typename... T> __forceinline bool In112(T...  coord){ return (... && (uint(coord) < FineMap_112)); }
template<typename... T> __forceinline bool To40 (T...  coord){ return (... && (uint(coord - 1) < Map_40      - 2)); }
template<typename... T> __forceinline bool To112(T...  coord){ return (... && (uint(coord - 1) < FineMap_112 - 2)); }
template<typename... T> __forceinline void Limit40 (T&... coord){ (..., LimitToRange(coord, 0, Map_40      - 1)); }
template<typename... T> __forceinline void Limit112(T&... coord){ (..., LimitToRange(coord, 0, FineMap_112 - 1)); }

#define sprintf(dst, ...) _snprintf_s(dst, _TRUNCATE, __VA_ARGS__)
#define sprintf_s(dst, ...) _snprintf_s(dst, _TRUNCATE, __VA_ARGS__)
#define strcpy(dst, src) strncpy_s(dst, src, _TRUNCATE)
#define strcpy_s(dst, src) strncpy_s(dst, src, _TRUNCATE)
#define strcat(dst, src) strncat_s(dst, src, _TRUNCATE)
#define strcat_s(dst, src) strncat_s(dst, src, _TRUNCATE)

// вспомогательная функция для печати строки в саму себя
template <size_t size>
__forceinline void sprints( char (&dst)[size], const char* stemplate, const char* s1, const char* s2 )
{
	char res[ size ];
	sprintf_s( res, stemplate, s1, s2 );
	sprintf_s( dst, "%s", res );
}

// добавлено для целей сверки, после можно заменить на (int) и включить Extended Instructions в компиляторе - станет быстрый инлайн
#define ftol(x) (int)(x) //((int)(__int64)((double)x))

// macro for selection value from index sequence, for example by difficulty: int add = by( Difficulty, 10, 20, 30 );
template<typename T> __forceinline T by( size_t i, T v1, T v2, T v3 ) { return i ? ( i == 1 ? v2 : v3 ) : v1; }
template<typename T, typename... V> __forceinline T by( size_t i, T v1, T v2, T v3, V... rest ) { T val[] = { v1, v2, v3, (T) rest... }; return val[ i < countof(val) ? i : countof(val) - 1 ]; }

// macro for expression like baseMonsterIndex == 1 || baseMonsterIndex == 2 || ..., чтобы писать is(baseMonsterIndex, 1, 2, ...)
template<typename V, typename... T > __forceinline bool is( V&& var, T&& ... val ){ return ( ... || (var == val) ); }
template<typename V, typename T, size_t N> __forceinline bool is( V&& var, T (&arr)[N] ){ for( auto v: arr ) if( var == v ) return true; return false; }

template<typename V, typename... T > __forceinline bool has( V&& arr, T&& ... val ){ for( auto v: arr ) if( ( ... || (v == val) ) ) return true; return false; }
template<typename V, typename... T > __forceinline bool has( int n, V&& arr, T&& ... val ){ for( int i = 0; i < n; ++i ) if( ( ... || (arr[i] == val) ) ) return true; return false; }

// macro for expression like monsterResist & 1 && spellType == 1 || ..., чтобы писать bit_is(monsterResist, spellType, 1, 1, 2, 2, ...)
template<typename V1, typename V2, typename... T> __forceinline bool bit_is( V1& v1, V2& v2, T... var ){ int v[] = {var...};
	for( size_t i = 0; i < sizeof...(var); i+=2 ) if( v1 & v[i] && v2 == v[i+1] ) return true; return false; }

// macro for expression like baseMonsterIndex1 == 1 && baseMonsterIndex2 == 1 || ..., чтобы писать all(1, baseMonsterIndex1, baseMonsterIndex2, ...)
template<typename V, typename... T > __forceinline bool all( V val, T ... var ) { return (... && (var == val)); }

// macro for expression like baseMonsterIndex != 1 && baseMonsterIndex != 2 && ..., чтобы писать not(baseMonsterIndex, 1, 2, ...)
template<typename V, typename... T > __forceinline bool not( V&& var, T ... val ){ return ( ... && (var != val) ); }

// macro for expression like item.AddsStrength += effectValue; item.AddsMagic += effectValue; ...
template<typename V, typename... T > __forceinline void addTo( V val, T& ... var ) { (..., (var += val)); }

// macro for each element in array
#define each(arr) for(auto&_:arr)_
#define fori(max) for(int i = 0; i < max; ++i)
#define for1(max) for(int i = 1; i < max; ++i)
#define formap(x, y, max) for(int x = 0; x < max; ++x) for(int y = 0; y < max; ++y)

// macro for any repetitious expression
#define fold(exp, ...) [&](auto&& ...x){ return ( ... exp ); }( __VA_ARGS__ )

// macro for fall through cases
#define or : [[fallthrough]]; case

// macro for allocating a local array on a stack of non-constexpr size (analogue to C99 Variable Lenght Array)
#define vla(type, name, size) type* name = (type*) alloca( (size) * sizeof(type) )

// memory debugging structs
template<class T> void Lock(T& data){ ulong dwOld = 0; VirtualProtect( (void*)&data, sizeof(T), PAGE_NOACCESS, &dwOld ); }
template<class T> void LockWrite(T& data){ ulong dwOld = 0; VirtualProtect( (void*)&data, sizeof(T), PAGE_READONLY, &dwOld ); }
template<class T> void Unlock(T& data){ ulong dwOld = 0; VirtualProtect( (void*)&data, sizeof(T), PAGE_EXECUTE_READWRITE, &dwOld ); }

template<typename T> struct debug
{
	__declspec(noinline) operator T() const { // impicit
		//Unlock(data);
		T value = data;
		//Lock(data);
		return value;
	}
	__declspec(noinline) void operator=(const T value){
		//if( *(int*)(&data - 37) == 1212167182 && value >= 20 ) __debugbreak();
		//if( DungeonLevel == 0 && abs(value) > 15 ) __debugbreak(); // killed monster drop in town fixed 16.03.2022 8:11:11
		//Unlock(data);
		data = value;
		//Lock(data);
	}
	#if 0
	__declspec(noinline) int operator--(int){
		int value = data;
		--data;
		//if( data == 0 ) __debugbreak();
		return data;
	}
	__declspec(noinline) debug<T>& operator--(){
		--data;
		//if( data == 0 ) __debugbreak();
		return *this;
	}
	__declspec(noinline) debug<T>& operator*= (int multi){
		data *= multi;
		//if( data == 0 ) __debugbreak();
		return *this;
	}
	#endif
	T data;
};

template<class T, size_t rows, size_t cols> class Cell
{
	T& cell;
	int row, col;
public:
	typedef T (&M)[rows][cols];
	inline static const T outOfMap = 0;
	Cell(M map, int row = 0, int col = 0): cell(map[row][col]), row(row), col(col) {}
	T& operator()(int rowOfs = 0, int colOfs = 0)
	{
		if( size_t(row + rowOfs) < rows && size_t(col + colOfs) < cols ){
			return M(cell)[rowOfs][colOfs];
		}else{
			return const_cast<T&>(outOfMap);
		}
	}
	operator T&() const { return cell; }
	void operator=(const T value) { cell = value; }
};

#define CHECK_DUPLICATES(A, ...) \
template <typename T, size_t N> constexpr bool has_duplicates(const T(&arr)[N]) \
{ \
	for( size_t i = 1; i < N; i++) \
		for( size_t j = 0; j < i; j++) \
			if( arr[i] __VA_ARGS__ == arr[j] __VA_ARGS__ ) \
				return true; \
	return false; \
} \
static_assert(!has_duplicates(foo), #A #__VA_ARGS__ " has duplicate values");