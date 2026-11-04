 #ifndef _vars_h_							
#define _vars_h_							
#else							
#error reinclude vars.h							
#endif
#include "CXBOXController.h"

extern uchar* FontIndexSmall;//	per-script pointer (Latin default); index is always Codepage[] output (unsigned)
extern uchar FontWidthSmall[115];//	85 for Cyrillic letterIDs 68..84; 94 for Polish letterIDs 85..93; 109 for Czech letterIDs 94..108; 111 for Croatian 109..110; 115 for Spanish 111..114 (th4)
// Codepage is wrapped so subscript always uses an unsigned byte index (call sites
// pass signed char; bytes >=0x80 would otherwise index out of bounds).
struct CodepagePtr {
	uchar* p;
	uchar& operator[](unsigned char i) const { return p[i]; }
	void operator=(uchar* q) { p = q; }
};
extern CodepagePtr Codepage;
extern uchar FontIndexSmall_Latin[256];
extern uchar FontIndexSmall_Cyrillic[256];
extern uchar FontIndexSmall_Portuguese[256];// (th4) PT
extern uchar FontIndexSmall_Spanish[256];// (th4) IT / SW fold table (named Spanish for history)
extern uchar FontIndexSmall_SpanishAccents[256];// (th4) ES real accents (dataseg.cpp "Spanish (LANG_ES) fonts, REAL ACCENTS")
extern uchar FontIndexMid_SpanishAccents[256];// (th4) ES
extern uchar FontWidthMid_SpanishAccents[89];// (th4) ES
extern uchar FontIndexBig_SpanishAccents[256];// (th4) ES
extern uchar FontWidthBig_SpanishAccents[89];// (th4) ES
extern uchar FontIndexSmall_Polish[256];// (th4) PL
extern uchar FontIndexSmall_Czech[256];// (th4) CS
extern uchar FontIndexSmall_Croatian[256];// (th4) HR
extern uchar Codepage_Latin[256];
extern uchar Codepage_Cyrillic[256];
extern uchar Codepage_Portuguese[256];// (th4) PT
extern uchar Codepage_Danish[256];	// (th4) DA: PT copy, aa/ae/oe folded to A/E/O
extern uchar Codepage_Turkish[256];// (th4) TR
extern uchar Codepage_Spanish[256];// (th4) ES
extern uchar Codepage_Polish[256];// (th4) PL
extern uchar Codepage_Czech[256];// (th4) CS: CP1250, non-Czech letters folded to ASCII
extern uchar Codepage_Croatian[256];// (th4) HR: CP1250, non-Croatian letters folded to ASCII
// ---- localization (th4) ----
// Language index. English (LANG_EN=0) is the guaranteed fallback and is never
// stored in any overlay table. To ADD A LANGUAGE: append an id BEFORE LANG_COUNT,
// give it a LanguageNames[] entry, register its overlay module in
// InitLanguageOverlays() (Localization.cpp), and (optionally) mount its audio
// pack in Windows.cpp. Nothing else in the engine needs to change: the monster
// name tables are dimensioned [LANG_COUNT] and the string overlays are looked up
// through the per-language registry, so both grow automatically.
enum { LANG_EN = 0, LANG_RU = 1, LANG_ID = 2, LANG_PT = 3, LANG_ES = 4, LANG_IT = 5, LANG_FR = 6, LANG_PL = 7, LANG_CHI = 8, LANG_DE = 9, LANG_TR = 10, LANG_KO = 11, LANG_DA = 12, LANG_BG = 13, LANG_NL = 14, LANG_UK = 15, LANG_JA = 16, LANG_AR = 17, LANG_HIN = 18, LANG_SW = 19, LANG_CS = 20, LANG_HR = 21, LANG_COUNT };
// ---- Chinese (GB2312) constants (th4) ----
// GB2312 is 2 bytes/char: lead 0xA1..0xF7, trail 0xA1..0xFE. Both ranges sit ABOVE the
// colour-control window 0x80..0x8B, so colour codes need no change -- the same free ride
// CP1251 gets. TH4loc_chi.mor (mounted only when language == CHI) keeps frames 0..255 as
// the original base font and appends GB2312 at frames 256..8433, so
//   letterID = CHI_FIRST_ID + (lead-CHI_LEAD_FIRST)*CHI_ROW_LEN + (trail-CHI_TRAIL_FIRST)
//
// The lead range starts at 0xA1, NOT 0xB0. Rows 0xB0..0xF7 are the 6763 hanzi; rows
// 0xA1..0xA9 are the SYMBOL rows, where GB2312 keeps the punctuation Chinese prose cannot
// do without (comma A3AC, full stop A1A3, question A3BF, brackets A1B6/A1B8). Covering
// only 0xB0..0xF7 left every one of those to fall through into Codepage_Latin and fold to
// a Latin letter. The formula and the font are a MATCHED PAIR: changing one without
// rebuilding the other shifts every glyph in the language.
//
// CELL WIDTHS: mid (22) and big (46) are the stock widths, so those two needed no blitter
// change. SMALL IS NOT: the small CJK cell is 17, not the stock 13. At 13 the hanzi were
// shipped (v0.471) but unreadable -- 13x13 is simply too few pixels to resolve a hanzi's
// strokes -- so the small CJK glyphs were rebuilt by downscaling the 22x22 medtexts.cel
// frames to 17x17. DrawLetter()/DrawLetterPixels() (Panel.cpp) therefore pick the RLE row
// stride per letterID: 17 for letterID >= CHI_FIRST_ID, SMALL_FONT_CELL_W (13) otherwise.
// The Latin frames 0..255 in the CHI smaltext.cel are still 13 wide and byte-identical to
// stock, which is why the stride has to be per-letter and not a file-wide constant.
enum { SMALL_FONT_CELL_W = 13 };   // stock Latin/Cyrillic/Polish small-font cell
enum { CHI_FIRST_ID = 257, CHI_CELL_W_SMALL = 17, CHI_CELL_W_MID = 22, CHI_CELL_W_BIG = 46 };
enum { CHI_LEAD_FIRST = 0xA1, CHI_LEAD_LAST = 0xF7, CHI_TRAIL_FIRST = 0xA1, CHI_TRAIL_LAST = 0xFE };
enum { CHI_ROW_LEN = CHI_TRAIL_LAST - CHI_TRAIL_FIRST + 1 };   // 94 chars per GB2312 row

// ---- Korean (EUC-KR / Wansung) constants (th4) ----
// Same shape as the Chinese block above: 2 bytes/char, both ranges above the colour-control
// window 0x80..0x8B, so colour codes need no change. EUC-KR lead 0xB0..0xC8, trail 0xA1..0xFE
// covers the 2350 precomposed Hangul syllables with NO holes -- 25 rows x 94 = exactly 2350,
// so unlike GB2312 there are no gaps to skip. TH4loc_kor.mor (mounted only when
// language == KO) keeps frames 0..255 as the original base font and appends Hangul from
// KOR_FIRST_ID, so
//   letterID = KOR_FIRST_ID + (lead-KOR_LEAD_FIRST)*KOR_ROW_LEN + (trail-KOR_TRAIL_FIRST)
//
// Rows 0xA1..0xAF (symbols/punctuation) are NOT covered: Korean prose uses ASCII punctuation,
// which the Latin frames 0..255 already carry. If Korean-specific punctuation is ever needed,
// widen KOR_LEAD_FIRST and rebuild the font -- the formula and the font are a MATCHED PAIR.
//
// CELL WIDTHS: same reasoning as CHI. Small is 17, not the stock 13 -- a 13x13 cell cannot
// resolve a Hangul syllable's jamo any better than a hanzi's strokes.
enum { KOR_FIRST_ID = 257, KOR_CELL_W_SMALL = 17, KOR_CELL_W_MID = 22, KOR_CELL_W_BIG = 46 };
enum { KOR_LEAD_FIRST = 0xB0, KOR_LEAD_LAST = 0xC8, KOR_TRAIL_FIRST = 0xA1, KOR_TRAIL_LAST = 0xFE };
enum { KOR_ROW_LEN = KOR_TRAIL_LAST - KOR_TRAIL_FIRST + 1 };   // 94 chars per EUC-KR row

// ---- Japanese (EUC-JP / JIS X 0208) constants (th4) ----
// EUC-JP, NOT Shift-JIS, and the reason is not style. Shift-JIS lead bytes run
// 0x81..0x9F, which drives straight through the colour-control window 0x80..0x8B:
// every string starting with a kana in that range would be eaten as a colour code.
// EUC-JP puts both bytes at 0xA1..0xFE, above the window -- the same free ride
// GB2312, EUC-KR and CP1251 already take, so colour codes need no change at all.
//
// Lead 0xA1..0xF4, trail 0xA1..0xFE. Rows 0xA1..0xA8 are symbols/kana/Greek/Cyrillic,
// row 0xA4 is hiragana and 0xA5 katakana; 0xB0..0xF4 are the 6355 JIS level 1+2 kanji.
// Unlike EUC-KR there ARE holes (undefined cells inside the rows) -- they simply get
// an empty frame, exactly as the unused GB2312 cells do.
//   letterID = JA_FIRST_ID + (lead-JA_LEAD_FIRST)*JA_ROW_LEN + (trail-JA_TRAIL_FIRST)
//
// CELL WIDTHS: same reasoning as CHI/KO. Small is 17, not the stock 13 -- kanji need
// the pixels. Mid 22 and big 46 are the stock widths.
//
// FONT SOURCE: TH4loc_jpn.mor. The glyph art the pack ships covers every Unicode page
// EUC-JP maps into EXCEPT pages 0x00/0x03/0x04/0x20..0x26 -- i.e. rows 0xA1, 0xA2, 0xA6
// (Greek), 0xA7 (Cyrillic) and 0xA8 (box drawing) are partly blank, 232 cells in all.
// Japanese punctuation proper is NOT affected: U+3001/3002 comma and full stop and
// U+300C/300D corner brackets all live in page 0x30 and are present.
enum { JA_FIRST_ID = 257, JA_CELL_W_SMALL = 17, JA_CELL_W_MID = 22, JA_CELL_W_BIG = 46 };
enum { JA_LEAD_FIRST = 0xA1, JA_LEAD_LAST = 0xF4, JA_TRAIL_FIRST = 0xA1, JA_TRAIL_LAST = 0xFE };
enum { JA_ROW_LEN = JA_TRAIL_LAST - JA_TRAIL_FIRST + 1 };   // 94 chars per EUC-JP row

// (th4) ARABIC. Rides the same double-byte path as CHI/KO/JA, but everything else
// about it is different, so read this before touching it.
//
// STORAGE IS LOGICAL, NOT VISUAL. Tables hold base letters in reading order; the
// shaper picks the contextual form and the reorder pass flips the run at DRAW time.
// That is the whole reason we do not pre-bake visual strings: %i values are
// substituted and paragraphs are wrapped at runtime, and both would land inside an
// already-reversed string.
//
// PRIVATE ENCODING, deliberately NOT UTF-8. Arabic in UTF-8 is 0xD8..0xDB followed
// by a trail in 0x80..0xBF, and trails for common letters land in 0x80..0x8B -- the
// colour-control window. That is the same collision that ruled out Shift-JIS for
// Japanese. Both bytes here are >= 0xA1 so no colour code can ever be forged.
//   lead 0xA1..0xA2, trail 0xA1..0xFE  ->  188 slots for ~50 base letters
//
// TWO SEPARATE NUMBERINGS, do not conflate them:
//   AR_ENC_*  the byte pair stored in a table  = BASE letter index
//   letterID  what reaches the font            = FORM index (isolated/initial/
//                                                medial/final, ligatures, digits)
// The byte stream is never drawn directly, so the two spaces may overlap freely.
//
// CELL WIDTHS match JA so the sheet machinery is unchanged, but the DRAW ADVANCE is
// proportional -- the .wid files carry each form's real ink width. Arabic letters
// are narrow and a fixed 22px step looks like spaced-out capitals.
enum { AR_FIRST_ID = 257, AR_CELL_W_SMALL = 17, AR_CELL_W_MID = 22, AR_CELL_W_BIG = 46 };
enum { AR_LEAD_FIRST = 0xA1, AR_LEAD_LAST = 0xA2, AR_TRAIL_FIRST = 0xA1, AR_TRAIL_LAST = 0xFE };
enum { AR_ROW_LEN = AR_TRAIL_LAST - AR_TRAIL_FIRST + 1 };   // 94 cells per row
enum { AR_FORM_COUNT = 157 };   // 143 presentation forms + 10 Arabic-Indic digits + 4 punctuation

// (th4) HINDI / DEVANAGARI. Rides the same double-byte path as CHI/KO/JA and needs
// NO shaper at runtime -- read this before touching it, because it is the opposite
// of the Arabic decision two blocks up.
//
// STORAGE IS VISUAL AND PRE-SHAPED. Devanagari needs three things Arabic did not:
// consonant clusters fuse into half-form + base, the short-i matra is stored after
// its consonant but drawn BEFORE it, and vowel signs stack above and below with a
// zero advance. Doing that at draw time would mean a cluster segmenter, a reorder
// pass and per-glyph x/y offsets in the glyph struct -- new draw logic at every one
// of the ~25 text sites. Instead the BUILD does it once, offline, with a real
// shaper, and stores each cluster as a single pre-composed cell in visual order.
// The engine then draws cell, advance, cell, advance, exactly as it does for
// Japanese, and ChiLetterID's plain formula is all that is needed.
//
// THE COST IS CELL COUNT, and it is why 35 letters becomes ~1200 cells. "ka" is one
// cell but ki, ku, kuu, ke, kai, ko, kau, kam, kaam... are each their own image,
// and conjuncts multiply that again. Measured: 205 distinct clusters in 120 words of
// game vocabulary, 1221 for every consonant x matra x nasal, 1774 including
// conjuncts. Twenty lead rows give 1880 slots, which covers the closed set with
// room to spare. Only clusters that actually occur in the text get generated.
//
// PRIVATE ENCODING, not UTF-8 -- same reason as Arabic and Japanese: UTF-8
// Devanagari is 0xE0 0xA4/0xA5 0x80..0xBF and those trails land in the colour-code
// window 0x80..0x8B. Both bytes here are >= 0xA1 so no colour code can be forged.
//   lead 0xA1..0xB4, trail 0xA1..0xFE  ->  20 x 94 = 1880 slots
//
// CELL WIDTHS match JA so the sheet machinery is unchanged. The draw advance is
// PROPORTIONAL like Arabic, not a fixed step: a bare consonant and a consonant
// carrying a left matra differ in width by roughly half a cell, and a fixed 22px
// step would space the text like capitals.
enum { HIN_FIRST_ID = 257, HIN_CELL_W_SMALL = 17, HIN_CELL_W_MID = 22, HIN_CELL_W_BIG = 46 };
enum { HIN_LEAD_FIRST = 0xA1, HIN_LEAD_LAST = 0xB4, HIN_TRAIL_FIRST = 0xA1, HIN_TRAIL_LAST = 0xFE };
enum { HIN_ROW_LEN = HIN_TRAIL_LAST - HIN_TRAIL_FIRST + 1 };   // 94 cells per row
enum { HIN_CELL_MAX = ( HIN_LEAD_LAST - HIN_LEAD_FIRST + 1 ) * HIN_ROW_LEN };  // 1880

extern int GameLanguage;
extern const char* LanguageNames[ LANG_COUNT ];
extern const char* MonsterNameLoc[ BM_COUNT ][ LANG_COUNT ];
extern const char* UniqueMonsterNameLoc[ UM_count ][ LANG_COUNT ];
extern uchar SomeChurchShadowsTable2[7*37];//	
extern uchar SomeChurchShadowsTable[208];//	
extern uchar ChurchTilesVariationsTable[208];//	
extern uchar PatMap4x4_ChurchPassageUp[34];//	
extern uchar PatMap4x5_CryptPassageUp[48];//	
extern uchar PatMap4x3_ChurchPassageDown[27];//	
extern uchar Pat2Map4x5_CryptPassageDown[48];//	
extern uchar Pat3Map4x5_CryptPassageToTown[48];//	
extern uchar PatMap2x2_ChurchOnFloorTorch[10];//	
extern uchar PatMap6x6_WaterSupplyEnter[76];//	
extern uchar CryptArcLeft[4];//	
extern uchar CryptArcRight[8];//	
extern uchar CryptGrateLeft[8];//	
extern uchar CryptGrateRight[8];//	
extern uchar Pat3Map1x1[4];//	
extern uchar Pat4Map1x1[4];//	
extern uchar Pat5Map1x1[4];//	
extern uchar Pat6Map1x1[4];//	
extern uchar CryptBigFloor[20];//	
extern uchar PatMap1x1a[4];//	
extern uchar PatMap1x1s[4];//	
extern uchar PatMap1x1d[4];//	
extern uchar PatMap1x1f[4];//	
extern uchar PatMap1x1g[4];//	
extern uchar PatMap1x1h[4];//	
extern uchar PatMap1x1j[4];//	
extern uchar PatMap1x1k[4];//	
extern uchar PatMap1x1l[4];//	
extern uchar PatMap1x1z[4];//	
extern uchar PatMap1x1x[4];//	
extern uchar PatMap1x1c[4];//	
extern uchar PatMap1x1v[4];//	
extern uchar PatMap1x1b[4];//	
extern uchar PatMap1x1n[4];//	
extern uchar PatMap1x1m[4];//	
extern uchar PatMap1x1q[4];//	
extern uchar PatMap1x1w[4];//	
extern uchar PatMap1x1e[4];//	
extern uchar PatMap1x1r[4];//	
extern uchar PatMap1x1t[4];//	
extern uchar PatMap1x1y[4];//	
extern uchar PatMap1x1u[4];//	
extern uchar PatMap1x1i[4];//	
extern uchar PatMap1x1o[4];//	
extern uchar PatMap1x1p[8];//	
extern uchar PatMap3x3q[24];//	
extern uchar PatMap3x3w[24];//	
extern uchar PatMap3x3e[24];//	
extern uchar PatMap3x3r[24];//	
extern uchar PatMap3x3t[24];//	
extern uchar PatMap3x3y[20];//	
extern uchar PatMap1x1p1[4];//	
extern uchar PatMap1x1p2[4];//	
extern uchar PatMap1x1p3[4];//	
extern uchar PatMap1x1p4[4];//	
extern uchar PatMap1x1p5[4];//	
extern uchar PatMap1x1p6[4];//	
extern uchar PatMap1x1p7[4];//	
extern uchar PatMap1x1p8[4];//	
extern uchar PatMap1x1p9[4];//	
extern uchar PatMap1x1q1[4];//	
extern uchar PatMap1x1q2[4];//	
extern uchar PatMap1x1q3[4];//	
extern uchar PatMap1x1q4[4];//	
extern uchar PatMap1x1q5[4];//	
extern uchar PatMap1x1q6[4];//	
extern uchar PatMap1x1q7[4];//	
extern uchar PatMap1x1q8[4];//	
extern uchar PatMap1x1q9[4];//	
extern uchar PatMap1x1w1[4];//	
extern uchar PatMap1x1w2[4];//	
extern uchar PatMap1x1w3[4];//	
extern uchar PatMap1x1w4[4];//	
extern uchar PatMap1x1w5[4];//	
extern uchar PatMap1x1w6[4];//	
extern uchar PatMap1x1_48A34C[4];//	
extern uchar PatMap1x1_48A350[4];//	
extern uchar PatMap1x1_48A354[4];//	
extern uchar PatMap1x1_48A358[4];//	
extern uchar PatMap1x1_48A35C[4];//	
extern uchar PatMap1x1_48A360[4];//	
extern uchar PatMap1x1_48A364[4];//	
extern uchar PatMap1x1_48A368[4];//	
extern uchar PatMap1x1_48A36C[4];//	
extern uchar PatMap1x1_48A370[4];//	
extern uchar PatMap1x1_48A374[4];//	
extern uchar PatMap1x1_48A378[4];//	
extern uchar PatMap1x1_48A37C[4];//	
extern uchar PatMap1x1_48A380[4];//	
extern uchar PatMap1x1_48A384[4];//	
extern uchar PatMap1x1_48A388[4];//	
extern uchar PatMap1x1_48A38C[4];//	
extern uchar PatMap1x1_48A390[4];//	
extern uchar PatMap1x1_48A394[4];//	
extern uchar PatMap1x1_48A398[4];//	
extern uchar PatMap1x1_48A39C[4];//	
extern uchar PatMap1x1_48A3A0[4];//	
extern uchar PatMap1x1_48A3A4[4];//	
extern uchar PatMap1x1_48A3A8[4];//	
extern uchar PatMap1x1_48A3AC[4];//	
extern uchar PatMap1x1_48A3B0[4];//	
extern uchar PatMap1x1_48A3B4[4];//	
extern uchar PatMap1x1_48A3B8[4];//	
extern uchar PatMap1x1_48A3BC[4];//	
extern uchar PatMap1x1_48A3C0[4];//	
extern uchar PatMap1x1_48A3C4[4];//	
extern uchar PatMap1x1_48A3C8[4];//	
extern uchar PatMap1x1_48A3CC[4];//	
extern uchar PatMap1x1_48A3D0[4];//	
extern uchar PatMap1x1_48A3D4[4];//	
extern uchar CaveOrientedWallsAndCorner[16];//	(rt) block start
extern uchar PatMap3x3_48A3E8_CaveDungeonEnterToUp[20];//	
extern uchar PatMap3x3_48A400_AbyssDungeonEnterToUp[20];//	
extern uchar PatMap3x3_48A418_CaveDungeonEnterToDown[20];//	
extern uchar PatMap3x3_48A430_AbyssDungeonEnterToDown[18];//	
extern uchar PatMap3x3_48A448_CaveDungeonEnterToTown[20];//	
extern uchar PatMap3x3_48A460_AbyssDungeonEnterToTown[20];//	
extern uchar PatMap4x4_48A478_STALAGMIT_1[35];//	
extern uchar PatMap4x4_48A4A0_STALAGNAT_1[35];//	
extern uchar PatMap4x4_48A4C8_STALAGNAT_2[35];//	
extern uchar PatMap5x4_48A4F0_STALAGNAT_3[42];//	
extern uchar PatMap4x5_48A520_STALAGNAT_4[42];//	
extern uchar PatMap3x3_48A550_STALAGMIT_2[20];//	
extern uchar PatMap3x3_48A568_STALAGMIT_3[20];//	
extern uchar PatMap3x3_48A580_STALAGMIT_4[20];//	
extern uchar PatMap3x3_48A598_STALAGMIT_5[20];//	
extern uchar PatMap2x1_48A5B0_STALAGNAT_ON_WALL_1[6];//	
extern uchar PatMap1x2_48A5B8_STALAGNAT_ON_WALL_2[6];//	
extern uchar PatMap2x1_48A5C0_CRACK_IN_WALL_1[6];//	
extern uchar PatMap2x1_48A5C8_CRACK_IN_WALL_2[6];//	
extern uchar PatMap1x2_48A5D0_CRACK_IN_WALL_3[6];//	
extern uchar PatMap2x1_48A5D8_CRACK_IN_WALL_4[6];//	
extern uchar PatMap1x2_48A5E0_CRACK_IN_WALL_5[6];//	
extern uchar PatMap1x2_48A5E8_CRACK_IN_WALL_6[6];//	
extern uchar PatMap2x1_48A5F0_CRACK_IN_WALL_7[6];//	
extern uchar PatMap1x2_48A5F8_CRACK_IN_WALL_8[6];//	
extern uchar PatMap2x1_48A600_CRACK_IN_WALL_9[6];//	
extern uchar PatMap2x1_48A608_CRACK_IN_WALL_10[6];//	
extern uchar PatMap1x2_48A610_CRACK_IN_WALL_11[6];//	
extern uchar PatMap2x3_48A618[14];//	
extern uchar PatMap3x2_48A628[14];//	
extern uchar PatMap2x3_48A638[14];//	
extern uchar PatMap3x2_48A648[14];//	
extern uchar PatMap2x2_48A658[10];//	
extern uchar PatMap1x1_48A664_FLOOR_VARIATION_1[4];//	
extern uchar PatMap1x1_48A668_FLOOR_VARIATION_2[4];//	
extern uchar PatMap1x1_48A66C_FLOOR_VARIATION_3[4];//	
extern uchar PatMap1x1_48A670_SWALL_UNM_MIDDLE_VARIATION[4];//	
extern uchar PatMap1x1_48A674_SWALL_M_MIDLE_VARIATION[4];//	
extern uchar PatMap11x11_48A678_AnvilOfFuryPattern[244];//	
extern uchar PatMap1x1_48A76C_ABYSS[4];//	
extern uchar PatMap1x1_48A770_ABYSS[4];//	
extern uchar PatMap1x1_48A774_ABYSS[4];//	
extern uchar PatMap1x1_48A778_ABYSS[4];//	
extern uchar PatMap1x1_48A77C_ABYSS[4];//	
extern uchar PatMap1x1_48A780_ABYSS[4];//	
extern uchar PatMap1x1_48A784_ABYSS[4];//	
extern uchar PatMap1x1_48A788_ABYSS[4];//	
extern uchar PatMap3x3_48A790_ABYSS[20];//	
extern uchar PatMap3x3_48A7A8_ABYSS[20];//	
extern uchar PatMap1x1_48A7BC_ABYSS[4];//	
extern uchar PatMap1x1_48A7C0_ABYSS[4];//	
extern uchar PatMap1x1_48A7C4_ABYSS[4];//	
extern uchar PatMap1x1_48A7C8_ABYSS[4];//	
extern uchar PatMap1x1_48A7CC_ABYSS[4];//	
extern uchar PatMap1x1_48A7D0_ABYSS[4];//	
extern uchar PatMap1x1_48A7D4_ABYSS[4];//	
extern uchar PatMap1x1_48A7D8_ABYSS[4];//	
extern uchar PatMap1x1_48A7DC_ABYSS[4];//	
extern uchar PatMap1x1_48A7E0_ABYSS[4];//	
extern uchar PatMap1x1_48A7E4_ABYSS[4];//	
extern uchar PatMap1x1_48A7E8_ABYSS[4];//	
extern uchar PatMap1x1_48A7EC_ABYSS[4];//	
extern uchar PatMap1x1_48A7F0_ABYSS[4];//	
extern uchar PatMap1x1_48A7F4_ABYSS[4];//	
extern uchar PatMap1x1_48A7F8_ABYSS[4];//	
extern uchar PatMap1x1_48A7FC_ABYSS[4];//	
extern uchar PatMap1x1_48A800_ABYSS[4];//	
extern uchar PatMap3x3_48A808_ABYSS[20];//	
extern uchar PatMap3x3_48A820_ABYSS[20];//	
extern uchar PatMap3x3_48A838_ABYSS[20];//	
extern uchar PatMap3x3_48A850_ABYSS[20];//	
extern uchar PatMap3x3_48A868_ABYSS[20];//	
extern uchar PatMap3x3_48A880_ABYSS[20];//	
extern uchar PatMap3x3_48A898_ABYSS[20];//	
extern uchar PatMap3x3_48A8B0_ABYSS[20];//	
extern uchar PatMap3x3_48A8C8_ABYSS[20];//	
extern uchar PatMap3x3_48A8E0_ABYSS[20];//	
extern uchar PatMap2x3_48A8F8_ABYSS[14];//	
extern uchar PatMap3x2_48A908_ABYSS[14];//	
extern uchar PatMap2x3_48A918_ABYSS[14];//	
extern uchar PatMap3x2_48A928_ABYSS[14];//	
extern uchar PatMap2x2_48A938_ABYSS[10];//	
extern uchar PatMap4x4_48A948_ABYSS[34];//	
extern uchar PatMap4x4_48A970_ABYSS[34];//	
extern uchar PatMap4x5_48A998_ABYSS[42];//	
extern uchar PatMap4x5_48A9C8_ABYSS[42];//	
extern uchar CaveLavaFlags[16];//	
extern uchar CaveLavaFlags2[16];//	
extern uchar CaveLavaMakingArray[32];//	
extern uchar HellOrientedWallsAndCorner[16];//	(rt) block start
extern uchar PatMap4x5_48AA48_HellDungeonEnterToUp[42];//	
extern uchar PatMap4x5_48AA78_HellDungeonEnterToTown[42];//	
extern uchar PatMap5x5_48AAA8_HellDungeonEnterToDown[52];//	
extern uchar PatMap5x5_48AAE0_PandemoniumEnterDesactivated[52];//	
extern uchar PatMap5x5_48AB18_PandemoniumEnterActivated[52];//	
extern uchar HellTilesVariationsTable[140];//	
extern unsigned char FontIndexBig[128];//	
extern unsigned char FontWidthBig[56];//	
extern uchar FontIndexBig_Cyrillic[256];// (th4)
extern uchar FontWidthBig_Cyrillic[56];// (th4)
extern uchar FontIndexBig_Portuguese[256];// (th4) PT
extern uchar FontWidthBig_Portuguese[89];// (th4) PT
extern uchar FontIndexBig_Spanish[256];// (th4) ES (folds accents -> base letters)
extern uchar FontIndexBig_Polish[256];// (th4) PL
extern uchar FontWidthBig_Polish[89];// (th4) PL
extern uchar FontIndexBig_Czech[256];// (th4) CS
extern uchar FontWidthBig_Czech[89];// (th4) CS
extern uchar FontIndexBig_Croatian[256];// (th4) HR
extern uchar FontWidthBig_Croatian[89];// (th4) HR
extern uchar* FontIndexBigPtr;// (th4)
extern uchar* FontWidthBigPtr;// (th4)
extern char HelpText[2268];//	
extern uchar ProgressBarColor[3];//	
extern Affix Affixes[4000];//	187
extern Affix AffixesColiseum[4000];//	187
extern Affix AffixesClassic[2000];//	187
extern Affix AffixesSigil[2000];
extern uchar FontIndexMid[128];//	
extern uchar FontWidthMid[56];//	
extern uchar FontIndexMid_Cyrillic[256];// (th4)
extern uchar FontWidthMid_Cyrillic[56];// (th4)
extern uchar FontIndexMid_Portuguese[256];// (th4) PT
extern uchar FontWidthMid_Portuguese[89];// (th4) PT
extern uchar FontIndexMid_Spanish[256];// (th4) ES
extern uchar FontIndexMid_Polish[256];// (th4) PL
extern uchar FontWidthMid_Polish[89];// (th4) PL
extern uchar FontIndexMid_Czech[256];// (th4) CS
extern uchar FontWidthMid_Czech[89];// (th4) CS
extern uchar FontIndexMid_Croatian[256];// (th4) HR
extern uchar FontWidthMid_Croatian[89];// (th4) HR
extern uchar* FontIndexMidPtr;// (th4) active
extern uchar* FontWidthMidPtr;// (th4) active
extern int CircleOffsets[20];//	
extern char StepOrientation[9];//	
extern char MonsterMissiles[4];//	
extern int ConstDword_49012C;//
extern int YArrayForRND[8];//	
extern int XArrayForRND[8];//	
extern char* ShrineNamesPtr[34];//	
extern char MinShrineDungeonLevels[34];//	
extern char MaxShrineDungeonLevels[34];//	
extern char ShrineAppearsMode[34];//	
extern char* old_BookNamePtrTable[16];//	(rt) old address
extern int QuestBooksSpeeches[9];//	
extern double _0_95_TimeCoef;//	
extern double _0_01_;//	
extern double GammaKoef;//	
extern char ArmorAnim[3];//	
extern const WeaponAnim WeaponAnims[PAW_COUNT];//	
extern char ClassAnim[6];//	
extern char ClassAnimBase[6];//	
extern char* ClassAnimName[6];//	
extern char* ClassAnimNameBase[6];//	
extern char ConstByteArrayForMap152x8[8];//	
extern Speech old_Speeches[338];//	(rt) old address
extern int CountOfSpeeches;//	
extern int CowSounds[ 3 ][ 6 ];//	
extern int SummonerSpellBooksIcons[3][3];
extern int SummonerSpellIconIndexes[3][3];
extern const char* SummonerSpellsNames[3][3];
extern array<Button, B_COUNT> Buttons;// Main panel buttons
extern array<Button, B_COUNT> ModButtons;
extern StatUpButton StatUpButtons[4];//	
extern int UseReservedSystemPalette;//	
extern int PaletteFadeLevel;   // (th4) 0..256, what SetFadeLevel last applied; see palette.cpp
extern bool PaletteFading;     // (th4) true only inside a PaletteFadeIn/Out loop; see palette.cpp
extern bool IsPlayerFriendly;
extern char* NetMsgTable[4];//	
extern char* KeyNameTable[4];//	
extern char aIntro[6];//	
extern uchar UberRoomPattern[26];//	(rt) block start
extern uchar CornerstoneRoomPattern[27];//	
extern uchar ChurchOrientedWallsAndCorner[16];//	
extern int MinCataHallSize;//	(rt) block start
extern int BigCataHallSize;//	
extern int MediumCataHallSize;//	
extern int SomeRowArray[5];//	
extern int SomeColArray[5];//	
extern StrangeCataPattern StrangeCataPatterns[2];//	(rt) block start
extern char CataTilesVariationsTable[161];//	
extern char SomeCataTilesTypes[161];//	
extern uchar PatMap2x4[18];//	
extern uchar PatMap2x4_0[18];//	
extern uchar PatMap2x4_1[18];//	
extern uchar PatMap2x4_2[18];//	
extern uchar PatMap2x4_3[18];//	
extern uchar PatMap2x4_4[18];//	
extern uchar PatMap2x4_5[18];//	
extern uchar PatMap2x4_6[18];//	
extern uchar PatMap2x4_7[18];//	
extern uchar PatMap2x4_8[18];//	
extern uchar PatMap2x4_9[18];//	
extern uchar PatMap2x4_10[18];//	
extern uchar PatMap2x4_11[18];//	
extern uchar PatMap2x4_12[18];//	
extern uchar PatMap2x4_13[18];//	
extern uchar PatMap2x4_14[18];//	
extern uchar PatMap2x3[14];//	
extern uchar PatMap2x3_0[14];//	
extern uchar PatMap2x3_1[14];//	
extern uchar PatMap2x3_2[14];//	
extern uchar PatMap2x3_3[14];//	
extern uchar PatMap2x3_4[14];//	
extern uchar PatMap2x3_5[14];//	
extern uchar PatMap2x3_6[14];//	
extern uchar PatMap2x4_15[18];//	
extern uchar PatMap2x4_16[18];//	
extern uchar PatMap2x4_17[18];//	
extern uchar PatMap2x4_18[18];//	
extern uchar PatMap2x4_19[18];//	
extern uchar PatMap2x4_20[18];//	
extern uchar PatMap2x4_21[18];//	
extern uchar PatMap2x4_22[18];//	
extern uchar PatMap2x4_23[18];//	
extern uchar PatMap2x4_24[18];//	
extern uchar PatMap2x4_25[18];//	
extern uchar PatMap2x4_26[18];//	
extern uchar PatMap2x4_27[18];//	
extern uchar PatMap2x4_28[18];//	
extern uchar PatMap2x4_29[18];//	
extern uchar PatMap2x4_30[18];//	
extern uchar PatMap3x2[14];//	
extern uchar PatMap3x2_0[14];//	
extern uchar PatMap3x2_1[14];//	
extern uchar PatMap3x2_2[14];//	
extern uchar PatMap3x2_3[14];//	
extern uchar PatMap3x2_4[14];//	
extern uchar PatMap3x2_5[14];//	
extern uchar PatMap3x2_6[14];//	
extern uchar PatMap3x2_7[14];//	
extern uchar PatMap3x2_8[14];//	
extern uchar PatMap3x2_9[14];//	
extern uchar PatMap3x2_10[14];//	
extern uchar PatMap3x2_11[14];//	
extern uchar PatMap3x2_12[14];//	
extern uchar PatMap3x2_13[14];//	
extern uchar PatMap3x2_14[14];//	
extern uchar PatMap3x2_15[14];//	
extern uchar PatMap3x2_16[14];//	
extern uchar PatMap3x2_17[14];//	
extern uchar PatMap3x2_18[14];//	
extern uchar PatMap3x2_19[14];//	
extern uchar PatMap3x2_20[14];//	
extern uchar PatMap3x2_21[14];//	
extern uchar PatMap3x2_22[14];//	
extern uchar PatMap3x2_23[14];//	
extern uchar PatMap3x2_24[14];//	
extern uchar PatMap3x2_25[14];//	
extern uchar PatMap3x2_26[14];//	
extern uchar PatMap3x2_27[14];//	
extern uchar PatMap3x2_28[14];//	
extern uchar PatMap3x2_29[14];//	
extern uchar PatMap3x2_30[14];//	
extern uchar PatMap3x2_31[14];//	
extern uchar PatMap3x2_32[14];//	
extern uchar PatMap3x2_33[14];//	
extern uchar PatMap3x2_34[14];//	
extern uchar PatMap3x2_35[14];//	
extern uchar PatMap3x2_36[14];//	
extern uchar PatMap3x2_37[14];//	
extern uchar PatMap3x2_38[14];//	
extern uchar CataDungeonEnterToUp[34];//	
extern uchar CataDungeonEnterToDown[34];//	
extern uchar CataDungeonEnterToTown[34];//	
extern uchar PatMap3x3_0[20];//	
extern uchar PatMap2x2_0[10];//	
extern uchar PatMap2x2_1[10];//	
extern uchar PatMap1x2[6];//	
extern uchar PatMap2x1[6];//	
extern uchar PatMap2x2_2[10];//	
extern uchar PatMap1x2_0[6];//	
extern uchar PatMap2x1_0[6];//	
extern uchar PatMap2x2_3[10];//	
extern uchar PatMap2x2_4[10];//	
extern uchar PatMap2x2_5[10];//	
extern uchar PatMap1x1_0[4];//	
extern uchar PatMap1x1_1[4];//	
extern uchar PatMap1x1_2[4];//	
extern uchar PatMap1x1_3[4];//	
extern uchar PatMap1x1_4[4];//	
extern uchar PatMap1x1_5[4];//	
extern uchar PatMap1x1_6[4];//	
extern uchar PatMap5x3[32];//	
extern uchar PatMap5x3_0[32];//	
extern uchar PatMap3x3_1[20];//	
extern uchar PatMap3x3_2[20];//	
extern uchar PatMap3x3_3[20];//	
extern uchar PatMap3x3_4[20];//	
extern uchar PatMap3x3_5[20];//	
extern uchar PatMap3x3_6[20];//	
extern uchar PatMap3x3_7[20];//	
extern uchar PatMap3x3_8[20];//	
extern int SomeCataPatternMaps10x100[715];//	
extern HANDLE DeltaThreadHandle;//	
extern int IsFastWalkEnabled;//	
extern Menu SPMainMenu[6];//
extern Menu MPMainMenu[5];//
extern Menu OptionsMenu[8];//
extern Menu MurderConfirmationMenu[5];
extern Menu ConfigPage1[6];
extern Menu VideoOptions1[6];
extern Menu VideoOptions2[6];
extern Menu GameplayOptions1[6];
extern Menu GameplayOptions2[6];
extern Menu VisualOptions1[6];
extern Menu VisualOptions2[6];
extern Menu VisualOptions3[6];
extern Menu CombatTextOptions1[6];
extern Menu CombatTextOptions2[6];
extern Menu CombatTextOptions3[6];
extern Menu SavedInfoMenu[6];
extern char* MusicMenu[2];//	
extern char* AmbientMenu[2];//	
extern char* SoundMenu[2];//	
extern char* WalkMenu[2];//	
extern char GameVersionString[MAX_PATH];//	
extern char TheHellVersion[MAX_PATH];//	
extern char InstallKey[14];//	
extern char GameCDKey[9];//	
extern int Put2x2ItemToInventoryCellsOrder[10];//	
extern int MaxGoldInCell;//	
extern int OilsQuality[10];//	
extern int OilPrice[10];//	
extern int OilsMagicCode[10];//	
extern char OilsName[10][25];//	
extern char* Flip_FileName[FL_81_COUNT];//	
extern char Flip_FramesCount[FL_81_COUNT];//	
extern int Flip_DropSound[FL_81_COUNT];//	
extern int Flip_InventorySound[FL_81_COUNT];//	
extern char* SitemPtr;//	
extern int ScanedMapCol;//	
extern int GriswoldQlvlRandomizationArray[15];//	
extern char aSitem[6];//	
extern char CircleAndWallSegmentDeltas[2752];//	
extern uchar NovaCircleDeltas[ 23 ][ 30 ];//	
extern uchar RadiusAdj[24];//	
extern int MenuMusicIndex;//	
extern int Speech_ScrollSteps[11];//	
extern BaseMissile BaseMissiles[MI_count];
extern MissileSprite MissileSprites[MIS_count];
extern int RowDelta[9];//	
extern int ColDelta[9];//	
extern int RowDeltaFB[9];
extern int ColDeltaFB[9];
extern MonsterFrameSpeed MonsterFramesSpeed[24];//	
extern char TypeOfAnimation[6];//	
extern int OrientMinusOne[8];//	
extern int OrientPlusOne[8];//	
extern int OrientInverted[8];//	
extern Ai AiFuncs[AI_count];//	
extern char aGendataDiabvic[21];//	
extern HANDLE CurrentSaveArchiveHandle;//	
extern char GameClientName[16];//	
extern char aLocal[6];//	
extern HANDLE NetFrameSyncThreadHandle;//	
extern BaseObject old_BaseObjects[99];//	(rt) old address
extern char* old_FloorObjectsNamesOriginal[38];//	(rt) old address
extern char* old_FloorObjectsNamesCrypt[38];//	(rt) old address
extern char* old_FloorObjectsNamesAbyss[56];//	(rt) old address
extern int ScreenGamma;
extern int ScreenContrast;
extern char PaletteLightening;//	
extern char StepOrientationIndex[9];//	
extern char CryptKey[9];//	
extern int PlrXoff[9];//	
extern int PlrYoff[9];//	
extern int PlrXoff2[9];//	
extern int PlrYoff2[9];//	
extern ClassProperty ClassPropTable[6];//	
extern ClassVelocity ClassVel[6];//
extern const char* ClassGroupsNames[PCG_COUNT];//
//(rt) extern PlayerStat MaxStat[6];//	block start
extern int TownStartingRow[4];//	(rt) block start
extern int TownStartingCol[4];//	
extern BaseQuest BaseQuests[Quest_Count];//
extern char MapDeltaRangeRow[8];//	
extern char MapDeltaRangeCol[8];//	
extern int old_QuestGroup2[4];//	(rt) old address
extern int QuestGroup3[3];//	
extern int old_QuestGroup4[4];//	(rt) old address
extern int old_QuestGroup5[2];//	(rt) old address
extern int* dungeonTilesOffset;//	
extern uint RightTransMask[32];//	
extern uint LeftTransMask[32];//	
extern uint RightTransMask_1[32];//	
extern int DwordArray_2[48];//	
extern int DwordArray_3[17];//	
extern int DwordArray_4[17];//	
extern MapArea MapAreas[2];//	
extern MapArea MapAreas6[2];//	
extern MapArea MapAreas2[5];//	
extern MapArea MapAreas4[7];//	
extern MapArea MapAreas7[5];//	
extern MapArea MapAreas3[2];//	
extern MapArea MapAreas5[9];//	
extern char IsMusicEnabled;//	
extern char IsAmbientEnabled;//	
extern char IsSoundEnable;//	
extern bool DuplicateSounds;
extern bool LimitCpuCore;
extern bool ClassicInMainMenu;
extern bool OutbreakInMainMenu;
extern bool ColiseumInMainMenu;
extern bool ClassicSaveFolder;
extern bool OutbreakSaveFolder;
extern bool DarkCampaignSaveFolder;
extern bool ColiseumSaveFolder;
extern bool SelectGameModeLater;
extern char* MusicName;
extern char* AmbientName;
extern Spell Spells[PS_COUNT];//
extern int StringRowYPosition[40];//	(rt) block start
extern char* TownersNames[9];//	
extern ROOM_TYPE DefaultRoomTypesTable[4];//	
extern int Offsets5x5_Rows[5][5];//	
extern int Offsets5x5_Cols[5][5];//	
extern int Offsets3x3_Rows[3][3];//	
extern int Offsets3x3_Cols[3][3];//	
extern char TownerAnimationFramesOrder[ 6 ][ 148 ];//	
extern int CowsRows[3];//	(rt) block start
extern int CowsCols[3];//	
extern int CowsOrients[3];//	
extern int CowsDeltaRow[8];//	
extern int CowsDeltaCol[8];//	
extern int TownersSpeechAboutQuestArray[TI_15_COUNT][Quest_Count];//
extern int CowClickPlayerSound;//	
extern int EntrisMapXxN[239];//	
extern char ErrorTextBuffer[256];//	(rt) block start
extern int IsErrorExitThreadPresent;//	
extern DWORD ErrorExitThread;//	
extern char ZeroString[8];//	
extern short LevelAutomapData[512];//	
extern int MapRowPos;//	
extern int MapColPos;//	
extern int AutomapMode;//	
extern char AutomapZoomOffset[42];//	
extern char AutoMap[ Map_40 ][ Map_40 ];//	
extern int AutomapZoomFactor;//	
extern int AutomapRow;//	
extern int AutomapCol;//	
extern int AutomapZoom32;//	
extern int AutomapZoom16;//	
extern int AutomapZoom8;//	(rt) block start
extern int AutomapZoom4;//	
extern int AutomapZoom2;//	
extern char TalkPanelIndex;//	
extern uchar CurTalkPanelIndex;//	
extern char* Items_DurIconsCEL;//	
extern char* Data_CharButCEL;//	
extern bool IsGoldSplitPanelVisible;//	
extern int ButtonArray[10];//	
extern int AttributeUpAvail[4];//	
extern char* CtrlPan_P8But2CEL;//	
extern char* CtrlPan_LogButtonCEL;
extern char* CtrlPan_Panel8buCEL;//	
extern char* CtrlPan_NewButtonsCEL;// new buttons
extern char* CtrlPan_TimeButtonCEL;
extern char* CtrlPan_MainMenuCEL;
extern char* Data_CharCEL;//	
extern bool IsCursorOnLevelUpButton;//	
extern bool IsCursorOnPerkUpButton;
extern char TalkPanelMessages[8][80];//	
extern int HowMuchGoldYouWantToRemove;//	
extern int ButtonStatDown;//	
extern char TalkPanelMessage[80];//	
extern char* FontSpriteSmall;//	
extern int PentSpnCurrentPicNumber;//	
extern uchar* LifeShereImage;//	
extern uchar* MainPanelImage/*[144][640]*/;//	
extern char* CtrlPan_TalkButtCEL;//	
extern int InfoPanelLinesCentered[16];//
extern int InfoPanelLinesUsed;//	
extern int InfoPanelUsed;//	
extern int TalkPanelButtonState[3];//	
extern char SelectedSpellIndex;//	
extern uchar* ManaShereImage;//	
extern char CurFontColor;//	(rt) block start
extern int MainPanelSelector;//	
extern char* CtrlPan_GolddropCEL;//	
extern char* Data_SpellBkBCEL;//	
extern char InfoPanelBuffer[256];//	(rt) block start
// (th4) Scrolled menu text. DrawFontString lays the string out as before but skips
// the first g_MenuTextSkipLines wrapped lines, and reports the total it laid out in
// g_MenuTextLineCount. Every caller except the scrolled description leaves skip at 0
// and gets exactly the old rendering.
extern int g_MenuTextSkipLines;
extern int g_MenuTextLineCount;
extern char MayPlayersTalk[4];//	
extern int CurrentSpellBookPage;//	
extern char SelectedSpellType;//	
extern int PentSpn2PicIndex;//	
extern int CurCellIndex;//	
extern int TalkPanelMode;//	
extern char* Data_SpellI2CEL;//	
extern char* Data_SpellI2CEL_2;//	
extern bool IsSpellBookVisible;//	
extern bool IsCHARPanelVisible;//	
extern char* Data_SpellBkCEL;//	
extern char InfoPanelHeader[MAX_PATH];//	
extern char InfoPanel4Lines[16][64];//
extern int MaybeNeedDrawInfoPanel;//	
extern uchar IconColors[256];//	
extern uchar IconColorsDisabled[256];//	
extern uchar IconColorsSpell[256];// 
extern uchar IconColorsRelict[256];// 
extern uchar IconColorsSkill[256];// 
extern uchar IconColorsCharges[256];// 
extern uchar IconColorsTrait[256];// 
extern uchar MonsterBarIconColors[6][256]; // DrawMonsterStatusBar
extern uchar MonsterBarBorderColors[3][256]; // DrawMonsterStatusBar
extern int GoldYouHave;//	
extern char* Data_SpelIconCEL;//	
extern char* Data_SpelIconCEL_2;//	
extern char* Data_HotkeyIconsCEL;
extern char* Monster_Bar_Border;
extern char* Monster_Bar_MonIcons;
extern char* Monster_Bar_ImIcons;
extern int MainPanelButtonFlag;//	
extern int SelectCurSpellMode;//	
extern int ItemWidth;//	
extern int ItemHeight;//	
extern int ItemWidthCells;//	
extern int ItemHeightCells;//	
extern char* TH2objcurs_Item_Sprites;	
extern char* TH3objcurs_Item_Sprites;	
extern int CurMon;//	
extern char* Invobj1_item_sprites;//	
extern CObjectUnderCursor Cur;//
extern DeadStruct dead[DeadMonsters_Sprites_Max_Count];//
extern DeadStruct blood[9];
extern char* SquareCELPtr;//
#if MULTI_PLAYER_DEBUG_CHECK
extern char MultiplayerDebugMap1[ (DungeonCount + 1) * FineMap_112 * FineMap_112 ];//	
extern char MultiplayerDebugMap2[ (DungeonCount + 1) * FineMap_112 * FineMap_112 ];//	
#endif
extern HWND ghMainWnd;//	(rt) WINDOWINFO structures could be compared, but everything in them differs anyway
extern HWND DlgHwnd;
extern int SystemWidth, SystemHeight;
extern int DungeonSeedCheck1[DungeonCount];
extern ExceptionFilter* OldTopLevelExceptionFilter;//	(rt) block start
extern int DungeonSeedCheck2[DungeonCount];
extern int CursorY;//	
extern int CursorX;//	
extern int IsWaitingNetTimeOut_MayBe;//	
extern bool NetThreadRun;
extern int DungeonRandSeed[DungeonExpantionLimit];
extern int gbRunGame;//	(rt) block start
extern int DungeonSeedCheck3[DungeonCount];
extern int gbRunGameResult;//	
extern int IsZoomDisable;//	
extern int BoolWithPlayer;//	
extern int DungeonSeedCheck4[DungeonCount];
extern HINSTANCE HInstance;//	
extern int MapRowArray[10];//	
extern char gbDoEnding;//	
extern bool UberDiabloKilled; // (rt) for nightmare and ironman; partly duplicates the flag above, but that one in the original is more likely named PlayVictoryVideo
extern int & SpeedrunTimer;
extern SPEEDRUN_STEP & CurrentSpeedrunStep;
extern int TestFOPMFlag;//	
extern bool Light4Levels;//	
extern bool DungeonDebug;//	
extern int SomeBoolFlag_12;//	
extern int SomeLevelRandSeed;//	(rt) block start
extern int MapRowArraySize;//	
extern int GamePaused;//	
extern char SomeBoolFlag_18;//	
extern char CowQuestOn;//	
extern char NotUsedFlagWithPalette;//	
extern char Enable5Class;//	
extern char Enable6Class;//	
extern char SomeBoolFlag_17;//	
extern int NetCursorItemId;//	
extern char MouseButtonPressed;//	(rt) 1=left, 2=right // temporarily disabled for the save/load comparison
extern i64 PaletteFrameSync;//	
extern char IsPopupScreenOpen;//	
extern char PopupScreen[228 * 1024];
extern int DiabloQuestTimer;//	
extern int SomeDword_60;//	
extern char InitalChurchWallScheme[ 80 ][ 80 ];//	(rt) block start
extern int IsFirstBigChurchHallPlaced;//	
extern int IsSecondBigChurchHallPlaced;//	
extern int IsThirdBigChurchHallPlaced;//	
extern int UberRow;//	
extern int UberCol;//	
extern int SomeDword_64;//	
extern int IsUberRoomOpened;//	
extern int UberLeverRow;//	
extern int UberLeverCol;//	
extern int IsUberLeverActivated;//	
extern int UberDiabloMonsterIndex;//	
extern int IsFirstBigChurchHallPlaced2;//	
extern int IsSecondBigChurchHallPlaced2;//	
extern int IsThirdBigChurchHallPlaced2;//	
extern vector<QuestPattern> QuestPatterns;//	
extern int CataQuestRoomStartRow;//	(rt) block start
extern int CataQuestRoomEndRow;//	
extern int CataQuestRoomStartCol;//	
extern int CataQuestRoomEndCol;//	
extern int CataHallsCount;//	
extern uchar CatacombMapAdvance[ Map_40 ][ Map_40 ];//	
extern CataHall CataHalls[80];//	
extern CataPassageQueue* CataPassageQueuePtr;//	(rt) block start
extern char MaybeHasLavaTiles;//	
extern int MapSize;//	
extern int LinkedFloorTilesCount;//	
extern char CaveLinkedFloorCheckMap[ Map_40 ][ Map_40 ];//	(rt) block start
extern int DiabloQuad1row;//	(rt) block start
extern int DiabloQuad1col;//	
extern int DiabloQuad3row;//	
extern int DiabloQuad3col;//	
extern int DiabloQuad2row;//	
extern int DiabloQuad2col;//	
extern int DiabloQuad4row;//	
extern int DiabloQuad4col;//	
extern int DiabloQuadsStartRow;//	
extern int DiabloQuadsStartCol;//	
extern int Map40x40gStartRow;//	
extern int Map40x40gEndRow;//	
extern int Map40x40gStartCol;//	
extern int Map40x40gEndCol;//	
extern char InitalHellWallScheme[ Map_40*2 ][ Map_40*2 ];//	
extern uchar BinaryHellMap[ Map_40/2 ][ Map_40/2 ];//	(rt) block start
extern uchar* SoftSrcSurfacePtr/*[656][768]*/;//	(rt) comparison temporarily disabled
extern LPDIRECTDRAW DirectDrawPtr;//	
//extern LPDIRECTDRAW7 DirectDrawPtr7;
extern LPDIRECTDRAWPALETTE DirectDrawPalettePtr;//	
extern int SurfaceLockCount;//	
extern uchar* WorkingSurface;//	
extern uchar* LightSurface;
// Wall occlusion buffer: 1 = opaque wall pixel drawn here, 0 = open.
// Non-wall sprites clear it during the main pass so only pixels where
// a wall was the LAST draw remain marked — the definition of occlusion.
extern uchar* WallOcclusionSurface;
extern uchar* IndexFlagSurface;// (th4) ultra vignetting flag (texture s4): 255 where WorkingSurface >= 128
extern uchar* AlphaSurface;// (th4) Stage 2 overlay weight (texture A byte source): 0 = no overlay
extern uint* SpriteLayerSurface;// (th4) Stage 2 occluded-sprite RGB (texture s2); A byte unused/zeroed
extern uint* LightTintSurface;// (th4) Stage 3 winning light color (texture s3); uint* by PIXEL offset, A unused
extern bool g_LightTintActive;// (th4) Stage 3 idle gate: any non-off light carries a non-white tint this frame
extern bool g_StaticTintActive;// (th4) Rev 2.46 Option C: this level has a colored static bake (StaticTintMap non-white)
extern const uint TINT_WHITE;// (th4) Stage 3 white tint word (0x00FFFFFF — A byte zero)
extern uint rgbLUT[256];// (th4) shared per-frame palette->RGB table (BuildScreenPaletteRGBLUT)
// Option C diagnostic: snapshot of LightSurface after pre-fill.
// Always declared (no #if) to avoid include-order issues; allocated
// only when OPTION_C_DIAG_LIGHTSURFACEBASE is defined in the TU.
extern uchar* LightSurfaceBase;
extern LPDIRECTDRAWSURFACE DDSrcSurface;//	(rt) block start
extern LPDIRECTDRAWSURFACE ScreenSurface;//	
extern LPDIRECTDRAWSURFACE ScreenSurface32;//	
extern LPDIRECTDRAWSURFACE ScreenSurface32Back;//	
extern CRITICAL_SECTION DDSurfaceCritSec;//	
extern bool DDrawEmulation;//	
extern HMODULE HModule;//	
extern int PlayerSoundMessageDelay;//	
extern int SoundMessage;//	
extern HANDLE CurSpeechHandle;//	
extern Sound* CurSpeechSound;//	
extern uint HashSource[5][256];//	
extern uchar AutomapCurrentDrawColor;//	
extern int AutomapRotated;//	
extern int LastSeed;//	
extern u64 LastSeed64;//	
extern int CurPitch;//	
extern int RandomSeed;//
extern u64 RandomSeed64;//
extern CRITICAL_SECTION MemoryAllocCritical;//	
extern uint RandUsingCounter;//	
extern uint Rand64UsingCounter;//	
extern int UnusedExceptionBuf;//	
extern LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter;//	(rt) marked as SYS for now; may be needed later for the comparison
extern char pCrashDumpFileName[264];//	(rt) marked as SYS for now; may be needed later for the comparison
extern char pExceptionStringBuffer[512];//	
extern ushort TileLightMapTop[2048];//	
extern int ChambersCount;//	
extern char CellTransparent[2049];//	
#if DEBUG_MEMORY
struct DebugMap{
	DebugMap(){ Lock(MonsterMap); }
	//char align[2496];
	debug<int> MonsterMap[ 128 ][ FineMap_112 ];
	uchar Map[ Map_40 ][ Map_40 ];
};
extern DebugMap __;
__forceinline void* Memset(void* dst, int val, size_t size, void* locked = &__.MonsterMap)
{
	if( dst == locked ) Unlock(locked);
	void* result = memset(dst, val, size);
	if( dst == locked ) Lock(locked);
	return result;
}
#define memset Memset
#define MonsterMap __.MonsterMap
#define Map __.Map
#else
extern int MonsterMap[FineMap_112][FineMap_112];
extern uchar Map[Map_40][Map_40]; // Big tile map (original dungeon)
#endif

extern char ObjectsMap[ FineMap_112 ][ FineMap_112 ];//	
extern array<char, 1024 * 1024> DarkenDungeonTiles;
extern int LevelCELsAmount;//	
extern uchar AltMap[ Map_40 ][ Map_40 ];//	
extern ushort DeathMonstersMap[ FineMap_112 ][ FineMap_112 ];//
extern uchar BloodMap[ FineMap_112 ][ FineMap_112 ];//
extern TileColumn TileMap[ FineMap_112 * FineMap_112 ];//	Cel map ?
extern unsigned char StaticLightMap[ FineMap_112 ][ FineMap_112 ];//
extern uint StaticTintMap[ FineMap_112 ][ FineMap_112 ];// (th4) Rev 2.46 Option C: static-bake winner color per tile (B|G<<8|R<<16, white = no color)	
extern uchar TransValue;//	
extern int MaxTilesInCell;//	
extern uchar FixedMap[ Map_40 ][ Map_40 ]; // Big tile flag map (original dflags)
extern int FineMap[ FineMap_112 ][ FineMap_112 ]; // (rt) and this is apparently the .min Map
extern char LightMap[ FineMap_112 ][ FineMap_112 ];//	movsx
extern int TileLightMapIndex[2048];//	
extern char* CurrentLevelTilesFile;//	
extern char* CurrentLevelMINFile;//	
extern GAME_MODE GameMode;
extern int GameModeFilter;
extern int DoMenuScreenShot;
extern uchar WallsTransparency[2049];//	
extern uchar TransMap[ FineMap_112 ][ FineMap_112 ];// Transparent active value
extern char IsWall[2049];//	
extern CurDungeon Dungeon;
extern char TransList[256];//	
extern uchar TileBlockWalking[2049];//	
extern uint TileLightMapCounter[2048];//	
extern ScrollView Scroll;
extern int PlayerShiftX, PlayerShiftY; // for correct motion interpolation (high fps)
extern int * CurrentLevelCELFile;//	
extern int DungeonTilesOffset[2048];//	
extern Chamber Chambers[RoomsMax];//	
extern char PlayerMap[ FineMap_112 ][ FineMap_112 ];//	
extern char ArchGraphicsMap[ FineMap_112 ][ FineMap_112 ];//	
extern uchar TileBlockSight[2049];//	
extern char* CurrentLevelSCELFile;//	
extern uchar FlagMap[ FineMap_112 ][ FineMap_112 ];//	
extern uchar ItemMap[ FineMap_112 ][ FineMap_112 ];//	
extern int SizeOfLevelCEL[2048];//	
extern uchar TileBlockMissle[2049];//	
extern int SomeYPos2;//	
extern int SomeXPos2;//	
extern int MapEndRow;//	
extern int MapEndCol;//	
extern QuestPattern CurPattern;
extern QuestPattern TmpPattern;
extern int PlayerRowPos;//	
extern int PlayerColPos;//	
extern MissileIndexT MissilesMap[ FineMap_112 ][ FineMap_112 ];//
extern int MapStartRow;//	
extern int MapStartCol;//	
extern TileColumn TileLightMap[ FineMap_112 ][ FineMap_112 ];//	
extern char* OptbarCELPtr;//	
extern bool IsMenuVisible;//	
extern char* PentSpinCELPtr;//	
extern Menu * CurrentMenuItem;//	
extern char* FontSpriteBig;//	
extern char LogoFrameCount;//	
extern i64 OldTickCount3;//	
extern char PentagramFrameCount;//	
extern MenuHandler* MainMenuHandlerPtr;//	
extern Menu* CurrentMenuWindowPtr;//	(rt) block start
extern char* OptionCELPtr;//	
extern char* Hf_logo3CELPtr;//	
extern int CurMenuSize;//	
extern i64 OldTickCount2;//	
extern int HelpCurrentPosition;//	
extern int SomeHelpMemory_1;//	
extern int IsHELPVisible;//	
extern int SomeHelpMemory_2;//	
extern int HelpStringsCount;//
extern char MpqPath_7[MAX_PATH];//	
extern HANDLE hellfireMPQPtr;//	(rt) all mpqPtr must be of type MPQ*
extern module_info ModuleInfo;//	
extern int IsMainWindowActive;//	
extern bool IsMainWindowMinimized;
extern char ThFilePath[MAX_PATH];//	
extern char MpqPath_1[MAX_PATH];//	
extern char MpqPath_2[MAX_PATH];//	
extern char MpqPath_a[MAX_PATH];//	
extern char MpqPath_3_Patch[MAX_PATH];//	
extern char MpqPath_4[MAX_PATH];//	
extern HANDLE hellmfMPQPtr;//	
extern ProcHandler Handler1;//	
extern char MpqPath_6[MAX_PATH];//	
extern HANDLE thmusicMPQPtr;//	
extern HANDLE thmusi3MPQPtr;//	
extern char MpqPath_9[MAX_PATH];//	
extern HANDLE helldfMPQPtr;//	
extern char MpqPath_8[MAX_PATH];//		
extern HANDLE diabdatMPQPtr;//	(rt) all mpqPtr must be of type MPQ*
extern HANDLE hfmonkMPQPtr;//	
extern char MpqPath_5[MAX_PATH];//	
extern HANDLE hellgfxMPQPtr;//	
extern HANDLE thlocRusMPQPtr;// (th4) RU audio pack
extern HANDLE thlocIdMPQPtr;// (th4) ID audio pack
extern HANDLE thlocPrtMPQPtr;// (th4) PT (Portuguese) loc datafile
extern HANDLE thlocTurMPQPtr;// (th4) TR (Turkish) loc datafile
extern HANDLE thlocEspMPQPtr;// (th4) ES (Spanish) loc datafile
extern HANDLE thlocPolMPQPtr;// (th4) PL (Polish) loc datafile
extern HANDLE thlocCzeMPQPtr;// (th4) CS (Czech) loc datafile
extern HANDLE thlocHrvMPQPtr;// (th4) HR (Croatian) loc datafile
extern HANDLE thlocChiMPQPtr;// (th4) CHI (Chinese) loc datafile
extern HANDLE thlocKorMPQPtr;// (th4) KO (Korean) loc datafile
extern HANDLE thlocJpnMPQPtr;
extern HANDLE thlocAraMPQPtr;// (th4) JA (Japanese) loc datafile
extern HANDLE thlocHinMPQPtr;// (th4) HIN (Hindi) loc datafile
extern HANDLE thlocBlgMPQPtr;// (th4) BG (Bulgarian) loc datafile
extern char MpqPath_Main[MAX_PATH];//	
extern HANDLE helldataMPQPtr;//	
extern HANDLE thassaMORPtr;//	
extern HANDLE thgladMPQPtr;//
extern char MpqPath_dev[MAX_PATH]; // dev mpq	
extern HANDLE th2devMPQPtr;//	
extern int MOMParentFound;//	
extern char SomeBoolFlag_38;//	
extern char* LoadingScreenCEL;//	
extern uint ProgressBar;//	
extern int ProgressBarIndex;//	
extern bool IsINVPanelVisible;//	
extern char* InvPanelCEL;//
extern char* VisualStashPanelCEL;
extern char* VisualStashButtCEL;
//extern char* VisualStashButtScrlCEL;
extern char* VisualStashButtLeftCEL;
extern char* VisualStashButtRightCEL;
extern char* VisualTradePanelCEL;
extern char* VisualTradeTabsCEL;
extern char* VisualTradeBtnsCEL;
extern char* CraftPanelCEL;
extern char* CraftTabsCEL;
//extern char* CraftBtnsCEL;
extern char* CraftPanel2CEL;
extern char* CraftSocketsLockCEL;
extern char* CraftSocketCEL;
extern Gem Gems[];
extern char* EnchantPanelCEL;
extern char* PowerupPanelCEL;
extern char* TransmutePanelCEL;
extern CellSlots<5, 10> TransmuteSlots;
//extern char* QuenchPanelCEL;
extern char* LongButtonCEL;
extern i64 StatusMessageTimeout;//	(rt) block start
extern int ItemActive[MaxItems_255];
extern int ItemAvailable[MaxItems_255];
extern Item UniqueInfo;//	
extern Item Items[MaxItems_255 + 1];// (rt) Item[127] is the buffer for transferring/loading items // size 128, not 129; item -1 is array access by itemNumber instead of itemIndex
extern int RowOfCornerStone;//	
extern int ColOfCornerStone;//	
extern char* FlipItemCELFiles[FL_81_COUNT];//	(rt) block start
extern int DoubleMaxGoldInCell;//	
extern int ItemsCount;//	
extern Light VisionList[32];//
extern char LightIndexes[Lights_128_Count];//
extern Light LightList[Lights_128_Count];//
extern LightTint LightTintList[Lights_128_Count];// (th4) Stage 3 colored-light tint side table (white = no tint)
extern int LightsCount;//	
extern uchar LightLevels[16][128];//	
extern int IsDoVision;//	
extern int VisionCount;//	
extern char LightMax;//	
extern int NeedLightRecalc;//	
extern uchar LightDist[8][8][16][16];//	
extern uchar g_A8L8LightBright[16][1025];//	A8L8 per-pixel falloff LUT, 1/64-tile linear distance
extern int VisionId;//	
// (th4) LightTable row map, previously three magic numbers scattered across three files:
//   rows 0-15  darkness levels        (or 0-3 when Light4Levels is on)
//   row  16    red
//   rows 17-18 stone / grey / infra
//   rows 19+   ONE PER LIVE UNIQUE MONSTER on the level -- Monster::uniqTrans is literally
//              the offset from row 19, and CL2_DrawDark resolves it back the same way.
// The boss range used to be 8 rows, which is why the DC hub needed 13 uniques to share a
// single row and ended up with every vendor wearing the last-loaded TRN. 21 rows fits the
// hub with headroom and costs 5 KB of static data.
const int LightTable_BossRowFirst = 19;
const int LightTable_BossRowCount = 21;	// was 8
const int LightTable_RowCount     = LightTable_BossRowFirst + LightTable_BossRowCount;	// 40
extern uchar LightTable[LightTable_RowCount * 256];
extern uchar* g_PPLAnimRemap; // LightTable row 0 for Hell (blood cycling); identity elsewhere
// Per-palette-index brightness mask for A8L8+PPL: 0x00 = always bright (crypt/abyss lava
// entries 1-15), 0xFF = use actual per-pixel light level.  Set by MakeLightTable.
extern uchar g_PPLBrightnessMask[256];
extern int FullLighting;//	
extern char* CurSaveData;//	
extern char HeroName[16];//	
extern int Speech_CurLineY;//	
extern char* Speech_FirstDrawingLine;//	
extern i64 Speech_PrevTick;//	
extern char Speech_IsPanelVisible;//	
extern bool DarkCampaignLazarusTalkWanted;// (th4) DC m13: player clicked Lazarus, walking over
extern bool DarkCampaignLazarusRewardPending;// (th4) DC m13: thanks playing, reward on close
extern bool DarkCampaignLazarusThanked;// (th4) DC m13: talk done -- releases the way home
extern bool DarkCampaignHephastoTalkWanted;// (th4) DC m15: player clicked Hephasto
extern bool DarkCampaignHephastoRewardPending;// (th4) DC m15: thanks playing, reward on close
extern bool DarkCampaignHephastoThanked;// (th4) DC m15: talk done -- releases the way home
extern bool DarkCampaignButcherTalkWanted;// (th4) DC m25: player clicked the Butcher, walking over
const int DC_BUTCHER_TALK_ROW = 68;	// (th4) DC m25: where the player stands to speak to him
const int DC_BUTCHER_TALK_COL = 53;
extern bool DarkCampaignButcherJoinPending;// (th4) DC m25: his line is playing, he joins on close
extern bool DarkCampaignButcherNeedsArrival;// (th4) DC m25: pull him to the player on the first hub tick
const int DC_END_LORD_DELAY = 40;	// (th4) DC m26: the pause between the portal opening and the Dark
                                	// Lord stepping out. Counted in GAME LOGIC ticks, which run at a
                                	// fixed 20/sec regardless of render fps -- so 40 is two seconds.
const int DC_END_LORD_STEPS = 2;	// (th4) DC m26: tiles he walks south out of it
const int DC_END_LORD_IDLE_DELAY = 2;	// (th4) DC m26: ticks per idle frame once he is standing
const int DC_END_PORTAL_ROW = 84;	// (th4) DC m26: where the Dark Lord's portal opens
const int DC_END_PORTAL_COL = 71;
extern bool DarkCampaignEndArrived;// (th4) DC m26: Ravenholm is clear, the Dark Lord has stepped through
extern int  DarkCampaignEndLordDelay;// (th4) DC m26: frames left before he steps through
extern int  DarkCampaignEndWalkSteps;// (th4) DC m26: tiles left to walk out of the portal
extern bool DarkCampaignEndTalkWanted;// (th4) DC m26: player clicked him, walking over

extern int Speech_ScrollCurStep;//	
extern int Speech_ScrollStep;//	
extern char* FontSpriteMid;//	
extern char* TextBoxCel;//	
extern int MissileIndexes[MissilesMax];//
extern int MissileIndexList[MissilesMax];//
extern Missile Missiles[MissilesMax];//
extern int MissileAmount;//	
extern int ManaShieldActed;//	(rt) block start
extern int MayBeExplodingMagicFlag;//	 
extern ushort TotalSlain[1024];//
extern int MonsterIndexes[Monsters_Max_Count];//
extern int MonstersCount;//
extern bool SummonMonstersAdded;
extern bool SummonCleanupSilent;	// (th4) level-transition summon cleanup: no death sound
extern char SaveSound;//	
extern Monster Monsters[Monsters_Max_Count];//	 MonstersArray
extern int MonsterArraySize;//	
extern MonsterSprite MonsterSprites[Monsters_Sprites_Max_Count];//
extern int MonstersSeedingSizeTotal;//	(rt) block start
extern int BossLightIndex;//	
extern int MonsterSpritesCount;//	
extern char VideoWindowExists;//	
extern int LoopMovie;//	
extern uint NextFileStart;//	
extern uchar WriteBuffer[4096];//	
extern int SaveChanged;//	
extern char SaveCreationKey;//
extern bool HashLoaded;
extern bool BlockLoaded;
extern char NetPlayerCount;//
extern uchar MaxCountOfPlayersInGame;//	1 == Single Player
extern uchar sgbNetUpdateRate;//
extern int gdwMsgLenTbl[4];//	size of arraydata 0x006DE0A8
extern CRITICAL_SECTION FrameSyncCritSection;//	
extern uint DeltaBytesPerSecond;//	
extern uint gdwTurnsInTransit;//	the number of calls before data is sent over the network // between 2 and 8; single player is set to 1
extern uint* glpMsgTbl[4];//
extern uint SyncNetFrameTimeThreadId;//	multi
extern char sgbSyncCountdown;//	net multithreading sync
extern int NetTurnNeedSync;//	
extern char sgbGotPacketOK;//	
extern char sgbPacketCountdown;//	
extern char FrameSyncCritSecState;//	
extern uint MaxNetMsgSize;//	
extern int gdwNormalMsgSize;//
extern i64 MainFrameTimer;//	(rt) every 10 frames, 1 reference frame for synchronization?
extern int UberBookReadParam;//	
extern int FlameGroupIndex;//	
extern int FlameOrientation;//	
extern char* ObjectsSprites[100];//
extern char ObjectSpriteIndexes[100];//
extern int ObjectActive[MaxObjects_127];//	
extern int ObjectsCount;//	
extern int LeverId;//	
extern int ObjectAvailable[MaxObjects_127];//	
extern Object Objects[MaxObjects_127];//	
extern int InitObjFlag;//	
extern int ObjectsSpritesCount;//	
extern PALETTEENTRY BasePalette[256];//	
extern PALETTEENTRY ScreenPalette[256];//	
extern PALETTEENTRY FilePalette[256];//	
extern int HalfReservedSystemPalette;//	
extern int CryptPalAnimEachSecFrame;//	(rt) block start
extern int CryptPalCyclingEachSecFrame;//	
extern int AbyssPalAnimEachSecFrame;//	
extern int AbyssPalCyclingEachSecFrame;//	
extern PathStep StepsPool[300];//	
extern int Steps300StackIndex;//	
extern int StepsCount;//	
extern int CurStepOrientIndex[MaxPathStep_25];//	
extern PathStep* PrevStep;//	
extern PathStep* Dword300Stek[300];//	
extern PathStep* CurrentStep;//	
extern CharName CharNameSlots[MaxSaveSlots];//
extern int gbValidSaveFile;//	
extern int gbSaveFileExists;//	
extern i64 AutoSaveTickCount;//	
extern int AmbientSfxCountdown[5];// th4: ambient event-sfx countdowns
extern int AnimSizeLightning;//	
extern int AnimSizeWalk;//	
extern int AnimSizeAttack;//	
extern uint CurrentPlayerIndex;//	
extern int AnimSizeFire;//	
extern int AnimSizeMagic;//	
extern int AnimSizeCounted;//	(rt) block start
extern int IsPlayerDead;//	
extern int AnimSizeHit;//	
extern int AnimSizeBlock;//	
extern int AnimSizeStay;//	
extern int DeathDelayTimer;//	
extern int AnimSizeDeath;//	
extern int AnimSizeTurning;
extern Player* Players;//	(rt) temporarily comparing only one player in single player
extern unsigned __int8 PlayerStatusMessageIndex;//	 
extern PlayerStatusMessage PlayerStatusMessages[MaxMessages_8];//	
extern Portal Portals[PlayersMax_4];//	
extern int CurrentPortal;//	
extern int QuestPanel_StartRowIndex;//	
extern bool IsQUESTPanelVisible;//	
extern char* Data_QuestCEL;//
extern char* Data_ExclamCEL;//
extern Quest Quests[Quest_Count];//	
extern int QuestPanel_SelectedRow;//	
extern int QuestPanel_ActiveQuestsList[Quest_Count];//	
extern int QuestPanel_ActiveQuestCounts;//	
extern int QuestPanel_Scroll;				// (th4) Quests.cpp, first quest shown when the log scrolls
extern bool questUpArrowPressed, questDownArrowPressed;	// (th4) Quests.cpp, arrow press visuals
extern int WaterPaletteInPoizonRiverQuestCave;//	
extern int QuestPanel_PentagramsPicFrame;//	
extern uchar DarkLevel;//	(rt) comparison temporarily disabled
extern int YOffsetHashTable[Y_SIZE];//	(rt) 1024 in the original
extern int DrawnCursorWIdth;//	(rt) block start
extern int CurCursorStartX;//	
extern int CurCursorStartY;//	
extern uchar* WorkingSurfaceDrawEnd;//	
extern uchar* WorkingSurfaceDrawBegin;
extern int CurCursorHeight;//	
extern uint CurTileID;//	
extern int DrawnCursorStartX;//	
extern int DrawnCursorStartY;//	
extern char PartialTrans;//	0,1,2
extern DDSURFACEDESC ScreenSurfaceDesc;//	
extern int TransparentMode;//	(rt) comparison temporarily disabled
extern int WallIndex;//	(rt) comparison temporarily disabled
extern int CurCursorWidth;//	
extern unsigned char CursorImage[1920 + MaxItemCellWidth_4 * CELL_SIZE * MaxItemCellHeight_7 * CELL_SIZE];//	(rt) comparison temporarily disabled
extern int DrawnCursorHeight;//	
extern ShaInfo ShaInfos[3];//
extern SoundDup SoundDups[MaxDupSounds];//	
extern IDirectSound* DirectSoundPtr;//	
extern bool IsDirectSoundAllocated;//	
extern int MusicVolume;//	
extern int AmbientVolume;//	
extern int SoundVolume;//	
extern HMODULE DirectSoundModule;//	(rt) block start
extern HANDLE MusicFile;//	
extern HANDLE AmbientFile;//	
extern IDirectSoundBuffer* IDirectSoundBufferPtr;//	
extern int TopSelectedRow;//	
extern int DialogRowsCount;//	
extern int SelectedDialogRowInPrevDialog;//	
extern Item WirtItem;//	
extern PLAYER_DIALOG ConfirmationCaller;//
extern Item MagicItemsStoreArray[PT_MagicItemsCount];//	
extern char* TextBox2CELPtr;//	
extern int GriswoldItemsLevelParam;//	
extern int CurrentTowner;//	
extern TownersDialogRow TownersDialogRows[PT_DialogRowsCount];//	
extern char IsBigMenuBox;//	
extern int ItemCount;//	
extern int CursorPentAnimationNumber;//	
extern Item PlayerItemsForSomeAction[PT_48_PlayerItemsCount];//	
extern int StartIndexOfGossibSpeechByCurrentCityzen;//	
extern Item WitchAdriaItems[25];//	
extern int DialogHasScroolBox;//	
extern int GriswoldMagicItemCount;//	
extern Item PepinItems[20];//	
extern Item ItemGoldBuffer;//	
extern char IndexOfPlayerItemInDialog[PT_48_PlayerItemsCount];//	
extern char* TextSlidCELPtr;//	
extern int ScrolledItemsCountInPrevDialog;//	
extern int SelectedDialogRow;//	
extern char DialogScrollDownDelay;//	
extern int EndIndexOfGossibSpeechByCurrentCityzen;//	
extern char* PentSpn2CELPtr;//	
extern int FirstVisibleItem;//	
extern int WirtItemHalfQlvl;//	
extern Item BasicalItemsArray[PT_BasicItemsCount];//
extern int BottomSelectedRow;//	
extern char DialogScrollUpDelay;//	
extern PLAYER_DIALOG CurrentDialogIndex;//
extern int RoomsCount;//
extern int IsNotArmorPlaced;//	
extern int IsWeapondStandNotPlaced;//	
extern int IsRoomWithGoldNotPlaced;//	
extern int IsMurkyPoolNotPlaced;//	
extern int IsCauldronNotPlaced;//	
extern int IsFountainOfTearsNotPlaced;//	
extern int ZharTheMadRoomIndex;//	
extern int RoomCursorRow;//	
extern int RoomCursorCol;//	
extern int RoomCursor_MonsterSprite_Or_Direction;//	
extern Room Rooms[RoomsMax];//	
extern int IsPuriFyingSpringNotPlaced;//	
extern int IsBloodFountanIsNotPlaced;//	(rt) block start
extern int IsBurnCrossAdded;//
extern int NPCMenusEnabled;//	
extern int CowClickPlayerSoundIndex;//	
extern uchar TownersCount;//	
extern int CowClickCount;//	
extern int SomeBoolFlag_41;//	
extern int WirtIsPlacedOnMap;//	
extern char* CowSprite;//	
extern Towner Towners[16];//
extern int TownDoorOpen[DD_COUNT];//	
extern int IswarpUnderCursor;//	(rt) block start
extern vector<Warp> Warps;//	
extern int WinBuildVer;//	
extern int WinMajorMinorVer;//	
extern int WinMajorVer;//	
extern int WinMinorVer;//	
extern UniqueItem* UniqueItems; // TODO: may be vector in future
extern size_t count_UniqueItems; extern const int NextNewUniqId;
extern UniqueSet UniqueSets[]; extern const size_t count_UniqueSet;
extern Speech Speeches[480];
extern uchar DarkMissionState[DARK_MISSION_MAX_50];	// (th4) Dark Campaign mission states (Quests.cpp, saved with the game)
extern const struct DarkMission DarkMissions[];	// (th4) Dark Campaign mission table (Monster.cpp)
extern const int DarkMissionCount;//	(rt) was 0x00490410
extern uchar ItemWidthTable[MAX_9000_ITEM_PICS];//
extern uchar ItemHeightTable[MAX_9000_ITEM_PICS];//
extern uchar ItemFlipTable[MAX_9000_ITEM_PICS];//
extern char MonsterIntel_1[16];//	
extern char MonsterIntel_2[16];//	
extern BaseItem* BaseItems; // TODO: may be vector in future
//extern BaseItem* BaseItems_Classic; // TODO: may be vector in future
extern size_t count_BaseItems; extern int GOOD_ITEM_COUNT; extern const int NextNewBaseId;
//extern size_t count_BaseItems_Classic; extern int GOOD_ITEM_COUNT_Classic; extern const int NextNewBaseId_Classic;
extern FlaskInfo FlasksInfo[140];//
extern TrapInfo TrapsInfo[42];
extern BaseMonster BaseMonsters[BM_COUNT];//	(rt) 222 was 0x004ABEF0
extern int BaseMonsterIndexes[BM_COUNT + 16];//	238
extern bool HorizontalHit;//	
extern uchar ClvlReqsForEnterInDungeon[9][6];//	
extern char ClvlNotReqForEnterInDungeonMessagesIndexesTable[9][6];//	
extern char* OnScreenMessages[120];//	
extern Sound Sounds[S_COUNT + Monsters_Sprites_Max_Count * Monster_Sound_Max_8];
extern UniqueMonster UniqueMonsters[UM_count];//	277
extern int QuestGroup1[4];//	
extern char* TheInfestedCellarDunsTable[3][3];//	mor: this is obsolete and should be deleted
extern BaseDungeon Dungeons[DungeonCount];//	(rt) was 0x004B7310
extern DungeonType DungeonTypes[DT_COUNT];
extern BASE_OBJECT BaseObjectsTableIndexes[247];//	(rt) was 0x004B3DC0 int,[114] // 125 was the size in th2 earlier
extern int QuestGroup5[5];//
extern int QuestGroup6[5];//
extern RandomizeQuestsGroup RandomizeQuestsTable[10];//	
extern char* BookNamePtrTable[25];//	(rt) was 0x00490270
extern int HorazonQuestBookSpeechTable[16][6];//	
extern char HorazonBooksCombinationTable[6][3];//	
extern int LeverClickPlayerSound;//	(rt) block start
extern int GrimspikeMonsterIndex;//	
extern int HorazonGrimspikeDelay;//	
extern int FleshDoomDelay;//Separate Flesh Doom quest timer from Horazon's
extern int NakrulMonsterIndex;//
extern int CerberusMonsterIndex;
extern int OrzogMonsterIndex;
extern int TheDenOneSecondTimer;
extern int TheDenMonsterSpawnTimer;
extern int Lava_Pits_OneSecondTimer;
extern int Lava_Pits_MonsterSpawnTimer;
extern MapCoord TheDenPortalPositions[];
extern int LeverClickPlayerSoundIndex;//
extern int LeverSounds[6*3];//	
extern uchar ItemUpdateTimer;//	
extern CompleteSetEffect CompleteSetEffects[20];//	(rt) block start
extern Item SetBufferItemForShowEffect;//	
extern int IsShiftPressed;//	
extern int AutoClickDelay;//	
extern uchar PatMap9x9_IzualPattern[164];//	
extern char CooldownDecDelay;//	(rt) verified in the original: the address matched AttackTypeFuncsTable
extern int QuestGroup4[4];//	(rt) was 0x004B6B30
extern uchar IslandPattern[52];//	Quest
extern uchar SwampPattern[52];
extern uchar GrottoPattern[52];
extern char AngelSeq[9];//	Quest
extern uchar CurAngelSeqIndex;//	Quest
extern uchar CritSplashNumbers[501];//	Monster
extern uchar CritAnimationsIndexTable[9];//	Monster
extern int QuestGroup2[4];//	(rt) was 0x004B6B10
extern uchar OilQuality[10];//	
extern ChampType* ChampTypeTable[10];//	Monster
extern char* ChampionsNameTable[10];//	Monster
extern LastItemInfo WeaponSwapItems[4];//	Item
extern char AltWeaponIndex;//	Item
extern char* WpnSwapBtnImg;//	Item
extern RECT WpnSwapBtnsRects[6];//	Item
extern BaseObject BaseObjects[100];//	(rt) was 0x004B3F88
extern char* FloorObjectsNamesOriginal[57];//	(rt) was 0x004B5090
extern char* FloorObjectsNamesCrypt[57];//	(rt) was 0x004B5170
extern char* FloorObjectsNamesAbyss[57];//	(rt) was 0x004B5250
extern char* PlayerTrnName[128];//	
extern uchar* PlayerTrn[128];//	
extern IDirect3DPixelShader9* DirectShader;// shader for automap
extern LPDIRECT3DSURFACE9 VideoSurface;//	
extern IDirect3DPixelShader9* PaletteShader;// Main shader
extern IDirect3DPixelShader9* InterfaceShader;// reserved for second layer
extern Item TempItem;//	
extern LPDIRECT3DTEXTURE9 D3dPalette;//	Palette texture (used by shaders)
extern LPDIRECT3DTEXTURE9 D3dTexture;//
extern LPDIRECT3DTEXTURE9 D3dTextureColor;//	(th4) ultra color texture (A8R8G8B8, s0)
extern LPDIRECT3DTEXTURE9 D3dTextureL8;//	(th4) ultra light texture (L8, s1)
extern LPDIRECT3DTEXTURE9 D3dTextureFlag;//	(th4) ultra vignetting flag texture (L8, s4)
extern LPDIRECT3DTEXTURE9 D3dTextureOverlay;//	(th4) ultra overlay texture (A8R8G8B8, s2)
extern LPDIRECT3DTEXTURE9 D3dTextureTint;//	(th4) Stage 3 tint texture (A8R8G8B8, s3)
extern LPDIRECT3DTEXTURE9 SecTextureColor;//	(th4) ultra color texture, second window
extern LPDIRECT3DTEXTURE9 SecTextureL8;//	(th4) ultra light texture, second window
extern LPDIRECT3DTEXTURE9 SecTextureFlag;//	(th4) ultra vignetting flag texture, second window
extern LPDIRECT3DTEXTURE9 SecTextureOverlay;//	(th4) ultra overlay texture, second window
extern LPDIRECT3DTEXTURE9 SecTextureTint;//	(th4) Stage 3 tint texture, second window
extern DisplayObject g_TooltipClearRect;// (th4) union of tooltip rects drawn this frame (post-UI overlay clear)
extern int DualLeftHand;//	
extern LPDIRECT3DDEVICE9 D3dDevice;//
extern bool g_D3dInReset;//	(th4) 2026-09-10 reentrancy guard: true while D3dReset()/SecondReset() is inside IDirect3DDevice9::Reset()	
extern LPDIRECT3DSURFACE9 D3dSurface;//	
extern LPDIRECT3DSURFACE9 BackBuffer;//	
extern int MainMenuEnterFlag;//	
extern uint StormReloc;//	
//extern uint HellReloc;//	
extern int ScreenWidth;//	
extern int ScreenHeight;//	
extern char PlayerSpellIndexOfCurNova;//	
extern char CastingNovaInProgress;//
extern ConfigLine ConfigStructure[];
extern bool UserPanelB;
extern bool UserPotionB;
extern bool D3dRef;
extern int AllocCount;
extern int FreeCount;
extern int WorldFrameIndex;
extern int WorkingHeight;
extern int WorkingWidth;
extern int MaxScreenHeight;
extern int MaxScreenWidth;
extern int MaxWorkingHeight;
extern int MaxWorkingWidth;
extern unsigned char  s_PerPixelLightBuf[64 * 32]; // backing buffer for ST CalculatePerPixelLight
extern unsigned char* PerPixelLight;               // points to s_PerPixelLightBuf (ST) or s_PPLData[r][c] (MT)
extern int           PerPixelXOffset;
extern bool          PerPixelLightUniform;
extern char MpqPath_TH2[MAX_PATH];
extern HANDLE thehell2MPQPtr;
extern bool IsAutoPickup;
extern int RowStep[9];
extern int ColStep[9];
extern PlayerClass PlayerClasses[PC_COUNT];
extern FullPlayerClass FullPlayerClasses[PFC_COUNT];
extern uchar MorMenuPal[1024];
extern uchar MenuPal[1024];
extern vector<Slot> Slots;
extern Slot* LeftHand, *RightHand, *Inv, *Belt;
extern DisplayObject PerksPanelPos;
extern HMODULE Storm2Base;
extern int StartLevelSeed[DungeonCount];
extern int MatchLevelSeed[DungeonCount];
extern bool	ExternalStorm;
extern bool	OriginalMenu;
extern bool	AllSound;
extern int	IHateMonsters;
extern bool	IsNotMapOpen;
extern bool	PixelLight;
extern bool	MultiThreadPixelLight;
extern bool	g_A8L8Lighting;
extern bool	A8L8PixelField;	// per-pixel light field pre-fill (Phase 2 of PixelLight_A8L8_PerPixelField plan; requires PixelLight + g_A8L8Lighting)
extern bool	g_ARGBLighting;	// (th4) ultra two-texture mode (A8R8G8B8 + L8); requires EnrichedLight + EnrichedLightEnhanced
extern int	EnrichedLightUltraAlpha;// (th4) Stage 2 overlay alpha 0-255; default 128 = overlay on when ultra is on; 0 = feature off
extern bool	g_ARGBAlphaContrast;// (th4) Stage 2 rung 2: contrast-weighted overlay alpha; default ON; OFF = constant (rung 1)
extern bool	g_ARGBAlphaRim;// (th4) Stage 2 rung 3: mask-edge emphasis (outline at full base alpha); default ON; requires contrast ON
extern int	A8L8GammaBlendPct;// A8L8 per-pixel falloff shape, percent: 100 = (1-f)^2 (original), 0 = linear
extern bool	g_SkipSpriteLightClear;	// temporarily skip LightSurface clearing during player draw (archway protection)
extern bool	g_InDungeonRender;		// true during DrawIsoLand; narrows R_DARK guard to dungeon only
// Force LightSurface clearing for specific sprites (torches/light sources)
// even when the dungeon-wide guard would otherwise skip it.
extern bool	g_ForceSpriteLightClear;
// Post-render LightSurface-only re-draw pass (see dataseg.cpp for details)
extern bool	g_LightSurfaceOnlyPass;
extern bool	g_TintExcludeCurrentSprite;	// (th4) Stage 3 owner-pixel tint exclusion marker for the replay
// Set BEFORE rendering opaque wall layers; read by linecpyPerPixel
// to mark WallOcclusionSurface.  Cleared after wall layers.
extern bool	g_IsDrawingOpaqueWall;
// Set while drawing opaque dungeon objects (doors/chests/barrels...); read by
// RleDraw to mark WallOcclusionSurface with WOB_OBJECT.
extern bool	g_IsDrawingOpaqueObject;
// Sprite type for the current post-render draw call.  Set before
// each CL2_Draw in the post-render replay loop; read by Rle2Draw.
extern SpriteType g_CurrentPostRenderType;
// Saved player draw parameters for post-render (one slot per player index 0-3).
// struct PlayerReDrawCmd defined in structs.h
extern PlayerReDrawCmd g_ReDraw[PlayersMax_4];

// Post-render sprite command list for monsters and missiles (Option C).
// Populated during normal rendering; replayed after all tiles to write LightSurface.
// struct PostRenderSpriteCmd defined in structs.h
// Should be no OOB risk as relevant logic is guarded.  1024 should be more than enough.
#define MAX_POSTRENDER_SPRITES 1024
extern PostRenderSpriteCmd g_PostRenderSprites[MAX_POSTRENDER_SPRITES];
extern int g_PostRenderSpriteCount;

#if CHEATS
extern bool DevelopMode;
extern bool FreeSpell;
extern bool	IsGodMode;
extern bool	Hedgehog;
extern bool	OneShoot;
extern bool	SaveAlwaysEnabled;
#else
#define DevelopMode false
#define FreeSpell false
#define IsGodMode false
#define Hedgehog false
#define OneShoot false
#define	SaveAlwaysEnabled false
#endif
//extern bool DotAcidMechanic;
extern bool PanelsOnCenter;
extern bool	DropUnderMonsters;
extern BaseAction BaseActions[ 100 ];
extern bool	IsFullScreen;
extern bool	IsThException;
extern bool	IsStartVideo;
extern bool	LoadOnStart;
extern bool NewGameOnStart;
extern int DifficultyOnStart;
extern bool LoadMulti;
extern int	AutoLoadSlot;
extern bool	AutoCopySave;
extern bool	VisualOnly;
extern bool	SyncTH;
extern bool SyncTimer;
extern int	SingleLog;
extern bool	SaveFolder;
extern char* SaveFolders[6];
extern char SaveFile_Swap[];
extern char SaveFile_Game[];
extern char SaveFile_Transmute[];
extern char SaveFile_HashTable[];
extern char SaveFile_BlockTable[];

extern bool	AutoBackup;
extern const int SaveSignature;

extern int DrawMode;
extern _D3DTEXTUREFILTERTYPE FilteringMode;
extern bool SharpScaling; // (th4) Config.cpp; used by dx_utility.cpp SharpStretch
extern bool GrottoTrip; // (th4) Config.cpp; dx_utility.cpp PostFxPresent / GrottoTripCA
extern bool IceAgeTreeSnow; // (th4) Config.cpp; Ice Age snow-covered, frosted trees, TownWater.cpp IceTrees_Active (default on)
extern bool IceAgeRiverMirror; // (th4) Config.cpp; Ice Age town river mirrors a winter sky, dx_utility.cpp IceRiver_Active (default off)
extern bool AutomapShadow; // (th4) Config.cpp; automap.cpp AUTOMAP SHADOW
extern bool ShowItemLevelOnGround; // (th4) Config.cpp; used by automap.cpp HighlightItemsNameOnMap
extern bool FsrUpscale;   // (th4) Config.cpp; used by dx_utility.cpp FsrPresent
extern int FsrSharpness;  // (th4) Config.cpp; 0..20 = 0.0..2.0 stops
extern int ChromaticAberration; // (th4) Config.cpp; used by dx_utility.cpp PostFxPresent
extern int HeatHaze;            // (th4) Config.cpp; used by dx_utility.cpp PostFxPresent / HeatHaze_*
extern int WaterShimmer, TiltShift, Shockwave, CrtEffect; // (th4) Config.cpp; post effects, dx_utility.cpp PostFxPresent
extern int GroundMist;          // (th4) Config.cpp; fixed 5 for now (row disabled) -- dx_utility.cpp Mist_Present
extern int Antialiasing;        // (th4) Config.cpp; 0 off, 1 FXAA, 2 SMAA -- dx_utility.cpp AntialiasPresent
extern bool WaterReflection;    // (th4) Config.cpp; heroes mirrored in the town river, TownWater.cpp
extern int Perspective;         // (th4) Config.cpp; used by Draw.cpp Perspective_* and Main.cpp cursor mapping
extern int FilmGrain;           // (th4) Config.cpp; used by dx_utility.cpp PostFxPresent
extern int IceFrost;            // (th4) Config.cpp; frost on the screen edges (Ice Age, Glacial Caverns, Frozen Loch), 0..10 (default 7), dx_utility.cpp Frost_Present
extern int RetroMode, RetroPixel; // (th4) Config.cpp; whole-screen retro look (default off), dx_utility.cpp Retro_Present / RetroModes[]
extern int TreeSway;            // (th4) Config.cpp; 0..10, default 4 -- dx_utility.cpp TreeSway_Present, TownWater.cpp BuildSwayLayers
extern int SunRays;             // (th4) Config.cpp; 0..10, default 2 -- dx_utility.cpp SunRays_Present (sunny days, Weather_SunQ8)
extern int SpellTrails, CloudShadows; // (th4) Config.cpp; 0..10 -- CloudShadows default 9, SpellTrails disabled (0); dx_utility.cpp ATMOSPHERE block
extern bool LevelUpGlow;        // (th4) Config.cpp; default off, dx_utility.cpp LevelUp_Present / LevelUpFx_Start
extern int WetGround;           // (th4) Config.cpp; 0..10, default 5 -- TownWater.cpp PUDDLES, dx_utility.cpp Wet_Present (Weather.cpp Weather_WetQ8 says when)
extern int ColdBreath;          // (th4) Config.cpp; 0..10, default 5 -- Breath.cpp (notes, puffs), dx_utility.cpp Breath_Present
extern int GrassBlades;         // (th4) Config.cpp; 0..10, default 10 (how tall) -- TownWater.cpp GRASS, dx_utility.cpp Grass_Present
extern int GrassResolution;     // (th4) Config.cpp; 0 window res, 1 game res, 2 half game res (default) -- dx_utility.cpp Grass_Present
extern int StartWindowX;
extern int StartWindowY;
extern bool BigWin;
extern int WinHeight;
extern int WinWidth;
extern bool IsMainDraw;
extern int InterpolationFrame;
extern int FramesMax;
extern int MainDrawCount;
extern int AllDrawCount;
extern int MaxFPS;
extern int FPS;
extern bool ShowFps;
extern bool Vsync;
extern int IsAltPressed;
extern int EasyClick;
extern bool ShiftReleasedEvent;
extern bool ShiftPressedEvent;
extern bool LButtonEvent;
extern bool RButtonEvent;
extern int OldMouseButtonPressed;
extern bool VerticalWin;
extern bool PanelClick;

extern char* Classic_Item_Sprites;
extern char* AddonItemSprites;

extern Sign Signes[10];
extern Sign MemorialSign;
extern int CurSign;
extern bool IsShowTileNum;
extern int CurFineMap;
extern bool IsInterface;
extern HWND SecWnd;
extern bool IsSecondWin;
extern uint SaveVersion;
extern char* Items_DurIconsCEL2;
extern bool RunInDungeon;
extern bool MapItem;
extern bool MapBlend;
extern bool showCombatText;
extern int FloatingHPRegenThreshold;
extern int FloatingManaRegenThreshold;
extern bool ShowHPRegen;
extern bool ShowMPRegen;
extern int ScreenCenterUserShiftVertical;
extern bool ShowBreakWarning;
extern bool showInflictedDamage;
extern bool ShowNumbersOnHealth;
extern bool ShowNumbersOnMana;
extern bool ShowReceivedDamage;
extern int ReceivedDamageThreshold;
extern bool showCombatExp;
extern bool AlwaysHighlightSummons;
extern int MonsterReceivedDamageThreshold;
extern bool ShowMonsterHealthBar;
extern bool ShowVersionOnScreen;
extern bool mergeCombatTexts;
extern int HealthRegen;
extern int ManaRegen;
extern int UpdateAuraDamage;
extern bool ShowPercentExpGain;
extern bool NeedAltHold;
extern bool AltHighlight; 
extern bool SwapSpellsOnScroll;
extern bool RedInactiveSpellIcons;
extern uchar const* virtualshader;
extern int FullscreenFX;
extern bool RandomizeSPQuests;
extern bool NpcSign;
extern bool ShowObjectsOnMiniMap;
extern bool BlueTownPortal;
extern bool BackgroundSound;
extern bool BackgroundDraw;
extern bool NewShortcuts;
extern bool SkipUnusableSpells;
extern bool MoreManualHotkeys;
extern bool InstantHotkeys;
extern bool ExpandedSpellsOnScroll;
extern bool DDraw;
extern bool NewDunConfirm;
extern bool AltOnlyItemPick;
extern bool OriginalMusic;
extern bool Doom_Music;
extern bool total_kills;	// (th4)
extern bool original_palettes;
extern bool SecondaryTooltip;
extern bool ProhibitAltHighlight;
extern bool AlternativeMonsterBarBorder;
extern bool ColoredMonsterBarBorder;
extern bool ExtraMouseButtons;
extern bool ShowMonsterStunThreshold;
extern bool DisableESCinBattle;
extern bool MonsterWalkSounds;
extern bool ShowPlayerAuraHitVisual;
extern bool ShowGameMode;
extern bool LowCpuUsage;
extern bool ForceZoom;
extern bool DynamicZoom;// (th4) mouse-wheel world zoom, DynamicZoom.cpp
extern bool ShowResolution;

extern char* Elements[ET_COUNT];
extern char* Spicies[MON_count];
extern char* Weapons[W_COUNT];
extern char* Stats[ST_count];
extern bool EnteredPanelWithAutoClick;
extern CObjectUnderCursor HighlightedItem;
extern vector<CombatText> CombatTextQueue;
extern float AspectX;
extern float AspectY;
extern Affix AffixesRare[4000];
extern int Xofs, Yofs;
extern int HeightShift;
extern vector<pair<string,string> > messageLog;
extern bool EnforceNoSave;
extern int PlayerLastDrawnHealth;
extern int PlayerLastDrawnMana;
extern bool InstantMurderHero;
extern char MurderHeroName[32];
extern int MurderHeroSlot;		// (th4) Player.cpp; exact save targeted by an armed murder
extern bool MurderHeroMulti;	// (th4) Player.cpp; true = the multiplayer hero list
extern int GriswoldRestockPrice;
extern int AdriaRestockPrice;
extern int AcidedMonsterFrameColor;
extern int AcidedHealthBarColor;
extern bool IsPerksPanelVisible;
extern bool perkUpArrowPressed;
extern bool perkDownArrowPressed;
extern int CeliaUniqsTable[6][9];
extern int AnvilOfFuryUniqsTable[PLAYER_FULL_CLASS::PFC_COUNT][9];

extern bool IsStashPanelVisible;
extern DisplayObject VisualStashPanel;

extern bool IsVisualTradePanelVisible;
extern DisplayObject VisualTradePanel;
extern VisualTradeMode CurrentVisualTradeMode;
extern VisualTradePage CurrentVisualTradePage;

extern DisplayObject QuestPanelRect;
extern DisplayObject QuestPanelTextAreaRect;

extern DisplayObject MainPanelRect;
extern DisplayObject CharPanelRect;
extern DisplayObject CharPanelNameRect;	// (th4)
extern DisplayObject CharPanelCurStrRect;
extern DisplayObject CharPanelCurMagRect;
extern DisplayObject CharPanelCurDexRect;
extern DisplayObject CharPanelCurVitRect;

extern DisplayObject CharPanelBaseStrRect;
extern DisplayObject CharPanelBaseMagRect;
extern DisplayObject CharPanelBaseDexRect;
extern DisplayObject CharPanelBaseVitRect;
extern DisplayObject CharPanelDamageRect;
extern DisplayObject CharPanelResistAcid;
extern DisplayObject CharPanelResistMagic;
extern DisplayObject CharPanelResistFire;
extern DisplayObject CharPanelResistCold;
extern DisplayObject CharPanelResistLightning;
extern DisplayObject CharPanelResistArrow;
extern DisplayObject CharPanelResistMelee;
extern DisplayObject CharPanelXp;
extern DisplayObject CharPanelXpNext;
extern DisplayObject CharPanelArmorClass;
extern DisplayObject charPanelToHit;
extern DisplayObject CharPanelLifeRegen;
extern DisplayObject CharPanelManaRegen;
extern DisplayObject CharPanelCritDmg;
extern DisplayObject CharPanelCritChance;

extern DisplayObject InventoryPanelRect;
extern DisplayObject GoldSplitPanelRect;

extern DisplayObject SpellBookRect;
extern DisplayObject SpellBookTextRect;
extern DisplayObject SpellBookPageButtonsRect;
extern DisplayObject SpellBookSpellButtonsRect;

extern bool IsCraftPanelVisible;
extern DisplayObject CraftPanel;
extern CraftPage CurrentCraftPage;

extern SpellbookHotkeysInfo SpellHotkeys;
extern AdditionalSaveInfo AdditionalStateData;
extern std::array<Item, 4>          GemCombineItems;

extern bool IsEnchantPanelVisible;
extern bool IsQuenchPanelVisible;
extern bool IsTransmutePanelVisible;
extern bool IsPowerupPanelVisible;
extern Affix AffixesEnchant[2000];
extern DisplayObject EnchantPanel;
extern DisplayObject QuenchPanel;
extern DisplayObject TransmutePanel;
extern DisplayObject PowerupPanel;
extern Item EnchantItem;
extern Item QuenchItem;
extern int EnchantsMainIndexesTable;

extern Aura Auras[1000];
extern bool AuraSoundStates[5];
extern int AuraCooldown[5];

extern std::array<std::queue<int>, 4> PlayersSpellcastQueue;
extern std::array<std::array<std::array<int, WallParamCount>, WallTypeCount>, PlayersMax_4> PlayerWall;
extern unordered_map<short, uchar> SpellIdxToWallType;

extern bool IsInfoWindowVisible;
extern DisplayObject InfoWindowRect;

extern bool IsChatHistoryVisible;
extern DisplayObject ChatHistoryRect;

extern const char* GC_Names_InfoWindow[GC_COUNT];
extern const char* GC_Descriptions_InfoWindow[GC_COUNT];

extern bool BattleTrance;
extern int ColorShifter;
extern bool TelekinesRecentlyAppliedToMonster;
extern bool Berserk;
extern int PlayerRunningSounds;
extern bool CanSwitchPerkSynergyPanel;
extern bool ShowSecondaryPerks;
extern bool RememberPerkSelection;
extern int PerkPanelLine[PERK_TYPE_COUNT];
extern PERK_TYPE CurPerkPanel;
extern bool ShowSystemTime;
extern bool ForceMoveOnCtrl;
extern bool ForceMove;
extern bool DropItemOnCtrl;
extern bool IsCtrlPressed;
extern bool KeepSpellsOnCtrls;


extern BackLightColor blColor;

extern bool InitBerserk;
extern double TimeSpeed;
extern int CurTimeSpeedIndex;

extern CharSaveInfo NewSaveInfo;
extern CharSaveInfo *CurCharSaveInfo;
extern int CharParamNameList[];
extern int CharParamValueList[];
extern int ClassDescriptionList[];
extern Pcx PcxGcs;
extern Pcx PcxTraits;
extern Pcx PcxHero_pix;

extern CXBOXController curXboxController;
extern bool XinputAutoTargettingOff;
extern int XinputAutoTargettingOffTimer;
extern bool XinputOnOFF;
extern bool IsXinputGuideVisible;
extern int XWelcomeMessageTime;
extern bool IsRestrictMouseToWindow;
extern bool PrevRestrictState;
extern bool IsSizingWindow;
extern bool XinputEnabled;
extern int MinDmgFromChar;
extern int MaxDmgFromChar;

extern void VisualTrade_ForEachPlayerItem(std::function<void(Item* item)> cb);
constexpr bool SpellAnimInTown = false;
extern bool CastColiseumPortal;
extern ITEM_SLOT LastInvSlotPicked;
extern bool FastInventoryItemSwap;
extern bool FastSwapRightOnAlt;
extern bool OriginalLightLevels;
extern bool OnScreenTextOutline;
extern bool DropRares;
extern bool RelixToScrolls;
extern bool DisplayMonstersLeft;
extern bool ShowMinionBars;
extern bool TransmuteAllowExtra;
extern bool TransmuteKeepItems;
extern bool TransmuteAllowMultiple;
extern bool TransmuteShowRecipeList;

extern const int valorRewards[VALOR_CLASSES][VALOR_DIFFS];
extern int blindRewards[HotB_UNIQUES];
extern int mundaneInventoryRewards[];

extern uint NonGameplaySeed;
extern bool Lovecraft;
extern char* SigilButtonCEL;
extern bool IsSigilPanelVisible;
extern char* SigilGridCEL;
extern char* LitheBuildCoverCEL;
// OB
extern int OutbreakState;
extern int OutbreakWave;
extern int OutbreakSpawned;
extern int OutbreakPauseTimer;
extern int OutbreakSpawnTimer;


extern bool Display_Player_HP_bars;
extern bool Display_Monster_HP_bars;

extern bool Show_Reflect_Layers;

// (th4) Stage 2: clear the alpha overlay under one WorkingSurface pixel or one
// contiguous row. UI drawn into WorkingSurface after the post-render replay
// must not have occluded-sprite RGB composited on top of it; these mirror the
// LightSurface clears the same draw paths already perform (DrawLetterPixel,
// DrawBox, OverheadBar_Row, ...). p must point into the WorkingSurface buffer
// (8 bpp: byte offset == pixel index; SpriteLayerSurface is uint* addressed by
// pixel index). Gated on the alloc condition — cheap no-op everywhere else.
// Defined here (after all externs) because engine.h is included before the
// globals are declared.
__forceinline void AlphaOverlayClearPixel(uchar* p)
{
	if (g_ARGBLighting && EnrichedLightUltraAlpha != 0 && AlphaSurface && SpriteLayerSurface && p >= SoftSrcSurfacePtr) {
		const ptrdiff_t pix = p - SoftSrcSurfacePtr;
		AlphaSurface[pix] = 0;
		SpriteLayerSurface[pix] = 0;
	}
}

__forceinline void AlphaOverlayClearRow(uchar* p, int width)
{
	if (g_ARGBLighting && EnrichedLightUltraAlpha != 0 && AlphaSurface && SpriteLayerSurface && p >= SoftSrcSurfacePtr) {
		const ptrdiff_t pix = p - SoftSrcSurfacePtr;
		memset(AlphaSurface + pix, 0, width);
		memset(SpriteLayerSurface + pix, 0, (size_t)width * sizeof(uint));
	}
}

// (th4) Stage 3: clear the colored-light tint word under one WorkingSurface pixel
// or one contiguous row — the Stage-3 analog of AlphaOverlayClearPixel/Row. UI
// drawn into WorkingSurface after the V2 fill must never be multiplied by the
// world's winning light color, so the tint word is reset to TINT_WHITE (32-bit
// buffer, unlike LightSurface's 8-bit 0). Placed next to the LightSurface clears
// the same draw paths already perform (DrawLetterPixel, DrawBox, OverheadBar_Row,
// Surface_DrawCEL's R_TINT_CLEAR pass, ...). Idle-gated on g_LightTintActive so
// frames without a colored light pay nothing.
__forceinline void TintClearPixel(uchar* p)
{
	if (g_ARGBLighting && g_LightTintActive && LightTintSurface && p >= SoftSrcSurfacePtr) {
		LightTintSurface[p - SoftSrcSurfacePtr] = TINT_WHITE;
	}
}

__forceinline void TintClearRow(uchar* p, int width)
{
	if (g_ARGBLighting && g_LightTintActive && LightTintSurface && p >= SoftSrcSurfacePtr) {
		uint* t = LightTintSurface + (p - SoftSrcSurfacePtr);
		for (int x = 0; x < width; ++x) t[x] = TINT_WHITE;
	}
}
