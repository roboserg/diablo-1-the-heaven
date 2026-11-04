// ArabicText.cpp  (th4)
// -----------------------------------------------------------------------------
// Arabic shaping and a restricted bidi reorder, for a renderer that only knows how
// to walk a byte string left to right.
//
// TWO JOBS, and the literature is unanimous that doing one without the other is
// what ships broken: SHAPING (a letter takes one of four contextual forms so the
// script joins) and BIDI (the run reverses, but numbers inside it do not). Skip the
// first and you get the PES 2018 failure -- disconnected letters. Skip the second
// and you get the PUBG failure -- a sentence in the wrong order.
//
// WHY THIS RUNS AT DRAW TIME rather than baking visual strings into the tables:
// TH4 substitutes %i at runtime and wraps paragraphs at runtime. Both would land
// inside an already-reversed string and corrupt it. By the time these functions are
// called the line is final.
//
// WHY NOT full UAX #9: it is 125 embedding levels and half a million conformance
// tests. TH4 produces Arabic runs, Latin runs, digit runs, spaces and brackets and
// nothing else, so this implements exactly those rules and nothing more.
//
// TABLES ARE GENERATED, NOT HAND-WRITTEN. /home/claude/ar/mkar.py derives them from
// the Unicode decompositions of the Arabic Presentation Forms-B block, so the form
// ids here and the glyph cells in TH4loc_ara.mor come from one source. Regenerate
// both together or neither.
#include "stdafx.h"
#include <string.h>   // (th4) strchr, for the UAX #9 neutral test

enum { AR_BASE_COUNT = 50, AR_NONE = 0xFFFF };

// Joining class per base slot: 0 = non-joining, 1 = right-joining (joins only the
// letter before it), 2 = dual-joining. This is the whole of the shaping rule.
static const unsigned char AR_Join[AR_BASE_COUNT] = {
    0, 1, 1, 1, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};

static const unsigned short AR_Iso[AR_BASE_COUNT] = {
    16, 17, 19, 21, 23, 25, 29, 31, 35, 37, 41, 45, 49, 53, 57, 59,
    61, 63, 65, 69, 73, 77, 81, 85, 89, 93, 97, 101, 105, 109, 113, 117,
    121, 125, 127, 129, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154,
    155, 156,
};

static const unsigned short AR_Ini[AR_BASE_COUNT] = {
    65535, 65535, 65535, 65535, 65535, 27, 65535, 33, 65535, 39, 43, 47, 51, 55, 65535, 65535,
    65535, 65535, 67, 71, 75, 79, 83, 87, 91, 95, 99, 103, 107, 111, 115, 119,
    123, 65535, 65535, 131, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535,
};

static const unsigned short AR_Med[AR_BASE_COUNT] = {
    65535, 65535, 65535, 65535, 65535, 28, 65535, 34, 65535, 40, 44, 48, 52, 56, 65535, 65535,
    65535, 65535, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120,
    124, 65535, 65535, 132, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535,
};

static const unsigned short AR_Fin[AR_BASE_COUNT] = {
    65535, 18, 20, 22, 24, 26, 30, 32, 36, 38, 42, 46, 50, 54, 58, 60,
    62, 64, 66, 70, 74, 78, 82, 86, 90, 94, 98, 102, 106, 110, 114, 118,
    122, 126, 128, 130, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535,
};

// lam + alef must fuse into one glyph -- it is not optional in Arabic.
struct ArLig { unsigned short lam, alef, iso, fin; };
static const ArLig AR_Lig[] = {
    { 29, 1, 133, 134 },
    { 29, 2, 135, 136 },
    { 29, 4, 137, 138 },
    { 29, 6, 139, 140 },
};
static const int AR_LigCount = 4;

// ASCII '0'..'9' -> Arabic-Indic cells. Ibragim's call: the mod shows Arabic-Indic
// digits, so the substitution happens here rather than in the tables -- %i values
// arrive as ASCII at runtime and would otherwise stay Western.
static const unsigned short AR_Digit[10] = { 143, 144, 145, 146, 147, 148, 149, 150, 151, 152 };

// ---------------------------------------------------------------------------
// Public API. AR_Glyph is what comes out: either a font cell (isForm) or a raw
// byte the caller draws through the ordinary Latin path.
// ---------------------------------------------------------------------------
// AR_Glyph is declared in funcs.h -- one definition, shared with the callers.

// A glyph belongs to a left-to-right run if it is a digit or a Latin letter. Spaces
// and punctuation are neutral and simply inherit whichever run they are adjacent to,
// which is what the run-splitting loop below does by treating them as right-to-left.
static inline int AR_IsLtr( const AR_Glyph& g )
{
	const unsigned char r = g.raw;
	if( r >= '0' && r <= '9' ) return 1;
	if( ( r >= 'A' && r <= 'Z' ) || ( r >= 'a' && r <= 'z' ) ) return 1;
	return 0;
}


static inline int AR_Slot( unsigned char lead, unsigned char trail )
{
	return (int)( lead - AR_LEAD_FIRST ) * AR_ROW_LEN + (int)( trail - AR_TRAIL_FIRST );
}

// Shape one line. src is a LOGICAL-order byte string in the private double-byte
// encoding (see vars.h); ASCII passes through untouched except for digits.
int AR_Shape( const unsigned char* src, int len, AR_Glyph* out, int maxOut )
{
	// Pass 1: decode to slots. Negative means "not Arabic, keep the raw byte".
	static int cp[ 512 ];
	int n = 0;
	for( int i = 0; i < len && n < 512; ){
		const unsigned char b = src[i];
		if( b >= AR_LEAD_FIRST && b <= AR_LEAD_LAST && i + 1 < len
		 && src[i+1] >= AR_TRAIL_FIRST && src[i+1] <= AR_TRAIL_LAST ){
			const int s = AR_Slot( b, src[i+1] );
			cp[n++] = ( s >= 0 && s < AR_BASE_COUNT ) ? s : -(int)b;
			i += 2;
		}else{
			cp[n++] = -(int)b;
			i += 1;
		}
	}

	// Pass 2: pick a form for every letter from its neighbours.
	int outN = 0;
	for( int k = 0; k < n && outN < maxOut; ){
		const int c = cp[k];
		if( c < 0 ){
			const unsigned char raw = (unsigned char)( -c );
			if( raw >= '0' && raw <= '9' ){
				// Arabic-Indic digits, substituted here because %i values only exist
				// at runtime -- see AR_Digit.
				out[outN].cell = AR_Digit[ raw - '0' ];
				out[outN].raw = raw;          // kept: the reorder needs to know it is a digit
				out[outN].isForm = 1;
			}else if( raw >= 0x80 && raw <= 0x8B ){
				// (th4) COLOUR-CONTROL BYTE, not text. The Latin path consumes these as
				// commands and draws nothing; passing one through as a raw glyph made
				// FontIndexSmall paint a stray letter -- the 'U' that appeared wherever a
				// %c sat in a stat line. isForm 0 with raw 0 means "a placeholder that draws nothing":
				// it still occupies a slot so the reorder sees the run correctly.
				out[outN].cell = 0; out[outN].raw = 0; out[outN].isForm = 0;
			}else{
				out[outN].cell = 0; out[outN].raw = raw; out[outN].isForm = 0;
			}
			++outN; ++k; continue;
		}

		// lam + alef is a mandatory ligature, so it is tested before ordinary forms.
		if( k + 1 < n && cp[k+1] >= 0 ){
			int hit = -1;
			for( int L = 0; L < AR_LigCount; ++L ){
				if( AR_Lig[L].lam == c && AR_Lig[L].alef == cp[k+1] ){ hit = L; break; }
			}
			if( hit >= 0 ){
				const int prev = ( k > 0 ) ? cp[k-1] : -1;
				const bool joinsPrev = ( prev >= 0 && AR_Join[prev] == 2 );
				out[outN].cell = joinsPrev ? AR_Lig[hit].fin : AR_Lig[hit].iso;
				out[outN].raw = 0; out[outN].isForm = 1;
				++outN; k += 2; continue;
			}
		}

		const int prev = ( k > 0 )     ? cp[k-1] : -1;
		const int next = ( k + 1 < n ) ? cp[k+1] : -1;
		// A letter joins backwards only if the letter before it is DUAL-joining, and
		// forwards only if it is itself dual and the next letter accepts a join.
		const bool joinsPrev = ( prev >= 0 && AR_Join[prev] == 2 );
		const bool joinsNext = ( AR_Join[c] == 2 && next >= 0 && AR_Join[next] != 0 );

		unsigned short cell = AR_NONE;
		if(      joinsPrev && joinsNext ) cell = AR_Med[c];
		else if( joinsNext )              cell = AR_Ini[c];
		else if( joinsPrev )              cell = AR_Fin[c];
		if( cell == AR_NONE )             cell = AR_Iso[c];   // fall back, never draw nothing
		out[outN].cell = cell; out[outN].raw = 0; out[outN].isForm = 1;
		++outN; ++k;
	}
	return outN;
}

// Reorder one SHAPED line for display, base direction right-to-left.
//
// The only subtlety, and the one that is easy to get wrong: a run of digits keeps
// its own left-to-right order while the line around it runs right-to-left. UAX #9
// gets there via W2/W4 and the L2 reversal pass; the same result falls out here by
// treating digits and Latin as left-to-right runs and reversing only the run ORDER.
// "+15" must read 15, never 51.
// (th4) UAX #9 neutrals: whitespace and the punctuation that carries no direction
// of its own. Rule N1 gives such a character the direction of its neighbours when
// they agree, which is what keeps "Diablo: The Hell 4" from being torn into runs
// at every colon and full stop.
static int AR_IsNeutral( unsigned char c )
{
	if( c == 0 ) return 0;
	if( c <= ' ' ) return 1;
	return strchr( "!\"#%&'()*+,-./:;<=>?@[\\]^_`{|}~", (char)c ) != 0;
}

// (th4) UAX #9 rule L4: a mirrored character is swapped for its pair when it
// resolves right-to-left. Without this "(PVP)" draws as ")PVP(" -- the glyphs
// keep their logical shape while the surrounding order flips, so the brackets
// point the wrong way. Only the pairs TH4 actually uses are listed.
static unsigned char AR_Mirror( unsigned char c )
{
	switch( c ){
		case '(': return ')';
		case ')': return '(';
		case '[': return ']';
		case ']': return '[';
		case '{': return '}';
		case '}': return '{';
		case '<': return '>';
		case '>': return '<';
	}
	return c;
}

void AR_Reorder( AR_Glyph* g, int n )
{
	if( n <= 1 ) return;
	// (th4) UAX #9 rule N1 -- a NEUTRAL between two characters of the same direction
	// takes that direction. Without this a space between two Latin words is its own
	// right-to-left run, so "New Hero Multi Player" comes out "Player Multi Hero New":
	// each word keeps its letters but the WORD order reverses. Resolve neutrals first,
	// then split into runs.
	static unsigned char dir[ 512 ];
	if( n > 512 ) return;
	for( int i = 0; i < n; ++i ) dir[i] = (unsigned char)AR_IsLtr( g[i] );
	for( int i = 0; i < n; ++i ){
		if( dir[i] || g[i].isForm || g[i].raw == 0 ) continue;   // only true neutrals
		if( AR_IsNeutral( g[i].raw ) == 0 ) continue;
		int a = i - 1, b = i + 1;
		while( b < n && !dir[b] && !g[b].isForm && AR_IsNeutral( g[b].raw ) ) ++b;
		if( a >= 0 && b < n && dir[a] && dir[b] )
			for( int k = i; k < b; ++k ) dir[k] = 1;
	}
	static AR_Glyph tmp[ 512 ];
	if( n > 512 ) return;

	int w = 0;
	// (th4) L4: mirror brackets that ended up right-to-left.
	for( int i = 0; i < n; ++i )
		if( !dir[i] && !g[i].isForm && g[i].raw )
			g[i].raw = AR_Mirror( g[i].raw );

	int end = n;
	while( end > 0 ){
		// walk backwards, copying whole runs
		const int t = dir[end-1];
		int start = end - 1;
		while( start > 0 && dir[start-1] == t ) --start;
		if( t ){
			for( int i = start; i < end; ++i ) tmp[w++] = g[i];       // keep order
		}else{
			for( int i = end - 1; i >= start; --i ) tmp[w++] = g[i];  // reverse
		}
		end = start;
	}
	for( int i = 0; i < n; ++i ) g[i] = tmp[i];
}

// (th4) One call for every byte-walking draw site: shape a logical string and put
// it in visual order. Sites differ only in how they paint a glyph, so this keeps
// the language logic in one place instead of copied into a dozen loops.
//
// Returns the glyph count. Each entry is either a font cell (isForm, draw at
// AR_FIRST_ID + cell with a fixed Arabic cell width) or a raw byte to push through
// the ordinary Latin path.
int AR_Layout( const char* text, AR_Glyph* out, int maxOut )
{
	// (th4) A string with NO Arabic in it is not Arabic text -- the mod name and
	// version line, a filename, a raw id. Shaping it is a no-op, but the reorder
	// would reverse its runs and the digit rule would swap 0-9 for Arabic-Indic,
	// so "Diablo: The Hell 4 (ALPHA) v.0.06" came out mirrored with eastern digits.
	// Detect the pure-Latin case up front and copy the bytes through untouched.
	{
		int hasAr = 0;
		for( const unsigned char* p = (const unsigned char*)text; *p; ++p )
			if( *p >= AR_LEAD_FIRST && *p <= AR_LEAD_LAST ){ hasAr = 1; break; }
		if( !hasAr ){
			int m = 0;
			for( const unsigned char* p = (const unsigned char*)text; *p && m < maxOut; ++p, ++m ){
				out[m].isForm = 0;
				out[m].cell   = 0;
				out[m].raw    = *p;
			}
			return m;
		}
	}
	if( !text || !out || maxOut <= 0 ) return 0;
	const int n = AR_Shape( (const unsigned char*)text, (int)strlen( text ), out, maxOut );
	AR_Reorder( out, n );
	return n;
}

// (th4) PROPORTIONAL ADVANCE. Cells are fixed width so the sheet lines up, but
// stepping a whole cell per letter is what made Arabic sprawl -- most letters use
// well under half their cell. These are the glyph BODY widths plus 2px; the cells
// then OVERLAP, which is harmless because only ink is written and each glyph
// carries its connector out to its own cell edge, so the joins still meet.
// Generated alongside the font in /home/claude/ar -- regenerate both together.
static const unsigned char AR_AdvSmall[AR_FORM_COUNT] = {
    3, 5, 3, 3, 3, 7, 2, 8, 3, 8, 3, 8, 5, 8, 3, 8, 5, 4, 5, 2, 4, 6, 7, 2,
    4, 8, 10, 7, 8, 1, 3, 10, 12, 7, 8, 5, 7, 10, 12, 7, 8, 10, 12, 7, 8, 7, 8, 11,
    12, 7, 8, 11, 12, 7, 8, 11, 12, 5, 6, 5, 6, 6, 8, 6, 8, 14, 15, 14, 15, 14, 15, 14,
    15, 14, 15, 14, 15, 14, 15, 14, 15, 10, 12, 13, 14, 10, 12, 13, 14, 7, 7, 10, 10, 7, 7, 10,
    10, 11, 13, 9, 11, 8, 10, 9, 11, 9, 10, 10, 11, 8, 9, 7, 9, 7, 8, 10, 12, 8, 9, 7,
    8, 5, 7, 10, 10, 6, 7, 8, 10, 8, 10, 7, 8, 8, 9, 7, 8, 6, 7, 6, 7, 7, 7, 2,
    3, 6, 6, 4, 6, 6, 6, 6, 6, 2, 2, 5, 5,
};
static const unsigned char AR_AdvMid[AR_FORM_COUNT] = {
    4, 6, 5, 4, 5, 9, 4, 9, 5, 9, 3, 9, 6, 9, 4, 9, 5, 6, 6, 3, 5, 7, 9, 3,
    5, 10, 12, 8, 9, 2, 4, 13, 15, 8, 9, 6, 8, 13, 15, 8, 9, 13, 15, 8, 9, 9, 9, 13,
    15, 9, 9, 13, 15, 9, 9, 13, 15, 6, 9, 6, 9, 8, 10, 8, 10, 17, 20, 17, 19, 17, 20, 17,
    19, 17, 19, 17, 19, 17, 19, 17, 19, 13, 14, 16, 18, 13, 14, 16, 18, 9, 9, 12, 12, 9, 9, 12,
    12, 14, 16, 11, 13, 11, 14, 11, 13, 11, 13, 12, 13, 9, 11, 8, 10, 8, 10, 12, 14, 10, 11, 8,
    9, 6, 8, 12, 12, 7, 9, 10, 12, 10, 12, 8, 9, 10, 12, 9, 11, 8, 10, 8, 10, 9, 9, 2,
    3, 8, 8, 6, 7, 8, 8, 8, 8, 3, 3, 6, 6,
};
static const unsigned char AR_AdvBig[AR_FORM_COUNT] = {
    9, 11, 9, 8, 9, 17, 9, 14, 9, 14, 9, 14, 11, 14, 9, 14, 10, 12, 12, 6, 9, 15, 19, 6,
    9, 21, 25, 12, 14, 3, 7, 26, 30, 11, 14, 12, 15, 26, 30, 12, 14, 26, 30, 12, 14, 19, 19, 22,
    25, 19, 19, 22, 25, 19, 19, 22, 25, 11, 16, 11, 16, 16, 20, 16, 20, 34, 39, 29, 33, 34, 39, 29,
    33, 34, 38, 29, 33, 34, 38, 29, 33, 25, 29, 28, 31, 25, 29, 28, 31, 18, 18, 20, 20, 18, 18, 20,
    20, 28, 31, 17, 21, 21, 26, 17, 21, 21, 25, 20, 22, 18, 23, 11, 15, 15, 20, 19, 23, 19, 23, 11,
    14, 12, 15, 19, 20, 15, 19, 21, 25, 21, 25, 12, 14, 19, 23, 16, 20, 15, 19, 14, 18, 17, 17, 4,
    7, 15, 15, 13, 13, 15, 16, 15, 15, 5, 5, 13, 11,
};

// size: 0 = small (17px cells), 1 = mid (22), 2 = big (46).

// (th4) Widest advance any FORM of this base letter can take.
//
// The speech wrapper fills a line from STORED BYTE PAIRS and has not shaped yet,
// so it cannot know which of the four forms a letter will end up in. It used to
// charge the whole cell -- 22px at mid, against real advances averaging 10.3 --
// which called a line full at about half its true width and wrapped every Arabic
// paragraph early, leaving a third of the panel unused. Taking the MAXIMUM over
// the letter's own forms keeps the estimate a safe upper bound (a line can still
// never overflow) while being close enough that lines fill the panel.
unsigned char AR_MaxAdvanceForPair( unsigned char lead, unsigned char trail, int size )
{
	const int slot = AR_Slot( lead, trail );
	if( slot < 0 || slot >= AR_BASE_COUNT ) return (unsigned char)AR_CELL_W_MID;
	int best = 0;
	const unsigned short* tabs[4] = { AR_Iso, AR_Ini, AR_Med, AR_Fin };
	for( int f = 0; f < 4; ++f ){
		const int cell = (int)tabs[f][ slot ];
		if( cell <= 0 ) continue;
		const int a = AR_Advance( cell, size );
		if( a > best ) best = a;
	}
	return (unsigned char)( best ? best : AR_CELL_W_MID );
}
int AR_Advance( int cell, int size )
{
	if( cell < 0 || cell >= AR_FORM_COUNT ) return 0;
	if( size == 0 ) return AR_AdvSmall[ cell ];
	if( size == 1 ) return AR_AdvMid[ cell ];
	return AR_AdvBig[ cell ];
}

