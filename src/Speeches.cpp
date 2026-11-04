#include "stdafx.h"

char* FontSpriteMid;
char* TextBoxCel;
int Speech_CurLineY;
char* Speech_FirstDrawingLine;
char Speech_IsPanelVisible;
// 004A7914
int Speech_ScrollSteps[11] = { 0, 2, 4, 6, 8, 0, -1, -2, -3, -4, 0	};
int Speech_ScrollCurStep;
int Speech_ScrollStep;
i64 Speech_PrevTick;

//----- (0042E4F5) --------------------------------------------------------
void FreeSpeechPanelPtrs()
{
	FreeMemZero(FontSpriteMid);
	FreeMemZero(TextBoxCel);
}

//----- (0042E519) --------------------------------------------------------
void LoadSpeechPanelData()
{
	FontSpriteMid = (char*)LoadFile( is(GameLanguage, LANG_RU, LANG_BG, LANG_UK) ? "Data\\medtexts_RUS.cel" : "Data\\MedTextS.CEL" ); // (th4)
	TextBoxCel = (char*)LoadFile("Data\\TextBox.CEL");
	Speech_IsPanelVisible = 0;
}

//----- (0042E543) --------------------------------------------------------
void __fastcall StartSpeech( int speechIndex )
{
	Speech& speech = Speeches[ speechIndex ];
	if( speech.HasText ){
		IsQUESTPanelVisible = false;
		Speech_FirstDrawingLine = LocalizedSpeech( speechIndex, speech.TextPtr ); // (th4) RU translation layer
		Speech_IsPanelVisible = true;
		Speech_CurLineY = Screen_TopBorder + Dialog_302_Height + (ScreenHeight - Dialog_302_Height - GUI_MainPanelHeight) / 2 + Speech_TopMargin;
		Speech_ScrollStep = Speech_ScrollSteps[ speech.stepIndex ];
		Speech_ScrollCurStep = Speech_ScrollStep;
		Speech_PrevTick = GetTickCountTh(); // (rt) this timer doesn't affect gameplay, not intercepted
	}
	PlayGlobalSound( speech.SoundIndex );
}

//----- (0042E600) --------------------------------------------------------
void __fastcall DrawLetterInSpeech( int xPos, int yPos, void* fontSet, int letterNumber )
{
	// (th4) Same guard as DrawLetter and Font_DrawChar. ChiLetterID returns -1 for
	// Arabic, and letterIndex would then be -2 -- a negative index into the frame
	// offset table.
	if( letterNumber < 1 ) return;
	int letterIndex = letterNumber - 1;
	int* fontData = (int*)fontSet;
	int* lettersOffsets = &fontData[1];
	int letterOffset = lettersOffsets[letterIndex];
	int nextLetterOffset = lettersOffsets[letterIndex + 1];
	int length = nextLetterOffset - letterOffset;
	char* pictureData = (char*)fontSet + letterOffset;
	int linePixelsLeft = Speech_LetterMaxWidth;

	uchar* curPixelPtr = WorkingSurface + YOffsetHashTable[yPos] + xPos;
	int dialogBottomY = Screen_TopBorder + Dialog_302_Height + (ScreenHeight - Dialog_302_Height - GUI_MainPanelHeight) / 2;
	uchar* speechTop = WorkingSurface + YOffsetHashTable[ dialogBottomY - Dialog_302_Height + Speech_TopMargin ];
	uchar* speechBottom = WorkingSurface + YOffsetHashTable[ dialogBottomY - Speech_TopMargin ];

	// (th4) A8L8: NPC speech is drawn straight into WorkingSurface, so on a dungeon map the
	// shader darkens it by whatever light level the tiles underneath happen to have -- which
	// is what made it unreadable with EnrichedLight on (e.g. the Dark Campaign hub). Clear the
	// matching LightSurface byte for every font pixel written so the text renders at full
	// brightness.
	//
	// lightOfs is a per-call constant: LightSurface is allocated parallel to WorkingSurface
	// with identical dimensions, so adding it to any valid WorkingSurface pointer gives the
	// matching light byte. Gated exactly as linecpyPerPixel does (engine.h): with
	// g_A8L8Lighting=false the shader samples an L8 texture and never reads LightSurface, so
	// the per-pixel store would be pure waste.
	const bool hasLight = g_A8L8Lighting && LightSurface;
	const intptr_t lightOfs = hasLight ? (intptr_t)LightSurface - (intptr_t)WorkingSurface : 0;

	// (th4) SNOWBOUND TOWN (PlayerTrade.cpp DrawGameDialogTitleText): over the Ice Age town's white snow ground the
	// glyphs get a black outline. Pass 0 walks the glyph exactly like the drawing pass and puts Panel.cpp's
	// DrawLetterOutline (the small font's outline: black on the 4 neighbours of each pixel) round every pixel it
	// would draw; pass 1 then draws the letter over it, as before. The outline may reach 1 px past the scroll clip.
	const bool outline = SnowGround_Active();
	uchar* const firstPixel = curPixelPtr;
	for( int pass = outline ? 0 : 1; pass < 2; ++pass ){
		curPixelPtr = firstPixel;
		linePixelsLeft = Speech_LetterMaxWidth;
		for( int row = 0; row < length; ){
			char curCode = pictureData[row++];
			if( curCode >= 0 && ( curPixelPtr < speechTop || curPixelPtr > speechBottom ) ){ // (rt) forbidden height 
				row += curCode;
				curCode = -curCode;// (rt) skip all bytes at once 
			}
			if( curCode < 0 ){ // (rt) Skip (-curCode) bytes 
				curPixelPtr -= curCode;
				linePixelsLeft += curCode;
				if( linePixelsLeft <= 0 ){
					curPixelPtr -= YOffsetHashTable[1] + Speech_LetterMaxWidth; // (rt) move up one line 
					linePixelsLeft = Speech_LetterMaxWidth;
				}
			}else if( !pass ){ // (th4) the outline of curCode pixels
				for( int j = 0; j < curCode; j++ ){
					DrawLetterOutline( curPixelPtr++, 0, 0 );
					row++;
					if( --linePixelsLeft <= 0 ){
						curPixelPtr -= YOffsetHashTable[1] + Speech_LetterMaxWidth; // (rt) move up one line 
						linePixelsLeft = Speech_LetterMaxWidth;
					}
				}
			}else{ // (rt) draw curCode pixels
				for( int j = 0; j < curCode; j++ ){
					*curPixelPtr++ = pictureData[row++];
					// (th4) A8L8: the pixel just written is at curPixelPtr-1, so its light byte is
					// curPixelPtr-1+lightOfs. Must happen before the line-wrap below moves the
					// pointer, or the two would desync.
					if( hasLight ) curPixelPtr[lightOfs - 1] = 0;
					// (th4) Stage 2 (2026-09-04): same for the alpha overlay — speech
					// glyphs must never alpha-blend with occluded sprites behind the
					// semi-transparent box (Rev 2.34).
					AlphaOverlayClearPixel(curPixelPtr - 1);
					// (th4) Stage 3 (2026-09-05): same for the colored-light tint —
					// speech glyphs must never be multiplied by the world's winning
					// light color, while the box dither holes keep the tint.
					TintClearPixel(curPixelPtr - 1);
					if( --linePixelsLeft <= 0 ){
						curPixelPtr -= YOffsetHashTable[1] + Speech_LetterMaxWidth; // (rt) move up one line 
						linePixelsLeft = Speech_LetterMaxWidth;
					}
				}
			}
		}
	}
}

// (rt) Drawing the smoothly rising NPC speech text 
//----- (0042E6A2) --------------------------------------------------------
void DrawSpeech()
{
	int leftX = Screen_LeftBorder + (ScreenWidth - Dialog_591_Width) / 2 + Speech_LeftMargin;
	int bottomY = Screen_TopBorder + Dialog_302_Height + (ScreenHeight - Dialog_302_Height - GUI_MainPanelHeight) / 2 + Speech_TopMargin; // (rt) bottom coordinate for drawing the speech
	char* curChar = Speech_FirstDrawingLine; // (rt) current letter being drawn = first letter of the top line being drawn 
	char* secondDrawingLine = Speech_FirstDrawingLine; // (rt) first letter of the second line being drawn 
	uchar curLine[ Speech_LineMaxLen ]; // (rt) buffer for the line being drawn, localized to the code page
	
	int dialogW = Dialog_591_Width;
	int dialogH = Dialog_302_Height;
	int xPos = (ScreenWidth - dialogW) / 2;
	int yPos = dialogH + ((ScreenHeight - dialogH - GUI_MainPanelHeight) / 2) + 8;
	DrawDialogBox( dialogW, dialogH, TextBoxCel, xPos, yPos );
	
	int charY = Speech_CurLineY; // (rt) top pixel of the line 
	for( char* nextChar = curChar; *nextChar != '|' && charY < bottomY; charY += Speech_LineHeight ){ // (rt) draw all text lines visible in the window down to the bottom 
		int curLineWidth = 0; // (rt) width of the output line in pixels 
		int lineLen = 0; // (rt) length of the line being drawn 
		for( nextChar = curChar; *nextChar != '|' && *nextChar != '\n' && curLineWidth < Dialog_591_Width - Speech_LeftMargin * 2 && lineLen < Speech_LineMaxLen; nextChar++ ){ // (rt) localize and fill the line up to the maximum width
			// (th4) Arabic: one stored byte pair is one glyph, and lam+alef is one glyph
			// for two pairs. Measure with the CELL width so the wrap point agrees with the
			// Arabic draw pass above; folding the pair byte-by-byte would wrap early and
			// leave the last letters off the line.
			if( GameLanguage == LANG_AR && (uchar)nextChar[0] >= AR_LEAD_FIRST && (uchar)nextChar[0] <= AR_LEAD_LAST
			 && (uchar)nextChar[1] >= AR_TRAIL_FIRST && (uchar)nextChar[1] <= AR_TRAIL_LAST ){
				if( lineLen + 1 >= Speech_LineMaxLen ) break;
				curLine[ lineLen++ ] = (uchar)nextChar[0];
				curLine[ lineLen++ ] = (uchar)nextChar[1];
				// (th4) Charge the widest form this base letter actually has, not the whole
				// cell. AR_CELL_W_MID is 22 while the real advances average 10.3, so the
				// old estimate called a line full at about half its true width and the
				// paragraph wrapped early, using only part of the panel. The per-letter
				// maximum is still an upper bound, so a line can never overflow.
				curLineWidth += AR_MaxAdvanceForPair( (uchar)nextChar[0], (uchar)nextChar[1], 1 );
				nextChar++;
				continue;
			}
			// (th4) CHI: GB2312 pair. Consumed ATOMICALLY so lineLen can never stop
			// between a lead and its trail. Both raw bytes go into curLine so it stays
			// 1:1 with source bytes -- the draw loop below advances curChar once per
			// curLine entry and that lockstep must hold. Needs room for BOTH bytes.
			if( IsChiLead( (uchar)nextChar[0] ) && IsChiTrail( (uchar)nextChar[1] ) ){
				if( lineLen + 1 >= Speech_LineMaxLen ) break;
				curLine[ lineLen++ ] = (uchar)nextChar[0];
				curLine[ lineLen++ ] = (uchar)nextChar[1];
				// (th4) Hindi advances per CLUSTER, not per cell -- see HindiText.cpp. The
				// measure and the draw must use the SAME expression or the wrap disagrees with
				// what is painted.
				curLineWidth += ( GameLanguage == LANG_HIN
				                ? HIN_Advance( ChiLetterID( (uchar)nextChar[0], (uchar)nextChar[1] ) - HIN_FIRST_ID, 1 )
				                : CHI_CELL_W_MID ) + Speech_Spacing;
				nextChar++; // loop's nextChar++ consumes the lead; this one the trail
				continue;
			}
			if( uchar localChar = ( is(GameLanguage, LANG_RU, LANG_BG, LANG_UK) ? (uchar)*nextChar : Codepage[ *nextChar ] ) ){ // (th4) RU: raw-byte (table raw-indexed); else Codepage // (th4) raw byte, table is raw-indexed like menu // zero characters are skipped 
				curLine[ lineLen++ ] = localChar; // (rt) fill the line and count its length 
				curLineWidth += FontWidthMidPtr[ FontIndexMidPtr[ localChar ] ] + Speech_Spacing; // (rt) count the line's width in pixels 
			}
		}
		if( *nextChar != '|' && *nextChar != '\n' ){ // (rt) if the text hasn't ended and the next character isn't a line break 
			// (th4) Back off to the last space so words don't split. Chinese contains NO
			// spaces, so the original loop would walk lineLen to 0, draw nothing, and never
			// advance curChar -- a blank speech panel. CJK breaks legally between any two
			// characters, so if there is no space on this line keep it as-is; the fill loop
			// already stopped on a character boundary. Latin is unaffected.
			// (th4) Back off to the last space so words don't split.
			// The original loop walked lineLen to 0 when the line held NO space, drew
			// nothing, and never advanced curChar. Chinese has no spaces at all, so that
			// was a blank speech panel; in Latin it blanks any word longer than the line.
			// Keeping the line as-is when there is no space fixes both: the fill loop
			// already stopped on a valid character boundary. Applies to ALL languages --
			// deliberately not CHI-gated, because the Latin behaviour was also wrong.
			{
				int wrapLen = lineLen;
				for( ; wrapLen > 0 && curLine[ wrapLen - 1 ] != ' '; wrapLen-- );
				if( wrapLen > 0 ) lineLen = wrapLen;
			}
		}
		curLine[ lineLen ] = 0; // (rt) terminate the output line with a zero 
		// (th4) ARABIC. The line is final at this point -- wrapped, and with any %i
		// already substituted upstream -- which is exactly when shaping and reordering
		// are allowed to happen. Doing either earlier would put a reversed run through
		// the wrapper and scramble both halves.
		//
		// curLine holds LOGICAL bytes; the source caret curChar must still advance one
		// step per SOURCE byte, so it is walked separately from the drawn glyphs. A
		// lam+alef pair collapses to a single glyph, so the two counts genuinely differ
		// and cannot be kept in the lockstep the CHI branch below relies on.
		if( GameLanguage == LANG_AR ){
			static AR_Glyph g[ Speech_LineMaxLen + 8 ];
			const int gn = AR_Shape( curLine, lineLen, g, Speech_LineMaxLen + 8 );
			AR_Reorder( g, gn );
			// (th4) RIGHT-ALIGN the line. Arabic paragraphs hang off the RIGHT margin with a
			// ragged left edge -- the mirror of Latin. Left-aligning leaves the right edge,
			// which is where the reader's eye starts every line, jagged. This is NOT UI
			// mirroring: nothing in the dialog moves, the panel keeps its geometry, and no
			// other language touches this branch. It is only where each line begins.
			// Measure the shaped line first -- the glyph advances, not the source bytes,
			// because a lam+alef pair is two bytes and one glyph.
			int arW = 0;
			for( int k = 0; k < gn; ++k ){
				if( g[k].isForm )     arW += AR_Advance( g[k].cell, 1 );
				else if( g[k].raw )   arW += FontWidthMidPtr[ FontIndexMidPtr[ g[k].raw ] ] + Speech_Spacing;
			}
			const int arAvail = Dialog_591_Width - Speech_LeftMargin * 2;
			int arX = leftX + ( arW < arAvail ? arAvail - arW : 0 );
			for( int k = 0; k < gn; ++k ){
				if( g[k].isForm ){
					DrawLetterInSpeech( arX, charY, FontSpriteMid, AR_FIRST_ID + g[k].cell );
					arX += AR_Advance( g[k].cell, 1 );
				}else if( g[k].raw ){
					const uchar ci = FontIndexMidPtr[ g[k].raw ];
					// (th4) whitespace ADVANCES but never paints. FontIndexMidPtr[' '] is not
					// zero and the Arabic medtexts frame it points at is not blank, so every
					// space in a speech drew a stray dot between the words.
					if( ci && g[k].raw > ' ' ) DrawLetterInSpeech( arX, charY, FontSpriteMid, ci );
					arX += FontWidthMidPtr[ ci ] + Speech_Spacing;
				}
			}
			for( int b = 0; b < lineLen; ++b ){
				if( *(++curChar) == '\n' ){ ++curChar; }
			}
			if( secondDrawingLine == Speech_FirstDrawingLine ){
				secondDrawingLine = curChar;
			}
			continue;
		}
		int charX = leftX; // (rt) left pixel of the line 
		for( uchar* localChar = curLine; *localChar != 0; ++localChar ){ // (rt) drawing the line 
			// (th4) CHI: GB2312 pair -> CJK frame. localChar and curChar each step twice:
			// curLine is 1:1 with source bytes, so a 2-byte character must retire two of
			// each or the source caret desyncs from the drawn text.
			if( IsChiLead( localChar[0] ) && IsChiTrail( localChar[1] ) ){
				DrawLetterInSpeech( charX, charY, FontSpriteMid, ChiLetterID( localChar[0], localChar[1] ) );
				charX += ( GameLanguage == LANG_HIN
				         ? HIN_Advance( ChiLetterID( localChar[0], localChar[1] ) - HIN_FIRST_ID, 1 )
				         : CHI_CELL_W_MID ) + Speech_Spacing;
				++localChar; // loop's ++localChar retires the lead; this one the trail
				if( *(++curChar) == '\n' ){ ++curChar; }
				if( *(++curChar) == '\n' ){ ++curChar; }
				continue;
			}
			uchar charImageNum = FontIndexMidPtr[ *localChar ]; // (rt) letter image number 
			if( charImageNum ){ // (rt) if not a whitespace character 
				DrawLetterInSpeech( charX, charY, FontSpriteMid, charImageNum ); // (rt) drawing the letter 
			}
			charX += FontWidthMidPtr[ charImageNum ] + Speech_Spacing; // (rt) move the pixel caret right (including whitespace characters) 
			if( *(++curChar) == '\n' ){ // (rt) move the source text caret 
				++curChar; // (rt) line breaks were skipped during localization 
			}
		}
		if( secondDrawingLine == Speech_FirstDrawingLine ){ // (rt) if the first line has been output 
			secondDrawingLine = curChar; // (rt) remember the start of the second one 
		}
	}
	if( IsMainDraw ){
		i64 curTick = GetTickCountTh(); // (rt) tick of the current frame  // this timer doesn't affect gameplay, not intercepted
		do{ // (rt) smooth scrolling 
			if( Speech_ScrollStep > 0 ){ // (rt) positive values for per-pixel scrolling within a step 
				if( Speech_ScrollCurStep-- > 1 ){ // (rt) until we have moved by all the pixels of the step 
					Speech_CurLineY--; // (rt) per-pixel scrolling 
				}else{
					Speech_ScrollCurStep = Speech_ScrollStep; // (rt) start the per-pixel step again 
				}
			}else{ // (rt) negative values for scrolling the whole step by several pixels per frame 
				Speech_CurLineY -= -Speech_ScrollStep + 1; // (rt) step-by-step scrolling 
			}
		}while( Speech_CurLineY > bottomY - Dialog_302_Height && curTick >= (Speech_PrevTick += MAX_FPS / ENGINE_FPS) ); // (rt) while not at the top edge of the text 
		if( curTick >= Speech_PrevTick ){ // (rt) Speech_LineScrollTime (50) milliseconds to scroll one line up ( with an overflow check ) 
			Speech_CurLineY += Speech_LineHeight; // (rt) switch to tracking the top of the next line 
			Speech_FirstDrawingLine = secondDrawingLine; // (rt) drop the top line that has scrolled past the top edge 
			if( *Speech_FirstDrawingLine == '|' ){ // (rt) if the text has ended 
				Speech_IsPanelVisible = 0; // (rt) close the speech window 
			}
		}
	}
}
