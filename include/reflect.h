//#pragma once
#ifndef _reflect_h_
#define _reflect_h_
#else
#error reinclude reflect.h
#endif

#include "template_counter.h"

constexpr int MAX_REFLECT = 100; // максимальное количество сериализуемых типов
constexpr int MAX_ARRAY_COUNT = 4096; // максимальное количество элементов в корневом сериализуемом массиве 

// Проверка наличия поля в классе (классический метод, работает в старых компиляторах) 
// применяется последним аргументом шаблона функции зависящей от наличия поля: 
// , HasMember( класс::поле ) 
template<typename M, M> struct CheckMember;
#define HasMember( memberName ) CheckMember< decltype(&memberName), &memberName > * = 0

// частичная специализации шаблонной фукнции через перегрузку 
#define COMN_FUNC( T, subtype, functype ) template<class T> typename std::enable_if< std::subtype<T>::value, functype >::type 
// полная специализация перегруженной шаблонной фукнции 
#define SPEC_FUNC( T, subtype, functype ) template<> typename std::enable_if< std::subtype<T>::value, functype >::type
// частичная специализации не такой шаблонной фукнции через перегрузку 
#define COMN_OTHER_FUNC( T, subtype, functype ) template<class T> typename std::enable_if< ! std::subtype<T>::value, functype >::type 
// полная специализация перегруженной не такой шаблонной фукнции 
#define SPEC_OTHER_FUNC( T, subtype, functype ) template<> typename std::enable_if< ! std::subtype<T>::value, functype >::type
// частичная специализации не такой шаблонной фукнции через перегрузку 

// функции вывода данных типа, int inner - уровень вложенности, th = 1 - вывод данных th1, 2 - th2
COMN_FUNC( T	, is_class		, void ) out_( T* var, int inner = 0, int th = 1 ); // вывод структуры 
COMN_FUNC( T	, is_union		, void ) out_( T* var, int inner = 0, int th = 1 ); // вывод объединения 
COMN_FUNC( T	, is_array		, void ) out_( T* var, int inner = 0, int th = 1 ); // вывод массива 
COMN_FUNC( T	, is_enum		, void ) out_( T* var, int inner = 0, int th = 1 ); // вывод перечисления 
COMN_FUNC( T	, is_fundamental, void ) out_( T* var, int inner = 0, int th = 1 ) { cout << *var; } // вывод обычного типа 
SPEC_FUNC( uchar, is_fundamental, void ) out_< uchar >( uchar *, int, int ); // вывод целого байта 
SPEC_FUNC( char	, is_fundamental, void ) out_< char  >( char  *, int, int ); // вывод знакового целого байта 
COMN_FUNC( T	, is_pointer	, void ) out_( T* var, int inner = 0, int th = 1 ); // вывод обычного указателя (возможно понадобиться переделать) 
SPEC_FUNC( char*, is_pointer	, void ) out_< char* >( char* *, int, int ); // вывод строки 

// тип функции вывода одного элемента структуры/типа 
typedef void (*func_out)(void* var, int inner, int th); // All machines have sizeof(char) == 1, but we need to unknowing size of var type, due to prevent improper calling

// функции сравнения данных типа
COMN_FUNC( T	, is_class		, bool ) dif_( T* var1, T* var2 ); // сравнение структуры 
COMN_FUNC( T	, is_union		, bool ) dif_( T* var1, T* var2 ); // сравнение объединения 
COMN_FUNC( T	, is_array		, bool ) dif_( T* var1, T* var2 ); // сравнение массива 
COMN_FUNC( T	, is_enum		, bool ) dif_( T* var1, T* var2 ) { return *var1 != *var2; } // сравнение перечисления 
COMN_FUNC( T	, is_fundamental, bool ) dif_( T* var1, T* var2 ) { return *var1 != *var2; } // сравнение обычного типа 
COMN_FUNC( T	, is_pointer	, bool ) dif_( T* var1, T* var2 ); // сравнение обычного указателя
SPEC_FUNC( char*, is_pointer	, bool ) dif_< char* >( char* *, char* * ); // сравнение строки 
SPEC_FUNC( HINSTANCE, is_pointer, bool ) dif_< HINSTANCE >( HINSTANCE *, HINSTANCE * );

// тип функции сравнения структуры/типа 
typedef bool( *func_dif )(void* var1, void* var2);

// метаданные для рефлексии переменной {offset,var_name,type_name,out_,dif_}
struct meta_{
	const int offset; // смещение относительно начала структуры / размер переменной в массиве
	const char* const field_name; // имя поля ( / массива ? )
	const char* const field_type; // имя типа переменной 
	const func_out out_; // функция вывода переменной этого типа
	const func_dif dif_; // функция сравнения переменной этого типа
};

// has member detector http://stackoverflow.com/questions/1005476/how-to-detect-whether-there-is-a-specific-member-variable-in-class
template<typename T, typename = void> struct has_reflect: std::false_type {};
template<typename T> struct has_reflect<T, decltype(T::this_type(), void())>: std::true_type {};

// определение константного массива с метаданными полей структуры в зависимости от количества полей (не более 500 для студии, ограничение компилятора)
template< class T > constexpr int members_count = 0; // нельзя создать массив размером 0, поэтому для нерефлексированных структур будет ошибка
template< class T > struct ext_reflect;
template< class T, int> struct member_;
template< class C, typename I = make_index_sequence< members_count<C> > > struct members_;
template< class C, size_t... i > struct members_< C, index_sequence<i...> >
{
	// если рефлексия задана внутри структуры 
	template<class T = C> static typename std::enable_if<   has_reflect<T>::value, const meta_* const >::type meta(){
		static const meta_ membersData[ members_count<T> ] = { { T::member_<i>::offset_(), T::member_<i>::field_name(), T::member_<i>::field_type(), T::member_<i>::out_(), T::member_<i>::dif_() } ... };
		return membersData;
	}
	// если рефлексия задана снаружи структуры 
	template<class T = C> static typename std::enable_if< ! has_reflect<T>::value, const meta_* const >::type meta(){
		static const meta_ membersData[ members_count<T> ] = { { member_<T,i>::offset_(), member_<T,i>::field_name(), member_<T,i>::field_type(), member_<T,i>::out_(), member_<T,i>::dif_() } ... };
		return membersData;
	}
};

// тип функции вывода корневого массива типов 
typedef void (*func_out_array)( void* var, int count );

// метаданные для рефлексии корневого массива типа/структуры/класса { имя типа, функция сериализации массива типа } 
struct meta_root{
	const char* const	 type_name; // имя типа
	const func_out_array out_array; // функция вывода корневого массива типа
};

// рефлексия корневого массива типа/структуры/класса { имя типа, функция сериализации типа } 
template<int N> struct reflected{
	static const char* const	type_name() { return ""; };
	static const func_out_array	out_array() { return 0;	 };
};

// массив для поиска типа по имени и вывода шапки .cpp, для out_ и имени переменных не нужен
#define F(N) { reflected<N>::type_name(), reflected<N>::out_array() },
template<class T = meta_root> struct types_{
	static const T* const meta(){
		static const T structsData[MAX_REFLECT] = { 
			F(0) F(1) F(2) F(3) F(4) F(5) F(6) F(7) F(8) F(9) F(10) F(11) F(12) F(13) F(14) F(15) F(16) F(17) F(18) F(19)
			F(20)F(21)F(22)F(23)F(24)F(25)F(26)F(27)F(28)F(29)F(30) F(31) F(32) F(33) F(34) F(35) F(36) F(37) F(38) F(39)
			F(40)F(41)F(42)F(43)F(44)F(45)F(46)F(47)F(48)F(49)F(50) F(51) F(52) F(53) F(54) F(55) F(56) F(57) F(58) F(59)
			F(60)F(61)F(62)F(63)F(64)F(65)F(66)F(67)F(68)F(69)F(70) F(71) F(72) F(73) F(74) F(75) F(76) F(77) F(78) F(79)
			F(80)F(81)F(82)F(83)F(84)F(85)F(86)F(87)F(88)F(89)F(90) F(91) F(92) F(93) F(94) F(95) F(96) F(97) F(98) F(99) 
		};
		return structsData;
	}
};
#undef F

//#if ! defined(__clang__)
struct reflected_count{}; // счетчик сериализованных типов 
// рефлексия корневого массива любого типа 
#define reflect( type )																									\
template<> struct reflected< COUNTER(reflected_count) >{																\
	static const char* const type_name   (){ return #type;															}	\
	static const func_out_array out_array(){ return (func_out_array)(void(*)(type*,int))::out_array<type>;			}	\
}; COUNTER_INC(reflected_count)

// начало рефлексии структуры/класса begin_( struct_name );
#define begin_( type )																									\
	typedef type this_type;																								\
	enum { begin_counter = __COUNTER__ };																				\
	template<int> struct member_;

// начало внешней рефлексии структуры/класса begin_( struct_name );
#define begin__( type )	template<> struct ext_reflect<type> { static constexpr int begin_counter = __COUNTER__; }

// рефлексия поля/массива в структуре/классе _( name )
#define _( name )																										\
template<> struct member_<__COUNTER__ - begin_counter - 1> {															\
	static const int		 offset_	() { return offsetof( this_type, name );									}	\
	static const char* const field_name	() { return #name;															}	\
	static const char* const field_type	() { return typeid( this_type::name ).na##me();								}	\
	static const func_out out_(){ return (func_out)(void(*)(decltype(this_type::name)*,int,int))::out_<decltype(this_type::name)>;}\
	static const func_dif dif_(){ return (func_dif)(bool(*)(decltype(this_type::name)*,decltype(this_type::name)*))::dif_<decltype(this_type::name)>;}}

// внешняя рефлексия поля/массива в структуре/классе __( struct, name )
#define _E( T, name )																									\
template<> struct member_<T, __COUNTER__ - ext_reflect<T>::begin_counter - 1> {											\
	static const int		 offset_	() { return offsetof( T, name );											}	\
	static const char* const field_name	() { return #name;															}	\
	static const char* const field_type	() { return typeid( T::name ).na##me();										}	\
	static const func_out out_(){ return (func_out)(void(*)(decltype(T::name)*,int,int))::out_<decltype(T::name)>;	}	\
	static const func_dif dif_(){ return (func_dif)(bool(*)(decltype(T::name)*,decltype(T::name)*))::dif_<decltype(T::name)>;}	}

// рефлексия поля со спецтипом
#define _U( name, type )																								\
template<> struct member_<__COUNTER__ - begin_counter - 1> {															\
	static const int		 offset_	() { return offsetof( this_type, name );									}	\
	static const char* const field_name	() { return #name;															}	\
	static const char* const field_type	() { return typeid( this_type::name ).na##me();								}	\
	static const func_out out_(){ return (func_out)(void(*)(type*,int,int))::out_<type>;}								\
	static const func_dif dif_(){ return (func_dif)(bool(*)(type*,type*))::dif_<type>;}}

// маркер конца рефлексии данных структуры/класса
#define end_(type, ...) template<> constexpr int members_count<type> = __COUNTER__ - type::begin_counter - 1 __VA_ARGS__

// маркер конца внешней рефлексии данных структуры/класса
#define end__(type) template<> constexpr int members_count<type> = __COUNTER__ - ext_reflect<type>::begin_counter - 1

char** enum_split(char* names, size_t count = 2048 );

// рефлексия имен элементов enum 
template< class En > struct Enum { static const char* const Name( En e ){ return "This enum is not reflected"; } };
#define reflect_enum( En, ... ) template<> struct Enum< En >{ \
	static constexpr const En values[] = { __VA_ARGS__ };	\
	static constexpr size_t count = countof(values);		\
	static char** get_name(){								\
		static char names[] = #__VA_ARGS__;					\
		static char** name = enum_split( names, count );	\
		return name;										\
	}														\
	static const char* const Name( En e ){					\
		char** name = get_name();							\
		for( int i = 0; i < count; ++i ){					\
			if( values[i] == e ) return name[i];			\
		}													\
		return 0;/*"Num not "#En;*/							\
	} }

// поиск имени enum по значению
#define EnumName( En, e ) Enum< En >::Name( (En) (e) )

#define ARGS_ template<typename R, typename...T> constexpr __forceinline
// количество аргументов функции
ARGS_ size_t ArgsCount( R( __stdcall* )(T...) ) { return sizeof...(T); }
ARGS_ size_t ArgsCount( R( __fastcall* )(T...) ) { return sizeof...(T); }
ARGS_ size_t ArgsCount( R( _cdecl* )(T...) ) { return sizeof...(T); }
ARGS_ size_t ArgsCount( R( _cdecl* )(T...,...) ) { return sizeof...(T) + 1; }
// calling convention функции
ARGS_ bool is_stdcall( R( __stdcall* )(T...) ) { return true; }
ARGS_ bool is_stdcall( R( __fastcall* )(T...) ) { return false; }
ARGS_ bool is_stdcall( R( _cdecl* )(T...) ) { return true; }
ARGS_ bool is_stdcall( R( _cdecl* )(T...,...) ) { return true; }
// есть ли возвращаемое значение у функции
ARGS_ bool is_ret( R( __stdcall* )(T...) ) { return ! is_void<R>::value; }
ARGS_ bool is_ret( R( __fastcall* )(T...) ) { return ! is_void<R>::value; }
ARGS_ bool is_ret( R( _cdecl* )(T...) ) { return !is_void<R>::value; }
ARGS_ bool is_ret( R( _cdecl* )(T...,...) ) { return ! is_void<R>::value; }
// функция вывода аргументов по типам
typedef void (_cdecl *args_out)(...);
// вызов произвольного кода по списку типов параметров
//struct pass { template<typename ...T> pass( T... ) {} }; //рекомендуется использовать initializer_list
// вывода аргументов по типам
// для usercall можно задать специальный макрос со списком регистров и выводить по нему
template<typename...T> void _cdecl OutArgs( int inner, int th, T... a ){ 
	int i = 0; void* p = (void*)0x12345678;
	initializer_list<int>{ ( ( is_reference<T>::value ? (p = &a, ::out_((remove_reference_t<T>**)&p,inner,th) ) : ::out_(&a,inner,th) ),
	++i, (i < sizeof...(T) ? cout << ", " : cout << "" ), 0)... }; }
// фукнция для получения функции вывода аргументов по типу
ARGS_ args_out GetOutArgs( R( __stdcall* )(T...) )	{ return (args_out) (void*) OutArgs<T...>; }
ARGS_ args_out GetOutArgs( R( __fastcall* )(T...) ) { return (args_out) (void*) OutArgs<T...>; }
ARGS_ args_out GetOutArgs( R( _cdecl* )(T...) )		{ return (args_out) (void*) OutArgs<T...>; }
ARGS_ args_out GetOutArgs( R( _cdecl* )(T...,...) ) { return (args_out) (void*) OutArgs<T...,int>; }
// вывод возвращаемого значения по типу
template<typename T> void _cdecl OutRet( int inner, int th, T a ){
	void* p = (void*)0x12345678; if( is_reference<T>::value ){ p = &a; ::out_((remove_reference_t<T>**)&p,inner,th); }else{ ::out_(&a,inner,th); }
}
// фукнция для получения функции вывода возвращаемого значения по типу
template<typename T> constexpr __forceinline args_out GetOutRet(){ return (args_out) (void*) OutRet<T>; }
template<> constexpr __forceinline args_out GetOutRet<void>() { return (args_out) 0; }
template<> constexpr __forceinline args_out GetOutRet<POINT>() { return (args_out) 0; } // с выводом составных значений пока не разобрался

// рефлексия метаданных и адресов глобальных функций и переменных
struct SubInfo
{
	char*		name;	// имя функции
	size_t		adr;	// адрес функции в th 1
	void*		adrNew;	// адрес функции в th 2
	bool		stable;	// переменные не проверяются, только сам вызов
	size_t		args;	// количество аргументов функции
	bool		stdcall;// тип передачи параметров
	bool		ret;	// возвращает ли функция значение
	args_out	out;	// функция вывода аргументов по типу
	args_out	rout;	// функция вывода возвращаемого значения по типу
	int			calls;	// количество вызовов для статистики
};
struct VarInfo
{
	size_t		adr;	// адрес переменной в th 1
	void*		adrNew;	// адрес переменной в th 2
	char*		name;	// имя переменной
	size_t		size;	// размер переменной
	char*		type1;	// префикс тип
	char*		type2;	// постфикс тип
	func_out	out;	// функция вывода переменной данного типа
	func_dif	dif;	// функция сравнения переменных данного типа
	bool		stable;	// переменная не проверяется
};
template<int i> struct Sub { static constexpr SubInfo v = { nullptr, 0, 0, true, 0, true, false, 0, 0 }; };
template<int i> struct Var { static const VarInfo v(){ return { 0, nullptr, 0, 0, 0, 0, 0, 0, true }; } };
#define SUB_TABLE constexpr int SUB_COUNTER = __COUNTER__ + 1;
#define VAR_TABLE constexpr int VAR_COUNTER = __COUNTER__ + 1;

// адрес функции которую надо сверять
#define SUB_ADR( adr, name, stable, ret ) template<> struct Sub<__COUNTER__ - SUB_COUNTER> { static constexpr SubInfo v = \
{ #name, adr, &name, stable, ArgsCount(name), is_stdcall(name), is_ret(name), GetOutArgs(name), GetOutRet<ret>() }; };
// адрес функции которую не надо сверять
#define CRT_ADR( adr, name ) template<> struct Sub<__COUNTER__ - SUB_COUNTER> { static constexpr SubInfo v = { #name, adr, 0 /*&name*/, true, 0, true, false, 0, 0 }; };
// адрес переменной которую надо сверять
#define VAR_ADR( adr, name, stable, t1, t2 ) template<> struct Var<__COUNTER__ - VAR_COUNTER> { static const VarInfo v(){ return { adr, &name,	#name, sizeof( name ), #t1, #t2, \
(func_out)(void(*)(decltype(name)*,int,int))::out_<decltype(name)>, (func_dif)(bool(*)(decltype(name)*,decltype(name)*))::dif_<decltype(name)>, stable }; } };
// адрес динамического массива который надо сверять
#define DYN_ADR( adr, name, stable, t1, t2 ) template<> struct Var<__COUNTER__ - VAR_COUNTER> { static const VarInfo v(){ return { adr, &name,	#name, sizeof( name ), #t1, "", \
(func_out)(void(*)(remove_pointer<decltype(name)>::type(**)t2,int,int))::out_<remove_pointer<decltype(name)>::type(*)t2>, \
(func_dif)(bool(*)(remove_pointer<decltype(name)>::type(**)t2,remove_pointer<decltype(name)>::type(**)t2))::dif_<remove_pointer<decltype(name)>::type(*)t2>, stable }; } };
// адрес системной переменной которую нельзя сверять
#define SYS_ADR( adr, name ) template<> struct Var<__COUNTER__ - VAR_COUNTER> { static const VarInfo v(){ return { adr, &name,	#name, sizeof( name ), 0, 0, 0, 0, true }; } };
// старый адрес перенесенной переменной
#define OLD_ADR( adr, name ) template<> struct Var<__COUNTER__ - VAR_COUNTER> { static const VarInfo v(){ return { adr, 0,		#name, sizeof( name ), 0, 0, 0, 0, true }; } }

constexpr int FCnt = 50; // максимальное количество функций 2500 == FCnt ^ 2
typedef SubInfo FuncTable[ FCnt*FCnt ];
constexpr int VCnt = 40; // максимальное количество переменных 1600 == VCnt ^ 2
typedef VarInfo VarTable[ VCnt*VCnt ];

void Validate( SubInfo* f = 0, int type = 5, int line = 0 );
#define VD Validate( (SubInfo*)__func__, 5, __LINE__ );

// ссылка на оригинальную переменную/функцию по адресу заданному вручную
#define REF_EXE( adr, name ) namespace Exe { /*constexpr size_t adr_##name = adr;*/ __declspec(selectany) auto* p_##name = (decltype(::name)*)adr; __declspec(selectany) auto& name = *p_##name; }

// объявление переменной или массива
#define VAR( adr, type1, type2, name ) extern type1 name type2; VAR_ADR( adr, name, false, type1, type2 ) REF_EXE( adr, name )
//#define VAR_( adr, type1, type2, name ) extern type1 name type2; VAR_ADR( adr, name, false, type1, type2 ) REF_EXE( adr, name )
#define VAR_( adr, type1, type2, name ) extern type1 name type2; VAR_ADR( adr, name, true, type1, type2 ) REF_EXE( adr, name )
#define DYN( adr, type1, type2, name ) extern type1 name; DYN_ADR( adr, name, false, type1, type2 ) REF_EXE( adr, name )
#define DYN_( adr, type1, type2, name ) extern type1 name; DYN_ADR( adr, name, true, type1, type2 ) REF_EXE( adr, name )
#define SYS( adr, type1, type2, name ) extern type1 name type2; SYS_ADR( adr, name ) REF_EXE( adr, name )
#define OLD( adr, type1, type2, name ) extern type1 old_##name type2; OLD_ADR( adr, old_##name )
// объявление функции
// сюда можно добавить список параметров и тип вызова и анализировать параметры на старте функции
#define SUB( adr, ret, call, name, ... ) ret call name( __VA_ARGS__ ); SUB_ADR( adr, name, false, ret ) REF_EXE( adr, name )
#define SUB_( adr, ret, call, name, ... ) ret call name( __VA_ARGS__ ); SUB_ADR( adr, name, true, ret ) REF_EXE( adr, name )
#define CRT( adr, ret, call, name, ... ) ret call name( __VA_ARGS__ ); CRT_ADR( adr, name ) REF_EXE( adr, name )
