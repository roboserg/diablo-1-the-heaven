#include "stdafx.h"

enum Tooltip_Param
{
	TOOLTIP_TEXT_MAX = 256,

	NO_BORDER = 0,
	BACKGROUND_COLOR = 254,
	BORDER_COLOR = 176,
	LINE_HEIGHT = 22,
	PADDING = 6,
};

struct TooltipLine
{
    char text[TOOLTIP_TEXT_MAX];
    int width;
    int color;
};

vector<TooltipLine> TooltipLines;
int TooltipWidth = 0;
vec2 TooltipHotspot {};
vec2 TooltipOffset {};

vector<TooltipLine> TooltipLines_2;
int TooltipWidth_2 = 0;
vec2 TooltipHotspot_2{};
vec2 TooltipOffset_2{};

vector<TooltipLine> TooltipLines_3;
int TooltipWidth_3 = 0;
vec2 TooltipHotspot_3{};
vec2 TooltipOffset_3{};

constexpr vec2 TooltipHotspotLeft   = {  0.0,   0.5 },
               TooltipOffsetLeft    = { -8  ,   0   },
               TooltipHotspotRight  = {  1.0,   0.5 },
               TooltipOffsetRight   = { 32  ,   0   },
               TooltipHotspotTop    = {  0.5,   0.0 },
               TooltipOffsetTop     = {  0  , -32   },
               TooltipHotspotBottom = {  0.5,   1.0 },
               TooltipOffsetBottom  = {  0  ,  32   };

// (th4) Stage 2: union of the tooltip rects actually drawn this frame (screen
// coords). Reset by Tooltip_Clear; consumed by Draw.cpp's post-UI overlay
// clear so sprite RGB never composites over tooltips (§4.7).
DisplayObject g_TooltipClearRect = { 0, 0, 0, 0, 0, 0 };

static void Tooltip_AddClearRect(const DisplayObject& r)
{
	if (r.Width <= 0 || r.Heigth <= 0) return;
	if (g_TooltipClearRect.Width <= 0 || g_TooltipClearRect.Heigth <= 0) {
		g_TooltipClearRect = r;
		return;
	}
	int l = min(g_TooltipClearRect.Left, r.Left);
	int t = min(g_TooltipClearRect.Top, r.Top);
	int right = max(g_TooltipClearRect.Right, r.Right);
	int down = max(g_TooltipClearRect.Down, r.Down);
	SetupDisplayObject(g_TooltipClearRect, l, t, right - l, down - t);
}

// ---- th2 -------------------------------------------------------------------------------
// A color control byte is stored as (C_x + 128), i.e. 0x80..0x8B (C_0..C_11).
// Cyrillic CP1251 bytes are >= 0xA8, so they must NOT be treated as color codes.
// The original test `c < 0` (signed char) wrongly caught all Cyrillic. (th4)
static inline bool IsColorControlByte( unsigned char b ) { return b >= 0x80 && b <= 0x8B; }


int __fastcall GetTextWidthColored( const char* s )
{
	// (th4) Arabic must be measured as SHAPED glyphs: lam+alef is two stored pairs
	// but one drawn glyph, so a byte-wise count disagrees with the draw and every
	// tooltip box would be sized wrong.
	if( GameLanguage == LANG_AR ){
		static AR_Glyph g[ 512 ];
		const int gn = AR_Layout( s, g, 512 );
		int w = 0;
		for( int k = 0; k < gn; ++k ){
			if( g[k].isForm ) w += AR_Advance( g[k].cell, 0 );
			else if( !IsColorControlByte( g[k].raw ) )
				w += FontWidthSmall[ FontIndexSmall[ Codepage[ g[k].raw ] ] ] + 1;
		}
		return w;
	}
	int l = 0;
	while( unsigned char c = (unsigned char)*s ){
	    if( IsChiLead( c ) && IsChiTrail( (unsigned char)s[1] ) ){ // (th4) GB2312 pair
	        l += ( GameLanguage == LANG_HIN   // (th4) see HindiText.cpp
	             ? HIN_Advance( ChiLetterID( c, (unsigned char)s[1] ) - HIN_FIRST_ID, 0 )
	             : CHI_CELL_W_SMALL ) + 1;
	        s += 2;
	        continue;
	    }
	    ++s;
	    if( !IsColorControlByte( c ) ){ // count printable chars (incl. Cyrillic)
		    l += FontWidthSmall[ FontIndexSmall[ Codepage[ c ] ] ] + 1;
        }
	}
	return l;
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall DrawTextColored( int xPos, int yPos, int xPosEnd, const char* text, int baseFontColor )
{
    // (th4) Arabic: shape and reorder the whole run, then paint. The colour-code
    // stack below is untouched for Latin; Arabic strings in this build carry no
    // colour codes, and AR_Layout passes any stray byte through as a raw glyph so
    // it still lands somewhere visible rather than being silently eaten.
    if( GameLanguage == LANG_AR ){
        static AR_Glyph g[ 512 ];
        const int gn = AR_Layout( text, g, 512 );
        // (th4) Right-align within the tooltip's own column. xPosEnd is the caller's
        // right edge, so measuring the shaped run and starting that far in makes the
        // line hang off the right like Arabic should, without moving the box itself.
        int arW = 0;
        for( int k = 0; k < gn; ++k ){
            if( g[k].isForm )   arW += AR_Advance( g[k].cell, 0 );
            else if( g[k].raw ) arW += FontWidthSmall[ FontIndexSmall[ Codepage[ g[k].raw ] ] ] + 1;
        }
        // (th4) CENTRE, not right-align. This function draws the SPELLBOOK and the
        // PERK/synergy panels (SpellPanel.cpp, Perks.cpp), and both centre their
        // Latin text inside the column -- right-aligning Arabic there left every
        // line hugging one edge. Item tooltips use InfoPanel_DrawLine instead and
        // keep their right alignment.
        const int arAvail = xPosEnd - xPos + 1;
        const int arPad = ( arW < arAvail ) ? ( ( arAvail - arW ) / 2 ) : 0;
        int off = YOffsetHashTable[ yPos + Screen_TopBorder ] + xPos + arPad + Screen_LeftBorder;
        for( int k = 0; k < gn; ++k ){
            if( g[k].isForm ){
                DrawLetter( off, AR_FIRST_ID + g[k].cell, baseFontColor );
                off += AR_Advance( g[k].cell, 0 );
            }else if( g[k].raw ){
                const int li = FontIndexSmall[ Codepage[ g[k].raw ] ];
                if( li ) DrawLetter( off, li, baseFontColor );
                off += FontWidthSmall[ li ] + 1;
            }
        }
        return;
    }
	int offset = YOffsetHashTable[yPos + Screen_TopBorder] + xPos + Screen_LeftBorder;

	// Center text
	int textLength = GetTextWidthColored( text );
	int maxWidth = xPosEnd - xPos + 1;
	if (textLength < maxWidth){
		offset += (maxWidth - textLength) / 2;
	}
	
	// Draw line
    const char* textPtr = text;
	stack<char> colorStack;
    while( *textPtr ){
        if( IsColorControlByte( (unsigned char)*textPtr ) ){
            char color = *textPtr;
            if( colorStack.empty() ){
                colorStack.push( color );
            }else{
                if( colorStack.top() != color ){
                    colorStack.push( color );
                }else{
                    colorStack.pop();
                }
            }
        }else if( IsChiLead( (unsigned char)*textPtr ) && IsChiTrail( (unsigned char)textPtr[1] ) ){
            // (th4) GB2312 pair -> CJK frame in TH4loc_chi.mor
            int color = colorStack.empty() ? baseFontColor : colorStack.top() + 128;
            DrawLetter( offset, ChiLetterID( (unsigned char)textPtr[0], (unsigned char)textPtr[1] ), color );
            // (th4) must match the measure in GetTextWidthColored above, or the
            // tooltip wraps at a width it does not actually draw.
            offset += ( GameLanguage == LANG_HIN
                      ? HIN_Advance( ChiLetterID( (unsigned char)textPtr[0], (unsigned char)textPtr[1] ) - HIN_FIRST_ID, 0 )
                      : CHI_CELL_W_SMALL ) + 1;
            textPtr += 2;
            continue;
        }else{
            int currentLetter = FontIndexSmall[Codepage[(unsigned char)*textPtr]];
            if( currentLetter ) {
    
                int color;
                if( !colorStack.empty() ){
                    color = colorStack.top() + 128;
                }else{
                    color = baseFontColor;
                }
                DrawLetter( offset, currentLetter, color );
            }
            offset += FontWidthSmall[currentLetter] + 1;
        }
        ++textPtr;
    }
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_Clear()
{
    TooltipLines.clear();
    TooltipWidth = 0;

    TooltipLines_2.clear();
    TooltipWidth_2 = 0;

    TooltipLines_3.clear();
    TooltipWidth_3 = 0;

    ProhibitAltHighlight = false;

    // (th4) Stage 2: reset the drawn-tooltip union rect for the next frame.
    memset(&g_TooltipClearRect, 0, sizeof(g_TooltipClearRect));
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_AddLine( const char* text, int color )
{
    TooltipLine line{};
    strncpy(line.text, text, TOOLTIP_TEXT_MAX - 1);   // (th4) was an unbounded strcpy
    line.text[TOOLTIP_TEXT_MAX - 1] = 0;
    line.width = GetTextWidthColored( line.text );
    line.color = color;

    TooltipLines.push_back( line );
    TooltipWidth = max( TooltipWidth, line.width );
}

// (th4) WRAPPED TOOLTIP TEXT ---------------------------------------------------------------
//
// A tooltip line is one physical line: Tooltip_Draw paints line i at y + i*22 and sizes
// the box from the widest one. Nothing wraps. That was fine while every string fit, and
// wrong the moment a trait penalty grew to 180 characters -- it went out one line wide,
// off both screen edges, with the box covering half the screen.
//
// This splits `text` into lines no wider than maxWidth pixels and adds each with
// Tooltip_AddLine, so the box grows DOWN instead. Everything else in the game keeps
// calling Tooltip_AddLine and is untouched.
//
// Rules: break on spaces; honour an explicit \n; a single word wider than the limit is
// broken at character level, never inside a double-byte pair (CJK / Hindi cells).
// Width is measured by GetTextWidthColored, the same function the box is sized with,
// so a line that measures short here draws short. Colour-control bytes are NOT
// carried across a break -- a colour run that straddles the split ends at the split.
// The trait strings this is used on carry none; if it is ever used on text that does,
// re-emit the active colour byte at the head of each continuation line.
void __fastcall Tooltip_AddWrapped( const char* text, int color, int maxWidth )
{
    if( !text ) return;
    if( maxWidth < 32 ) maxWidth = 32;

    char line[TOOLTIP_TEXT_MAX];
    int  lineLen = 0;

    auto flush = [&]() {
        line[lineLen] = 0;
        if( lineLen ) Tooltip_AddLine( line, color );
        lineLen = 0;
    };
    auto stepLen = [&]( const char* p ) -> int {   // bytes in the glyph at p
        const unsigned char c = (unsigned char)*p;
        return ( IsChiLead( c ) && IsChiTrail( (unsigned char)p[1] ) ) ? 2 : 1;
    };
    auto widthOf = [&]( const char* p, int n ) -> int {
        char tmp[TOOLTIP_TEXT_MAX];
        if( n > TOOLTIP_TEXT_MAX - 1 ) n = TOOLTIP_TEXT_MAX - 1;
        memcpy( tmp, p, n ); tmp[n] = 0;
        return GetTextWidthColored( tmp );
    };

    const char* p = text;
    while( *p ){
        if( *p == '\n' ){ flush(); ++p; continue; }
        if( *p == ' ' ){
            // a space only goes in if the line is not empty; trailing ones are trimmed at flush
            if( lineLen && lineLen < TOOLTIP_TEXT_MAX - 1 ) line[lineLen++] = ' ';
            ++p; continue;
        }
        // next word
        const char* w = p;
        int wl = 0;
        while( w[wl] && w[wl] != ' ' && w[wl] != '\n' ) wl += stepLen( w + wl );

        // does it fit on the current line?
        if( lineLen + wl < TOOLTIP_TEXT_MAX - 1 ){
            memcpy( line + lineLen, w, wl );
            if( widthOf( line, lineLen + wl ) <= maxWidth ){
                lineLen += wl; p += wl; continue;
            }
        }
        // no: close the current line (dropping its trailing space) and retry on a fresh one
        if( lineLen ){
            while( lineLen && line[lineLen-1] == ' ' ) --lineLen;
            flush();
            continue;            // same word, empty line
        }
        // the word alone is wider than the limit: break it glyph by glyph
        int used = 0;
        while( used < wl ){
            const int g = stepLen( w + used );
            if( lineLen + g >= TOOLTIP_TEXT_MAX - 1 ){ flush(); continue; }
            memcpy( line + lineLen, w + used, g );
            if( lineLen && widthOf( line, lineLen + g ) > maxWidth ){ flush(); continue; }
            lineLen += g; used += g;
        }
        p += wl;
    }
    while( lineLen && line[lineLen-1] == ' ' ) --lineLen;
    flush();
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_AddLine_2(const char* text, int color)
{
    TooltipLine line{};
    strcpy(line.text, text);
    line.width = GetTextWidthColored(line.text);
    line.color = color;

    TooltipLines_2.push_back(line);
    TooltipWidth_2 = max(TooltipWidth_2, line.width);
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_AddLine_3(const char* text, int color)
{
    TooltipLine line{};
    strcpy(line.text, text);
    line.width = GetTextWidthColored(line.text);
    line.color = color;

    TooltipLines_3.push_back(line);
    TooltipWidth_3 = max(TooltipWidth_3, line.width);
}


// ---- th2 -------------------------------------------------------------------------------
// (th4) TOOLTIP LIFT LIST -- see AnimOrbs.cpp.
//
// The animated orbs are composited in D3D AFTER the whole 8-bit frame, so anything
// the software path drew inside the 88x88 orb rect is painted over. AnimOrbs already
// lifts the HP/mana numbers, the head art and the cursor back on top; the floating
// item tooltip was not on that list, so a tall tooltip under a low cursor lost a
// band of text to the globe. Each Tooltip_Draw* publishes its final rect here and
// AnimOrbs re-blits the overlapping part from WorkingSurface after drawing the orbs.
// Reset once per frame in Draw.cpp, before the three draws.
int  TooltipLiftCount = 0;
int  TooltipLiftRect[ TOOLTIP_LIFT_MAX ][ 4 ];   // x, y, w, h in screen pixels

void Tooltip_ResetLiftRects()
{
    TooltipLiftCount = 0;
}

static void Tooltip_PublishLiftRect( int x, int y, int w, int h )
{
    if( w <= 0 || h <= 0 ) return;
    if( TooltipLiftCount >= TOOLTIP_LIFT_MAX ) return;
    int* r = TooltipLiftRect[ TooltipLiftCount++ ];
    r[0] = x; r[1] = y; r[2] = w; r[3] = h;
}

void __fastcall Tooltip_Draw()
{
    if( TooltipLines.empty() ){
        return;
    }
    
    int width = TooltipWidth + PADDING*2;
    int height = TooltipLines.size()*LINE_HEIGHT - 11 + PADDING*2;
    // (th4) A tooltip can be taller than the screen (long set lists at low resolutions such as
    // 1056x594). Cap it at the screen height so LimitToRange(y, 0, ScreenHeight - height) below
    // keeps y >= 0, and stop drawing lines that no longer fit (see the loop). Without the cap
    // y went negative and DrawBoxTransparent() ran off the top of WorkingSurface. The same cap
    // is in all three Tooltip_Draw*() functions -- keep them in step.
    if (height > ScreenHeight) height = ScreenHeight;
    int x = CursorX + int(width * (TooltipHotspot.x - 1.0f));
    int y = CursorY + int(height * (TooltipHotspot.y - 1.0f));
    
    x += int(TooltipOffset.x);
    y += int(TooltipOffset.y);
    
    LimitToRange(x, 0 + ((TooltipWidth_2 && (TooltipHotspot.x == TooltipHotspotLeft.x)) ? (TooltipWidth_2 + (PADDING * 2) + 2) : 0) + ((TooltipWidth_3 && (TooltipHotspot.x == TooltipHotspotLeft.x)) ? (TooltipWidth_3 + (PADDING * 2) + 2) : 0), ScreenWidth - width - ((TooltipWidth_2 && (is(TooltipHotspot.x, TooltipHotspotRight.x, TooltipHotspotBottom.x))) ? (TooltipWidth_2 + (PADDING * 2) + 2) : 0) - ((TooltipWidth_3 && (TooltipHotspot.x == TooltipHotspotRight.x)) ? (TooltipWidth_3 + (PADDING * 2) + 2) : 0));
    LimitToRange(y, 0, ScreenHeight - height);
    
    DisplayObject tooltipRect;
    SetupDisplayObject(tooltipRect, x, y, width, height);
    
    // DrawTransparentBackground( tooltipRect.Left + Screen_LeftBorder, tooltipRect.Down + Screen_TopBorder, tooltipRect.Width, tooltipRect.Heigth, NO_BORDER, NO_BORDER, BACKGROUND_COLOR, BORDER_COLOR );
    DrawBoxTransparent(tooltipRect.Left, tooltipRect.Top, tooltipRect.Width, tooltipRect.Heigth);
    Tooltip_PublishLiftRect( tooltipRect.Left, tooltipRect.Top, tooltipRect.Width, tooltipRect.Heigth );

    for( int i = 0, ie = TooltipLines.size(); i != ie; ++i ){
        const TooltipLine& line = TooltipLines[i];
        // (th4) The last line's baseline sits PADDING above the box bottom; anything lower was cut by the height cap.
        if( tooltipRect.Top + i*LINE_HEIGHT + 11 + PADDING > tooltipRect.Top + tooltipRect.Heigth - PADDING ) break;
        DrawTextColored( tooltipRect.Left + PADDING, tooltipRect.Top + i*LINE_HEIGHT + 11 + PADDING, tooltipRect.Right - PADDING, line.text, line.color );
    }
    Tooltip_AddClearRect(tooltipRect);
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_Draw_2()
{
    if (TooltipLines_2.empty()) {
        return;
    }

    int width = TooltipWidth_2 + PADDING * 2;
    int height = TooltipLines_2.size() * LINE_HEIGHT - 11 + PADDING * 2;
    // (th4) A tooltip can be taller than the screen (long set lists at low resolutions such as
    // 1056x594). Cap it at the screen height so LimitToRange(y, 0, ScreenHeight - height) below
    // keeps y >= 0, and stop drawing lines that no longer fit (see the loop). Without the cap
    // y went negative and DrawBoxTransparent() ran off the top of WorkingSurface. The same cap
    // is in all three Tooltip_Draw*() functions -- keep them in step.
    if (height > ScreenHeight) height = ScreenHeight;
    int x = CursorX + int((width + (TooltipWidth + (PADDING * 2))) * (TooltipHotspot.x - 1.0f));
    int y = CursorY + int((TooltipLines.size() * LINE_HEIGHT - 11 + PADDING * 2) * (TooltipHotspot.y - 1.0f));

    x += int(TooltipOffset.x);
    y += int(TooltipOffset.y);

    if (TooltipHotspot.x == TooltipHotspotRight.x) {
        x += (TooltipWidth + (PADDING * 2));
        x+=2;
    }
    else if (TooltipHotspot.x == TooltipHotspotBottom.x) {
        x -= int((width + (TooltipWidth + (PADDING * 2))) * (TooltipHotspot.x - 1.0f));
        x += (TooltipWidth + (PADDING * 2)) / 2;
        x+=2;
    }
    else {
        x-=2;
    }

    

    LimitToRange(x, 0 + ((TooltipWidth_3 && (TooltipHotspot.x == TooltipHotspotLeft.x)) ? (TooltipWidth_3 + (PADDING * 2) + 2 ) : 0) , ScreenWidth - width - ((TooltipWidth_3 && (is (TooltipHotspot.x, TooltipHotspotRight.x, TooltipHotspotBottom.x))) ? (TooltipWidth_3 + (PADDING * 2) + 2) : 0));
    LimitToRange(y, 0, ScreenHeight - height);

    DisplayObject tooltipRect;
    SetupDisplayObject(tooltipRect, x, y, width, height);

    // DrawTransparentBackground( tooltipRect.Left + Screen_LeftBorder, tooltipRect.Down + Screen_TopBorder, tooltipRect.Width, tooltipRect.Heigth, NO_BORDER, NO_BORDER, BACKGROUND_COLOR, BORDER_COLOR );
    DrawBoxTransparent(tooltipRect.Left, tooltipRect.Top, tooltipRect.Width, tooltipRect.Heigth);
    Tooltip_PublishLiftRect( tooltipRect.Left, tooltipRect.Top, tooltipRect.Width, tooltipRect.Heigth );

    for (int i = 0, ie = TooltipLines_2.size(); i != ie; ++i) {
        const TooltipLine& line = TooltipLines_2[i];
        if (tooltipRect.Top + i * LINE_HEIGHT + 11 + PADDING > tooltipRect.Top + tooltipRect.Heigth - PADDING) break; // (th4) cut by the height cap
        DrawTextColored(tooltipRect.Left + PADDING, tooltipRect.Top + i * LINE_HEIGHT + 11 + PADDING, tooltipRect.Right - PADDING, line.text, line.color);
    }
    Tooltip_AddClearRect(tooltipRect);
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_Draw_3()
{
    if (TooltipLines_3.empty()) {
        return;
    }

    int width = TooltipWidth_3 + PADDING * 2;
    int height = TooltipLines_3.size() * LINE_HEIGHT - 11 + PADDING * 2;
    // (th4) A tooltip can be taller than the screen (long set lists at low resolutions such as
    // 1056x594). Cap it at the screen height so LimitToRange(y, 0, ScreenHeight - height) below
    // keeps y >= 0, and stop drawing lines that no longer fit (see the loop). Without the cap
    // y went negative and DrawBoxTransparent() ran off the top of WorkingSurface. The same cap
    // is in all three Tooltip_Draw*() functions -- keep them in step.
    if (height > ScreenHeight) height = ScreenHeight;
    int x = CursorX + int((width + (TooltipWidth + (PADDING * 2)) + (TooltipWidth_2 + (PADDING * 2))) * (TooltipHotspot.x - 1.0f));
    int y = CursorY + int((TooltipLines.size() * LINE_HEIGHT - 11 + PADDING * 2) * (TooltipHotspot.y - 1.0f));

    x += int(TooltipOffset.x);
    y += int(TooltipOffset.y);

    if (TooltipHotspot.x == TooltipHotspotRight.x) {
        x += (TooltipWidth + (PADDING * 2)) + (TooltipWidth_2 + (PADDING * 2));
        x += 4;
    }
    else if (TooltipHotspot.x == TooltipHotspotBottom.x) {
        x -= int((width + (TooltipWidth + (PADDING * 2)) + (TooltipWidth_2 + (PADDING * 2))) * (TooltipHotspot.x - 1.0f));
        x += ((TooltipWidth + (PADDING * 2)) + (TooltipWidth_2 + (PADDING * 2))) / 2;
        x += 4;
    }
    else {
        x -= 4;
    }

    LimitToRange(x, 0, ScreenWidth - width);
    LimitToRange(y, 0, ScreenHeight - height);

    DisplayObject tooltipRect;
    SetupDisplayObject(tooltipRect, x, y, width, height);

    // DrawTransparentBackground( tooltipRect.Left + Screen_LeftBorder, tooltipRect.Down + Screen_TopBorder, tooltipRect.Width, tooltipRect.Heigth, NO_BORDER, NO_BORDER, BACKGROUND_COLOR, BORDER_COLOR );
    DrawBoxTransparent(tooltipRect.Left, tooltipRect.Top, tooltipRect.Width, tooltipRect.Heigth);
    Tooltip_PublishLiftRect( tooltipRect.Left, tooltipRect.Top, tooltipRect.Width, tooltipRect.Heigth );

    for (int i = 0, ie = TooltipLines_3.size(); i != ie; ++i) {
        const TooltipLine& line = TooltipLines_3[i];
        if (tooltipRect.Top + i * LINE_HEIGHT + 11 + PADDING > tooltipRect.Top + tooltipRect.Heigth - PADDING) break; // (th4) cut by the height cap
        DrawTextColored(tooltipRect.Left + PADDING, tooltipRect.Top + i * LINE_HEIGHT + 11 + PADDING, tooltipRect.Right - PADDING, line.text, line.color);
    }
    Tooltip_AddClearRect(tooltipRect);
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_SetOffsetLeft()
{
    TooltipHotspot = TooltipHotspotLeft;
    TooltipOffset = TooltipOffsetLeft;
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_SetOffsetRight()
{
    TooltipHotspot = TooltipHotspotRight;
    TooltipOffset = TooltipOffsetRight;
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_SetOffsetTop()
{
    TooltipHotspot = TooltipHotspotTop;
    TooltipOffset = TooltipOffsetTop;
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_SetOffsetBottom()
{
    TooltipHotspot = TooltipHotspotBottom;
    TooltipOffset = TooltipOffsetBottom;
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_ShowItemInfo_2(int slot)
{
    if (slot == -1) {
        return;
    }
    //Tooltip_Clear();
    ProhibitAltHighlight = true;
    Tooltip_AddLine("Selected", C_7_Grey);
    Tooltip_AddLine_2("Equipped", C_7_Grey);

    char buffer[256];
    bool isVisualTradeSlot = false;
    bool isVisualStashSlot = false;
    bool isCraftSlot = false;
    Player& player = Players[CurrentPlayerIndex];
	Item& item = player.OnBodySlots[slot];

    if (item.ItemCode == IC_M1_NONE) return;

    BaseItem& baseItem = BaseItems[item.baseItemIndex];

    int nameColor = ItemColor(item);
    bool identified = item.Identified || item.MagicLevel == ML_0_USUAL;
    bool rare = item.dropType & D_RARE;

    const char* namePtr = nullptr;
    if (identified) {
        namePtr = item.FullMagicalItemName;
    }
    else {
        namePtr = item.Name;
    }

    if (item.ItemCode == IC_11_GOLD) {
        sprintf(InfoPanelBuffer, L("%i gold"), item.amount);
        namePtr = InfoPanelBuffer;
    }

    // Skip * in set's name
    if (*namePtr == '*') {
        ++namePtr;
    }

    Tooltip_AddLine_2(namePtr, nameColor);

    if (rare) {
        Tooltip_AddLine_2("Rare Item", nameColor);
    }
    else if (item.MagicLevel == ML_2_UNIQUE) {
        Tooltip_AddLine_2(L("Unique Item"), nameColor);
    }
    else if (item.MagicLevel == ML_3_SET) {
        Tooltip_AddLine_2(L("Set Item"), nameColor);
    }
    else if (IsQuestItem(item.baseItemIndex)) {
        Tooltip_AddLine_2(L("Quest item"), nameColor);
    }

    if (item.TypeID == ITEM_1_WEAPON || item.TypeID == ITEM_2_ARMOR) {
        if (item.BaseDurability) {
            if (item.BaseDurability == ITEM_DUR_INDESTRUBLE) {
                Tooltip_AddLine_2("indestructible", C_3_Gold);
            }
            else {
                char curDurColor = C_0_White;
                char baseDurColor = C_0_White;

                if (item.CurDurability <= 1) {
                    curDurColor = C_2_Red;
                }
                else if (item.CurDurability >= 2 && item.CurDurability <= 5) {
                    curDurColor = C_5_Yellow;
                }

                if (item.BaseDurability <= 1) {
                    baseDurColor = C_2_Red;
                }
                else if (item.CurDurability <= 1) {
                    baseDurColor = C_5_Yellow;
                }

                curDurColor += 128;
                baseDurColor += 128;

                sprintf(InfoPanelBuffer, L("durability: %c%i%c of %c%i%c"), curDurColor, item.CurDurability, curDurColor, baseDurColor, item.BaseDurability, baseDurColor);
                Tooltip_AddLine_2(InfoPanelBuffer);
            }
        }
    }

    if (item.BaseCharges) {
        char color = ((item.CurCharges > 0) ? C_0_White : C_2_Red) + 128;
        sprintf(InfoPanelBuffer, "%s charges: %c%i%c of %i", getSpellName(item.SpellIndex), color, item.CurCharges, color, item.BaseCharges);
        Tooltip_AddLine_2(InfoPanelBuffer);
    }

    if (item.TypeID == ITEM_1_WEAPON) {
        if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk) || GameMode == GM_CLASSIC) {
            goto MUM_1;
        }
        else if (!is(player.fullClassId, PFC_MAGE, PFC_ELEMENTALIST, PFC_WARLOCK, PFC_NECROMANCER, PFC_DEMONOLOGIST)) {
        MUM_1:
            const char* damageName = L("damage");
            if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
                goto MUMZ_4;
            }
            if (BaseItems[item.baseItemIndex].AnimationDelay) {
                damageName = L("slow damage");
            }
            else if (item.ItemCode == IC_1_SWORD && player.fullClassId != PFC_INQUISITOR && player.fullClassId != PFC_DRUID) {
                damageName = L("slashing damage");
            }
            else if (item.ItemCode == IC_4_MACE && player.fullClassId != PFC_INQUISITOR && player.fullClassId != PFC_DRUID) {
                damageName = L("crushing damage");
            }
        MUMZ_4:
            sprintf(InfoPanelBuffer, L("%s: "), damageName);
            if (item.overTime) {
                if (item.MinDamage == item.MaxDamage) {
                    sprintf(InfoPanelBuffer, "%s%i over %i sec", InfoPanelBuffer, item.MinDamage, item.overTime);
                }
                else {
                    sprintf(InfoPanelBuffer, "%s%i-%i over %i sec", InfoPanelBuffer, item.MinDamage, item.MaxDamage, item.overTime);
                }
            }
            else {
                if (item.MinDamage == item.MaxDamage) {
                    sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.MinDamage);
                }
                else {
                    sprintf(InfoPanelBuffer, "%s%i-%i", InfoPanelBuffer, item.MinDamage, item.MaxDamage);
                }
            }
            Tooltip_AddLine_2(InfoPanelBuffer);

            if (!(player.gameChanger & BIT(GC_10_HACKNSLASH)) && GameMode != GM_COLISEUM) {
                if (item.ItemCode == IC_1_SWORD) {
                    if (player.fullClassId == PFC_THRAEX) {
                        Tooltip_AddLine_2(L("(+50% to beasts)"));
                    }
                    else if (player.fullClassId == PFC_INQUISITOR || player.fullClassId == PFC_DRUID || HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
                        //Tooltip_AddLine("(+50% to beasts, -50% to undead)");
                    }
                    else if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS) {
                        Tooltip_AddLine_2(L("(+15% to beasts, -50% to undead)"));
                    }
                    else if (player.fullClassId == PFC_SECUTOR && HasTrait(CurrentPlayerIndex, TraitId::Bestiarius)) {
                        Tooltip_AddLine_2(L("(+80% to beasts, -50% to undead)"));
                    }
                    else if (player.fullClassId == PFC_KENSEI) {
                        Tooltip_AddLine_2(L("(+30% to beasts, -50% to undead)"));
                    }
                    else if (player.fullClassId == PFC_GUARDIAN) {
                        Tooltip_AddLine_2(L("(+40% to beasts, -50% to undead)"));
                    }
                    else {
                        Tooltip_AddLine_2(L("(+50% to beasts, -50% to undead)"));
                    }
                }
                else if (item.ItemCode == IC_4_MACE) {
                    if (player.fullClassId == PFC_TEMPLAR) {
                        Tooltip_AddLine_2(L("(+70% to undead, -50% to beasts)"));
                    }
                    else if (player.fullClassId == PFC_INQUISITOR || player.fullClassId == PFC_DRUID) {
                        //Tooltip_AddLine("(+70% to undead, -50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS) {
                        Tooltip_AddLine_2(L("(+15% to undead, -50% to beasts)"));
                    }
                    else if (player.fullClassId == PFC_SECUTOR && HasTrait(CurrentPlayerIndex, TraitId::Bestiarius)) {
                        Tooltip_AddLine_2(L("(+25% to undead, -50% to beasts)"));
                    }
                    else if (player.fullClassId == PFC_KENSEI) {
                        Tooltip_AddLine_2(L("(+30% to undead, -50% to beasts)"));
                    }
                    else if (player.fullClassId == PFC_GUARDIAN) {
                        Tooltip_AddLine_2(L("(+40% to undead, -50% to beasts)"));
                    }
                    else {
                        Tooltip_AddLine_2(L("(+50% to undead, -50% to beasts)"));
                    }
                }
                else if (item.ItemCode == IC_10_STAFF && player.fullClassId == PFC_MONK) {
                    Tooltip_AddLine_2(L("(+20% to demons, -20% to beasts)"));
                }
                else if (item.ItemCode == IC_20_CLAW && player.fullClassId == PFC_ASSASSIN) {//as if smb else wields claws...
                    Tooltip_AddLine_2(L("(+20% to beasts, -20% to undead)"));
                }//else if (item.ItemCode == IC_21_KNIFE && player.fullClassId == PFC_ASSASSIN) {//as if smb else wields knifes...
                 //   Tooltip_AddLine("(+PLACEHOLDER% to PLACEHOLDER, -PLACEHOLDER% to PLACEHOLDER)");
                //}
            }
            if (item.ItemCode == IC_19_TRAP) {
                BaseItem& baseItem = BaseItems[item.baseItemIndex];
                TrapInfo& trapInfo = TrapsInfo[baseItem.FlaskOrTrapCode];
                sprintf(InfoPanelBuffer, "%s: %.2f", "Delay time", trapInfo.ShootDelay / 20.00);
                Tooltip_AddLine_2(InfoPanelBuffer);
            }

            if (item.ArmorClass) {
                int ac = item.ArmorClass;
                const char* modificationSrt;
                if (ac > 0) {
                    modificationSrt = L("increased");
                }
                else {
                    ac = -ac;
                    modificationSrt = L("reduced");
                }
                sprintf(InfoPanelBuffer, L("armor class %s by: %i"), modificationSrt, ac);
                Tooltip_AddLine_2(InfoPanelBuffer);
            }
        }
    }

    if (item.TypeID == ITEM_2_ARMOR) {
        if (item.damageFromEnemies) {
            sprintf(InfoPanelBuffer, L("%+i damage from enemies"), item.damageFromEnemies);
            Tooltip_AddLine_2(InfoPanelBuffer);
        }
        if (item.ArmorClass) {
            sprintf(InfoPanelBuffer, L("armor class: %i"), item.ArmorClass);
            Tooltip_AddLine_2(InfoPanelBuffer);
        }
        if (item.minMeleeAbsorbPercent || item.maxMeleeAbsorbPercent) {
            sprintf(InfoPanelBuffer, L("resist melee damage: "));
            if (item.minMeleeAbsorbPercent == item.maxMeleeAbsorbPercent) {
                sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.maxMeleeAbsorbPercent);
            }
            else {
                sprintf(InfoPanelBuffer, L("%s%i to %i"), InfoPanelBuffer, item.minMeleeAbsorbPercent, item.maxMeleeAbsorbPercent);
            }
            Tooltip_AddLine_2(InfoPanelBuffer);
        }
        if (item.minArrowAbsorbPercent || item.maxArrowAbsorbPercent) {
            sprintf(InfoPanelBuffer, L("resist arrow damage: "));
            if (item.minArrowAbsorbPercent == item.maxArrowAbsorbPercent) {
                sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.maxArrowAbsorbPercent);
            }
            else {
                sprintf(InfoPanelBuffer, L("%s%i to %i"), InfoPanelBuffer, item.minArrowAbsorbPercent, item.maxArrowAbsorbPercent);
            }
            Tooltip_AddLine_2(InfoPanelBuffer);
        }
        if (item.minThorns || item.maxThorns) {
            sprintf(InfoPanelBuffer, "thorns damage: ");
            if (item.minThorns == item.maxThorns) {
                sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.minThorns);
            }
            else {
                sprintf(InfoPanelBuffer, "%s%i-%i", InfoPanelBuffer, item.minThorns, item.maxThorns);
            }
            Tooltip_AddLine_2(InfoPanelBuffer);
        }
        if (item.MinDamage || item.MaxDamage) {
            int minDmg = item.MinDamage;
            int maxDmg = item.MaxDamage;
            const char* modificationSrt;
            if (minDmg > 0) {
                modificationSrt = L("increased");
            }
            else {
                minDmg = -minDmg;
                maxDmg = -maxDmg;
                modificationSrt = L("reduced");
            }
            if (item.MinDamage == item.MaxDamage) {
                sprintf(InfoPanelBuffer, L("damage %s by: %i"), modificationSrt, minDmg);
            }
            else {
                sprintf(InfoPanelBuffer, L("damage %s by: %i-%i"), modificationSrt, minDmg, maxDmg);
            }
            Tooltip_AddLine_2(InfoPanelBuffer);
        }
    }

	if( item.dropType & D_QUENCHED && (identified || item.MagicLevel == ML_0_USUAL) ){
		for( int i = 0; i < countof(item.effect); ++i ){
			Effect& e = item.effect[i];
			if( e.id != AE_NO && e.origin == AO_QUENCH ){
				if( PrintItemEffectToStringBuffer(e.id, &item, i) ){
					char temp[256]; sprintf(temp, L("Quenched: %s"), InfoPanelBuffer);
					Tooltip_AddLine_2(temp, C_1_Blue);
				}
			}
		}
	}

    for (int i = 0; i < baseItem.EffectCount; ++i) {
        int effectIndex = 20 + i;
		if( PrintItemEffectToStringBuffer(baseItem.Effects[i].id, &item, effectIndex) ){
			Tooltip_AddLine_2(InfoPanelBuffer);
		}
    }

    // Effects
    if (identified) {
		for( int i = 0; i < countof(item.effect); ++i ){
			Effect& e = item.effect[i];
			if( e.id != AE_NO && e.origin < AO_BASE && PrintItemEffectToStringBuffer(e.id, &item, i) ){
				Tooltip_AddLine_2(InfoPanelBuffer, C_1_Blue);
			}
		}
    }else{
        if (item.ItemCode == IC_12_RING || item.ItemCode == IC_13_AMULET || item.TypeID == ITEM_1_WEAPON || item.TypeID == ITEM_2_ARMOR) {
            if (item.MagicLevel) {
                Tooltip_AddLine_2(L("Not Identified"), C_2_Red);
            }
        }
    }

    // Usage effects
    {
        MAGIC_CODE magicCode = item.MagicCode;
        int restoreDurMin = 0;
        int restoreDurMax = 0;
        int addDurMin = 0;
        int addDurMax = 0;
        int addDurLimit = 0;

        switch (magicCode) {
        case MC_31_OIL_OF_REPAIR_I:
            restoreDurMin = 1;
            restoreDurMax = 10;
            break;
        case MC_32_OIL_OF_REPAIR_II:
            restoreDurMin = 6;
            restoreDurMax = 15;
            break;
        case MC_33_OIL_OF_REPAIR_III:
            restoreDurMin = 11;
            restoreDurMax = 20;
            break;
        case MC_34_OIL_OF_REPAIR_IV:
            restoreDurMin = 16;
            restoreDurMax = 25;
            break;
        case MC_35_OIL_OF_REPAIR_V:
            restoreDurMin = 21;
            restoreDurMax = 30;
            break;
        case MC_36_BLACKSMITH_OIL:
            addDurMin = 1;
            addDurMax = 3;
            addDurLimit = 100;
            break;
        case MC_37_OIL_OF_FORTITUDE:
            addDurMin = addDurMax = 1;
            addDurLimit = 300;
            break;
        case MC_38_OIL_OF_REPAIR_VI:
            restoreDurMin = 26;
            restoreDurMax = 35;
            break;
        case MC_39_OIL_OF_REPAIR_VII:
            restoreDurMin = 31;
            restoreDurMax = 45;
            break;
        case MC_40_OIL_OF_HARDENING:
            addDurMin = 1;
            addDurMax = 2;
            addDurLimit = 150;
            break;
        case MC_2_POTION_OF_FULL_HEALING:
        {
            if (HasTrait(CurrentPlayerIndex, TraitId::Giant)) {
                Tooltip_AddLine_2(L("recover 50% of your life"));
            }
            else {
                Tooltip_AddLine_2(L("fully recover life"));
            }
        }
            break;
        case MC_3_POTION_OF_HEALING:
            Tooltip_AddLine_2(L("recover partial life"));
            break;
        case MC_6_POTION_OF_MANA:
            Tooltip_AddLine_2(L("recover partial mana"));
            break;
        case MC_7_POTION_OF_FULL_MANA:
        {
            if (HasTrait(CurrentPlayerIndex, TraitId::Giant)) {
                Tooltip_AddLine_2(L("recover 50% of your mana"));
            }
            else {
                Tooltip_AddLine_2(L("fully recover mana"));
            }
        }
            break;
        case MC_10_ELIXIR_OF_STRENGTH:
            Tooltip_AddLine_2(L("increases strength"));
            break;
        case MC_11_ELIXIR_OF_MAGIC:
            Tooltip_AddLine_2(L("increases magic"));
            break;
        case MC_12_ELIXIR_OF_DEXTERITY:
            Tooltip_AddLine_2(L("increases dexterity"));
            break;
        case MC_13_ELIXIR_OF_VITALITY:
            Tooltip_AddLine_2(L("increases vitality"));
            break;
        case MC_44_SPECTRAL_ELIXIR:
            Tooltip_AddLine_2(L("increases all stats"));
            break;
        case MC_18_POTION_OF_REJUVENATION:
            Tooltip_AddLine_2(L("recover life and mana"));
            break;
        case MC_19_POTION_OF_FULL_REJUVENATION:
        {
            if (HasTrait(CurrentPlayerIndex, TraitId::Giant)) {
                Tooltip_AddLine_2(L("recover 50% of life and mana"));
            }
            else {
                Tooltip_AddLine_2(L("fully recover life and mana"));
            }
        }
            break;
        case MC_47_RUNE_OF_FIRE:
        case MC_50_RUNE_OF_BLAZE:
            Tooltip_AddLine_2("Explodes with Fire Nova");
            break;
        case MC_48_RUNE_OF_LIGHTNING:
        case MC_49_RUNE_OF_SHOCK:
            Tooltip_AddLine_2("Explodes with Lightning Nova");
            break;
        case MC_51_RUNE_OF_STONE:
            Tooltip_AddLine_2("sets petrification trap");
            break;
        case MC_GEM:
        {
            optional<Gem> gemInfoOpt = FindGemInfoByBaseItemIndex(item.baseItemIndex);
            if (gemInfoOpt) {
                bool isSocketingPanel = IsCraftPanelVisible && CurrentCraftPage == CraftPage::CP_Socketing;

                bool isTwoHanded = [&]() {
                    if (isSocketingPanel) {
                        Item& socketedItem = Craft_GetSocketedItem();
                        if (socketedItem.ItemCode != IC_M1_NONE) {
                            return socketedItem.EquippedLocation == EL_2_TWO_HANDED;
                        }
                    }
                    return false;
                }();

                if (isSocketingPanel && isCraftSlot) {
                    Item& socketedItem = Craft_GetSocketedItem();
                    if (socketedItem.ItemCode != IC_M1_NONE) {
                        BaseItem& socketedBaseItem = BaseItems[socketedItem.baseItemIndex];
                        PrintGemEffectToStringBuffer(socketedBaseItem.ItemType == ITEM_1_WEAPON ? gemInfoOpt->weaponEffect : gemInfoOpt->armorEffect, isTwoHanded);
                        Tooltip_AddLine_2(InfoPanelBuffer);

                        int price = Craft_Socketing_CalculateGemExtractionFinalPrice(gemInfoOpt->grade);
                        sprintf(InfoPanelBuffer, L("Extraction price: %i"), price);
                        Tooltip_AddLine_2(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }
                else {
                    Tooltip_AddLine_2(L("can be inserted into socketed items"));

                    PrintGemEffectToStringBuffer(gemInfoOpt->weaponEffect);
                    sprintf(buffer, L("weapons: %s"), InfoPanelBuffer);
                    if (gemInfoOpt->weaponEffect.type == GemEffectType::Damage) {
                        if (HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
                            sprintf(buffer, "%s", buffer);
                        }
                        else {
                            sprintf(buffer, L("%s (2X FOR 2H WEAPONS)"), buffer);
                        }
                    }
                    Tooltip_AddLine_2(buffer);

                    PrintGemEffectToStringBuffer(gemInfoOpt->armorEffect);
                    if (gemInfoOpt->type == GEM::Chrysolite) {
                        sprintf(buffer, L("body armors: %s"), InfoPanelBuffer);
                    }
                    else {
                        sprintf(buffer, L("armors: %s"), InfoPanelBuffer);
                    }
                    Tooltip_AddLine_2(buffer);

                    if (isSocketingPanel && !isCraftSlot) {
                        if (Craft_Socketing_IsSocketingItemReady()) {
                            if (Craft_Socketing_IsGemSuitableForCurrentItem(*gemInfoOpt)) {
                                int price = Craft_Socketing_CalculateGemInsertFinalPrice(gemInfoOpt->grade);
                                sprintf(InfoPanelBuffer, L("Insertion price: %i"), price);
                                Tooltip_AddLine_2(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                            }
                            else {
                                Tooltip_AddLine_2(L("Not suitable for current item"), C_2_Red);
                            }
                        }
                    }
                }
                if (IsCraftPanelVisible && CurrentCraftPage == CraftPage::CP_Gems && !isCraftSlot) {
                    Tooltip_AddLine_2(L("Shift click to move to crafting slot"));
                }
            }
            break;
        }
        }

        if (restoreDurMin || restoreDurMax) {
            if (restoreDurMin == restoreDurMax) {
                sprintf(InfoPanelBuffer, L("restores %i points"), restoreDurMin);
            }
            else {
                sprintf(InfoPanelBuffer, L("restores %i-%i points"), restoreDurMin, restoreDurMax);
            }
            Tooltip_AddLine_2(InfoPanelBuffer);

            Tooltip_AddLine_2(L("of current durability"));
        }

        if (addDurMin || addDurMax) {
            if (addDurMin == addDurMax) {
                sprintf(InfoPanelBuffer, L("+%i to maxdurability"), addDurMin);
            }
            else {
                sprintf(InfoPanelBuffer, L("+[%i-%i] to max durability"), addDurMin, addDurMax);
            }
            if (addDurLimit) {
                sprintf(InfoPanelBuffer, "%s,", InfoPanelBuffer);
            }
            Tooltip_AddLine_2(InfoPanelBuffer);

            if (addDurLimit) {
                sprintf(InfoPanelBuffer, L("up to %i points"), addDurLimit);
                Tooltip_AddLine_2(InfoPanelBuffer);
                int elTorso = addDurLimit * 2;
                sprintf(InfoPanelBuffer, L("(%i for body armor)"), elTorso);
                Tooltip_AddLine_2(InfoPanelBuffer);
            }
        }

        if (IsValueStoneItem(item.baseItemIndex)) {
            Tooltip_AddLine_2(L("Sell in town for gold"));
        }

    }

    if( item.socketsAmount > 0 && (not(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SPEEDRUN) || item.MagicLevel < ML_2_UNIQUE) ){
        sprintf(InfoPanelBuffer, L("Socketed [%i]"), item.socketsAmount);
        Tooltip_AddLine_2(InfoPanelBuffer, C_1_Blue);
	}
	for( int i = 0; i < countof(item.effect); ++i ){
		Effect& e = item.effect[i];
		if( e.id != AE_NO && e.origin == AO_GEM && PrintItemEffectToStringBuffer(e.id, &item, i) ){
			Tooltip_AddLine_2(InfoPanelBuffer, C_1_Blue);
		}
	}

    // Usage hint
    if (!IsVisualTradePanelVisible && !IsStashPanelVisible)
    {
        switch (item.MagicCode) {
        case MC_2_POTION_OF_FULL_HEALING:
        case MC_3_POTION_OF_HEALING:
        case MC_6_POTION_OF_MANA:
        case MC_7_POTION_OF_FULL_MANA:
        case MC_18_POTION_OF_REJUVENATION:
        case MC_19_POTION_OF_FULL_REJUVENATION:
        case MC_10_ELIXIR_OF_STRENGTH:
        case MC_11_ELIXIR_OF_MAGIC:
        case MC_12_ELIXIR_OF_DEXTERITY:
        case MC_13_ELIXIR_OF_VITALITY:
        case MC_44_SPECTRAL_ELIXIR:
        case MC_30_OIL_OF_SOMETHING:
        case MC_31_OIL_OF_REPAIR_I:
        case MC_32_OIL_OF_REPAIR_II:
        case MC_33_OIL_OF_REPAIR_III:
        case MC_34_OIL_OF_REPAIR_IV:
        case MC_35_OIL_OF_REPAIR_V:
        case MC_36_BLACKSMITH_OIL:
        case MC_37_OIL_OF_FORTITUDE:
        case MC_38_OIL_OF_REPAIR_VI:
        case MC_39_OIL_OF_REPAIR_VII:
        case MC_40_OIL_OF_HARDENING:
        case MC_47_RUNE_OF_FIRE:
        case MC_48_RUNE_OF_LIGHTNING:
        case MC_49_RUNE_OF_SHOCK:
        case MC_50_RUNE_OF_BLAZE:
        case MC_51_RUNE_OF_STONE:
            Tooltip_AddLine_2(L("Right click to use"));
            break;
        case MC_21_RELIC_NEED_NO_TARGET:
        case MC_24_BOOKS:
        case MC_54_RECONSTRUCTED_NOTE:
            Tooltip_AddLine_2(L("Right click to read"));
            break;
        case MC_22_RELIC_NEED_TARGET:
            Tooltip_AddLine_2(L("Right-click to read, then"));
            Tooltip_AddLine_2(L("left-click to target"));
            break;
        case MC_42_MAP_OF_THE_STARS:
            Tooltip_AddLine_2(L("Right click to view")); // (th4) localizable
            break;
        case MC_43_EAR_HEART:
            sprintf(InfoPanelBuffer, L("Level : %i"), item.quality); // (th4) localizable
            Tooltip_AddLine_2(InfoPanelBuffer);
            break;
        case MC_53_AURIC_AMULET:
            Tooltip_AddLine_2("Doubles gold capacity");
            break;
        }

        if (item.baseItemIndex == BI_33_STAFF_OF_LAZARUS && HasTrait(CurrentPlayerIndex, TraitId::Leper)) {
            Tooltip_AddLine_2(L("Right click to use"));
        }
        else if (isTownPortalForbidden()) {
            switch (item.baseItemIndex) {
            case BI_19_FUNGAL_TOME:		 Tooltip_AddLine_2(L("Right click to transmute into spectral elixir")); break;
            case BI_2512_FULL_CONTAINERS:Tooltip_AddLine_2(L("Right click to transmute into random elixir")); break;
            case BI_33_STAFF_OF_LAZARUS: Tooltip_AddLine_2(L("Right click to use")); break;
            case BI_16_ANVIL_OF_FURY:
            case BI_9_MAGIC_ROCK:
            case BI_327_TAVERN_SIGN:
            case BI_2457_IRON_FIGURINE:
            case BI_2459_REAGENTS:
            case BI_2460_THE_RIGHT_GOODS: 
            case BI_2510_DUSTY_CRATE:
            case BI_2461_MUNDANE_INVENTORY:
            case BI_334_THEODORE:		 Tooltip_AddLine_2("Right click to transmute into quest reward"); break;
            }
        }
        if (isTownPortalForbidden()) {
            if (item.baseItemIndex >= BI_296_QUARTZ && item.baseItemIndex <= BI_301_AQUAMARINE) {
                Tooltip_AddLine_2("Right click to transmute into gold");
            }
        }
        if (item.baseItemIndex == BI_2472_THE_LOCKED_CHEST) {
            Tooltip_AddLine_2("Right click to open (requires key)"); 
        }
        if (item.baseItemIndex == BI_2476_THE_STRANGE_BOOK) {
            Tooltip_AddLine_2(L("Right click to read (requires crystal eyepiece)"));
        }

        if (IsItemAllowedForBelt(item)) {
            if (slot >= IS_50_47_Belt_Start && slot < IS_50_47_Belt_End) {
                Tooltip_AddLine_2(L("Shift Click to move left"));
			}else if( ! IsTransmutePanelVisible ){
                Tooltip_AddLine_2(L("Shift Click to move to belt"));
            }
        }
    }

    // Requirements
    if (item.MagicCode == MC_24_BOOKS && item.RequiredLevel == 255u) {
        Tooltip_AddLine_2(L("Spell is maxed"), C_3_Gold);
    }
    else if (item.RequiredMagic || item.RequiredStrength || item.RequiredDexterity || item.RequiredVitality || item.RequiredLevel) {
        strcpy(InfoPanelBuffer, L("required:"));

        auto appendReqInfo = [&player](const char* name, int req, int current, int magicCode, bool isLevel = false)
        {
            if (req) {
                uchar color;
                if (current >= req) {
                    color = C_0_White + 128;
                }
                else if (!isLevel && req <= current + (is(magicCode,
                    MC_24_BOOKS, MC_21_RELIC_NEED_NO_TARGET, MC_22_RELIC_NEED_TARGET,
                    MC_47_RUNE_OF_FIRE, MC_48_RUNE_OF_LIGHTNING, MC_49_RUNE_OF_SHOCK,
                    MC_50_RUNE_OF_BLAZE, MC_51_RUNE_OF_STONE) ? 0 : player.AdvancedUser)) {
                    color = C_1_Blue + 128;
                }
                else {
                    color = C_2_Red + 128;
                }
                sprintf(InfoPanelBuffer, "%s %c%i %s%c", InfoPanelBuffer, color, req, name, color);
            }
        };

        appendReqInfo(L("lvl"), item.RequiredLevel, player.CharLevel, item.MagicCode, true); // (th4) isLevel: level is never lowered by Advanced User, so it goes straight from white to red. This used to be `name != "lvl"`, a POINTER compare: name is L("lvl"), a different pointer in every translated language, so a missed level requirement showed blue there. Same fix in all three tooltip copies.
        appendReqInfo(L("str"), item.RequiredStrength, player.CurStrength, item.MagicCode);
        appendReqInfo(L("mag"), item.RequiredMagic, player.CurMagic, item.MagicCode);
        appendReqInfo(L("dex"), item.RequiredDexterity, player.CurDexterity, item.MagicCode);
        appendReqInfo(L("vit"), item.RequiredVitality, player.CurVitality, item.MagicCode);
        Tooltip_AddLine_2(InfoPanelBuffer);
    }

    uint playerFullClassMask = (1u << static_cast<uint>(player.fullClassId));
    uint requiredClassMask = GetRequiredClassMask(&item);
    bool sort_out = false;
    if (requiredClassMask == IPCM_ALL_CLASSES) {
        sort_out = true;
    }
    else if (player.gameChanger & BIT(GC_27_TERMINATOR)
        || player.gameChanger & BIT(GC_28_SUPERMUTANT)
        || player.gameChanger & BIT(GC_29_ENCLAVE_SOLDIER)) {
        sort_out = true;
    }
    if (!sort_out){
        printItemClassReqInfoInBuffer(&item);
        Tooltip_AddLine_2(InfoPanelBuffer, (playerFullClassMask & requiredClassMask) ? C_0_White : C_2_Red);
    }

    if ((playerFullClassMask & IPCM_ANY_EXILE) != 0 && baseItem.ForbiddenForExiles) {
        Tooltip_AddLine_2("[Can't wear item]", C_2_Red);
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::OldFashioned)) {
        if (is(item.ItemCode, IC_6_LIGHT_ARMOR, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR)) {
            Tooltip_AddLine_2("[Can't wear body armor]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::Fechtmeister, TraitId::Ranger, TraitId::Mamluk)) {
        if (is(item.ItemCode, IC_5_SHIELD)) {
            Tooltip_AddLine_2("[Can't use shields]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::Black_Witchery)) {
        if (is(item.ItemCode, IC_21_KNIFE)) {
            Tooltip_AddLine_2("[Can't use throwing knives]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::Paladin) || player.fullClassId == PFC_DRUID) {
        if (is(item.ItemCode, IC_3_BOW)) {
            Tooltip_AddLine_2("[Can't use crossbows]", C_2_Red);
        }
        else if (is(item.ItemCode, IC_22_MALLET)) {
            Tooltip_AddLine_2("[Can't use throwing mallets]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::TwoTowers)) {
        if (is(item.ItemCode, IC_1_SWORD, IC_4_MACE)) {
            Tooltip_AddLine_2("[Can't use melee weapons]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::Axepertise)) {
        if (is(item.ItemCode, IC_1_SWORD, IC_4_MACE)) {
            Tooltip_AddLine_2("[Can't use item]", C_2_Red);
        }
    }
    if (HasTrait(CurrentPlayerIndex, TraitId::WildSorcery) || HasTrait(CurrentPlayerIndex, TraitId::Rabid)) {
        if (is(item.MagicCode,
            MC_3_POTION_OF_HEALING,
            MC_6_POTION_OF_MANA,
            MC_2_POTION_OF_FULL_HEALING,
            MC_7_POTION_OF_FULL_MANA,
            MC_18_POTION_OF_REJUVENATION,
            MC_19_POTION_OF_FULL_REJUVENATION)) {
            Tooltip_AddLine_2("[Can't use potions]", C_2_Red);
        }
    }
    //----------------------
    if (IsVisualTradePanelVisible) {

        if (isVisualTradeSlot) {
            int price = VisualTrade_GetBuyPrice(&item);
            sprintf(InfoPanelBuffer, L("Buy price: %i"), price);
            Tooltip_AddLine_2(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
            Tooltip_AddLine_2(L("Shift click to buy fast"));
        }
        else {
            if (Cur.GraphicsID == CM_1_NORMAL_HAND) {
                if (CurrentVisualTradeMode != VTM_Elder) {
                    int price = VisualTrade_GetSellPrice(&item);
                    if (price) {
                        sprintf(InfoPanelBuffer, L("Sell price: %i"), price);
                        Tooltip_AddLine_2(InfoPanelBuffer);
                        Tooltip_AddLine_2(L("Shift click to sell fast"));
                    }
                    else {
                        Tooltip_AddLine_2(L("Item cannot be sold"), C_2_Red);
                    }
                }
            }
            else if (Cur.IsTradeService) {
                if (Cur.GraphicsID == CM_2_IDENTIFY) {
                    int price = VisualTrade_GetIdentificationPrice(&item);
                    if (price) {
                        sprintf(InfoPanelBuffer, "Identification price: %i", price);
                        Tooltip_AddLine_2(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }
                else if (Cur.GraphicsID == CM_3_REPAIR) {
                    int price = VisualTrade_GetRepairPrice(&item);
                    if (price) {
                        sprintf(InfoPanelBuffer, "Repair price: %i", price);
                        Tooltip_AddLine_2(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }
                else if (Cur.GraphicsID == CM_4_STAFF_RECHARGE) {
                    int price = VisualTrade_GetRechargePrice(&item);
                    if (price) {
                        sprintf(InfoPanelBuffer, "Recharge price: %i", price);
                        Tooltip_AddLine_2(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }
            }
        }
    }

    if (IsStashPanelVisible) {
        if (slot >= IS_Inventory) {
            if (!IsItemAllowedForStash(&item)) {
                Tooltip_AddLine_2(L("Cannot be stashed"), C_2_Red);
            }
            else {
                if (isVisualStashSlot) {
                    Tooltip_AddLine_2(L("Shift Click to move to inventory"));
                }
                else {
                    Tooltip_AddLine_2(L("Shift Click to move to stash"));
                }
            }
        }
    }
	if( IsTransmutePanelVisible && Transmute_Allowed(item) ) {
		if( slot >= Stash_SlotIDStart ){
			Tooltip_AddLine_2(L("Shift Click to move to inventory"), C_7_Grey);
		}else if (slot >= IS_Inventory) {
			Tooltip_AddLine_2(L("Shift Click to transmute panel"), C_7_Grey);
		}
	}

    if (identified && item.MagicLevel == ML_3_SET) {
        UniqueItem& uniq = UniqueItems[item.UniqueItemIndex];
        UniqueSet& set = UniqueSets[uniq.uniqueSetIndex];

        Tooltip_AddLine_2(""); // blank line
        bool hasActiveEffects = false;
        int countOfEquippedItemsFromSet = SetItem_GetCountOfEquippedItemsFromSet(CurrentPlayerIndex, uniq.uniqueSetIndex);
        for (int i = 0, ie = set.EffectCount; i != ie; ++i) {
            auto& e = set.Effects[i];
            if (countOfEquippedItemsFromSet >= e.Threshold) {
				if( PrintItemEffectToStringBuffer(e.Effect.id, const_cast<Item*>(&set.FakeItemToShowEffect), i) ){
					Tooltip_AddLine_2(InfoPanelBuffer, C_4_Orange);
				}
				hasActiveEffects = true;
			}
        }
        if (hasActiveEffects) Tooltip_AddLine_2(""); // blank line
        Tooltip_AddLine_2(LocalizedUniqueItemName(set.Name), C_3_Gold);
        for (int i = 0; i < set.UniquesCount; ++i)
        {
            int uniqueItemIndex = set.UniquesList[i];
            UniqueItem& uniq2 = UniqueItems[uniqueItemIndex];
            Tooltip_AddLine_2(LocalizedUniqueItemName(uniq2.Name), SetItem_IsItemFromSetEquipped(CurrentPlayerIndex, uniqueItemIndex) ? C_1_Blue : C_2_Red);
        }
    }
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_ShowItemInfo_3(int slot)
{
    if (slot == -1) {
        return;
    }

    //Tooltip_Clear();

    Tooltip_AddLine_3("Equipped", C_7_Grey);

    char buffer[256];
    bool isVisualTradeSlot = false;
    bool isVisualStashSlot = false;
    bool isCraftSlot = false;
	Player& player = Players[CurrentPlayerIndex];
	Item& item = player.OnBodySlots[slot];

	if (item.ItemCode == IC_M1_NONE) return;

    BaseItem& baseItem = BaseItems[item.baseItemIndex];

    int nameColor = ItemColor(item);
    bool identified = item.Identified || item.MagicLevel == ML_0_USUAL;
    bool rare = item.dropType & D_RARE;

    const char* namePtr = nullptr;
    if (identified) {
        namePtr = item.FullMagicalItemName;
    }
    else {
        namePtr = item.Name;
    }

    if (item.ItemCode == IC_11_GOLD) {
        sprintf(InfoPanelBuffer, L("%i gold"), item.amount);
        namePtr = InfoPanelBuffer;
    }

    // Skip * in set's name
    if (*namePtr == '*') {
        ++namePtr;
    }

    Tooltip_AddLine_3(namePtr, nameColor);

    if (rare) {
        Tooltip_AddLine_3("Rare Item", nameColor);
    }
    else if (item.MagicLevel == ML_2_UNIQUE) {
        Tooltip_AddLine_3(L("Unique Item"), nameColor);
    }
    else if (item.MagicLevel == ML_3_SET) {
        Tooltip_AddLine_3(L("Set Item"), nameColor);
    }
    else if (IsQuestItem(item.baseItemIndex)) {
        Tooltip_AddLine_3(L("Quest item"), nameColor);
    }

    if (item.TypeID == ITEM_1_WEAPON || item.TypeID == ITEM_2_ARMOR) {
        if (item.BaseDurability) {
            if (item.BaseDurability == ITEM_DUR_INDESTRUBLE) {
                Tooltip_AddLine_3("indestructible", C_3_Gold);
            }
            else {
                char curDurColor = C_0_White;
                char baseDurColor = C_0_White;

                if (item.CurDurability <= 1) {
                    curDurColor = C_2_Red;
                }
                else if (item.CurDurability >= 2 && item.CurDurability <= 5) {
                    curDurColor = C_5_Yellow;
                }

                if (item.BaseDurability <= 1) {
                    baseDurColor = C_2_Red;
                }
                else if (item.CurDurability <= 1) {
                    baseDurColor = C_5_Yellow;
                }

                curDurColor += 128;
                baseDurColor += 128;

                sprintf(InfoPanelBuffer, L("durability: %c%i%c of %c%i%c"), curDurColor, item.CurDurability, curDurColor, baseDurColor, item.BaseDurability, baseDurColor);
                Tooltip_AddLine_3(InfoPanelBuffer);
            }
        }
    }

    if (item.BaseCharges) {
        char color = ((item.CurCharges > 0) ? C_0_White : C_2_Red) + 128;
        sprintf(InfoPanelBuffer, "%s charges: %c%i%c of %i", getSpellName(item.SpellIndex), color, item.CurCharges, color, item.BaseCharges);
        Tooltip_AddLine_3(InfoPanelBuffer);
    }

    if (item.TypeID == ITEM_1_WEAPON) {
        if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk) || GameMode == GM_CLASSIC) {
            goto MUM_2;
        }
        else if (!is(player.fullClassId, PFC_MAGE, PFC_ELEMENTALIST, PFC_WARLOCK, PFC_NECROMANCER, PFC_DEMONOLOGIST)) {
        MUM_2:
            const char* damageName = L("damage");
            if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
                goto MUMZ_5;
            }
            if (BaseItems[item.baseItemIndex].AnimationDelay) {
                damageName = L("slow damage");
            }
            else if (item.ItemCode == IC_1_SWORD && player.fullClassId != PFC_INQUISITOR && player.fullClassId != PFC_DRUID) {
                damageName = L("slashing damage");
            }
            else if (item.ItemCode == IC_4_MACE && player.fullClassId != PFC_INQUISITOR && player.fullClassId != PFC_DRUID) {
                damageName = L("crushing damage");
            }
            MUMZ_5:
            sprintf(InfoPanelBuffer, L("%s: "), damageName);
            if (item.overTime) {
                if (item.MinDamage == item.MaxDamage) {
                    sprintf(InfoPanelBuffer, "%s%i over %i sec", InfoPanelBuffer, item.MinDamage, item.overTime);
                }
                else {
                    sprintf(InfoPanelBuffer, "%s%i-%i over %i sec", InfoPanelBuffer, item.MinDamage, item.MaxDamage, item.overTime);
                }
            }
            else {
                if (item.MinDamage == item.MaxDamage) {
                    sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.MinDamage);
                }
                else {
                    sprintf(InfoPanelBuffer, "%s%i-%i", InfoPanelBuffer, item.MinDamage, item.MaxDamage);
                }
            }
            Tooltip_AddLine_3(InfoPanelBuffer);

            if (!(player.gameChanger & BIT(GC_10_HACKNSLASH)) && GameMode != GM_COLISEUM) {
                if (item.ItemCode == IC_1_SWORD) {
                    if (player.fullClassId == PFC_THRAEX) {
                        Tooltip_AddLine_3(L("(+50% to beasts)"));
                    }
                    else if (player.fullClassId == PFC_INQUISITOR || player.fullClassId == PFC_DRUID || HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
                        //Tooltip_AddLine("(+50% to beasts, -50% to undead)");
                    }
                    else if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS) {
                        Tooltip_AddLine_3(L("(+15% to beasts, -50% to undead)"));
                    }
                    else if (player.fullClassId == PFC_SECUTOR && HasTrait(CurrentPlayerIndex, TraitId::Bestiarius)) {
                        Tooltip_AddLine_3(L("(+80% to beasts, -50% to undead)"));
                    }
                    else if (player.fullClassId == PFC_KENSEI) {
                        Tooltip_AddLine_3(L("(+30% to beasts, -50% to undead)"));
                    }
                    else if (player.fullClassId == PFC_GUARDIAN) {
                        Tooltip_AddLine_3(L("(+40% to beasts, -50% to undead)"));
                    }
                    else {
                        Tooltip_AddLine_3(L("(+50% to beasts, -50% to undead)"));
                    }
                }
                else if (item.ItemCode == IC_4_MACE) {
                    if (player.fullClassId == PFC_TEMPLAR) {
                        Tooltip_AddLine_3(L("(+70% to undead, -50% to beasts)"));
                    }
                    else if (player.fullClassId == PFC_INQUISITOR || player.fullClassId == PFC_DRUID) {
                        //Tooltip_AddLine("(+70% to undead, -50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS) {
                        Tooltip_AddLine_3(L("(+15% to undead, -50% to beasts)"));
                    }
                    else if (player.fullClassId == PFC_SECUTOR && HasTrait(CurrentPlayerIndex, TraitId::Bestiarius)) {
                        Tooltip_AddLine_3(L("(+25% to undead, -50% to beasts)"));
                    }
                    else if (player.fullClassId == PFC_KENSEI) {
                        Tooltip_AddLine_3(L("(+30% to undead, -50% to beasts)"));
                    }
                    else if (player.fullClassId == PFC_GUARDIAN) {
                        Tooltip_AddLine_3(L("(+40% to undead, -50% to beasts)"));
                    }
                    else {
                        Tooltip_AddLine_3(L("(+50% to undead, -50% to beasts)"));
                    }
                }
                else if (item.ItemCode == IC_10_STAFF && player.fullClassId == PFC_MONK) {
                    Tooltip_AddLine_3(L("(+20% to demons, -20% to beasts)"));
                }
                else if (item.ItemCode == IC_20_CLAW && player.fullClassId == PFC_ASSASSIN) {//as if smb else wields claws...
                    Tooltip_AddLine_3(L("(+20% to beasts, -20% to undead)"));
                }//else if (item.ItemCode == IC_21_KNIFE && player.fullClassId == PFC_ASSASSIN) {//as if smb else wields knifes...
                 //   Tooltip_AddLine("(+PLACEHOLDER% to PLACEHOLDER, -PLACEHOLDER% to PLACEHOLDER)");
                //}
            }
            if (item.ItemCode == IC_19_TRAP) {
                BaseItem& baseItem = BaseItems[item.baseItemIndex];
                TrapInfo& trapInfo = TrapsInfo[baseItem.FlaskOrTrapCode];
                sprintf(InfoPanelBuffer, "%s: %.2f", "Delay time", trapInfo.ShootDelay / 20.00);
                Tooltip_AddLine_3(InfoPanelBuffer);
            }

            if (item.ArmorClass) {
                int ac = item.ArmorClass;
                const char* modificationSrt;
                if (ac > 0) {
                    modificationSrt = L("increased");
                }
                else {
                    ac = -ac;
                    modificationSrt = L("reduced");
                }
                sprintf(InfoPanelBuffer, L("armor class %s by: %i"), modificationSrt, ac);
                Tooltip_AddLine_3(InfoPanelBuffer);
            }
        }
    }

    if (item.TypeID == ITEM_2_ARMOR) {
        if (item.damageFromEnemies) {
            sprintf(InfoPanelBuffer, L("%+i damage from enemies"), item.damageFromEnemies);
            Tooltip_AddLine_3(InfoPanelBuffer);
        }
        if (item.ArmorClass) {
            sprintf(InfoPanelBuffer, L("armor class: %i"), item.ArmorClass);
            Tooltip_AddLine_3(InfoPanelBuffer);
        }
        if (item.minMeleeAbsorbPercent || item.maxMeleeAbsorbPercent) {
            sprintf(InfoPanelBuffer, L("resist melee damage: "));
            if (item.minMeleeAbsorbPercent == item.maxMeleeAbsorbPercent) {
                sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.maxMeleeAbsorbPercent);
            }
            else {
                sprintf(InfoPanelBuffer, L("%s%i to %i"), InfoPanelBuffer, item.minMeleeAbsorbPercent, item.maxMeleeAbsorbPercent);
            }
            Tooltip_AddLine_3(InfoPanelBuffer);
        }
        if (item.minArrowAbsorbPercent || item.maxArrowAbsorbPercent) {
            sprintf(InfoPanelBuffer, L("resist arrow damage: "));
            if (item.minArrowAbsorbPercent == item.maxArrowAbsorbPercent) {
                sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.maxArrowAbsorbPercent);
            }
            else {
                sprintf(InfoPanelBuffer, L("%s%i to %i"), InfoPanelBuffer, item.minArrowAbsorbPercent, item.maxArrowAbsorbPercent);
            }
            Tooltip_AddLine_3(InfoPanelBuffer);
        }
        if (item.minThorns || item.maxThorns) {
            sprintf(InfoPanelBuffer, "thorns damage: ");
            if (item.minThorns == item.maxThorns) {
                sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.minThorns);
            }
            else {
                sprintf(InfoPanelBuffer, "%s%i-%i", InfoPanelBuffer, item.minThorns, item.maxThorns);
            }
            Tooltip_AddLine_3(InfoPanelBuffer);
        }
        if (item.MinDamage || item.MaxDamage) {
            int minDmg = item.MinDamage;
            int maxDmg = item.MaxDamage;
            const char* modificationSrt;
            if (minDmg > 0) {
                modificationSrt = L("increased");
            }
            else {
                minDmg = -minDmg;
                maxDmg = -maxDmg;
                modificationSrt = L("reduced");
            }
            if (item.MinDamage == item.MaxDamage) {
                sprintf(InfoPanelBuffer, L("damage %s by: %i"), modificationSrt, minDmg);
            }
            else {
                sprintf(InfoPanelBuffer, L("damage %s by: %i-%i"), modificationSrt, minDmg, maxDmg);
            }
            Tooltip_AddLine_3(InfoPanelBuffer);
        }
    }

	if( item.dropType & D_QUENCHED && (identified || item.MagicLevel == ML_0_USUAL) ){
		for( int i = 0; i < countof(item.effect); ++i ){
			Effect& e = item.effect[i];
			if( e.id != AE_NO && e.origin == AO_QUENCH ){
				if( PrintItemEffectToStringBuffer(e.id, &item, i) ){
					char temp[256]; sprintf(temp, L("Quenched: %s"), InfoPanelBuffer);
					Tooltip_AddLine_3(temp, C_1_Blue);
				}
			}
		}
	}

    for (int i = 0; i < baseItem.EffectCount; ++i) {
        const int effectIndex = 20 + i;
        if( PrintItemEffectToStringBuffer(baseItem.Effects[i].id, const_cast<Item*>(&item), effectIndex) ){
			Tooltip_AddLine_3(InfoPanelBuffer);
		}
    }

    // Effects
    if (identified) {
		for( int i = 0; i < countof(item.effect); ++i ){
			Effect& e = item.effect[i];
			if( e.id != AE_NO && e.origin < AO_BASE && PrintItemEffectToStringBuffer(e.id, &item, i) ){
				Tooltip_AddLine_3(InfoPanelBuffer, C_1_Blue);
			}
		}
    }else{
        if (item.ItemCode == IC_12_RING || item.ItemCode == IC_13_AMULET || item.TypeID == ITEM_1_WEAPON || item.TypeID == ITEM_2_ARMOR) {
            if (item.MagicLevel) {
                Tooltip_AddLine_3(L("Not Identified"), C_2_Red);
            }
        }
    }

    // Usage effects
    {
        MAGIC_CODE magicCode = item.MagicCode;
        int restoreDurMin = 0;
        int restoreDurMax = 0;
        int addDurMin = 0;
        int addDurMax = 0;
        int addDurLimit = 0;

        switch (magicCode) {
        case MC_31_OIL_OF_REPAIR_I:
            restoreDurMin = 1;
            restoreDurMax = 10;
            break;
        case MC_32_OIL_OF_REPAIR_II:
            restoreDurMin = 6;
            restoreDurMax = 15;
            break;
        case MC_33_OIL_OF_REPAIR_III:
            restoreDurMin = 11;
            restoreDurMax = 20;
            break;
        case MC_34_OIL_OF_REPAIR_IV:
            restoreDurMin = 16;
            restoreDurMax = 25;
            break;
        case MC_35_OIL_OF_REPAIR_V:
            restoreDurMin = 21;
            restoreDurMax = 30;
            break;
        case MC_36_BLACKSMITH_OIL:
            addDurMin = 1;
            addDurMax = 3;
            addDurLimit = 100;
            break;
        case MC_37_OIL_OF_FORTITUDE:
            addDurMin = addDurMax = 1;
            addDurLimit = 300;
            break;
        case MC_38_OIL_OF_REPAIR_VI:
            restoreDurMin = 26;
            restoreDurMax = 35;
            break;
        case MC_39_OIL_OF_REPAIR_VII:
            restoreDurMin = 31;
            restoreDurMax = 45;
            break;
        case MC_40_OIL_OF_HARDENING:
            addDurMin = 1;
            addDurMax = 2;
            addDurLimit = 150;
            break;
        case MC_2_POTION_OF_FULL_HEALING:
        {
            if (HasTrait(CurrentPlayerIndex, TraitId::Giant)) {
                Tooltip_AddLine_3(L("recover 50% of life"));
            }
            else {
                Tooltip_AddLine_3(L("fully recover life"));
            }
        }
            break;
        case MC_3_POTION_OF_HEALING:
            Tooltip_AddLine_3(L("recover partial life"));
            break;
        case MC_6_POTION_OF_MANA:
            Tooltip_AddLine_3(L("recover partial mana"));
            break;
        case MC_7_POTION_OF_FULL_MANA:
        {
            if (HasTrait(CurrentPlayerIndex, TraitId::Giant)) {
                Tooltip_AddLine_3(L("recover 50% of mana"));
            }
            else {
                Tooltip_AddLine_3(L("fully recover mana"));
            }
        }
            break;
        case MC_10_ELIXIR_OF_STRENGTH:
            Tooltip_AddLine_3(L("increases strength"));
            break;
        case MC_11_ELIXIR_OF_MAGIC:
            Tooltip_AddLine_3(L("increases magic"));
            break;
        case MC_12_ELIXIR_OF_DEXTERITY:
            Tooltip_AddLine_3(L("increases dexterity"));
            break;
        case MC_13_ELIXIR_OF_VITALITY:
            Tooltip_AddLine_3(L("increases vitality"));
            break;
        case MC_44_SPECTRAL_ELIXIR:
            Tooltip_AddLine_3(L("increases all stats"));
            break;
        case MC_18_POTION_OF_REJUVENATION:
            Tooltip_AddLine_3(L("recover life and mana"));
            break;
        case MC_19_POTION_OF_FULL_REJUVENATION:
        {
            if (HasTrait(CurrentPlayerIndex, TraitId::Giant)) {
                Tooltip_AddLine_3(L("recover 50% of life and mana"));
            }
            else {
                Tooltip_AddLine_3(L("fully recover life and mana"));
            }
        }
            break;
        case MC_47_RUNE_OF_FIRE:
        case MC_50_RUNE_OF_BLAZE:
            Tooltip_AddLine_3("Explodes with Fire Nova");
            break;
        case MC_48_RUNE_OF_LIGHTNING:
        case MC_49_RUNE_OF_SHOCK:
            Tooltip_AddLine_3("Explodes with Lightning Nova");
            break;
        case MC_51_RUNE_OF_STONE:
            Tooltip_AddLine_3("sets petrification trap");
            break;
        case MC_GEM:
        {
            optional<Gem> gemInfoOpt = FindGemInfoByBaseItemIndex(item.baseItemIndex);
            if (gemInfoOpt) {
                bool isSocketingPanel = IsCraftPanelVisible && CurrentCraftPage == CraftPage::CP_Socketing;

                bool isTwoHanded = [&]() {
                    if (isSocketingPanel) {
                        Item& socketedItem = Craft_GetSocketedItem();
                        if (socketedItem.ItemCode != IC_M1_NONE) {
                            return socketedItem.EquippedLocation == EL_2_TWO_HANDED;
                        }
                    }
                    return false;
                }();

                if (isSocketingPanel && isCraftSlot) {
                    Item& socketedItem = Craft_GetSocketedItem();
                    if (socketedItem.ItemCode != IC_M1_NONE) {
                        BaseItem& socketedBaseItem = BaseItems[socketedItem.baseItemIndex];
                        PrintGemEffectToStringBuffer(socketedBaseItem.ItemType == ITEM_1_WEAPON ? gemInfoOpt->weaponEffect : gemInfoOpt->armorEffect, isTwoHanded);
                        Tooltip_AddLine_3(InfoPanelBuffer);

                        int price = Craft_Socketing_CalculateGemExtractionFinalPrice(gemInfoOpt->grade);
                        sprintf(InfoPanelBuffer, L("Extraction price: %i"), price);
                        Tooltip_AddLine_3(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }
                else {
                    Tooltip_AddLine_3(L("can be inserted into socketed items"));

                    PrintGemEffectToStringBuffer(gemInfoOpt->weaponEffect);
                    sprintf(buffer, L("weapons: %s"), InfoPanelBuffer);
                    if (gemInfoOpt->weaponEffect.type == GemEffectType::Damage) {
                        if (HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
                            sprintf(buffer, "%s", buffer);
                        }
                        else {
                            sprintf(buffer, L("%s (2X FOR 2H WEAPONS)"), buffer);
                        }
                    }
                    Tooltip_AddLine_3(buffer);

                    PrintGemEffectToStringBuffer(gemInfoOpt->armorEffect);
                    if (gemInfoOpt->type == GEM::Chrysolite) {
                        sprintf(buffer, L("body armors: %s"), InfoPanelBuffer);
                    }
                    else {
                        sprintf(buffer, L("armors: %s"), InfoPanelBuffer);
                    }
                    Tooltip_AddLine_3(buffer);

                    if (isSocketingPanel && !isCraftSlot) {
                        if (Craft_Socketing_IsSocketingItemReady()) {
                            if (Craft_Socketing_IsGemSuitableForCurrentItem(*gemInfoOpt)) {
                                int price = Craft_Socketing_CalculateGemInsertFinalPrice(gemInfoOpt->grade);
                                sprintf(InfoPanelBuffer, L("Insertion price: %i"), price);
                                Tooltip_AddLine_3(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                            }
                            else {
                                Tooltip_AddLine_3(L("Not suitable for current item"), C_2_Red);
                            }
                        }
                    }
                }
                if (IsCraftPanelVisible && CurrentCraftPage == CraftPage::CP_Gems && !isCraftSlot) {
                    Tooltip_AddLine_3(L("Shift click to move to crafting slot"));
                }
            }
            break;
        }
        }

        if (restoreDurMin || restoreDurMax) {
            if (restoreDurMin == restoreDurMax) {
                sprintf(InfoPanelBuffer, L("restores %i points"), restoreDurMin);
            }
            else {
                sprintf(InfoPanelBuffer, L("restores %i-%i points"), restoreDurMin, restoreDurMax);
            }
            Tooltip_AddLine_3(InfoPanelBuffer);

            Tooltip_AddLine_3(L("of current durability"));
        }

        if (addDurMin || addDurMax) {
            if (addDurMin == addDurMax) {
                sprintf(InfoPanelBuffer, L("+%i to max durability"), addDurMin);
            }
            else {
                sprintf(InfoPanelBuffer, L("+[%i-%i] to max durability"), addDurMin, addDurMax);
            }
            if (addDurLimit) {
                sprintf(InfoPanelBuffer, "%s,", InfoPanelBuffer);
            }
            Tooltip_AddLine_3(InfoPanelBuffer);

            if (addDurLimit) {
                sprintf(InfoPanelBuffer, L("up to %i points"), addDurLimit);
                Tooltip_AddLine_3(InfoPanelBuffer);
                int elTorso = addDurLimit * 2;
                sprintf(InfoPanelBuffer, L("(%i for body armor)"), elTorso);
                Tooltip_AddLine_3(InfoPanelBuffer);
            }
        }

        if (IsValueStoneItem(item.baseItemIndex)) {
            Tooltip_AddLine_3(L("Sell in town for gold"));
        }

    }

	if( item.socketsAmount > 0 && (not(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SPEEDRUN) || item.MagicLevel < ML_2_UNIQUE) ){
		sprintf(InfoPanelBuffer, L("Socketed [%i]"), item.socketsAmount);
        Tooltip_AddLine_3(InfoPanelBuffer, C_1_Blue);
	}
	for( int i = 0; i < countof(item.effect); ++i ){
		Effect& e = item.effect[i];
		if( e.id != AE_NO && e.origin == AO_GEM && PrintItemEffectToStringBuffer(e.id, &item, i) ){
			Tooltip_AddLine_3(InfoPanelBuffer, C_1_Blue);
		}
	}

    // Usage hint
    if (!IsVisualTradePanelVisible && !IsStashPanelVisible)
    {
        switch (item.MagicCode) {
        case MC_2_POTION_OF_FULL_HEALING:
        case MC_3_POTION_OF_HEALING:
        case MC_6_POTION_OF_MANA:
        case MC_7_POTION_OF_FULL_MANA:
        case MC_18_POTION_OF_REJUVENATION:
        case MC_19_POTION_OF_FULL_REJUVENATION:
        case MC_10_ELIXIR_OF_STRENGTH:
        case MC_11_ELIXIR_OF_MAGIC:
        case MC_12_ELIXIR_OF_DEXTERITY:
        case MC_13_ELIXIR_OF_VITALITY:
        case MC_44_SPECTRAL_ELIXIR:
        case MC_30_OIL_OF_SOMETHING:
        case MC_31_OIL_OF_REPAIR_I:
        case MC_32_OIL_OF_REPAIR_II:
        case MC_33_OIL_OF_REPAIR_III:
        case MC_34_OIL_OF_REPAIR_IV:
        case MC_35_OIL_OF_REPAIR_V:
        case MC_36_BLACKSMITH_OIL:
        case MC_37_OIL_OF_FORTITUDE:
        case MC_38_OIL_OF_REPAIR_VI:
        case MC_39_OIL_OF_REPAIR_VII:
        case MC_40_OIL_OF_HARDENING:
        case MC_47_RUNE_OF_FIRE:
        case MC_48_RUNE_OF_LIGHTNING:
        case MC_49_RUNE_OF_SHOCK:
        case MC_50_RUNE_OF_BLAZE:
        case MC_51_RUNE_OF_STONE:
            Tooltip_AddLine_3(L("Right click to use"));
            break;
        case MC_21_RELIC_NEED_NO_TARGET:
        case MC_24_BOOKS:
        case MC_54_RECONSTRUCTED_NOTE:
            Tooltip_AddLine_3(L("Right click to read"));
            break;
        case MC_22_RELIC_NEED_TARGET:
            Tooltip_AddLine_3(L("Right-click to read, then"));
            Tooltip_AddLine_3(L("left-click to target"));
            break;
        case MC_42_MAP_OF_THE_STARS:
            Tooltip_AddLine_3(L("Right click to view")); // (th4) localizable
            break;
        case MC_43_EAR_HEART:
            sprintf(InfoPanelBuffer, L("Level : %i"), item.quality); // (th4) localizable
            Tooltip_AddLine_3(InfoPanelBuffer);
            break;
        case MC_53_AURIC_AMULET:
            Tooltip_AddLine_3("Doubles gold capacity");
            break;
        }

        if (item.baseItemIndex == BI_33_STAFF_OF_LAZARUS && HasTrait(CurrentPlayerIndex, TraitId::Leper)) {
            Tooltip_AddLine_3(L("Right click to use"));
        }
        else if (isTownPortalForbidden()) {
            switch (item.baseItemIndex) {
            case BI_19_FUNGAL_TOME:		 Tooltip_AddLine_3(L("Right click to transmute into spectral elixir")); break;
            case BI_2512_FULL_CONTAINERS:Tooltip_AddLine_3(L("Right click to transmute into random elixir")); break;
            case BI_33_STAFF_OF_LAZARUS: Tooltip_AddLine_3(L("Right click to use")); break;
            case BI_16_ANVIL_OF_FURY:
            case BI_9_MAGIC_ROCK:
            case BI_327_TAVERN_SIGN:
            case BI_2457_IRON_FIGURINE:
            case BI_2459_REAGENTS:
            case BI_2460_THE_RIGHT_GOODS:
            case BI_2510_DUSTY_CRATE:
            case BI_2461_MUNDANE_INVENTORY:
            case BI_334_THEODORE:		 Tooltip_AddLine_3("Right click to transmute into quest reward"); break;
            }
        }
        if (isTownPortalForbidden()) {
            if (item.baseItemIndex >= BI_296_QUARTZ && item.baseItemIndex <= BI_301_AQUAMARINE) {
                Tooltip_AddLine_3("Right click to transmute into gold");
            }
        }
        if (item.baseItemIndex == BI_2472_THE_LOCKED_CHEST) { 
            Tooltip_AddLine_3("Right click to open (requires key)");
        }
        if (item.baseItemIndex == BI_2476_THE_STRANGE_BOOK) {
            Tooltip_AddLine_3(L("Right click to read (requires crystal eyepiece)"));
        }

        if (IsItemAllowedForBelt(item)) {
            if (slot >= IS_50_47_Belt_Start && slot < IS_50_47_Belt_End) {
                Tooltip_AddLine_3(L("Shift Click to move left"));
            }else if( ! IsTransmutePanelVisible ){
                Tooltip_AddLine_3(L("Shift Click to move to belt"));
            }
        }
    }

    // Requirements
    if (item.MagicCode == MC_24_BOOKS && item.RequiredLevel == 255u) {
        Tooltip_AddLine_3("Spell is maxed", C_3_Gold);
    }
    else if (item.RequiredMagic || item.RequiredStrength || item.RequiredDexterity || item.RequiredVitality || item.RequiredLevel) {
        strcpy(InfoPanelBuffer, L("required:"));

        auto appendReqInfo = [&player](const char* name, int req, int current, int magicCode, bool isLevel = false)
        {
            if (req) {
                uchar color;
                if (current >= req) {
                    color = C_0_White + 128;
                }
                else if (!isLevel && req <= current + (is(magicCode,
                    MC_24_BOOKS, MC_21_RELIC_NEED_NO_TARGET, MC_22_RELIC_NEED_TARGET,
                    MC_47_RUNE_OF_FIRE, MC_48_RUNE_OF_LIGHTNING, MC_49_RUNE_OF_SHOCK,
                    MC_50_RUNE_OF_BLAZE, MC_51_RUNE_OF_STONE) ? 0 : player.AdvancedUser)) {
                    color = C_1_Blue + 128;
                }
                else {
                    color = C_2_Red + 128;
                }
                sprintf(InfoPanelBuffer, "%s %c%i %s%c", InfoPanelBuffer, color, req, name, color);
            }
        };

        appendReqInfo(L("lvl"), item.RequiredLevel, player.CharLevel, item.MagicCode, true); // (th4) isLevel: level is never lowered by Advanced User, so it goes straight from white to red. This used to be `name != "lvl"`, a POINTER compare: name is L("lvl"), a different pointer in every translated language, so a missed level requirement showed blue there. Same fix in all three tooltip copies.
        appendReqInfo(L("str"), item.RequiredStrength, player.CurStrength, item.MagicCode);
        appendReqInfo(L("mag"), item.RequiredMagic, player.CurMagic, item.MagicCode);
        appendReqInfo(L("dex"), item.RequiredDexterity, player.CurDexterity, item.MagicCode);
        appendReqInfo(L("vit"), item.RequiredVitality, player.CurVitality, item.MagicCode);
        Tooltip_AddLine_3(InfoPanelBuffer);
    }

    uint playerFullClassMask = (1u << static_cast<uint>(player.fullClassId));
    uint requiredClassMask = GetRequiredClassMask(&item);
    bool sort_out = false;
    if (requiredClassMask == IPCM_ALL_CLASSES) {
        sort_out = true;
    }
    else if (player.gameChanger & BIT(GC_27_TERMINATOR)
        || player.gameChanger & BIT(GC_28_SUPERMUTANT)
        || player.gameChanger & BIT(GC_29_ENCLAVE_SOLDIER)) {
        sort_out = true;
    }
    if (!sort_out) {
        printItemClassReqInfoInBuffer(&item);
        Tooltip_AddLine_3(InfoPanelBuffer, (playerFullClassMask & requiredClassMask) ? C_0_White : C_2_Red);
    }

    if ((playerFullClassMask & IPCM_ANY_EXILE) != 0 && baseItem.ForbiddenForExiles) {
        Tooltip_AddLine_3("[Can't wear item]", C_2_Red);
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::OldFashioned)) {
        if (is(item.ItemCode, IC_6_LIGHT_ARMOR, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR)) {
            Tooltip_AddLine_3("[Can't wear body armor]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::Fechtmeister, TraitId::Ranger, TraitId::Mamluk)) {
        if (is(item.ItemCode, IC_5_SHIELD)) {
            Tooltip_AddLine_3("[Can't use shields]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::Black_Witchery)) {
        if (is(item.ItemCode, IC_21_KNIFE)) {
            Tooltip_AddLine_3("[Can't use throwing knives]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::Paladin) || player.fullClassId == PFC_DRUID) {
        if (is(item.ItemCode, IC_3_BOW)) {
            Tooltip_AddLine_3("[Can't use crossbows]", C_2_Red);
        }
        else if (is(item.ItemCode, IC_22_MALLET)) {
            Tooltip_AddLine_3("[Can't use throwing mallets]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::TwoTowers)) {
        if (is(item.ItemCode, IC_1_SWORD, IC_4_MACE)) {
            Tooltip_AddLine_3("[Can't use melee weapons]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::Axepertise)) {
        if (is(item.ItemCode, IC_1_SWORD, IC_4_MACE)) {
            Tooltip_AddLine_3("[Can't use item]", C_2_Red);
        }
    }
    if (HasTrait(CurrentPlayerIndex, TraitId::WildSorcery) || HasTrait(CurrentPlayerIndex, TraitId::Rabid)) {
        if (is(item.MagicCode,
            MC_3_POTION_OF_HEALING,
            MC_6_POTION_OF_MANA,
            MC_2_POTION_OF_FULL_HEALING,
            MC_7_POTION_OF_FULL_MANA,
            MC_18_POTION_OF_REJUVENATION,
            MC_19_POTION_OF_FULL_REJUVENATION)) {
            Tooltip_AddLine_3("[Can't use potions]", C_2_Red);
        }
    }
    //----------------------
    if (IsVisualTradePanelVisible) {

        if (isVisualTradeSlot) {
            int price = VisualTrade_GetBuyPrice(&item);
            sprintf(InfoPanelBuffer, L("Buy price: %i"), price);
            Tooltip_AddLine_3(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
            Tooltip_AddLine_3(L("Shift click to buy fast"));
        }
        else {
            if (Cur.GraphicsID == CM_1_NORMAL_HAND) {
                if (CurrentVisualTradeMode != VTM_Elder) {
                    int price = VisualTrade_GetSellPrice(&item);
                    if (price) {
                        sprintf(InfoPanelBuffer, L("Sell price: %i"), price);
                        Tooltip_AddLine_3(InfoPanelBuffer);
                        Tooltip_AddLine_3(L("Shift click to sell fast"));
                    }
                    else {
                        Tooltip_AddLine_3(L("Item cannot be sold"), C_2_Red);
                    }
                }
            }
            else if (Cur.IsTradeService) {
                if (Cur.GraphicsID == CM_2_IDENTIFY) {
                    int price = VisualTrade_GetIdentificationPrice(&item);
                    if (price) {
                        sprintf(InfoPanelBuffer, "Identification price: %i", price);
                        Tooltip_AddLine_3(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }
                else if (Cur.GraphicsID == CM_3_REPAIR) {
                    int price = VisualTrade_GetRepairPrice(&item);
                    if (price) {
                        sprintf(InfoPanelBuffer, "Repair price: %i", price);
                        Tooltip_AddLine_3(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }
                else if (Cur.GraphicsID == CM_4_STAFF_RECHARGE) {
                    int price = VisualTrade_GetRechargePrice(&item);
                    if (price) {
                        sprintf(InfoPanelBuffer, "Recharge price: %i", price);
                        Tooltip_AddLine_3(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }
            }
        }
    }

    if (IsStashPanelVisible) {
        if (slot >= IS_Inventory) {
            if (!IsItemAllowedForStash(&item)) {
                Tooltip_AddLine_3(L("Cannot be stashed"), C_2_Red);
            }
            else {
                if (isVisualStashSlot) {
                    Tooltip_AddLine_3(L("Shift Click to move to inventory"));
                }
                else {
                    Tooltip_AddLine_3(L("Shift Click to move to stash"));
                }
            }
        }
    }
	if( IsTransmutePanelVisible && Transmute_Allowed(item) ){
		if( slot >= Stash_SlotIDStart ){
			Tooltip_AddLine_3(L("Shift Click to move to inventory"), C_7_Grey);
		}else if (slot >= IS_Inventory) {
			Tooltip_AddLine_3(L("Shift Click to transmute panel"), C_7_Grey);
		}
	}

    if (identified && item.MagicLevel == ML_3_SET) {
        UniqueItem& uniq = UniqueItems[item.UniqueItemIndex];
        UniqueSet& set = UniqueSets[uniq.uniqueSetIndex];

        Tooltip_AddLine_3(""); // blank line
        bool hasActiveEffects = false;
        int countOfEquippedItemsFromSet = SetItem_GetCountOfEquippedItemsFromSet(CurrentPlayerIndex, uniq.uniqueSetIndex);
        for (int i = 0, ie = set.EffectCount; i != ie; ++i) {
            auto& e = set.Effects[i];
            if (countOfEquippedItemsFromSet >= e.Threshold) {
                if( PrintItemEffectToStringBuffer(e.Effect.id, const_cast<Item*>(&set.FakeItemToShowEffect), i) ){
					Tooltip_AddLine_3(InfoPanelBuffer, C_4_Orange);
				}
                hasActiveEffects = true;
            }
        }
        if (hasActiveEffects) Tooltip_AddLine_3(""); // blank line
        Tooltip_AddLine_3(LocalizedUniqueItemName(set.Name), C_3_Gold);
        for (int i = 0; i < set.UniquesCount; ++i)
        {
            int uniqueItemIndex = set.UniquesList[i];
            UniqueItem& uniq2 = UniqueItems[uniqueItemIndex];
            Tooltip_AddLine_3(LocalizedUniqueItemName(uniq2.Name), SetItem_IsItemFromSetEquipped(CurrentPlayerIndex, uniqueItemIndex) ? C_1_Blue : C_2_Red);
        }
    }
}

bool ProhibitAltHighlight = false;

// ---- th2 -------------------------------------------------------------------------------
void __fastcall ShowSecondaryTooltip(Item& item)
{
    // Sigils are ring-type items but occupy their own inventory; never compare them to equipped rings
    if (item.baseItemIndex >= BI_2504_SIGIL_FIRST && item.baseItemIndex <= BI_2508_SIGIL_LAST) return;

    if( ! SecondaryTooltip || ! IsAltPressed ){
        if (IsController()) {
            if (curXboxController.GetState().Gamepad.bLeftTrigger < 100) {
                return;
            }
        }
        else {
            return;
        }
    }
    Player& player = Players[CurrentPlayerIndex];

    switch (item.ItemCode) {
    case IC_1_SWORD:
    case IC_2_AXE:
    case IC_4_MACE:
    case IC_18_FLASK:
    case IC_20_CLAW:  
    case IC_10_STAFF:
    case IC_19_TRAP:
        if (is(player.OnBodySlots[IS_LeftHand].ItemCode, IC_1_SWORD, IC_2_AXE, IC_4_MACE, IC_18_FLASK, IC_20_CLAW, IC_10_STAFF, IC_19_TRAP) && is(player.OnBodySlots[IS_RightHand].ItemCode, IC_1_SWORD, IC_2_AXE, IC_4_MACE, IC_18_FLASK, IC_20_CLAW, IC_10_STAFF, IC_19_TRAP)) {
            Tooltip_ShowItemInfo_2(IS_LeftHand);
            Tooltip_ShowItemInfo_3(IS_RightHand);
        } 
        else if (is(player.OnBodySlots[IS_LeftHand].ItemCode, IC_1_SWORD, IC_2_AXE, IC_4_MACE, IC_18_FLASK, IC_20_CLAW, IC_10_STAFF, IC_19_TRAP)) {
            Tooltip_ShowItemInfo_2(IS_LeftHand);
        }
        else if (is(player.OnBodySlots[IS_RightHand].ItemCode, IC_1_SWORD, IC_2_AXE, IC_4_MACE, IC_18_FLASK, IC_20_CLAW, IC_10_STAFF, IC_19_TRAP)) {
            Tooltip_ShowItemInfo_2(IS_RightHand);
        }
        break;
    case IC_3_BOW: 
    case IC_21_KNIFE:
    case IC_22_MALLET:
    case IC_23_PISTOL:
        if (is(player.OnBodySlots[IS_LeftHand].ItemCode, item.ItemCode)) {
            Tooltip_ShowItemInfo_2(IS_LeftHand);
        }
        else if (is(player.OnBodySlots[IS_RightHand].ItemCode, item.ItemCode)) {
            Tooltip_ShowItemInfo_2(IS_RightHand);
        }
        break;
    case IC_5_SHIELD: 
        if (is(player.OnBodySlots[IS_LeftHand].ItemCode, item.ItemCode) && is(player.OnBodySlots[IS_RightHand].ItemCode, item.ItemCode)) {
            Tooltip_ShowItemInfo_2(IS_LeftHand);
            Tooltip_ShowItemInfo_3(IS_RightHand);
        }
        else if (is(player.OnBodySlots[IS_LeftHand].ItemCode, item.ItemCode)) {
            Tooltip_ShowItemInfo_2(IS_LeftHand);
        }
        else if (is(player.OnBodySlots[IS_RightHand].ItemCode, item.ItemCode)) {
            Tooltip_ShowItemInfo_2(IS_RightHand);
        }
        break;
    case IC_6_LIGHT_ARMOR:
    case IC_8_MID_ARMOR:
    case IC_9_HEAVY_ARMOR: if (!is(player.OnBodySlots[IS_Torso].ItemCode, IC_M1_NONE)) Tooltip_ShowItemInfo_2(IS_Torso); break;
    case IC_7_HELM: if (!is(player.OnBodySlots[IS_Head].ItemCode, IC_M1_NONE)) Tooltip_ShowItemInfo_2(IS_Head); break;
    case IC_12_RING: 
        if (!is(player.OnBodySlots[IS_LeftRing].ItemCode, IC_M1_NONE) && !is(player.OnBodySlots[IS_RightRing].ItemCode, IC_M1_NONE)) {
            Tooltip_ShowItemInfo_2(IS_LeftRing);
            Tooltip_ShowItemInfo_3(IS_RightRing);
        }
        else if (!is(player.OnBodySlots[IS_LeftRing].ItemCode, IC_M1_NONE)) {
            Tooltip_ShowItemInfo_2(IS_LeftRing);
        }
        else if (!is(player.OnBodySlots[IS_RightRing].ItemCode, IC_M1_NONE)) {
            Tooltip_ShowItemInfo_2(IS_RightRing);
        }
        break;
    case IC_13_AMULET: if (!is(player.OnBodySlots[IS_Neck].ItemCode, IC_M1_NONE)) Tooltip_ShowItemInfo_2(IS_Neck); break;
    case IC_15_GLOVE: if (!is(player.OnBodySlots[IS_Hands].ItemCode, IC_M1_NONE)) Tooltip_ShowItemInfo_2(IS_Hands); break;
    case IC_16_BOOTS: if (!is(player.OnBodySlots[IS_Feet].ItemCode, IC_M1_NONE)) Tooltip_ShowItemInfo_2(IS_Feet); break;
    case IC_17_BELT: if (!is(player.OnBodySlots[IS_Waist].ItemCode, IC_M1_NONE)) Tooltip_ShowItemInfo_2(IS_Waist); break;
    default: return;
    }
}

// ---- th2 -------------------------------------------------------------------------------
void __fastcall Tooltip_ShowItemInfo( int slot )
{
    if( slot == -1 ){
        return;
    }
    XinputAutoTargettingOffTimer = 0;
    Tooltip_Clear();
 
    char buffer[256];
    bool isVisualTradeSlot = false;
    bool isVisualStashSlot = false;
    bool isCraftSlot = false;
    Player& player = Players[CurrentPlayerIndex];
    Item* itemPtr = nullptr;

  

	if( slot < IS_50_47_Belt_Start ){ // TODO: change to unified func
		if( slot >= IS_Inventory ){
			// Inventory item: slot encodes as IS_Inventory + itemIndex.
			// OnBodySlots[IS_Inventory + n] aliases InventorySlots[n] or TalismanSlots[n] depending on which inv is shown.
			int itemIndex = slot - IS_Inventory;
			extern bool IsSigilInventoryOpen;
			itemPtr = IsSigilInventoryOpen ? &player.TalismanSlots[itemIndex] : &player.InventorySlots[itemIndex];
		} else {
			itemPtr = &player.OnBodySlots[ slot ];
		}
	    Tooltip_SetOffsetLeft();
        if (slot > IS_Feet) {
            ShowSecondaryTooltip(*itemPtr);      
        }

    }else if( slot < IS_50_47_Belt_End ){
        itemPtr = &player.BeltInventory[ slot - IS_50_47_Belt_Start ];
        Tooltip_SetOffsetTop();
    }else if( StashPanel_IsStashSlot( slot ) ){
        itemPtr = StashPanel_GetItemBySlotId( slot );
        Tooltip_SetOffsetRight();
        ShowSecondaryTooltip(*itemPtr);
        isVisualStashSlot = true;
    }else if( VisualTrade_IsVisualTradeSlot( slot ) ){
        itemPtr = VisualTrade_GetItemBySlotId( slot );
        Tooltip_SetOffsetRight();
        ShowSecondaryTooltip(*itemPtr);
        isVisualTradeSlot = true;
    }else if( Craft_IsCraftSlot( slot ) ){
        itemPtr = Craft_GetItemBySlotId( slot );
        Tooltip_SetOffsetBottom();
        ShowSecondaryTooltip(*itemPtr);
        isCraftSlot = true;
    }else if (IsEnchantPanelVisible && slot == 101) {
        itemPtr = &EnchantItem;
        Tooltip_SetOffsetBottom();
        ShowSecondaryTooltip(*itemPtr);
	}else if (IsQuenchPanelVisible && slot == 102) {
		itemPtr = &QuenchItem;
		Tooltip_SetOffsetBottom();
		ShowSecondaryTooltip(*itemPtr);
	}else if (IsTransmutePanelVisible) {
		itemPtr = TransmuteSlots_GetItemBySlotId( slot );
		Tooltip_SetOffsetBottom();
		ShowSecondaryTooltip(*itemPtr);
	}else{
        return;
    }
    
    // sigil boost for Raw Power trait -------------------
    struct SigilBooster {
        Item* ptr;
        bool modified;
        SigilBooster(Item* p) : ptr(p), modified(false) {
            if (p->baseItemIndex >= BI_2504_SIGIL_FIRST && p->baseItemIndex <= BI_2508_SIGIL_LAST
                && HasTrait(CurrentPlayerIndex, TraitId::RawPower)) {
                for (Effect& e : p->effect) {
                    e.minVal *= 10;
                    e.maxVal *= 10;
                    e.chance *= 10;
                }
                modified = true;
            }
        }
        ~SigilBooster() {
            if (modified) {
                for (Effect& e : ptr->effect) {
                    e.minVal /= 10;
                    e.maxVal /= 10;
                    e.chance /= 10;
                }
            }
        }
    };
    SigilBooster booster(itemPtr);
    //------------------------------------------

    Item& item = *itemPtr;
    BaseItem& baseItem = BaseItems[item.baseItemIndex];
    
	int nameColor = ItemColor(item);
    bool identified = item.Identified || item.MagicLevel == ML_0_USUAL;
    bool rare = item.dropType & D_RARE;

    const char* namePtr = nullptr;
    if( identified ){
        namePtr = item.FullMagicalItemName;
    }else{
        namePtr = item.Name;
    }
    
    if( item.ItemCode == IC_11_GOLD ){
		sprintf(InfoPanelBuffer, L("%i gold"), item.amount);
		namePtr = InfoPanelBuffer;
	}
    
    // Skip * in set's name
    if( *namePtr == '*' ){
        ++namePtr;
    }

    Tooltip_AddLine(namePtr, nameColor);
    
    if( rare ){
        Tooltip_AddLine("Rare Item", nameColor);
    }else if( item.MagicLevel == ML_2_UNIQUE ){
        Tooltip_AddLine(L("Unique Item"), nameColor);
    }else if( item.MagicLevel == ML_3_SET ){
        Tooltip_AddLine(L("Set Item"), nameColor);
    }else if( IsQuestItem(item.baseItemIndex) ){
        Tooltip_AddLine(L("Quest item"), nameColor);
    }

    if( item.TypeID == ITEM_1_WEAPON || item.TypeID == ITEM_2_ARMOR ){
        if( item.BaseDurability ){
            if( item.BaseDurability == ITEM_DUR_INDESTRUBLE ){
                Tooltip_AddLine("indestructible", C_3_Gold);
            }else{
                char curDurColor = C_0_White;
                char baseDurColor = C_0_White;
                
                if( item.CurDurability <= 1 ){
                    curDurColor = C_2_Red;
                }else if( item.CurDurability >= 2 && item.CurDurability <= 5 ){
                    curDurColor = C_5_Yellow;
                }
                
                if( item.BaseDurability <= 1 ){
                    baseDurColor = C_2_Red;
                }else if( item.CurDurability <= 1 ){
                    baseDurColor = C_5_Yellow;
                }

                curDurColor += 128;
                baseDurColor += 128;
                
                sprintf(InfoPanelBuffer, L("durability: %c%i%c of %c%i%c"), curDurColor, item.CurDurability, curDurColor, baseDurColor, item.BaseDurability, baseDurColor);
                Tooltip_AddLine(InfoPanelBuffer);
            }
        }
    }
    
    if( item.BaseCharges ){
        char color = ((item.CurCharges > 0) ? C_0_White : C_2_Red) + 128;
        sprintf(InfoPanelBuffer, "%s charges: %c%i%c of %i", getSpellName( item.SpellIndex ), color, item.CurCharges, color, item.BaseCharges);
        Tooltip_AddLine(InfoPanelBuffer);
    }
    
    if (item.TypeID == ITEM_1_WEAPON) {
        if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk) || GameMode == GM_CLASSIC) {
            goto MUM_3;
        }
        else if (!is(player.fullClassId, PFC_MAGE, PFC_ELEMENTALIST, PFC_WARLOCK, PFC_NECROMANCER, PFC_DEMONOLOGIST)) {
        MUM_3:
            const char* damageName = L("damage");
            if (HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
                goto MUMZ_6;
            }
            if (BaseItems[item.baseItemIndex].AnimationDelay) {
                damageName = L("slow damage");
            }
            else if (item.ItemCode == IC_1_SWORD && player.fullClassId != PFC_INQUISITOR && player.fullClassId != PFC_DRUID) {
                damageName = L("slashing damage");
            }
            else if (item.ItemCode == IC_4_MACE && player.fullClassId != PFC_INQUISITOR && player.fullClassId != PFC_DRUID) {
                damageName = L("crushing damage");
            }
            MUMZ_6:
            sprintf(InfoPanelBuffer, L("%s: "), damageName);
            if (item.overTime) {
                if (item.MinDamage == item.MaxDamage) {
                    sprintf(InfoPanelBuffer, "%s%i over %i sec", InfoPanelBuffer, item.MinDamage, item.overTime);
                }
                else {
                    sprintf(InfoPanelBuffer, "%s%i-%i over %i sec", InfoPanelBuffer, item.MinDamage, item.MaxDamage, item.overTime);
                }
            }
            else {
                if (item.MinDamage == item.MaxDamage) {
                    sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.MinDamage);
                }
                else {
                    sprintf(InfoPanelBuffer, "%s%i-%i", InfoPanelBuffer, item.MinDamage, item.MaxDamage);
                }
            }
            Tooltip_AddLine(InfoPanelBuffer);

            if (!(player.gameChanger & BIT(GC_10_HACKNSLASH)) && GameMode != GM_COLISEUM) {
                if (item.ItemCode == IC_1_SWORD) {
                    if (player.fullClassId == PFC_THRAEX) {
                        Tooltip_AddLine(L("(+50% to beasts)"));
                    }
                    else if (player.fullClassId == PFC_INQUISITOR || player.fullClassId == PFC_DRUID || HasTrait(CurrentPlayerIndex, TraitId::Mamluk)) {
                        //Tooltip_AddLine("(+50% to beasts, -50% to undead)");
                    }
                    else if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS) {
                        Tooltip_AddLine(L("(+15% to beasts, -50% to undead)"));
                    }
                    else if (player.fullClassId == PFC_SECUTOR && HasTrait(CurrentPlayerIndex, TraitId::Bestiarius)) {
                        Tooltip_AddLine(L("(+80% to beasts, -50% to undead)"));
                    }
                    else if (player.fullClassId == PFC_KENSEI) {
                        Tooltip_AddLine(L("(+30% to beasts, -50% to undead)"));
                    }
                    else if (player.fullClassId == PFC_GUARDIAN) {
                        Tooltip_AddLine(L("(+40% to beasts, -50% to undead)"));
                    }
                    else {
                        Tooltip_AddLine(L("(+50% to beasts, -50% to undead)"));
                    }
                }
                else if (item.ItemCode == IC_4_MACE) {
                    if (player.fullClassId == PFC_TEMPLAR) {
                        Tooltip_AddLine(L("(+70% to undead, -50% to beasts)"));
                    }
                    else if (player.fullClassId == PFC_INQUISITOR || player.fullClassId == PFC_DRUID) {
                        //Tooltip_AddLine("(+70% to undead, -50% to beasts)");
                    }
                    else if (player.fullClassId == PFC_ROGUE || player.fullClassId == PFC_DIMACHAERUS) {
                        Tooltip_AddLine(L("(+15% to undead, -50% to beasts)"));
                    }
                    else if (player.fullClassId == PFC_SECUTOR && HasTrait(CurrentPlayerIndex, TraitId::Bestiarius)) {
                        Tooltip_AddLine(L("(+25% to undead, -50% to beasts)"));
                    }
                    else if (player.fullClassId == PFC_KENSEI) {
                        Tooltip_AddLine(L("(+30% to undead, -50% to beasts)"));
                    }
                    else if (player.fullClassId == PFC_GUARDIAN) {
                        Tooltip_AddLine(L("(+40% to undead, -50% to beasts)"));
                    }
                    else {
                        Tooltip_AddLine(L("(+50% to undead, -50% to beasts)"));
                    }
                }
                else if (item.ItemCode == IC_10_STAFF && player.fullClassId == PFC_MONK) {
                    Tooltip_AddLine(L("(+20% to demons, -20% to beasts)"));
                }
                else if (item.ItemCode == IC_20_CLAW && player.fullClassId == PFC_ASSASSIN) {//as if smb else wields claws...
                    Tooltip_AddLine(L("(+20% to beasts, -20% to undead)"));
                }//else if (item.ItemCode == IC_21_KNIFE && player.fullClassId == PFC_ASSASSIN) {//as if smb else wields knifes...
                 //   Tooltip_AddLine("(+PLACEHOLDER% to PLACEHOLDER, -PLACEHOLDER% to PLACEHOLDER)");
                //}
            }
            if (item.ItemCode == IC_19_TRAP) {
                const BaseItem& baseItem = BaseItems[item.baseItemIndex];
                const TrapInfo& trapInfo = TrapsInfo[baseItem.FlaskOrTrapCode];
                sprintf(InfoPanelBuffer, "%s: %.2f", "Delay time", trapInfo.ShootDelay / 20.00);
                Tooltip_AddLine(InfoPanelBuffer);
            }

            if (item.ArmorClass) {
                int ac = item.ArmorClass;
                const char* modificationSrt;
                if (ac > 0) {
                    modificationSrt = L("increased");
                }
                else {
                    ac = -ac;
                    modificationSrt = L("reduced");
                }
                sprintf(InfoPanelBuffer, L("armor class %s by: %i"), modificationSrt, ac);
                Tooltip_AddLine(InfoPanelBuffer);
            }
        }
    }
    
    if( item.TypeID == ITEM_2_ARMOR ){
        if( item.damageFromEnemies ){
            sprintf(InfoPanelBuffer, L("%+i damage from enemies"), item.damageFromEnemies);
            Tooltip_AddLine(InfoPanelBuffer);
        }
        if( item.ArmorClass ){
            sprintf(InfoPanelBuffer, L("armor class: %i"), item.ArmorClass);
            Tooltip_AddLine(InfoPanelBuffer);
        }
        if( item.minMeleeAbsorbPercent || item.maxMeleeAbsorbPercent ){
            sprintf(InfoPanelBuffer, L("resist melee damage: "));
            if( item.minMeleeAbsorbPercent == item.maxMeleeAbsorbPercent ){
                sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.maxMeleeAbsorbPercent);
            }else{
                sprintf(InfoPanelBuffer, L("%s%i to %i"), InfoPanelBuffer, item.minMeleeAbsorbPercent, item.maxMeleeAbsorbPercent);
            }
            Tooltip_AddLine(InfoPanelBuffer);
        }
        if( item.minArrowAbsorbPercent || item.maxArrowAbsorbPercent ){
            sprintf(InfoPanelBuffer, L("resist arrow damage: "));
            if( item.minArrowAbsorbPercent == item.maxArrowAbsorbPercent ){
                sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.maxArrowAbsorbPercent);
            }else{
                sprintf(InfoPanelBuffer, L("%s%i to %i"), InfoPanelBuffer, item.minArrowAbsorbPercent, item.maxArrowAbsorbPercent);
            }
            Tooltip_AddLine(InfoPanelBuffer);
        }
        if( item.minThorns || item.maxThorns ){
            sprintf(InfoPanelBuffer, "thorns damage: ");
            if( item.minThorns == item.maxThorns ){
                sprintf(InfoPanelBuffer, "%s%i", InfoPanelBuffer, item.minThorns);
            }else{
                sprintf(InfoPanelBuffer, "%s%i-%i", InfoPanelBuffer, item.minThorns, item.maxThorns);
            }
            Tooltip_AddLine(InfoPanelBuffer);
        }
        if( item.MinDamage || item.MaxDamage ){
            int minDmg = item.MinDamage;
            int maxDmg = item.MaxDamage;
            const char* modificationSrt;
            if( minDmg > 0 ){
                modificationSrt = L("increased");
            }else{
                minDmg = -minDmg;
                maxDmg = -maxDmg;
                modificationSrt = L("reduced");
            }
			if( item.MinDamage == item.MaxDamage ){
				sprintf(InfoPanelBuffer, L("damage %s by: %i"), modificationSrt, minDmg);
			}else{
				sprintf(InfoPanelBuffer, L("damage %s by: %i-%i"), modificationSrt, minDmg, maxDmg);
			}
            Tooltip_AddLine(InfoPanelBuffer);
        }
    }
 
	if( item.dropType & D_QUENCHED && (identified || item.MagicLevel == ML_0_USUAL) ){
		for( int i = 0; i < countof(item.effect); ++i ){
			Effect& e = item.effect[i];
			if( e.id != AE_NO && e.origin == AO_QUENCH ){
				if( PrintItemEffectToStringBuffer(e.id, &item, i) ){
					char temp[256]; sprintf(temp, L("Quenched: %s"), InfoPanelBuffer);
					Tooltip_AddLine(temp, C_1_Blue);
				}
			}
		}
	}

    for( int i = 0; i < baseItem.EffectCount; ++i ){
        const int effectIndex = 20 + i;
		if( PrintItemEffectToStringBuffer(baseItem.Effects[i].id, const_cast<Item*>(&item), effectIndex) ){
			Tooltip_AddLine(InfoPanelBuffer);
		}
	}
    
    // Effects
	if( identified ){
		for( int i = 0; i < countof(item.effect); ++i ){
			Effect& e = item.effect[i];
			if( e.id != AE_NO && e.origin < AO_BASE && PrintItemEffectToStringBuffer(e.id, &item, i) ){
				Tooltip_AddLine(InfoPanelBuffer, C_1_Blue);
			}
		}
	}else{
		if( item.ItemCode == IC_12_RING || item.ItemCode == IC_13_AMULET || item.TypeID == ITEM_1_WEAPON || item.TypeID == ITEM_2_ARMOR ){
            if( item.MagicLevel ){
                Tooltip_AddLine(L("Not Identified"), C_2_Red);
            }
        }
	}
 
    // Usage effects
    {
        const MAGIC_CODE magicCode = item.MagicCode;
        int restoreDurMin = 0;
        int restoreDurMax = 0;
        int addDurMin = 0;
        int addDurMax = 0;
        int addDurLimit = 0;
        
        switch( magicCode ){
        case MC_31_OIL_OF_REPAIR_I:
            restoreDurMin = 1;
            restoreDurMax = 10;
            break;
        case MC_32_OIL_OF_REPAIR_II:
            restoreDurMin = 6;
            restoreDurMax = 15;
            break;
        case MC_33_OIL_OF_REPAIR_III:
            restoreDurMin = 11;
            restoreDurMax = 20;
            break;
        case MC_34_OIL_OF_REPAIR_IV:
            restoreDurMin = 16;
            restoreDurMax = 25;
            break;
        case MC_35_OIL_OF_REPAIR_V:
            restoreDurMin = 21;
            restoreDurMax = 30;
            break;
        case MC_36_BLACKSMITH_OIL:
            addDurMin = 1;
            addDurMax = 3;
            addDurLimit = 100;
            break;
        case MC_37_OIL_OF_FORTITUDE:
            addDurMin = addDurMax = 1;
            addDurLimit = 300;
            break;
        case MC_38_OIL_OF_REPAIR_VI:
            restoreDurMin = 26;
            restoreDurMax = 35;
            break;
        case MC_39_OIL_OF_REPAIR_VII:
            restoreDurMin = 31;
            restoreDurMax = 45;
            break;
        case MC_40_OIL_OF_HARDENING:
            addDurMin = 1;
            addDurMax = 2;
            addDurLimit = 150;
            break;
        case MC_2_POTION_OF_FULL_HEALING:
        {
            if (HasTrait(CurrentPlayerIndex, TraitId::Giant)) {
                Tooltip_AddLine(L("recover 50% of life"));
            }
            else {
                Tooltip_AddLine(L("fully recover life"));
            }
        }
            break;
        case MC_3_POTION_OF_HEALING:
        {
            if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_27_TERMINATOR)) {
                Tooltip_AddLine("unable to consume", C_2_Red);
            }
            else { 
                Tooltip_AddLine(L("recover partial life")); 
            }
        }
            break;
        case MC_6_POTION_OF_MANA:
        {
            if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_27_TERMINATOR)) {
                Tooltip_AddLine("unable to consume", C_2_Red);
            }
            else { 
                Tooltip_AddLine(L("recover partial mana")); 
            }
        }
            break;
        case MC_7_POTION_OF_FULL_MANA:
        {
            if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_27_TERMINATOR)) {
                Tooltip_AddLine("unable to consume", C_2_Red);
            }
            else if (HasTrait(CurrentPlayerIndex, TraitId::Giant)) {
                Tooltip_AddLine(L("recover 50% of mana"));
            }
            else {
                Tooltip_AddLine(L("fully recover mana"));
            }
        }
            break;
        case MC_10_ELIXIR_OF_STRENGTH:
            Tooltip_AddLine( L("increases strength"));
            break;
        case MC_11_ELIXIR_OF_MAGIC:
            Tooltip_AddLine( L("increases magic"));
            break;
        case MC_12_ELIXIR_OF_DEXTERITY:
            Tooltip_AddLine( L("increases dexterity"));
            break;
        case MC_13_ELIXIR_OF_VITALITY:
            Tooltip_AddLine( L("increases vitality"));
            break;
        case MC_44_SPECTRAL_ELIXIR:
            Tooltip_AddLine( L("increases all stats"));
            break;
        case MC_18_POTION_OF_REJUVENATION:
        {
            if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_27_TERMINATOR)) {
                Tooltip_AddLine("unable to consume", C_2_Red);
            }
            else {
                Tooltip_AddLine(L("recover life and mana"));
            }
        }
            break;
        case MC_19_POTION_OF_FULL_REJUVENATION:
        {
            if (Players[CurrentPlayerIndex].gameChanger & BIT(GC_27_TERMINATOR)) {
                Tooltip_AddLine("unable to consume", C_2_Red);
            }
            else if (HasTrait(CurrentPlayerIndex, TraitId::Giant)) {
                Tooltip_AddLine(L("recover 50% of life and mana"));
            }
            else {
                Tooltip_AddLine(L("fully recover life and mana"));
            }
        }
            break;
        case MC_47_RUNE_OF_FIRE:
        case MC_50_RUNE_OF_BLAZE:
            Tooltip_AddLine( "Explodes with Fire Nova");
            break;
        case MC_48_RUNE_OF_LIGHTNING:
        case MC_49_RUNE_OF_SHOCK:
            Tooltip_AddLine( "Explodes with Lightning Nova");
            break;
        case MC_51_RUNE_OF_STONE:
            Tooltip_AddLine( "sets petrification trap");
            break;
        case MC_GEM:
        {
            optional<Gem> gemInfoOpt = FindGemInfoByBaseItemIndex( item.baseItemIndex );
            if( gemInfoOpt ){
                bool isSocketingPanel = IsCraftPanelVisible && CurrentCraftPage == CraftPage::CP_Socketing;
                
                bool isTwoHanded = [&](){
                        if( isSocketingPanel ){
                            Item& socketedItem = Craft_GetSocketedItem();
                            if( socketedItem.ItemCode != IC_M1_NONE ){
                                return socketedItem.EquippedLocation == EL_2_TWO_HANDED;
                            }
                        }
                        return false;
                    }();
                
                if( isSocketingPanel && isCraftSlot ){
                    Item& socketedItem = Craft_GetSocketedItem();
                    if( socketedItem.ItemCode != IC_M1_NONE ){
                        BaseItem& socketedBaseItem = BaseItems[socketedItem.baseItemIndex];
                        PrintGemEffectToStringBuffer( socketedBaseItem.ItemType == ITEM_1_WEAPON ? gemInfoOpt->weaponEffect : gemInfoOpt->armorEffect, isTwoHanded );
                        Tooltip_AddLine( InfoPanelBuffer );
                        
                        int price = Craft_Socketing_CalculateGemExtractionFinalPrice( gemInfoOpt->grade );
                        sprintf(InfoPanelBuffer, L("Extraction price: %i"), price);
                        Tooltip_AddLine(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }else{
                    Tooltip_AddLine( L("can be inserted into socketed items"));
                    
                    PrintGemEffectToStringBuffer( gemInfoOpt->weaponEffect );
                    sprintf(buffer, L("weapons: %s"), InfoPanelBuffer);
                    if( gemInfoOpt->weaponEffect.type == GemEffectType::Damage ){
                        if (HasTrait(CurrentPlayerIndex, TraitId::Pistoleer)) {
                            sprintf(buffer, "%s", buffer);
                        }
                        else {
                            sprintf(buffer, L("%s (2X FOR 2H WEAPONS)"), buffer);
                        }
                    }
                    Tooltip_AddLine( buffer );
                    
                    PrintGemEffectToStringBuffer( gemInfoOpt->armorEffect );
                    if( gemInfoOpt->type == GEM::Chrysolite ){
                        sprintf(buffer, L("body armors: %s"), InfoPanelBuffer);
                    }else{
                        sprintf(buffer, L("armors: %s"), InfoPanelBuffer);
                    }
                    Tooltip_AddLine( buffer );
                    
                    if( isSocketingPanel && !isCraftSlot ){
                        if( Craft_Socketing_IsSocketingItemReady() ){
                            if( Craft_Socketing_IsGemSuitableForCurrentItem( *gemInfoOpt ) ){
                                int price = Craft_Socketing_CalculateGemInsertFinalPrice( gemInfoOpt->grade );
                                sprintf(InfoPanelBuffer, L("Insertion price: %i"), price);
                                Tooltip_AddLine(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                            }else{
                                Tooltip_AddLine(L("Not suitable for current item"), C_2_Red);
                            }
                        }
                    }
                }
                if (IsCraftPanelVisible && CurrentCraftPage == CraftPage::CP_Gems && !isCraftSlot) {
                    if (IsController() && !XinputNotInUse(true)) {
                        Tooltip_AddLine("Right Trigger + A to move to crafting slot");
                    }
                    else {
                        Tooltip_AddLine(L("Shift click to move to crafting slot"));
                    }
                }
            }
            break;
        }
        }
        
        if( restoreDurMin || restoreDurMax ){
            if( restoreDurMin == restoreDurMax ){
                sprintf(InfoPanelBuffer, L("restores %i points"), restoreDurMin);
            }else{
                sprintf(InfoPanelBuffer, L("restores %i-%i points"), restoreDurMin, restoreDurMax);
            }
            Tooltip_AddLine( InfoPanelBuffer );
            
            Tooltip_AddLine( L("of current durability") );
        }
        
        if( addDurMin || addDurMax ){
            if( addDurMin == addDurMax ){
                sprintf(InfoPanelBuffer, L("+%i to max durability"), addDurMin);
            }
            else{
                sprintf(InfoPanelBuffer, L("+[%i-%i] to max durability"), addDurMin, addDurMax);
            }
            if( addDurLimit ){
                sprintf(InfoPanelBuffer, "%s,", InfoPanelBuffer);
            }
            Tooltip_AddLine( InfoPanelBuffer );
            
            if( addDurLimit ){
                sprintf(InfoPanelBuffer, L("up to %i points"), addDurLimit);
                Tooltip_AddLine( InfoPanelBuffer ); 
                int elTorso = addDurLimit * 2;
                sprintf(InfoPanelBuffer, L("(%i for body armor)"), elTorso);
                Tooltip_AddLine(InfoPanelBuffer);
            }
        }
        
        if( IsValueStoneItem(item.baseItemIndex) ){
            Tooltip_AddLine(L("Sell in town for gold"));
        }

    }
    
	if( item.socketsAmount > 0 && (not(GameMode, GM_IRONMAN, GM_NIGHTMARE, GM_SPEEDRUN) || item.MagicLevel < ML_2_UNIQUE) ){
		sprintf(InfoPanelBuffer, L("Socketed [%i]"), item.socketsAmount );
        Tooltip_AddLine( InfoPanelBuffer, C_1_Blue );
	}
	for( int i = 0; i < countof(item.effect); ++i ){
		Effect& e = item.effect[i];
		if( e.id != AE_NO && e.origin == AO_GEM && PrintItemEffectToStringBuffer(e.id, &item, i) ){
			Tooltip_AddLine(InfoPanelBuffer, C_1_Blue);
		}
	}
	
    // Usage hint
    if( !IsVisualTradePanelVisible && !IsStashPanelVisible )
    {
        switch (item.MagicCode) {
        case MC_2_POTION_OF_FULL_HEALING:
        case MC_3_POTION_OF_HEALING:
        case MC_6_POTION_OF_MANA:
        case MC_7_POTION_OF_FULL_MANA:
        case MC_18_POTION_OF_REJUVENATION:
        case MC_19_POTION_OF_FULL_REJUVENATION:
        case MC_10_ELIXIR_OF_STRENGTH:
        case MC_11_ELIXIR_OF_MAGIC:
        case MC_12_ELIXIR_OF_DEXTERITY:
        case MC_13_ELIXIR_OF_VITALITY:
        case MC_44_SPECTRAL_ELIXIR:
        case MC_30_OIL_OF_SOMETHING:
        case MC_31_OIL_OF_REPAIR_I:
        case MC_32_OIL_OF_REPAIR_II:
        case MC_33_OIL_OF_REPAIR_III:
        case MC_34_OIL_OF_REPAIR_IV:
        case MC_35_OIL_OF_REPAIR_V:
        case MC_36_BLACKSMITH_OIL:
        case MC_37_OIL_OF_FORTITUDE:
        case MC_38_OIL_OF_REPAIR_VI:
        case MC_39_OIL_OF_REPAIR_VII:
        case MC_40_OIL_OF_HARDENING:
        case MC_47_RUNE_OF_FIRE:
        case MC_48_RUNE_OF_LIGHTNING:
        case MC_49_RUNE_OF_SHOCK:
        case MC_50_RUNE_OF_BLAZE:
        case MC_51_RUNE_OF_STONE:
            Tooltip_AddLine(L("Right click to use"));
            break;
        case MC_21_RELIC_NEED_NO_TARGET:
        case MC_24_BOOKS:
        case MC_54_RECONSTRUCTED_NOTE:
        {
            if (item.baseItemIndex == BI_2888_SELF_REPAIR_MODULE) {
                Tooltip_AddLine("Right click to perform self-repair");
            }
            else {
                Tooltip_AddLine(L("Right click to read"));
            }
        }
            break;
        case MC_22_RELIC_NEED_TARGET:
            Tooltip_AddLine(L("Right-click to read, then"));
            Tooltip_AddLine(L("left-click to target"));
            break;
        case MC_42_MAP_OF_THE_STARS:
            Tooltip_AddLine(L("Right click to view")); // (th4) localizable
            break;
        case MC_43_EAR_HEART:
            sprintf(InfoPanelBuffer, L("Level : %i"), item.quality); // (th4) localizable
            Tooltip_AddLine(InfoPanelBuffer);
            break;
        case MC_53_AURIC_AMULET:
            Tooltip_AddLine("Doubles gold capacity");
            break;
        }
    
		if (item.baseItemIndex == BI_33_STAFF_OF_LAZARUS && HasTrait(CurrentPlayerIndex, TraitId::Leper)) {
			Tooltip_AddLine(L("Right click to use"));
		}
        else if( isTownPortalForbidden()  ){
			switch (item.baseItemIndex) {
				case BI_19_FUNGAL_TOME:		 Tooltip_AddLine(L("Right click to transmute into spectral elixir")); break;
                case BI_2512_FULL_CONTAINERS:Tooltip_AddLine(L("Right click to transmute into random elixir")); break;
                case BI_33_STAFF_OF_LAZARUS: Tooltip_AddLine(L("Right click to use")); break;
				case BI_16_ANVIL_OF_FURY: 
				case BI_9_MAGIC_ROCK:
				case BI_327_TAVERN_SIGN:
                case BI_2457_IRON_FIGURINE:
                case BI_2459_REAGENTS:
                case BI_2460_THE_RIGHT_GOODS:
                case BI_2510_DUSTY_CRATE:
                case BI_2461_MUNDANE_INVENTORY:
                case BI_334_THEODORE:		 Tooltip_AddLine("Right click to transmute into quest reward"); break;
			}
        }
		if (isTownPortalForbidden()) {
			if (item.baseItemIndex >= BI_296_QUARTZ && item.baseItemIndex <= BI_301_AQUAMARINE) {
				Tooltip_AddLine("Right click to transmute into gold");
			}
		}
        if (item.baseItemIndex == BI_2472_THE_LOCKED_CHEST) { 
            Tooltip_AddLine("Right click to open (requires key)");
        }
        if (item.baseItemIndex == BI_2476_THE_STRANGE_BOOK) {
            Tooltip_AddLine(L("Right click to read (requires crystal eyepiece)"));
        }

        if( IsItemAllowedForBelt(item) ){
                if (IsController() && !XinputNotInUse(true)) {
                    if (slot >= IS_50_47_Belt_Start && slot < IS_50_47_Belt_End) {
                        Tooltip_AddLine("Right trigger to move left");
                    }
                    else {
                        Tooltip_AddLine("Right trigger to move to belt");
                    }
                }
                else {
                    if (slot >= IS_50_47_Belt_Start && slot < IS_50_47_Belt_End) {
                        Tooltip_AddLine(L("Shift Click to move left"));
					}else if( ! IsTransmutePanelVisible ){
						Tooltip_AddLine(L("Shift Click to move to belt"));
                    }
                }
        }
    }
 
	// Requirements
	if( item.MagicCode == MC_24_BOOKS && item.RequiredLevel == 255u ){
		Tooltip_AddLine( "Spell is maxed", C_3_Gold );
	}else if( item.RequiredMagic || item.RequiredStrength || item.RequiredDexterity || item.RequiredVitality || item.RequiredLevel ){
		strcpy(InfoPanelBuffer, L("required:"));
		
		auto appendReqInfo = [&player]( const char* name, int req, int current, int magicCode, bool isLevel = false )
        {
            if( req ){
				uchar color;
				if( current >= req ){
					color = C_0_White + 128;
                }else if( !isLevel && req <= current + ( is(magicCode,
                 MC_24_BOOKS, MC_21_RELIC_NEED_NO_TARGET, MC_22_RELIC_NEED_TARGET, 
                 MC_47_RUNE_OF_FIRE, MC_48_RUNE_OF_LIGHTNING, MC_49_RUNE_OF_SHOCK,
                 MC_50_RUNE_OF_BLAZE, MC_51_RUNE_OF_STONE) ? 0 : player.AdvancedUser) ){
					color = C_1_Blue + 128;
                }else{
					color = C_2_Red + 128;
                }
                sprintf( InfoPanelBuffer, "%s %c%i %s%c", InfoPanelBuffer, color, req, name, color );
            }
        };
		
		appendReqInfo( L("lvl"), item.RequiredLevel, player.CharLevel, item.MagicCode, true); // (th4) isLevel: level is never lowered by Advanced User, so it goes straight from white to red. This used to be `name != "lvl"`, a POINTER compare: name is L("lvl"), a different pointer in every translated language, so a missed level requirement showed blue there. Same fix in all three tooltip copies.
		appendReqInfo( L("str"), item.RequiredStrength, player.CurStrength, item.MagicCode);
		appendReqInfo( L("mag"), item.RequiredMagic, player.CurMagic, item.MagicCode);
		appendReqInfo( L("dex"), item.RequiredDexterity, player.CurDexterity, item.MagicCode);
		appendReqInfo( L("vit"), item.RequiredVitality, player.CurVitality, item.MagicCode );
        Tooltip_AddLine(InfoPanelBuffer);
	}
	
	uint playerFullClassMask = (1u << static_cast<uint>( player.fullClassId ) );
	uint requiredClassMask = GetRequiredClassMask( &item );
    bool sort_out = false;
    if (requiredClassMask == IPCM_ALL_CLASSES) {
        sort_out = true;
    }
    else if (player.gameChanger & BIT(GC_27_TERMINATOR)
        || player.gameChanger & BIT(GC_28_SUPERMUTANT)
        || player.gameChanger & BIT(GC_29_ENCLAVE_SOLDIER)) {
        sort_out = true;
    }
    if (!sort_out) {
	    printItemClassReqInfoInBuffer( &item );
	    Tooltip_AddLine( InfoPanelBuffer, (playerFullClassMask & requiredClassMask) ? C_0_White : C_2_Red );
	}
 
	if( (playerFullClassMask & IPCM_ANY_EXILE) != 0 && baseItem.ForbiddenForExiles ){
		Tooltip_AddLine( "[Can't wear item]", C_2_Red );
	}
	else if (HasTrait(CurrentPlayerIndex, TraitId::OldFashioned)) {
		if (is(item.ItemCode, IC_6_LIGHT_ARMOR, IC_8_MID_ARMOR, IC_9_HEAVY_ARMOR)) {
			Tooltip_AddLine("[Can't wear body armor]", C_2_Red);
		}
	}
	else if (HasTrait(CurrentPlayerIndex, TraitId::Fechtmeister, TraitId::Ranger, TraitId::Mamluk)) {
		if (is(item.ItemCode, IC_5_SHIELD)) {
			Tooltip_AddLine("[Can't use shields]", C_2_Red);
		}
	}
	else if (HasTrait(CurrentPlayerIndex, TraitId::Black_Witchery)) {
		if (is(item.ItemCode, IC_21_KNIFE)) {
			Tooltip_AddLine("[Can't use throwing knives]", C_2_Red);
		}
	}
    else if (HasTrait(CurrentPlayerIndex, TraitId::Paladin) || player.fullClassId == PFC_DRUID) {
        if (is(item.ItemCode, IC_3_BOW)) {
            Tooltip_AddLine("[Can't use crossbows]", C_2_Red);
        }
        else if (is(item.ItemCode, IC_22_MALLET)) {
            Tooltip_AddLine("[Can't use throwing mallets]", C_2_Red);
        }
    }
    else if (HasTrait(CurrentPlayerIndex, TraitId::TwoTowers)) {
		if (is(item.ItemCode, IC_1_SWORD, IC_4_MACE)) {
			Tooltip_AddLine("[Can't use melee weapons]", C_2_Red);
		}
	}
	else if (HasTrait(CurrentPlayerIndex, TraitId::Axepertise)) {
		if (is(item.ItemCode, IC_1_SWORD, IC_4_MACE)) {
			Tooltip_AddLine("[Can't use item]", C_2_Red);
		}
	}
	if (HasTrait(CurrentPlayerIndex, TraitId::WildSorcery) || HasTrait(CurrentPlayerIndex, TraitId::Rabid)) {
		if (is(item.MagicCode,
			MC_3_POTION_OF_HEALING,
			MC_6_POTION_OF_MANA,
			MC_2_POTION_OF_FULL_HEALING,
			MC_7_POTION_OF_FULL_MANA,
			MC_18_POTION_OF_REJUVENATION,
			MC_19_POTION_OF_FULL_REJUVENATION)) {
			Tooltip_AddLine("[Can't use potions]", C_2_Red);
		}
	}
	//----------------------
	if( IsVisualTradePanelVisible ){
	
	    if( isVisualTradeSlot ){
	        int price = VisualTrade_GetBuyPrice(&item);
                sprintf(InfoPanelBuffer, L("Buy price: %i"), price);
                Tooltip_AddLine(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                if (IsController() && !XinputNotInUse(true)) {
                    Tooltip_AddLine("Right Trigger + A to buy fast");
                }
                else {
                    Tooltip_AddLine(L("Shift click to buy fast"));
                }
        }else{
            if( Cur.GraphicsID == CM_1_NORMAL_HAND ){
                if( CurrentVisualTradeMode != VTM_Elder ){
                    int price = VisualTrade_GetSellPrice(&item);
                    if( price ){
                        sprintf(InfoPanelBuffer, L("Sell price: %i"), price);
                        Tooltip_AddLine(InfoPanelBuffer);
                        if (IsController() && !XinputNotInUse(true)) {
                            Tooltip_AddLine("Right Trigger + A to sell fast");
                        }
                        else {
                            Tooltip_AddLine(L("Shift click to sell fast"));
                        }
                    }else{
                        Tooltip_AddLine(L("Item cannot be sold"), C_2_Red);
                    }
                }
            }else if( Cur.IsTradeService ){
                if( Cur.GraphicsID == CM_2_IDENTIFY ){
                    int price = VisualTrade_GetIdentificationPrice(&item);
                    if( price ){
                        sprintf(InfoPanelBuffer, "Identification price: %i", price);
                        Tooltip_AddLine(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }else if( Cur.GraphicsID == CM_3_REPAIR ){
                    int price = VisualTrade_GetRepairPrice(&item);
                    if( price ){
                        sprintf(InfoPanelBuffer, "Repair price: %i", price);
                        Tooltip_AddLine(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }else if( Cur.GraphicsID == CM_4_STAFF_RECHARGE ){
                    int price = VisualTrade_GetRechargePrice(&item);
                    if( price ){
                        sprintf(InfoPanelBuffer, "Recharge price: %i", price);
                        Tooltip_AddLine(InfoPanelBuffer, player.TotalGold >= price ? C_0_White : C_2_Red);
                    }
                }
            }
        }
    }
    
    if( IsStashPanelVisible ){
        if( slot >= IS_Inventory ){
            if( !IsItemAllowedForStash(&item) ){
                Tooltip_AddLine(L("Cannot be stashed"), C_2_Red);
            }else{
                if (IsController() && !XinputNotInUse(true)) {
                    if (isVisualStashSlot) {
                        Tooltip_AddLine("Right Trigger + A to move to inventory");
                    }
                    else {
                        Tooltip_AddLine("Right Trigger + A to move to stash");
                    }
                }
                else {
                    if (isVisualStashSlot) {
                        Tooltip_AddLine(L("Shift Click to move to inventory"));
                    }
                    else {
                        Tooltip_AddLine(L("Shift Click to move to stash"));
                    }
                }
            }
        }
    }
	if( IsTransmutePanelVisible && Transmute_Allowed(item) ){
		if( slot >= Stash_SlotIDStart ){
			Tooltip_AddLine(L("Shift Click to move to inventory"), C_7_Grey);
		}else if (slot >= IS_Inventory) {
			Tooltip_AddLine(L("Shift Click to transmute panel"), C_7_Grey);
		}
	}
    // --- NEW: Check for sigils ---
  //Item& item = *itemPtr;
    bool isSigil = (item.baseItemIndex >= BI_2504_SIGIL_FIRST && item.baseItemIndex <= BI_2508_SIGIL_LAST);
    // -----------------------------
    if (SecondaryTooltip && !IsAltPressed) {
        if (((slot > IS_Feet && slot < IS_50_47_Belt_Start)
            || StashPanel_IsStashSlot(slot)
            || VisualTrade_IsVisualTradeSlot(slot)
            || Craft_IsCraftSlot(slot)
            || IsEnchantPanelVisible && slot == 101
			|| IsQuenchPanelVisible  && slot == 102)
			&& !is(item.ItemCode, IC_0_OTHER, IC_11_GOLD)
            && !isSigil// dont work for sigils
            )
            if (IsController() && !XinputNotInUse(true)) {
                Tooltip_AddLine("Press Left Trigger to compare", C_7_Grey);
            }
            else {
                Tooltip_AddLine(L("Press ALT to compare"), C_7_Grey);
            }
    }

    if (slot <= IS_50_47_Belt_End) {
        if (IsController() && !XinputNotInUse(true)) {
            Tooltip_AddLine("Press X to drop", C_7_Grey);
        } 
        else if (DropItemOnCtrl) {
            Tooltip_AddLine(L("Ctrl Click to drop"), C_7_Grey);
        }
    }
  
    if (FastInventoryItemSwap)
    {
        if (!IsVisualTradePanelVisible && !IsStashPanelVisible) {
            if (slot <= IS_Feet) {
                if (IsController() && !XinputNotInUse(true)) {
                    Tooltip_AddLine("Right Trigger + A to unequip", C_7_Grey);
                }else{
                    Tooltip_AddLine(L("Shift Click to unequip"), C_7_Grey);
                }
            }else if( slot >= IS_Inventory
			 && (is(BaseItems[item.baseItemIndex].ItemType, ITEM_1_WEAPON, ITEM_2_ARMOR) || is(item.ItemCode, IC_12_RING, IC_13_AMULET))
			 && !IsTransmutePanelVisible && !isSigil){
                if (IsController() && !XinputNotInUse(true)) {
                    Tooltip_AddLine("Right Trigger + A to equip", C_7_Grey);
                }else{
                    if( RightSwapAllowed(CurrentPlayerIndex, item) ){
                        Tooltip_AddLine(L("Shift Click to equip to left slot"), C_7_Grey);
                        if (FastSwapRightOnAlt) {
                            Tooltip_AddLine(L("Alt Click to equip to right slot"), C_7_Grey);
                        }
                    }else{
                        Tooltip_AddLine(L("Shift Click to equip"), C_7_Grey);
                    }
                }
            }
        }
    }
    
 	if (identified && item.MagicLevel == ML_3_SET) {
		UniqueItem& uniq = UniqueItems[item.UniqueItemIndex];
		UniqueSet& set = UniqueSets[uniq.uniqueSetIndex];

		Tooltip_AddLine(""); // blank line
		bool hasActiveEffects = false;
		int countOfEquippedItemsFromSet = SetItem_GetCountOfEquippedItemsFromSet(CurrentPlayerIndex, uniq.uniqueSetIndex);
		for (int i = 0, ie = set.EffectCount; i != ie; ++i) {
			auto& e = set.Effects[i];
			if (countOfEquippedItemsFromSet >= e.Threshold) {
				if( PrintItemEffectToStringBuffer(e.Effect.id, const_cast<Item*>(&set.FakeItemToShowEffect), i) ){
					Tooltip_AddLine(InfoPanelBuffer, C_4_Orange);
				}
				hasActiveEffects = true;
			}
		}
		if (hasActiveEffects) Tooltip_AddLine(""); // blank line
		Tooltip_AddLine(LocalizedUniqueItemName(set.Name), C_3_Gold);
		for (int i = 0; i < set.UniquesCount; ++i)
		{
			int uniqueItemIndex = set.UniquesList[i];
			UniqueItem& uniq2 = UniqueItems[uniqueItemIndex];
			Tooltip_AddLine(LocalizedUniqueItemName(uniq2.Name), SetItem_IsItemFromSetEquipped(CurrentPlayerIndex, uniqueItemIndex) ? C_1_Blue : C_2_Red);
		}
	}
}
