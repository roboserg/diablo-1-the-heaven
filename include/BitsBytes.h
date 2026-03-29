#ifndef _bitsbytes_h_
#define _bitsbytes_h_
#else
#error reinclude bitsbytes.h
#endif

// work with bit masks
#define B(i)   (1ull<<(i))     // bit mask by index
#define BIT(n) (1ull<<((n)-1)) // bit mask by number
#define BIT_INTERVAL(x, y) (-1 << (x)) & (-1 >> (sizeof(int)*8-(y)))
#define ClearBit	( value, bitMask ) value	&= ~( bitMask )	// AND reverse
#define SetBit		( value, bitMask ) value	|=	( bitMask )	// OR 
#define CheckBit	( value, bitMask )(value	&	( bitMask ))// AND
#define ChangeBit	( value, bitMask ) value	^=	( bitMask )	// XOR
__forceinline DWORD MostBit( DWORD val ) { DWORD i = 0; _BitScanReverse( &i, val ); return i; } // версия без проверки на нулевое входное значение
//__forceinline DWORD MostBit( DWORD val ) { DWORD i; if( _BitScanReverse( &i, val ) ) return i; else return -1; } // если подали нуль, возвращаем -1

#define LOBYTE_IDA(x)   (*((uchar*)&(x)))   // low byte
#define LOWORD_IDA(x)   (*((ushort*)&(x)))   // low word
#define HIBYTE_IDA(x)   (*((uchar*)&(x)+1))
#define HIWORD_IDA(x)   (*((ushort*)&(x)+1))
#define BYTEn(x, n)   (*((uchar*)&(x)+n))
#define BYTE1(x)   BYTEn(x,  1)         // byte 1 (counting from 0)
#define BYTE2(x)   BYTEn(x,  2)
#define BYTE3(x)   BYTEn(x,  3)
