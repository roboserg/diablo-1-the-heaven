#include "stdafx.h"
#ifndef OLD_PCX_SCREENSHOT
#include "ScreenShotMaker.h"
#endif

LPDIRECT3DDEVICE9 D3dDevice = 0; // Our rendering device
LPDIRECT3DDEVICE9 SecDevice = 0; // Second win rendering device
LPDIRECT3DSURFACE9 BackBuffer = 0; // d3d hardware backbuffer, for copy render surface
LPDIRECT3DSURFACE9 SecBuffer = 0; // d3d hardware backbuffer, for second win
LPDIRECT3DSURFACE9 D3dSurface = 0; // rendering surface, like frontbuffer ScreenSurface32
LPDIRECT3DSURFACE9 SecD3dSurface = 0; // rendering surface, for second win
LPDIRECT3DSURFACE9 VideoSurface = 0; // stretching video surface
LPDIRECT3DSURFACE9 SecVideoSurface = 0; // stretching video surface for second win
// http://stackoverflow.com/questions/14955954/update-directx-texture
// http://stackoverflow.com/questions/10596453/how-do-i-correctly-implement-texture-pixel-to-texel-correction-in-direct3d-9
LPDIRECT3DTEXTURE9 D3dTexture = 0; // rendering texture
LPDIRECT3DTEXTURE9 SecTexture = 0; // rendering texture for second win
// http://stackoverflow.com/questions/13145084/fast-paletted-screen-blit-with-direct3d-9
LPDIRECT3DTEXTURE9 D3dPalette = 0; // palette texture
LPDIRECT3DTEXTURE9 SecPalette = 0; // palette texture for second win
IDirect3DPixelShader9* PaletteShader = 0; // Main shader
IDirect3DPixelShader9* InterfaceShader = 0; // reserved for second layer
IDirect3DPixelShader9* DirectShader = 0; // shader for automap
struct TransformedVertex { vec4 pos;	vec2 tex; };

extern unsigned int AutomapPointsCount;
extern unsigned int AutomapLinePointsCount;
extern line_vertex AutomapPoints[20000];
extern line_vertex AutomapLines[10000];

bool D3dRef = false;
void SetD3dParam(D3DPRESENT_PARAMETERS& d3dpp);
bool D3dReset();

//LPDIRECTDRAW7 DirectDrawPtr7 = 0;
LPDIRECTDRAWSURFACE ScreenSurface32 = 0;
LPDIRECTDRAWSURFACE ScreenSurface32Back = 0;

LPDIRECTDRAW DirectDrawPtr;
LPDIRECTDRAWPALETTE DirectDrawPalettePtr;
LPDIRECTDRAWSURFACE DDSrcSurface;
LPDIRECTDRAWSURFACE ScreenSurface;       // match ScreenSurface *(LPDIRECTDRAWSURFACE2*)0x1501EE40 from storm.dll
int ScreenSurfacePitch;
// 15009BD0 DrawPrimarySurface storm.dll
CRITICAL_SECTION DDSurfaceCritSec;
DDSURFACEDESC ScreenSurfaceDesc;
int SurfaceLockCount;
uchar* SoftSrcSurfacePtr;           // указатель на память размером 656*WorkingWidth

int DrawnCursorWIdth;
int CurCursorWidth;
int DrawnCursorStartX;
int DrawnCursorStartY;
int DrawnCursorHeight;
int CurCursorStartX;
int CurCursorStartY;
int CurCursorHeight;
unsigned char CursorImage[1920 + MaxItemCellWidth_4 * CELL_SIZE * MaxItemCellHeight_4 * CELL_SIZE];

//----- PCX & GIF ------------------------------------------------------------------------
enum PIC_FORMAT { PF_PCX = 0, PF_GIF = 1 };
int ScreenshotFormat = PF_GIF;

struct PcxHeader
{
	char Manufacturer;          // Постоянный флаг 10 = ZSoft .PCX
	char Version;               // 5 = Версия 3.0
	char Encoding;              // 1 = .PCX кодирование длинными сериями
	char BitsPerLayer;          // Число бит на пиксел в слое
	unsigned short Xmin;        // Размеры изображения
	unsigned short Ymin;        // Размеры изображения
	unsigned short Xmax;        // Размеры изображения
	unsigned short Ymax;        // Размеры изображения
	unsigned short HDpi;        // Горизонтальное разрешение создающего устройства
	unsigned short VDpi;        // Вертикальное разрешение создающего устройства
	char Colormap[ 48 ];          // CGA/EGA Палитра
	char Reserved;
	char NPlanes;               // Число цветовых слоев
	unsigned short BPL;         // Число байт на строку в цветовом слое (четнjt)
	unsigned short Palette;     // Как интерпретировать EGA палитру 1 = цветная/черно-белая, 2 = градации серого
	unsigned short HscreenSize; // Horizontal screen size in pixels. Not used.
	unsigned short VscreenSize; // Vertical screen size in pixels. Not used.
	char Filler[ 54 ];            // Заполняется нулями до конца заголовка
};

struct GIF
{
	ushort w, h;
	int depth;
	int fd;
	int offset;
	int nframes;
	uchar *frame, *back;
	uint partial;
	uchar buffer[ 255 ];
};

GIF* new_gif( const char* fname, ushort width, ushort height, uchar* palette, int depth, int loop );
void add_frame( GIF *gif, ushort delay );
void close_gif( GIF* gif );

typedef uchar uchara[];

/* helper to write a little-endian 16-bit number portably */
#define write_num(fd, n) write((fd), uchara {uchar((n) & 0xFF), uchar((n) >> 8)} , 2)

static uchar vga[0x30] = {
    0x00, 0x00, 0x00,
    0xAA, 0x00, 0x00,
    0x00, 0xAA, 0x00,
    0xAA, 0x55, 0x00,
    0x00, 0x00, 0xAA,
    0xAA, 0x00, 0xAA,
    0x00, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA,
    0x55, 0x55, 0x55,
    0xFF, 0x55, 0x55,
    0x55, 0xFF, 0x55,
    0xFF, 0xFF, 0x55,
    0x55, 0x55, 0xFF,
    0xFF, 0x55, 0xFF,
    0x55, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF,
};

#pragma warning(disable: 4200)
struct Node {
    ushort key;
    Node *children[];
};

Node* new_node(ushort key, int degree)
{
    Node* node = (Node*) calloc(1, sizeof(*node) + degree * sizeof(Node *));
    if( node )
        node->key = key;
    return node;
}

void del_trie(Node *root, int degree)
{
    if( !root )
        return;
    for( int i = 0; i < degree; i++ )
        del_trie(root->children[i], degree);
    free(root);
}

void put_loop(GIF *gif, ushort loop);

GIF* new_gif( const char *fname, ushort width, ushort height, uchar *palette, int depth, int loop )
{
    int i, r, g, b, v;
    GIF* gif = (GIF*) calloc(1, sizeof(*gif) + 2*width*height);
    if( !gif )
        goto no_gif;
    gif->w = width; gif->h = height;
    gif->depth = depth > 1 ? depth : 2;
    gif->frame = (uint8_t *) &gif[1];
    gif->back = &gif->frame[width*height];
    gif->fd = _creat(fname, _S_IREAD | _S_IWRITE );
    if( gif->fd == -1 )
        goto no_fd;
	#ifdef _WIN32
    setmode(gif->fd, O_BINARY);
	#endif
    write(gif->fd, "GIF89a", 6);
    write_num(gif->fd, width);
    write_num(gif->fd, height);
	write(gif->fd, uchara {uchar(0xF0 | (depth-1)), 0x00, 0x00}, 3);
    if( palette ){
        write(gif->fd, palette, 3 << depth);
    }else if( depth <= 4 ){
        write(gif->fd, vga, 3 << depth);
    }else{
        write(gif->fd, vga, sizeof(vga));
        i = 0x10;
        for( r = 0; r < 6; r++ ){
            for( g = 0; g < 6; g++ ){
                for( b = 0; b < 6; b++ ){
                    write(gif->fd, uchara {uchar(r*51), uchar(g*51), uchar(b*51)}, 3);
                    if( ++i == 1 << depth )
                        goto done_gct;
                }
            }
        }
        for( i = 1; i <= 24; i++ ){
            v = i * 0xFF / 25;
            write(gif->fd, uchara {(uchar)v, (uchar)v, (uchar)v}, 3);
        }
    }
	done_gct:
    if( loop >= 0 && loop <= 0xFFFF )
        put_loop(gif, (ushort) loop);
    return gif;
	no_fd:
    free(gif);
	no_gif:
    return NULL;
}

void put_loop(GIF *gif, ushort loop)
{
    write(gif->fd, uchara {'!', 0xFF, 0x0B}, 3);
    write(gif->fd, "NETSCAPE2.0", 11);
    write(gif->fd, uchara {0x03, 0x01}, 2);
    write_num(gif->fd, loop);
    write(gif->fd, "\0", 1);
}

/* Add packed key to buffer, updating offset and partial.
 *   gif->offset holds position to put next *bit*
 *   gif->partial holds bits to include in next byte */
void put_key(GIF *gif, ushort key, int key_size)
{
    int byte_offset, bit_offset, bits_to_write;
    byte_offset = gif->offset / 8;
    bit_offset = gif->offset % 8;
    gif->partial |= ((uint) key) << bit_offset;
    bits_to_write = bit_offset + key_size;
    while( bits_to_write >= 8 ){
        gif->buffer[byte_offset++] = gif->partial & 0xFF;
        if( byte_offset == 0xFF ){
            write(gif->fd, "\xFF", 1);
            write(gif->fd, gif->buffer, 0xFF);
            byte_offset = 0;
        }
        gif->partial >>= 8;
        bits_to_write -= 8;
    }
    gif->offset = (gif->offset + key_size) % (0xFF * 8);
}

void end_key(GIF *gif)
{
    int byte_offset;
    byte_offset = gif->offset / 8;
    gif->buffer[byte_offset++] = gif->partial & 0xFF;
    write(gif->fd, uchara {(uchar)byte_offset}, 1);
    write(gif->fd, gif->buffer, byte_offset);
    write(gif->fd, "\0", 1);
    gif->offset = gif->partial = 0;
}

void put_image(GIF *gif, ushort w, ushort h, ushort x, ushort y)
{
    int nkeys, key_size, i, j;
    Node *node, *child, *root;
    int degree = 1 << gif->depth;

    write(gif->fd, ",", 1);
    write_num(gif->fd, x);
    write_num(gif->fd, y);
    write_num(gif->fd, w);
    write_num(gif->fd, h);
    write(gif->fd, uchara {0x00, (uchar)gif->depth}, 2);
    root = new_node(0, degree);
    /* Create nodes for single pixels. */
    for( nkeys = 0; nkeys < degree; nkeys++ )
        root->children[nkeys] = new_node(nkeys, degree);
    node = root;
    nkeys += 2; /* skip clear code and stop code */
    key_size = gif->depth + 1;
    put_key(gif, degree, key_size); /* clear code */
    for( i = y; i < y+h; i++ ){
        for( j = x; j < x+w; j++ ){
            uchar pixel = gif->frame[i*gif->w+j] & (degree - 1);
            child = node->children[pixel];
            if( child ){
                node = child;
            }else{
                put_key(gif, node->key, key_size);
                if( nkeys < 0x1000 ){
                    if( nkeys == (1 << key_size) )
                        key_size++;
                    node->children[pixel] = new_node(nkeys++, degree);
                }
                node = root->children[pixel];
            }
        }
    }
    put_key(gif, node->key, key_size);
    put_key(gif, degree + 1, key_size); /* stop code */
    end_key(gif);
    del_trie(root, degree);
}

int get_bbox(GIF* gif, ushort* w, ushort* h, ushort* x, ushort* y)
{
    int i, j, k;
    int left, right, top, bottom;
    left = gif->w; right = 0;
    top = gif->h; bottom = 0;
    k = 0;
    for( i = 0; i < gif->h; i++ ){
        for( j = 0; j < gif->w; j++, k++ ){
            if( gif->frame[k] != gif->back[k] ){
                if( j < left)   left    = j;
                if( j > right)  right   = j;
                if( i < top)    top     = i;
                if( i > bottom) bottom  = i;
            }
        }
    }
    if( left != gif->w && top != gif->h ){
        *x = left; *y = top;
        *w = right - left + 1;
        *h = bottom - top + 1;
        return 1;
    }else{
        return 0;
    }
}

void set_delay(GIF *gif, ushort d)
{
    write(gif->fd, uchara {'!', 0xF9, 0x04, 0x04}, 4);
    write_num(gif->fd, d);
    write(gif->fd, "\0\0", 2);
}

void add_frame(GIF *gif, ushort delay)
{
    ushort w, h, x, y;
    uchar* tmp;

    if( delay )
        set_delay(gif, delay);
    if( gif->nframes == 0 ){
        w = gif->w;
        h = gif->h;
        x = y = 0;
    }else if( !get_bbox(gif, &w, &h, &x, &y) ){
        /* image's not changed; save one pixel just to add delay */
        w = h = 1;
        x = y = 0;
    }
    put_image(gif, w, h, x, y);
    gif->nframes++;
    tmp = gif->back;
    gif->back = gif->frame;
    gif->frame = tmp;
}

void close_gif( GIF* gif)
{
    write(gif->fd, ";", 1);
    close(gif->fd);
    free(gif);
}

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           jmp inline chunks                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

__forceinline void EncodePixel( unsigned char countSamePixel, unsigned char pixel, unsigned char*&destBuffer, int& bytesToWrite )
{
    if( countSamePixel > 1 || pixel >= 0b11000000 ){
        *destBuffer++ = countSamePixel | 0b11000000;
        ++bytesToWrite;
    }
    *destBuffer++ = pixel;
    ++bytesToWrite;
}

// ╔═════════════════════════════════════════════════════════════════════════════╗
// ║                           end of inline	                                 ║
// ╚═════════════════════════════════════════════════════════════════════════════╝

// th2, patch for TH1 on windows 7 and up for debug purpose
uint GetCooperativeLevel()
{
	//ex( WinMajorMinorVer )
    //return ( (short) GetVersion() ) == 0x0106 /*windows 7*/ ? // alternate ver 
    return WinMajorMinorVer >= 0x0601 /*windows 7*/ ? 
        //#ifdef _DEBUG
            DDSCL_NORMAL
        //#elif defined _PROTECTION
        //	DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT
        //#else
        //	DDSCL_NORMAL | DDSCL_FULLSCREEN
        //#endif
    : 
        DDSCL_NORMAL /*DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT*/
    ;
}

int DoMenuScreenShot, ShotPitch, ShotStartingX, ShotStartingY, ShotWidth, ShotHeight;
uchar* ShotSrc;
PALETTEENTRY* ShotPalette;

//----- (00403368) --------------------------------------------------------
void SaveScreenShot()
{
	#ifdef OLD_PCX_SCREENSHOT
    bool retVal;
    char FileName[260];
	//SaveScreenShot_gif( );
    HANDLE hFileHandle = CreateScreenShot(FileName);
	if( ScreenshotFormat == PF_GIF || DoMenuScreenShot ){
		uchar palette[ 3 * 256 ];
		int width = DoMenuScreenShot ? ShotWidth : ScreenWidth;
		int height = DoMenuScreenShot ? ShotHeight : ScreenHeight;
		int pitch = DoMenuScreenShot ? ShotPitch : WorkingWidth;
		PALETTEENTRY* srcPal = DoMenuScreenShot ? ShotPalette : ScreenPalette;
		uchar* srcBuf = DoMenuScreenShot ? ShotSrc : SoftSrcSurfacePtr;
		int startX = DoMenuScreenShot ? ShotStartingX : Screen_LeftBorder;
		int startY = DoMenuScreenShot ? ShotStartingY : Screen_TopBorder;
		if( DoMenuScreenShot && !VideoWindowExists && Handler1 != DisableInputWindowProc ){
			POINT p;
			GetCursorPos( &p );
			ScreenToClient( ghMainWnd, &p );
			if( p.x >= 0 && p.x < GUI_Width - 1 && p.y >= 0 && p.y < GUI_Width - 1 ){
				HDC hdc1;
				HDC hdc;
				ScreenSurface->GetDC( &hdc1 );
				ScreenSurface->GetDC( &hdc );
				DrawIcon( hdc, p.x, p.y, LoadCursor( HInstance, "DIABLOCURSOR" ) );
				ScreenSurface->ReleaseDC( hdc );
				ScreenSurface->ReleaseDC( hdc1 );
			}
		}
		for( int i = 0; i < 256; ++i ) memcpy( &palette[ 3 * i ], &srcPal[ i ], 3 );
		GIF *gif = new_gif( FileName, width, height, palette, 8, -1 );
		uchar* dst = gif->frame;
		uchar* src = srcBuf + startX + startY * pitch;
		for( int i = 0; i < height; ++i ){
			memcpy( dst, src, width );
			dst += width;
			src += pitch;
		}
		add_frame( gif, 0 );
		close_gif( gif );
	}else{
		if( hFileHandle != INVALID_HANDLE_VALUE ){
			retVal = WriteScreenShotHeader(hFileHandle, ScreenWidth, ScreenHeight);
			if( retVal ){
				#ifndef USING_DDSURFACE_INGAME
				retVal = PCXWriteRow( hFileHandle, ScreenWidth, ScreenHeight, WorkingWidth, SoftSrcSurfacePtr + Screen_LeftBorder + Screen_TopBorder * WorkingWidth );
				if( false ){
					char buf[ 300 ]; // for status string (~)
					sprintf_s( buf, 300, "Screenshot saved %s", FileName ); // TheHellVersion
                    NetSendString( 1 << CurrentPlayerIndex, buf );
				}
				#else
				PALETTEENTRY palette[ 256 ];
				DirectDrawPalettePtr->GetEntries(0, 0, 256, palette);
				MakeScreenRedWhileSavingScreenshot(palette);
		        DDSURFACEDESC ScreenSurfaceDesc;
		        ScreenSurfaceDesc.dwSize = sizeof( ScreenSurfaceDesc );
		        ScreenSurface->Lock(0, &ScreenSurfaceDesc, DDLOCK_WAIT, 0);
		        retVal = PCXWriteRow(hFileHandle, ScreenWidth, ScreenHeight, ScreenSurfaceDesc.lPitch, (unsigned char*)ScreenSurfaceDesc.lpSurface);
		        ScreenSurface->Unlock(0);
		        DirectDrawPalettePtr->SetEntries(0, 0, 256, palette);
				#endif
				#ifdef USE_32BITCOLOR
		        retVal = WritePaletteToPCX(hFileHandle, ScreenPalette);
				#else
		        PALETTEENTRY pal[256];
		        GetSystemPaletteEntries(GetDC(0), 0, 256, pal);
		        retVal = WritePaletteToPCX(hFileHandle, pal);
				#endif
		    }
		    CloseHandle(hFileHandle);
		    if( !retVal )
		        DeleteFileA(FileName);
		    //Sleep(300);
		}
	}
	#else
	// @todo Move this guard into a common header file
	class DDSurfaceGuard{
	public:
		explicit DDSurfaceGuard(LPDIRECTDRAWSURFACE surface)
		  : m_surface(surface){};
		~DDSurfaceGuard()
		{
		  if( m_surface ){
		    m_surface->Unlock(0);
		  }
		}
	private:
		LPDIRECTDRAWSURFACE m_surface;
	};
	
	DDSURFACEDESC dsd = {0};
	dsd.dwSize = sizeof(DDSURFACEDESC);
	ScreenSurface->Lock(0, &dsd, DDLOCK_WAIT, 0);
	DDSurfaceGuard dds_guard(ScreenSurface);
	TheHell::Graphics::MakeScreenShot(FilePalette, dsd.dwWidth, dsd.dwHeight, dsd.lPitch, 
		static_cast<unsigned char*>(dsd.lpSurface), TheHell::Graphics::ScreenshotFormat::PNG);
	#endif
}

//----- (00403451) --------------------------------------------------------
bool __fastcall WriteScreenShotHeader(HANDLE hFile, int width, int height)
{
	#ifdef OLD_PCX_SCREENSHOT
    DWORD NumberOfBytesWritten;
	PcxHeader pcxHeader;
    memset(&pcxHeader, 0, sizeof( pcxHeader ));
    pcxHeader.Manufacturer = 10;
    pcxHeader.Version = 5;
    pcxHeader.Encoding = 1;
    pcxHeader.BitsPerLayer = 8;
    pcxHeader.Xmin = 0;
    pcxHeader.Ymin = 0;
    pcxHeader.Xmax = (ushort)width - 1;
    pcxHeader.Ymax = (ushort)height - 1;
    pcxHeader.HDpi = 300;
    pcxHeader.VDpi = 300;
    pcxHeader.NPlanes = 1;
    pcxHeader.BPL = width;
    WriteFile(hFile, &pcxHeader, sizeof( pcxHeader ), &NumberOfBytesWritten, 0);
    return NumberOfBytesWritten == sizeof( pcxHeader );
	#else
	throw std::exception(__FUNCTION__ " Deprecated.");
	#endif
}

// some PCX image writing-related stuff
//----- (004034E7) --------------------------------------------------------
bool __fastcall WritePaletteToPCX(HANDLE hFile, PALETTEENTRY* a2)
{
	#ifdef OLD_PCX_SCREENSHOT
	int v2;                     // eax@1
	int v3;                     // edx@1
	uchar* v4;                  // esi@1
	uchar Buffer[772];          // [sp+8h] [bp-308h]@1
	DWORD NumberOfBytesWritten; // [sp+30Ch] [bp-4h]@3
	v2 = (int)a2 + 2;
	Buffer[0] = 12;
	v4 = &Buffer[2];
	v3 = 256;
	do{
		*(v4 - 1) = *(uchar*)(v2 - 2);
		*v4 = *(uchar*)(v2 - 1);
		*(v4 + 1) = *(uchar*)v2;
		v2 += 4;
		v4 += 3;
		--v3;
	}while( v3 );
	WriteFile(hFile, Buffer, 0x301u, &NumberOfBytesWritten, 0);
	return NumberOfBytesWritten == 769;
	#else
	//
	// Nothing to do
	// This function is deprecated and will be removed eventually
	//
	throw std::exception(__FUNCTION__ " Deprecated. Not Implemented");
#endif
}

// PCX-body writing
//----- (00403550) --------------------------------------------------------
bool __fastcall PCXWriteRow(HANDLE hFile, int width, int height, long pitch, unsigned char* pixels)
{
	#ifdef OLD_PCX_SCREENSHOT
    DWORD NumberOfBytesWritten;
    unsigned char* encoded = (uchar*) malloc( 2 * width ); //allocating a row of pixels, 2 bytes per pixel for PCX format
    for( int row = 0; row < height; row++ ){
		int bytesToWrite = PCXConvertRow(pixels, encoded, width);
		BOOL writen = WriteFile(hFile, encoded, bytesToWrite, &NumberOfBytesWritten, 0);
		if( !writen || NumberOfBytesWritten != bytesToWrite ){
			return false;
		}
		pixels += pitch;
    }
    free( encoded );
    return true;
	#else
	// Nothing to do
	// This function is deprecated and will be removed eventually
	throw std::exception(__FUNCTION__ " Deprecated. Not Implemented");
#endif
}

//Run-length encoding raw pixels row to PCX format
//----- (004035C9) --------------------------------------------------------
int __fastcall PCXConvertRow(unsigned char* sourcePixels, unsigned char* destBuffer, long rowWidth)
{
	#ifdef OLD_PCX_SCREENSHOT
    int bytesToWrite = 0;
    unsigned char pixel = *sourcePixels++, prevPixel = pixel, countSamePixel = 1;
    for( int x = 2; x <= rowWidth; x++, prevPixel = pixel ){
		pixel = *sourcePixels++;
		if( pixel == prevPixel && countSamePixel < 63 ){
			countSamePixel++;
		}else{
			EncodePixel(countSamePixel, prevPixel, destBuffer, bytesToWrite);
			countSamePixel = 1;
		}
    }
    EncodePixel(countSamePixel, pixel, destBuffer, bytesToWrite);
    return bytesToWrite;
	#else
	//
	// Nothing to do
	// This function is deprecated and will be removed eventually
	//
	throw std::exception(__FUNCTION__ " Deprecated. Not Implemented");
#endif
}

//function creates a screen shot file (generates next name) an returns Win32 handle to created file (INVALID_HANDLE_VALUE for error)
//----- (00403605) --------------------------------------------------------
HANDLE __fastcall CreateScreenShot(char (&pOutputFilename)[MAX_PATH])
{
	#ifdef OLD_PCX_SCREENSHOT
    int v2;                // esi@1
    int snapshotIdx;       // eax@6
    __int32 v5;            // eax@1
    uchar Dst[10000];      // [sp+124h] [bp-64h]@1
    struct _finddata_t a2; // [sp+Ch] [bp-17Ch]@1
    memset(Dst, 0, sizeof( Dst ));
	v5 = _findfirst( ScreenshotFormat ? "THscrn????.gif" : "THscrn????.PCX", &a2 );
    v2 = v5;
    if( v5 != -1 ){
        do{
            if( isdigit(a2.name[6]) ){
                if( isdigit(a2.name[7]) ){
                    if( isdigit(a2.name[8]) ){
                        if( isdigit(a2.name[9]) ){
                            Dst[1000 * a2.name[6] - 48000 + 100 * a2.name[7] - 4800 + 10 * a2.name[8] - 480 + a2.name[9] - 48] = 1; //53328
                        }
                    }
                }
            }
        }while( !_findnext(v2, &a2) );
    }
    snapshotIdx = 0;
    while( Dst[snapshotIdx] ){
        ++snapshotIdx;
        if( snapshotIdx >= sizeof( Dst ) )
            return INVALID_HANDLE_VALUE;
    }
    sprintf(pOutputFilename, ScreenshotFormat ? "THscrn%04d.gif" : "THscrn%04d.PCX", snapshotIdx);
    if( ScreenshotFormat ){
		return 0;
	}else{
		return CreateFileA(pOutputFilename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	}
	#else
	throw std::exception(__FUNCTION__ " Deprecated");
	#endif
}

//----- (004036CF) --------------------------------------------------------
int __fastcall MakeScreenRedWhileSavingScreenshot(PALETTEENTRY* screenPalette)
{
#ifdef OLD_PCX_SCREENSHOT
    PALETTEENTRY redPal[256];
    for( int i = 0; i < 256; i++ ){
        redPal[i].peRed = screenPalette[i].peRed;
        redPal[i].peGreen = 0;
        redPal[i].peBlue = 0;
        redPal[i].peFlags = 0;
    }
    return DirectDrawPalettePtr->SetEntries(0, 0, 256, redPal);
#else
  //
  // Nothing to do
  // This function is deprecated and will be removed eventually
  //
  throw std::exception(__FUNCTION__ " Deprecated. Not Implemented");
#endif
}

// loaded from ui_art\mainmenz.pcx (dragon)
uchar MorMenuPal[1024] = {
    0xff, 0xff, 0xff, 0, 0x00, 0xff, 0xff, 0, 0xff, 0x00, 0xff, 0, 0x00, 0x00, 0xff, 0,
    0xff, 0xff, 0x00, 0, 0x00, 0xff, 0x00, 0, 0xff, 0x00, 0x00, 0, 0x80, 0x80, 0x80, 0,
    0xa0, 0xa0, 0xa4, 0, 0xc1, 0xcc, 0xd9, 0, 0x00, 0x00, 0x00, 0, 0x08, 0x00, 0x00, 0,
    0x08, 0x00, 0x08, 0, 0x00, 0x08, 0x00, 0, 0x12, 0x00, 0x00, 0, 0x00, 0x08, 0x08, 0,
    0x00, 0x00, 0x00, 0, 0x11, 0x11, 0x11, 0, 0x1e, 0x1e, 0x1e, 0, 0x2e, 0x2e, 0x2e, 0,
    0x3d, 0x3d, 0x3d, 0, 0x4c, 0x4c, 0x4c, 0, 0x59, 0x59, 0x59, 0, 0x66, 0x66, 0x66, 0,
    0x73, 0x73, 0x73, 0, 0x85, 0x85, 0x85, 0, 0x94, 0x94, 0x94, 0, 0xa3, 0xa3, 0xa3, 0,
    0xb8, 0xb8, 0xb8, 0, 0xcc, 0xcc, 0xcc, 0, 0xde, 0xde, 0xde, 0, 0xf3, 0xf3, 0xf3, 0,
    0x0c, 0x05, 0x05, 0, 0x23, 0x09, 0x09, 0, 0x39, 0x0d, 0x0d, 0, 0x4f, 0x11, 0x11, 0,
    0x66, 0x15, 0x15, 0, 0x7c, 0x19, 0x19, 0, 0x93, 0x1e, 0x1e, 0, 0xa9, 0x22, 0x22, 0,
    0xbf, 0x27, 0x27, 0, 0xc7, 0x38, 0x38, 0, 0xcf, 0x49, 0x49, 0, 0xd8, 0x5b, 0x5b, 0,
    0xe0, 0x6c, 0x6c, 0, 0xe8, 0x7d, 0x7d, 0, 0xf4, 0x96, 0x96, 0, 0xff, 0xbd, 0xbd, 0,
    0x0f, 0x05, 0x00, 0, 0x25, 0x0e, 0x03, 0, 0x3f, 0x17, 0x08, 0, 0x5a, 0x22, 0x0c, 0,
    0x6f, 0x29, 0x10, 0, 0x85, 0x32, 0x13, 0, 0x9b, 0x3b, 0x18, 0, 0xb1, 0x43, 0x1b, 0,
    0xc7, 0x4b, 0x1f, 0, 0xcc, 0x61, 0x33, 0, 0xc7, 0x7b, 0x52, 0, 0xd0, 0x8c, 0x62, 0,
    0xdc, 0x9f, 0x70, 0, 0xe7, 0xb3, 0x7e, 0, 0xf4, 0xc9, 0x96, 0, 0xff, 0xe2, 0xb3, 0,
    0x14, 0x0b, 0x00, 0, 0x1a, 0x14, 0x08, 0, 0x31, 0x28, 0x16, 0, 0x39, 0x31, 0x1d, 0,
    0x48, 0x40, 0x27, 0, 0x5b, 0x51, 0x34, 0, 0x69, 0x60, 0x3f, 0, 0x78, 0x6f, 0x49, 0,
    0x87, 0x7e, 0x54, 0, 0x98, 0x8b, 0x5d, 0, 0xab, 0x9a, 0x63, 0, 0xbc, 0xa8, 0x6c, 0,
    0xcc, 0xb7, 0x75, 0, 0xdd, 0xc4, 0x7e, 0, 0xee, 0xd1, 0x8c, 0, 0xff, 0xe3, 0xa4, 0,
    0x05, 0x07, 0x0c, 0, 0x0d, 0x11, 0x1b, 0, 0x19, 0x1e, 0x2d, 0, 0x25, 0x2b, 0x41, 0,
    0x2f, 0x36, 0x50, 0, 0x39, 0x41, 0x5f, 0, 0x43, 0x4c, 0x6f, 0, 0x4e, 0x58, 0x7d, 0,
    0x58, 0x63, 0x8d, 0, 0x66, 0x70, 0x99, 0, 0x79, 0x7f, 0xa0, 0, 0x87, 0x8d, 0xac, 0,
    0x93, 0x99, 0xb9, 0, 0x9f, 0xa5, 0xc6, 0, 0xb2, 0xb7, 0xd7, 0, 0xc8, 0xcd, 0xea, 0,
    0x0c, 0x07, 0x07, 0, 0x1b, 0x0e, 0x0e, 0, 0x33, 0x19, 0x19, 0, 0x44, 0x21, 0x21, 0,
    0x56, 0x29, 0x29, 0, 0x68, 0x31, 0x31, 0, 0x79, 0x39, 0x39, 0, 0x8b, 0x41, 0x41, 0,
    0x9c, 0x49, 0x49, 0, 0xa5, 0x5a, 0x5a, 0, 0xa8, 0x71, 0x71, 0, 0xb3, 0x80, 0x80, 0,
    0xbd, 0x8f, 0x8f, 0, 0xca, 0x9e, 0x9e, 0, 0xd7, 0xb2, 0xb2, 0, 0xe8, 0xca, 0xca, 0,
    0x1e, 0x07, 0x00, 0, 0x57, 0x1a, 0x00, 0, 0x8f, 0x2e, 0x00, 0, 0xc7, 0x41, 0x00, 0,
    0xf0, 0x4c, 0x00, 0, 0xfe, 0x69, 0x24, 0, 0xff, 0x8c, 0x57, 0, 0xfe, 0xbe, 0xa0, 0,
    0x19, 0x19, 0x00, 0, 0x57, 0x55, 0x00, 0, 0x86, 0x86, 0x00, 0, 0xc3, 0xc3, 0x00, 0,
    0xf0, 0xec, 0x00, 0, 0xfe, 0xfb, 0x24, 0, 0xff, 0xfc, 0x57, 0, 0xff, 0xfd, 0x9f, 0,
    0x23, 0x00, 0x00, 0, 0x5a, 0x00, 0x00, 0, 0x91, 0x00, 0x00, 0, 0xbd, 0x00, 0x00, 0,
    0xf0, 0x00, 0x00, 0, 0xfe, 0x24, 0x24, 0, 0xff, 0x57, 0x57, 0, 0xff, 0x9f, 0x9f, 0,
    0x00, 0x00, 0x19, 0, 0x00, 0x00, 0x57, 0, 0x00, 0x00, 0x8a, 0, 0x00, 0x00, 0xbd, 0,
    0x01, 0x01, 0xef, 0, 0x24, 0x24, 0xfe, 0, 0x57, 0x57, 0xff, 0, 0x9f, 0x9f, 0xff, 0,
    0x6c, 0x6c, 0x68, 0, 0x64, 0x64, 0x60, 0, 0x60, 0x60, 0x5c, 0, 0x5c, 0x5c, 0x58, 0,
    0x58, 0x58, 0x54, 0, 0x54, 0x54, 0x50, 0, 0x50, 0x50, 0x48, 0, 0x4c, 0x4c, 0x48, 0,
    0x48, 0x48, 0x44, 0, 0x44, 0x44, 0x40, 0, 0x40, 0x40, 0x3c, 0, 0x40, 0x40, 0x38, 0,
    0x40, 0x3c, 0x38, 0, 0x3c, 0x3c, 0x38, 0, 0x3c, 0x3c, 0x34, 0, 0x3c, 0x38, 0x34, 0,
    0x38, 0x38, 0x34, 0, 0x38, 0x38, 0x30, 0, 0x38, 0x34, 0x30, 0, 0x34, 0x34, 0x30, 0,
    0x34, 0x34, 0x2c, 0, 0x34, 0x30, 0x2c, 0, 0x34, 0x30, 0x28, 0, 0x30, 0x30, 0x2c, 0,
    0x30, 0x30, 0x28, 0, 0x30, 0x2c, 0x28, 0, 0x2c, 0x2c, 0x28, 0, 0x2c, 0x2c, 0x24, 0,
    0x2c, 0x28, 0x24, 0, 0x2c, 0x28, 0x20, 0, 0x28, 0x28, 0x24, 0, 0x28, 0x28, 0x20, 0,
    0x28, 0x24, 0x20, 0, 0x28, 0x24, 0x1c, 0, 0x24, 0x24, 0x20, 0, 0x24, 0x24, 0x1c, 0,
    0x24, 0x24, 0x18, 0, 0x24, 0x20, 0x1c, 0, 0x24, 0x20, 0x18, 0, 0x24, 0x20, 0x14, 0,
    0x20, 0x20, 0x18, 0, 0x20, 0x1c, 0x18, 0, 0x20, 0x1c, 0x14, 0, 0x20, 0x20, 0x0c, 0,
    0x20, 0x1c, 0x10, 0, 0x1c, 0x1c, 0x14, 0, 0x1c, 0x1c, 0x10, 0, 0x18, 0x1c, 0x14, 0,
    0x1c, 0x18, 0x14, 0, 0x1c, 0x1c, 0x08, 0, 0x1c, 0x18, 0x10, 0, 0x18, 0x18, 0x14, 0,
    0x18, 0x18, 0x10, 0, 0x14, 0x18, 0x10, 0, 0x18, 0x14, 0x10, 0, 0x18, 0x18, 0x08, 0,
    0x14, 0x14, 0x10, 0, 0x14, 0x14, 0x0c, 0, 0x10, 0x14, 0x10, 0, 0x14, 0x14, 0x08, 0,
    0x14, 0x10, 0x0c, 0, 0x10, 0x10, 0x0c, 0, 0x10, 0x10, 0x08, 0, 0x0c, 0x10, 0x0c, 0,
    0x0c, 0x0c, 0x0c, 0, 0x0c, 0x0c, 0x08, 0, 0x0c, 0x0c, 0x04, 0, 0x08, 0x08, 0x08, 0,
    0x04, 0x08, 0x08, 0, 0x08, 0x08, 0x04, 0, 0x04, 0x04, 0x04, 0, 0x08, 0x0c, 0x08, 0,
    0x04, 0x08, 0x04, 0, 0x28, 0x20, 0x14, 0, 0x20, 0x18, 0x10, 0, 0x18, 0x14, 0x0c, 0,
    0x14, 0x10, 0x08, 0, 0x10, 0x0c, 0x08, 0, 0x0c, 0x08, 0x08, 0, 0x08, 0x04, 0x04, 0,
    0xbf, 0x26, 0x0c, 0, 0x89, 0x3e, 0x07, 0, 0xa9, 0x1b, 0x08, 0, 0x82, 0x2e, 0x05, 0,
    0x98, 0x17, 0x05, 0, 0x6a, 0x30, 0x08, 0, 0x84, 0x16, 0x04, 0, 0x82, 0x06, 0x02, 0,
    0x64, 0x1d, 0x01, 0, 0x6c, 0x0a, 0x01, 0, 0x41, 0x1e, 0x07, 0, 0x61, 0x00, 0x00, 0,
    0x49, 0x0c, 0x00, 0, 0x37, 0x00, 0x00, 0, 0x28, 0x0c, 0x01, 0, 0x11, 0x0a, 0x03, 0,
    0xe1, 0x95, 0x2d, 0, 0xc6, 0x6a, 0x24, 0, 0xb3, 0x55, 0x17, 0, 0xbe, 0x43, 0x0d, 0,
    0xa0, 0x44, 0x0f, 0, 0x8d, 0x44, 0x13, 0, 0x83, 0x99, 0xb1, 0, 0x4f, 0x65, 0x7d, 0,
    0xc0, 0xc0, 0xc0, 0, 0x00, 0x80, 0x80, 0, 0x80, 0x00, 0x80, 0, 0x00, 0x00, 0x80, 0,
    0x80, 0x80, 0x00, 0, 0x00, 0x80, 0x00, 0, 0x80, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0,
};

// load from original ex main menu screenshot
uchar MenuPal[1024] = { 
    0xFF, 0xFF, 0xFF, 0, 0x00, 0xFF, 0xFF, 0, 0xFF, 0x00, 0xFF, 0, 0x00, 0x00, 0xFF, 0, 0xFF, 0xFF, 0x00, 0, 0x00, 0xFF, 0x00, 0, 0xFF, 0x00, 0x00, 0, 0x80, 0x80, 0x80, 0,
    0xA0, 0xA0, 0xA4, 0, 0xFF, 0xFB, 0xF0, 0, 0x00, 0x00, 0x00, 0, 0x08, 0x00, 0x00, 0, 0x08, 0x00, 0x08, 0, 0x00, 0x08, 0x00, 0, 0x12, 0x00, 0x00, 0, 0x00, 0x08, 0x08, 0,
    0x00, 0x00, 0x00, 0, 0x11, 0x11, 0x11, 0, 0x1E, 0x1E, 0x1E, 0, 0x2E, 0x2E, 0x2E, 0, 0x3D, 0x3D, 0x3D, 0, 0x59, 0x59, 0x59, 0, 0x66, 0x66, 0x66, 0, 0x73, 0x73, 0x73, 0,
    0x85, 0x85, 0x85, 0, 0x94, 0x94, 0x94, 0, 0xA3, 0xA3, 0xA3, 0, 0xB8, 0xB8, 0xB8, 0, 0xCC, 0xCC, 0xCC, 0, 0xDE, 0xDE, 0xDE, 0, 0xF3, 0xF3, 0xF3, 0, 0xF3, 0xF3, 0xF3, 0,
    0x0C, 0x05, 0x05, 0, 0x23, 0x09, 0x09, 0, 0x39, 0x0D, 0x0D, 0, 0x4F, 0x11, 0x11, 0, 0x66, 0x15, 0x15, 0, 0x7C, 0x19, 0x19, 0, 0x93, 0x1E, 0x1E, 0, 0xA9, 0x22, 0x22, 0,
    0xBF, 0x27, 0x27, 0, 0xC7, 0x38, 0x38, 0, 0xCF, 0x49, 0x49, 0, 0xD8, 0x5B, 0x5B, 0, 0xE0, 0x6C, 0x6C, 0, 0xE8, 0x7D, 0x7D, 0, 0xF4, 0x96, 0x96, 0, 0xFF, 0xBD, 0xBD, 0,
    0x0F, 0x05, 0x00, 0, 0x25, 0x0E, 0x03, 0, 0x3F, 0x17, 0x08, 0, 0x5A, 0x22, 0x0C, 0, 0x6F, 0x29, 0x10, 0, 0x85, 0x32, 0x13, 0, 0x9B, 0x3B, 0x18, 0, 0xB1, 0x43, 0x1B, 0,
    0xC7, 0x4B, 0x1F, 0, 0xCC, 0x61, 0x33, 0, 0xC7, 0x7B, 0x52, 0, 0xD0, 0x8C, 0x62, 0, 0xDC, 0x9F, 0x70, 0, 0xE7, 0xB3, 0x7E, 0, 0xF4, 0xC9, 0x96, 0, 0xFF, 0xE2, 0xB3, 0,
    0x14, 0x0B, 0x00, 0, 0x1A, 0x14, 0x08, 0, 0x31, 0x28, 0x16, 0, 0x39, 0x31, 0x1D, 0, 0x48, 0x40, 0x27, 0, 0x5B, 0x51, 0x34, 0, 0x69, 0x60, 0x3F, 0, 0x78, 0x6F, 0x49, 0,
    0x87, 0x7E, 0x54, 0, 0x98, 0x8B, 0x5D, 0, 0xAB, 0x9A, 0x63, 0, 0xBC, 0xA8, 0x6C, 0, 0xCC, 0xB7, 0x75, 0, 0xDD, 0xC4, 0x7E, 0, 0xEE, 0xD1, 0x8C, 0, 0xFF, 0xE3, 0xA4, 0,
    0x05, 0x07, 0x0C, 0, 0x0D, 0x11, 0x1B, 0, 0x19, 0x1E, 0x2D, 0, 0x25, 0x2B, 0x41, 0, 0x2F, 0x36, 0x50, 0, 0x39, 0x41, 0x5F, 0, 0x43, 0x4C, 0x6F, 0, 0x4E, 0x58, 0x7D, 0,
    0x58, 0x63, 0x8D, 0, 0x66, 0x70, 0x99, 0, 0x79, 0x7F, 0xA0, 0, 0x87, 0x8D, 0xAC, 0, 0x93, 0x99, 0xB9, 0, 0x9F, 0xA5, 0xC6, 0, 0xB2, 0xB7, 0xD7, 0, 0xC8, 0xCD, 0xEA, 0,
    0x0C, 0x07, 0x07, 0, 0x1B, 0x0E, 0x0E, 0, 0x33, 0x19, 0x19, 0, 0x44, 0x21, 0x21, 0, 0x56, 0x29, 0x29, 0, 0x68, 0x31, 0x31, 0, 0x79, 0x39, 0x39, 0, 0x8B, 0x41, 0x41, 0,
    0x9C, 0x49, 0x49, 0, 0xA5, 0x5A, 0x5A, 0, 0xAB, 0x77, 0x73, 0, 0xB3, 0x80, 0x80, 0, 0xBD, 0x8F, 0x8F, 0, 0xCA, 0x9E, 0x9E, 0, 0xD7, 0xB2, 0xB2, 0, 0xE8, 0xCA, 0xCA, 0,
    0x1E, 0x07, 0x00, 0, 0x57, 0x1A, 0x00, 0, 0x8F, 0x2E, 0x00, 0, 0xC7, 0x41, 0x00, 0, 0xF0, 0x4C, 0x00, 0, 0xFE, 0x69, 0x24, 0, 0xFF, 0x8C, 0x57, 0, 0xFE, 0xBE, 0xA0, 0,
    0x19, 0x19, 0x00, 0, 0x57, 0x55, 0x00, 0, 0x86, 0x86, 0x00, 0, 0xC3, 0xC3, 0x00, 0,	0xF0, 0xEC, 0x00, 0, 0xFE, 0xFB, 0x24, 0, 0xFF, 0xFC, 0x57, 0, 0xFF, 0xFD, 0x9F, 0,
    0x23, 0x00, 0x00, 0, 0x5A, 0x00, 0x00, 0, 0x91, 0x00, 0x00, 0, 0xBD, 0x00, 0x00, 0, 0xF0, 0x00, 0x00, 0, 0xFE, 0x24, 0x24, 0, 0xFF, 0x57, 0x57, 0, 0xFF, 0x9F, 0x9F, 0,
    0x00, 0x00, 0x19, 0, 0x00, 0x00, 0x57, 0, 0x00, 0x00, 0x8A, 0, 0x00, 0x00, 0xBD, 0, 0x01, 0x01, 0xEF, 0, 0x24, 0x24, 0xFE, 0, 0x57, 0x57, 0xFF, 0, 0x9F, 0x9F, 0xFF, 0,
    0x30, 0x28, 0x24, 0, 0x2C, 0x24, 0x1C, 0, 0x28, 0x20, 0x1C, 0, 0x20, 0x1C, 0x18, 0, 0x84, 0x60, 0x4C, 0, 0x7C, 0x5C, 0x48, 0, 0x74, 0x54, 0x44, 0, 0x6C, 0x50, 0x3C, 0,
    0x6C, 0x4C, 0x38, 0, 0x64, 0x48, 0x34, 0, 0x60, 0x44, 0x30, 0, 0x5C, 0x40, 0x30, 0, 0x50, 0x38, 0x2C, 0, 0x48, 0x34, 0x24, 0, 0x3C, 0x2C, 0x24, 0, 0x00, 0x00, 0xD7, 0,
    0x2B, 0x17, 0x17, 0, 0x17, 0x0B, 0x0B, 0, 0x00, 0x00, 0xA3, 0, 0x00, 0x00, 0x6F, 0, 0x00, 0x00, 0x43, 0, 0x0F, 0x0B, 0x00, 0, 0x2B, 0x0B, 0x00, 0, 0x00, 0x00, 0x2B, 0,
    0x00, 0x00, 0x4F, 0, 0x00, 0x00, 0x37, 0, 0x00, 0x00, 0x2F, 0, 0x0F, 0x0F, 0x0F, 0, 0x00, 0x00, 0x27, 0, 0x6B, 0x43, 0x3B, 0, 0x53, 0x37, 0x2F, 0, 0x6F, 0x43, 0x3B, 0,
    0x50, 0x24, 0x20, 0, 0x50, 0x1C, 0x18, 0, 0x48, 0x18, 0x14, 0, 0x38, 0x1C, 0x18, 0, 0x3C, 0x14, 0x10, 0, 0x34, 0x10, 0x10, 0, 0x2C, 0x10, 0x0C, 0, 0x2C, 0x0C, 0x0C, 0,
    0x28, 0x0C, 0x0C, 0, 0x24, 0x14, 0x0C, 0, 0x20, 0x10, 0x08, 0, 0x1C, 0x10, 0x08, 0, 0x18, 0x0C, 0x08, 0, 0x14, 0x0C, 0x04, 0, 0x10, 0x08, 0x04, 0, 0x0C, 0x08, 0x04, 0,
    0x00, 0xCC, 0x00, 0, 0x00, 0xB8, 0x00, 0, 0x00, 0xAC, 0x00, 0, 0x00, 0xA4, 0x00, 0, 0x00, 0x90, 0x00, 0, 0x00, 0x78, 0x00, 0, 0x00, 0x6C, 0x00, 0, 0x00, 0x64, 0x00, 0,
    0x40, 0x40, 0x3C, 0, 0x3C, 0x3C, 0x38, 0, 0x34, 0x34, 0x30, 0, 0x2C, 0x2C, 0x28, 0, 0x24, 0x28, 0x24, 0, 0x20, 0x24, 0x20, 0, 0x1C, 0x24, 0x1C, 0, 0x18, 0x1C, 0x18, 0,
    0xF0, 0x9C, 0x88, 0, 0xE4, 0x94, 0x80, 0, 0xD8, 0x8C, 0x7C, 0, 0xD4, 0x88, 0x78, 0, 0xCC, 0x84, 0x74, 0, 0xC0, 0x7C, 0x6C, 0, 0xAC, 0x70, 0x60, 0, 0x9C, 0x64, 0x58, 0,
    0x90, 0x5C, 0x50, 0, 0x88, 0x58, 0x4C, 0, 0x7C, 0x50, 0x44, 0, 0x74, 0x48, 0x40, 0, 0x5C, 0x3C, 0x34, 0, 0x48, 0x2C, 0x28, 0, 0x34, 0x20, 0x1C, 0, 0x2C, 0x1C, 0x18, 0,
    0xBF, 0x26, 0x0C, 0, 0x89, 0x3E, 0x07, 0, 0xA9, 0x1B, 0x08, 0, 0x82, 0x2E, 0x05, 0, 0x98, 0x17, 0x05, 0, 0x6A, 0x30, 0x08, 0, 0x84, 0x16, 0x04, 0, 0x82, 0x06, 0x02, 0,
    0x64, 0x1D, 0x01, 0, 0x6C, 0x0A, 0x01, 0, 0x41, 0x1E, 0x07, 0, 0x61, 0x00, 0x00, 0, 0x49, 0x0C, 0x00, 0, 0x37, 0x00, 0x00, 0, 0x28, 0x0C, 0x01, 0, 0x11, 0x0A, 0x03, 0,
    0xE1, 0x95, 0x2D, 0, 0xC6, 0x6A, 0x24, 0, 0xB3, 0x55, 0x17, 0, 0xBE, 0x43, 0x0D, 0, 0xA0, 0x44, 0x0F, 0, 0x8D, 0x44, 0x13, 0, 0xEC, 0xE9, 0xD8, 0, 0xAC, 0xA8, 0x99, 0,
    0xC0, 0xC0, 0xC0, 0, 0x00, 0x80, 0x80, 0, 0x80, 0x00, 0x80, 0, 0x00, 0x00, 0x80, 0, 0x80, 0x80, 0x00, 0, 0x00, 0x80, 0x00, 0, 0x80, 0x00, 0x00, 0, 0x00, 0x00, 0x00, 0
};

//----- (th2) -------------------------------------------------------------
void LoadDragonMenuPalette()
{
	PALETTEENTRY* menuPal = (PALETTEENTRY*)MorMenuPal;
    for( int i = 0; i < 256; i++ ){
        ScreenPalette[i].peBlue = menuPal[255 - i].peBlue;
        ScreenPalette[i].peGreen = menuPal[255 - i].peGreen;
        ScreenPalette[i].peRed = menuPal[255 - i].peRed;
    }
}

//----- (th2) -------------------------------------------------------------
void LoadMainMenuPalette()
{
	PALETTEENTRY* menuPal = (PALETTEENTRY*)MenuPal;
    for( int i = 0; i < 256; i++ ){
        ScreenPalette[i].peBlue = menuPal[255 - i].peBlue;
        ScreenPalette[i].peGreen = menuPal[255 - i].peGreen;
        ScreenPalette[i].peRed = menuPal[255 - i].peRed;
    }
}

//----- (th2) -------------------------------------------------------------
void __fastcall AdjustDDrawWindow( HWND hwnd )
{
	RECT r;
	GetWindowRect( hwnd, &r );
	int x = r.left;
	int y = r.top;
	AdjustWindowRectEx( &r, GetWindowLong( hwnd, GWL_STYLE ), GetMenu( hwnd ) != NULL, GetWindowLong( hwnd, GWL_EXSTYLE ) );
	MoveWindow( hwnd, x, y, r.right - r.left, r.bottom - r.top, true );
	// SetFocus(hwnd);// keep the mouse from leaving the window.
	// ClipCursor(&client);
}

//----- (th2) -------------------------------------------------------------
template <typename T, int redMax, int greenMax, int blueMax, int greenPos, int redPos> //__forceinline
void drawPixels( int left, int top, uchar* src, void* dst, int dstPitch, int srcPitch, int width, int height, PALETTEENTRY* screenPalette  )
{
	T pal[ 256 ];
	for( int i = 0; i < 256; i++ ){
		pal[ i ] = (T) ((float) screenPalette[ i ].peBlue / 256.0*blueMax) + ((T) ((float) screenPalette[ i ].peGreen / 256.0*greenMax) << greenPos) + ((T) ((float) screenPalette[ i ].peRed / 256.0*redMax) << redPos);
	}
	uchar* pixel = src + left + top * width;
	T* pixel32 = (T*) dst + left + top * dstPitch / sizeof( T );
	int srcDelta = srcPitch - width;
	for( int y = 0; y < height; ++y ){
		for( int x = 0; x < width; ++x ){
			*pixel32++ = pal[ *pixel++ ];
		}
		pixel += srcDelta;
		*(char**)&pixel32 += dstPitch - width * sizeof(T);
	}
}

bool AltEnterPressed = false;
extern bool MainMainMenu;
int OldWorldProcess = 0;

//----- (th2) -------------------------------------------------------------
bool AltEnter()
{
	if( IsSecondWin /*|| VideoWindowExists*/ ) return false;
	int isAltPressed = GetAsyncKeyState( VK_MENU ) < 0;
	if (ExtraMouseButtons) {
		isAltPressed |= GetAsyncKeyState(VK_XBUTTON1) < 0;
	}
	IsAltPressed = isAltPressed;
	bool enterPressed = GetAsyncKeyState( VK_RETURN ) < 0;
	if( AltEnterPressed && ! IsAltPressed && ! enterPressed ){
		AltEnterPressed = false;
		if( gbRunGame /*|| MainMainMenu*/ ){
			if( !IsFullScreen ){
				CursorOff();
			}
			IsFullScreen = ! IsFullScreen;
			return true;
		}
	}else if(! AltEnterPressed && IsAltPressed && enterPressed){
		AltEnterPressed = true;
	}
	if( OldWorldProcess != gbRunGame ){
		OldWorldProcess = gbRunGame;
		if( gbRunGame ){
			POINT p;
			GetCursorPos( &p );
			SetCursorPos( p.x + (ScreenWidth - GUI_Width)/2 * WinWidth / ScreenWidth, p.y + (ScreenHeight - GUI_Height)/2 * WinHeight / ScreenHeight );
			ScreenToClient( ghMainWnd, &p );
			CursorX = p.x + (ScreenWidth - GUI_Width) / 2;
			CursorY = p.y + (ScreenHeight - GUI_Height) / 2;
		}else{
			POINT c{ 0, 0 };
			ClientToScreen( ghMainWnd, &c );
			SetCursorPos( c.x + GUI_Width/2  * WinWidth / ScreenWidth, c.y + GUI_Height/2 * WinHeight / ScreenHeight );
		}
		if( IsFullScreen && ! BigWin ){
			return true;
		}
	}
	return false;
}

#define PS_PALETTE
int PrevWinWidth, PrevWinHeight;

bool OnlySecondWin = false;
//----- (th2) -------------------------------------------------------------
void SwitchFullscreen( bool forceWindowed /*= false*/ )
{
	if( forceWindowed ){
		if( IsFullScreen ){
			IsFullScreen = false;
		}else{
			return;
		}
	}
	int width = WinWidth, height = WinHeight;
	if( IsFullScreen ){
		PrevWinWidth = WinWidth;
		PrevWinHeight = WinHeight;
	buggy_res:
		width = BigWin ? GetSystemMetrics( SM_CXSCREEN ) : ScreenWidth;
		height = BigWin ? GetSystemMetrics( SM_CYSCREEN ) : ScreenHeight;
	} else{
		width = PrevWinWidth ? PrevWinWidth : ScreenWidth;
		height = PrevWinHeight ? PrevWinHeight : ScreenHeight;
	}
	RECT r = { 0, 0, width - 1, height - 1 };
	if( !IsFullScreen ){
		AdjustWindowRectEx( &r, WS_OVERLAPPEDWINDOW, 0, 0 );
	}
	SetWindowLongPtr( ghMainWnd, GWL_STYLE, IsFullScreen ? WS_POPUP : WS_OVERLAPPEDWINDOW );
	SetWindowPos( ghMainWnd, HWND_NOTOPMOST, IsFullScreen ? 0 : StartWindowX, IsFullScreen ? 0 : StartWindowY,
		r.right - r.left + 1, r.bottom - r.top + 1, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOSENDCHANGING );
	if( !D3dReset() ){
		if( IsFullScreen && !BigWin ){ // обработка кривых разрешений пытающихся в fullscreen
			BigWin = true;
			goto buggy_res;
		}
	}
	if( IsFullScreen ){
		CursorOff();
	}
	PrevRestrictState = false;
}

//----- (th2) -------------------------------------------------------------
int RenderQuadTexture( LPDIRECT3DDEVICE9 dev, LPDIRECT3DTEXTURE9 tex, LPDIRECT3DTEXTURE9 pal, LPDIRECT3DSURFACE9 vid, uchar* src )
{
	D3DLOCKED_RECT rect;
	if( D3D_OK != tex->LockRect( 0, &rect, 0, 0 ) ) return 0;
	// обновляем палитровую текстуру
	int dstPitch = rect.Pitch;
	int srcPitch = ( !gbRunGame && !LoadingScreenCEL /*Handler1 != LoadingWindowHandler*/ ) ? ScreenWidth : WorkingWidth;
	#if defined(MORE_COLOR)
	ushort* dst = (ushort*) rect.pBits;
	for( int row = 0; row < ScreenHeight; ++row ){
		for( int col = 0; col < ScreenWidth; ++col ){
			*dst++ = *src++;
		}
		dst += dstPitch/sizeof(*dst) - ScreenWidth;
		src += srcPitch - ScreenWidth;
	}
	#else
	uchar* dst = (uchar*) rect.pBits;
	for( int row = 0; row < ScreenHeight; ++row ){
		memcpy( dst, src, ScreenWidth );
		dst += dstPitch;
		src += srcPitch;
	}
	#endif
	tex->UnlockRect( 0 );
	if( D3D_OK != pal->LockRect( 0, &rect, 0, 0 ) ) return 0; // обновляем палитровую 
	uint* dstPal = (uint*) rect.pBits;
	PALETTEENTRY* srcPal = ScreenPalette;
	for( int i = 0; i < 256; ++i ){
		*dstPal++ = srcPal->peBlue + (srcPal->peGreen << 8) + (srcPal->peRed << 16);
		++srcPal;
	}
	pal->UnlockRect( 0 );
	//D3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 255 ), 1.0f, 0 );
	D3DSURFACE_DESC desc;
	LPDIRECT3DSURFACE9 pSurf = 0;
	if( D3D_OK == dev->GetRenderTarget( 0, &pSurf ) ){
		if( D3D_OK != pSurf->GetDesc( &desc ) ){ pSurf->Release(); return 0; }
		bool useRenderTexture = FilteringMode != D3DTEXF_NONE && (desc.Width != ScreenWidth || desc.Height != ScreenHeight);
		float width = (float) (useRenderTexture ? ScreenWidth : desc.Width) - 0.5f;
		float height = (float) (useRenderTexture ? ScreenHeight : desc.Height) - 0.5f;
		TransformedVertex QuadVertex[4] = {
			{ {  -0.5,   -0.5, 0, 1 }, { 0, 0 } },
			{ { width,   -0.5, 0, 1 }, { 1, 0 } },
			{ {  -0.5, height, 0, 1 }, { 0, 1 } },
			{ { width, height, 0, 1 }, { 1, 1 } }
		};
		if( useRenderTexture ){
			if( D3D_OK != dev->SetRenderTarget( 0, vid ) ){ pSurf->Release(); return 0; }
		}
		if( D3D_OK != dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 ) ){ pSurf->Release(); return 0; }
		if( D3D_OK != dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, QuadVertex, sizeof( TransformedVertex ) ) ){ pSurf->Release(); return 0; }

		if( gbRunGame == 1 && AutomapMode && MapBlend ){
			if( D3D_OK != dev->SetPixelShader( DirectShader ) ){ pSurf->Release(); return 0; }
			if( D3D_OK != dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE ) ){ pSurf->Release(); return 0; }
			if( D3D_OK != dev->DrawPrimitiveUP( D3DPT_LINELIST, AutomapLinePointsCount/2, &AutomapLines, sizeof( line_vertex ) ) ){ pSurf->Release(); return 0; }
			if( D3D_OK != dev->DrawPrimitiveUP( D3DPT_POINTLIST, AutomapPointsCount, &AutomapPoints, sizeof( line_vertex ) ) ){ pSurf->Release(); return 0; }
			if( D3D_OK != dev->SetPixelShader( PaletteShader ) ){ pSurf->Release(); return 0; }
		}

		if( useRenderTexture ){
			if( D3D_OK != dev->SetRenderTarget( 0, pSurf ) ){ pSurf->Release(); return 0; }
			if( D3D_OK != dev->StretchRect( vid, 0, pSurf, 0, FilteringMode ) ){ pSurf->Release(); return 0; }
		}
	}else{
		if( pSurf ) pSurf->Release(); return 0; 
	}
	if( pSurf ) pSurf->Release();
	return 1;
}

//----- (th2) -------------------------------------------------------------
int RenderSurface( LPDIRECT3DDEVICE9 dev, LPDIRECT3DSURFACE9 vid, LPDIRECT3DSURFACE9 sur, LPDIRECT3DSURFACE9 buf, int startingX, int startingY, uchar* src, int pixelFormat, int srcPitch, int srcWidth, int srcHeight, PALETTEENTRY* screenPalette, RECT& srcRect, RECT& dstRect )
{
	int dstPitch = 0;
	void* dst = 0;
	D3DLOCKED_RECT rect;
	if( D3D_OK != sur->LockRect( &rect, 0, 0 ) ) return 0;
	dstPitch = rect.Pitch;
	dst = rect.pBits;
	switch( pixelFormat ){
	case 15: drawPixels<ushort, 32, 32,  32, 5, 10>( startingX, startingY, src, dst, dstPitch, srcPitch, srcWidth, srcHeight, screenPalette ); break;
	case 16: drawPixels<ushort, 32, 64,  32, 5, 11>( startingX, startingY, src, dst, dstPitch, srcPitch, srcWidth, srcHeight, screenPalette ); break;
	case 32: drawPixels<uint, 256, 256, 256, 8, 16>( startingX, startingY, src, dst, dstPitch, srcPitch, srcWidth, srcHeight, screenPalette ); break;
	}
	sur->UnlockRect();
	if( !gbRunGame ){
		if( D3D_OK != dev->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 /*a++*/ ), 1.0f, 0 ) ) return 0;
	}
	if( !gbRunGame && !VideoWindowExists ){ // главное меню
		if( D3D_OK != dev->UpdateSurface( sur, &srcRect, vid, 0 ) ) return 0;
		if( D3D_OK != dev->StretchRect( vid, gbRunGame ? 0 : &srcRect, buf, gbRunGame ? 0 : &dstRect, FilteringMode ) ) return 0;
	} else if( VideoWindowExists || LoadingScreenCEL ){ // видео и загрузочный экран
		if( D3D_OK != dev->UpdateSurface( sur, 0, vid, 0 ) ) return 0;
		if( D3D_OK != dev->StretchRect( vid, &srcRect, buf, 0, FilteringMode )  ) return 0;
	} else{  // геймплей в режиме поверхности
		if( D3D_OK != dev->UpdateSurface( sur, 0, vid, 0 ) ) return 0;
		if( D3D_OK != dev->StretchRect( vid, 0, buf, 0, FilteringMode ) ) return 0;
	}
	return 1;
}

//----- (th2) -------------------------------------------------------------
LPDIRECTDRAWSURFACE __fastcall ConvertPaletteSurfaceToTrueColor(uchar* src)
{
	if( (uint)src < 0x000'0100u ){
		//__debugbreak();
		return ScreenSurface;
	}
	PALETTEENTRY* screenPalette = gbRunGame && ! VideoWindowExists ? ScreenPalette : (PALETTEENTRY*) (StormReloc + 0x1505E670 /*0x15039B14*/ ); // рабочая палитра storm 1611
	int pixelFormat = D3dRef ? 16 : 32;
	int width = IsFullScreen && !BigWin ? ScreenWidth : WinWidth;
	int height = IsFullScreen && !BigWin ? ScreenHeight : WinHeight;
	int gui_width = IsFullScreen && !BigWin || ScreenWidth == WinWidth ? GUI_Width : (int)((double) WinWidth / ScreenWidth * GUI_Width);
	int gui_height = IsFullScreen && !BigWin || ScreenHeight == WinHeight ? GUI_Height : (int)((double) WinHeight / ScreenHeight * GUI_Height);
	POINT metaMenuPoint{ ( width - gui_width) / 2, ( height - gui_height) / 2 };
	RECT classicRect{ 0, 0, GUI_Width - 1, GUI_Height - 1 };
	RECT metaMenuRect{ metaMenuPoint.x, metaMenuPoint.y, metaMenuPoint.x + gui_width - 1, metaMenuPoint.y + gui_height - 1 };
	int startingX = 0;
	int startingY = 0;//gbRunGame ? 0 : updatingPoint.y;
	if( DDraw /*render == ddraw*/ ){
		#if ADJUST_WINDOW_RECT_WITH_MENU
		RECT window, client = { 0, 0, ScreenWidth, ScreenHeight };
		GetWindowRect( ghMainWnd, &window );
		AdjustWindowRectEx(&client, GetWindowLong(ghMainWnd, GWL_STYLE), GetMenu(ghMainWnd) != NULL, GetWindowLong(ghMainWnd, GWL_EXSTYLE));
		startingX = window.left - client.left;
		startingY = window.top - client.top;
		#endif
		DDSURFACEDESC surfDesc32;
		surfDesc32.dwSize = sizeof( surfDesc32 );
		// draw backbuffer
		ScreenSurface32Back->Lock( nullptr, &surfDesc32, /*DDLOCK_WRITEONLY |*/ DDLOCK_WAIT | DDLOCK_NOSYSLOCK, 0 );
		pixelFormat = surfDesc32.ddpfPixelFormat.dwRGBBitCount;
		int dstPitch = surfDesc32.lPitch;
		void* dst = surfDesc32.lpSurface;
		int srcPitch = ( VideoWindowExists || (!gbRunGame && !LoadingScreenCEL/* Handler1 != LoadingWindowHandler*/) ) ? ScreenSurfacePitch : WorkingWidth;
		int srcWidth = gbRunGame && !VideoWindowExists ? ScreenWidth : GUI_Width;
		int srcHeight = gbRunGame && !VideoWindowExists ? ScreenHeight : GUI_Height;
		switch( pixelFormat ){
		case 15: drawPixels<ushort, 32, 32,  32, 5, 10>( startingX, startingY, src, dst, dstPitch, srcPitch, srcWidth, srcHeight, screenPalette ); break;
		case 16: drawPixels<ushort, 32, 64,  32, 5, 11>( startingX, startingY, src, dst, dstPitch, srcPitch, srcWidth, srcHeight, screenPalette ); break;
		case 32: drawPixels<uint, 256, 256, 256, 8, 16>( startingX, startingY, src, dst, dstPitch, srcPitch, srcWidth, srcHeight, screenPalette ); break;
		}
		ScreenSurface32Back->Unlock(0);
		// blit/flip on screen
		POINT p = { 0, 0 };
		ClientToScreen(ghMainWnd, &p);
		RECT windowRect;
		GetClientRect(ghMainWnd, &windowRect);
		OffsetRect(&windowRect, p.x, p.y);
		RECT* srcRect = 0; // gameplay and loading screen - full rect
		if( !gbRunGame && !VideoWindowExists ){ // meta menu
			windowRect.right   = windowRect.left + metaMenuRect.right;
			windowRect.bottom  = windowRect.top  + metaMenuRect.bottom;
			windowRect.left   += metaMenuRect.left ;
			windowRect.top    += metaMenuRect.top;
			srcRect = &classicRect;
		}else if( VideoWindowExists ){ // video
			srcRect = &classicRect;
		}
		HRESULT hr = ScreenSurface32->Blt(&windowRect, ScreenSurface32Back, srcRect, DDBLT_WAIT, NULL);
	}else{ /*render == d3d*/
		int ok = -1;
		bool second = IsSecondWin && (OnlySecondWin || VideoWindowExists || !gbRunGame || LoadingScreenCEL /*Handler1 == LoadingWindowHandler*/);
		HRESULT d3dOk = D3D_OK;
		if( D3D_OK == (d3dOk = (OnlySecondWin ? D3D_OK : D3dDevice->BeginScene()) ) ){
			if( second ) SecDevice->BeginScene();
			int srcPitch = ( VideoWindowExists || (!gbRunGame && !LoadingScreenCEL/* Handler1 != LoadingWindowHandler*/) ) ? ScreenSurfacePitch : WorkingWidth;
			int srcWidth = gbRunGame && !VideoWindowExists ? ScreenWidth : GUI_Width;
			int srcHeight = gbRunGame && !VideoWindowExists ? ScreenHeight : GUI_Height;
			if( DoMenuScreenShot && !OnlySecondWin ){
				ShotPalette = screenPalette;
				ShotSrc = src;
				ShotPitch = srcPitch;
				ShotStartingX = startingX;
				ShotStartingY = startingY;
				ShotWidth = srcWidth;
				ShotHeight = srcHeight;
				SaveScreenShot();
				DoMenuScreenShot = 0;
			}
			if( DrawMode == 0 || !gbRunGame || VideoWindowExists /*|| LoadingScreenCEL*/ ){ // update backbuffer
				if( !OnlySecondWin ) ok = RenderSurface( D3dDevice, VideoSurface, D3dSurface, BackBuffer, startingX, startingY, src, pixelFormat, srcPitch, srcWidth, srcHeight, screenPalette, classicRect, metaMenuRect );
				if( second         ) ok = RenderSurface( SecDevice, SecVideoSurface, SecD3dSurface, SecBuffer, startingX, startingY, src, pixelFormat, srcPitch, srcWidth, srcHeight, screenPalette, classicRect, metaMenuRect );
			} else{ // DrawMode == 1 // draw textured quad
				if( !OnlySecondWin ) ok = RenderQuadTexture( D3dDevice, D3dTexture, D3dPalette, VideoSurface   , src );
				if( second         ) ok = RenderQuadTexture( SecDevice, SecTexture, SecPalette, SecVideoSurface, src );
			}
			if( !OnlySecondWin ) D3dDevice->EndScene();
			if( second         ) SecDevice->EndScene();
			//if( Vsync && (BigWin || !IsFullScreen) ) DirectDrawPtr->WaitForVerticalBlank( DDWAITVB_BLOCKBEGIN, 0 ); // проба vsync для оконного режим, не очень
			HRESULT pres = OnlySecondWin ? D3D_OK : D3dDevice->Present(NULL, NULL, ( D3dRef && ! gbRunGame && ! VideoWindowExists ) ? DlgHwnd : NULL, NULL);
			if( second ) SecDevice->Present( NULL, NULL, SecWnd, NULL );
			if( !ok || D3D_OK != pres /*&& !(IsStartTh1 && !Exe::IsMainWindowActive)*/ ){
				if( !D3dReset() ){} // TerminateWithError( "Direct 3D device restoring error" );
			}
		}else{
			--AllDrawCount;
		}
		if( !IsMainWindowMinimized && not(d3dOk, D3D_OK, D3DERR_WASSTILLDRAWING) ){
			D3dReset();
		}
	}
	if( AltEnter() ){
		SwitchFullscreen();
	}
	//}
	return ScreenSurface;
}
