#include "stdafx.h"

#pragma pack(push, 1)
typedef struct TMsg TMsg;

typedef struct TMsgHdr
{
    TMsg* pNext;
    int dwTime;
    uchar bLen;
} TMsgHdr;

typedef struct TMsg
{
    TMsgHdr hdr;
    // this is actually alignment padding, but the message body is appended to the struct
    // so it's convenient to use byte-alignment and name it "body"
    unsigned char body[3];
} TMsg;
#pragma pack(pop)

static TMsg* sgpTimedMsgHead;

//----- (0047336E) --------------------------------------------------------
int tmsg_get( uchar* pbMsg, DWORD dwMaxLen )
{
    int len = 0;
	if( sgpTimedMsgHead && sgpTimedMsgHead->hdr.dwTime < ms(GetTickCountTh()) ){
		TMsg* head = sgpTimedMsgHead;
		sgpTimedMsgHead = head->hdr.pNext;
		len = head->hdr.bLen;
		// BUGFIX: ignores dwMaxLen
		memcpy( pbMsg, head->body, len );
		FreeMem( head );
	}
    return len;
}

//----- (th2) -------------------------------------------------------------
void tmsg_add( uchar* pbMsg, uchar bLen )
{
    TMsg** tail;
    
    TMsg* msg = (TMsg*)AllocMem( bLen + sizeof( *msg ) );
    msg->hdr.pNext = nullptr;
    msg->hdr.dwTime = ms(GetTickCountTh() + CatchUpDelta_500);
    msg->hdr.bLen = bLen;
    memcpy( msg->body, pbMsg, bLen );
    for( tail = &sgpTimedMsgHead; *tail; tail = &( *tail )->hdr.pNext );
    *tail = msg;
}

//----- (th2) -------------------------------------------------------------
void tmsg_start()
{
    /// ASSERT: assert(! sgpTimedMsgHead);
}

//----- (0047340C) --------------------------------------------------------
void tmsg_cleanup()
{
    while( sgpTimedMsgHead )
    {
        TMsg* next = sgpTimedMsgHead->hdr.pNext;
        FreeMem( sgpTimedMsgHead );
        sgpTimedMsgHead = next;
    }
}
