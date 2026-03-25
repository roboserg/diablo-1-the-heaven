#include "stdafx.h"

ShaInfo ShaInfos[3];

//----- (0046B48C) --------------------------------------------------------
void ShaDestroy()
{
	memzero(ShaInfos);
}

//----- (0046B4A1) --------------------------------------------------------
void __fastcall ShaGetLastHash( int streamNum, uchar* out )
{
	if( out ) memcpy(out, ShaInfos[streamNum].digest, sizeof(ShaInfo::digest));
}

//----- (0046B4C3) --------------------------------------------------------
void __fastcall ShaHash( int streamNum, uchar* in, uchar* out )
{
	UpdateHash(ShaInfos[streamNum], in, SHA1_BLOCK_SIZE);
	if( out ){
		ShaGetLastHash(streamNum, out);
	}
}

//----- (0046B4E9) --------------------------------------------------------
void __fastcall UpdateHash( ShaInfo& info, uchar* buffer, int size )
{
	if( info.countLo + (size << 3) < info.countLo ){
		info.countHi++;
	}
	info.countLo += size << 3;
	info.countHi += size >> 29;
	
	for( uchar* end = buffer + size; buffer < end; buffer += SHA1_BLOCK_SIZE ){
		memcpy(info.data, buffer, SHA1_BLOCK_SIZE);
		TransformHash(info);
	}
}

//----- (0046B53A) --------------------------------------------------------
void __fastcall TransformHash( ShaInfo& info )
{
	int w[80] {};
	// 16 слов по 32-бита дополняются до 80 32-битовых слов:
	memcpy(w, info.data, SHA1_BLOCK_SIZE);
	for( int i = 16; i < 80; i++ ){
		w[i] = w[i - 3] ^  w[i - 8] ^ w[i - 14] ^ w[i - 16];// циклический сдвиг влево 1
	}

	// Инициализация хеш-значений этой части:
	int a = info.digest[0];
	int b = info.digest[1];
	int c = info.digest[2];
	int d = info.digest[3];
	int e = info.digest[4];

	// Основной цикл:
	int f, k;
	#define S(n,x) ((x << n) | (x >> (32 - n)))
	#define CALC int temp = S(5, a) + f + e + k + w[i]; e = d; d = c; c = S(30, b); b = a; a = temp

	for( int i =  0; i < 20; ++i ){
		f = ~b & d | b & c;
		k = 0x5A827999;
		CALC;
	}
	for( int i = 20; i < 40; ++i ){
		f = b ^ c ^ d;
		k = 0x6ED9EBA1;
		CALC;
	}
	for( int i = 40; i < 60; ++i ){
		f = b & c | d & (b | c);
		k = 0x8F1BBCDC;
		CALC;
	}
	for( int i = 60; i < 80; ++i ){
		f = b ^ c ^ d;
		k = 0xCA62C1D6;
		CALC;
	}
	#undef S
	#undef CALC

	// Добавляем хеш-значение этой части к результату:
	info.digest[0] += a;
	info.digest[1] += b;
	info.digest[2] += c;
	info.digest[3] += d;
	info.digest[4] += e;
	
	memzero(w);
	a = b = c = d = e = 0;
}

//----- (0046B737) --------------------------------------------------------
void __fastcall ShaInitialize( int streamNum )
{
	ShaInfo& info = ShaInfos[streamNum];
	info.digest[0] = 0x67452301; // H0INIT
	info.digest[1] = 0xEFCDAB89; // H1INIT
	info.digest[2] = 0x98BADCFE; // H2INIT
	info.digest[3] = 0x10325476; // H3INIT
	info.digest[4] = 0xC3D2E1F0; // H4INIT
	info.countLo = 0;
	info.countHi = 0;
}
