//#pragma once
#ifndef _serialize_h_
#define _serialize_h_
#else
#error reinclude serialize.h
#endif

// модуль сериализации любых бинарных данных (типов, структур, классов )                          
// сериализация это вывод бинарных данных в процессоро-независмомо текстовом виде                 
// для переноса между программами и платформами                                                   
// данные для сериализации получаются методом полуавтоматической рефлексии из объявления стуктуры 
// с помощью макросов begin_, _, end_, reflect, reflect_enum                                      
// вложенные структуры должны быть также предварительно объявлены этими макросами                 
// пример объявления сериализуемой структуры:                                                     
// 
// для вывода вложенных структур специализирована глобальная функция out_ для всех 
// сериализуемых типов и задавется параметр формата вывода OutMode 

#include "reflect.h"

// Форматы выгрузки 
enum OutMode { OM_CPP, OM_TXT, OM_XLS, OM_XML };
extern char* OutModes[4];
extern int CurOutMode; // = OM_CPP; // текущий формат выгрузки данных 
extern bool IgnoreEndMarker; // Флаг игнорирования конца списка для принудительной выгрузки заданного числа элементов
extern int MaxDifRecursion;
extern bool OutElementAlwaysOnNewLine;

// поиск значения enum по имени (надо переделать на рефлексию Enum)
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

// пробный вынос основного тела функции в модуль пока убран, Clang сходит с ума
// вывод структуры, класса, объединения: T* var - адрес структуры, int inner - уровень вложенности структуры 
template<class T> __forceinline void out_class_union( T* var, int inner, int th )
{
	static const meta_* meta = members_<T>::meta<>();
	if( inner > MaxDifRecursion ) return;
	int count = members_count<T>;
	switch( CurOutMode ){
	// вывод структуры в формате С++ _{ field_1, field_2, ... }, 
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
	// текстовый вывод структуры с указанием всех полей их типов и смещений для отладки 
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

// вывод массива являющегося полем структуры
COMN_FUNC( T, is_array, void ) out_( T* arr, int inner, int th )
{
	if( inner > MaxDifRecursion) return;
	// не получилось вынести, т.к. для этого нужна функция возврата указателя на функцию вывода элемента
	switch( CurOutMode ){
	case OM_CPP: // вывод массива в формате С++
	case OM_TXT: { // текстовый вывод массива 
		int i = 0;
		auto var = begin( *arr );
		auto last = end( *arr ) - 1;
		while( var < last ){
			// добавлен вывод двумерного массива как двумерного
			// структурные выводятся в один столбик, так как структура сама линия
			if( is_array<remove_extent_t<T>>::value
			 || is_class<remove_extent_t<T>>::value
			 || is_union<remove_extent_t<T>>::value ){
				if( inner >= 0 ) cout << '\n'; for( int i = 0; i <= inner; ++i ) cout << '\t';
				cout << "/*" << i++ << "*/\t";
			//}else if( true /* CutArrayOut */ ){ // надо добавить опцию в конфиг для вывода длинных массивов
			//	if( i++ % 128 == 0 ){
			//		cout << '\n';
			//		// вывод индексов одномерного массива
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

// вывод перечисления (enum) 
COMN_FUNC( T, is_enum, void ) out_( T* var, int inner, int th )
{
	// проверка на наличии bit_enum operator|
	#pragma warning(suppress: 4555)
	if constexpr( is_same< decltype(T()|T()), T >::value ){
		// добавляю вывод битовых перечислений
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


// если есть маркер конца списка проверяем его 
// специализация если есть маркер
template<class T> __forceinline bool NotEndMarker( T* structArray, HasMember( T::EndMarker ) )
{
	return IgnoreEndMarker || structArray->EndMarker != -1;
}
// специализация если нет маркера 
__forceinline bool NotEndMarker( ... )
{
	return true;
}

// вывод корневого массива структур в заданном глобальной переменной OutMode формате 
// T* structArray - адрес массива, int maxCount - максимальное количество элементов
COMN_FUNC( T, is_class, void ) out_array( T* struct_array, int maxCount = MAX_ARRAY_COUNT)
{
	// запрос метаинформации о полях структуры 
	static const meta_* meta = members_<T>::meta<>();
	static const char* type_name = typeid(T).name();
	// аналогично, для выноса нужен адрес out_<decltype(&structArray[0])>, не буду пока огород городить
	switch( CurOutMode ){
	// вывод массива структур в формате С++ - {{ field_1, field_2 }, {...} }
	// вместе с шапкой структуры в комментарии и заголовком с модуля 
	case OM_CPP: {
		// шапка .cpp 
		//cout << UTF8_BOM << "#include \"th1_stdafx.h\"\n\n";
		cout << "// " << type_name << " array at 0x" << hex << struct_array << dec << " \n";
		cout << type_name << ' ' << type_name;
		if( type_name[ strlen(type_name) - 1 ] == 'x' || *(short*)&type_name[ strlen(type_name) - 2 ] == 'hc' ){
			cout << "e";
		}
		cout << "s[] = {\n";
		// шапка структуры (перечисление имен полей) 
		cout << "\t// " << meta[0].field_name;
		//cout << " " << meta[0].offset;
		//cout << " " << meta[0].field_type;
		for( int i = 1; i < members_count<T>; i++ ){
			cout << ", " << meta[i].field_name;
			//cout << " " << meta[i].offset;
			//cout << " " << meta[i].field_type;
		}
		cout << '\n';
		// вывод данных структуры в цикле до EndMarker == -1 
		int i = 0; do{
			cout << "/* " << i << "\t*/ "; //cout << "\t";
			out_( &struct_array[i] );
			cout << '\n'; //cout << "// " << i << " \n";
		}while( NotEndMarker( &struct_array[i++] ) && i < maxCount );
		cout << "};\n";
		break; }
	// текстовый вывод массива структур с указанием всех полей их типов и смещений (адресов) для отладки 
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

// вывод корневого массива plain типа в формате С++ - { element_1, element_2, ... }
// вместе с шапкой структуры в комментарии и заголовком си модуля 
// T* structArray - адрес массива 
COMN_FUNC( T, is_fundamental, void ) out_array(T* type_array, int maxCount = MAX_ARRAY_COUNT)
{
	// тоже адрес out_ для выноса нужен
	static const char* type_name = typeid( *type_array ).name();
	// шапка .cpp 
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

// вывод корневого массива pointer типа в формате С++ - { element_1, // index ... }
// вместе с шапкой структуры в комментарии и заголовком си модуля 
// T* structArray - адрес массива 
COMN_FUNC( T, is_pointer, void ) out_array( T* type_array, int maxCount = MAX_ARRAY_COUNT )
{
	// тоже адрес out_ для выноса нужен
	static const char* type_name = typeid(*type_array).name();
	// шапка .cpp 
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

// специализация для вывода массивов типов по строковому имени
reflect(char);
reflect(uchar);
reflect(char*);
reflect(int);
reflect(uint);
reflect(short);
reflect(ushort);

// поиск структуры по имени в списке сериализуемых типов (объявленных макросом struct_ или type_) и ее вывод 
// !!! Эта функция должна быть шаблонной, иначе она скомпилируется до внешних спецификаций, с пустым списком структур 
// в .cpp переносятся независимые от типа части, кроме этой (а export template вообще убран из стандарта c++)
template<class T> void OutArrayByName(char* type_name, void* address, T count = MAX_ARRAY_COUNT )
{
	// запрос метаинформации о серилизуемых типах производится внутри функции 
	// что-бы к моменту запроса сериализация, где угодно объявленных типов, была инстанцирована 
	static const meta_root* const serializedStructs = types_<>::meta();
	for( int i = 0; i < MAX_REFLECT && serializedStructs[i].out_array; i++ ){
		if( !strcmp( serializedStructs[i].type_name, type_name ) ){
			serializedStructs[i].out_array( address, count );
			return;
		}
	}
	cout << "Type \"" << type_name << "\" not registered.\nCheck type name, not array name.\n";
}

// функции для доступа к таблицам функций и переменных
FuncTable& func_table();
VarTable& var_table();
char* func_name( size_t adr, int th = 1 );
char* var_name( size_t adr, int th = 1 );
int func_index( void* adr, int th = 1 );
int var_index( void* adr, int th = 1 );
SubInfo* func_info( char* name );
VarInfo* var_info( char* name );

// сравнение структуры
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

// сравнение объединения
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
																		 
// сравнение массива 
COMN_FUNC( T, is_array, bool ) dif_( T* arr1, T* arr2 )
{
	if( is_fundamental<remove_all_extents_t<T>>::value ){
		// int a[ 3 ][ 3 ];
		// &a == a == *a == a[ 0 ] = &a[ 0 ] = &a[ 0 ][ 0 ]
		// разыменование адреса массива влияет только на тип, физического косвенного обращения к памяти нет
		return memcmp( (void*) *arr1, (void*) *arr2, sizeof(T) ) != 0;
	}else{
		// проверил в отладке количество элементов и типы разыменования для двумерных массивов		
		// countof получает только верхнее измерение, разыменование тоже
		for( size_t i = 0; i < countof( *arr1 ); ++i ){
			if( dif_( &(*arr1)[i], &(*arr2)[i] ) ){
				return true;
			}
		}
		return false;
	}
}

// закомменченные функции не убирать, он помеченны как использованные при сверке и пока больше не используются
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
// сравнение обычного указателя
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
		int f = func_index( *var1 ); // предполагается что var1 это адрес th 1
		if( f != -1 ){
			//--count;
			return *var2 != func[ f ].adrNew;
		}
		//#ifdef SEEK_VAR_TABLE_REFERENCE
		VarTable& var = var_table();
		int v = var_index( *var1 ); // предполагается что var1 это адрес th 1
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

// закомменченные функции не убирать, он помеченны как использованные при сверке и пока больше не используются
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

SPEC_FUNC(FILE*, is_pointer, void) out_< FILE* >(FILE* *, int, int); // заглушка FILE*
SPEC_FUNC(HINSTANCE__*, is_pointer, void) out_< HINSTANCE__* >(HINSTANCE__* *, int, int); // заглушка HINSTANCE__*
SPEC_FUNC(HWND__*, is_pointer, void) out_< HWND__* >(HWND__* *, int, int); // заглушка HWND__*
//SPEC_FUNC(MMRESULT*, is_pointer, void) out_< MMRESULT* >(MMRESULT* *, int, int); // заглушка MMRESULT*

// вывод указателя на:
//	1)	фундаментальный тип (кроме char), структуру - выводится адрес переменной
//		вероятно нужен флаг для вывода по значению
//	2)	адрес функции/глобальной статической переменной - выводится имя функции/переменной
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
		// добавить поиск адреса среди статических переменных
		// добавить регистрацию всей динамической памяти, аналогично статическим переменных в var_table
		// добавить поиск адреса среди динамических переменных
		//cout << "0x" << *var; 
		out_val( *var , inner, th );
	}
}

// сделать шаблон функции-переходника в котором есть шаблонные параметры: количество аргументов и аргумент адрес
// сделать спецификации этого шаблона для 1,2,3,4 аргументов и универсальный шаблон для 5 и больше
// Watcom fastcall:                                 arg3 arg4 
// первые 4 через регистры слева направо: EAX, EDX, EBX, ECX, arg5, arg6, arg7; остальные через стек справа налево (msvc fastcall тоже справа налево)
// стек чистит вызываемая программа (msvc fastcall тоже вызываемая), возврат через регистр EAX (msvc тоже)

// c++ не умеет частичную специализацию функций, все фукнции с одинаковым именем это перегруженные функции
// msvc почему то не позволяет naked static member функций, хотя это и выглядит допустимым
// по этим причинам остается два варианта, либо проверять реалтайм счетчик аргументов
// либо выбирать функцию в условном выражении, но это возможно все равно приведет к генерации всех вариантов для каждой функции
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
	// 2 и 1 :
		mov eax, ecx			// установили 1 аргумент
		push adr
		ret						// вызвали оригинал
	_3 :
		xchg ebx, [ esp ]		// в ebx ret и ebx сохранили для восстановления
		xchg ebx, [ esp + 4 ]	// в ebx 3 аргумент и ret сохранили для возврата
		mov eax, ecx
		jmp _call
	_4 :
		xchg ebx, [ esp + 4 ]	// 3 аргумент установили и ebx сохранили для восстановления
		mov eax, [ esp ]		// адрес возврата
		xchg eax, ecx			// 1 аргумент установили и в ecx адрес возврата
		xchg ecx, [ esp + 8 ]	// 4 аргумент установили и адрес возврата поместили перед сохраненным ebx
		add esp, 4				// убрали со стека оригинальный адрес возврата
		jmp _call
	_5_more :
		push ecx
		push ebx
		mov ebx, [ esp + 4 + 8 ] // arg 3 в ebx
		mov ecx, [ esp + 8 + 8 ] // arg 4 в ecx

		push edx			// edx (arg 2) сохраняем
		mov eax, 1
	_shift:
		mov edx, [ esp + eax * 4 + 8 + 12 ] // сдвиг списка аргументов начиная с 5
		mov[ esp + eax * 4 + 12 ], edx		// на две позиции вниз
		inc eax
		cmp eax, args - 4
		jle _shift
		pop edx				// edx (arg 2) восстанавливаем

		pop eax				// ebx
		mov[ esp + args4 - 12 + 4 ], eax // сохраняем ebx
		mov eax, [ esp + 4 ]	// адрес возврата
		mov[ esp + args4 - 8 + 4 ], eax // адрес возврата перед сохраненным ebx
		pop eax				// 1 аргумент (из ecx)
		add esp, 4			// убрали со стека оригинальный адрес возврата
	_call:
		call _store_eip		// сохранили eip
	_store_eip :
		add dword ptr[ esp ], 10	// вычислили новый адрес возврата
		push adr
		ret					// вызвали оригинал
	// новый адрес возврата тут:
		pop ebx				// восстановили ebx
		ret					// вернулись на оригинальный адрес возврата
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
		push eax				// сохранили 1 аргумент
		mov eax, args
		cmp eax, 4
		jge _4_more
		cmp eax, 2
		jg _3
	// 2 и 1 :
		pop eax					// восстановили 1 аргумент
		push ecx				// ecx сохранили для восстановления
		jmp _call
	_3 :
		pop eax					// восстановили 1 аргумент
		xchg ebx, [ esp ]		// в ebx ret и установили 3 аргумент
		push ebx				// ret сохранили для возврата
		push ecx				// ecx сохранили для восстановления
		jmp _call
	_4_more :
		pop eax					// восстановили 1 аргумент
		xchg ecx, [ esp ]		// в eсx ret и установили 4 аргумент
		push ebx				// установили 3 аргумент
		push ecx				// ret сохранили для возврата
	}
		mov_ecx( func );		// в ecx адрес оригинала
	__asm{
		push ecx
		mov ecx, eax			// установили 1 аргумент
		ret						// вызвали оригинал
	_call:
		call _store_eip			// сохранили eip
	_store_eip :
		add dword ptr[ esp ], 13// вычислили новый адрес возврата
	}
		mov_ecx( func );		// в ecx адрес оригинала
	__asm{
		push ecx
		mov ecx, eax			// установили 1 аргумент
		ret						// вызвали оригинал
	// новый адрес возврата тут:
		pop ecx					// восстановили ecx
		ret						// вернулись на оригинальный адрес возврата
	}
}

// Внимание!!! Для функций без аргументов следует применять макросы _EXE()
#define EXE_WC( name ) decltype(::name)* name = th_ == Th1id ? (decltype(::name)*)ms_fastcall_to_watcom<Exe::adr_##name,ArgsCount(::name)> : ::name;
#define WC_TO_MS( name ) watcom_fastcall_to_ms< decltype(&name), &name, ArgsCount(name)>
#define INS_JMP_WC(from, to ) INS_JMP( from, WC_TO_MS(to) )
#define INS_CALL_WC( from, to ) INS_CALL( from, WC_TO_MS(to) )

// кросс-макрос, автоматически подменяет ссылку на фунции и глобальные переменные в зависимости от потока выполнения TH 1 или TH 2
#define EXE( name ) decltype(::name)* p_##name = th_ == Th1id ? &Exe::name : &::name; decltype(::name)& name = *p_##name;
#define _EXE( name ) uint th_ = GetCurrentThreadId(); EXE( name )
#define _EXE_WC( name ) uint th_ = GetCurrentThreadId(); EXE_WC( name )
#define isEXE (th_ == Th1id)

//#define EXE( name ) using Exe::name;
//#define _EXE( name ) uint th_ = Th1id; EXE( name )
//#define _EXE_WC( name ) uint th_ = Th1id; EXE_WC( name )
