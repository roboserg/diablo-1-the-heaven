//#pragma once
#ifndef _reflect_h_
#define _reflect_h_
#else
#error reinclude reflect.h
#endif

#include "template_counter.h"

constexpr int MAX_REFLECT = 100; // (rt) maximum number of serializable types
constexpr int MAX_ARRAY_COUNT = 4096; // (rt) maximum number of elements in the root serializable array 

// (rt) Check whether a class has a field (the classic method, works in old compilers) 
// (rt) used as the last template argument of a function that depends on the field's presence: 
// (rt) , HasMember( class::field ) 
template<typename M, M> struct CheckMember;
#define HasMember( memberName ) CheckMember< decltype(&memberName), &memberName > * = 0

// (rt) partial specialization of a template function via overloading 
#define COMN_FUNC( T, subtype, functype ) template<class T> typename std::enable_if< std::subtype<T>::value, functype >::type 
// (rt) full specialization of an overloaded template function 
#define SPEC_FUNC( T, subtype, functype ) template<> typename std::enable_if< std::subtype<T>::value, functype >::type
// (rt) partial specialization of a not-quite-template function via overloading 
#define COMN_OTHER_FUNC( T, subtype, functype ) template<class T> typename std::enable_if< ! std::subtype<T>::value, functype >::type 
// (rt) full specialization of an overloaded not-quite-template function 
#define SPEC_OTHER_FUNC( T, subtype, functype ) template<> typename std::enable_if< ! std::subtype<T>::value, functype >::type
// (rt) partial specialization of a not-quite-template function via overloading 

// (rt) type data output functions, int inner - nesting level, th = 1 - output th1 data, 2 - th2
COMN_FUNC( T	, is_class		, void ) out_( T* var, int inner = 0, int th = 1 ); // (rt) output a struct 
COMN_FUNC( T	, is_union		, void ) out_( T* var, int inner = 0, int th = 1 ); // (rt) output a union 
COMN_FUNC( T	, is_array		, void ) out_( T* var, int inner = 0, int th = 1 ); // (rt) output an array 
COMN_FUNC( T	, is_enum		, void ) out_( T* var, int inner = 0, int th = 1 ); // (rt) output an enum 
COMN_FUNC( T	, is_fundamental, void ) out_( T* var, int inner = 0, int th = 1 ) { cout << *var; } // (rt) output a plain type 
SPEC_FUNC( uchar, is_fundamental, void ) out_< uchar >( uchar *, int, int ); // (rt) output an unsigned byte 
SPEC_FUNC( char	, is_fundamental, void ) out_< char  >( char  *, int, int ); // (rt) output a signed byte 
COMN_FUNC( T	, is_pointer	, void ) out_( T* var, int inner = 0, int th = 1 ); // (rt) output a plain pointer (may need reworking) 
SPEC_FUNC( char*, is_pointer	, void ) out_< char* >( char* *, int, int ); // (rt) output a string 

// (rt) type of the function that outputs one struct element/type 
typedef void (*func_out)(void* var, int inner, int th); // All machines have sizeof(char) == 1, but we need to unknowing size of var type, due to prevent improper calling

// (rt) type data comparison functions
COMN_FUNC( T	, is_class		, bool ) dif_( T* var1, T* var2 ); // (rt) compare a struct 
COMN_FUNC( T	, is_union		, bool ) dif_( T* var1, T* var2 ); // (rt) compare a union 
COMN_FUNC( T	, is_array		, bool ) dif_( T* var1, T* var2 ); // (rt) compare an array 
COMN_FUNC( T	, is_enum		, bool ) dif_( T* var1, T* var2 ) { return *var1 != *var2; } // (rt) compare an enum 
COMN_FUNC( T	, is_fundamental, bool ) dif_( T* var1, T* var2 ) { return *var1 != *var2; } // (rt) compare a plain type 
COMN_FUNC( T	, is_pointer	, bool ) dif_( T* var1, T* var2 ); // (rt) compare a plain pointer
SPEC_FUNC( char*, is_pointer	, bool ) dif_< char* >( char* *, char* * ); // (rt) compare a string 
SPEC_FUNC( HINSTANCE, is_pointer, bool ) dif_< HINSTANCE >( HINSTANCE *, HINSTANCE * );

// (rt) type of the struct/type comparison function 
typedef bool( *func_dif )(void* var1, void* var2);

// (rt) metadata for reflecting a variable {offset,var_name,type_name,out_,dif_}
struct meta_{
	const int offset; // (rt) offset from the start of the struct / size of the variable in the array
	const char* const field_name; // (rt) field name ( / array? )
	const char* const field_type; // (rt) variable's type name 
	const func_out out_; // (rt) function that outputs a variable of this type
	const func_dif dif_; // (rt) function that compares a variable of this type
};

// has member detector http://stackoverflow.com/questions/1005476/how-to-detect-whether-there-is-a-specific-member-variable-in-class
template<typename T, typename = void> struct has_reflect: std::false_type {};
template<typename T> struct has_reflect<T, decltype(T::this_type(), void())>: std::true_type {};

// (rt) definition of a constant array with the struct's field metadata, depending on the number of fields (no more than 500 for Visual Studio, a compiler limit)
template< class T > constexpr int members_count = 0; // (rt) an array of size 0 can't be created, so non-reflected structs will give an error
template< class T > struct ext_reflect;
template< class T, int> struct member_;
template< class C, typename I = make_index_sequence< members_count<C> > > struct members_;
template< class C, size_t... i > struct members_< C, index_sequence<i...> >
{
	// (rt) if reflection is set inside the struct 
	template<class T = C> static typename std::enable_if<   has_reflect<T>::value, const meta_* const >::type meta(){
		static const meta_ membersData[ members_count<T> ] = { { T::member_<i>::offset_(), T::member_<i>::field_name(), T::member_<i>::field_type(), T::member_<i>::out_(), T::member_<i>::dif_() } ... };
		return membersData;
	}
	// (rt) if reflection is set outside the struct 
	template<class T = C> static typename std::enable_if< ! has_reflect<T>::value, const meta_* const >::type meta(){
		static const meta_ membersData[ members_count<T> ] = { { member_<T,i>::offset_(), member_<T,i>::field_name(), member_<T,i>::field_type(), member_<T,i>::out_(), member_<T,i>::dif_() } ... };
		return membersData;
	}
};

// (rt) type of the function that outputs a root array of types 
typedef void (*func_out_array)( void* var, int count );

// (rt) metadata for reflecting a root array of a type/struct/class { type name, type array serialization function } 
struct meta_root{
	const char* const	 type_name; // (rt) type name
	const func_out_array out_array; // (rt) function that outputs a root array of the type
};

// (rt) reflection of a root array of a type/struct/class { type name, type serialization function } 
template<int N> struct reflected{
	static const char* const	type_name() { return ""; };
	static const func_out_array	out_array() { return 0;	 };
};

// (rt) array for finding a type by name and for outputting the .cpp header; not needed for out_ and variable names
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
struct reflected_count{}; // (rt) counter of serialized types 
// (rt) reflection of a root array of any type 
#define reflect( type )																									\
template<> struct reflected< COUNTER(reflected_count) >{																\
	static const char* const type_name   (){ return #type;															}	\
	static const func_out_array out_array(){ return (func_out_array)(void(*)(type*,int))::out_array<type>;			}	\
}; COUNTER_INC(reflected_count)

// (rt) start of a struct/class reflection begin_( struct_name );
#define begin_( type )																									\
	typedef type this_type;																								\
	enum { begin_counter = __COUNTER__ };																				\
	template<int> struct member_;

// (rt) start of an external struct/class reflection begin_( struct_name );
#define begin__( type )	template<> struct ext_reflect<type> { static constexpr int begin_counter = __COUNTER__; }

// (rt) reflection of a field/array in a struct/class _( name )
#define _( name )																										\
template<> struct member_<__COUNTER__ - begin_counter - 1> {															\
	static const int		 offset_	() { return offsetof( this_type, name );									}	\
	static const char* const field_name	() { return #name;															}	\
	static const char* const field_type	() { return typeid( this_type::name ).na##me();								}	\
	static const func_out out_(){ return (func_out)(void(*)(decltype(this_type::name)*,int,int))::out_<decltype(this_type::name)>;}\
	static const func_dif dif_(){ return (func_dif)(bool(*)(decltype(this_type::name)*,decltype(this_type::name)*))::dif_<decltype(this_type::name)>;}}

// (rt) external reflection of a field/array in a struct/class __( struct, name )
#define _E( T, name )																									\
template<> struct member_<T, __COUNTER__ - ext_reflect<T>::begin_counter - 1> {											\
	static const int		 offset_	() { return offsetof( T, name );											}	\
	static const char* const field_name	() { return #name;															}	\
	static const char* const field_type	() { return typeid( T::name ).na##me();										}	\
	static const func_out out_(){ return (func_out)(void(*)(decltype(T::name)*,int,int))::out_<decltype(T::name)>;	}	\
	static const func_dif dif_(){ return (func_dif)(bool(*)(decltype(T::name)*,decltype(T::name)*))::dif_<decltype(T::name)>;}	}

// (rt) reflection of a field with a special type
#define _U( name, type )																								\
template<> struct member_<__COUNTER__ - begin_counter - 1> {															\
	static const int		 offset_	() { return offsetof( this_type, name );									}	\
	static const char* const field_name	() { return #name;															}	\
	static const char* const field_type	() { return typeid( this_type::name ).na##me();								}	\
	static const func_out out_(){ return (func_out)(void(*)(type*,int,int))::out_<type>;}								\
	static const func_dif dif_(){ return (func_dif)(bool(*)(type*,type*))::dif_<type>;}}

// (rt) end marker of the struct/class data reflection
#define end_(type, ...) template<> constexpr int members_count<type> = __COUNTER__ - type::begin_counter - 1 __VA_ARGS__

// (rt) end marker of the external struct/class data reflection
#define end__(type) template<> constexpr int members_count<type> = __COUNTER__ - ext_reflect<type>::begin_counter - 1

char** enum_split(char* names, size_t count = 2048 );

// (rt) reflection of enum element names 
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

// (rt) find an enum name by value
#define EnumName( En, e ) Enum< En >::Name( (En) (e) )

#define ARGS_ template<typename R, typename...T> constexpr __forceinline
// (rt) number of function arguments
ARGS_ size_t ArgsCount( R( __stdcall* )(T...) ) { return sizeof...(T); }
ARGS_ size_t ArgsCount( R( __fastcall* )(T...) ) { return sizeof...(T); }
ARGS_ size_t ArgsCount( R( _cdecl* )(T...) ) { return sizeof...(T); }
ARGS_ size_t ArgsCount( R( _cdecl* )(T...,...) ) { return sizeof...(T) + 1; }
// (rt) function calling convention
ARGS_ bool is_stdcall( R( __stdcall* )(T...) ) { return true; }
ARGS_ bool is_stdcall( R( __fastcall* )(T...) ) { return false; }
ARGS_ bool is_stdcall( R( _cdecl* )(T...) ) { return true; }
ARGS_ bool is_stdcall( R( _cdecl* )(T...,...) ) { return true; }
// (rt) whether the function has a return value
ARGS_ bool is_ret( R( __stdcall* )(T...) ) { return ! is_void<R>::value; }
ARGS_ bool is_ret( R( __fastcall* )(T...) ) { return ! is_void<R>::value; }
ARGS_ bool is_ret( R( _cdecl* )(T...) ) { return !is_void<R>::value; }
ARGS_ bool is_ret( R( _cdecl* )(T...,...) ) { return ! is_void<R>::value; }
// (rt) function that outputs arguments by type
typedef void (_cdecl *args_out)(...);
// (rt) calling arbitrary code over a list of parameter types
//(rt) struct pass { template<typename ...T> pass( T... ) {} }; //using initializer_list is recommended
// (rt) output of arguments by type
// (rt) for usercall a special macro with a list of registers can be set and output by it
template<typename...T> void _cdecl OutArgs( int inner, int th, T... a ){ 
	int i = 0; void* p = (void*)0x12345678;
	initializer_list<int>{ ( ( is_reference<T>::value ? (p = &a, ::out_((remove_reference_t<T>**)&p,inner,th) ) : ::out_(&a,inner,th) ),
	++i, (i < sizeof...(T) ? cout << ", " : cout << "" ), 0)... }; }
// (rt) function that gets the argument output function by type
ARGS_ args_out GetOutArgs( R( __stdcall* )(T...) )	{ return (args_out) (void*) OutArgs<T...>; }
ARGS_ args_out GetOutArgs( R( __fastcall* )(T...) ) { return (args_out) (void*) OutArgs<T...>; }
ARGS_ args_out GetOutArgs( R( _cdecl* )(T...) )		{ return (args_out) (void*) OutArgs<T...>; }
ARGS_ args_out GetOutArgs( R( _cdecl* )(T...,...) ) { return (args_out) (void*) OutArgs<T...,int>; }
// (rt) output of the return value by type
template<typename T> void _cdecl OutRet( int inner, int th, T a ){
	void* p = (void*)0x12345678; if( is_reference<T>::value ){ p = &a; ::out_((remove_reference_t<T>**)&p,inner,th); }else{ ::out_(&a,inner,th); }
}
// (rt) function that gets the return value output function by type
template<typename T> constexpr __forceinline args_out GetOutRet(){ return (args_out) (void*) OutRet<T>; }
template<> constexpr __forceinline args_out GetOutRet<void>() { return (args_out) 0; }
template<> constexpr __forceinline args_out GetOutRet<POINT>() { return (args_out) 0; } // (rt) haven't figured out output of compound values yet

// (rt) reflection of metadata and addresses of global functions and variables
struct SubInfo
{
	char*		name;	// (rt) function name
	size_t		adr;	// (rt) function address in th 1
	void*		adrNew;	// (rt) function address in th 2
	bool		stable;	// (rt) variables are not checked, only the call itself
	size_t		args;	// (rt) number of function arguments
	bool		stdcall;// (rt) parameter passing type
	bool		ret;	// (rt) whether the function returns a value
	args_out	out;	// (rt) function that outputs arguments by type
	args_out	rout;	// (rt) function that outputs the return value by type
	int			calls;	// (rt) call count for statistics
};
struct VarInfo
{
	size_t		adr;	// (rt) variable address in th 1
	void*		adrNew;	// (rt) variable address in th 2
	char*		name;	// (rt) variable name
	size_t		size;	// (rt) variable size
	char*		type1;	// (rt) type prefix
	char*		type2;	// (rt) type postfix
	func_out	out;	// (rt) function that outputs a variable of this type
	func_dif	dif;	// (rt) function that compares variables of this type
	bool		stable;	// (rt) the variable is not checked
};
template<int i> struct Sub { static constexpr SubInfo v = { nullptr, 0, 0, true, 0, true, false, 0, 0 }; };
template<int i> struct Var { static const VarInfo v(){ return { 0, nullptr, 0, 0, 0, 0, 0, 0, true }; } };
#define SUB_TABLE constexpr int SUB_COUNTER = __COUNTER__ + 1;
#define VAR_TABLE constexpr int VAR_COUNTER = __COUNTER__ + 1;

// (rt) address of a function to be compared
#define SUB_ADR( adr, name, stable, ret ) template<> struct Sub<__COUNTER__ - SUB_COUNTER> { static constexpr SubInfo v = \
{ #name, adr, &name, stable, ArgsCount(name), is_stdcall(name), is_ret(name), GetOutArgs(name), GetOutRet<ret>() }; };
// (rt) address of a function not to be compared
#define CRT_ADR( adr, name ) template<> struct Sub<__COUNTER__ - SUB_COUNTER> { static constexpr SubInfo v = { #name, adr, 0 /*&name*/, true, 0, true, false, 0, 0 }; };
// (rt) address of a variable to be compared
#define VAR_ADR( adr, name, stable, t1, t2 ) template<> struct Var<__COUNTER__ - VAR_COUNTER> { static const VarInfo v(){ return { adr, &name,	#name, sizeof( name ), #t1, #t2, \
(func_out)(void(*)(decltype(name)*,int,int))::out_<decltype(name)>, (func_dif)(bool(*)(decltype(name)*,decltype(name)*))::dif_<decltype(name)>, stable }; } };
// (rt) address of a dynamic array to be compared
#define DYN_ADR( adr, name, stable, t1, t2 ) template<> struct Var<__COUNTER__ - VAR_COUNTER> { static const VarInfo v(){ return { adr, &name,	#name, sizeof( name ), #t1, "", \
(func_out)(void(*)(remove_pointer<decltype(name)>::type(**)t2,int,int))::out_<remove_pointer<decltype(name)>::type(*)t2>, \
(func_dif)(bool(*)(remove_pointer<decltype(name)>::type(**)t2,remove_pointer<decltype(name)>::type(**)t2))::dif_<remove_pointer<decltype(name)>::type(*)t2>, stable }; } };
// (rt) address of a system variable that must not be compared
#define SYS_ADR( adr, name ) template<> struct Var<__COUNTER__ - VAR_COUNTER> { static const VarInfo v(){ return { adr, &name,	#name, sizeof( name ), 0, 0, 0, 0, true }; } };
// (rt) old address of a moved variable
#define OLD_ADR( adr, name ) template<> struct Var<__COUNTER__ - VAR_COUNTER> { static const VarInfo v(){ return { adr, 0,		#name, sizeof( name ), 0, 0, 0, 0, true }; } }

constexpr int FCnt = 50; // (rt) maximum number of functions 2500 == FCnt ^ 2
typedef SubInfo FuncTable[ FCnt*FCnt ];
constexpr int VCnt = 40; // (rt) maximum number of variables 1600 == VCnt ^ 2
typedef VarInfo VarTable[ VCnt*VCnt ];

void Validate( SubInfo* f = 0, int type = 5, int line = 0 );
#define VD Validate( (SubInfo*)__func__, 5, __LINE__ );

// (rt) reference to the original variable/function by a manually set address
#define REF_EXE( adr, name ) namespace Exe { /*constexpr size_t adr_##name = adr;*/ __declspec(selectany) auto* p_##name = (decltype(::name)*)adr; __declspec(selectany) auto& name = *p_##name; }

// (rt) declaration of a variable or array
#define VAR( adr, type1, type2, name ) extern type1 name type2; VAR_ADR( adr, name, false, type1, type2 ) REF_EXE( adr, name )
//#define VAR_( adr, type1, type2, name ) extern type1 name type2; VAR_ADR( adr, name, false, type1, type2 ) REF_EXE( adr, name )
#define VAR_( adr, type1, type2, name ) extern type1 name type2; VAR_ADR( adr, name, true, type1, type2 ) REF_EXE( adr, name )
#define DYN( adr, type1, type2, name ) extern type1 name; DYN_ADR( adr, name, false, type1, type2 ) REF_EXE( adr, name )
#define DYN_( adr, type1, type2, name ) extern type1 name; DYN_ADR( adr, name, true, type1, type2 ) REF_EXE( adr, name )
#define SYS( adr, type1, type2, name ) extern type1 name type2; SYS_ADR( adr, name ) REF_EXE( adr, name )
#define OLD( adr, type1, type2, name ) extern type1 old_##name type2; OLD_ADR( adr, old_##name )
// (rt) declaration of a function
// (rt) the parameter list and call type could be added here and the parameters analysed at the function start
#define SUB( adr, ret, call, name, ... ) ret call name( __VA_ARGS__ ); SUB_ADR( adr, name, false, ret ) REF_EXE( adr, name )
#define SUB_( adr, ret, call, name, ... ) ret call name( __VA_ARGS__ ); SUB_ADR( adr, name, true, ret ) REF_EXE( adr, name )
#define CRT( adr, ret, call, name, ... ) ret call name( __VA_ARGS__ ); CRT_ADR( adr, name ) REF_EXE( adr, name )
