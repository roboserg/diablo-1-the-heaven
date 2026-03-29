#ifndef _sync_h_
#define _sync_h_
#else
#error reinclude sync.h
#endif

#pragma pack(push, 1)

struct TSyncMonster
{
	uchar  monsterIndex;
	uchar  monsterIndex_reserved;
	uchar  row;
	uchar  row_reserved;
	uchar  col;
	uchar  col_reserved;
	uchar  targetIndex;
	uchar  targetIndex_reserved;
	uchar  activationTimer;
};

//static_assert( sizeof( TSyncMonster ) == 5, "Wrong TSyncMonster size" );

#pragma pack(pop)

DWORD sync_packData( const uchar* pbBuf, DWORD maxSize );
DWORD sync_applyData( int playerIndex, const uchar* pbBuf);
void sync_init();
