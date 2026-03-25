#include "stdafx.h"

// разделение строки на массив строк (в том числе для парсина списка имен элементов enum)
//----- (th2) -------------------------------------------------------------
char** enum_split( char* names, size_t count /*=2048*/ )
{
	char** name = new char*[ count + 1 ];
	bool name_begin = true;
	size_t i = 0;
	while( *names ){
		switch( *names ){
		case ',': name_begin = true; // идем ниже, break не нужен
		case ' ': *names = 0; break; // табы и переводы строк из списка убираются компилятором (проверено msvc)
		default: if( name_begin ){
			name[ i++ ] = names; name_begin = false;
		}
		}
		++names;
		if( i >= count ) break;
	}
	name[ i ] = 0;
	return name;
}

#ifdef NET_LOG
#include <Intrin.h>
#pragma intrinsic(_ReturnAddress)
volatile int LogNum = 0;
thread_local ofstream LogFile;
thread_local char CallStack[8][128];
thread_local int CurCall = 0;

#define HEX( x ) setw(2) << setfill('0') << hex << (int)( x )
#define OPEN_LOG if( !LogFile.is_open() ) LogFile.open("net"s+to_string(++LogNum)+".log")

//----- (th2) -------------------------------------------------------------
__forceinline void OutCalls()
{
	OPEN_LOG;
	for( int i = 1; i <= 8; i++ ){
		if( *CallStack[(CurCall + i) & 7] ) LogFile << CallStack[(CurCall + i) & 7] << '\n';
	}
}

//----- (th2) -------------------------------------------------------------
int __stdcall NetReceiveMessage(unsigned int* senderPlayerIndex, void** packet, unsigned int* size)
{
	OutCalls();
	LogFile << __func__ << '\n';
	auto result = eSNetReceiveMessage(senderPlayerIndex, packet, size);
	if( result ){
		LogFile << "    senderPlayerIndex: " << *senderPlayerIndex << '\n';
		LogFile << "    size: " << *size << '\n';
		LogFile << "    packet:";
		for( uint i = 0; i < *size; ){
			LogFile << ' ' << HEX(((uchar*)*packet)[i]);
			if( !(++i % 32) || i == *size ) LogFile << "\n           ";
		}
		LogFile << dec;
	}
	LogFile << '\n';
	return result;
}

//----- (th2) -------------------------------------------------------------
int __stdcall NetReceiveTurns(unsigned int firstPlayerId, int arraySize, void *arraydata, void *arrayDataBytes, void *arrayPlayerStatus)
{
	OutCalls();
	LogFile << __func__ << '\n';
	auto result = eSNetReceiveTurns(firstPlayerId, arraySize, arraydata, arrayDataBytes, arrayPlayerStatus);
	if( result ){
		//LogFile << "firstPlayerId: " << firstPlayerId << '\n';
	}
	LogFile << '\n';
	return result;
}

//----- (th2) -------------------------------------------------------------
int __stdcall NetSendMessage(int playerID, void *packet, size_t size)
{
	OutCalls();
	LogFile << __func__ << '\n';
	LogFile << "    playerID: " << playerID << '\n';
	LogFile << "    size: " << size << '\n';
	LogFile << "    packet:";
	for( uint i = 0; i < size; ){
		LogFile << ' ' << HEX(((uchar*)packet)[i]);
		if( !(++i % 32) || i == size ) LogFile << "\n           ";
	}
	LogFile << dec;
	LogFile << '\n';
	return eSNetSendMessage(playerID, packet, size);
}

//----- (th2) -------------------------------------------------------------
int __stdcall NetSendTurn(char *data, size_t databytes)
{
	OutCalls();
	LogFile << __func__ << '\n';
	LogFile << '\n';
	return eSNetSendTurn(data, databytes);
}

//----- (th2) -------------------------------------------------------------
extern "C" void __declspec(naked) __cdecl _penter( void ) {
	_asm {
		push eax
		push ebx
		push ecx
		push edx
		push ebp
		push edi
		push esi
	}
	if( not(_ReturnAddress(), (void*)NetReceiveMessage, (void*)NetReceiveTurns, (void*)NetSendMessage, (void*)NetSendTurn) ){
		// TraceCall
		//OPEN_LOG;
		//sprintf(CallStack[CurCall++], "%8.8x", (uint)_ReturnAddress());

		// BufCall
		sprintf(CallStack[CurCall++], "%8.8x", (uint)_ReturnAddress());
		CurCall &= 7;
	}
	_asm {
		pop esi
		pop edi
		pop ebp
		pop edx
		pop ecx
		pop ebx
		pop eax
		ret
	}
}
#endif

#ifdef SHOW_CALL_STACK

// use call: sw_show_callstack(g_stackwalk, NULL);

#undef strcat_s
#define SW_IMPL
#include "stackwalkerc.h"

static void symbol_init(const char* search_path, uint32_t sym_opts, void* userptr)
{
	printf("Search path: %s\n", search_path);
}

static void load_module(const char* img, const char* module, uint64_t base_addr, uint32_t size, void* userptr)
{
	printf("Load module:\n\timage: %s\n\tmodule: %s\n", img, module);
}

static void callstack_entry(const sw_callstack_entry* entry, void* userptr)
{
	printf("\t%s(%d): %s\n", entry->line_filename, entry->line, entry->und_name);
}

static void callstack_begin(void* userptr)
{
	puts("Callstack:");
}

static void callstack_end(void* userptr)
{
	puts("EndCallstack");
}

static sw_context* g_stackwalk;

void InitShowCallStack()
{
	sw_callbacks callbacks;
	callbacks.load_module = load_module;
	callbacks.callstack_begin = callstack_begin;
	callbacks.callstack_entry = callstack_entry;
	callbacks.callstack_end = callstack_end;
	callbacks.symbol_init = symbol_init;
	g_stackwalk = sw_create_context_capture(SW_OPTIONS_ALL, callbacks, NULL);
	if (!g_stackwalk) TerminateWithError("ERROR: stackwalk init");
	atexit( [](){ sw_destroy_context(g_stackwalk); } );
}

#endif