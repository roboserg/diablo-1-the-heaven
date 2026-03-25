//#include "..\src\stdafx.h"
/***********************************************************************
*
* Description:  GfxDecode -- functions for reading Diablo's GFX files
* Author: Marko Friedemann <marko.friedemann@bmx-chemnitz.de>
* Created at: Son Jan 27 15:20:43 CET 2002
* Computer: hangloose.flachland-chemnitz.de 
* System: Linux 2.4.16 on i686
*    
* Copyright (c) 2002  BMX-Chemnitz.DE  All rights reserved.
*
* ---------------------------------------------------------------------
* included are functions for getting:
*  - the framecount of .CEL-files                  -> celGetFrameCount()
*  - single frames of .CEL-files                   -> celGetFrameData()
*  - the framecount of .CL2-files                  -> cl2GetFrameCount()
*  - single directions of .CL2-files (all frames)  -> cl2GetDirData()
*  - single .PCX-files (256 color; v2, v5)         -> pcxGetData()
***********************************************************************/

#include "StormLib.h"

#define TRANS_COL 256

using std::cerr;
using std::vector;

/****** RAMP stuff *****************************************************
 * for a more detailed description/explanation see below
 ***********************************************************************/
// two variations: one/two ramp(s)
static  ushort c_2RampSize = 544;    // the frame size
static  ushort c_1RampSize = 800;    // the frame size

// ramps (both variations) can be either left or right
static  ushort c_RampOffsetLeft[17] = {
    0,      //                                      __
    8,      // +  8     note that this          __--
    24,     // + 16     "drawing" is        __--
    48,     // + 24     upside down!    __--    this area
    80,     // + 32                 __--        is always
    120,    // + 40             __--            colored
    168,    // + 48         __--
    224,    // + 56     __--  lower ramp ends here (+30 == 254)
    288,    // + 64   --__    upper ramp might be missing
    348,    // + 60   |   --__
    400,    // + 52   |       --__              this area
    444,    // + 44   |           --__          is always
    480,    // + 36   |               --__      colored
    508,    // + 28   | either trans-     --__
    528,    // + 20   | parent or colored     --__
    540,    // + 12   |                           --__   +2 Pixels = 544
    542     // +  2   | this last one doesn't exist, it's those^ 2 pixels
};

static  ushort c_RampOffsetRight[17] = {
    2,      //        __         before this, there are 2 Pixels
    14,     // + 12     --__              4^2 - 2
    34,     // + 20         --__          6^2 - 2
    62,     // + 28  this area  --__      8^2 - 2 
    98,     // + 36  is always      --__ 10^2 - 2  pattern anyone? ;)
    142,    // + 44  colored            --__ 
    194,    // + 52                         --__
    254,    // + 60  lower ramp ends here       --__     (-30 == 224)
    318,    // + 64  upper ramp might be missing  __--   
    374,    // + 56                           __--   |
    422,    // + 48  this area            __--       |   note that this
    462,    // + 40  is always        __--           |   "drawing"
    494,    // + 32  colored      __--  eiter trans- |   is upside down!
    518,    // + 24           __-- parent or colored |
    534,    // + 16       __--                       |
    542,    // +  8   __--   +2 Startpixels = 544    |
    542     // +  0  this last one doesn't exist, it |   would be EOF
};

/****** FrameBuffer class **********************************************
 * holds buffers and size information of the actual target image
 *  purpose: buffer management and avoidance of ugly globals
 **********************************************************************/
class FrameBuf
{
 protected:
    vector <uchar *> vecData;
    uchar  *pCurrRow;
    uchar  *pPalette;
    ushort  uRows;
    ushort *upYSize;
    ushort *upMaxX;
 public:
    ushort  uCols;
    ushort  uXSize;
    ushort  uMaxBlock;
    ushort  uFrameNum;
    bool      bHasBlocks;
    bool      bHasRamps;
    FrameBuf(
        uchar *pPal=NULL, ushort frame=0, ushort xsize=0,
        ushort *pysize=NULL, ushort *pmaxx=NULL, ushort maxblock=0)
    {
        pCurrRow  = new uchar[4*xsize];
        pPalette  = pPal;
        uCols     = 0;
        uRows     = 0;
        uXSize    = xsize;
        uFrameNum = frame;
        uMaxBlock = maxblock;
        upYSize   = pysize;
        upMaxX    = pmaxx;
        bHasBlocks= false;
        bHasRamps = false;
    }
    ~FrameBuf()
    {
        delete[] pCurrRow;
        for (vector <uchar *>::iterator vi=vecData.begin(); vi!=vecData.end(); vi++)
            delete[] *vi;
        vecData.clear();
    }
    void addLine()
    {
        ++uRows;
        uCols = 0;
        vecData.push_back(pCurrRow);
        pCurrRow = new uchar[4*uXSize];
    }
    void addPixel(ushort uColorNum)
    {
        if (uColorNum > TRANS_COL) {
            cerr << "\n*** there seemed to be an error, illegal color index " << uColorNum;
            cerr << "\n +++ at (" << uCols << "," << uRows << "), see for yourself *** \n\n";
            uColorNum = TRANS_COL;      // sane setting to avoid segfaults
        }
        
        memcpy(pCurrRow + 4*uCols, pPalette + 4*uColorNum, 4*sizeof(uchar));
        if (++uCols == uXSize)
            addLine();
        else if ((uColorNum != TRANS_COL) && (upMaxX != NULL) && (uCols > *upMaxX))
            *upMaxX = uCols;
    }
    // used to return the actual image data
    uchar *getData()
    {
        ushort i;
        vector <uchar *>::reverse_iterator vri;
        // allocate a buffer to hold the actual image size
        uchar *tmp = new uchar[4*uXSize*uRows];
        
        // the lines are upside down inside the vector, use reverse iterator
        for (i=0, vri=vecData.rbegin(); vri!=vecData.rend(); vri++, i++)
            memcpy(tmp+4*uXSize*i, *vri, 4*uXSize*sizeof(uchar));
        
        *upYSize = uRows;    // set height
        
        if (uCols > 0) {
            cerr << "\n*** there seemed to be an error (last line does not match boundary, " << uCols << " pixels left)";
            cerr << "\n +++ this is often caused by specifying an invalid width, see for yourself *** \n\n";
        }
        
        return tmp;
    }
};

ushort WINAPI celGetFrameCount(uchar *pFileBuf)
{
    uint tmp;
    memcpy(&tmp, pFileBuf, sizeof(uint));
    return (ushort)tmp;
}

/***** Block Decoder ***************************************************
 * one of three possible decoding techniques necessary for .cel
 *  possible block contents are either colored (in that case the
 *  appropriate number of pixels are read) or transparent pixels
 *  there are neither ramps nor plain pixels allowed here
 ***********************************************************************/
uchar *celDecodeBlocks(uchar *pFileBuf, FrameBuf *pFrame, uint *framestart)
{
    uint uFilePos=framestart[pFrame->uFrameNum];
    uchar cRead=0, i=0;

    if (!pFrame->bHasBlocks)      // sanity check
        return NULL;
        
    while (uFilePos < framestart[pFrame->uFrameNum+1]) {
        cRead = pFileBuf[uFilePos++];
        
        if ((uFilePos == framestart[pFrame->uFrameNum]+1))
            // TODO: what is this 0x0A 0x00 stuff all about?
            if ((cRead == 0x0A) && (pFileBuf[uFilePos] == 0x00)) {
                uFilePos += 9;
                cRead = pFileBuf[uFilePos++];
            }
    
        if (cRead > 0x7F)
            // transparent block (complement, 256-val)
            for (i=0; i<256-cRead; i++)
                pFrame->addPixel(TRANS_COL);
        else if (cRead < pFrame->uMaxBlock + 1)
                // pixel block (block size pixels to be read!)
                for (i=0; i<cRead; i++)
                    pFrame->addPixel(pFileBuf[uFilePos++]);
        else
            cerr << "\n*** block mode: illegal block (> max_size) ***\n\n";
    }
    return pFrame->getData();
}

/***** Ramp Decoder ****************************************************
 * the second of three possible decoding techniques necessary for .cel
 *  each block save the first/last is enclosed by two 0x00 pairs
 *  those blocks affect _TWO_ rows with one being 2 colored pixels shorter
 *  the first/last "block" affects only one row
 ***********************************************************************/
uchar *celDecodeRamps(uchar *pFileBuf, FrameBuf *pFrame, uint *framestart, bool bLeft)
{
    uint uFrameLen = framestart[pFrame->uFrameNum+1]-framestart[pFrame->uFrameNum];
    uint uFilePos=0;
    ushort uBlockLen=0, i=0, j=0;
    bool bFirstLonger=false;
    
    if (!pFrame->bHasRamps)     // sanity check
        return NULL;
        
    if (pFrame->uXSize != 32)   // only used in that case
        return NULL;

    if (!bLeft) { // get first two pixels for right side ramps
        pFrame->addPixel(pFileBuf[framestart[pFrame->uFrameNum]]);
        pFrame->addPixel(pFileBuf[framestart[pFrame->uFrameNum]+1]);
    }

    // do all the ramp blocks
    for (i=0; i<(uFrameLen == c_2RampSize ? 15 : 7); i++) {
        uBlockLen = (bLeft ? (c_RampOffsetLeft[i+1] - c_RampOffsetLeft[i]) : (c_RampOffsetRight[i+1] - c_RampOffsetRight[i]));
        uFilePos = framestart[pFrame->uFrameNum] + (bLeft ? c_RampOffsetLeft[i] : c_RampOffsetRight[i]) + 2;
        bFirstLonger = (i>(bLeft ? 7 : 6));
        if (bLeft) {
            // OK, first line, starting with transparency
            for (j=0; j<pFrame->uXSize - uBlockLen/2 + (bFirstLonger ? 0 : 2); j++)
                pFrame->addPixel(TRANS_COL);
            // fill it up with the pixel block
            for (j=0; j<uBlockLen/2 - (bFirstLonger ? 0 : 2); j++)
                pFrame->addPixel(pFileBuf[uFilePos++]);
            // second line, starting again with transparency
            for (j=0; j<pFrame->uXSize - uBlockLen/2 + (bFirstLonger ? 2 : 0); j++)
                pFrame->addPixel(TRANS_COL);
            // fill the second line with the remaining pixels
            for (j=0; j<uBlockLen/2 - (bFirstLonger ? 2 : 0); j++)
                pFrame->addPixel(pFileBuf[uFilePos++]);
        } else {
            if (pFrame->uCols != 0) // fill current line with trans (if not empty)
                for (j=pFrame->uXSize - pFrame->uCols; j>0; j--)
                    pFrame->addPixel(TRANS_COL);
            // OK, insert the first pixels into a new line
            for (j=0; j<uBlockLen/2 - (bFirstLonger ? 0 : 2); j++)
                pFrame->addPixel(pFileBuf[uFilePos++]);
            // fill the line with transparency
            for (j=0; j<pFrame->uXSize - uBlockLen/2 + (bFirstLonger ? 0 : 2); j++)
                pFrame->addPixel(TRANS_COL);
            // start a second line with the remaining pixels
            for (j=0; j<uBlockLen/2 - (bFirstLonger ? 2 : 0); j++)
                pFrame->addPixel(pFileBuf[uFilePos++]);
        }
    }
    
    // now read the last 0x00 pair and fill up
    uBlockLen = (uFrameLen == c_2RampSize ? 30 : 2); // one or two ramps?
    uFilePos = framestart[pFrame->uFrameNum] + (bLeft ? c_RampOffsetLeft[i] : c_RampOffsetRight[i]) + 2;
    // the transparency for the last (single) 0x00 pair
    for (j=0; j<uBlockLen; j++)
        pFrame->addPixel(TRANS_COL);
    if (bLeft) {  // left side only: the remaining line
        for (j=0; j<pFrame->uXSize - uBlockLen; j++)
            pFrame->addPixel(pFileBuf[uFilePos++]);
    }
    
    // now the rest of the file (plain)
    while (uFilePos < framestart[pFrame->uFrameNum+1])
        pFrame->addPixel(pFileBuf[uFilePos++]);
    
    // the uppermost line is emtpy when 2 ramps are used
    if (uFrameLen == c_2RampSize)
        for (j=0; j<pFrame->uXSize; j++)
            pFrame->addPixel(TRANS_COL);
            
    return pFrame->getData();
}

/***** celGetFrameData *************************************************
 *             decode .cel data for given frame and xsize
 * Args:
 *     *vpFileBuf       the buffer containing the filecontent
 *     *palette         the palette (4 bytes for each of the 257 entries)
 *                        256 colors are needed + 1 for alpha
 *      uXSize          this information must be given
 *      uFrameNume      the frame to get
 *     *uYSize          the actual value is returned therein
 *     *uMaxX           this can be used (if != NULL) to get the column
 *                      of the rightmost nontransparent pixel (useable
 *                      eg for fonts)
 *
 * Returns: an array containing 4 Bytes (RGBA) for each pixel
 *
 * ---------------------------------------------------------------
 * Comments:     dirty hack, started from scratch @ 2000-10-11
 *               cleanly rewritten during incorporation into ladiks StormLib
 *      status:  structured hack ;)
 *   
 *   It took me approx. 6 days to understand the format basics (hex viewer)
 *   For this I had a little help from a dos tool ("ddecode", from
 *   www.cowlevel.com, binary only, no sources) which, however, gave 
 *   me the general idea what the pictures are actually supposed to look like.
 *   
 *   The fine adjustments, however, took quite some time and a little luck.
 *   After I had written to various people (mickyk and ladik), which could
 *   not help me, but wished best luck (thanks, btw, it helped ;)), I tried
 *   some reverse engineering which was not succesful in the end.
 *   
 *   I then had incidentally a new idea of what could be going on @ 2002-01-23.
 *   It just came to my mind that I could retry some actual painting in
 *   reverse order (had done that before to no avail) and when looking closer
 *   at it I realized the "ramp" stuff. This really is the trickiest part and
 *   it took me some eight days to implement it without breaking the other
 *   parts of the code. Very odd format indeed.
 *
 * TODO: learn what 0x0A 0x00 means
 **********************************************************************/
uchar * WINAPI celGetFrameData(uchar *pFileBuf, uchar *palette, ushort uXSize, ushort uFrameNum, ushort *uYSize, ushort *uMaxX)
{
    FrameBuf *pFrame;
    uint *framestart=NULL, frames=0, uFilePos=0;
    ushort i, tmpWord=0;
    uchar cRead=0, *data;
    
    memcpy(&frames, pFileBuf, sizeof(uint));
    uFilePos += sizeof(uint);
    
    if (pFileBuf == NULL) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }
    
    if (palette == NULL) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }
    
    if (uFrameNum > frames-1) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }
        
    if (uYSize == NULL) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }
        
    // in case we want to know the rightmost pixels column (usable eg. for fonts)
    if (uMaxX != NULL)
        *uMaxX = 0;
    
    // get the frame offsets
    framestart = new uint[frames+1];
    for (i=0; i<frames+1; i++) {
        memcpy(&framestart[i], pFileBuf+uFilePos, sizeof(uint));
        uFilePos += sizeof(uint);
    }
    
    /****** block size *************************************************
     * depends on the image width
     ******************************/
    
    double erg = floor(sqrt(pow(2, floor(log((double)(framestart[uFrameNum+1] - framestart[uFrameNum])) / log(2.0)))));
    pFrame = new FrameBuf(palette, uFrameNum, uXSize, uYSize, uMaxX, max((ushort)min((int)erg, 0x7F), uXSize));
    
    /****** ramp detection -- AFAIK only needed for 32x32 tiles ********
     * here I use hard coded constants because this is the only simple
     * way to get the detection done; plus this stuff is only to be
     * found in such 32x32 (tile) files and so wont hurt anyone ;)
     ******************************************************************/

    uint uFrameLen = framestart[uFrameNum+1] - framestart[uFrameNum];
    if ((uXSize == 32) && ((uFrameLen == c_2RampSize) || (uFrameLen == c_1RampSize))) {

        // use the static arrays for the check        
        for (i=0; i<(uFrameLen == c_2RampSize ? 16 : 8); i++) {
            memcpy(&tmpWord, pFileBuf+framestart[uFrameNum]+c_RampOffsetLeft[i], sizeof(ushort));
            if (tmpWord != 0)
                break;
        }
        bool bRampsLeft = pFrame->bHasRamps = (i==(uFrameLen == c_2RampSize ? 16 : 8));
        if (!pFrame->bHasRamps) { // only one can apply
            for (i=0; i<(uFrameLen == c_2RampSize ? 16 : 8); i++) {
                memcpy(&tmpWord, pFileBuf+framestart[uFrameNum]+c_RampOffsetRight[i], sizeof(ushort));
                if (tmpWord != 0)
                    break;
            }
            pFrame->bHasRamps = (i==(uFrameLen == c_2RampSize ? 16 : 8)); // bRampsLeft stays false in this case
        }
        
        if (pFrame->bHasRamps) {  // decode ramps and be off (if appropriate)
            data = celDecodeRamps(pFileBuf, pFrame, framestart, bRampsLeft);
            delete pFrame;
            delete[] framestart;
            return data;
        }
    }
    
    /*********** block detection ***************************************
     * 0x0A as start byte seems to be sufficient (though I still dunno
     * what the trailing 10 bytes mean); in any other case we act as if
     * blocks were to be used and check afterwards if the image looks
     * OK (that is, the last line has no pixels in it)
     ******************************************************************/
    
    cRead = pFileBuf[framestart[uFrameNum]];
    if (cRead == 0x0A)          // sufficient
        pFrame->bHasBlocks = true;
    // if width == 32 && framelen == 32*32, assume plain
    else if ((uXSize != 32) || (uFrameLen != 32*32)) {    // check needed
        uFilePos=framestart[uFrameNum];             
        i=0;
        // rush through the frame
        while (uFilePos < framestart[uFrameNum+1]) {
            cRead = pFileBuf[uFilePos++];
            
            // transparency blocks
            while (cRead > 0x7F) {
                i += 256-cRead;
                i %= uXSize;
                if (uFilePos < framestart[uFrameNum+1])
                    cRead = pFileBuf[uFilePos++];
                else
                    cRead = 0;
            }
            
            // colored pixel block
            if (uFilePos < framestart[uFrameNum+1]) {
                if (cRead < pFrame->uMaxBlock + 1) {
                    i+=cRead;
                    i%=uXSize;
                    uFilePos+=cRead;
                } else {
                    // when the value is out of valid blockrange
                    i=1;    // trigger error (1%uXSize != 0)
                    break;
                }
            }
        }
        if (i%uXSize == 0)      // looks as if we got it right
            pFrame->bHasBlocks=true;
    }
    
    if (pFrame->bHasBlocks) {   // use block decoder if appropriate
        data = celDecodeBlocks(pFileBuf, pFrame, framestart);
        delete pFrame;
        delete[] framestart;
        return data;
    }
        
    // plain mode (#3), read each color index and write the pixel
    uFilePos=framestart[uFrameNum];
    while (uFilePos < framestart[uFrameNum+1])
        pFrame->addPixel(pFileBuf[uFilePos++]);
    
    // cleanup, return image data and height
    data = pFrame->getData();
    delete pFrame;
    delete[] framestart;
    return data;
}

ushort WINAPI cl2GetFrameCount(uchar *pFileBuf)
{
    uint tmp;
    memcpy(&tmp, pFileBuf, sizeof(uint));
    memcpy(&tmp, pFileBuf+tmp, sizeof(uint));
    return (ushort)tmp;
}

/***** cl2GetDirData ***************************************************
 *         decodes all frames of a .cl2 for given direction and xsize
 * Args:
 *     *pFileBuf        the buffer containing the filecontent
 *     *palette         the palette (4 bytes for each of the 257 entries)
 *                        256 colors are needed + 1 for alpha
 *      uXSize          this information must be given
 *      uDirNum         the direction to get the frames from
 *     *uYSize          the actual height is returned herein
 *
 * Returns: <frames> arrays containing 4 Bytes (RGBA) for each pixel
 *     where <frames> is read at runtime and handed back via *uFrames
 *
 * ---------------------------------------------------------------
 * Comments:     dirty hack, started from scratch @ 2000-10-12
 *   
 *   The format basics are similar to .cel, with the main difference
 *   that the values read have reverse interpretation. In .cel a value
 *   greater than 0x7F means transparency, while in .cl2 this means
 *   color and vice-versa. .cl2 has the additional understanding
 *   of blocks of the same color (0x80 .. 0xBF) where the one color is
 *   written multiple times.
 *
 *   .cl2 only uses the block scheme, so there is no detection
 *   necessary in order to get it right. The only thing still unknown
 *   is that 0x0A 0x00 stuff...
 *   
 * TODO: learn what 0x0A 0x00 means
 ***********************************************************************/
BYTE ** WINAPI cl2GetDirData(BYTE *pFileBuf, BYTE *palette, WORD uXSize, WORD uDirNum, WORD *uYSize)
{
    FrameBuf *pFrame;
    uint frames=0, *framestart=NULL, uFilePos=0;
    ushort i, fc;
    uchar cRead=0, **data=NULL;
    
    if (pFileBuf == NULL) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }
    
    if (palette == NULL) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }
    
    if (uDirNum > 7) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }
        
    if (uYSize == NULL) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }
    
    // get direction offsets
    uint dirstart[8];
    for (i=0; i<8; i++) {
        memcpy(&dirstart[i], pFileBuf+uFilePos, sizeof(uint));
        uFilePos += sizeof(uint);
    }

    uFilePos = dirstart[uDirNum];
    
    memcpy(&frames, pFileBuf+uFilePos, sizeof(uint));
    uFilePos += sizeof(uint);
    
    data = new uchar*[frames];
    
    // get frame offsets
    framestart = new uint[frames+1];
    for (i=0; i<frames+1; i++) {
        memcpy(&framestart[i], pFileBuf+uFilePos, sizeof(uint));
        uFilePos += sizeof(uint);
    }
    
    // get frame data        
    for (fc=0; fc<frames; fc++) {
        pFrame = new FrameBuf(palette, 0, uXSize, uYSize, NULL, 0);
        
        uFilePos = dirstart[uDirNum] + framestart[fc];
        while (uFilePos < dirstart[uDirNum] + framestart[fc+1]) {

            cRead = pFileBuf[uFilePos++];
            if (cRead < 0x80) { // transparency
                // TODO: what is this 0x0A 0x00 stuff all about?
                if ((cRead == 0x0A) && (pFileBuf[uFilePos] == 0) && (uFilePos == dirstart[uDirNum] + framestart[fc] + 1))
                    uFilePos += 9;  // ignore the 9 bytes after 0x0A 0x00 at the framestart
                else
                    for (i=0; i<cRead; i++)
                        pFrame->addPixel(TRANS_COL);
            } else if (cRead < 0xC0) {
                // read the next byte and write it <0xBF - cRead> times
                for (i=0; i<0xBF - cRead; i++)
                    pFrame->addPixel(pFileBuf[uFilePos]);
                ++uFilePos;
            } else // cRead > 0xBF
                // read a block of the given size and write it
                for (i=0; i < 256-cRead; i++)
                    pFrame->addPixel(pFileBuf[uFilePos++]);
        }
        
        // got the frame data, save it
        data[fc] = pFrame->getData();
        delete pFrame;
    }
    
    delete[] framestart;
    return data;
}

/****** pcxGetData *****************************************************
 *           decodes pcx files (256 color, as in diablo mpq)
 * Args:
 *     *pFileBuf        the buffer containing the filecontent
 *      uFileSize       the size of the file buffer
 *      uTransColor     the palette entry to be transparent
 *     *uXSize          the actual width is returned herein
 *     *uYSize          the actual height is returned herein
 *
 * Returns: an array containing 4 Bytes (RGBA) for each pixel
 *
 * ---------------------------------------------------------------
 * Comments:    format info and pseudocode taken from:
 *          Klaus Holtorf, "Das Handbuch der Grafikformate"
 *          ISBN 3-7723-6393-8
 ***********************************************************************/
BYTE * WINAPI pcxGetData(BYTE *pFileBuf, uint uFileSize, BYTE uTransColor, WORD *uXSize, WORD *uYSize)
{
    uint uFilePos=0;
    uint uDataRead=0;   // potentially big! (logo.pcx: 550 * 216 * 15 = 1,782,000)
    ushort i=0;
    uchar *data, *palette;
    uchar uColorNum=0, uCount=0;
    
    struct pcx_header_t {
        uchar     id;
        uchar     version;
        uchar     compressed;
        uchar     bpp;
        ushort    x0;
        ushort    y0;
        ushort    x1;
        ushort    y1;
        ushort    xdpi;
        ushort    ydpi;
        uchar     pal[16][3];
        uchar     reserved;
        uchar     layers;
        ushort    rowbytes;
        ushort    colortype;
        uchar     pad[58];
    } pcxHeader;
    
    if (pFileBuf == NULL) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }
    
    if (uXSize == NULL) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }
        
    if (uYSize == NULL) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }

    // get image information
    memcpy(&pcxHeader, pFileBuf, sizeof(struct pcx_header_t));
    *uXSize = (pcxHeader.x1 - pcxHeader.x0 + 1);
    *uYSize = (pcxHeader.y1 - pcxHeader.y0 + 1);
    
    if ((pcxHeader.version != 2) && (pcxHeader.version != 5)) {
        cerr << "cannot handle pcx v" << pcxHeader.version << "\n";
        return NULL;
    }
    
    // get palette
    palette = new uchar[256*4];
    if (pFileBuf[uFileSize - 768 - 1] != 0x0C) {
        cerr << "palette error at " << uFileSize - 768 - 1 << "\n";
        return NULL;
    }
    for (i=0; i<256; i++) {
        memcpy(palette+i*4, pFileBuf+uFileSize-768+i*3, 3*sizeof(uchar));
        palette[i*4+3] = 0xFF;
    }
    memset(palette+uTransColor*4, 0, 4*sizeof(uchar)); // transparent black
    
    // start right after the header
    uFilePos = sizeof(struct pcx_header_t);
    data = new uchar[*uXSize * *uYSize * 4];
    while (uDataRead < (uint)(*uXSize * *uYSize)) {
        // decompress
        uColorNum = pFileBuf[uFilePos++];
        if ((pcxHeader.compressed) && (uColorNum > 0xBF)) {
            uCount = (uColorNum & 0x3F);
            uColorNum = pFileBuf[uFilePos++];
        } else
            uCount = 1;
    
        // draw count pixels with color val
        for (i=0; i<uCount; i++)
            memcpy(data+(uDataRead++)*4, palette+uColorNum*4, 4*sizeof(uchar));
    }
    
    // cleanup
    delete[] palette;
    
    return data;
}
