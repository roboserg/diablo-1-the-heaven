//#pragma once
#ifndef _serialize_h_
#define _serialize_h_
#else
#error reinclude serialize.h
#endif

// (rt) serialization module for any binary data (types, structs, classes)                          
// (rt) serialization is writing binary data out in a processor-independent text form                 
// (rt) for moving between programs and platforms                                                   
// (rt) the data to serialize is obtained by semi-automatic reflection from the struct declaration 
// (rt) using the macros begin_, _, end_, reflect, reflect_enum                                      
// (rt) nested structs must also be declared with these macros beforehand                 
// (rt) example declaration of a serializable struct:                                                     
// 
// (rt) to output nested structs, the global function out_ is specialized for all 
// (rt) serializable types, and the output format parameter OutMode is set 

#include "reflect.h"

// (rt) Output formats 
enum OutMode { OM_CPP, OM_TXT, OM_XLS, OM_XML };
extern char* OutModes[4];
extern int CurOutMode; // (rt) = OM_CPP; // current data output format 
extern bool IgnoreEndMarker; // (rt) Flag to ignore the end-of-list marker, to force output of the given number of elements
extern int MaxDifRecursion;
extern bool OutElementAlwaysOnNewLine;

// (rt) look up an enum value by name (should be redone with Enum reflection)
template< class En, class Arr, size_t size > bool SetKeyByEnum( En& enumVar, const char* name, Arr (&arr)[size] )
{
	for( En i = 0; i < size; ++i ){
		if( !strcmp( name, arr[i] ) ){
			enumVar = i;
			return true;
		}
	}
	return false;
}

// (rt) a trial move of the main function body into a module is removed for now; Clang fails on it
// (rt) output a struct, class or union: T* var - struct address, int inner - struct nesting level 
template<class T> __forceinline void out_class_union( T* var, int inner, int th )
{
	static const meta_* meta = members_<T>::meta<>();
	if( inner > MaxDifRecursion ) return;
	int count = members_count<T>;
	switch( CurOutMode ){
	// (rt) output a struct in C++ format _{ field_1, field_2, ... }, 
	case OM_CPP: {
		if( OutElementAlwaysOnNewLine || inner == 0 ){
			cout << "\n\t{ "; // "{ "
		}else{
			cout << "{ ";
		}
		for( int m = 0; m < count; m++ ){
			cout << "/*" << meta[m].field_name << "*/ ";
			meta[m].out_((char*)var + meta[m].offset, inner + 1, th);
			if( m != count - 1 ){
				cout << ",\t";
			}
		}
		cout << " }";
		break; }
	// (rt) text output of a struct listing all fields, their types and offsets, for debugging 
	case OM_TXT: {
		cout << typeid(T).name() << " ( " << sizeof(T) << " bytes ):\n";
		for( int m = 0; m < count; m++ ){
			int offset = meta[m].offset;
			for( int l = 0; l < inner; l++ ){
				cout << '\t';
			}
			cout << setw(4) << offset << ": " << meta[m].field_type << '\t' << meta[m].field_name << "\t= ";
			meta[m].out_((char*)var + offset, inner + 1, th);
			if( m != count - 1 || inner < 1 ){
				cout << '\n';
			}
		}
		break; }
	case OM_XLS: cout << "Error: Excel out mode not yet implemented; "; break;
	case OM_XML: cout << "Error: XML out mode not yet implemented; "; break;
	default: cout << "Error: Out mode id = " << CurOutMode << " not defined; "; break;
	}
}
COMN_FUNC( T, is_class, void ) out_( T* var, int inner, int th ){ out_class_union(var, inner, th); }
COMN_FUNC( T, is_union, void ) out_( T* var, int inner, int th ){ out_class_union(var, inner, th); }

// (rt) output an array that is a struct field
COMN_FUNC( T, is_array, void ) out_( T* arr, int inner, int th )
{
	if( inner > MaxDifRecursion) return;
	// (rt) couldn't move it out, since that needs a function returning a pointer to the element output function
	switch( CurOutMode ){
	case OM_CPP: // (rt) output an array in C++ format
	case OM_TXT: { // (rt) text output of an array 
		int i = 0;
		auto var = begin( *arr );
		auto last = end( *arr ) - 1;
		while( var < last ){
			// (rt) added output of a two-dimensional array as two-dimensional
			// (rt) struct elements are output in one column, since a struct is itself a line
			if( is_array<remove_extent_t<T>>::value
			 || is_class<remove_extent_t<T>>::value
			 || is_union<remove_extent_t<T>>::value ){
				if( inner >= 0 ) cout << '\n'; for( int i = 0; i <= inner; ++i ) cout << '\t';
				cout << "/*" << i++ << "*/\t";
			//(rt) }else if( true /* CutArrayOut */ ){ // need to add a config option for outputting long arrays
			//	if( i++ % 128 == 0 ){
			//		cout << '\n';
			//		(rt) // output of one-dimensional array indices
			//		cout << "/*" << i << "*/\t";
			//	}
			}
			out_( var++, inner + 1, th ); cout << ',';
			if( ! is_array<remove_extent_t<T>>::value
			 && ! is_class<remove_extent_t<T>>::value
			 && ! is_union<remove_extent_t<T>>::value ){
				cout << '\t';
			}
		}
		if( is_array<remove_extent_t<T>>::value
		 || is_class<remove_extent_t<T>>::value
		 || is_union<remove_extent_t<T>>::value ){
			if( inner >= 0 ) cout << '\n'; for( int i = 0; i <= inner; ++i ) cout << '\t';
			cout << "/*" << i++ << "*/\t";
		}
		out_( var, inner + 1, th );
		break; }
	case OM_XLS: cout << "Error: Excel out mode not yet implemented; "; break;
	case OM_XML: cout << "Error: XML out mode not yet implemented; "; break;
	default: cout << "Error: Out mode id = " << CurOutMode << " not defined; "; break;
	}
}

// (rt) output an enumeration (enum) 
COMN_FUNC( T, is_enum, void ) out_( T* var, int inner, int th )
{
	// (rt) check for the presence of bit_enum operator|
	#pragma warning(suppress: 4555)
	if constexpr( is_same< decltype(T()|T()), T >::value ){
		// (rt) adding output of bit enumerations
		underlying_type<T>::type mask = 0, v = *var;
		char** name = Enum<T>::get_name();
		for( int i = 0; i < Enum<T>::count - 1; ++i ){
			if( v & Enum<T>::values[ i ] ){
				mask |= Enum<T>::values[ i ];
			}
		}
		if( !mask || mask != v){
			cout << "(" << typeid( *var ).name() << ") " << (int) v;
		}else if( mask == Enum<T>::values[ Enum<T>::count - 1 ] ){
			cout << name[ Enum<T>::count - 1 ];
		}else{
			mask = 0;
			char** name = Enum<T>::get_name();
			for( int i = 0; i < Enum<T>::count - 1; ++i ){
				if( v & Enum<T>::values[ i ] ){
					mask |= Enum<T>::values[ i ];
					cout << name[ i ];
					if( mask != v ) cout << "|";
				}
			}
		}
	}else{
		const char* enumName = EnumName( T, *var );
		if( enumName ){
			cout << enumName;
		}else{
			cout << "(" << typeid( *var ).name() << ") " << (int) *var;
		}
	}
} 


// (rt) if there is an end-of-list marker, check it 
// (rt) specialization if there is a marker
template<class T> __forceinline bool NotEndMarker( T* structArray, HasMember( T::EndMarker ) )
{
	return IgnoreEndMarker || structArray->EndMarker != -1;
}
// (rt) specialization if there is no marker 
__forceinline bool NotEndMarker( ... )
{
	return true;
}

// (rt) output a root array of structs in the format set by the global variable OutMode 
// (rt) T* structArray - array address, int maxCount - maximum number of elements
COMN_FUNC( T, is_class, void ) out_array( T* struct_array, int maxCount = MAX_ARRAY_COUNT)
{
	// (rt) query the metadata of the struct's fields 
	static const meta_* meta = members_<T>::meta<>();
	static const char* type_name = typeid(T).name();
	// (rt) likewise, moving it out needs the address of out_<decltype(&structArray[0])>, not going to build that yet
	switch( CurOutMode ){
	// (rt) output an array of structs in C++ format - {{ field_1, field_2 }, {...} }
	// (rt) together with the struct header in a comment and the module header 
	case OM_CPP: {
		// (rt) .cpp header 
		//cout << UTF8_BOM << "#include \"th1_stdafx.h\"\n\n";
		cout << "// " << type_name << " array at 0x" << hex << struct_array << dec << " \n";
		cout << type_name << ' ' << type_name;
		if( type_name[ strlen(type_name) - 1 ] == 'x' || *(short*)&type_name[ strlen(type_name) - 2 ] == 'hc' ){
			cout << "e";
		}
		cout << "s[] = {\n";
		// (rt) struct header (list of field names) 
		cout << "\t// " << meta[0].field_name;
		//cout << " " << meta[0].offset;
		//cout << " " << meta[0].field_type;
		for( int i = 1; i < members_count<T>; i++ ){
			cout << ", " << meta[i].field_name;
			//cout << " " << meta[i].offset;
			//cout << " " << meta[i].field_type;
		}
		cout << '\n';
		// (rt) output the struct data in a loop until EndMarker == -1 
		int i = 0; do{
			cout << "/* " << i << "\t*/ "; //cout << "\t";
			out_( &struct_array[i] );
			cout << '\n'; //cout << "// " << i << " \n";
		}while( NotEndMarker( &struct_array[i++] ) && i < maxCount );
		cout << "};\n";
		break; }
	// (rt) text output of an array of structs listing all fields, their types and offsets (addresses), for debugging 
	case OM_TXT: {	
		//cout.rdbuf
		cout << type_name << " at 0x" << hex << struct_array << dec <<" :\n\n";
		int i = 0; do{
			cout << i << " ";
			out_( &struct_array[i] );
			cout << '\n';
		}while( NotEndMarker( &struct_array[i++] ) && i < maxCount );
		break; }
	case OM_XLS: cout << "Error: Excel out mode not yet implemented; "; break;
	case OM_XML: cout << "Error: XML out mode not yet implemented; "; break;
	default: cout << "Error: Out mode id = " << CurOutMode << " not defined; "; break;
	}
}

// (rt) output a root array of a plain type in C++ format - { element_1, element_2, ... }
// (rt) together with the struct header in a comment and the C module header 
// (rt) T* structArray - array address 
COMN_FUNC( T, is_fundamental, void ) out_array(T* type_array, int maxCount = MAX_ARRAY_COUNT)
{
	// (rt) moving it out also needs the address of out_
	static const char* type_name = typeid( *type_array ).name();
	// (rt) .cpp header 
	//cout << UTF8_BOM << "#include \"th1_stdafx.h\"\n\n";
	cout << "// " << type_name << " array at 0x" << hex << (uint)type_array << dec << " \n";
	cout << type_name << ' ' << type_name << "s[" << maxCount << "] = {\n";
	for( int i = 0; i < maxCount; i++ ){
		out_( &type_array[i] );
		cout << ",\t";
		if( i % 20 == 19 ){
			cout << '\n';
		}
	}
	cout << "};\n";
}

// (rt) output a root array of a pointer type in C++ format - { element_1, // index ... }
// (rt) together with the struct header in a comment and the C module header 
// (rt) T* structArray - array address 
COMN_FUNC( T, is_pointer, void ) out_array( T* type_array, int maxCount = MAX_ARRAY_COUNT )
{
	// (rt) moving it out also needs the address of out_
	static const char* type_name = typeid(*type_array).name();
	// (rt) .cpp header 
	//cout << UTF8_BOM << "#include \"th1_stdafx.h\"\n\n";
	cout << "// " << type_name << " array at 0x" << hex << (uint) type_array << dec << " \n";
	cout << type_name << ' ' << type_name << "s[" << maxCount << "] = {\n";
	for( int i = 0; i < maxCount; i++ ){
		cout << "/* " << i << "\t*/\t";
		out_( &type_array[ i ] );
		cout << ",\n";
	}
	cout << "};\n";
}

// (rt) specialization for outputting arrays of types by string name
reflect(char);
reflect(uchar);
reflect(char*);
reflect(int);
reflect(uint);
reflect(short);
reflect(ushort);

// (rt) find a struct by name in the list of serializable types (declared with the struct_ or type_ macro) and output it 
// (rt) !!! This function must be a template, otherwise it compiles before the external specializations, with an empty list of structs 
// (rt) the type-independent parts are moved into the .cpp, except this one (and export template was removed from the C++ standard altogether)
template<class T> void OutArrayByName(char* type_name, void* address, T count = MAX_ARRAY_COUNT )
{
	// (rt) the metadata of serializable types is queried inside the function 
	// (rt) so that by the time of the query, serialization of types declared anywhere has been instantiated 
	static const meta_root* const serializedStructs = types_<>::meta();
	for( int i = 0; i < MAX_REFLECT && serializedStructs[i].out_array; i++ ){
		if( !strcmp( serializedStructs[i].type_name, type_name ) ){
			serializedStructs[i].out_array( address, count );
			return;
		}
	}
	cout << "Type \"" << type_name << "\" not registered.\nCheck type name, not array name.\n";
}

// (rt) functions for accessing the function and variable tables
FuncTable& func_table();
VarTable& var_table();
char* func_name( size_t adr, int th = 1 );
char* var_name( size_t adr, int th = 1 );
int func_index( void* adr, int th = 1 );
int var_index( void* adr, int th = 1 );
SubInfo* func_info( char* name );
VarInfo* var_info( char* name );

// (rt) compare a struct
COMN_FUNC( T, is_class, bool ) dif_( T* var1, T* var2 )
{
	static const meta_* meta = members_<T>::meta<>();
	int count = members_count<T>;
	for( int m = 0; m < count; m++ ){
		if( meta[ m ].dif_( (char*) var1 + meta[ m ].offset, (char*) var2 + meta[ m ].offset ) ){
			return true;
		}
	}
	return false;
}

// (rt) compare a union
COMN_FUNC( T, is_union, bool ) dif_( T* var1, T* var2 )
{
	static const meta_* meta = members_<T>::meta<>();
	int count = members_count<T>;
	for( int m = 0; m < count; m++ ){
		if( meta[ m ].dif_( (char*) var1 + meta[ m ].offset, (char*) var2 + meta[ m ].offset ) ){
			return true;
		}
	}
	return false;
}
																		 
// (rt) compare an array 
COMN_FUNC( T, is_array, bool ) dif_( T* arr1, T* arr2 )
{
	if( is_fundamental<remove_all_extents_t<T>>::value ){
		// int a[ 3 ][ 3 ];
		// &a == a == *a == a[ 0 ] = &a[ 0 ] = &a[ 0 ][ 0 ]
		// (rt) dereferencing the array address only affects the type; there is no physical indirect memory access
		return memcmp( (void*) *arr1, (void*) *arr2, sizeof(T) ) != 0;
	}else{
		// (rt) checked in the debugger the element count and dereference types for two-dimensional arrays		
		// (rt) countof gets only the top dimension, and so does dereferencing
		for( size_t i = 0; i < countof( *arr1 ); ++i ){
			if( dif_( &(*arr1)[i], &(*arr2)[i] ) ){
				return true;
			}
		}
		return false;
	}
}

// (rt) do not remove the commented-out functions; they are marked as used in the comparison and are not used any more for now
COMN_FUNC( T, is_function, bool ) dif_val( T* var1, T* var2 ){ return var1 != var2; }
COMN_OTHER_FUNC( T, is_function, bool ) dif_val( T* var1, T* var2 ){ return dif_( var1, var2 ); }
SPEC_OTHER_FUNC( void, is_function, bool ) dif_val<void>( void* var1, void* var2 );
//SPEC_OTHER_FUNC( HWND__, is_function, bool ) dif_val<HWND__>( HWND__* var1, HWND__* var2 );
SPEC_OTHER_FUNC( IDirectSoundBuffer, is_function, bool ) dif_val<IDirectSoundBuffer>( IDirectSoundBuffer* var1, IDirectSoundBuffer* var2 );
//SPEC_OTHER_FUNC( IDirectDrawPalette, is_function, bool ) dif_val<IDirectDrawPalette>( IDirectDrawPalette* var1, IDirectDrawPalette* var2 );
//SPEC_OTHER_FUNC( IDirectDrawSurface, is_function, bool ) dif_val<IDirectDrawSurface>( IDirectDrawSurface* var1, IDirectDrawSurface* var2 );
//SPEC_OTHER_FUNC( IDirectDraw, is_function, bool ) dif_val<IDirectDraw>( IDirectDraw* var1, IDirectDraw* var2 );
SPEC_OTHER_FUNC( IDirectSound, is_function, bool ) dif_val<IDirectSound>( IDirectSound* var1, IDirectSound* var2 );

bool IsBadPtr( const void* p );
// (rt) compare a plain pointer
COMN_FUNC( T, is_pointer, bool ) dif_( T* var1, T* var2 )
{
	//static int count = 0;
	//++count;
	//if( count > MaxDifRecursion ){
	//	--count;
	//	return false;
	//	//__debugbreak(); // 
	//}
	if( (int)*var1 == 0 || (int)*var2 == 0 || IsBadPtr(*var1) || IsBadPtr(*var2) ){
		//--count;
		return *var1 != *var2;
	}else{
		FuncTable& func = func_table();
		int f = func_index( *var1 ); // (rt) var1 is assumed to be a th 1 address
		if( f != -1 ){
			//--count;
			return *var2 != func[ f ].adrNew;
		}
		//#ifdef SEEK_VAR_TABLE_REFERENCE
		VarTable& var = var_table();
		int v = var_index( *var1 ); // (rt) var1 is assumed to be a th 1 address
		if( v != -1 ){
			//--count;
			return *var2 != var[ v ].adrNew;
		}
		//#endif
		bool result = dif_val( *var1, *var2 );
		//--count;
		return result;
	}
}

// (rt) do not remove the commented-out functions; they are marked as used in the comparison and are not used any more for now
COMN_FUNC( T, is_function, void ) out_val( T* var, int inner = 0, int th = 1 ){ cout << "0x"; if( inner >= -100 ) cout << var; }
COMN_OTHER_FUNC( T, is_function, void ) out_val( T* var, int inner = 0, int th = 1 ){ out_( var, inner, th ); }
SPEC_OTHER_FUNC( void, is_function, void) out_val<void>( void* var, int inner, int th );
//SPEC_OTHER_FUNC( HWND__, is_function, void ) out_val<HWND__>( HWND__* var, int inner, int th );
//SPEC_OTHER_FUNC( HINSTANCE__, is_function, void ) out_val<HINSTANCE__>( HINSTANCE__* var, int inner, int th );
SPEC_OTHER_FUNC( IDirectSoundBuffer, is_function, void ) out_val<IDirectSoundBuffer>( IDirectSoundBuffer* var, int inner, int th );
//SPEC_OTHER_FUNC( IDirectDrawPalette, is_function, void ) out_val<IDirectDrawPalette>( IDirectDrawPalette* var, int inner, int th );
//SPEC_OTHER_FUNC( IDirectDrawSurface, is_function, void ) out_val<IDirectDrawSurface>( IDirectDrawSurface* var, int inner, int th );
//SPEC_OTHER_FUNC( IDirectDraw, is_function, void ) out_val<IDirectDraw>( IDirectDraw* var, int inner, int th );
SPEC_OTHER_FUNC( IDirectSound, is_function, void ) out_val<IDirectSound>( IDirectSound* var, int inner, int th );
SPEC_OTHER_FUNC( IUnknown, is_function/*is_base_of*/, void ) out_val<IUnknown>( IUnknown* var, int inner, int th );
SPEC_OTHER_FUNC( GUID, is_function, void ) out_val<GUID>( GUID* var, int inner, int th );

SPEC_FUNC(FILE*, is_pointer, void) out_< FILE* >(FILE* *, int, int); // (rt) stub FILE*
SPEC_FUNC(HINSTANCE__*, is_pointer, void) out_< HINSTANCE__* >(HINSTANCE__* *, int, int); // (rt) stub HINSTANCE__*
SPEC_FUNC(HWND__*, is_pointer, void) out_< HWND__* >(HWND__* *, int, int); // (rt) stub HWND__*
//(rt) SPEC_FUNC(MMRESULT*, is_pointer, void) out_< MMRESULT* >(MMRESULT* *, int, int); // stub MMRESULT*

// (rt) output a pointer to:
//	(rt) 1)	a fundamental type (except char), a struct - the variable's address is output
//		(rt) a flag for output by value is probably needed
//	(rt) 2)	the address of a function / global static variable - the function / variable name is output
COMN_FUNC( T, is_pointer, void ) out_( T* var, int inner /*= 0*/, int th /*= 1*/ )
{
	if( (int)*var == 0 ){
		cout << '0';
	}else if( IsBadPtr(*var) ){
		cout << "0x" << hex << (uint) *var << dec;
	}else{
		const char* name = func_name( (size_t) *var, th );
		//#ifdef SEEK_VAR_TABLE_REFERENCE
		if( !name ){
			name = var_name( (size_t) *var, th );
		}
		//#endif
		if( name ){
			cout << name;
			return;
		}
		// (rt) add address lookup among static variables
		// (rt) add registration of all dynamic memory, like static variables in var_table
		// (rt) add address lookup among dynamic variables
		//cout << "0x" << *var; 
		out_val( *var , inner, th );
	}
}

// (rt) make an adapter function template with template parameters: argument count and address argument
// (rt) make specializations of this template for 1, 2, 3, 4 arguments and a generic template for 5 and more
// Watcom fastcall:                                 arg3 arg4 
// (rt) the first 4 in registers left to right: EAX, EDX, EBX, ECX, arg5, arg6, arg7; the rest on the stack right to left (msvc fastcall is also right to left)
// (rt) the callee cleans the stack (msvc fastcall: also the callee), return via the EAX register (msvc too)

// (rt) C++ cannot partially specialize functions; all functions with the same name are overloads
// (rt) for some reason msvc doesn't allow naked static member functions, although it looks permissible
// (rt) for these reasons two options remain: either check a runtime argument counter
// (rt) or pick the function in a conditional expression, but that will probably still generate all variants for each function
template<size_t addr, size_t arg> __declspec(naked) void ms_fastcall_to_watcom()
{
	enum: size_t { adr = addr, args = arg, args4 = args * 4 };
	__asm{
		mov eax, args
		cmp eax, 4
		je _4
		jg _5_more
		cmp eax, 2
		jg _3
	// (rt) 2 and 1 :
		mov eax, ecx			// (rt) set argument 1
		push adr
		ret						// (rt) called the original
	_3 :
		xchg ebx, [ esp ]		// (rt) ret into ebx, and ebx saved for restoring
		xchg ebx, [ esp + 4 ]	// (rt) argument 3 into ebx, and ret saved for returning
		mov eax, ecx
		jmp _call
	_4 :
		xchg ebx, [ esp + 4 ]	// (rt) set argument 3, and ebx saved for restoring
		mov eax, [ esp ]		// (rt) return address
		xchg eax, ecx			// (rt) set argument 1, and the return address into ecx
		xchg ecx, [ esp + 8 ]	// (rt) set argument 4, and put the return address before the saved ebx
		add esp, 4				// (rt) removed the original return address from the stack
		jmp _call
	_5_more :
		push ecx
		push ebx
		mov ebx, [ esp + 4 + 8 ] // (rt) arg 3 into ebx
		mov ecx, [ esp + 8 + 8 ] // (rt) arg 4 into ecx

		push edx			// (rt) save edx (arg 2)
		mov eax, 1
	_shift:
		mov edx, [ esp + eax * 4 + 8 + 12 ] // (rt) shift the argument list starting from 5
		mov[ esp + eax * 4 + 12 ], edx		// (rt) down by two positions
		inc eax
		cmp eax, args - 4
		jle _shift
		pop edx				// (rt) restore edx (arg 2)

		pop eax				// ebx
		mov[ esp + args4 - 12 + 4 ], eax // (rt) save ebx
		mov eax, [ esp + 4 ]	// (rt) return address
		mov[ esp + args4 - 8 + 4 ], eax // (rt) return address before the saved ebx
		pop eax				// (rt) argument 1 (from ecx)
		add esp, 4			// (rt) removed the original return address from the stack
	_call:
		call _store_eip		// (rt) saved eip
	_store_eip :
		add dword ptr[ esp ], 10	// (rt) computed the new return address
		push adr
		ret					// (rt) called the original
	// (rt) the new return address is here:
		pop ebx				// (rt) restored ebx
		ret					// (rt) returned to the original return address
	}
}

template<typename F> __forceinline void mov_ecx(F adr)
{
	__asm mov ecx, adr
}

template<typename F, F func, size_t arg> __declspec(naked) void watcom_fastcall_to_ms()
{
	enum: size_t { args = arg };
	__asm{
		push eax				// (rt) saved argument 1
		mov eax, args
		cmp eax, 4
		jge _4_more
		cmp eax, 2
		jg _3
	// (rt) 2 and 1 :
		pop eax					// (rt) restored argument 1
		push ecx				// (rt) saved ecx for restoring
		jmp _call
	_3 :
		pop eax					// (rt) restored argument 1
		xchg ebx, [ esp ]		// (rt) ret into ebx and set argument 3
		push ebx				// (rt) saved ret for returning
		push ecx				// (rt) saved ecx for restoring
		jmp _call
	_4_more :
		pop eax					// (rt) restored argument 1
		xchg ecx, [ esp ]		// (rt) ret into ecx and set argument 4
		push ebx				// (rt) set argument 3
		push ecx				// (rt) saved ret for returning
	}
		mov_ecx( func );		// (rt) the original's address into ecx
	__asm{
		push ecx
		mov ecx, eax			// (rt) set argument 1
		ret						// (rt) called the original
	_call:
		call _store_eip			// (rt) saved eip
	_store_eip :
		add dword ptr[ esp ], 13// (rt) computed the new return address
	}
		mov_ecx( func );		// (rt) the original's address into ecx
	__asm{
		push ecx
		mov ecx, eax			// (rt) set argument 1
		ret						// (rt) called the original
	// (rt) the new return address is here:
		pop ecx					// (rt) restored ecx
		ret						// (rt) returned to the original return address
	}
}

// (rt) Attention!!! For functions without arguments use the _EXE() macros
#define EXE_WC( name ) decltype(::name)* name = th_ == Th1id ? (decltype(::name)*)ms_fastcall_to_watcom<Exe::adr_##name,ArgsCount(::name)> : ::name;
#define WC_TO_MS( name ) watcom_fastcall_to_ms< decltype(&name), &name, ArgsCount(name)>
#define INS_JMP_WC(from, to ) INS_JMP( from, WC_TO_MS(to) )
#define INS_CALL_WC( from, to ) INS_CALL( from, WC_TO_MS(to) )

// (rt) cross macro: automatically substitutes references to functions and global variables depending on the execution thread, TH 1 or TH 2
#define EXE( name ) decltype(::name)* p_##name = th_ == Th1id ? &Exe::name : &::name; decltype(::name)& name = *p_##name;
#define _EXE( name ) uint th_ = GetCurrentThreadId(); EXE( name )
#define _EXE_WC( name ) uint th_ = GetCurrentThreadId(); EXE_WC( name )
#define isEXE (th_ == Th1id)

//#define EXE( name ) using Exe::name;
//#define _EXE( name ) uint th_ = Th1id; EXE( name )
//#define _EXE_WC( name ) uint th_ = Th1id; EXE_WC( name )
