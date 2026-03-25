#include "stdafx.h"
/////////////////////////////////////////////////////////////////////////////
// Mini Web Server code
/////////////////////////////////////////////////////////////////////////////
#include "Server.h"

#ifdef WIN32
	#ifndef SYS_MINGW
		#define read _read
		#define open _open
		#define close _close
		#define lseek _lseek
		#define read _read
		#define write _write
		#define strdup _strdup
		#define dup2 _dup2
		#define dup _dup
		#define pipe _pipe
		#define spawnvpe _spawnvpe
		#define spawnvp _spawnvp
	#endif
	#define snprintf _snprintf
	#define stricmp _stricmp
	#define ssize_t size_t
	#define socklen_t int
	#ifndef HAVE_PTHREAD
		typedef HANDLE pthread_t;
		typedef HANDLE pthread_mutex_t;
	#endif
	typedef DWORD (WINAPI *PFNGetProcessId)(HANDLE hProcess);
#else
	#include <unistd.h>
	#include <netdb.h>
	#include <dirent.h>
	#include <signal.h>
	#include <sys/time.h>
	#include <sys/types.h>
	#include <sys/wait.h>
	#include <sys/select.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#define closesocket close
	#define CloseHandle close
	#define stricmp strcasecmp
	#if !defined(O_BINARY)
		#define O_BINARY 0
	#endif
	#define closesocket close
	#ifndef MAX_PATH
	#define MAX_PATH 256
	#endif
	#ifndef FALSE
	#define FALSE 0
	#endif
	#ifndef TRUE
	#define TRUE 1
	#endif
	typedef int SOCKET;
	typedef unsigned int DWORD;
	typedef unsigned short int WORD;
	typedef unsigned char BYTE;
	typedef int BOOL;
#endif

#ifdef SYS_MINGW
#ifndef WIN32
#define WIN32
#endif
#endif

#ifdef HAVE_PTHREAD
#include <pthread.h>
#endif

typedef unsigned char OCTET;

#define SHELL_NOREDIRECT 1
#define SHELL_SHOWWINDOW 2
#define SHELL_NOWAIT 4

#ifdef WIN32
#define msleep(ms) (Sleep(ms))
#else
#define msleep(ms) (usleep(ms<<10))
#endif


#ifdef __cplusplus
extern "C" {
#endif

	int InitSocket();
	void UninitSocket();
	char *GetTimeString();
	int ThreadCreate(pthread_t *pth, void* (*start_routine)(void*), void* arg);
	int ThreadKill(pthread_t pth);
	int ThreadWait(pthread_t pth, int timeout, void** ret);
	void MutexCreate(pthread_mutex_t* mutex);
	void MutexDestroy(pthread_mutex_t* mutex);
	void MutexLock(pthread_mutex_t* mutex);
	void MutexUnlock(pthread_mutex_t* mutex);
	int ReadDir(const char* pchDir, char* pchFileNameBuf);
	int IsFileExist(const char* filename);
	int IsDir(const char* pchName);

#ifdef __cplusplus
}
#endif

#define VER_MAJOR 1
#define VER_MINOR 0

#ifndef min
#define min(x,y) (x>y?y:x)
#endif

#ifdef HTTPD_DEBUG
#define DBG printf
#else
#define DBG
#endif
#define LOG_ERR 1

#define ASSERT
#define GETDWORD(ptrData) (*(DWORD*)(ptrData))
#define SETDWORD(ptrData,data) (*(DWORD*)(ptrData)=data)
#define GETWORD(ptrData) (*(WORD*)(ptrData))
#define SETWORD(ptrData,data) (*(WORD*)(ptrData)=data)
#ifndef BIG_ENDINE
#define DEFDWORD(char1,char2,char3,char4) ((char1)+((char2)<<8)+((char3)<<16)+((char4)<<24))
#define DEFWORD(char1,char2) (char1+(char2<<8))
#else
#define DEFDWORD(char1,char2,char3,char4) ((char4)+((char3)<<8)+((char2)<<16)+((char1)<<24))
#define DEFWORD(char1,char2) (char2+(char1<<8))
#endif

///////////////////////////////////////////////////////////////////////
// Public definitions
///////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

// file types
typedef enum {
  HTTPFILETYPE_UNKNOWN = 0,
  HTTPFILETYPE_HTML,
  HTTPFILETYPE_XML,
  HTTPFILETYPE_TEXT,
  HTTPFILETYPE_XUL,
  HTTPFILETYPE_CSS,
  HTTPFILETYPE_JS,
  HTTPFILETYPE_PNG,
  HTTPFILETYPE_JPEG,
  HTTPFILETYPE_GIF,
  HTTPFILETYPE_SWF,
  HTTPFILETYPE_MPA,
  HTTPFILETYPE_MPEG,
  HTTPFILETYPE_AVI,
  HTTPFILETYPE_MP4,
  HTTPFILETYPE_MOV,
  HTTPFILETYPE_264,
  HTTPFILETYPE_FLV,
  HTTPFILETYPE_TS,
  HTTPFILETYPE_3GP,
  HTTPFILETYPE_ASF,
  HTTPFILETYPE_OCTET,
  HTTPFILETYPE_STREAM,
  HTTPFILETYPE_M3U8,
} HttpFileType;

#define MAXPOSTPARAMS 50
#define MAXPOSTREDIRECTFILENAME (200)
#define MAX_CONN_REQUESTS 99

/////////////////////////////////////////////////////////////////////////////
// typedefs
/////////////////////////////////////////////////////////////////////////////

typedef struct _tagPostParam {
  //  char* pchPageName;
  struct {
    char* pchParamName;
    char* pchParamValue;
  } stParams[MAXPOSTPARAMS];
  void *httpParam;
  int iNumParams;
  char *pchPath;
} PostParam;

// multipart file upload post (per socket) structure
typedef struct {
  char pchBoundaryValue[80];
  OCTET oFileuploadStatus;
  size_t writeLocation;
  PostParam pp;
  char *pchFilename;
  void *pxCallBackData;
} HttpMultipart;

typedef struct _tagSubstParam {
  char* pchParamName;
  char* pchParamValue;	// returned
  int iMaxValueBytes;
} SubstParam;

#define FLAG_REQUEST_GET	0x1
#define FLAG_REQUEST_POST	0x2
#define FLAG_HEADER_SENT	0x4
#define FLAG_CONN_CLOSE		0x10
#define FLAG_SUBST			0x20
#define FLAG_AUTHENTICATION	0x40
#define FLAG_MORE_CONTENT	0x80
#define FLAG_TO_FREE		0x100
#define FLAG_CHUNK			0x200

#define FLAG_DATA_FILE		0x10000
#define FLAG_DATA_RAW		0x20000
#define FLAG_DATA_FD		0x40000
#define FLAG_DATA_REDIRECT	0x80000
#define FLAG_DATA_STREAM	0x100000
#define FLAG_DATA_SOCKET	0x200000

#define FLAG_RECEIVING		0x80000000
#define FLAG_SENDING		0x40000000

#define SETFLAG(hs,bit) (hs->flags|=(bit));
#define CLRFLAG(hs,bit) (hs->flags&=~(bit));
#define ISFLAGSET(hs,bit) ((hs->flags&(bit)))

typedef union {
	unsigned long laddr;
	unsigned short saddr[2];
	unsigned char caddr[4];
} IPADDR;

typedef struct {
	int iHttpVer;
	size_t startByte;
	unsigned char *pucPath;
	int ofReferer;
	int ofHost;
	int headerSize;
	unsigned char* pucPayload;
	int payloadSize;
} HttpRequest;

typedef struct {
	int headerBytes;
	int sentBytes;
	int contentLength;
	HttpFileType fileType;
} HttpResponse;

typedef struct {
	char *name;
	char *value;
} HttpVariables;

// Callback function protos
typedef int (*PFNPOSTCALLBACK)(PostParam*);
typedef int (*PFNSUBSTCALLBACK)(SubstParam*);
typedef int (*PFNFILEUPLOADCALLBACK)(HttpMultipart*, OCTET*, size_t);

typedef enum {
	MW_INIT = 0,
	MW_UNINIT,
	MW_PARSE_ARGS,
} MW_EVENT;

typedef int (*MW_EVENT_HANDLER)(MW_EVENT msg, Option arg);

typedef struct {
	time_t startTime;
	int clientCount;
	int clientCountMax;
	int reqCount;
	int reqGetCount;
	int fileSentCount;
	size_t fileSentBytes;
	int varSubstCount;
	int urlProcessCount;
	int timeOutCount;
	int authFailCount;
	int reqPostCount;
	int fileUploadCount;
} HttpStats;

#define HTTP_BUFFER_SIZE (32*1024 /*bytes*/)

// per connection/socket structure
typedef struct _HttpSocket{
	struct _HttpSocket *next;
	SOCKET socket;
	IPADDR ipAddr;

	HttpRequest request;
	HttpResponse response;
	unsigned char *pucData;
	int bufferSize;			// the size of buffer pucData pointing to
	int dataLength;

	int fd;
	unsigned int flags;
	void* handler;				// http handler function address
	void* ptr;					
	time_t tmAcceptTime;
	time_t tmExpirationTime;
	int iRequestCount;
	char* mimeType;
#ifndef _NO_POST
	HttpMultipart* pxMP;
#endif
	unsigned char buffer[HTTP_BUFFER_SIZE];
} HttpSocket;

typedef struct {
	void* hp;
	HttpSocket* hs;
	char *pucRequest;
	HttpVariables* pxVars;
	int iVarCount;
	char *pucHeader;
	char *pucBuffer;
	int dataBytes;
	int contentBytes;
	HttpFileType fileType;
	void *p_sys;
} UrlHandlerParam;

typedef int (*PFNURLCALLBACK)(UrlHandlerParam*);

typedef struct {
	char* pchUrlPrefix;
	PFNURLCALLBACK pfnUrlHandler;
	MW_EVENT_HANDLER pfnEventHandler;
	void *p_sys;
} UrlHandler;

#ifndef DISABLE_BASIC_WWWAUTH
typedef struct {
	char* pchUrlPrefix;
	char pchUsername[MAX_PATH];
	char pchPassword[MAX_PATH];
	char *pchAuthString;
} AuthHandler;
#endif

#define FLAG_DIR_LISTING 1

struct ServerThread {
	HttpSocket *phsSocketHead; /* head of the socket linked list */
	int	bKillWebserver; 
	int	bWebserverRunning; 
	unsigned int flags;
	SOCKET listenSocket;
	int httpPort;
	int maxClients;
	int socketRcvBufSize;	/* socket receive buffer size in KB */
	char *pchWebPath;
	UrlHandler *pxUrlHandler;		/* pointer to URL handler array */
#ifndef DISABLE_BASIC_WWWAUTH
	AuthHandler *pxAuthHandler;     /* pointer to authorization handler array */
#endif
	// substitution callback
	PFNSUBSTCALLBACK pfnSubst;
	// post callbacks
	PFNFILEUPLOADCALLBACK pfnFileUpload;
	PFNPOSTCALLBACK pfnPost;
	DWORD dwAuthenticatedNode;
	time_t tmAuthExpireTime;
	time_t tmSocketExpireTime;
	pthread_t tidHttpThread;
	HttpStats stats;
	u_long hlBindIP;
	void* szctx;
};

typedef struct {
	char* pchRootPath;
	char* pchHttpPath;
	char cFilePath[MAX_PATH];
	char* pchExt;
	int fTailSlash;
} HttpFilePath;

///////////////////////////////////////////////////////////////////////
// Return codes
///////////////////////////////////////////////////////////////////////
// for post callback
#define WEBPOST_OK                (0)
#define WEBPOST_AUTHENTICATED     (1)
#define WEBPOST_NOTAUTHENTICATED  (2)
#define WEBPOST_AUTHENTICATIONON  (3)
#define WEBPOST_AUTHENTICATIONOFF (4)

// for multipart file uploads
#define HTTPUPLOAD_MORECHUNKS     (0)
#define HTTPUPLOAD_FIRSTCHUNK     (1)
#define HTTPUPLOAD_LASTCHUNK      (2)

///////////////////////////////////////////////////////////////////////
// Public functions
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// mwServerStart. Startup the webserver
///////////////////////////////////////////////////////////////////////
int mwServerStart(ServerThread* hp);

///////////////////////////////////////////////////////////////////////
// mwServerShutdown. Shutdown the webserver (closes connections and
// releases resources)
///////////////////////////////////////////////////////////////////////
int mwServerShutdown(ServerThread* hp);

///////////////////////////////////////////////////////////////////////
// mwSetRcvBufSize. Change the TCP windows size of acceped sockets
///////////////////////////////////////////////////////////////////////
int mwSetRcvBufSize(WORD wSize);

///////////////////////////////////////////////////////////////////////
// mwPostRegister. Specify the callback to be called when a POST is
// recevied.
///////////////////////////////////////////////////////////////////////
PFNPOSTCALLBACK mwPostRegister(ServerThread *httpParam, PFNPOSTCALLBACK);

///////////////////////////////////////////////////////////////////////
// mwFileUploadRegister. Specify the callback to be called whenever the 
// server has the next data chunk available from a multipart file upload.
///////////////////////////////////////////////////////////////////////
PFNFILEUPLOADCALLBACK mwFileUploadRegister(ServerThread *httpParam, PFNFILEUPLOADCALLBACK);

///////////////////////////////////////////////////////////////////////
// Default subst, post and file-upload callback processing
///////////////////////////////////////////////////////////////////////
int DefaultWebSubstCallback(SubstParam* sp);
int DefaultWebPostCallback(PostParam* pp);
int DefaultWebFileUploadCallback(HttpMultipart *pxMP, OCTET *poData, size_t dataChunkSize);

int mwGetHttpDateTime(time_t tm, char *buf, int bufsize);
int mwGetLocalFileName(HttpFilePath* hfp);
char* mwGetVarValue(HttpVariables* vars, const char *varname, int index);
int mwGetVarValueInt(HttpVariables* vars, const char *varname, int defval);
int mwParseQueryString(UrlHandlerParam* up);
int mwGetContentType(const char *pchExtname);
void mwDecodeString(char* s);

#ifdef __cplusplus
}
#endif

// HTTP messages/part messages
#ifndef HTTP_SERVER_NAME
#define HTTP_SERVER_NAME "MiniWeb"
#endif
#define HTTP200_HEADER "HTTP/1.1 %s\r\nServer: %s\r\nCache-control: no-cache\r\nPragma: no-cache\r\nAccept-Ranges: bytes\r\nKeep-Alive: timeout=%d, max=%d\r\nConnection: %s\r\n"
#define HTTP200_HDR_EST_SIZE ((sizeof(HTTP200_HEADER)+256)&(-4))
#define HTTP404_HEADER "HTTP/1.1 404 Not Found\r\nServer: %s\r\nConnection: Keep-Alive\r\nContent-length: %d\r\nContent-Type: text/html\r\n\r\n"
#define HTTP404_BODY "<html><head><title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested URL was not found on this server.</p></body></html>"
#define HTTPBODY_REDIRECT "<html><head><meta http-equiv=\"refresh\" content=\"0; URL=%s\"></head><body></body></html>"
#define HTTP301_HEADER "HTTP/1.1 301 Moved Permanently\r\nServer: %s\r\nLocation: %s\r\n\r\n"
#define HTTP_CONTENTLENGTH "Content-Length:"
#define HTTP_MULTIPARTHEADER "multipart/form-data"
#define HTTP_MULTIPARTCONTENT "Content-Disposition: form-data; name="
#define HTTP_MULTIPARTBOUNDARY "boundary="
#define HTTP_FILENAME "filename="
#define HTTP_HEADEREND DEFDWORD('\r','\n','\r','\n')
#define HTTP_HEADEREND_STR "\r\n\r\n"
#define HTTP_SUBST_PATTERN (WORD)(('$' << 8) + '$')

// Define file extensions
#define FILEEXT_HTM DEFDWORD('H','T','M',0)
#define FILEEXT_XML DEFDWORD('X','M','L',0)
#define FILEEXT_XSL DEFDWORD('X','S','L',0)
#define FILEEXT_TEXT DEFDWORD('T','X','T',0)
#define FILEEXT_XUL DEFDWORD('X','U','L',0)
#define FILEEXT_GIF DEFDWORD('G','I','F',0)
#define FILEEXT_JPG DEFDWORD('J','P','G',0)
#define FILEEXT_PNG DEFDWORD('P','N','G',0)
#define FILEEXT_CSS DEFDWORD('C','S','S',0)
#define FILEEXT_JS DEFDWORD('J','S',0,0)
#define FILEEXT_SWF DEFDWORD('S','W','F',0)
#define FILEEXT_HTML DEFDWORD('H','T','M','L')
#define FILEEXT_MPG DEFDWORD('M','P','G',0)
#define FILEEXT_MPEG DEFDWORD('M','P','E','G')
#define FILEEXT_MPA DEFDWORD('M','P','3' - 32,0)
#define FILEEXT_AVI DEFDWORD('A','V','I',0)
#define FILEEXT_MP4 DEFDWORD('M','P','4' - 32,0)
#define FILEEXT_MOV DEFDWORD('M','O','V',0)
#define FILEEXT_FLV DEFDWORD('F','L','V',0)
#define FILEEXT_3GP DEFDWORD('3' - 32, 'G','P',0)
#define FILEEXT_ASF DEFDWORD('A','S','F',0)
#define FILEEXT_264 DEFDWORD('2' - 32, '6' - 32, '4' - 32, 0)
#define FILEEXT_TS DEFDWORD('T', 'S', 0, 0)
#define FILEEXT_M3U8 DEFDWORD('M', '3' - 32, 'U', '8' - 32)

// Settings for http server
#define HTTP_EXPIRATION_TIME (120/*secs*/)
#define HTTP_KEEPALIVE_TIME (15/*secs*/)
#define MAX_RECV_RETRIES (3/*times*/)
#define HTTPAUTHTIMEOUT   (300/*secs*/)
#define HTTPSUBSTEXPANSION (0/*bytes*/)
#define HTTPHEADERSIZE (512/*bytes*/)
#define HTTPSMALLBUFFER (256/*bytes*/)
#define HTTPMAXRECVBUFFER HTTP_BUFFER_SIZE
#define HTTPUPLOAD_CHUNKSIZE (HTTPMAXRECVBUFFER / 2/*bytes*/)
#define MAX_REQUEST_PATH_LEN (512/*bytes*/)
#define MAX_REQUEST_SIZE (2*1024 /*bytes*/)

#if 1
#define SLASH '/'
#else
#define SLASH '\\'
#endif

#ifdef NOCONSOLE
#define SYSLOG
#else
#define SYSLOG fprintf
#endif

/////////////////////////////////////////////////////////////////////////////
// local helper function prototypes
/////////////////////////////////////////////////////////////////////////////
SOCKET _mwAcceptSocket(ServerThread* hp, struct sockaddr_in *sinaddr);
int _mwProcessReadSocket(ServerThread* hp, HttpSocket* phsSocket);
int _mwProcessWriteSocket(ServerThread *hp, HttpSocket* phsSocket);
void _mwCloseSocket(ServerThread* hp, HttpSocket* phsSocket);
int _mwStartSendFile(ServerThread* hp, HttpSocket* phsSocket);
int _mwSendFileChunk(ServerThread *hp, HttpSocket* phsSocket);
void _mwProcessPost(ServerThread* httpParam, HttpSocket* phsSocket);
int _mwProcessMultipartPost(ServerThread *httpParam, HttpSocket* phsSocket, BOOL fNoRecv);
int _mwSubstVariables(ServerThread* hp, char* pchData, int iLength, int* piBytesUsed);
char* _mwStrStrNoCase(char* pchHaystack, char* pchNeedle);
void _mwProcessPostVars(ServerThread *httpParam, HttpSocket* phsSocket,
	int iContentOffset, int contentLength);
void _mwRedirect(HttpSocket* phsSocket, char* pchFilename);
int _mwSendRawDataChunk(ServerThread *hp, HttpSocket* phsSocket);
int _mwStartSendRawData(ServerThread *hp, HttpSocket* phsSocket);
void* _mwHttpThread(ServerThread* hp);
int _mwGetToken(char* pchBuffer,int iTokenNumber,char** ppchToken); 
__inline char _mwDecodeCharacter(char* pchEncodedChar);
int _mwLoadFileChunk(ServerThread *hp, HttpSocket* phsSocket);
OCTET* _mwFindMultipartBoundary(OCTET *poHaystack, int iHaystackSize, 
	OCTET *poNeedle);
void _mwNotifyPostVars(HttpSocket* phsSocket, PostParam *pp);
BOOL _mwCheckAuthentication(HttpSocket* phsSocket);
int _mwStartSendMemoryData(HttpSocket* phsSocket);
int _GetContentType(char *pchFilename);
int _mwCheckAccess(HttpSocket* phsSocket);
int _mwGetContentType(char *pchExtname);
int _mwSendHttpHeader(HttpSocket* phsSocket);
char* _mwStrDword(char* pchHaystack, DWORD dwSub, DWORD dwCharMask);
SOCKET _mwStartListening(ServerThread* hp);
int _mwParseHttpHeader(HttpSocket* phsSocket);
int _mwStrCopy(char *dest, const char *src);
int _mwStrHeadMatch(const char* buf1, const char* buf2);
int _mwRemoveSocket(ServerThread* hp, HttpSocket* hs);

void crc32Init(unsigned long *pCrc32);
void crc32Update(unsigned long *pCrc32, unsigned char *pData, unsigned long uSize);
void crc32Finish(unsigned long *pCrc32);

#define FLAG_REQUEST_ONLY 0x1
#define FLAG_KEEP_ALIVE 0x2
#define FLAG_KEEP_HEADER 0x4
#define FLAG_CHUNKED 0x8

typedef enum {
	HS_IDLE=0,
	HS_REQUESTING,
	HS_RECEIVING,
	HS_STOPPING,
} HTTP_STATES;

typedef enum {
	HM_GET = 0,
	HM_HEAD,
	HM_POST,
	HM_POST_STREAM,
	HM_POST_MULTIPART,
} HTTP_METHOD;

#define postPayload_STRING 0
#define postPayload_BINARY 1
#define postPayload_FD 2
#define postPayload_CALLBACK 3

#define POST_BUFFER_SIZE 1024
typedef int (*PFNpostPayloadCALLBACK)(void* buffer, int bufsize);

typedef struct {
	void* data;
	int type;
	size_t length;
} POST_CHUNK;

typedef struct {
	int sockfd;
	HTTP_METHOD method;
	HTTP_STATES state;
	const char *url;
	const char *proxy;
	unsigned short flags;
	unsigned short port;
	char* referer;
	char* header;
	char* buffer;
	int bufferSize;
	char* postPayload;
	char* hostname;
	int postPayloadBytes;
	int dataSize;
	int bytesStart;
	int bytesEnd;
	int payloadSize;
	//info parsed from response header
	char* contentType;
	char* location;
	int httpVer;	// 0 for 1.0, 1 for 1.1
	int httpCode;
	//Multipart-Post 
	POST_CHUNK* chunk;
	int chunkCount;
	const char* filename;
} HTTP_REQUEST;

#ifdef __cplusplus
extern "C" {
#endif

	void httpInitReq(HTTP_REQUEST* req, char* proxy);
	int httpRequest(HTTP_REQUEST* param, const char* url);
	int httpGetResponse(HTTP_REQUEST* param);
	void httpClean(HTTP_REQUEST* param);
	size_t httpSend(HTTP_REQUEST* param, char* data, size_t length);
	int httpPostFile(HTTP_REQUEST* req, char* url, char* fieldname, const char* filename);
	int PostFileStream(char* url, const char* filename);

#ifdef __cplusplus
}
#endif

#define XN_CDATA 1

#ifdef __cplusplus
extern "C" {
#endif
	typedef struct {
		char* name;
		char* fmt;
		void* value;
		int indent;
		int flags;
	} HTTP_XML_NODE;

	int mwWriteXmlHeader(char** pbuf, int* pbufsize, int ver, char* charset, char* xsl);
	int mwWriteXmlLine(char** pbuf, int* pbufsize, HTTP_XML_NODE *node, char *attr);
	void mwWriteXmlString(char** pbuf, int* pbufsize, int indent, char* str);

#ifdef __cplusplus
}
#endif

typedef struct _PL_ENTRY {
	struct _PL_ENTRY *next;
	void* data;
	int datalen;
} PL_ENTRY;

typedef enum {
	ACT_NOTHING=0,
	ACT_SKIP,
} VOD_CLIENT_ACTIONS;

typedef int (*PL_ENUM_CALLBACK)(PL_ENTRY *entry, void *arg);

PL_ENTRY* plAddEntry(PL_ENTRY **hdr, void* data, int datalen);
PL_ENTRY* plFindEntry(PL_ENTRY *hdr, void* data, int datalen);
void* plGetEntry(PL_ENTRY **hdr);
void* plDelEntry(PL_ENTRY **hdr, void* data);
void* plDelEntryByIndex(PL_ENTRY **hdr, int index);
PL_ENTRY* plPinEntry(PL_ENTRY **hdr, void* data);
PL_ENTRY* plPinEntryByIndex(PL_ENTRY **hdr, int index);

typedef struct _CLIP_INFO {
	char* filename;
	char* title;
	int chars;
	int flags;
	int hash;
	struct _CLIP_INFO* next;
} CLIP_INFO;

struct CATEGORY_INFO2 {
	char* name;
	int count;
	int flags;
	int hash;
	CLIP_INFO* clips;
	CATEGORY_INFO2* next;
};

typedef struct _VOD_CTX {
	PL_ENTRY* playlist;
	DWORD ip;
	int nextaction;
	struct _VOD_CTX* next;
} VOD_CTX;


#define SF_ALLOC 0x1
#define SF_SHOW_WINDOW 0x2
#define SF_READ_STDOUT_ALL 0x4
#define SF_CONVERT_LF 0x8
#define SF_REDIRECT_STDIN 0x1000
#define SF_REDIRECT_STDOUT 0x2000
#define SF_REDIRECT_STDERR 0x4000
#define SF_REDIRECT_OUTPUT (0x8000 | SF_REDIRECT_STDOUT)

typedef struct {
	char *pchCurDir;
	char *pchPath;
#ifdef WIN32
	HANDLE fdRead;
	HANDLE fdWrite;
	int fdStdErr;
	PROCESS_INFORMATION piProcInfo;
	HWND hWnd;
#else
	int fdRead;
	int fdWrite;
	int pid;
#endif
	char *pchEnv;
	char *buffer;
	int iBufferSize;
	int iDelimiter;
	int flags;
}SHELL_PARAM;

#ifdef __cplusplus
extern "C" {
#endif
	int ShellRead(SHELL_PARAM* param, int timeout);
	int ShellWrite(SHELL_PARAM* param, void* data, int bytes);
	void ShellClean(SHELL_PARAM* param);
	int ShellWait(SHELL_PARAM* param, int iTimeout);
	int ShellExec(SHELL_PARAM* param, char* cmdline, int hasGui);
	int ShellTerminate(SHELL_PARAM* param);
#ifdef __cplusplus
}
#endif

#ifdef MEDIA_SERVER
	#include "mediaserver.h"
#endif
#ifdef _7Z
	#include "7zDec/7zInc.h"
#endif

int uhMpd(UrlHandlerParam* param);
int ehMpd(MW_EVENT msg, int argi, void* argp);
int uhStats(UrlHandlerParam* param);
int uhVod(UrlHandlerParam* param);
int uhLib(UrlHandlerParam* param);
int uhVodStream(UrlHandlerParam* param);
int uhStream(UrlHandlerParam* param);
int ehVod(MW_EVENT msg, int argi, void* argp);
int uhTest(UrlHandlerParam* param);
int uh7Zip(UrlHandlerParam* param);
int uhFileStream(UrlHandlerParam* param);

int _mwBuildHttpHeader(HttpSocket *phsSocket, time_t contentDateTime, unsigned char* buffer);
void _mwCloseSocket(ServerThread* hp, HttpSocket* phsSocket);

extern FILE *fpLog;

UrlHandler urlHandlerList[] = {
	{"stats", uhStats, NULL},
	{"getfile", uhFileStream, NULL},
#ifdef MEDIA_SERVER
	{"MediaServer/VideoItems/", uhMediaItemsTranscode, ehMediaItemsEvent},
#endif
#ifdef _7Z
	{"7z", uh7Zip, NULL},
#endif
#ifdef _MPD
	{"mpd", uhMpd, ehMpd},
#endif
#ifdef _VOD
	{"vodstream", uhVodStream,NULL},
	{"vodlib", uhLib,0},
	{"vodplay", uhVod,ehVod},
	{"stream", uhStream,NULL},
#endif
	{NULL},
};

/*******************************************************************
* Process Platform Independent Layer
* Distributed under GPL license
* Copyright (c) 2005-06 Stanley Huang <stanleyhuangyc@yahoo.com.cn>
* All rights reserved.
*******************************************************************/

#define SF_BUFFER_SIZE 256

int ShellRead(SHELL_PARAM* param, int timeout)
{
	int offset=0;
#ifdef WIN32
	DWORD dwRead;
#else
	int ret;
	fd_set fds;
	struct timeval tv;
#endif

	if (!param->buffer || param->iBufferSize <= 0) {
		if (param->flags & SF_ALLOC) {
			param->buffer = (char*)malloc(SF_BUFFER_SIZE);
			param->iBufferSize=SF_BUFFER_SIZE;
		} else {
			return -1;
		}
	}

#ifndef WIN32
	FD_ZERO(&fds);
	FD_SET(param->fdRead,&fds);
	tv.tv_sec = timeout / 1000;
	tv.tv_usec = (timeout - tv.tv_sec * 1000) * 1000;
#endif

	if (param->iDelimiter) {
		for(;;) {
#ifdef WIN32
			if (!PeekNamedPipe(param->fdRead,0,0,0,0,0) ||
				!ReadFile(param->fdRead, param->buffer+offset, 1, &dwRead, NULL))
				return -1;
#else
			ret=select(param->fdRead+1,&fds,NULL,NULL,&tv);
			if (ret<1) break;
			ret=read(param->fdRead,param->buffer+offset,1);
			if (ret<1) break;;
#endif
			if ((int)param->buffer[offset++]==param->iDelimiter) break;
			if (offset==param->iBufferSize-1) {
				if  (!(param->flags & SF_ALLOC)) break;
				param->iBufferSize+=SF_BUFFER_SIZE;
				param->buffer=(char*)realloc(param->buffer,param->iBufferSize);
			}
		}
		param->buffer[offset]=0;
#ifdef WIN32
		return 0;
#else
		return (ret>0)?offset:-1;
#endif
	} else if (!(param->flags & SF_READ_STDOUT_ALL)) {
#ifdef WIN32
		if (!PeekNamedPipe(param->fdRead,0,0,0,0,0) ||
			!ReadFile(param->fdRead, param->buffer, param->iBufferSize - 1, &dwRead, NULL))
			return -1;
		param->buffer[dwRead]=0;
		return dwRead;
#else
		if (select(param->fdRead+1,&fds,NULL,NULL,&tv) < 1) return -1;
		ret=read(param->fdRead,param->buffer, param->iBufferSize - 1);
		if (ret<1) return -1;
		param->buffer[ret] = 0;
		return ret;
#endif
	} else {
#ifdef WIN32
		int fSuccess;
		for(;;) {
			int i;
			if (offset >= param->iBufferSize - 1) {
				if (param->flags & SF_ALLOC) {
					param->iBufferSize <<= 1;
					param->buffer = (char*)realloc(param->buffer, param->iBufferSize);
				} else {
					offset = 0;
				}
			}
			fSuccess = (PeekNamedPipe(param->fdRead,0,0,0,0,0) &&
				ReadFile(param->fdRead, param->buffer + offset, param->iBufferSize - 1 - offset, &dwRead, NULL));
			if (!fSuccess) break;
			// convert line-feed
			if (param->flags & SF_CONVERT_LF) {
				for (i = 0; i < (int)dwRead; i++) {
					if (*(param->buffer + offset + i) == '\n' && *(param->buffer + offset + i - 1) != '\r') {
						if (offset + (int)dwRead >= param->iBufferSize - 2) {
							if (param->flags & SF_ALLOC) {
								param->iBufferSize <<= 1;
								param->buffer = (char*)realloc(param->buffer, param->iBufferSize);
							} else {
								break;
							}
						}
						memmove(param->buffer + offset + i + 1, param->buffer + offset + i, dwRead - i);
						param->buffer[offset + i] = '\r';
						i++;
						dwRead++;
					}
				}
			}
			offset += dwRead;
		}
		param->buffer[offset]=0;
		return offset;
#endif
	}
}

int ShellWrite(SHELL_PARAM* param, void* data, int bytes)
{
#ifdef WIN32
	DWORD written;
	return WriteFile(param->fdWrite, data, bytes, &written, 0) ? written : -1;
#else
	return write(param->fdWrite, data, bytes);
#endif
}

int ShellTerminate(SHELL_PARAM* param)
{
	int ret;
#ifdef WIN32
	if (!param->piProcInfo.hProcess) return -1;
	ret=TerminateProcess(param->piProcInfo.hProcess,0)?0:-1;
#else
	if (!param->pid) return 0;
	ret=kill(param->pid,SIGSTOP);
	param->pid=0;
#endif
	return ret;
}

void ShellClean(SHELL_PARAM* param)
{
#ifdef WIN32
	if (param->fdRead) CloseHandle((HANDLE)param->fdRead);
	if (param->fdWrite) CloseHandle((HANDLE)param->fdWrite);
	CloseHandle(param->piProcInfo.hProcess);
	CloseHandle(param->piProcInfo.hThread);
	memset(&param->piProcInfo, 0, sizeof(PROCESS_INFORMATION));
#else
	if (param->fdRead) close(param->fdRead);
	if (param->fdWrite) close(param->fdWrite);
	param->pid=0;
#endif
	param->fdRead=0;
	param->fdWrite=0;
	if (param->flags & SF_ALLOC) {
		free(param->buffer);
		param->buffer=NULL;
		param->iBufferSize=0;
	}
}

int ShellWait(SHELL_PARAM* param, int iTimeout)
{
#ifdef WIN32
	switch (WaitForSingleObject(param->piProcInfo.hProcess,(iTimeout==-1)?INFINITE:iTimeout)) {
	case WAIT_OBJECT_0:
		return 1;
	case WAIT_TIMEOUT:
		return 0;
	default:
		return -1;
	}
#else
	int ret=-1;
	return waitpid(param->pid,&ret,iTimeout==0?WNOHANG:0);
#endif
}

#ifdef WIN32
static char* GetAppName(char* commandLine)
{
	char* appname;
	char* p;

	p = strrchr(commandLine, '.');
	if (p && !_stricmp(p + 1, "bat")) {
		return _strdup("cmd.exe");
	} else if (*commandLine == '\"') {
		appname = _strdup(commandLine + 1);
		p = strchr(appname, '\"');
		*p = 0;
	} else {
		appname = _strdup(commandLine);
		p = strchr(appname, ' ');
		if (p) {
			*p = 0;
		}
	}
	return appname;	
}

BOOL CALLBACK EnumWindowCallBack(HWND hwnd, LPARAM lParam) 
{ 
	SHELL_PARAM *param = (SHELL_PARAM * )lParam; 
	DWORD ProcessId;
	char title[128];

	GetWindowThreadProcessId ( hwnd, &ProcessId ); 

	// note: In order to make sure we have the MainFrame, verify that the title 
	// has Zero-Length. Under Windows 98, sometimes the Client Window ( which doesn't 
	// have a title ) is enumerated before the MainFrame 

	if ( ProcessId  == param->piProcInfo.dwProcessId && GetWindowText(hwnd, title, sizeof(title)) > 0) 
	{ 
		param->hWnd = hwnd; 
		return FALSE; 
	} 
	else 
	{ 
		// Keep enumerating 
		return TRUE; 
	} 
}

static HWND GetWindowHandle(SHELL_PARAM* param)
{
	param->hWnd = 0;
	EnumWindows( EnumWindowCallBack, (LPARAM)param ) ;
	return 0;
}
#endif

int ShellExec(SHELL_PARAM* param, char* cmdline, int hasGui)
{
#ifdef WIN32
	SECURITY_ATTRIBUTES saAttr;
	STARTUPINFO siStartInfo;
	BOOL fSuccess;
	char newPath[256],prevPath[256];
	HANDLE hChildStdinRd, hChildStdinWr, hChildStdoutRd, hChildStdoutWr;
#else
	int fdin[2], fdout[2],pid,i;
	int fdStdinChild;
	int fdStdoutChild;
	char **args=NULL,*argString=NULL, *p;
	char *filePath;
	char newPath[256],*prevPath;
	char *env[2];
#endif

#ifdef WIN32
	if (param->piProcInfo.hProcess || !cmdline) return -1;

	_setmode( _fileno( stdin ), _O_BINARY );
	_setmode( _fileno( stdout ), _O_BINARY );

	// modify path variable
	if (param->pchPath) {
		GetEnvironmentVariable("PATH",prevPath,sizeof(prevPath));
		snprintf(newPath, sizeof(newPath), "%s;s", param->pchPath, prevPath);
		SetEnvironmentVariable("PATH",newPath);
	}

	memset( &param->piProcInfo, 0, sizeof(PROCESS_INFORMATION) );
	memset( &siStartInfo, 0, sizeof(STARTUPINFO) );

	///////////////////////////////////////////////////////////////////////
	//	create redirecting pipes
	///////////////////////////////////////////////////////////////////////

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	siStartInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	siStartInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	if (param->flags & (SF_REDIRECT_STDOUT | SF_REDIRECT_STDERR)) {
		// Create a pipe for the child process's STDOUT.
		if (! CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0)) {
			return -1;
		}
		// Create noninheritable read handle and close the inheritable read
		// handle.
		fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdoutRd,
			GetCurrentProcess(), (LPHANDLE)&param->fdRead, 0,
			FALSE,
			DUPLICATE_SAME_ACCESS);
		if( !fSuccess ) return 0;
		CloseHandle(hChildStdoutRd);
		if (param->flags & SF_REDIRECT_STDOUT) {
			siStartInfo.hStdOutput = hChildStdoutWr;
		}
		if (param->flags & SF_REDIRECT_STDERR) {
			siStartInfo.hStdError = hChildStdoutWr;
		}
	}
	siStartInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	if (param->flags & SF_REDIRECT_STDIN) {
		// Create a pipe for the child process's STDIN.
		if (! CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0)) return 0;

		// Duplicate the write handle to the pipe so it is not inherited.
		fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdinWr,
			GetCurrentProcess(), (LPHANDLE)&param->fdWrite, 0,
			FALSE,                  // not inherited
			DUPLICATE_SAME_ACCESS);
		if (! fSuccess) return -1;
		CloseHandle(hChildStdinWr);
		siStartInfo.hStdInput = hChildStdinRd;
	}

	siStartInfo.dwFlags |= STARTF_USESHOWWINDOW;
	if (hasGui)  {
		siStartInfo.wShowWindow = SW_SHOW;
	} else {
		siStartInfo.wShowWindow = SW_HIDE;
		if (param->flags & (SF_REDIRECT_STDIN | SF_REDIRECT_STDOUT | SF_REDIRECT_STDERR)) siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
	}

	///////////////////////////////////////////////////////////////////////
	//	create child process
	///////////////////////////////////////////////////////////////////////
	// Create the child process.

	{
		char* appname = GetAppName(cmdline);
		fSuccess = CreateProcess(appname,
			cmdline,	// command line
			NULL,					// process security attributes
			NULL,					// primary thread security attributes
			TRUE,					// handles are inherited
			0,						// creation flags
			NULL,					// process' environment
			param->pchCurDir,		// current directory
			&siStartInfo,			// STARTUPINFO pointer
			&param->piProcInfo);	// receives PROCESS_INFORMATION

		free(appname);
	}

	if (hasGui) {
		WaitForInputIdle(param->piProcInfo.hProcess, 3000);
		GetWindowHandle(param);
	}

	if (param->pchPath) SetEnvironmentVariable("PATH",prevPath);
	if (!fSuccess) return -1;
	WaitForInputIdle(param->piProcInfo.hProcess,INFINITE);

	if (param->flags & SF_REDIRECT_STDIN)
		CloseHandle(hChildStdinRd);
	if (param->flags & SF_REDIRECT_STDOUT)
		CloseHandle(hChildStdoutWr);
#else

	if (param->pid) return -1;

	if (param->flags & SF_REDIRECT_STDIN) {
		pipe(fdin);
		param->fdWrite = fdin[1];
		fdStdinChild=fdin[0];
	}
	if (param->flags & SF_REDIRECT_STDOUT) {
		pipe(fdout);
		param->fdRead = fdout[0];
		fdStdoutChild=fdout[1];
	}

	pid = fork();
	if (pid == -1) return -1;
	if (pid == 0) { /* chid process */
		//generate argument list
		for (p=cmdline,i=2;*p;p++) {
			if (*p==' ') i++;
		}
		args=malloc(i*sizeof(char*));
		argString=strdup(cmdline);
		i=0;
		if (argString) {
			p=strtok(argString," ");
			while (p) {
				args[i++]=p;
				p=strtok(NULL," ");
			}
		}
		args[i]=NULL;
		p=strrchr(args[0],'/');
		if (p) {
			filePath=args[0];
			args[0]=p+1;
		} else {
			filePath=args[0];
		}

		//set PATH
		env[0]=NULL;
		if (param->pchPath) {
			prevPath=getenv("PATH");
			env[0]=malloc(strlen(prevPath)+strlen(param->pchPath)+2+5);
			sprintf(env[0],"PATH=%s:%s",prevPath,param->pchPath);
			env[1]=NULL;
		}
		if (param->flags & SF_REDIRECT_STDIN) {
			close(fdin[1]);
			dup2(fdStdinChild, 0);
		}
		if (param->flags & SF_REDIRECT_STDOUT) {
			close(fdout[0]);
			dup2(fdStdoutChild, 1);
		}
		if (execve(filePath, args, env)<0) {
			printf("Error starting specified program\n");
		}
		return 0;
	}
	if (param->flags & SF_REDIRECT_STDIN) {
		close(fdin[0]);
	}
	if (param->flags & SF_REDIRECT_STDOUT) {
		close(fdout[1]);
	}
	param->pid=pid;
#endif
	return 0;
}

#ifdef _MPD

typedef enum {
	MP_IDLE = 0,
	MP_LOADING,
	MP_PLAYING,
	MP_PAUSED,
} MP_STATES;

static char* states[] = {"idle", "loading", "playing", "paused"};

MP_STATES mpState = MP_IDLE;
int mpPos = 0;
static SHELL_PARAM mpx;
pthread_t mpThreadHandle = 0;
pthread_mutex_t mpConsoleMutex;
static char* mpbin = "mplayer";
static char* loopclip = 0;
static PL_ENTRY* playlist = 0;

int mpRead(char* buf, int bufsize)
{
	int n;
	if (mpState == MP_IDLE || !mpx.fdStdoutRead) return 0;
	mpx.buffer = buf;
	mpx.iBufferSize = bufsize;
	n = ShellRead(&mpx);
	if (n < 0) return -1;
	buf[n] = 0;
	return n;
}

int mpCommand(char* cmd)
{
	int ret=write(mpx.fdStdinWrite,cmd,strlen(cmd));
	write(mpx.fdStdinWrite,"\n",1);
	return ret;
}

void mpClose()
{
	mpPos = -1;
	if (mpState != MP_IDLE) {
		if (mpCommand("quit") > 0) ShellWait(&mpx,1000);
		ShellTerminate(&mpx);
		ShellClean(&mpx);
	}
}

int mpOpen(char* pchFilename, char* opts)
{
	char buf[512];
	mpClose();
	snprintf(buf, sizeof(buf), "%s %s -slave -quiet %s", mpbin, pchFilename, opts ? opts : "");
	printf("MPlayer command line:\n%s\n", buf);
	mpx.flags = SF_REDIRECT_STDIN | SF_REDIRECT_STDOUT;
	mpState = MP_LOADING;
	if (ShellExec(&mpx, buf)) return -1;
	return 0;
}

void* mpThread(void* _args)
{
	char *p = NULL;
	char buf[1024];
	char* args = _args ? strdup(_args) : 0;
	int n;
	int offset;
	void* data;
	for (;;) {
		if (data = plGetEntry(&playlist)) {
			n = mpOpen(data, args);
		} else {
			if (!loopclip) break;
			n = mpOpen(loopclip, 0);
		}
		free(data);
		if (n) break;

		mpState = MP_PLAYING;
		MutexLock(&mpConsoleMutex);
		while (mpCommand("get_time_pos") <= 0) msleep(500);
		do {
			offset = 0;
			while (offset < sizeof(buf) - 1) {
				n = mpRead(buf + offset, sizeof(buf) - 1 - offset);
				if (n <= 0) break;
				offset += n;
				buf[offset] = 0;
				if (p = strstr(buf, "ANS_TIME_POSITION=")) {
					mpPos = atoi(p + 18);
					break;
				}
			}
			// stop here when paused
			MutexUnlock(&mpConsoleMutex);
			do {
				msleep(500);
			} while (mpState == MP_PAUSED);
			MutexLock(&mpConsoleMutex);
		} while (mpCommand("get_time_pos") > 0);
		MutexUnlock(&mpConsoleMutex);
		ShellTerminate(&mpx);
		ShellClean(&mpx);
	}
	free(args);
	mpState = MP_IDLE;
	mpThreadHandle = 0;
	return 0;
}

int ehMpd(MW_EVENT event, int argi, void* argp)
{
	switch (event) {
	case MW_INIT:
		if (mpConsoleMutex) return 0;	// already inited
		memset(&mpx,0,sizeof(mpx));
		MutexCreate(&mpConsoleMutex);
		if (loopclip) ThreadCreate(&mpThreadHandle, mpThread, 0);
		break;
	case MW_UNINIT:
		MutexDestroy(&mpConsoleMutex);
		mpClose();
		break;
	case MW_PARSE_ARGS: {
		int i = 0;
		char** argv = (char**)argp;
		for (i = 0; i < argi; i++) {
			if (!strcmp(argv[i], "--mploop")) {
				loopclip = argv[++i];
				break;
			} else if (!strcmp(argv[i], "--mpbin")) {
				mpbin = argv[++i];
			}
		}
						} break;
	}
	return 0;
}

int uhMpd(UrlHandlerParam* param)
{
	char *action;
	char *pbuf = param->pucBuffer;
	int bufsize = param->iDataBytes;
	HTTP_XML_NODE node;

	mwParseQueryString(param);
	action = mwGetVarValue(param->pxVars, "action", 0);

	mwWriteXmlHeader(&pbuf, &bufsize, 10, "gb2312", mwGetVarValue(param->pxVars, "xsl", 0));
	mwWriteXmlString(&pbuf, &bufsize, 0, "<response>");

	node.indent = 1;
	node.name = "state";
	node.fmt = "%s";
	node.flags = 0;

	if (!strcmp(param->pucRequest + 1, "playlist")) {
		PL_ENTRY *ptr;
		int i;
		if (!action) {
		} else if (!strcmp(action, "add")) {
			char *filename = mwGetVarValue(param->pxVars, "stream", 0);
			char *title = mwGetVarValue(param->pxVars, "title", 0);
			if (!title) title = "";
			if (!filename) {
				node.value = "error";
			} else {
				int fnlen;
				int titlelen;
				char *entrydata;
				mwDecodeString(filename);
				fnlen = strlen(filename);
				titlelen = strlen(title);
				entrydata = (char*)malloc(fnlen + titlelen + 3);
				strcpy(entrydata, filename);
				strcpy(entrydata + fnlen + 1, title);
				node.value = plAddEntry(&playlist, entrydata, fnlen + titlelen + 2) ? "OK" : "error";
			}
			mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
		} else if (!strcmp(action, "pin")) {
			int index = mwGetVarValueInt(param->pxVars, "arg", 0);
			node.value = plPinEntryByIndex(&playlist, index) ? "OK" : "error";
			mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
		} else if (!strcmp(action, "del")) {
			int index = mwGetVarValueInt(param->pxVars, "arg", 0);
			node.value = plDelEntryByIndex(&playlist, index) ? "OK" : "error";
			mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
		}
		ptr = playlist;
		mwWriteXmlString(&pbuf, &bufsize, 1, "<playlist>");
		for (i=0; ptr; ptr = ptr->next, i++) {
			char buf[32];
			snprintf(buf, sizeof(buf), "<item index=\"%03d\">", i);
			mwWriteXmlString(&pbuf, &bufsize, 2, buf);

			node.indent = 3;
			node.name = "stream";
			node.fmt = "%s";
			node.value = ptr->data;
			mwWriteXmlLine(&pbuf, &bufsize, &node, 0);

			node.flags = XN_CDATA;
			node.name = "title";
			node.value = (char*)ptr->data + strlen(ptr->data) + 1;
			mwWriteXmlLine(&pbuf, &bufsize, &node, 0);

			mwWriteXmlString(&pbuf, &bufsize, 2, "</item>");
		}
		mwWriteXmlString(&pbuf, &bufsize, 1, "</playlist>");
	} else if (!action) {

	} else if (!strcmp(action, "play")) {
		char *filename = mwGetVarValue(param->pxVars, "stream", 0);
		char *args = mwGetVarValue(param->pxVars, "arg", 0);
		if (filename && *filename) {
			mwDecodeString(filename);
			filename = strdup(filename);
		}
		if ((!filename || !*filename) || plAddEntry(&playlist, filename, strlen(filename) + 1)) {
			if (!mpThreadHandle) {
				if (args) mwDecodeString(args);
				ThreadCreate(&mpThreadHandle, mpThread, args);
			} else {
				mpClose();
			}
			node.value = "OK";
		} else {
			node.value = "error";
		}
		mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
	} else if (!strcmp(action, "query")) {
		int i;
		char* info;
		for (i = 0; info = mwGetVarValue(param->pxVars, "info", i); i++) {
			if (!strcmp(info, "pos")) {
				node.name = "pos";
				node.fmt = "%d";
				node.value = (void*)mpPos;
				mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
			} else if (!strcmp(info, "state")) {
				node.name = "state";
				node.fmt = "%s";
				node.value = states[mpState];
				mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
			}
		}
	} else if (!strcmp(action, "pause")) {
		mpState = (mpState == MP_PLAYING ? MP_PAUSED : MP_PLAYING);
		mpCommand("pause");
	} else if (!strcmp(action, "seek")) {
		char buf[32];
		char *args = mwGetVarValue(param->pxVars, "arg", 0);
		if (args) {
			snprintf(buf, sizeof(buf), "seek %s", args);
			mpCommand(buf);
		}
	} else if (!strcmp(action, "command")) {
		char *cmd = mwGetVarValue(param->pxVars, "arg", 0);
		char *hasResult = mwGetVarValue(param->pxVars, "result", 0);
		if (cmd) {
			if (mpCommand(cmd) > 0) {
				node.value = "OK";
				if (hasResult) {
					int bytes;
					MutexLock(&mpConsoleMutex);

					bytes = snprintf(pbuf, bufsize,  "  <result><![CDATA[");
					pbuf += bytes;
					bufsize -= bytes;

					bytes = mpRead(pbuf, bufsize);
					MutexUnlock(&mpConsoleMutex);
					if (bytes > 0) {
						pbuf += bytes;
						bufsize -= bytes;
					}

					bytes = snprintf(pbuf, bufsize,  "]]></result>");
					pbuf += bytes;
					bufsize -= bytes;
				}
			} else {
				node.value = "error";
			}
			mwWriteXmlLine(&pbuf, &bufsize, &node, 0);

		}
	} else {
		return 0;
	}

	mwWriteXmlString(&pbuf, &bufsize, 0, "</response>");

	param->iDataBytes=(int)(pbuf-param->pucBuffer);
	param->fileType=HTTPFILETYPE_XML;
	return FLAG_DATA_RAW;
}
#endif

int mwWriteXmlHeader(char** pbuf, int* pbufsize, int ver, char* charset, char* xsl)
{
	int len2 = 0;
	int len1 = snprintf(*pbuf, *pbufsize, "<?xml version=\"%d.%d\" encoding=\"%s\"?>\n",
		ver / 10, ver % 10, charset ? charset : "utf-8");
	*pbuf += len1;
	*pbufsize -= len1;
	if (xsl){
		len2 = snprintf(*pbuf, *pbufsize, "<?xml-stylesheet type=\"text/xsl\" href=\"%s\"?>\n", xsl);
		*pbuf += len2;
		*pbufsize -= len2;
	}
	return len1 + len2;
}

void mwWriteXmlString(char** pbuf, int* pbufsize, int indent, char* str)
{
	int i;
	int len;
	int bufsize = *pbufsize;
	if (bufsize < indent * 2) return;
	for (i = 0; i < indent; i++) {
		*((*pbuf) ++) = ' ';
		*((*pbuf) ++) = ' ';
	}
	bufsize -= indent * 2;
	len = snprintf(*pbuf, bufsize, "%s\n", str);
	*pbuf += len;
	bufsize -= len;
	*pbufsize = bufsize;
}

int mwWriteXmlLine(char** pbuf, int* pbufsize, HTTP_XML_NODE *node, char *attr)
{
	int bufsize = *pbufsize;
	int len;
	int i;

	if (bufsize < node->indent * 2) return -1;
	for (i = 0; i < node->indent; i++) {
		*((*pbuf)++) = ' ';
		*((*pbuf)++) = ' ';
	}
	bufsize -= node->indent * 2;
	len = snprintf(*pbuf, bufsize, "<%s", node->name);
	*pbuf += len;
	bufsize -= len;
	if (attr) {
		len = snprintf(*pbuf, bufsize, " %s", attr);
		*pbuf += len;
		bufsize -= len;
	}
	*((*pbuf)++) = '>';
	bufsize--;

	if (node->flags & XN_CDATA) {
		len = snprintf(*pbuf, bufsize, "%s", "<![CDATA[");
		*pbuf += len;
		bufsize -= len;
	}

	len = snprintf(*pbuf, bufsize, node->fmt, node->value);
	*pbuf += len;
	bufsize -= len;

	if (node->flags & XN_CDATA) {
		len = snprintf(*pbuf, bufsize, "%s", "]]>");
		*pbuf += len;
		bufsize -= len;
	}

	len = snprintf(*pbuf, bufsize, "</%s>\n", node->name);
	*pbuf += len;
	bufsize -= len;

	*pbufsize = bufsize;
	return 0;
}

#define PRE_ALLOC_UNIT 16

static VOD_CTX vodctx;
static int listcount=0;
static int playcount=0;
static char *vodloop = 0;
static char *vodhost = 0;
static char vodbuf[256];
static unsigned long* hashmap = 0;

#define MAX_CHARS 30
static int charsinfo[MAX_CHARS + 1];
static char *vodroot=NULL;
static char** filelist = NULL;
static int filecount = 0;
static CATEGORY_INFO2 cats;
static int prefixlen = 0;

static int nextarg = 0;

int EnumDir(char* pchDir)
{
	int i;
	char buf[256];
	char* path;
	int dirlen = strlen(pchDir) + 1;
	int pathlen = 0;
	char** dirlist = NULL;
	int dircount = 0;

	for (i = ReadDir(pchDir, buf); !i; i = ReadDir(NULL, buf)) {
		int len;
		if (buf[0] == '.' && (buf[1] == 0 || (buf[1] == '.' && buf[2] == 0))) continue;
		len = dirlen + strlen(buf) + 1;
		if (len > pathlen) {
			if (pathlen) free(path);
			path = (char*)malloc(len);
			pathlen = len;
		}
		sprintf(path, "%s/%s", pchDir, buf);
		if (IsDir(path)) {
			if (!(dircount % PRE_ALLOC_UNIT)) {
				dirlist = (char**)realloc(dirlist, (dircount + PRE_ALLOC_UNIT) * sizeof(char*));
			}
			dirlist[dircount++] = strdup(buf);
		} else {
			if (!(filecount % PRE_ALLOC_UNIT)) {
				filelist = (char**)realloc(filelist, (filecount + PRE_ALLOC_UNIT) * sizeof(char*));
			}
			filelist[filecount++] = strdup(path + prefixlen);
			//printf("%s\n", path);
		}
	}
	for (i = 0; i < dircount; i++) {
		int len = dirlen + strlen(dirlist[i]) + 1;
		if (len > pathlen) {
			if (pathlen) free(path);
			path = (char*)malloc(len);
			pathlen = len;
		}
		sprintf(path, "%s/%s", pchDir, dirlist[i]);
		free(dirlist[i]);
		EnumDir(path);
	}
	free(dirlist);
	if (pathlen) free(path);
	return 0;
}

int GetCategoryHash(CATEGORY_INFO2* ci)
{
	char *p = ci->name;
	int i;
	unsigned sum = 0;
	for (i = 0; p[i]; i++) sum += p[i];
	return sum % 100;
}

int GetTitleHash(CLIP_INFO* ci)
{
	unsigned long val;
	crc32Init(&val);
	crc32Update(&val, (uchar*)ci->title, strlen(ci->title));
	crc32Finish(&val);
	return val % 10000;
}

char* FilterDup(char* str)
{
	char *newstr = (char*)malloc(strlen(str) + 1);
	char *p, *q;
	for (p = str, q = newstr; *p; p++, q++) {
		switch (*p) {
		case '_':
			*q = ' ';
			break;
		case '&':
			*q = ',';
			break;
		default:
			*q = *p;
		}
	}
	*q = 0;
	return newstr;
}

CATEGORY_INFO2* FindCategory(char* name)
{
	int pos = 0;
	CATEGORY_INFO2* ptr;
	CATEGORY_INFO2* prev = 0;
	if (!name || !*name) {
		cats.count++;
		return &cats;
	}
	{
		char *p = strchr(name, ',');
		if (p) *p = 0;
	}
	ptr = &cats;
	for (;;) {
		int n = strcmp(ptr->name, name);
		if (n == 0) {
			ptr->count++;
			return ptr;
		} else if (n > 0 && prev) {
			prev->next = (CATEGORY_INFO2*)calloc(1, sizeof(CATEGORY_INFO2));
			prev->next->next = ptr;
			ptr = prev->next;
			break;
		} else if (!ptr->next) {
			ptr->next = (CATEGORY_INFO2*)calloc(1, sizeof(CATEGORY_INFO2));
			ptr = ptr->next;
			break;
		}
		prev = ptr;
		ptr = ptr->next;		
	}

	ptr->name = FilterDup(name);
	ptr->count = 1;
	ptr->hash = GetCategoryHash(ptr);
	return ptr;
}

int IsHashed(int hash, int autoset)
{
	int dwoff = hash >> 5;
	int bitmask = 1 << (hash & 0x1f);
	if (hashmap[dwoff] & bitmask) return 1;
	if (autoset) hashmap[dwoff] |= bitmask;
	return 0;
}

CLIP_INFO* GetClipByHash(int hash, CATEGORY_INFO2** pcat)
{
	int cathash = hash / 10000;
	CATEGORY_INFO2* cat;
	for (cat = &cats; cat; cat = cat->next) {
		if (cathash == cat->hash) {
			CLIP_INFO* ptr;
			for (ptr = cat->clips; ptr; ptr = ptr->next) {
				if (ptr->hash == hash) {
					if (pcat) *pcat = cat;
					return ptr;
				}
			}
		}
	}
	return 0;
}

CLIP_INFO* GetClipByName(char* catname, char* title, CATEGORY_INFO2** pcat)
{
	CATEGORY_INFO2* cat;
	for (cat = &cats; cat; cat = cat->next) {
		if (!catname || !strcmp(catname, cat->name)) {
			CLIP_INFO* ptr;
			for (ptr = cat->clips; ptr; ptr = ptr->next) {
				if (!title || !strcmp(title, ptr->title)) {
					if (pcat) *pcat = cat;
					return ptr;
				}
			}
		}
	}
	return 0;
}

CLIP_INFO* GetClipByFile(char* filename, CATEGORY_INFO2** pcat)
{
	CATEGORY_INFO2* cat;
	for (cat = &cats; cat; cat = cat->next) {
		CLIP_INFO* ptr;
		for (ptr = cat->clips; ptr; ptr = ptr->next) {
			if (!strcmp(filename, ptr->filename)) {
				if (pcat) *pcat = cat;
				return ptr;
			}
		}
	}
	return 0;
}

FILE* fpdup;

int AddClip(char* filename)
{
	CLIP_INFO* pinfo;
	CATEGORY_INFO2* cat;
	char buf[256];
	char *p;
	char *s;
	int i;
	s = strrchr(filename, '/');
	if (!(s++)) s = filename;
	if (s[5] == ' ' && atoi(s) > 0) s += 5;
	while (*s == ' ') s++;
	if (*s == '.') return -1;
	pinfo = (CLIP_INFO*)malloc(sizeof(CLIP_INFO));
	pinfo->next = 0;
	pinfo->filename = filename;
	p = strrchr(s, '[');
	if (p) {
		// get category
		strcpy(buf, p + 1);
		p = strchr(buf, ']');
		if (p) *p = 0;
		cat = FindCategory(buf);

		// get title
		strcpy(buf, s);
		p = strchr(buf, '[');
		while (--p >= buf && *p == ' ');
		*(p + 1) = 0;
		pinfo->title = FilterDup(buf);
	} else {
		strcpy(buf, s);
		s = buf;
		if (p = strstr(s, " - ")) {
			*p = 0;
			cat = FindCategory(s);
			s = p + 3;
			p = strrchr(s, '.');
			if (p) *p = 0;
			pinfo->title = FilterDup(s);
		} else if ((p = strchr(s, '-')) || (p = strchr(s, ' '))) {
			*p = 0;
			cat = FindCategory(s);
			for (s = p + 1; *s == '-' || *s == ' '; s++);
			p = strrchr(s, '.');
			if (p) *p = 0;
			pinfo->title = FilterDup(s);
		} else {
			cat = FindCategory(0);
			p = strrchr(s, '.');
			if (p) *p = 0;
			pinfo->title = FilterDup(s);
		}
	}
	pinfo->hash = cat->hash * 10000 + GetTitleHash(pinfo);
	for (i = 0, p = pinfo->title; *p && *p != '(' && *p != '['; p++) {
		if (*p < 0) {
			p++;
			i++;
		} else if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <='Z') || (*p >= '0' && *p <='9')) {
			i++;
		}
	}
	pinfo->chars = i;
	charsinfo[(i > MAX_CHARS || i < 1) ? 0 : i]++;

	while (IsHashed(pinfo->hash, 1)) {
		CATEGORY_INFO2* excat;
		CLIP_INFO* exclip = GetClipByHash(pinfo->hash, &excat);
		if (!exclip) break;
		if (excat == cat && !strcmp(exclip->title, pinfo->title)) {	
			fprintf(fpdup, "<li>%06d %s [ <a href='/vodstream?file=%s'>%s</a> ] [ <a href='/vodstream?file=%s'>%s</a> ]</li>\n",
				pinfo->hash, cat->name, exclip->filename, exclip->title, pinfo->filename, pinfo->title);	
		} else {
			//printf("HASH FAULT @ %06d Exist: %s - %s New: %s - %s\n", pinfo->hash, cat->name, exclip->title, cat->name, pinfo->title);	
		}
		pinfo->hash++;
	}

	// add clip to category
	if (!cat->clips) {
		cat->clips = pinfo;
	} else {
		CLIP_INFO *ptr = cat->clips;
		CLIP_INFO *prev = 0;
		for (;;) {
			int n = strcmp(ptr->title, pinfo->title);
			if (n > 0) {
				if (!prev) {
					pinfo->next = cat->clips;
					cat->clips = pinfo;
				} else {
					pinfo->next = prev->next;
					prev->next = pinfo;
				}
				break;
			}
			if (!ptr->next) {
				ptr->next = pinfo;
				break;
			}
			prev = ptr;
			ptr = ptr->next;
		}
	}
	return 0;
}

void vodInit()
{
	int i;
	int code = 0;
	int count = 0;
	if (!vodroot)
		return;
	memset(&vodctx, 0, sizeof(vodctx));
	memset(&charsinfo, 0, sizeof(charsinfo));
	hashmap = (ulong*)calloc(1000000 / 32, sizeof(long));
	//sprintf(vodroot,"%s",hp->pchWebPath);
	cats.name = "";
	cats.hash = GetCategoryHash(&cats);
	cats.next = 0;
	prefixlen = strlen(vodroot) + 1;
	EnumDir(vodroot);

	fpdup = fopen("webroot/dup.htm", "w");
	fprintf(fpdup, "<html><body>");
	for (i = 0; i < filecount; i++) {
		if (!AddClip(filelist[i])) count++;
	}
	fprintf(fpdup, "</body></html>");
	fclose(fpdup);

	printf("\n\nCount: %d\n", count);
}

int ehVod(MW_EVENT event, int argi, void* argp)
{
	switch (event) {
	case MW_INIT:
		if (hashmap) return 0; 	// already inited
		vodInit();
		break;
	case MW_UNINIT:
		//un-initialization
		if (hashmap) {
			free(hashmap);
			hashmap = 0;
			if (vodloop) {
				free(vodloop);
				vodloop = 0;
			}
		}
		break;
	case MW_PARSE_ARGS: {
		int i = 0;
		char** argv = (char**)argp;
		for (i = 0; i < argi; i++) {
			if (!strcmp(argv[i], "--vodroot")) {
				vodroot = argv[++i];
			} else if (!strcmp(argv[i], "--vodloop")) {
				i++;
				if (vodhost) {
					vodloop = (char*)malloc(strlen(vodhost) + strlen(argv[i]) + 1);
					sprintf(vodloop, "%s%s", vodhost, argv[i]);
				} else {
					vodloop = strdup(argv[i]);
				}
			} else if (!strcmp(argv[i], "--vodhost")) {
				vodhost = argv[++i];
			}
		}
						}
	}
	return 0;
}

#define VOD_LOOP_COUNT 4

#ifdef WIN32
#define OPEN_FLAG O_RDONLY|0x8000
#else
#define OPEN_FLAG O_RDONLY
#endif

int uhVodStream(UrlHandlerParam* param)
{
	static int code = 0;
	static int prevtime = 0;
	int id;
	char* file;
	mwParseQueryString(param);
	file = mwGetVarValue(param->pxVars, "file", 0);
	id = mwGetVarValueInt(param->pxVars, "id" , -1);
	if (!file) {
		CLIP_INFO* clip = GetClipByHash(id, 0);
		if (clip) {
			snprintf(param->pucBuffer, param->dataBytes, "~%s/%s", vodroot, clip->filename);
			return FLAG_DATA_FILE;
		}
	} else {
		snprintf(param->pucBuffer, param->dataBytes, "~%s/%s", vodroot, file);
	}
	return 0;
}

int uhStream(UrlHandlerParam* param)
{
	char* file;
	mwParseQueryString(param);
	file = mwGetVarValue(param->pxVars, "file", 0);
	param->hs->fd = open(file, _O_BINARY|_O_RDONLY);
	return FLAG_DATA_FD;
}

static void OutputItemInfo(char** pbuf, int* pbufsize, char* id)
{
	char buf[256];
	CATEGORY_INFO2 *cat;
	CLIP_INFO* clip;
	int hash = atoi(id);
	if (hash > 0)
		clip = GetClipByHash(hash, &cat);
	else
		clip = GetClipByName(0, id, &cat);
	if (!clip) return;
	mwWriteXmlString(pbuf, pbufsize, 2, "<item>");
	snprintf(buf, sizeof(buf), "<id><![CDATA[%s]]></id>", id);
	mwWriteXmlString(pbuf, pbufsize, 3, buf);
	snprintf(buf, sizeof(buf), "<file><![CDATA[%s]]></file>", clip->filename);
	mwWriteXmlString(pbuf, pbufsize, 3, buf);
	snprintf(buf, sizeof(buf), "<title><![CDATA[%s]]></title>", clip->title);
	mwWriteXmlString(pbuf, pbufsize, 3, buf);
	snprintf(buf, sizeof(buf), "<category><![CDATA[%s]]></category>", cat->name);
	mwWriteXmlString(pbuf, pbufsize, 3, buf);
	mwWriteXmlString(pbuf, pbufsize, 2, "</item>");
}

int uhLib(UrlHandlerParam* param)
{
	char *pbuf;
	int bufsize;
	char buf[256];
	char *id;
	char *p;
	int from, count;

	p = strstr(param->pucRequest, "back=");
	if (p) *p = 0;
	mwParseQueryString(param);

	id = mwGetVarValue(param->pxVars, "id", 0);
	from = mwGetVarValueInt(param->pxVars, "from", 0);
	count = mwGetVarValueInt(param->pxVars, "count", -1);

	if (count <= 0)
		bufsize = 1024 * 1024;
	else
		bufsize = count * 256;

	pbuf = (char*)malloc(bufsize);
	param->pucBuffer = pbuf;

	mwWriteXmlHeader(&pbuf, &bufsize, 10, "gb2312", mwGetVarValue(param->pxVars, "xsl", 0));
	mwWriteXmlString(&pbuf, &bufsize, 0, "<response>");

	if (!strcmp(param->pucRequest, "/category")) {
		int mincount = mwGetVarValueInt(param->pxVars, "min", 2);
		int catid = id ? atoi(id) : -1;
		int hash = mwGetVarValueInt(param->pxVars, "hash", -1);
		char* name = mwGetVarValue(param->pxVars, "name", 0);
		CATEGORY_INFO2* cat;
		int i = 0;
		int idx = 0;
		for (cat = &cats; cat; cat = cat->next, i++) {
			if ((hash >= 0 && hash != cat->hash) || (name && strcmp(name, cat->name)) || (catid >= 0 && catid != i) || cat->count < mincount)
				continue;

			if (idx >= from) { 
				if ((count--) == 0) break;

				snprintf(buf, sizeof(buf), "<category id=\"%d\" hash=\"%02d\" index=\"%03d\">", i, cat->hash, idx);
				mwWriteXmlString(&pbuf, &bufsize, 1, buf);

				snprintf(buf, sizeof(buf), "<name><![CDATA[%s]]></name>", cat->name);
				mwWriteXmlString(&pbuf, &bufsize, 1, buf);

				snprintf(buf, sizeof(buf), "<clips>%d</clips>", cat->count);
				mwWriteXmlString(&pbuf, &bufsize, 1, buf);

				mwWriteXmlString(&pbuf, &bufsize, 1, "</category>");
			}
			idx++;
		}
	} else if (!strcmp(param->pucRequest, "/title")) {
		int hash = id ? atoi(id) : -1;
		int chars = mwGetVarValueInt(param->pxVars, "chars", 0);
		char* catname = mwGetVarValue(param->pxVars, "category", 0);
		int catid = mwGetVarValueInt(param->pxVars, "catid", -1);
		int i = 0;
		int idx = 0;
		BOOL matched = 0;
		CLIP_INFO* info;
		CATEGORY_INFO2* cat;
		int pos = 1;
		for (cat = &cats; cat; cat = cat->next, i++) {
			if ((catid >= 0 && catid != i) || (catname && strcmp(catname, cat->name))) continue;
			for (info = cat->clips; info; info = info->next) {
				if ((hash >= 0 && hash != info->hash) || (chars && info->chars != chars)) continue;
				if (idx >= from) { 
					if (count == 0) break;
					count--;
					if (!matched) {
						snprintf(buf, sizeof(buf), "<category name=\"%s\">", cat->name);
						mwWriteXmlString(&pbuf, &bufsize, 1, buf);
						matched = 1;
					}
					snprintf(buf, sizeof(buf), "<item id=\"%06d\" pos=\"%d\">", info->hash, pos++);
					mwWriteXmlString(&pbuf, &bufsize, 2, buf);

					snprintf(buf, sizeof(buf), "<name><![CDATA[%s]]></name>", info->title);
					mwWriteXmlString(&pbuf, &bufsize, 2, buf);

					snprintf(buf, sizeof(buf), "<chars>%d</chars>", info->chars);
					mwWriteXmlString(&pbuf, &bufsize, 2, buf);

					mwWriteXmlString(&pbuf, &bufsize, 2, "</item>");
				}
				idx++;
			}
			if (matched) mwWriteXmlString(&pbuf, &bufsize, 1, "</category>");
			if (count == 0) break;
			matched = FALSE;
		}
	} else if (!strcmp(param->pucRequest, "/chars")) {
		int i;
		for (i = 1; i <= MAX_CHARS; i++) {
			if (i > from) { 
				if ((count--) == 0) break;
				snprintf(buf, sizeof(buf), "<category chars=\"%d\" count=\"%d\"/>", i, charsinfo[i]);
				mwWriteXmlString(&pbuf, &bufsize, 2, buf);
			}
		}
	} else if (!strcmp(param->pucRequest, "/query")) {
		char buf[256];
		CATEGORY_INFO2 *cat;
		CLIP_INFO* clip;
		int idx = 0;
		int hash = id ? atoi(id) : -1;
		if (hash > 0)
			clip = GetClipByHash(hash, &cat);
		else
			clip = GetClipByName(0, id, &cat);
		if (!clip) clip = GetClipByFile(id, &cat);
		if (clip) {
			mwWriteXmlString(&pbuf, &bufsize, 2, "<item>");
			snprintf(buf, sizeof(buf), "<id><![CDATA[%d]]></id>", clip->hash);
			mwWriteXmlString(&pbuf, &bufsize, 3, buf);
			snprintf(buf, sizeof(buf), "<file><![CDATA[%s]]></file>", clip->filename);
			mwWriteXmlString(&pbuf, &bufsize, 3, buf);
			snprintf(buf, sizeof(buf), "<title><![CDATA[%s]]></title>", clip->title);
			mwWriteXmlString(&pbuf, &bufsize, 3, buf);
			snprintf(buf, sizeof(buf), "<category><![CDATA[%s]]></category>", cat->name);
			mwWriteXmlString(&pbuf, &bufsize, 3, buf);
			mwWriteXmlString(&pbuf, &bufsize, 2, "</item>");
		}
	}

	mwWriteXmlString(&pbuf, &bufsize, 0, "</response>");

	param->dataBytes=(int)(pbuf-param->pucBuffer);
	param->fileType=HTTPFILETYPE_XML;
	return FLAG_DATA_RAW | FLAG_TO_FREE;
}

static VOD_CTX* GetVodContext(DWORD ip)
{
	VOD_CTX* ctx = &vodctx;
	if (!ip) return NULL;
	for (;;) {
		if (ctx->ip == 0) break;
		if (ctx->ip == ip) return ctx;
		if (!ctx->next) {
			ctx->next = (VOD_CTX*)calloc(1, sizeof(VOD_CTX));
			ctx = ctx->next;
			ctx->next = 0;
			ctx->playlist = 0;
			break;
		}
		ctx = ctx->next;
	}
	ctx->ip = ip;
	return ctx;
}

int uhVod(UrlHandlerParam* param)
{
	HTTP_XML_NODE node;
	char *req=param->pucRequest;
	char *pbuf = param->pucBuffer;
	int bufsize = param->dataBytes;
	char *action;
	PL_ENTRY *ptr;
	VOD_CTX* ctx;
	int i;

	if (*req && *req != '?') return 0;
	node.indent = 1;
	node.name = "state";
	node.fmt = "%s";
	node.flags = 0;
	mwWriteXmlHeader(&pbuf, &bufsize, 10, "gb2312", mwGetVarValue(param->pxVars, "xsl", 0));
	mwWriteXmlString(&pbuf, &bufsize, 0, "<response>");

	mwParseQueryString(param);

	ctx = GetVodContext(param->hs->ipAddr.laddr);

	action = mwGetVarValue(param->pxVars, "action", 0);

	if (!action) {
		int count = mwGetVarValueInt(param->pxVars, "count", -1);
		ptr = ctx->playlist;
		mwWriteXmlString(&pbuf, &bufsize, 1, "<playlist>");
		for (i=0; ptr && (unsigned int)i < (unsigned int)count; ptr = ptr->next, i++) {
			char buf[32];
			snprintf(buf, sizeof(buf), "<item index=\"%03d\">", i);
			mwWriteXmlString(&pbuf, &bufsize, 2, buf);

			node.indent = 3;
			node.name = "stream";
			node.fmt = "%s";
			node.value = ptr->data;
			mwWriteXmlLine(&pbuf, &bufsize, &node, 0);

			node.flags = XN_CDATA;
			node.name = "title";
			node.value = (char*)ptr->data + strlen((char*)ptr->data) + 1;
			mwWriteXmlLine(&pbuf, &bufsize, &node, 0);

			mwWriteXmlString(&pbuf, &bufsize, 2, "</item>");
		}
		mwWriteXmlString(&pbuf, &bufsize, 1, "</playlist>");
	} else if (!strcmp(action, "add")) {
		char *filename = mwGetVarValue(param->pxVars, "stream", 0);
		char *title = mwGetVarValue(param->pxVars, "title", 0);
		if (!title) title = "";
		if (!filename) {
			node.value = "error";
		} else {
			int fnlen;
			int titlelen;
			char *entrydata;
			mwDecodeString(filename);
			fnlen = strlen(filename);
			titlelen = strlen(title);
			entrydata = (char*)malloc(fnlen + titlelen + 3);
			strcpy(entrydata, filename);
			strcpy(entrydata + fnlen + 1, title);
			node.value = plAddEntry(&ctx->playlist, entrydata, fnlen + titlelen + 2) ? "OK" : "error";
		}
		mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
	} else if (!strcmp(action, "pin")) {
		int index = mwGetVarValueInt(param->pxVars, "arg", 0);
		node.value = plPinEntryByIndex(&ctx->playlist, index) ? "OK" : "error";
		mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
	} else if (!strcmp(action, "del")) {
		int index = mwGetVarValueInt(param->pxVars, "arg", 0);
		void* data = plDelEntryByIndex(&ctx->playlist, index) ? "OK" : "error";
		if (data) {
			free(data);
			node.value = "OK";
		} else {
			node.value = "error";
		}
		mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
	} else if (!strcmp(action, "play")) {
		char* stream = (char*)plGetEntry(&ctx->playlist);
		if (stream) {
			node.name = "stream";
		}
		node.name = "stream";
		node.value = stream ? stream : vodloop;
		mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
		node.name = "control";
		node.fmt = "%s/vodplay?action=control";
		node.value = vodhost;
		mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
		if (stream) free(stream);
		ctx->nextaction = 0;
	} else if (!strcmp(action, "control")) {
		int arg = mwGetVarValueInt(param->pxVars, "arg", 0);
		if (arg)
			ctx->nextaction = arg;
		else if (ctx->nextaction) {
			node.name = "action";
			node.fmt = "%d";
			node.value = (void*)ctx->nextaction;
			mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
			ctx->nextaction = 0;
		}
	}

#if comment
	arg = mwGetVarValue(param->pxVars, "arg", 0);
	id = mwGetVarValue(param->pxVars, "id", 0);
	switch (GETDWORD(param->pucRequest + 1)) {
	case DEFDWORD('n','o','p',0):
		strcpy(pbuf,"state=OK");
		break;
	case DEFDWORD('c','m','d',0):
		//action=ACT_SKIP;
		strcpy(pbuf,"Play next");
		param->dataBytes=9;
		return FLAG_DATA_RAW;
	case DEFDWORD('l','i','s','t'): {
		PL_ENTRY *ptr = plhdr[session];
		int i;
		mwWriteXmlString(&pbuf, &bufsize, 1, "<playlist>");
		for (i=0; ptr; ptr = ptr->next, i++) {
			OutputItemInfo(&pbuf, &bufsize, ptr->data);
		}
		mwWriteXmlString(&pbuf, &bufsize, 1, "</playlist>");
									} break;
	case DEFDWORD('a','d','d',0): {
		node.name = "state";
		if (plFindEntry(plhdr[session],(void*)id, strlen(id))) {
			OutputItemInfo(&pbuf, &bufsize, id);
			node.value = "ordered";
			mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
			break;
		}
		if (plAddEntry(&plhdr[session], strdup(id), strlen(id) + 1)) {
			node.value =  "OK";
			OutputItemInfo(&pbuf, &bufsize, id);
		} else {
			node.value =  "error";
		}
		node.name = "state";
		mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
								  } break;
	case DEFDWORD('d','e','l',0):
		OutputItemInfo(&pbuf, &bufsize, id);
		node.name = "state";
		node.value = plDelEntry(&plhdr[session],(void*)id) ? "OK" : "error";
		mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
		break;
	case DEFDWORD('p','i','n',0):
		OutputItemInfo(&pbuf, &bufsize, id);
		node.name = "state";
		node.value = plPinEntry(&plhdr[session],(void*)id) ? "OK" : "error";
		mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
		break;
	case DEFDWORD('p','l','a','y'): {
		void* data = plGetEntry(&plhdr[session]);
		node.name = "state";
		if (data) {
			OutputItemInfo(&pbuf, &bufsize, data);
			node.value = "OK";
			free(data);
		} else {
			node.value = "error";
		}
		mwWriteXmlLine(&pbuf, &bufsize, &node, 0);
									} break;
	default:
		strcpy(pbuf,"Invalid request");
	}
#endif	
	mwWriteXmlString(&pbuf, &bufsize, 0, "</response>");
	param->dataBytes=(int)(pbuf-param->pucBuffer);
	param->fileType=HTTPFILETYPE_XML;
	return FLAG_DATA_RAW;
}

//////////////////////////////////////////////////////////////////
// Playlist implementation
//////////////////////////////////////////////////////////////////

PL_ENTRY* plAddEntry(PL_ENTRY **hdr, void* data, int datalen)
{
	PL_ENTRY *ptr=*hdr;
	if (!ptr) {
		// allocate header
		*hdr=(PL_ENTRY*)malloc(sizeof(PL_ENTRY));
		ptr=*hdr;
	} else {
		// travel to the end of list
		while (ptr->next) ptr=ptr->next;
		ptr->next=(PL_ENTRY*)malloc(sizeof(PL_ENTRY));
		ptr=ptr->next;
	}
	ptr->data=data;
	ptr->datalen = datalen;
	ptr->next=NULL;
	listcount++;
	return ptr;
}

void* plGetEntry(PL_ENTRY **hdr)
{
	PL_ENTRY *ptr=*hdr;
	void* data;
	if (!ptr) return NULL;
	data=ptr->data;
	*hdr=ptr->next;
	free(ptr);
	listcount--;
	return data;
}

PL_ENTRY* plFindEntry(PL_ENTRY *hdr, void* data, int datalen)
{
	PL_ENTRY *ptr;
	for (ptr=hdr; ptr; ptr=ptr->next) {
		if (!memcmp(ptr->data, data, min(datalen, ptr->datalen))) return ptr;
	}
	return NULL;
}

void* plDelEntry(PL_ENTRY **hdr, void* data)
{
	PL_ENTRY *ptr=*hdr,*prev=NULL;
	void* deleted;
	if (!ptr) return NULL;
	if (ptr->data==data) {
		deleted=ptr->data;
		*hdr=ptr->next;
		free(ptr);
		listcount--;
		return deleted;
	}
	for (prev=ptr, ptr=ptr->next; ptr; prev=ptr, ptr=ptr->next) {
		if (ptr->data==data) { 
			deleted=ptr->data;
			prev->next=ptr->next;
			free(ptr);
			listcount--;
			return deleted;
		}
	}
	return NULL;
}

void* plDelEntryByIndex(PL_ENTRY **hdr, int index)
{
	PL_ENTRY *ptr=*hdr,*prev=NULL;
	void* deleted;
	int i;
	if (!ptr) return NULL;
	if (index == 0) {
		deleted = ptr->data;
		*hdr = ptr->next;
		free(ptr);
		listcount--;
		return deleted;
	}
	for (i = 0; i < index && ptr; i++, prev = ptr, ptr = ptr->next);
	if (ptr) {
		deleted = ptr->data;
		prev->next = ptr->next;
		free(ptr);
		listcount--;
		return deleted;
	}
	return NULL;
}

PL_ENTRY* plPinEntry(PL_ENTRY **hdr, void* data)
{
	PL_ENTRY *ptr=*hdr,*prev=NULL;
	while (ptr) {
		if (ptr->data==data) {
			if (!prev) return ptr;
			prev->next=ptr->next;
			ptr->next=*hdr;
			*hdr=ptr;
			return ptr;
		}
		prev=ptr;
		ptr=ptr->next;
	}
	return NULL;
}

PL_ENTRY* plPinEntryByIndex(PL_ENTRY **hdr, int index)
{
	PL_ENTRY *ptr=*hdr,*prev=NULL;
	int i;
	if (index == 0 || !ptr) return ptr;
	for (i = 0; i < index && ptr; i++, prev = ptr, ptr = ptr->next);
	if (ptr) {
		prev->next = ptr->next;
		ptr->next = *hdr;
		*hdr = ptr;
	}
	return ptr;
}

int plEnumEntries(PL_ENTRY *hdr, PL_ENUM_CALLBACK pfnEnumCallback, 
	void *arg, int from, int count)
{
	PL_ENTRY *ptr=hdr;
	int hits;
	if (!pfnEnumCallback) return -1;
	for (hits=0;ptr;ptr=ptr->next,hits++) {
		if (count && hits>=from+count) return hits;
		if (hits>=from)
			if ((*pfnEnumCallback)(ptr, arg)) break;
	}
	return hits;
}

/////////////////////////////////////////////////////////////////////////////
//
// http.c
//
// MiniWeb HTTP POST implementation for 
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _NO_POST
#define DEBUG printf

////////////////////////////////////////////////////////////////////////////
// _mwFindMultipartBoundary
// Searches a memory buffer for a multi-part boundary string
////////////////////////////////////////////////////////////////////////////
OCTET* _mwFindMultipartBoundary(OCTET *poHaystack, int iHaystackSize, OCTET *poNeedle)
{
  int i;
  int iNeedleLength = (int)strlen((char*)poNeedle);
  
  ASSERT(iNeedleLength > 0);
  for (i=0; i <= (iHaystackSize-iNeedleLength-2); i++){
    if (*(poHaystack+i)==0x0d && *(poHaystack+i+1)==0x0a &&
        memcmp(poHaystack+i+2, poNeedle, iNeedleLength) == 0) {
      return (poHaystack+i);
    }
  }
  
  return NULL;
}

////////////////////////////////////////////////////////////////////////////
// mwFileUploadRegister
// Register a MULTIPART FILE UPLOAD callback
////////////////////////////////////////////////////////////////////////////
PFNFILEUPLOADCALLBACK mwFileUploadRegister(ServerThread *httpParam, PFNFILEUPLOADCALLBACK pfnUploadCb) 
{
  PFNFILEUPLOADCALLBACK pfnUploadPrevCb=httpParam->pfnFileUpload;
  
  // save new CB
  if (pfnUploadCb==NULL) return NULL;
  httpParam->pfnFileUpload=pfnUploadCb;
  
  // return previous CB (so app can chain onto it)
  return pfnUploadPrevCb;
}

////////////////////////////////////////////////////////////////////////////
// mwPostRegister
// Register a POST callback
////////////////////////////////////////////////////////////////////////////
PFNPOSTCALLBACK mwPostRegister(ServerThread *httpParam, PFNPOSTCALLBACK pfnPostCb) 
{
  PFNPOSTCALLBACK pfnPostPrevCb=httpParam->pfnPost;

  // save new CB
  if (pfnPostCb==NULL) return NULL;
  httpParam->pfnPost=pfnPostCb;

  // return previous CB (so app can chain onto it)
  return pfnPostPrevCb;
}

////////////////////////////////////////////////////////////////////////////
// _mwNotifyPostVars
// Process posted variables and do callback with parameter list
////////////////////////////////////////////////////////////////////////////
void _mwNotifyPostVars(HttpSocket* phsSocket, PostParam *pp)
{
  // if found any vars
  if (pp->iNumParams>0) {
    int iReturn;
    
    // call app callback to process post vars
    //ASSERT(g_httpParam.pfnPost!=NULL);
	iReturn=(((ServerThread*)pp->httpParam)->pfnPost)(pp);
    
#ifdef HTTPAUTH
    switch(iReturn) {
    case WEBPOST_AUTHENTICATIONON:
      DEBUG("Http authentication on\n");
	  SETFLAG(phsSocket,FLAG_AUTHENTICATION)
      break;
    case WEBPOST_AUTHENTICATIONOFF:
      DEBUG("Http authentication off\n");
	  CLRFLAG(phsSocket,FLAG_AUTHENTICATION)
      break;
    case WEBPOST_AUTHENTICATED:
      {
        struct sockaddr_in sinAddress;
        socklen_t sLength=sizeof(struct sockaddr_in);
        getpeername(phsSocket->socket,
                    (struct sockaddr*)&sinAddress,&sLength);
        
        g_httpParam.dwAuthenticatedNode=ntohl(sinAddress.sin_addr.s_addr);
        
        DEBUG("Http authenticated node %s\n",
               inet_ntoa(sinAddress.sin_addr));
        
        // Set authentication period
        g_httpParam.tmAuthExpireTime = time(NULL) + HTTPAUTHTIMEOUT;
      }
      break;
    case WEBPOST_NOTAUTHENTICATED:
      {
        struct sockaddr_in sinAddress;
        socklen_t sLength=sizeof(struct sockaddr_in);
        getpeername(phsSocket->socket,
                    (struct sockaddr*)&sinAddress,&sLength); 
        DEBUG("Http authentication fail! (%s NOT authenticated)\n",
               inet_ntoa(sinAddress.sin_addr));
        g_httpParam.dwAuthenticatedNode=0;
      }
      break;
    }
#endif
  }
  

  // was a redirect filename returned
  if (strlen(pp->pchPath) == 0) {
    // redirect to index page
    _mwRedirect(phsSocket, "/");
  }
}

////////////////////////////////////////////////////////////////////////////
// _mwProcessMultipartPost
// Process a multipart POST request
////////////////////////////////////////////////////////////////////////////
int _mwProcessMultipartPost(ServerThread *httpParam, HttpSocket* phsSocket, BOOL fNoRecv)
{
  int sLength = 0;
  char *pchBoundarySearch = NULL;
  HttpMultipart *pxMP = phsSocket->pxMP;
  
  if (pxMP == NULL) {
    return -1;
  }

  // Grab more POST data from the socket
  if (!fNoRecv) {
	  sLength = recv(phsSocket->socket, 
                 (char*)phsSocket->buffer + pxMP->writeLocation,
                 (int)(HTTPMAXRECVBUFFER - pxMP->writeLocation), 
                 0);
	  if (sLength < 0) {
		DEBUG("Socket closed by peer\n");
		return -1;
	  }  else if (sLength > 0) {
		// reset expiration timer
		phsSocket->tmExpirationTime=time(NULL)+httpParam->tmSocketExpireTime;
		pxMP->writeLocation += sLength;
	  } else {
		return 1;
	  }
  }
  
  
  
  //ASSERT(pxMP->writeLocation <= HTTPMAXRECVBUFFER);
  
  // Search new data for boundary indicator
  pchBoundarySearch = (char*)_mwFindMultipartBoundary(phsSocket->buffer, 
                                                 HTTPMAXRECVBUFFER, 
                                                 (uchar*)pxMP->pchBoundaryValue);
  
  for (; pchBoundarySearch != NULL; pchBoundarySearch = (char*)_mwFindMultipartBoundary(phsSocket->buffer, 
                                                   HTTPMAXRECVBUFFER, 
												   (uchar*)pxMP->pchBoundaryValue)) {
    if (pxMP->pchFilename) {
      // It's the last chunk of the posted file
      // Warning: MAY BE BIGGER THAN HTTPUPLOAD_CHUNKSIZE
      pxMP->oFileuploadStatus |= HTTPUPLOAD_LASTCHUNK;
      (httpParam->pfnFileUpload)(pxMP, phsSocket->buffer, (DWORD)pchBoundarySearch - (DWORD)phsSocket->buffer);
      pxMP->pchFilename = NULL;
      
      DEBUG("Multipart file POST on socket %d complete\n",
                   phsSocket->socket);
    }
    
    else {
      char *pchStart = _mwStrStrNoCase((char*)phsSocket->buffer, HTTP_MULTIPARTCONTENT);
      char *pchEnd = _mwStrDword((char*)phsSocket->buffer, HTTP_HEADEREND, 0);
      char *pchFilename = _mwStrStrNoCase((char*)phsSocket->buffer, HTTP_FILENAME);

      if (pchStart == NULL || pchEnd == NULL) {
		if (strncmp((char*)phsSocket->buffer + strlen(pxMP->pchBoundaryValue) + 2, "--\r\n",4) == 0) {
			// yes, we're all done
			_mwNotifyPostVars(phsSocket, &(pxMP->pp));
			DEBUG("Multipart POST on socket %d complete!\n", phsSocket->socket);
			return 1;
		} else {
			DEBUG("Waiting for multipart header description on socket %d\n", phsSocket->socket);
			break;
		}
      }
      
      if (pchFilename == NULL || 
          pchFilename > pchEnd) {    // check filename belongs to this variable
        // It's a normal (non-file) var
        // check we have the entire section (up to start of next boundary)
        pchFilename = NULL;
        if (strstr(pchEnd+4, "\r\n") == NULL) {
          DEBUG("Waiting for multipart variable value on socket %d\n",
                       phsSocket->socket);
          break;
        }
      }
      
      pchStart+=sizeof(HTTP_MULTIPARTCONTENT); // move past first quote
      pchEnd=strchr(pchStart,0x22);              // find end quote
      
      // Is peer authenticated to post this var?
	  if (
#ifdef HTTPAUTH
		_mwCheckAuthentication(phsSocket)
#else
		1
#endif
		) {
        
        pxMP->pp.stParams[pxMP->pp.iNumParams].pchParamName = (char*)calloc(pchEnd-pchStart+1, sizeof(char));
        memcpy(pxMP->pp.stParams[pxMP->pp.iNumParams].pchParamName, pchStart, pchEnd-pchStart);
        
        if (pchFilename!=NULL) {
          // use filename as var value
          pchStart=pchFilename+strlen(HTTP_FILENAME)+1;  // move past first quote
          pchEnd=strchr(pchStart,0x22);                  // find end quote
        } else {
          // use data as var value
          pchStart=_mwStrDword(pchEnd, HTTP_HEADEREND, 0) + 4;
          pchEnd=strstr(pchStart,"\r\n");
        }
        
        pxMP->pp.stParams[pxMP->pp.iNumParams].pchParamValue = (char*)calloc(pchEnd-pchStart+1, sizeof(char));  
        memcpy(pxMP->pp.stParams[pxMP->pp.iNumParams].pchParamValue, pchStart, 
               pchEnd-pchStart);
        
        DEBUG("Http multipart POST var %d [%s]=[%s]\n",
               pxMP->pp.iNumParams,
               pxMP->pp.stParams[pxMP->pp.iNumParams].pchParamName,
               pxMP->pp.stParams[pxMP->pp.iNumParams].pchParamValue);
        
        pxMP->pp.iNumParams++;
        
        if (pchFilename!=NULL) {
          pxMP->pchFilename = pxMP->pp.stParams[pxMP->pp.iNumParams-1].pchParamValue;
          
          // shift to start of file data
          pxMP->oFileuploadStatus = HTTPUPLOAD_FIRSTCHUNK;
          pchEnd=_mwStrDword(pchFilename, HTTP_HEADEREND, 0) + 4;  //move past "\r\n\r\n"
          pxMP->writeLocation -= (DWORD)pchEnd - (DWORD)phsSocket->buffer;
          memmove(phsSocket->buffer, pchEnd, pxMP->writeLocation);
		  if (pxMP->writeLocation == 0) break;
		  /*
          memset(phsSocket->buffer + pxMP->writeLocation, 0,
                HTTPMAXRECVBUFFER - pxMP->writeLocation);
				*/
          continue;
        } 
        else {
          // move to start of next boundary indicator
          pchBoundarySearch = pchEnd;
        }
      }
    }
    
    // Shift to start of next boundary section
    pxMP->writeLocation -= (DWORD)pchBoundarySearch - (DWORD)phsSocket->buffer;
    memmove(phsSocket->buffer, pchBoundarySearch, pxMP->writeLocation);
    memset(phsSocket->buffer + pxMP->writeLocation, 0, HTTPMAXRECVBUFFER - pxMP->writeLocation);
    
    if (strncmp((char*)phsSocket->buffer + strlen(pxMP->pchBoundaryValue) + 2, "--\r\n",4) == 0) {
		// yes, we're all done
		_mwNotifyPostVars(phsSocket, &(pxMP->pp));
		DEBUG("Multipart POST on socket %d complete!\n", phsSocket->socket);
		return 1;
    }
    
  }
  
  // check if buffer is full
  if (pxMP->writeLocation == HTTPMAXRECVBUFFER) {
    if (pxMP->pchFilename != NULL) {
      // callback with next chunk of posted file
		if ((httpParam->pfnFileUpload)(pxMP, phsSocket->buffer, HTTPUPLOAD_CHUNKSIZE)) {
			return 1;
		}
      pxMP->oFileuploadStatus = HTTPUPLOAD_MORECHUNKS;
      pxMP->writeLocation -= HTTPUPLOAD_CHUNKSIZE;
      memmove(phsSocket->buffer, phsSocket->buffer + HTTPUPLOAD_CHUNKSIZE, 
              HTTPMAXRECVBUFFER - HTTPUPLOAD_CHUNKSIZE);
      //memset(phsSocket->buffer + HTTPUPLOAD_CHUNKSIZE, 0, HTTPMAXRECVBUFFER - HTTPUPLOAD_CHUNKSIZE);
    } 
    else {
      DEBUG("Error, posted variable too large?\n");
	  DEBUG("%s\n", phsSocket->buffer);
      return -1;
    }
  }
  
  return 0;
} // end of _mwProcessMultipartPost

////////////////////////////////////////////////////////////////////////////
// _mwProcessPostVars
// Extract and process POST variables
// NOTE: the function damages the recvd data
////////////////////////////////////////////////////////////////////////////
void _mwProcessPostVars(ServerThread *httpParam, HttpSocket* phsSocket,
                          int iContentOffset,
                          int contentLength)
{
  BOOL bAuthenticated;
  
#ifdef HTTPAUTH
  bAuthenticated=_mwCheckAuthentication(phsSocket);
#else
  bAuthenticated=TRUE;
#endif

  //ASSERT(iContentOffset+contentLength<=phsSocket->dataLength);

  // extract the posted vars
  if (httpParam->pfnPost!=NULL) {
    int i;
    char* pchPos;
    char* pchVar=(char*)phsSocket->buffer+iContentOffset;
    PostParam pp;
    
    // init number of param block
    memset(&pp, 0, sizeof(PostParam));
	pp.httpParam = httpParam;
    
    // null terminate content data
    *(pchVar+contentLength)='\0';
    
    // process each param
    for (i=0;i<MAXPOSTPARAMS;i++) {
      // find =
      pchPos=strchr(pchVar,'=');
      if (pchPos==NULL) {
        break;
      }
      // terminate var name and add to parm list
      *pchPos='\0'; 
      pp.stParams[pp.iNumParams].pchParamName=pchVar;
      
      // terminate var value and add to parm list
      pp.stParams[pp.iNumParams].pchParamValue=pchPos+1;
      pchPos=strchr(pchPos+1,'&');
      if (pchPos!=NULL) {
        *pchPos='\0'; // null term current value
      }
      
      // if not authenticated then only process vars starting with .
      if (bAuthenticated || 
          (*pp.stParams[pp.iNumParams].pchParamName=='.')) {
        // convert any encoded characters
        mwDecodeString(pp.stParams[pp.iNumParams].pchParamValue);
        
        DEBUG("Http POST var %d [%s]=[%s]\n",
               pp.iNumParams,
               pp.stParams[pp.iNumParams].pchParamName,
               pp.stParams[pp.iNumParams].pchParamValue);
        
        pp.iNumParams++;
      } else {
        DEBUG("Http POST var [%s]=[%s] skipped - not authenticated\n",
               pp.stParams[pp.iNumParams].pchParamName,
               pp.stParams[pp.iNumParams].pchParamValue);
      }
      
      // if last var then quit
      if (pchPos==NULL) {
        break;
      }
      
      // move to next var
      pchVar=pchPos+1;
    }

    // process and callback with list of vars
    _mwNotifyPostVars(phsSocket, &pp);

  } else {
    // redirect to index page
    _mwRedirect(phsSocket, "/");
  }
} // end of _mwProcessPostVars

////////////////////////////////////////////////////////////////////////////
// _mwProcessPost
// Process a POST request 
////////////////////////////////////////////////////////////////////////////
void _mwProcessPost(ServerThread* httpParam, HttpSocket* phsSocket)
{
  int contentLength=-1;
  int iHeaderLength=0;
  
  //ASSERT(phsSocket->buffer!=NULL);
  
  // null terminate the buffer
  *(phsSocket->buffer+phsSocket->dataLength)=0;
  
  // find content length
  {
    char* pchContentLength;
    
    pchContentLength=strstr((char*)phsSocket->buffer, HTTP_CONTENTLENGTH);
    if (pchContentLength!=NULL) {
      pchContentLength+=strlen(HTTP_CONTENTLENGTH);
      contentLength=atoi(pchContentLength);
    }
  }
  
  // check if content length found
  if (contentLength>0) {
    
    // check if this is a multipart POST
    if ((HttpMultipart*)phsSocket->ptr == NULL) {
      char *pchMultiPart = _mwStrStrNoCase((char*)phsSocket->buffer, HTTP_MULTIPARTHEADER);
      
      if (pchMultiPart != NULL) {
        // We need the full HTTP header before processing (ends in '\r\n\r\n')
        char *pchHttpHeaderEnd = _mwStrDword((char*)phsSocket->buffer, HTTP_HEADEREND, 0);
        
        if (pchHttpHeaderEnd != NULL) {
          char *pchBoundarySearch = NULL;
          int iHttpHeaderLength = (DWORD)pchHttpHeaderEnd + 2 - (DWORD)phsSocket->buffer;
          
          DEBUG("Http multipart POST received on socket %d\n",
                 phsSocket->socket);
          
          // Allocate multipart structure information for socket
          phsSocket->ptr = calloc(1,sizeof(HttpMultipart));
          //ASSERT((HttpMultipart*)phsSocket->ptr != NULL);
          
          // What is the 'boundary' value
          strcpy(((HttpMultipart*)phsSocket->ptr)->pchBoundaryValue,"--");
          pchBoundarySearch = _mwStrStrNoCase((char*)phsSocket->buffer, HTTP_MULTIPARTBOUNDARY);
          if (pchBoundarySearch != NULL) {
            sscanf(pchBoundarySearch+9,"%s",
                   ((HttpMultipart*)phsSocket->ptr)->pchBoundaryValue+2);
          } else {
            DEBUG("Error! Http multipart POST header recvd on socket %d does not contain a boundary value\n",
                   phsSocket->socket);
            SETFLAG(phsSocket, FLAG_CONN_CLOSE);
            return;
          }
          
          //ASSERT(phsSocket->buffer != NULL);
          
          // Shift window to start at first boundary indicator
          ((HttpMultipart*)phsSocket->ptr)->writeLocation = 
            phsSocket->dataLength - iHttpHeaderLength;
          //ASSERT(((HttpMultipart*)phsSocket->ptr)->writeLocation >= 0);
          memmove(phsSocket->buffer, pchHttpHeaderEnd + 2, 
                  ((HttpMultipart*)phsSocket->ptr)->writeLocation);
          memset(phsSocket->buffer + ((HttpMultipart*)phsSocket->ptr)->writeLocation, 0,
                HTTPMAXRECVBUFFER - ((HttpMultipart*)phsSocket->ptr)->writeLocation);
        } 
        else {
          DEBUG("Http multipart POST on socket %d waiting for additional header info\n",
                       phsSocket->socket);
        }
        
        return;
      }
    }
    
    // it's a normal POST. find body of message
    {
      int iLineLength;
      
      do {
        iLineLength=(int)strcspn((char*)phsSocket->buffer+iHeaderLength,"\r\n");
        iHeaderLength+=(iLineLength+2); // move to next line
      } while (iLineLength>0 && iHeaderLength<=phsSocket->dataLength);
    }
    
    // check if we have the whole message
    if (iHeaderLength+contentLength <= phsSocket->dataLength) {
      // process the variables
      _mwProcessPostVars(httpParam, phsSocket,iHeaderLength,contentLength);
    } else {
      // not enough content received yet
      DEBUG("Http POST on socket %d waiting for additional data (%d of %d recvd)\n",
                   phsSocket->socket,phsSocket->dataLength-iHeaderLength,
                   contentLength);
    }
  } else {
    #if comment
    // header does not contain content length
    SYSLOG(LOG_ERR,"Error! Http POST header recvd on socket %d does not contain content length\n",
           phsSocket->socket);
    #endif
   
  }
} // end of _mwProcessPost
#endif	//HTTPPOST

/////////////////////////////////////////////////////////////////////////////
//
// httppil.c
//
// MiniWeb Platform Independent Layer
//
/////////////////////////////////////////////////////////////////////////////
int InitSocket()
{
#ifdef WIN32
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 1), &wsaData) && 
		WSAStartup(MAKEWORD(1, 1), &wsaData )) {
			return 0;
	}
#endif
	return 1;   
}

void UninitSocket()
{
#ifdef WIN32
	WSACleanup();
#endif
}

char *GetTimeString()
{
	static char buf[16];
	time_t tm=time(NULL);
	memcpy(buf,ctime(&tm)+4,15);
	buf[15]=0;
	return buf;
}

#ifndef NOTHREAD
int ThreadCreate(pthread_t *pth, void* (*start_routine)(void*), void* arg)
{
#ifndef HAVE_PTHREAD
	DWORD dwid;	    
	*pth=CreateThread(0,0,(LPTHREAD_START_ROUTINE)start_routine,arg,0,&dwid);
	return *pth!=NULL?0:1;
#else
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	// Change policy
	pthread_attr_setschedpolicy(&attr,   SCHED_RR);
	int ret = pthread_create(pth, &attr, start_routine, arg);
	pthread_attr_destroy(&attr);
	return ret;
#endif
}

int ThreadKill(pthread_t pth)
{
#ifndef HAVE_PTHREAD
	return TerminateThread(pth,0)?0:1;
#else
	return pthread_cancel(pth);
#endif
}

int ThreadWait(pthread_t pth, int timeout, void** ret)
{
#ifndef HAVE_PTHREAD
	if (WaitForSingleObject(pth, timeout)==WAIT_TIMEOUT)
		return 1;
	if (ret) GetExitCodeThread(pth,(LPDWORD)ret);
	CloseHandle(pth);
	return 0;
#else
	return pthread_join(pth,ret);
#endif
}

void MutexCreate(pthread_mutex_t* mutex)
{
#ifndef HAVE_PTHREAD
	*mutex = CreateMutex(0,FALSE,NULL);
#else
	pthread_mutex_init(mutex,NULL);
#endif
}

void MutexDestroy(pthread_mutex_t* mutex)
{
#ifndef HAVE_PTHREAD
	CloseHandle(*mutex);
#else
	pthread_mutex_destroy(mutex);
#endif
}

void MutexLock(pthread_mutex_t* mutex)
{
#ifndef HAVE_PTHREAD
	WaitForSingleObject(*mutex,INFINITE);
#else
	pthread_mutex_lock(mutex);
#endif
}

void MutexUnlock(pthread_mutex_t* mutex)
{
#ifndef HAVE_PTHREAD
	ReleaseMutex(*mutex);
#else
	pthread_mutex_unlock(mutex);
#endif
}

#endif

int IsDir(const char* pchName)
{
#ifdef WIN32
	DWORD attr=GetFileAttributes(pchName);
	//if (attr==INVALID_FILE_ATTRIBUTES) return 0;
	return (attr & FILE_ATTRIBUTE_DIRECTORY)?1:0;
#else
	struct stat stDirInfo;
	if (stat( pchName, &stDirInfo) < 0) return 0;
	return (stDirInfo.st_mode & S_IFDIR)?1:0;
#endif //WIN32
}

int ReadDir(const char* pchDir, char* pchFileNameBuf)
{
#ifdef WIN32
	static HANDLE hFind=NULL;
	WIN32_FIND_DATA finddata;

	if (!pchFileNameBuf) {
		if (hFind) {
			FindClose(hFind);
			hFind=NULL;
		}
		return 0;
	}
	if (pchDir) {
		char *p;
		int len;
		if (!IsDir(pchDir)) return -1;
		if (hFind) FindClose(hFind);
		len = strlen(pchDir);
		p = (char*)malloc(len + 5);
		snprintf(p, len + 5, "%s\\*.*", pchDir);
		hFind=FindFirstFile(p,&finddata);
		free(p);
		if (hFind==INVALID_HANDLE_VALUE) {
			hFind=NULL;
			return -1;
		}
		strcpy(pchFileNameBuf,finddata.cFileName);
		return 0;
	}
	if (!hFind) return -1;
	if (!FindNextFile(hFind,&finddata)) {
		FindClose(hFind);
		hFind=NULL;
		return -1;
	}
	strcpy(pchFileNameBuf,finddata.cFileName);
#else
	static DIR *stDirIn=NULL;
	struct dirent *stFiles;

	if (!pchFileNameBuf) {
		if (stDirIn) {
			closedir(stDirIn);
			stDirIn=NULL;
		}
		return 0;
	}
	if (pchDir) {
		if (!IsDir(pchDir)) return -1;
		if (stDirIn) closedir(stDirIn);
		stDirIn = opendir( pchDir);
	}
	if (!stDirIn) return -1;
	stFiles = readdir(stDirIn);
	if (!stFiles) {
		closedir(stDirIn);
		stDirIn=NULL;
		return -1;
	}
	strcpy(pchFileNameBuf, stFiles->d_name);
#endif
	return 0;
}

int IsFileExist(const char* filename)
{
	struct stat s;
	return stat(filename, &s) == 0;
}

//////////////////////////////////////////////////////////////////////////
// callback from the web server whenever a valid request comes in
//////////////////////////////////////////////////////////////////////////
int uhStats(UrlHandlerParam* param)
{
	char *p;
	char buf[128];
	HttpStats *stats=&((ServerThread*)param->hp)->stats;
	HttpRequest *req=&param->hs->request;
	IPADDR ip = param->hs->ipAddr;
	HTTP_XML_NODE node;
	int bufsize = param->dataBytes;
	int ret=FLAG_DATA_RAW;

	mwGetHttpDateTime(time(NULL), buf, sizeof(buf));

	if (stats->clientCount>4) {
		param->pucBuffer=(char*)malloc(stats->clientCount*256+1024);
		ret=FLAG_DATA_RAW | FLAG_TO_FREE;
	}
		
	p=param->pucBuffer;
	
	//generate XML
	mwWriteXmlHeader(&p, &bufsize, 10, 0, 0);

	mwWriteXmlString(&p, &bufsize, 0, "<ServerStats>");

	sprintf(buf, "%d.%d.%d.%d", ip.caddr[3], ip.caddr[2], ip.caddr[1], ip.caddr[0]);

	node.indent = 1;
	node.fmt = "%s";
	node.name = "ClientIP";
	node.value = buf;
	mwWriteXmlLine(&p, &bufsize, &node, 0);

	node.fmt = "%d";
	node.name = "UpTime";
	node.value = (void*)(time(NULL)-stats->startTime);
	mwWriteXmlLine(&p, &bufsize, &node, 0);

	node.fmt = "%d";
	node.name = "MaxClients";
	node.value = (void*)(stats->clientCountMax);
	mwWriteXmlLine(&p, &bufsize, &node, 0);

	node.fmt = "%d";
	node.name = "Requests";
	node.value = (void*)(stats->reqCount);
	mwWriteXmlLine(&p, &bufsize, &node, 0);

	node.fmt = "%d";
	node.name = "FileSent";
	node.value = (void*)(stats->fileSentCount);
	mwWriteXmlLine(&p, &bufsize, &node, 0);

	node.fmt = "%d";
	node.name = "ByteSent";
	node.value = (void*)(stats->fileSentBytes);
	mwWriteXmlLine(&p, &bufsize, &node, 0);

	mwWriteXmlString(&p, &bufsize, 1, "<Clients>");

	{
		HttpSocket *phsSocketCur;
		time_t curtime=time(NULL);
		for (phsSocketCur=((ServerThread*)param->hp)->phsSocketHead; phsSocketCur; phsSocketCur=phsSocketCur->next) {
			ip=phsSocketCur->ipAddr;
			sprintf(buf,"<Client ip=\"%d.%d.%d.%d\" requests=\"%d\" expire=\"%d\"/>",
				ip.caddr[3],ip.caddr[2],ip.caddr[1],ip.caddr[0],phsSocketCur->iRequestCount,phsSocketCur->tmExpirationTime-curtime);
			mwWriteXmlString(&p, &bufsize, 2, buf);
			/*
			if (phsSocketCur->request.pucPath)
				p+=sprintf(p,"(%d/%d)",phsSocketCur->response.iSentBytes,phsSocketCur->response.iContentLength);
			else
				p+=sprintf(p,"(idle)");
			*/
		}
	}
	
	mwWriteXmlString(&p, &bufsize, 1, "</Clients>");
	mwWriteXmlString(&p, &bufsize, 0, "</ServerStats>");

	//return data to server
	param->dataBytes=(int)p-(int)(param->pucBuffer);
	param->fileType=HTTPFILETYPE_XML;
	return ret;
}

#ifdef _7Z

int uh7Zip(UrlHandlerParam* param)
{
	HttpRequest *req=&param->hs->request;
	ServerThread *hp= (ServerThread*)param->hp;
	char *path;
	char *filename;
	void *content;
	int len;
	char *p = strchr(req->pucPath, '/');
	if (p) p = strchr(p + 1, '/');
	if (!p) return 0;
	filename = p + 1;
	*p = 0;
	path = (char*)malloc(strlen(req->pucPath) + strlen(hp->pchWebPath) + 5);
	sprintf(path, "%s/%s.7z", hp->pchWebPath, req->pucPath);
	*p = '/';
	
	if (!IsFileExist(path)) {
		free(path);
		return 0;
	}
	
	len = SzExtractContent(hp->szctx, path, filename, &content);
	free(path);
	if (len < 0) return 0;

	p = strrchr(filename, '.');
	param->fileType = p ? mwGetContentType(p + 1) : HTTPFILETYPE_OCTET;
	param->dataBytes = len;
	param->pucBuffer = content;
	return FLAG_DATA_RAW;
}

#endif

#ifdef WIN32
void FileReadThread(UrlHandlerParam* param)
{
	int bytes;
	fd_set fds;
	struct timeval timeout;
	FILE *fp = fopen("f:\\11.xml", "rb");
	ServerThread* hp = (ServerThread*)param->hp;
	HttpSocket* phsSocket = param->hs;
	SOCKET s = phsSocket->socket;
	char buf[1024];

	free(param);
	param = 0;


	FD_ZERO(&fds);
	FD_SET(s, &fds);

	/* Set time limit. */
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;

	/* build http response header */
	phsSocket->dataLength=_mwBuildHttpHeader(
		phsSocket,
		time(0),
		phsSocket->pucData);
	phsSocket->response.fileType = HTTPFILETYPE_XML;
	phsSocket->response.headerBytes = phsSocket->dataLength;
	phsSocket->response.sentBytes = 0;

	/* wait until the socket is allowed to send */
	while (!ISFLAGSET(phsSocket,FLAG_SENDING)) msleep(100);

	bytes = send(s, (char*)phsSocket->pucData, phsSocket->dataLength, 0);
	for (;;) {
		bytes = fread(buf, 1, sizeof(buf), fp);
		if (bytes > 0) {
			int rc = select(1, NULL, &fds, NULL, &timeout);
			if (rc==-1) {
				break;
			} else if (rc > 0) {
				bytes = send(s, buf, bytes, 0);
			} else {
				continue;
			}
		} else
			break;
	}
	/* tear down connection */
	SETFLAG(phsSocket, FLAG_CONN_CLOSE);
	_mwCloseSocket(hp, phsSocket);
}

int uhFileStream(UrlHandlerParam* param)
{
	if (!param->hs->ptr) {
		// first request
		DWORD dwid;
		UrlHandlerParam* p = (UrlHandlerParam*)malloc(sizeof(UrlHandlerParam));
		memcpy(p, param, sizeof(UrlHandlerParam));
		param->hs->ptr = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)FileReadThread, p, 0, &dwid);
	}
	return FLAG_DATA_SOCKET;
}
#endif

//////////////////////////////////////////////////////////////////////////
// stream handler sample
//////////////////////////////////////////////////////////////////////////
#ifndef NOTHREAD
typedef struct {
	int state;
	pthread_t thread;
	char result[16];
} HANDLER_DATA;

void* WriteContent(HANDLER_DATA* hdata)
{
	char *p = hdata->result;
	int i;
	for (i = 0; i < 10; i++, p++) {
		*p = '0' + i;
		msleep(100);
	}
	*p = 0;
	return 0;
}

int uhAsyncDataTest(UrlHandlerParam* param)
{
	int ret = FLAG_DATA_STREAM | FLAG_TO_FREE;
	HANDLER_DATA* hdata = (HANDLER_DATA*)param->hs->ptr;
	
	if (param->pucBuffer) {
		if (!hdata) {
			// first invoke
			param->hs->ptr = calloc(1, sizeof(HANDLER_DATA));
			hdata = (HANDLER_DATA*)param->hs->ptr;
			ThreadCreate(&hdata->thread, (void*(*)(void*))WriteContent, hdata);
			param->dataBytes = 0;
		} else {
			if (hdata->state == 1) {
				// done
				ret = 0;
			} else if (ThreadWait(hdata->thread, 10, 0)) {
				// data not ready
				param->dataBytes = 0;
			} else {
				// data ready
				strcpy(param->pucBuffer, hdata->result);
				param->dataBytes = strlen(param->pucBuffer);
				hdata->state = 1;
			}
		}
	} else {
		// cleanup
		ret = 0;
	}
	param->fileType=HTTPFILETYPE_TEXT;
	return ret;
}
#endif

/*******************************************************************
* HTTP helper
*******************************************************************/

/****************************************************************************
* HTTP protocol client
****************************************************************************/
#define HEADER_END_FLAG  "\r\n\r\n"
#ifdef _DEBUG
#define DEBUG printf
#else
#define DEBUG
#endif
#define CONN_RETRIES 3
#define HTTP_GET_HEADER "%s %s HTTP/1.%d\r\nAccept: */*\r\nConnection: %s\r\nUser-Agent: Transerver/1.0\r\nHost: %s\r\n%s\r\n"
#define HTTP_POST_HEADER "POST %s HTTP/1.0\r\nHost: %s\r\nContent-Type: application/x-www-form-urlencoded\r\nUser-Agent: Mozilla/5.0\r\nContent-Length: %d\r\n%s\r\n"
#define HTTP_POST_MULTIPART_HEADER "POST %s HTTP/1.0\r\nHost: %s\r\nUser-Agent: Mozilla/5.0\r\nAccept: text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\nAccept-Language: en-us,en;q=0.5\r\nAccept-Encoding: gzip,deflate\r\nAccept-Charset: ISO-8859-1;q=0.7,*;q=0.7\r\nKeep-Alive: 300\r\nConnection: keep-alive\r\nContent-Type: multipart/form-data; boundary=%s\r\nContent-Length: %d\r\n%s\r\n"
#define MULTIPART_BOUNDARY "---------------------------24464570528145"
#define HTTP_POST_STREAM_HEADER "POST %s HTTP/1.0\r\nHost: %s\r\nUser-Agent: Mozilla/5.0\r\nAccept: text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\nAccept-Language: en-us,en;q=0.5\r\nAccept-Encoding: gzip,deflate\r\nAccept-Charset: ISO-8859-1;q=0.7,*;q=0.7\r\nKeep-Alive: 300\r\nConnection: close\r\nContent-Type: application/octet-stream; filename=%s\r\nContent-Length: %d\r\n%s\r\n"

void httpInitReq(HTTP_REQUEST* req, char* proxy)
{
	memset(req, 0, sizeof(HTTP_REQUEST));
	if (proxy && *proxy) req->proxy = proxy;
}

static char* parseURL(const char* url, HTTP_REQUEST* param)
{
	char *ptr;
	size_t len;
	char *path;

	if (strncmp(url,"http://",7) != 0 ) { 
		return NULL;
	}
	
	url += 7;
	ptr = (char*)strchr(url, '/');
	if (ptr == NULL) {
		path="/";
		len = strlen(url);
	} else {
		len = (int)(ptr - url);
		path = ptr;
	}
	ptr = (char*)strchr(url, ':');
	if (ptr && *(ptr + 1) != '/') {
		unsigned short int port = atoi(ptr + 1);
		if (port) {
			param->port = port;
		}
		len = (int)(ptr - url);
	}
	param->hostname = (char*)malloc(len + 1);
	memcpy(param->hostname, url, len);
	param->hostname[len] = 0;
	if (!param->port) param->port = 80;
	return path;
}
	
#define MAX_HEADER_SIZE (8192 - 1)

size_t httpSend(HTTP_REQUEST* param, char* data, size_t length)
{
	size_t offset = 0;
	size_t bytes;
	do {
		bytes = send(param->sockfd, data + offset, (int)(length - offset), 0);
		if (bytes <= 0) break;
		offset += bytes;
		DEBUG("Sent %d/%d bytes\n", offset, length);
	} while(offset < length);
	return offset;
}

void httpClean(HTTP_REQUEST* param)
{
	if (param->sockfd) {
		closesocket(param->sockfd);
		param->sockfd = 0;
	}
	if (param->buffer) {
		free(param->buffer);
		param->buffer = 0;
	}
	if (param->hostname){
		free(param->hostname);
		param->hostname = 0;
	}
	if (param->header) {
		free(param->header);
		param->header = 0;
	}
}

int httpRequest(HTTP_REQUEST* param, const char* url)
{
	const char *path;
	struct hostent *target_host;
	int ret = 0;
	int bytes;

	if (url) param->url = url;
	param->state = HS_REQUESTING;
	param->payloadSize=0;

	if (param->proxy) {
		parseURL(param->proxy, param);
		path = param->url;
	} else {
		path = parseURL(param->url, param);
		if (!path) return -1;
	}

	if (param->header) free(param->header);
	param->header = (char*)malloc(MAX_HEADER_SIZE + 1);
	if ((param->bytesStart|param->bytesEnd) == 0) {
		char headerAddition[128];
		headerAddition[0] = 0;
		if (param->referer) {
			snprintf(headerAddition, sizeof(headerAddition), "Referer: %s\r\n", param->referer);
		}
		switch (param->method) {
		case HM_GET:
			snprintf(param->header, MAX_HEADER_SIZE + 1, HTTP_GET_HEADER, "GET",
				path, param->httpVer, (param->flags & FLAG_KEEP_ALIVE) ? "Keep-Alive" : "close", param->hostname, headerAddition);
			break;
		case HM_HEAD:
			snprintf(param->header, MAX_HEADER_SIZE + 1, HTTP_GET_HEADER, "HEAD",
				path, param->httpVer, (param->flags & FLAG_KEEP_ALIVE) ? "Keep-Alive" : "close", param->hostname, headerAddition);
			break;
		case HM_POST:
			snprintf(param->header, MAX_HEADER_SIZE + 1, HTTP_POST_HEADER, path, param->hostname, param->postPayloadBytes, headerAddition);
			break;
		case HM_POST_STREAM: {
			snprintf(param->header, MAX_HEADER_SIZE + 1, HTTP_POST_STREAM_HEADER, path, param->hostname, param->filename, 0, headerAddition);
			break;
			} break;
		case HM_POST_MULTIPART: {
			size_t bytes = 0;
			int i;
			for (i = 0; i < param->chunkCount; i++) {
				if (param->chunk[i].type == postPayload_STRING) {
					param->chunk[i].length = strlen((char*)param->chunk[i].data);
				}
				bytes += param->chunk[i].length;
				bytes += sizeof(MULTIPART_BOUNDARY) - 1 + 6;
			}
			bytes += sizeof(MULTIPART_BOUNDARY) - 1 + 6;
			snprintf(param->header, MAX_HEADER_SIZE + 1, HTTP_POST_MULTIPART_HEADER, path, param->hostname, MULTIPART_BOUNDARY, bytes, headerAddition);
			} break;
		}
	} else {
		char tokenRange[48],*p=tokenRange;
		p+=snprintf(p, sizeof(tokenRange), "Range: bytes=%u-", param->bytesStart);
		if (param->bytesEnd>0) {
			snprintf(p, 16, "%u\r\n",param->bytesEnd);
		} else {
			strcpy(p,"\r\n");
		}
		snprintf(param->header, MAX_HEADER_SIZE + 1, HTTP_GET_HEADER, path, "close", param->hostname, tokenRange);
	}
	
	do {
		int hdrsize = (int)strlen(param->header);
		int retry = 3;
		ret = 0;
		do {
		if (!param->sockfd) {
			struct sockaddr_in server_addr;

			if ((target_host = gethostbyname((const char*)param->hostname)) == NULL) {
				ret = -1;
				continue;
			}

			if ((param->sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
				DEBUG("Failed to open socket\n");
				ret = -1;
				continue;
			}

			memset(&server_addr.sin_zero,0,8);
			server_addr.sin_family = AF_INET;
			server_addr.sin_addr.s_addr = ((struct in_addr *)(target_host->h_addr))->s_addr;
			server_addr.sin_port = htons(param->port);
			DEBUG("Connecting to server...\n");

			if (connect(param->sockfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr)) < 0) {
				DEBUG("Failed to connect\n");
				ret = -1;
				continue;
			}
		}
		DEBUG("Sending request...\n");
		if (httpSend(param, param->header, hdrsize) != hdrsize) {
				closesocket(param->sockfd);
				param->sockfd = 0;
				ret = -1;
				continue;;
		}
			break;
		} while (--retry > 0);
		if (ret == -1) break;

		if (param->method == HM_POST) {
			if (httpSend(param, param->postPayload, param->postPayloadBytes) != param->postPayloadBytes) break;
		} else if (param->method == HM_POST_MULTIPART) {
			POST_CHUNK* chunk;
			int i;
			char* sendbuf = (char*)malloc(POST_BUFFER_SIZE);
			for (i = 0; i < param->chunkCount && param->state != HS_STOPPING; i++) {
				chunk = param->chunk + i;
				bytes = snprintf(sendbuf, POST_BUFFER_SIZE, "--%s\r\n", MULTIPART_BOUNDARY);
				if (httpSend(param, sendbuf, bytes) != bytes) break;
				switch (chunk->type) {
				case postPayload_STRING:
				case postPayload_BINARY:
					if (httpSend(param, (char*)chunk->data, chunk->length) != chunk->length) break;
					break;
				case postPayload_FD:
					for(;;) {
						bytes = read((int)chunk->data, sendbuf, POST_BUFFER_SIZE);
						if (bytes <= 0) break;
						if (httpSend(param, sendbuf, bytes) != bytes) break;
					}
					break;
				case postPayload_CALLBACK:
					for(;;) {
						bytes = (*(PFNpostPayloadCALLBACK)chunk->data)(sendbuf, POST_BUFFER_SIZE);
						if (bytes < 0) {
							param->state = HS_STOPPING;
							break;
						}
						if (bytes == 0) break;
						if (httpSend(param, sendbuf, bytes) != bytes) break;
					}
					break;
				}
				if (httpSend(param, "\r\n", 2) != 2) break;
			}
			free(sendbuf);
			httpSend(param, "--", 2);
			httpSend(param, MULTIPART_BOUNDARY, sizeof(MULTIPART_BOUNDARY) - 1);
			httpSend(param, "--\r\n", 4);
		}
	} while(0);

	if (!ret && param->method != HM_POST_STREAM && param->method != HM_POST_MULTIPART)
		return httpGetResponse(param);

	return ret;
}

int httpGetResponse(HTTP_REQUEST* param)
{
	int receivedBytes;
	int rspHeaderBytes;
	int ret;
	char *p = 0;

	do {
		//receive header
		DEBUG("Receiving response...\n");
		receivedBytes=0;
		for(;;) {
			ret = recv(param->sockfd, param->header + receivedBytes, MAX_HEADER_SIZE-receivedBytes, 0);
			if (ret <= 0) break;
			receivedBytes += ret;
			param->header[receivedBytes] = 0;
			DEBUG("Received %d/%d bytes\n", ret, receivedBytes);
			if ((p = strstr(param->header,"\r\n\r\n"))) {
				*(p+2) = '\0';
				break;
			}
			if (receivedBytes == MAX_HEADER_SIZE) {
				p = param->header + receivedBytes - 4;
			}
		}
		if (!p) {
			DEBUG("Invalid server response\n");
			ret = -1;
			continue;
		} else {
			ret = 0;
			p+=4;
		}
		rspHeaderBytes = (int)(p - param->header);

	//process header
	{
		char *p;
		DEBUG("Response header:\n%s\n", param->header);
		if (p = strstr(param->header,"HTTP/1.")) {
			param->httpCode = atoi(p+9);
		}
		if (param->httpCode >= 404) {
			DEBUG("Invalid response or file not found on server\n");
			closesocket(param->sockfd);
			param->sockfd=0;
			param->state=HS_IDLE;
			free(param->header);
			param->header = 0;
			return -1;
		}
		while ((p = strstr(p, "\r\n"))) {
			char *q;
			q=strchr((p += 2),':');
			if (!q) continue;
			*q = 0;
			if (!stricmp(p,"Content-length")) {
				param->payloadSize=atoi(q+2);
			} else if (!stricmp(p,"Content-type")) {
				param->contentType = q+2;
			} else if (!stricmp(p, "Transfer-Encoding")) {
				if (!strncmp(p + 19, "chunked", 7)) {
					param->flags |= FLAG_CHUNKED;
				}
			} else if (!_stricmp(p, "Location")) {
				param->location = q + 2;
			}
			*q = ':';
		}
	}
	DEBUG("Payload bytes: %d\n",param->payloadSize);
	if (param->payloadSize == 0 && param->dataSize > 0)
		param->payloadSize = param->dataSize - 1;
	if (param->flags & FLAG_REQUEST_ONLY) {
		closesocket(param->sockfd);
		param->state=HS_IDLE;
		return 0;
	}
	// receive payload
	if (param->method != HM_HEAD) {
		int bytes;
		int recvBytes = 0;
		int bytesToRecv;
		int payloadWithHeader = receivedBytes - rspHeaderBytes;
		int eof = 0;
		param->state=HS_RECEIVING;
		do {
			if (param->bufferSize > 0) {
				if (param->payloadSize > 0) {
					if (param->bufferSize < param->payloadSize + 1) {
						free(param->buffer);
						param->bufferSize = param->payloadSize + 1;
						param->buffer = (char*)calloc(1, param->bufferSize);
					}
				} else {
					param->bufferSize *= 2;
					param->buffer = (char*)realloc(param->buffer, param->bufferSize);
				}
			} else {
				DEBUG("Allocating %d bytes for payload buffer\n",param->payloadSize);
				if (param->payloadSize) {
					param->bufferSize = param->payloadSize + 1;
					param->buffer = (char*)calloc(1, param->bufferSize);
				} else {
					param->bufferSize = 1024 + payloadWithHeader;
					param->buffer = (char*)calloc(1, param->bufferSize);
				}
			}
			if (recvBytes == 0) {
				recvBytes = payloadWithHeader;
				if (recvBytes > 0) {
					DEBUG("Header includes %d bytes of payload\n", recvBytes);
					memcpy(param->buffer, param->header + rspHeaderBytes, recvBytes);
				}
			}
			//receive payload data
			bytesToRecv = param->payloadSize ? param->payloadSize : param->bufferSize - 1;
			for (; recvBytes < bytesToRecv; recvBytes += bytes) {
				bytes = recv(param->sockfd, param->buffer + recvBytes, bytesToRecv - recvBytes, 0);
				if (bytes<=0) {
					eof = 1;
					break;
				}
			}
			if (recvBytes == param->payloadSize) eof = 1;
			DEBUG("Payload received: %d bytes\n", recvBytes);
		} while (!eof);
		DEBUG("End of stream\n");
		*(param->buffer + recvBytes)=0;
		param->dataSize = recvBytes;
	}
	
	} while(0);
	if (param->sockfd && !(param->flags & FLAG_KEEP_ALIVE)) {
		closesocket(param->sockfd);
		param->sockfd=0;
	}
	param->state=HS_IDLE;
	if (!(param->flags & FLAG_KEEP_HEADER)) {
		free(param->header);
		param->header = 0;
	}
	return ret;
}

static int fd;
static char fileheader[512];
static int filelen;

static int ReadData(void* buffer, int bufsize)
{
	if (fileheader[0]) {
		int ret = snprintf((char*)buffer, bufsize, "%s", fileheader);
		fileheader[0] = 0;
		return ret;
	} else {
		return read(fd, buffer, bufsize);
	}
}

#define FILE_CHUNK_HEADER "Content-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n\r\n"

int httpPostFile(HTTP_REQUEST* req, char* url, char* fieldname, const char* filename)
{
	int ret;
	POST_CHUNK chunk;
	char *p;

	httpInitReq(req, 0);

	fd = open(filename, O_BINARY | O_RDONLY);
	if (fd <= 0) return -1;

	p = (char*)strrchr(filename, '\\');
	if (!p) p = (char*)strrchr(filename, '/');
	if (!p)
		req->filename = filename;
	else
		req->filename = p + 1;

	filelen = lseek(fd, 0, SEEK_END);
	chunk.data = (void*)ReadData;
	chunk.length = filelen + snprintf(fileheader, sizeof(fileheader), FILE_CHUNK_HEADER, fieldname, req->filename);
	chunk.type = postPayload_CALLBACK;
	lseek(fd, 0, SEEK_SET );
	req->method = HM_POST_MULTIPART;
	req->chunk = &chunk;
	req->chunkCount = 1;
	ret = httpRequest(req, url);
	close(fd);
	if (!ret) {
		ret = httpGetResponse(req);
	}
	return 0;
}

int PostFileStream(char* url, const char* filename)
{
	int ret;
	HTTP_REQUEST req;
	
	char buf[1024];
	int bytes;

	memset(&req, 0, sizeof(req));

	fd = open(filename, O_BINARY | O_RDONLY);
	if (fd <= 0) return 0;

	req.filename = strrchr(filename, '\\');
	if (!req.filename)
		req.filename = filename;
	else
		req.filename++;

	req.method = HM_POST_STREAM;
	ret = httpRequest(&req, url);

	while ((bytes = read(fd, buf, sizeof(buf))) > 0 
		&& httpSend(&req, buf, bytes) == bytes);

	close(fd);
	return ret;
}

#ifdef HTTPAUTH  
extern ServerThread g_httpParam;
////////////////////////////////////////////////////////////////////////////
// _mwCheckAuthentication
// Check if a connected peer is authenticated
////////////////////////////////////////////////////////////////////////////
BOOL _mwCheckAuthentication(HttpSocket* phsSocket)
{
	if (!ISFLAGSET(phsSocket,FLAG_AUTHENTICATION))
		return TRUE;
	if (g_httpParam.dwAuthenticatedNode!=phsSocket->ipAddr.laddr) {
		// Not authenticated
		g_httpParam.stats.authFailCount++;
		return FALSE;
	} 
	// Extend authentication period
	g_httpParam.tmAuthExpireTime = time(NULL) + HTTPAUTHTIMEOUT;
	return TRUE;
}
#endif


////////////////////////////////////////////////////////////////////////////
// global variables
////////////////////////////////////////////////////////////////////////////
// default pages

const char* contentTypeTable[]={
	"application/octet-stream",
	"text/html",
	"text/xml",
	"text/plain",
	"application/vnd.mozilla.xul+xml",
	"text/css",
	"application/x-javascript",
	"image/png",
	"image/jpeg",
	"image/gif",
	"application/x-shockwave-flash",
	"audio/mpeg",
	"video/mpeg",
	"video/avi",
	"video/mp4",
	"video/quicktime",
	"video/x-mpeg-avc",
	"video/flv",
	"video/MP2T",
	"video/3gpp",
	"video/x-ms-asf",
	"application/octet-stream",
	"application/x-datastream",
	"application/x-mpegURL",
};

char* defaultPages[]={"index.htm","index.html","default.htm","main.xul"};

FILE *fpLog = NULL;

////////////////////////////////////////////////////////////////////////////
// API callsc
////////////////////////////////////////////////////////////////////////////

const char *dayNames="Sun\0Mon\0Tue\0Wed\0Thu\0Fri\0Sat";
const char *monthNames="Jan\0Feb\0Mar\0Apr\0May\0Jun\0Jul\0Aug\0Sep\0Oct\0Nov\0Dec";
const char *httpDateTimeFormat="%s, %02d %s %d %02d:%02d:%02d GMT";

char* mwGetVarValue(HttpVariables* vars, const char *varname, int index)
{
	int i;
	int count = 0;
	if (vars && varname) {
		for (i=0; (vars+i)->name; i++) {
			if (!strcmp((vars+i)->name,varname)) {
				if (index == count++) return (vars+i)->value;
			}
		}
	}
	return NULL;
}

int mwGetVarValueInt(HttpVariables* vars, const char *varname, int defval)
{
	int i;
	if (vars && varname) {
		for (i=0; (vars+i)->name; i++) {
			if (!strcmp((vars+i)->name,varname)) {
				char *p = (vars+i)->value;
				return p ? atoi(p) : defval;
			}
		}
	}
	return defval;
}

int mwGetHttpDateTime(time_t timer, char *buf, int bufsize)
{
	struct tm *btm;
	btm=gmtime(&timer);
	return snprintf(buf, bufsize, httpDateTimeFormat,
		dayNames+(btm->tm_wday<<2),
		btm->tm_mday,
		monthNames+(btm->tm_mon<<2),
		1900+btm->tm_year,
		btm->tm_hour,
		btm->tm_min,
		btm->tm_sec);
}

////////////////////////////////////////////////////////////////////////////
// mwServerStart
// Start the webserver
////////////////////////////////////////////////////////////////////////////
int mwServerStart(ServerThread* hp)
{
	if (hp->bWebserverRunning) {
		DBG("Error: Webserver thread already running\n");
		return -1;
	}
	if (!fpLog) fpLog=stderr;
	if (!InitSocket()) {
		DBG("Error initializing Winsock\n");
		return -1;
	}

	{
		int i;
		for (i=0;(hp->pxUrlHandler+i)->pchUrlPrefix;i++) {
			if ((hp->pxUrlHandler+i)->pfnEventHandler &&
				(hp->pxUrlHandler+i)->pfnEventHandler(MW_INIT, 0) ) {
					//remove the URL handler
					(hp->pxUrlHandler+i)->pfnUrlHandler=NULL;
			}
		}
	}

#ifdef _7Z
	hp->szctx = SzInit();
#endif

	if (!(hp->listenSocket=_mwStartListening(hp))) {
		DBG("Error listening on port %d\n", hp->httpPort);
		return -1;
	}

	hp->stats.startTime=time(NULL);
	hp->bKillWebserver=FALSE;
	hp->bWebserverRunning=TRUE;
	if (!hp->tmSocketExpireTime) hp->tmSocketExpireTime = HTTP_EXPIRATION_TIME;

#ifndef NOTHREAD
	if (ThreadCreate(&hp->tidHttpThread, (void*(*)(void*))_mwHttpThread,(void*)hp)) {
		DBG("Error creating server thread\n");
		return -1;
	}
#else
	_mwHttpThread((void*)hp);
#endif

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// mwServerShutdown
// Shutdown the webserver
////////////////////////////////////////////////////////////////////////////
int mwServerShutdown(ServerThread* hp)
{
	int i;

	DBG("Shutting down web server thread\n");
	// signal webserver thread to quit
	hp->bKillWebserver=TRUE;

	for (i=0;(hp->pxUrlHandler+i)->pchUrlPrefix;i++) {
		if ((hp->pxUrlHandler+i)->pfnUrlHandler && (hp->pxUrlHandler+i)->pfnEventHandler)
			(hp->pxUrlHandler+i)->pfnEventHandler(MW_UNINIT, 0);
	}

	if (hp->listenSocket) closesocket(hp->listenSocket);

	// and wait for thread to exit
	for (i=0;hp->bWebserverRunning && i<30;i++) msleep(100);

#ifdef _7Z
	SzUninit(hp->szctx);
#endif

	DBG("Webserver shutdown complete\n");
	return 0;
}

int mwGetLocalFileName(HttpFilePath* hfp)
{
	char ch;
	char *p=hfp->cFilePath,*s=hfp->pchHttpPath,*upLevel=NULL;

	hfp->pchExt=NULL;
	hfp->fTailSlash=0;
	if (*s == '~') {
		s++;
	} else if (hfp->pchRootPath) {
		p+=_mwStrCopy(hfp->cFilePath,hfp->pchRootPath);
		if (*(p-1)!=SLASH) {
			*p=SLASH;
			*(++p)=0;
		}
	}
	while ((ch=*s) && ch!='?' && (int)(p-hfp->cFilePath)<sizeof(hfp->cFilePath)-1) {
		if (ch=='%') {
			*(p++) = _mwDecodeCharacter(++s);
			s += 2;
		} else if (ch=='/') {
			*p=SLASH;
			upLevel=(++p);
			while (*(++s)=='/');
			continue;
		} else if (ch=='+') {
			*(p++)=' ';
			s++;
		} else if (ch=='.') {
			if (upLevel && GETWORD(s+1)==DEFWORD('.','/')) {
				s+=2;
				p=upLevel;
			} else {
				*(p++)='.';
				hfp->pchExt=p;
				while (*(++s)=='.');	//avoid '..' appearing in filename for security issue
			}
		} else {
			*(p++)=*(s++);
		}
	}
	if (*(p-1)==SLASH) {
		p--;
		hfp->fTailSlash=1;
	}
	*p=0;
	return (int)(p - hfp->cFilePath);
}

////////////////////////////////////////////////////////////////////////////
// Internal (private) helper functions
////////////////////////////////////////////////////////////////////////////

SOCKET _mwStartListening(ServerThread* hp)
{
	SOCKET listenSocket;
	int iRc;

	// create a new socket
	listenSocket=socket(AF_INET,SOCK_STREAM,0);
	if (listenSocket <= 0) {
		DBG("Error creating socket\n");
		return 0;
	}

#if comment
	// allow reuse of port number
	{
		int iOptVal=1;
		iRc=setsockopt(listenSocket,SOL_SOCKET,SO_REUSEADDR,
			(char*)&iOptVal,sizeof(iOptVal));
		if (iRc<0) return 0;
	}
#endif

	// bind it to the http port
	{
		struct sockaddr_in sinAddress;
		memset(&sinAddress,0,sizeof(struct sockaddr_in));
		sinAddress.sin_family=AF_INET;
		// INADDR_ANY is 0
		//sinAddress.sin_addr.s_addr=htonl(hp->dwBindIP);
		sinAddress.sin_addr.s_addr = hp->hlBindIP;
		sinAddress.sin_port = htons(hp->httpPort); // http port
		iRc=bind(listenSocket,(struct sockaddr*)&sinAddress,
			sizeof(struct sockaddr_in));
		if (iRc<0) {
			DBG("Error binding on port %d\n",hp->httpPort);
			return 0;
		}
	}

#ifndef WIN32
	// set to non-blocking to avoid lockout issue (see Section 15.6
	// in Unix network programming book)
	{
		int iSockFlags;
		iSockFlags = fcntl(listenSocket, F_GETFL, 0);
		iSockFlags |= O_NONBLOCK;
		iRc = fcntl(listenSocket, F_SETFL, iSockFlags);
	}
#endif

	// listen on the socket for incoming calls
	if (listen(listenSocket,hp->maxClients-1)) {
		DBG("Unable to listen on socket %d\n",listenSocket);
		return 0;
	}

	DBG("Http listen socket %d opened\n",listenSocket);
	return listenSocket;
}

void _mwInitSocketData(HttpSocket *phsSocket)
{
	memset(&phsSocket->response,0,sizeof(HttpResponse));
	phsSocket->request.startByte = 0;
	phsSocket->request.ofHost = 0;
	phsSocket->request.pucPath = 0;
	phsSocket->request.headerSize = 0;
	phsSocket->fd = 0;
	phsSocket->flags = FLAG_RECEIVING;
	phsSocket->pucData = phsSocket->buffer;
	phsSocket->dataLength = 0;
	phsSocket->bufferSize = HTTP_BUFFER_SIZE;
	phsSocket->ptr = NULL;
	phsSocket->handler = NULL;
	phsSocket->pxMP = NULL;
	phsSocket->mimeType = NULL;
}

////////////////////////////////////////////////////////////////////////////
// _mwHttpThread
// Webserver independant processing thread. Handles all connections
////////////////////////////////////////////////////////////////////////////
void* _mwHttpThread(ServerThread *hp)
{ 
	HttpSocket *phsSocketCur;
	SOCKET socket;
	struct sockaddr_in sinaddr;
	int iRc;

	// main processing loop
	while (!hp->bKillWebserver) {
		time_t tmCurrentTime;
		SOCKET iSelectMaxFds;
		fd_set fdsSelectRead;
		fd_set fdsSelectWrite;

		// clear descriptor sets
		FD_ZERO(&fdsSelectRead);
		FD_ZERO(&fdsSelectWrite);
		FD_SET(hp->listenSocket,&fdsSelectRead);
		iSelectMaxFds=hp->listenSocket;

		// get current time
		tmCurrentTime=time(NULL);  
		// build descriptor sets and close timed out sockets
		for (phsSocketCur=hp->phsSocketHead; phsSocketCur; phsSocketCur=phsSocketCur->next) {
			int iError=0;
			int iOptSize=sizeof(int);
			// get socket fd
			socket=phsSocketCur->socket;
			if (!socket) continue;
			if (getsockopt(socket,SOL_SOCKET,SO_ERROR,(char*)&iError,&iOptSize)) {
				// if a socket contains a error, close it
				SYSLOG(fpLog,"[%d] Socket no longer vaild.\n",socket);
				phsSocketCur->flags=FLAG_CONN_CLOSE;
				_mwCloseSocket(hp, phsSocketCur);
				continue;
			}
			// check expiration timer (for non-listening, in-use sockets)
			if (tmCurrentTime > phsSocketCur->tmExpirationTime) {
				SYSLOG(fpLog,"[%d] Http socket expired\n",phsSocketCur->socket);
				hp->stats.timeOutCount++;
				// close connection
				phsSocketCur->flags=FLAG_CONN_CLOSE;
				_mwCloseSocket(hp, phsSocketCur);
			} else {
				if (ISFLAGSET(phsSocketCur,FLAG_RECEIVING)) {
					// add to read descriptor set
					FD_SET(socket,&fdsSelectRead);
				}
				if (ISFLAGSET(phsSocketCur,FLAG_SENDING)) {
					// add to write descriptor set
					FD_SET(socket,&fdsSelectWrite);
				}
				// check if new max socket
				if (socket>iSelectMaxFds) {
					iSelectMaxFds=socket;
				}
			}
		}

		{
			struct timeval tvSelectWait;
			// initialize select delay
			tvSelectWait.tv_sec = 1;
			tvSelectWait.tv_usec = 0; // note: using timeval here -> usec not nsec

			// and check sockets (may take a while!)
			iRc=select(iSelectMaxFds+1, &fdsSelectRead, &fdsSelectWrite,
				NULL, &tvSelectWait);
		}
		if (iRc<0) {
			if (hp->bKillWebserver) break;
			DBG("Select error\n");
			msleep(1000);
			continue;
		}
		if (iRc>0) {
			HttpSocket *phsSocketNext;
			// check which sockets are read/write able
			phsSocketCur=hp->phsSocketHead;
			while (phsSocketCur) {
				BOOL bRead;
				BOOL bWrite;

				phsSocketNext=phsSocketCur->next;
				// get socket fd
				socket=phsSocketCur->socket;

				// get read/write status for socket
				bRead=FD_ISSET(socket, &fdsSelectRead);
				bWrite=FD_ISSET(socket, &fdsSelectWrite);

				if ((bRead|bWrite)!=0) {
					//DBG("socket %d bWrite=%d, bRead=%d\n",phsSocketCur->socket,bWrite,bRead);
					// if readable or writeable then process
					if (bWrite && ISFLAGSET(phsSocketCur,FLAG_SENDING)) {
						iRc=_mwProcessWriteSocket(hp, phsSocketCur);
					} else if (bRead && ISFLAGSET(phsSocketCur,FLAG_RECEIVING)) {
						iRc=_mwProcessReadSocket(hp,phsSocketCur);
						if (ISFLAGSET(phsSocketCur, FLAG_DATA_SOCKET)) {
							_mwRemoveSocket(hp, phsSocketCur);
						}
					} else {
						iRc=-1;
						DBG("Invalid socket state (flag: %08x)\n",phsSocketCur->flags);
						SETFLAG(phsSocketCur,FLAG_CONN_CLOSE);
					}
					if (!iRc) {
						// and reset expiration timer
						phsSocketCur->tmExpirationTime=time(NULL)+hp->tmSocketExpireTime;
					} else {
						_mwCloseSocket(hp, phsSocketCur);
					}
				}
				phsSocketCur=phsSocketNext;
			}

			// check if any socket to accept and accept the socket
			if (FD_ISSET(hp->listenSocket, &fdsSelectRead)) {
				if (hp->stats.clientCount > hp->maxClients) {
					DBG("WARNING: clientCount:%d > maxClients:%d\n", hp->stats.clientCount, hp->maxClients);
					msleep(200);
				}

				socket = _mwAcceptSocket(hp,&sinaddr);
				if (socket == 0) continue;

				// create a new socket structure and insert it in the linked list
				phsSocketCur=(HttpSocket*)malloc(sizeof(HttpSocket));
				if (!phsSocketCur) {
					DBG("Out of memory\n");
					break;
				}
				phsSocketCur->next=hp->phsSocketHead;
				hp->phsSocketHead=phsSocketCur;	//set new header of the list
				//fill structure with data
				_mwInitSocketData(phsSocketCur);
				phsSocketCur->request.pucPayload = 0;
				phsSocketCur->tmAcceptTime=time(NULL);
				phsSocketCur->socket=socket;
				phsSocketCur->tmExpirationTime=time(NULL)+hp->tmSocketExpireTime;
				phsSocketCur->iRequestCount=0;
				phsSocketCur->ipAddr.laddr=ntohl(sinaddr.sin_addr.s_addr);
				SYSLOG(fpLog,"[%d] IP: %d.%d.%d.%d\n",
					phsSocketCur->socket,
					phsSocketCur->ipAddr.caddr[3],
					phsSocketCur->ipAddr.caddr[2],
					phsSocketCur->ipAddr.caddr[1],
					phsSocketCur->ipAddr.caddr[0]);
				hp->stats.clientCount++;
				SYSLOG(fpLog,"Connected clients: %d\n",hp->stats.clientCount);
				//update max client count
				if (hp->stats.clientCount>hp->stats.clientCountMax) hp->stats.clientCountMax=hp->stats.clientCount;
			}
		} else {
			HttpSocket *phsSocketPrev=NULL;
			// select timeout
			// clean up the link list
			phsSocketCur=hp->phsSocketHead;
			while (phsSocketCur) {
				if (!phsSocketCur->socket) {
					DBG("Free up socket structure 0x%08x\n",phsSocketCur);
					if (phsSocketPrev) {
						phsSocketPrev->next=phsSocketCur->next;
						free(phsSocketCur);
						phsSocketCur=phsSocketPrev->next;
					} else {
						hp->phsSocketHead=phsSocketCur->next;
						free(phsSocketCur);
						phsSocketCur=hp->phsSocketHead;
					}
				} else {
					phsSocketPrev=phsSocketCur;
					phsSocketCur=phsSocketCur->next;
				}
			}
		}
	}

	for (phsSocketCur=hp->phsSocketHead; phsSocketCur;) {
		HttpSocket *phsSocketNext;
		phsSocketCur->flags|=FLAG_CONN_CLOSE;
		_mwCloseSocket(hp, phsSocketCur);
		phsSocketNext=phsSocketCur->next;
		free(phsSocketCur);
		phsSocketCur=phsSocketNext;
	}
	hp->phsSocketHead = 0;

	// clear state vars
	hp->bKillWebserver=FALSE;
	hp->bWebserverRunning=FALSE;

	return NULL;
} // end of _mwHttpThread

int _mwRemoveSocket(ServerThread* hp, HttpSocket* hs)
{
	HttpSocket* phsSocketCur=hp->phsSocketHead;
	if (phsSocketCur == hs) {
		hp->phsSocketHead = phsSocketCur->next;
		return 0;
	}
	while (phsSocketCur) {
		if (phsSocketCur->next == hs) {
			phsSocketCur->next = phsSocketCur->next->next;
			return 0;
		}
		phsSocketCur = phsSocketCur->next;
	}
	return -1;
}

////////////////////////////////////////////////////////////////////////////
// _mwAcceptSocket
// Accept an incoming connection
////////////////////////////////////////////////////////////////////////////
SOCKET _mwAcceptSocket(ServerThread* hp,struct sockaddr_in *sinaddr)
{
	SOCKET socket;
	int namelen=sizeof(struct sockaddr);

	socket=accept(hp->listenSocket, (struct sockaddr*)sinaddr,&namelen);

	SYSLOG(fpLog,"[%d] connection accepted @ %s\n",socket,GetTimeString());

	if ((int)socket<=0) {
		DBG("Error accepting socket\n");
		return 0;
	} 

#ifndef WIN32
	// set to non-blocking to stop sends from locking up thread
	{
		int iRc;
		int iSockFlags;
		iSockFlags = fcntl(socket, F_GETFL, 0);
		iSockFlags |= O_NONBLOCK;
		iRc = fcntl(socket, F_SETFL, iSockFlags);
	}
#endif

	if (hp->socketRcvBufSize) {
		int iSocketBufSize=hp->socketRcvBufSize<<10;
		setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (const char*)&iSocketBufSize, sizeof(int));
	}

	return socket;
} // end of _mwAcceptSocket

int _mwBuildHttpHeader(HttpSocket *phsSocket, time_t contentDateTime, unsigned char* buffer)
{
	unsigned char *p=buffer;
	p+=snprintf((char*)p, 512, HTTP200_HEADER,
		(phsSocket->request.startByte==0)?"200 OK":"206 Partial content",
		HTTP_SERVER_NAME,
		HTTP_KEEPALIVE_TIME,
		MAX_CONN_REQUESTS,
		ISFLAGSET(phsSocket,FLAG_CONN_CLOSE)?"close":"Keep-Alive");
	if (contentDateTime) {
		p += snprintf((char*)p, 512, "Last-Modified: ");
		p+=mwGetHttpDateTime(contentDateTime, (char*)p, 512);
		SETWORD(p,DEFWORD('\r','\n'));
		p+=2;
	}
	p+=snprintf((char*)p, 512, "Content-Type: %s\r\n", phsSocket->mimeType ? phsSocket->mimeType : contentTypeTable[phsSocket->response.fileType]);
	if (phsSocket->response.contentLength > 0 && !(phsSocket->flags & FLAG_CHUNK)) {
		p+=snprintf((char*)p, 512,"Content-Length: %d\r\n", phsSocket->response.contentLength);
	}
	if (phsSocket->flags & FLAG_CHUNK) {
		p += sprintf((char*)p, "Transfer-Encoding: chunked\r\n");
	}
	SETDWORD(p,DEFDWORD('\r','\n',0,0));
	return (int)(p- buffer + 2);
}

int mwParseQueryString(UrlHandlerParam* up)
{
	if (up->iVarCount==-1) {
		//parsing variables from query string
		char *p,*s;
		// get start of query string
		s = strchr(up->pucRequest, '?');
		if (s) {
			*(s++) = 0;
		} else if (ISFLAGSET(up->hs,FLAG_REQUEST_POST)){
			s = (char*)up->hs->request.pucPayload;
		}
		if (s && *s && strncmp(s, "<?xml", 5)) {
			int i;
			int n = 1;
			//get number of variables
			for (p = s; *p ; ) if (*(p++)=='&') n++;
			up->pxVars = (HttpVariables*)calloc(n + 1, sizeof(HttpVariables));
			up->iVarCount = n;
			//store variable name and value
			for (i = 0, p = s; i < n; p++) {
				switch (*p) {
				case '=':
					if (!(up->pxVars + i)->name) {
						*p = 0;
						(up->pxVars + i)->name = s;
						s=p+1;
					}
					break;
				case 0:
				case '&':
					*p = 0;
					if ((up->pxVars + i)->name) {
						(up->pxVars + i)->value = s;
						mwDecodeString(s);
					} else {
						(up->pxVars + i)->name = s;
						(up->pxVars + i)->value = p;
					}
					s = p + 1;
					i++;
					break;
				}
			}
			(up->pxVars + n)->name = NULL;
		}
	}
	return up->iVarCount;
}

#ifndef DISABLE_BASIC_WWWAUTH
////////////////////////////////////////////////////////////////////////////
// _mwBase64Encode
// buffer size of out_str is (in_len * 4 / 3 + 1)
////////////////////////////////////////////////////////////////////////////
void _mwBase64Encode(const char *in_str, int in_len, char *out_str)
{ 
	const char base64[] ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int curr_out_len = 0;
	int i = 0;
	char a, b, c;

	out_str[0] = '\0';

	if (in_len <= 0) return;

	while (i < in_len) {
		a = in_str[i];
		b = (i + 1 >= in_len) ? 0 : in_str[i + 1];
		c = (i + 2 >= in_len) ? 0 : in_str[i + 2];
		if (i + 2 < in_len) {
			out_str[curr_out_len++] = (base64[(a >> 2) & 0x3F]);
			out_str[curr_out_len++] = (base64[((a << 4) & 0x30) + ((b >> 4) & 0xf)]);
			out_str[curr_out_len++] = (base64[((b << 2) & 0x3c) + ((c >> 6) & 0x3)]);
			out_str[curr_out_len++] = (base64[c & 0x3F]);
		}
		else if (i + 1 < in_len) {
			out_str[curr_out_len++] = (base64[(a >> 2) & 0x3F]);
			out_str[curr_out_len++] = (base64[((a << 4) & 0x30) + ((b >> 4) & 0xf)]);
			out_str[curr_out_len++] = (base64[((b << 2) & 0x3c) + ((c >> 6) & 0x3)]);
			out_str[curr_out_len++] = '=';
		}
		else {
			out_str[curr_out_len++] = (base64[(a >> 2) & 0x3F]);
			out_str[curr_out_len++] = (base64[((a << 4) & 0x30) + ((b >> 4) & 0xf)]);
			out_str[curr_out_len++] = '=';
			out_str[curr_out_len++] = '=';
		}
		i += 3;
	}

	out_str[curr_out_len] = '\0';
}

char *_mwGetBaisAuthorization(const char* username, const char* password)
{
	const char prefix[] = "Authorization: Basic ";
	int len = strlen(username) + 1 + strlen(password);
	char *tmp = (char*)malloc(len + 1);
	char *out = (char*)malloc(sizeof(prefix) + (len * 4 / 3 + 1) + 2);
	char *p = out + sizeof(prefix) - 1;

	sprintf(tmp, "%s:%s", username, password);
	strcpy(out, prefix);
	_mwBase64Encode(tmp, len, p);
	p += strlen(p);
	p[0] = '\r'; p[1] = '\n'; p[2] = '\0';

	free(tmp);

	return out;
}

////////////////////////////////////////////////////////////////////////////
// _mwBasicAuthorizationHandlers
// Basic Authorization implement
// RETURN VALUE: 0 (OK), -1(failed), 1(Authorization needed)
////////////////////////////////////////////////////////////////////////////
int _mwBasicAuthorizationHandlers(ServerThread* hp, HttpSocket* phsSocket)
{
	AuthHandler* pah;

	for (pah=hp->pxAuthHandler; pah && pah->pchUrlPrefix; pah++) {
		if (pah->pchUsername == NULL || *pah->pchUsername == '\0' ||
			pah->pchPassword == NULL || *pah->pchPassword == '\0') continue;
		if (pah->pchAuthString == NULL) pah->pchAuthString = 
			_mwGetBaisAuthorization(pah->pchUsername, pah->pchPassword);
		//TODO here!
	}

	return 0;
}
#endif

int _mwCheckUrlHandlers(ServerThread* hp, HttpSocket* phsSocket)
{
	UrlHandler* puh;
	UrlHandlerParam up;
	int ret=0;

	up.pxVars=NULL;
	for (puh=hp->pxUrlHandler; puh && puh->pchUrlPrefix; puh++) {
		size_t prefixLen=strlen(puh->pchUrlPrefix);
		if (puh->pfnUrlHandler && !strncmp((char*)phsSocket->request.pucPath,puh->pchUrlPrefix,prefixLen)) {
			//URL prefix matches
			memset(&up, 0, sizeof(up));
			up.hp=hp;
			up.p_sys = puh->p_sys;
			up.hs = phsSocket;
			up.dataBytes=phsSocket->bufferSize;
			up.pucRequest=(char*)phsSocket->request.pucPath+prefixLen;
			up.pucHeader=(char*)phsSocket->buffer;
			up.pucBuffer=(char*)phsSocket->pucData;
			up.pucBuffer[0]=0;
			up.iVarCount=-1;
			ret=(*puh->pfnUrlHandler)(&up);
			if (!ret) continue;
			phsSocket->flags|=ret;
			if (ret & FLAG_DATA_REDIRECT) {
				_mwRedirect(phsSocket, up.pucBuffer);
				DBG("URL handler: redirect to %s\n", up.pucBuffer);
			} else {
				phsSocket->response.fileType=up.fileType;
				hp->stats.urlProcessCount++;
				phsSocket->handler = puh;
				if (ret & FLAG_DATA_RAW) {
					SETFLAG(phsSocket, FLAG_DATA_RAW);
					phsSocket->pucData=(uchar*)up.pucBuffer;
					phsSocket->dataLength=up.dataBytes;
					phsSocket->response.contentLength=up.contentBytes>0?up.contentBytes:up.dataBytes;
					if (ret & FLAG_TO_FREE) {
						phsSocket->ptr=up.pucBuffer;	//keep the pointer which will be used to free memory later
					}
					DBG("URL handler: raw data\n");
				} else if (ret & FLAG_DATA_STREAM) {
					SETFLAG(phsSocket, FLAG_DATA_STREAM);
					phsSocket->pucData = (uchar*)up.pucBuffer;
					phsSocket->dataLength = up.dataBytes;
					phsSocket->response.contentLength = up.dataBytes;
					DBG("URL handler: stream\n");
				} else if (ret & FLAG_DATA_FILE) {
					SETFLAG(phsSocket, FLAG_DATA_FILE);
					if (up.pucBuffer[0]) {
						free(phsSocket->request.pucPath);
						phsSocket->request.pucPath=(uchar*)strdup(up.pucBuffer);
					}
					DBG("URL handler: file\n");
				} else if (ret & FLAG_DATA_FD) {
					SETFLAG(phsSocket, FLAG_DATA_FILE);
					DBG("URL handler: file descriptor\n");
				} else if (ret & FLAG_DATA_SOCKET) {
					SETFLAG(phsSocket, FLAG_DATA_SOCKET);
					DBG("URL handler: socket\n");
				}
				break;
			}
		}
	}
	if (up.pxVars) free(up.pxVars);
	return ret;
}

////////////////////////////////////////////////////////////////////////////
// _mwProcessReadSocket
// Process a socket (read)
////////////////////////////////////////////////////////////////////////////
int _mwProcessReadSocket(ServerThread* hp, HttpSocket* phsSocket)
{
#ifndef _NO_POST
	if (ISFLAGSET(phsSocket,FLAG_REQUEST_POST)) {
		if (phsSocket->pxMP && phsSocket->pxMP->pchBoundaryValue[0]) {
			// multipart and has valid boundary string
			int ret = _mwProcessMultipartPost(hp, phsSocket, FALSE);
			if (!ret) return 0;
			if (ret < 0) {
				SETFLAG(phsSocket, FLAG_CONN_CLOSE);
				return -1;
			}
			goto done;
		}
	}
#endif

	// read next chunk of data
	{
		int iLength = recv(phsSocket->socket, 
			(char*)phsSocket->pucData+phsSocket->dataLength,
			(int)(phsSocket->bufferSize - phsSocket->dataLength - 1), 0);
		int i;
		printf("_______________________________________________________\n");
		for( i = 0; i < iLength; i++ ){
			printf("%c", (phsSocket->pucData+phsSocket->dataLength)[i] );
		}
		printf("\n_______________________________________________________\n");
		if (iLength <= 0) {
			SYSLOG(fpLog,"[%d] socket closed by client\n",phsSocket->socket);
			SETFLAG(phsSocket, FLAG_CONN_CLOSE);

			if (ISFLAGSET(phsSocket,FLAG_REQUEST_POST) && phsSocket->pxMP) {
				HttpMultipart* pxMP = phsSocket->pxMP;
				if (!pxMP->pchBoundaryValue[0]) {
					// no boundary, commit remaining data
					pxMP->oFileuploadStatus = HTTPUPLOAD_LASTCHUNK;
					(hp->pfnFileUpload)(phsSocket->pxMP, phsSocket->pucData, (int)phsSocket->dataLength);
				}
				_mwCloseSocket(hp, phsSocket);
				return 0;
			}
			_mwCloseSocket(hp, phsSocket);
			return -1;
		}
		// add in new data received
		phsSocket->dataLength += iLength;
		phsSocket->pucData[phsSocket->dataLength] = 0;
	}

	// check if end of request
	if (phsSocket->request.headerSize==0) {
		int i=0;
		char *path = 0;

		while (GETDWORD(phsSocket->buffer + i) != HTTP_HEADEREND) {
			if (++i > phsSocket->dataLength - 3) return 0;
		}
		// reach the end of the header
		//check request type
		if (!memcmp(phsSocket->buffer, "GET", 3)) {
			SETFLAG(phsSocket,FLAG_REQUEST_GET);
			path = (char*)phsSocket->pucData + 5;
#ifndef _NO_POST
		} else if (!memcmp(phsSocket->buffer, "POST", 4)) {
			SETFLAG(phsSocket,FLAG_REQUEST_POST);
			path = (char*)phsSocket->pucData + 6;
#endif
		} else {
			SYSLOG(fpLog,"[%d] Unsupported method\n",phsSocket->socket);		
			SETFLAG(phsSocket,FLAG_CONN_CLOSE);
			phsSocket->request.pucPath = 0;
			return -1;
		}

		phsSocket->request.headerSize = i + 4;
		DBG("[%d] header size: %d bytes\n",phsSocket->socket,phsSocket->request.headerSize);
		if (_mwParseHttpHeader(phsSocket)) {
			SYSLOG(fpLog,"Error parsing request\n");
			SETFLAG(phsSocket, FLAG_CONN_CLOSE);
			return -1;
		} else {
			// keep request path
			for (i = 0; i < MAX_REQUEST_PATH_LEN; i++) {
				if ((path[i] == ' ' && !strncmp(path + i + 1, "HTTP/", 5)) || path[i] == '\r') {
					break;
				}
			}
			if (i >= MAX_REQUEST_PATH_LEN) {
				SETFLAG(phsSocket, FLAG_CONN_CLOSE);
				return -1;
			}
			phsSocket->request.pucPath = (uchar*)malloc(i + 1);
			memcpy(phsSocket->request.pucPath, path, i);
			phsSocket->request.pucPath[i] = 0;
#ifndef _NO_POST
			if (ISFLAGSET(phsSocket,FLAG_REQUEST_POST)) {
				hp->stats.reqPostCount++;
				if (phsSocket->pxMP) {
					// is multipart request
					int ret;
					HttpMultipart *pxMP = phsSocket->pxMP;
					pxMP->pp.pchPath = (char*)phsSocket->request.pucPath;

					if (!pxMP->pchFilename) {
						// multipart POST
						phsSocket->dataLength -= (phsSocket->request.headerSize);
						memmove(phsSocket->buffer, phsSocket->buffer + phsSocket->request.headerSize, phsSocket->dataLength);
						phsSocket->pucData = phsSocket->buffer;
						phsSocket->request.headerSize = 0;
						pxMP->pp.httpParam = hp;
						pxMP->writeLocation = phsSocket->dataLength;
						ret = _mwProcessMultipartPost(hp, phsSocket, phsSocket->dataLength != 0);
						if (ret < 0) {
							SETFLAG(phsSocket, FLAG_CONN_CLOSE);
							return -1;
						} else if (ret > 0) {
							goto done;
						}
						return 0;
					} else {
						// direct POST with filename in Content-Type
						phsSocket->dataLength -= phsSocket->request.headerSize;
						pxMP->oFileuploadStatus = HTTPUPLOAD_FIRSTCHUNK;
						(hp->pfnFileUpload)(pxMP, phsSocket->buffer + phsSocket->request.headerSize, phsSocket->dataLength);
						pxMP->oFileuploadStatus = HTTPUPLOAD_MORECHUNKS;
						phsSocket->pucData = phsSocket->buffer;
						phsSocket->bufferSize = HTTP_BUFFER_SIZE;
						phsSocket->dataLength = 0;
					}
				} else {
					if (phsSocket->request.pucPayload) free(phsSocket->request.pucPayload);
					phsSocket->bufferSize = phsSocket->response.contentLength + 1;
					phsSocket->request.pucPayload = (uchar*)malloc(phsSocket->bufferSize);
					phsSocket->request.payloadSize = phsSocket->response.contentLength;
					phsSocket->request.pucPayload[phsSocket->request.payloadSize]=0;
					phsSocket->dataLength -= phsSocket->request.headerSize;
					memcpy(phsSocket->request.pucPayload, phsSocket->buffer + phsSocket->request.headerSize, phsSocket->dataLength);
					phsSocket->pucData = phsSocket->request.pucPayload;
				}
			}
#endif
		}
		// add header zero terminator
		phsSocket->buffer[phsSocket->request.headerSize]=0;
		DBG("%s",phsSocket->buffer);
	}
	if (ISFLAGSET(phsSocket,FLAG_REQUEST_POST) && phsSocket->pxMP) {
		if (!phsSocket->pxMP->pchBoundaryValue[0]) {
			// no boundary, simply receive raw data
			if (phsSocket->dataLength == phsSocket->bufferSize) {
				(hp->pfnFileUpload)(phsSocket->pxMP, phsSocket->pucData, phsSocket->dataLength);
				phsSocket->dataLength = 0;
			}
		}
		return 0;
	}
	if ( phsSocket->dataLength < phsSocket->response.contentLength ) {
		// there is more data
		return 0;
	}
done:

	if (phsSocket->request.headerSize) {
		phsSocket->pucData = phsSocket->buffer + phsSocket->request.headerSize + 4;
		phsSocket->bufferSize = HTTP_BUFFER_SIZE - phsSocket->request.headerSize - 4;
	} else {
		phsSocket->pucData = phsSocket->buffer;
		phsSocket->bufferSize = HTTP_BUFFER_SIZE;
	}

	SYSLOG(fpLog,"[%d] request path: /%s\n",phsSocket->socket,phsSocket->request.pucPath);
	hp->stats.reqCount++;
	if (ISFLAGSET(phsSocket,FLAG_REQUEST_GET|FLAG_REQUEST_POST)) {
		if (hp->pxUrlHandler) {
			if (!_mwCheckUrlHandlers(hp,phsSocket))
				SETFLAG(phsSocket,FLAG_DATA_FILE);
		}
		// set state to SENDING (actual sending will occur on next select)
		CLRFLAG(phsSocket,FLAG_RECEIVING)
			SETFLAG(phsSocket,FLAG_SENDING);
		hp->stats.reqGetCount++;
		if (phsSocket->request.iHttpVer == 0) {
			CLRFLAG(phsSocket, FLAG_CHUNK);
		}
		if (ISFLAGSET(phsSocket,FLAG_DATA_RAW | FLAG_DATA_STREAM)) {
			return _mwStartSendRawData(hp, phsSocket);
		} else if (ISFLAGSET(phsSocket,FLAG_DATA_FILE)) {
			// send requested page
			return _mwStartSendFile(hp,phsSocket);
		} else if (ISFLAGSET(phsSocket,FLAG_DATA_SOCKET | FLAG_DATA_REDIRECT)) {
			return 0;
		}
	}
	SYSLOG(fpLog,"Error occurred (might be a bug)\n");
	return -1;
} // end of _mwProcessReadSocket

////////////////////////////////////////////////////////////////////////////
// _mwProcessWriteSocket
// Process a socket (write)
////////////////////////////////////////////////////////////////////////////
int _mwProcessWriteSocket(ServerThread *hp, HttpSocket* phsSocket)
{
	if (ISFLAGSET(phsSocket,FLAG_DATA_REDIRECT)) {
		return 1;
	}
	if (phsSocket->dataLength<=0 && !ISFLAGSET(phsSocket,FLAG_DATA_STREAM)) {
		SYSLOG(fpLog,"[%d] Data sending completed (%d/%d)\n",phsSocket->socket,phsSocket->response.sentBytes,phsSocket->response.contentLength);
		return 1;
	}
	//SYSLOG(LOG_INFO,"[%d] sending data\n",phsSocket->socket);
	if (ISFLAGSET(phsSocket,FLAG_DATA_RAW|FLAG_DATA_STREAM)) {
		return _mwSendRawDataChunk(hp, phsSocket);
	} else if (ISFLAGSET(phsSocket,FLAG_DATA_FILE)) {
		return _mwSendFileChunk(hp, phsSocket);
	} else if (ISFLAGSET(phsSocket, FLAG_DATA_SOCKET)) {
		return 0;
	} else {
		SYSLOG(fpLog,"Invalid content source\n");
		return -1;
	}
} // end of _mwProcessWriteSocket

////////////////////////////////////////////////////////////////////////////
// _mwCloseSocket
// Close an open connection
////////////////////////////////////////////////////////////////////////////
void _mwCloseSocket(ServerThread* hp, HttpSocket* phsSocket)
{
	if (phsSocket->fd > 0) {
		close(phsSocket->fd);
	}
	phsSocket->fd = 0;
#ifndef _NO_POST
	if (phsSocket->pxMP) {
		int i;
		HttpMultipart *pxMP = phsSocket->pxMP;
		(hp->pfnFileUpload)(pxMP , 0, 0);
		// clear multipart structure
		for (i=0; i<pxMP->pp.iNumParams; i++) {
			free(pxMP->pp.stParams[i].pchParamName);
			free(pxMP->pp.stParams[i].pchParamValue);
		}
		//if (pxMP->pchFilename) free(pxMP->pchFilename);
		free(pxMP);
		phsSocket->pxMP = 0;
	}
	if (phsSocket->request.pucPayload) {
		free(phsSocket->request.pucPayload);
		phsSocket->request.pucPayload = 0;
	}
#endif
	if (ISFLAGSET(phsSocket,FLAG_DATA_STREAM) && phsSocket->handler) {
		UrlHandlerParam up;
		UrlHandler* pfnHandler = (UrlHandler*)phsSocket->handler;
		up.hs = phsSocket;
		up.hp = hp;
		up.pucBuffer = 0;	// indicate connection closed
		up.dataBytes = -1;
		(pfnHandler->pfnUrlHandler)(&up);
	}
	if (ISFLAGSET(phsSocket,FLAG_TO_FREE) && phsSocket->ptr) {
		free(phsSocket->ptr);
		phsSocket->ptr=NULL;
		CLRFLAG(phsSocket, FLAG_TO_FREE);
	}
	if (phsSocket->request.pucPath) {
		free(phsSocket->request.pucPath);
		phsSocket->request.pucPath = 0;
	}
	if (!ISFLAGSET(phsSocket,FLAG_CONN_CLOSE) && phsSocket->iRequestCount< MAX_CONN_REQUESTS) {
		_mwInitSocketData(phsSocket);
		//reset flag bits
		phsSocket->iRequestCount++;
		phsSocket->tmExpirationTime=time(NULL)+HTTP_KEEPALIVE_TIME;
		return;
	}
	if (phsSocket->socket != 0) {
		closesocket(phsSocket->socket);
		hp->stats.clientCount--;
		phsSocket->iRequestCount=0;
		SYSLOG(fpLog,"[%d] socket closed after responded for %d requests\n",phsSocket->socket,phsSocket->iRequestCount);
		SYSLOG(fpLog,"Connected clients: %d\n",hp->stats.clientCount);
		phsSocket->socket=0;
	} else {
		SYSLOG(fpLog,"[%d] [warning] socket=0 (structure: 0x%x) \n", phsSocket->socket, phsSocket);
	}
} // end of _mwCloseSocket

int _mwStrCopy(char *dest, const char *src)
{
	int i;
	for (i=0; src[i]; i++) {
		dest[i]=src[i];
	}
	dest[i]=0;
	return i;
}

int _mwStrHeadMatch(const char* buf1, const char* buf2) {
	unsigned int i;
	int x;
	for(i=0;buf2[i];i++) {
		if ((x=tolower(buf1[i])-tolower(buf2[i]))) return x;
	}
	return 0;
}

int _mwListDirectory(HttpSocket* phsSocket, char* dir)
{
	char cFileName[128];
	char cFilePath[MAX_PATH];
	char *p=(char*)phsSocket->pucData;
	int ret;
	char *pagebuf=(char*)phsSocket->pucData;
	size_t bufsize=phsSocket->bufferSize;

	p+=snprintf(p, 256, "<html><head><title>/%s</title></head><body><table border=0 cellpadding=0 cellspacing=0 width=100%%><h2>Directory of /%s</h2><hr>",
		phsSocket->request.pucPath,phsSocket->request.pucPath);
	if (!*dir) SETWORD(dir,DEFWORD('.',0));
	DBG("Listing directory: %s\n",dir);
	for (ret=ReadDir(dir,cFileName); !ret; ret=ReadDir(NULL,cFileName)) {
		struct stat st;
		char *s;
		size_t bytes;
		if (GETWORD(cFileName)==DEFWORD('.',0)) continue;
		DBG("Checking %s ...\n",cFileName);
		bytes=(int)(p-pagebuf);
		if (bytes+384>bufsize) {
			//need to expand buffer
			bufsize+=2048;
			if (!ISFLAGSET(phsSocket,FLAG_TO_FREE)) {
				//first time expanding
				SETFLAG(phsSocket,FLAG_TO_FREE);
				pagebuf=(char*)malloc(bufsize);
				memcpy(pagebuf,phsSocket->pucData,bytes);
			} else {
				pagebuf=(char*)realloc(pagebuf,bufsize);
			}
			p=pagebuf+bytes;
			DBG("Buffer expanded to %d bytes\n",bufsize);
		}
		snprintf(cFilePath, sizeof(cFilePath), "%s/%s",dir,cFileName);
		if (stat(cFilePath,&st)) continue;
		if (st.st_mode & S_IFDIR) {
			p+=snprintf(p, 256, "<tr><td width=35%%><a href='%s/'>%s</a></td><td width=15%%>&lt;dir&gt;</td><td width=15%%>",
				cFileName,cFileName);
		} else {
			p+=snprintf(p, 256, "<tr><td width=35%%><a href='%s'>%s</a></td><td width=15%%>%u bytes</td><td width=15%%>",
				cFileName,cFileName,st.st_size);
			s=strrchr(cFileName,'.');
			if (s) {
				int filetype=mwGetContentType(++s);
				if (filetype!=HTTPFILETYPE_OCTET)
					p+=_mwStrCopy(p,contentTypeTable[filetype]);
				else
					p+=snprintf(p, 256, "%s file",s);
			}
		}
		p+=_mwStrCopy(p,"</td><td>");
		p+=mwGetHttpDateTime(st.st_mtime, p, 512);
		p+=_mwStrCopy(p,"</td></tr>");
	}
	p+=snprintf(p, 256, "</table><hr><i>Directory content generated by %s</i></body></html>", HTTP_SERVER_NAME);
	ReadDir(NULL,NULL);
	phsSocket->response.contentLength=(phsSocket->dataLength=(int)(p-pagebuf));
	phsSocket->response.fileType=HTTPFILETYPE_HTML;
	if (ISFLAGSET(phsSocket,FLAG_TO_FREE)) {
		phsSocket->pucData=(uchar*)pagebuf;
		phsSocket->ptr=pagebuf;
	}
	return 0;
}

void _mwSend404Page(HttpSocket* phsSocket)
{
	char hdr[128];
	int hdrsize = snprintf(hdr, sizeof(hdr), HTTP404_HEADER, HTTP_SERVER_NAME, sizeof(HTTP404_BODY) - 1);
	SYSLOG(fpLog,"[%d] Http file not found\n",phsSocket->socket);
	// send 404 page
	send(phsSocket->socket, hdr, hdrsize, 0);
	send(phsSocket->socket, HTTP404_BODY, sizeof(HTTP404_BODY)-1, 0);
}

#ifdef WIN32
#define OPEN_FLAG O_RDONLY|0x8000
#else
#define OPEN_FLAG O_RDONLY
#endif

////////////////////////////////////////////////////////////////////////////
// _mwStartSendFile
// Setup for sending of a file
////////////////////////////////////////////////////////////////////////////
int _mwStartSendFile(ServerThread* hp, HttpSocket* phsSocket)
{
	struct stat st;
	HttpFilePath hfp;

	hfp.pchRootPath=hp->pchWebPath;
	// check type of file requested
	if (!ISFLAGSET(phsSocket, FLAG_DATA_FD)) {
		hfp.pchHttpPath=(char*)phsSocket->request.pucPath;
		mwGetLocalFileName(&hfp);
		if (stat(hfp.cFilePath,&st) < 0) {
#ifdef _7Z
			char* szfile = 0;
			char *p = strstr(hfp.cFilePath, ".7z/");
			if (p) {
				p += 3;
				*p = 0;
				szfile = p + 1;
			}
			if (szfile) {
				char* data;
				int len = SzExtractContent(hp->szctx, hfp.cFilePath, szfile, &data);
				if (len > 0) {
					p = strrchr(szfile, '.');
					SETFLAG(phsSocket,FLAG_DATA_RAW);
					phsSocket->pucData = (char*)malloc(len);
					memcpy(phsSocket->pucData, data, len);
					phsSocket->ptr = phsSocket->pucData;
					phsSocket->response.contentLength = len;
					phsSocket->response.fileType = p ? mwGetContentType(p + 1) : HTTPFILETYPE_OCTET;
					phsSocket->dataLength = len;
					return _mwStartSendRawData(hp, phsSocket); 
				}
			}
			if (p) *p = SLASH;
#endif

			// file/dir not found
			_mwSend404Page(phsSocket);
			return -1;
		}


		// open file
		if (!(st.st_mode & S_IFDIR))
			phsSocket->fd=open(hfp.cFilePath,OPEN_FLAG);
		else
			phsSocket->fd = -1;
	} else if ((phsSocket->flags & FLAG_DATA_FD) && phsSocket->fd > 0) {
		strcpy(hfp.cFilePath, (char*)phsSocket->request.pucPath);
		hfp.pchExt = strrchr(hfp.cFilePath, '.');
		if (hfp.pchExt) hfp.pchExt++;
		st.st_mtime = 0;
		st.st_size = 0;
		fstat(phsSocket->fd, &st);
	} else {
		_mwSend404Page(phsSocket);
		return -1;
	}

	if (phsSocket->fd < 0) {
		char *p;
		int i;

		if (!(st.st_mode & S_IFDIR)) {
			// file/dir not found
			_mwSend404Page(phsSocket);
			return -1;
		}

		DBG("Process Directory...\n");
		//requesting for directory, first try opening default pages
		for (p = hfp.cFilePath; *p; p++);
		*(p++)=SLASH;
		for (i=0; i<sizeof(defaultPages)/sizeof(defaultPages[0]); i++) {
			strcpy(p,defaultPages[i]);
			phsSocket->fd=open(hfp.cFilePath,OPEN_FLAG);
			if (phsSocket->fd > 0) {
				fstat(phsSocket->fd, &st);
				hfp.pchExt = strchr(defaultPages[i], '.') + 1;
				break;
			}
		}

		if (phsSocket->fd <= 0 && (hp->flags & FLAG_DIR_LISTING)) {
			SETFLAG(phsSocket,FLAG_DATA_RAW);
			if (!hfp.fTailSlash) {
				p=(char*)phsSocket->request.pucPath;
				while(*p) p++;				//seek to the end of the string
				SETWORD(p,DEFWORD('/',0));	//add a tailing slash
				while(--p>(char*)phsSocket->request.pucPath) {
					if (*p=='/') {
						p++;
						break;
					}
				}
				_mwRedirect(phsSocket,p);
			} else {
				*(p-1)=0;
				_mwListDirectory(phsSocket,hfp.cFilePath);
			}
			return _mwStartSendRawData(hp, phsSocket);
		}
	}
	if (phsSocket->fd > 0) {
		phsSocket->response.contentLength = (int)(st.st_size - phsSocket->request.startByte);
		if (phsSocket->response.contentLength <= 0) {
			phsSocket->request.startByte = 0;
			phsSocket->response.contentLength = st.st_size;
		}
		if (phsSocket->request.startByte) {
			lseek(phsSocket->fd, (long)phsSocket->request.startByte, SEEK_SET);
		}
		if (!phsSocket->response.fileType && hfp.pchExt) {
			phsSocket->response.fileType=(HttpFileType)mwGetContentType(hfp.pchExt);
		}
		// mark if substitution needed
		if (hp->pfnSubst && (phsSocket->response.fileType==HTTPFILETYPE_HTML ||phsSocket->response.fileType==HTTPFILETYPE_JS)) {
			SETFLAG(phsSocket,FLAG_SUBST);
		}
	} else {
		_mwSend404Page(phsSocket);
		return -1;
	}

	//SYSLOG(LOG_INFO,"File/requested size: %d/%d\n",st.st_size,phsSocket->response.contentLength);

	// build http header
	phsSocket->dataLength=_mwBuildHttpHeader(
		phsSocket,
		st.st_mtime,
		phsSocket->pucData);

	phsSocket->response.headerBytes = phsSocket->dataLength;
	phsSocket->response.sentBytes = 0;
	hp->stats.fileSentCount++;
	return 0;
} // end of _mwStartSendFile

////////////////////////////////////////////////////////////////////////////
// _mwSendFileChunk
// Send a chunk of a file
////////////////////////////////////////////////////////////////////////////
int _mwSendFileChunk(ServerThread *hp, HttpSocket* phsSocket)
{
	int iBytesWritten;
	int iBytesRead;

	if ((phsSocket->flags & FLAG_CHUNK) && ISFLAGSET(phsSocket, FLAG_HEADER_SENT)) {
		char buf[16];
		iBytesRead = snprintf(buf, sizeof(buf), "%X\r\n", phsSocket->dataLength);
		iBytesWritten = send(phsSocket->socket, buf, iBytesRead, 0);
	}
	// send a chunk of data
	iBytesWritten=send(phsSocket->socket, (char*)phsSocket->pucData,(int)phsSocket->dataLength, 0);
	if (iBytesWritten<=0) {
		// close connection
		DBG("[%d] error sending data\n", phsSocket->socket);
		SETFLAG(phsSocket,FLAG_CONN_CLOSE);
		close(phsSocket->fd);
		phsSocket->fd = 0;
		return -1;
	}
	SETFLAG(phsSocket, FLAG_HEADER_SENT);
	phsSocket->response.sentBytes+=iBytesWritten;
	phsSocket->pucData+=iBytesWritten;
	phsSocket->dataLength-=iBytesWritten;
	SYSLOG(fpLog,"[%d] %d bytes sent\n",phsSocket->socket,phsSocket->response.sentBytes);
	// if only partial data sent just return wait the remaining data to be sent next time
	if (phsSocket->dataLength>0)	return 0;

	// used all buffered data - load next chunk of file
	phsSocket->pucData=phsSocket->buffer;
	if (phsSocket->fd > 0)
		iBytesRead=read(phsSocket->fd,phsSocket->buffer,HTTP_BUFFER_SIZE);
	else
		iBytesRead=0;
	if (iBytesRead<=0) {
		// finished with a file
		int remainBytes = phsSocket->response.contentLength + phsSocket->response.headerBytes - phsSocket->response.sentBytes;
		DBG("[%d] EOF reached\n",phsSocket->socket);
		if (remainBytes > 0) {
			if (remainBytes>HTTP_BUFFER_SIZE) remainBytes=HTTP_BUFFER_SIZE;
			DBG("Sending %d padding bytes\n",remainBytes);
			memset(phsSocket->buffer,0,remainBytes);
			phsSocket->dataLength=remainBytes;
			return 0;
		} else {
			if (phsSocket->flags & FLAG_CHUNK) {
				send(phsSocket->socket, "0\r\n\r\n", 5, 0);
			}
			DBG("Closing file (fd=%d)\n",phsSocket->fd);
			hp->stats.fileSentBytes+=phsSocket->response.sentBytes;
			if (phsSocket->fd > 0) close(phsSocket->fd);
			phsSocket->fd = 0;
			return 1;
		}
	}
	if (ISFLAGSET(phsSocket,FLAG_SUBST)) {
		int iBytesUsed;
		// substituted file - read smaller chunk
		phsSocket->dataLength=_mwSubstVariables(hp, (char*)phsSocket->buffer,iBytesRead,&iBytesUsed);
		if (iBytesUsed<iBytesRead) {
			lseek(phsSocket->fd,iBytesUsed-iBytesRead,SEEK_CUR);
		}
	} else {
		phsSocket->dataLength=iBytesRead;
	}
	return 0;
} // end of _mwSendFileChunk

////////////////////////////////////////////////////////////////////////////
// _mwStartSendRawData
// Start sending raw data blocks
////////////////////////////////////////////////////////////////////////////
int _mwStartSendRawData(ServerThread *hp, HttpSocket* phsSocket)
{
	unsigned char header[HTTP200_HDR_EST_SIZE];
	int offset=0,hdrsize,bytes;
	hdrsize=_mwBuildHttpHeader(phsSocket,time(NULL),header);
	// send http header
	do {
		bytes=send(phsSocket->socket, (char*)header+offset,hdrsize-offset,0);
		if (bytes<=0) break;
		offset+=bytes;
	} while (offset<hdrsize);
	if (bytes<=0) {
		// failed to send header (socket may have been closed by peer)
		SYSLOG(fpLog,"Failed to send header\n");
		return -1;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////
// _mwSendRawDataChunk
// Send a chunk of a raw data block
////////////////////////////////////////////////////////////////////////////
int _mwSendRawDataChunk(ServerThread *hp, HttpSocket* phsSocket)
{
	int  iBytesWritten;

	if (phsSocket->flags & FLAG_CHUNK) {
		char buf[16];
		int bytes = snprintf(buf, sizeof(buf), "%x\r\n", phsSocket->dataLength);
		iBytesWritten = send(phsSocket->socket, buf, bytes, 0);
	}
	// send a chunk of data
	if (phsSocket->dataLength > 0) {
		iBytesWritten=(int)send(phsSocket->socket, (char*)phsSocket->pucData, phsSocket->dataLength, 0);
		if (iBytesWritten<=0) {
			// failure - close connection
			SYSLOG(fpLog,"Connection closed\n");
			SETFLAG(phsSocket,FLAG_CONN_CLOSE);
			_mwCloseSocket(hp, phsSocket);
			return -1;
		} else {
			SYSLOG(fpLog,"[%d] sent %d bytes of raw data\n",phsSocket->socket,iBytesWritten);
			phsSocket->response.sentBytes+=iBytesWritten;
			phsSocket->pucData+=iBytesWritten;
			phsSocket->dataLength-=iBytesWritten;
		}
	} else {
		if (ISFLAGSET(phsSocket,FLAG_DATA_STREAM) && phsSocket->handler) {
			//load next chuck of raw data
			UrlHandlerParam up;
			UrlHandler* pfnHandler = (UrlHandler*)phsSocket->handler;
			up.hs = phsSocket;
			up.hp = hp;
			up.pucBuffer=(char*)phsSocket->pucData;
			up.dataBytes=HTTP_BUFFER_SIZE;
			if ((pfnHandler->pfnUrlHandler)(&up) == 0) {
				if (phsSocket->flags & FLAG_CHUNK) {
					send(phsSocket->socket, "0\r\n\r\n", 5, 0);
				}
				SETFLAG(phsSocket, FLAG_CONN_CLOSE);
				return 1;	// EOF
			}
			phsSocket->dataLength=up.dataBytes;
			phsSocket->pucData = (uchar*)up.pucBuffer;
		} else {
			if (phsSocket->flags & FLAG_CHUNK) {
				send(phsSocket->socket, "0\r\n\r\n", 5, 0);
			}
			return 1;
		}
	}
	return 0;
} // end of _mwSendRawDataChunk

////////////////////////////////////////////////////////////////////////////
// _mwRedirect
// Setup for redirect to another file
////////////////////////////////////////////////////////////////////////////
void _mwRedirect(HttpSocket* phsSocket, char* pchPath)
{
	char hdr[128];
	int n = snprintf(hdr, sizeof(hdr), HTTP301_HEADER, HTTP_SERVER_NAME, pchPath);
	send(phsSocket->socket, hdr, n, 0);
	SETFLAG(phsSocket, FLAG_CONN_CLOSE);
} // end of _mwRedirect

////////////////////////////////////////////////////////////////////////////
// _mwSubstVariables
// Perform substitution of variables in a buffer
// returns new length
////////////////////////////////////////////////////////////////////////////
int _mwSubstVariables(ServerThread* hp, char* pchData, int iLength, int* piBytesUsed)
{
	int lastpos,pos=0,used=0;
	SubstParam sp;
	int iValueBytes;
	DBG("_SubstVariables input len %d\n",iLength);
	iLength--;
	for(;;) {
		lastpos=pos;
		for (; pos<iLength && *(WORD*)(pchData+pos)!=HTTP_SUBST_PATTERN; pos++);
		used+=(pos-lastpos);
		if (pos==iLength) {
			*piBytesUsed=used+1;
			return iLength+1;
		}
		lastpos=pos;
		for (pos=pos+2; pos<iLength && *(WORD*)(pchData+pos)!=HTTP_SUBST_PATTERN; pos++);
		if (pos==iLength) {
			*piBytesUsed=used;
			return lastpos;
		}
		pos+=2;
		used+=pos-lastpos;
		pchData[pos-2]=0;
		sp.pchParamName=pchData+lastpos+2;
		sp.iMaxValueBytes=pos-lastpos;
		sp.pchParamValue=pchData+lastpos;
		iValueBytes=hp->pfnSubst(&sp);
		if (iValueBytes>=0) {
			if (iValueBytes>sp.iMaxValueBytes) iValueBytes=sp.iMaxValueBytes;
			memmove(pchData+lastpos+iValueBytes,pchData+pos,iLength-pos);
			iLength=iLength+iValueBytes-(pos-lastpos);
			pos=lastpos+iValueBytes;
		} else {
			DBG("No matched variable for %s\n",sp.pchParamName);
			pchData[pos-2]='$';
		}
	}
} // end of _mwSubstVariables

////////////////////////////////////////////////////////////////////////////
// _mwStrStrNoCase
// Case insensitive version of ststr
////////////////////////////////////////////////////////////////////////////
char* _mwStrStrNoCase(char* pchHaystack, char* pchNeedle)
{
	char* pchReturn=NULL;

	while(*pchHaystack!='\0' && pchReturn==NULL) {
		if (toupper(*pchHaystack)==toupper(pchNeedle[0])) {
			char* pchTempHay=pchHaystack;
			char* pchTempNeedle=pchNeedle;
			// start of match
			while(*pchTempHay!='\0') {
				if(toupper(*pchTempHay)!=toupper(*pchTempNeedle)) {
					// failed to match
					break;
				}
				pchTempNeedle++;
				pchTempHay++;
				if (*pchTempNeedle=='\0') {
					// completed match
					pchReturn=pchHaystack;
					break;
				}
			}
		}
		pchHaystack++;
	}

	return pchReturn;
} // end of _mwStrStrNoCase

////////////////////////////////////////////////////////////////////////////
// _mwStrDword
////////////////////////////////////////////////////////////////////////////
char* _mwStrDword(char* pchHaystack, DWORD dwSub, DWORD dwCharMask)
{
	dwCharMask = dwCharMask?(dwCharMask & 0xdfdfdfdf):0xdfdfdfdf;
	dwSub &= dwCharMask;
	while(*pchHaystack) {
		if (((*(DWORD*)pchHaystack) & dwCharMask)==dwSub)
			return pchHaystack;
		pchHaystack++;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////
// _mwDecodeCharacter
// Convert and individual character
////////////////////////////////////////////////////////////////////////////
__inline char _mwDecodeCharacter(char* s)
{
	register unsigned char v;
	if (!*s) return 0;
	if (*s>='a' && *s<='f')
		v = *s-('a'-'A'+7);
	else if (*s>='A' && *s<='F')
		v = *s-7;
	else
		v = *s;
	if (*(++s)==0) return v;
	v <<= 4;
	if (*s>='a' && *s<='f')
		v |= (*s-('a'-'A'+7)) & 0xf;
	else if (*s>='A' && *s<='F')
		v |= (*s-7) & 0xf;
	else
		v |= *s & 0xf;
	return v;
} // end of _mwDecodeCharacter

////////////////////////////////////////////////////////////////////////////
// _mwDecodeString
// This function converts URLd characters back to ascii. For example
// %3A is '.'
////////////////////////////////////////////////////////////////////////////
void mwDecodeString(char* pchString)
{
	int bEnd=FALSE;
	char* pchInput=pchString;
	char* pchOutput=pchString;

	do {
		switch (*pchInput) {
		case '%':
			if (*(pchInput+1)=='\0' || *(pchInput+2)=='\0') {
				// something not right - terminate the string and abort
				*pchOutput='\0';
				bEnd=TRUE;
			} else {
				*pchOutput=_mwDecodeCharacter(pchInput+1);
				pchInput+=3;
			}
			break;
		case '+':
			*pchOutput=' ';
			pchInput++;
			break;
		case '\0':
			bEnd=TRUE;
			// drop through
		default:
			// copy character
			*pchOutput=*pchInput;
			pchInput++;
		}
		pchOutput++;
	} while (!bEnd);
} // end of mwDecodeString

int mwGetContentType(const char *pchExtname)
{
	// check type of file requested
	if (pchExtname[1]=='\0') {
		return HTTPFILETYPE_OCTET;
	} else if (pchExtname[2]=='\0') {
		switch (GETDWORD(pchExtname) & 0xffdfdf) {
		case FILEEXT_JS: return HTTPFILETYPE_JS;
		case FILEEXT_TS: return HTTPFILETYPE_TS;
		}
	} else if (pchExtname[3]=='\0' || pchExtname[3]=='?') {
		//identify 3-char file extensions
		switch (GETDWORD(pchExtname) & 0xffdfdfdf) {
		case FILEEXT_HTM:	return HTTPFILETYPE_HTML;
		case FILEEXT_XML:	return HTTPFILETYPE_XML;
		case FILEEXT_XSL:	return HTTPFILETYPE_XML;
		case FILEEXT_TEXT:	return HTTPFILETYPE_TEXT;
		case FILEEXT_XUL:	return HTTPFILETYPE_XUL;
		case FILEEXT_CSS:	return HTTPFILETYPE_CSS;
		case FILEEXT_PNG:	return HTTPFILETYPE_PNG;
		case FILEEXT_JPG:	return HTTPFILETYPE_JPEG;
		case FILEEXT_GIF:	return HTTPFILETYPE_GIF;
		case FILEEXT_SWF:	return HTTPFILETYPE_SWF;
		case FILEEXT_MPA:	return HTTPFILETYPE_MPA;
		case FILEEXT_MPG:	return HTTPFILETYPE_MPEG;
		case FILEEXT_AVI:	return HTTPFILETYPE_AVI;
		case FILEEXT_MP4:	return HTTPFILETYPE_MP4;
		case FILEEXT_MOV:	return HTTPFILETYPE_MOV;
		case FILEEXT_264:	return HTTPFILETYPE_264;
		case FILEEXT_FLV:	return HTTPFILETYPE_FLV;
		case FILEEXT_3GP:	return HTTPFILETYPE_3GP;
		case FILEEXT_ASF:	return HTTPFILETYPE_ASF;
		}
	} else if (pchExtname[4]=='\0' || pchExtname[4]=='?') {
		//logic-and with 0xdfdfdfdf gets the uppercase of 4 chars
		switch (GETDWORD(pchExtname)&0xdfdfdfdf) {
		case FILEEXT_HTML:	return HTTPFILETYPE_HTML;
		case FILEEXT_MPEG:	return HTTPFILETYPE_MPEG;
		case FILEEXT_M3U8:	return HTTPFILETYPE_M3U8;
		}
	}
	return HTTPFILETYPE_OCTET;
}

int _mwGrabToken(char *pchToken, char chDelimiter, char *pchBuffer, int iMaxTokenSize)
{
	char *p=pchToken;
	int iCharCopied=0;

	while (*p && *p!=chDelimiter && iCharCopied < iMaxTokenSize - 1) {
		*(pchBuffer++)=*(p++);
		iCharCopied++;
	}
	*pchBuffer='\0';
	return (*p==chDelimiter)?iCharCopied:0;
}

int _mwParseHttpHeader(HttpSocket* phsSocket)
{
	int iLen;
	char buf[256];
	char *p=(char*)phsSocket->buffer;		//pointer to header data
	HttpRequest *req=&phsSocket->request;

	p = strstr((char*)phsSocket->buffer, "HTTP/1.");
	if (!p) return -1;
	p += 7;
	req->iHttpVer = (*p - '0');
	//analyze rest of the header
	for(;;) {
		//look for a valid field name
		while (*p && *p!='\r') p++;		//travel to '\r'
		if (!*p || GETDWORD(p)==HTTP_HEADEREND)
			break;
		p+=2;							//skip "\r\n"
		switch (*(p++)) {
		case 'C':
			if (!_mwStrHeadMatch(p,"onnection: ")) {
				p+=11;
				if (!_mwStrHeadMatch(p,"close")) {
					SETFLAG(phsSocket,FLAG_CONN_CLOSE);
					p+=5;
				} else if (!_mwStrHeadMatch(p,"Keep-Alive")) {
					CLRFLAG(phsSocket,FLAG_CONN_CLOSE);
					p+=10;
				}
			} else if (!_mwStrHeadMatch(p, "ontent-Length: ")) {
				p+=15;
				p+=_mwGrabToken(p,'\r',buf,sizeof(buf));
				phsSocket->response.contentLength=atoi(buf);
			} else if (!_mwStrHeadMatch(p, "ontent-Type: ")) {
				p += 13;
				if (!_mwStrHeadMatch(p, "multipart/form-data; boundary=")) {
					// request is a multipart POST
					p += 30;
					buf[0] = '-';
					buf[1] = '-';
					p += _mwGrabToken(p ,'\r', buf + 2, sizeof(buf) - 2);
					phsSocket->pxMP = (HttpMultipart*)calloc(1, sizeof(HttpMultipart));
					strcpy(phsSocket->pxMP->pchBoundaryValue, buf);
				} else {
					for (; *p != '\r'; p++) {
						if (!_mwStrHeadMatch(p, "; filename=")) {
							p += 11;
							p += _mwGrabToken(p ,'\r', buf, sizeof(buf));
							phsSocket->pxMP = (HttpMultipart*)calloc(1, sizeof(HttpMultipart));
							phsSocket->pxMP->pchFilename = strdup(buf);
							break;
						}
					}
				}
			}
			break;
		case 'R':
			if (!_mwStrHeadMatch(p,"eferer: ")) {
				p+=8;
				phsSocket->request.ofReferer=(int)(p - (char*)phsSocket->buffer);
			} else if (!_mwStrHeadMatch(p,"ange: bytes=")) {
				int iEndByte;
				p+=12;
				iLen=_mwGrabToken(p,'-',buf,sizeof(buf));
				if (iLen==0) continue;
				p+=iLen;
				phsSocket->request.startByte=atoi(buf);
				iLen=_mwGrabToken(p,'/',buf,sizeof(buf));
				if (iLen==0) continue;
				p+=iLen;
				iEndByte = atoi(buf);
				if (iEndByte > 0)
					phsSocket->response.contentLength = (int)(iEndByte-phsSocket->request.startByte+1);
			}
			break;
		case 'H':
			if (!_mwStrHeadMatch(p,"ost: ")) {
				p+=5;
				phsSocket->request.ofHost=(int)(p - (char*)phsSocket->buffer);
			}
			break;
		}
	}
	return 0;					//end of header
}
//////////////////////////// END OF FILE ///////////////////////////////////

// CRC-32 polynominal:
// X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X+1

static unsigned long crc32tab[] = {
	0x00000000L, 0x77073096L, 0xEE0E612CL, 0x990951BAL,	0x076DC419L, 0x706AF48FL, 0xE963A535L, 0x9E6495A3L,
	0x0EDB8832L, 0x79DCB8A4L, 0xE0D5E91EL, 0x97D2D988L,	0x09B64C2BL, 0x7EB17CBDL, 0xE7B82D07L, 0x90BF1D91L,
	0x1DB71064L, 0x6AB020F2L, 0xF3B97148L, 0x84BE41DEL,	0x1ADAD47DL, 0x6DDDE4EBL, 0xF4D4B551L, 0x83D385C7L,
	0x136C9856L, 0x646BA8C0L, 0xFD62F97AL, 0x8A65C9ECL,	0x14015C4FL, 0x63066CD9L, 0xFA0F3D63L, 0x8D080DF5L,
	0x3B6E20C8L, 0x4C69105EL, 0xD56041E4L, 0xA2677172L,	0x3C03E4D1L, 0x4B04D447L, 0xD20D85FDL, 0xA50AB56BL,
	0x35B5A8FAL, 0x42B2986CL, 0xDBBBC9D6L, 0xACBCF940L,	0x32D86CE3L, 0x45DF5C75L, 0xDCD60DCFL, 0xABD13D59L,
	0x26D930ACL, 0x51DE003AL, 0xC8D75180L, 0xBFD06116L,	0x21B4F4B5L, 0x56B3C423L, 0xCFBA9599L, 0xB8BDA50FL,
	0x2802B89EL, 0x5F058808L, 0xC60CD9B2L, 0xB10BE924L,	0x2F6F7C87L, 0x58684C11L, 0xC1611DABL, 0xB6662D3DL,
	0x76DC4190L, 0x01DB7106L, 0x98D220BCL, 0xEFD5102AL,	0x71B18589L, 0x06B6B51FL, 0x9FBFE4A5L, 0xE8B8D433L,
	0x7807C9A2L, 0x0F00F934L, 0x9609A88EL, 0xE10E9818L,	0x7F6A0DBBL, 0x086D3D2DL, 0x91646C97L, 0xE6635C01L,
	0x6B6B51F4L, 0x1C6C6162L, 0x856530D8L, 0xF262004EL,	0x6C0695EDL, 0x1B01A57BL, 0x8208F4C1L, 0xF50FC457L,
	0x65B0D9C6L, 0x12B7E950L, 0x8BBEB8EAL, 0xFCB9887CL,	0x62DD1DDFL, 0x15DA2D49L, 0x8CD37CF3L, 0xFBD44C65L,
	0x4DB26158L, 0x3AB551CEL, 0xA3BC0074L, 0xD4BB30E2L,	0x4ADFA541L, 0x3DD895D7L, 0xA4D1C46DL, 0xD3D6F4FBL,
	0x4369E96AL, 0x346ED9FCL, 0xAD678846L, 0xDA60B8D0L,	0x44042D73L, 0x33031DE5L, 0xAA0A4C5FL, 0xDD0D7CC9L,
	0x5005713CL, 0x270241AAL, 0xBE0B1010L, 0xC90C2086L,	0x5768B525L, 0x206F85B3L, 0xB966D409L, 0xCE61E49FL,
	0x5EDEF90EL, 0x29D9C998L, 0xB0D09822L, 0xC7D7A8B4L,	0x59B33D17L, 0x2EB40D81L, 0xB7BD5C3BL, 0xC0BA6CADL,
	0xEDB88320L, 0x9ABFB3B6L, 0x03B6E20CL, 0x74B1D29AL,	0xEAD54739L, 0x9DD277AFL, 0x04DB2615L, 0x73DC1683L,
	0xE3630B12L, 0x94643B84L, 0x0D6D6A3EL, 0x7A6A5AA8L,	0xE40ECF0BL, 0x9309FF9DL, 0x0A00AE27L, 0x7D079EB1L,
	0xF00F9344L, 0x8708A3D2L, 0x1E01F268L, 0x6906C2FEL,	0xF762575DL, 0x806567CBL, 0x196C3671L, 0x6E6B06E7L,
	0xFED41B76L, 0x89D32BE0L, 0x10DA7A5AL, 0x67DD4ACCL,	0xF9B9DF6FL, 0x8EBEEFF9L, 0x17B7BE43L, 0x60B08ED5L,
	0xD6D6A3E8L, 0xA1D1937EL, 0x38D8C2C4L, 0x4FDFF252L,	0xD1BB67F1L, 0xA6BC5767L, 0x3FB506DDL, 0x48B2364BL,
	0xD80D2BDAL, 0xAF0A1B4CL, 0x36034AF6L, 0x41047A60L,	0xDF60EFC3L, 0xA867DF55L, 0x316E8EEFL, 0x4669BE79L,
	0xCB61B38CL, 0xBC66831AL, 0x256FD2A0L, 0x5268E236L,	0xCC0C7795L, 0xBB0B4703L, 0x220216B9L, 0x5505262FL,
	0xC5BA3BBEL, 0xB2BD0B28L, 0x2BB45A92L, 0x5CB36A04L,	0xC2D7FFA7L, 0xB5D0CF31L, 0x2CD99E8BL, 0x5BDEAE1DL,
	0x9B64C2B0L, 0xEC63F226L, 0x756AA39CL, 0x026D930AL,	0x9C0906A9L, 0xEB0E363FL, 0x72076785L, 0x05005713L,
	0x95BF4A82L, 0xE2B87A14L, 0x7BB12BAEL, 0x0CB61B38L,	0x92D28E9BL, 0xE5D5BE0DL, 0x7CDCEFB7L, 0x0BDBDF21L,
	0x86D3D2D4L, 0xF1D4E242L, 0x68DDB3F8L, 0x1FDA836EL,	0x81BE16CDL, 0xF6B9265BL, 0x6FB077E1L, 0x18B74777L,
	0x88085AE6L, 0xFF0F6A70L, 0x66063BCAL, 0x11010B5CL,	0x8F659EFFL, 0xF862AE69L, 0x616BFFD3L, 0x166CCF45L,
	0xA00AE278L, 0xD70DD2EEL, 0x4E048354L, 0x3903B3C2L,	0xA7672661L, 0xD06016F7L, 0x4969474DL, 0x3E6E77DBL,
	0xAED16A4AL, 0xD9D65ADCL, 0x40DF0B66L, 0x37D83BF0L,	0xA9BCAE53L, 0xDEBB9EC5L, 0x47B2CF7FL, 0x30B5FFE9L,
	0xBDBDF21CL, 0xCABAC28AL, 0x53B39330L, 0x24B4A3A6L,	0xBAD03605L, 0xCDD70693L, 0x54DE5729L, 0x23D967BFL,
	0xB3667A2EL, 0xC4614AB8L, 0x5D681B02L, 0x2A6F2B94L,	0xB40BBE37L, 0xC30C8EA1L, 0x5A05DF1BL, 0x2D02EF8DL
};

void crc32Init(unsigned long *pCrc32)
{
	*pCrc32 = 0xFFFFFFFF;
}

void crc32Update(unsigned long *pCrc32, unsigned char *pData, unsigned long uSize)
{
	unsigned long i = 0;

	for(i = 0; i < uSize; i++)
		*pCrc32 = ((*pCrc32) >> 8) ^ crc32tab[(pData[i]) ^ ((*pCrc32) & 0x000000FF)];
}

// Make the final adjustment
void crc32Finish(unsigned long *pCrc32)
{
	*pCrc32 = ~(*pCrc32);
}

//int nInst = 0;
ServerThread* httpParam = 0;

//////////////////////////////////////////////////////////////////////////
// callback from the web server whenever it needs to substitute variables
//////////////////////////////////////////////////////////////////////////
int DefaultWebSubstCallback(SubstParam* sp)
{
	// the maximum length of variable value should never exceed the number
	// given by sp->iMaxValueBytes
	if (!strcmp(sp->pchParamName,"mykeyword")) {
		return sprintf(sp->pchParamValue, "%d", time(NULL));
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////////
// callback from the web server whenever it recevies posted data
//////////////////////////////////////////////////////////////////////////
int DefaultWebPostCallback(PostParam* pp)
{
  int iReturn=WEBPOST_OK;

  // by default redirect to config page
  //strcpy(pp->chFilename,"index.htm");

  return iReturn;
}

//////////////////////////////////////////////////////////////////////////
// callback from the web server whenever it receives a multipart 
// upload file chunk
//////////////////////////////////////////////////////////////////////////
int DefaultWebFileUploadCallback(HttpMultipart *pxMP, OCTET *poData, size_t dwDataChunkSize)
{
  // Do nothing with the data
	int fd = (int)pxMP->pxCallBackData;
	if (!poData) {
		// to cleanup
		if (fd > 0) {
			close(fd);
			pxMP->pxCallBackData = NULL;
		}
		return 0;
	}
	if (!fd) {
		char filename[256];
		snprintf(filename, sizeof(filename), "%s/%s", httpParam[0].pchWebPath, pxMP->pchFilename);
		fd = open(filename, O_CREAT | O_TRUNC | O_RDWR | O_BINARY, 0);
		pxMP->pxCallBackData = (void*)fd;
	}
	if (fd <= 0) return -1;
	write(fd, poData, dwDataChunkSize);
	if (pxMP->oFileuploadStatus & HTTPUPLOAD_LASTCHUNK) {
		close(fd);
		pxMP->pxCallBackData = NULL;
	}
	printf("Received %lu bytes for multipart upload file %s\n", dwDataChunkSize, pxMP->pchFilename);
	return 0;
}

// Parameter object for optional parameters 
class Option
{
//public: // to delete 
	int count;
	char** value;
	int current;
public:
	Option( int _count, char* _value[] ): count(_count), value(_value), current(0) {}
	void Set( int &param ){ param = atoi( value[current] ); }
	void Set( char* &param ){ param = value[current]; }
	bool Find( char* option )
	{
		for( int i = 0; i < count; i++ ) {
			if( !strcmp( value[i], option) && i + 1 < count ){
				current = i + 1; 
				return true;
			}
		}
		return false;
	}
	bool FindNext( char* option )
	{
		for( int i = current; i < count; i++ ) {
			if( !strcmp( value[i], option) && i + 1 < count ){
				current = i + 1; 
				return true;
			}
		}
		return false;	
	}
	Option SubGroup( char* option )
	{
		int i = current;
		for(; i < count; i++ ) {
			if( !strcmp( value[i], option) ){
				break;
			}
		}
		return Option( i - current, value + current );	
	}
};

// Server application 
class ServerApp
{
	static char* name;
	static ServerThread* httpParam;
	static int nInst;
	static void Shutdown()
	{
		if( !nInst ) return;
		for( int i = 0; i < nInst; i++ ){
			printf("Shutting down instance %d\n",i);
			mwServerShutdown(&httpParam[i]);
		}
		fclose(fpLog);
		UninitSocket();
	}	
	static int MiniWebQuit(int arg)
	{
		static bool quitting = false;
		if( !quitting ){
			quitting = true;
			if( arg ){
				printf("\nCaught signal (%d). %s shutting down...\n", arg, name );
			}
			Shutdown();
		}
		return 0;
	}
public:	
	static int error;
	ServerApp( char* appName, char* author, Option option ){
		if( nInst ){ throw; } // Singleton guard 
		name = appName;
		printf("%s %d.%d.%d (C)%s\n\n", name, VER_MAJOR, VER_MINOR, TH2_REVISION, author);
		#ifdef WIN32
		SetConsoleCtrlHandler( (PHANDLER_ROUTINE) MiniWebQuit, TRUE );
		#else
		signal(SIGINT, (void *) MiniWebQuit);
		signal(SIGTERM, (void *) MiniWebQuit);
		signal(SIGPIPE, SIG_IGN);
		#endif
		#ifndef NOTHREAD
		if( option.Find("-n") ){ option.Set( nInst ); }
		#endif
		if( !nInst ) nInst=1;
		//initialize HTTP parameter structure
		int iParamBytes = nInst * sizeof(ServerThread);
		httpParam = (ServerThread*)malloc( iParamBytes );
		if( !httpParam ){
			printf("Out of memory\n");
			error = -1; nInst = 0; return;
		}
		memset( httpParam, 0, iParamBytes );
		//fill in default settings
		for( int i = 0; i < nInst; i++ ){
			httpParam[i].maxClients=32;
			httpParam[i].pchWebPath="webroot";
			httpParam[i].pxUrlHandler=urlHandlerList;
			httpParam[i].flags=FLAG_DIR_LISTING;
			httpParam[i].tmSocketExpireTime = 180;
			#ifndef _NO_POST
			httpParam[i].pfnPost = DefaultWebPostCallback;
			#ifdef MEDIA_SERVER
			httpParam[i].pfnFileUpload = TranscodeUploadCallback;
			#else
			httpParam[i].pfnFileUpload = DefaultWebFileUploadCallback;
			#endif
			#endif
		}
		//parsing command line arguments
		if( option.Find("-l") ){ 
			char* logName; 
			option.Set( logName );
			fpLog = freopen( logName, "w", stderr);
		}
		int inst = 0;
		bool nextOption = option.Find("-i");
		do{
			if( nextOption ) option.Set( inst );
			if( inst > nInst ){ inst = nInst; }
			Option serverThreadOption = option.SubGroup("-i");
			if( serverThreadOption.Find("-p") ){ serverThreadOption.Set( httpParam[inst].httpPort ); }
			if( serverThreadOption.Find("-r") ){ serverThreadOption.Set( httpParam[inst].pchWebPath ); }
			if( serverThreadOption.Find("-m") ){ serverThreadOption.Set( httpParam[inst].maxClients ); }
			if( serverThreadOption.Find("-d") ){ httpParam[inst].flags &= ~FLAG_DIR_LISTING; }
		}while( nextOption = option.FindNext("-i") );

		{
			int i;
			int err = 0;
			for (i = 0; urlHandlerList[i].pchUrlPrefix; i++) {
				if (urlHandlerList[i].pfnEventHandler && urlHandlerList[i].pfnEventHandler(MW_PARSE_ARGS, option) ){
					err++;
				}
			}
			if (err > 0) {
				printf("Error parsing command line options\n");
				error = -1; return;
			}
		}
		//adjust port setting
		{
			short int port=80;
			for( int i = 0; i < nInst; i++ ) {
				if (httpParam[i].httpPort){
					port = httpParam[i].httpPort+1;
				}else{
					httpParam[i].httpPort = port++;
				}
			}
		}
		InitSocket();
		if (nInst>1) printf("Number of instances: %d\n",nInst);
		{
			int i;
			int err=0;
			for (i=0;i<nInst;i++) {
				int n;
				if (nInst>1) printf("\nInstance %d\n",i);
				printf("Listening port: %d\n",httpParam[i].httpPort);
				printf("Web root: %s\n",httpParam[i].pchWebPath);
				printf("Max clients: %d\n",httpParam[i].maxClients);
				for (n=0;urlHandlerList[n].pchUrlPrefix;n++);
				printf("URL handlers: %d\n",n);
				if (httpParam[i].flags & FLAG_DIR_LISTING) printf("Dir listing: on\n");

				//register page variable substitution callback
				//httpParam[i].pfnSubst=DefaultWebSubstCallback;

				//start server
				if (mwServerStart(&httpParam[i])) {
					printf("Error starting instance #%d\n",i);
					err++;
				}
			}

			if (err < nInst) {
				#ifndef NOTHREAD
				ThreadWait(httpParam[0].tidHttpThread,INFINITE,NULL);
				#endif
			}else{
				printf("Failed to launch miniweb\n");
			}
		}

	}
	~ServerApp(){ Shutdown(); }
};

char* ServerApp::name = 0;
ServerThread* ServerApp::httpParam = 0;
int ServerApp::nInst = 0;
int ServerApp::error = 0;

int main(int argc, char* argv[])
{
	ServerApp( "TH Web", "2011 by Desert Dragon", Option(argc - 1, argv + 1) );
	return ServerApp::error;
}
////////////////////////////// END OF FILE //////////////////////////////


/*
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

WSADATA wsaData;
SOCKET ListenSocket = INVALID_SOCKET,
	ClientSocket = INVALID_SOCKET;
struct addrinfo *result = NULL,
	hints;
char recvbuf[DEFAULT_BUFLEN];
int recvbuflen = DEFAULT_BUFLEN;

int ServerShutdown()
{
	// shutdown the connection since we're done
	int iResult;
	if( ClientSocket == INVALID_SOCKET ){
		printf("No one client socket\n");
	}
	iResult = shutdown(ClientSocket, SD_SEND);
	if ( iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		_getch();
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	printf("shutdown succeed with result: %d\n", iResult);
	_getch();

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();


	//	int i;
	// 	for (i=0;i<nInst;i++) {
	// 		printf("Shutting down instance %d\n",i);
	// 		mwServerShutdown(&httpParam[i]);
	// 	}
	// 	fclose(fpLog);
	// 	UninitSocket();
	
	return 0;
}

int ServerConsoleQuit(int arg)
{
	static int quitting = 0;
	if (quitting) return 0;
	quitting = 1;
	if (arg){
		printf("\nCaught signal (%d). TH2 server shutting down...\n", arg);
	}
	ServerShutdown();
	return 1;
}

void InitConsole() 
{
	if( AllocConsole() ){
		//SetConsoleOutputCP(1251);
		FILE *stdOut = _fdopen( _open_osfhandle((long) GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT), "w");
		FILE *stdIn = _fdopen( _open_osfhandle((long) GetStdHandle(STD_INPUT_HANDLE), _O_RDONLY), "r");
		if( setvbuf(stdOut, NULL, _IONBF, 0) == 0 ){
			*stdout = *stdOut;
			*stderr = *stdOut;
		}
		*stdin = *stdIn;
	}
}

// WinMainCRTStartup

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	InitConsole();

	SetConsoleCtrlHandler( (PHANDLER_ROUTINE) ServerConsoleQuit, TRUE );

	printf("TH2 Server ver 1.%d\n", TH2_REVISION);
	
	int iResult, iSendResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}
	printf("WSAStartup succeed with result: %d\n", iResult);

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if ( iResult != 0 ) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	printf("getaddrinfo succeed with result: %d\n", iResult);


	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	printf("socket succeed with ListenSocket: %ld\n", ListenSocket);

	// Setup the TCP listening socket
	iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	printf("bind succeed with result: %d\n", iResult);

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	printf("listen succeed with result: %d\n", iResult);
	printf("Listening port: %s\n", DEFAULT_PORT);
	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	printf("accept succeed with ClientSocket: %d\n", ClientSocket);

	// No longer need server socket
	closesocket(ListenSocket);

	// Receive until the peer shuts down the connection
	do{
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if( iResult > 0 ){
			printf("Bytes received: %d\n", iResult);
			
			// Echo the buffer back to the sender
			iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
			if( iSendResult == SOCKET_ERROR ){
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}else if( iResult == 0 ){
			printf("Connection closing...\n");
		}else{
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	}while( iResult > 0 );

	return ServerShutdown();
}
//*/