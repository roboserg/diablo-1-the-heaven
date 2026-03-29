#ifndef _tmsg_h_
#define _tmsg_h_
#else
#error reinclude tmsg.h
#endif

// Temp storage for messages that were received while player join stage
// They will be read after loading stage is done

int tmsg_get( uchar* pbMsg, DWORD dwMaxLen );
void tmsg_add( uchar* pbMsg, uchar bLen );
void tmsg_start();
void tmsg_cleanup();
