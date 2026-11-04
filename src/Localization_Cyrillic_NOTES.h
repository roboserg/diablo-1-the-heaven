#pragma once
// =============================================================================
//  CYRILLIC LOCALIZATION — QUICKSTART BREADCRUMB  (The Hell 4)
//  Author of notes: Claude, for Mordor. Safe to read first in any new chat.
//  This header is documentation + ready-to-use mapping arrays. Including it is
//  optional; its main job is to let a fresh session skip re-analyzing fonts.
// =============================================================================
//
//  CONTEXT (what was already figured out)
//  --------------------------------------
//  * Localization string framework already works (see Config.cpp:
//      GameLanguage, MonsterNameLoc[BM_COUNT][LANG_COUNT], InitLocalization(),
//      LocalizedMonsterName()). Monster names translate in-game today via
//      config.ini "set language 1". English (LANG_EN=0) is the fallback.
//    Confirmed-working checkpoint: "Shazam 01".
//
//  TWO FONT PIPELINES (different Cyrillic requirements!)
//  ----------------------------------------------------
//  A) IN-GAME text (monster/item names, tooltips, on-screen msgs):
//        byte -> Codepage[256] -> FontIndexSmall/Mid/Big[128] -> glyph in
//        CtrlPan\SmalText.CEL.  Decoded by RleDraw() (engine.h) / Font_DrawChar.
//        Fonts are CAPS-ONLY (no lowercase glyphs exist in this game/mod).
//  B) MENU/dialog text (class/trait/GC names+desc, difficulty, errors):
//        Loaded by LoadFont() from ui_art\font16/24/30/42.bin + .pcx.
//        DiabloFont has charWidth[256] + trans[256], indexed DIRECTLY by raw
//        byte (LoadFont iterates letterIndex<=256). => menu Cyrillic needs
//        ONLY art (glyphs at 0xC0..0xFF in the .pcx + widths in .bin), NO code.
//
//  CEL FORMAT (so you don't re-derive it)
//  --------------------------------------
//  Header: [uint32 frameCount][uint32 offset_0]..[uint32 offset_n], then data.
//  Each glyph frame here = 13x11 cell, rows stored BOTTOM-to-TOP, RLE:
//    signed byte len; len>0 => len opaque pixels (len palette bytes follow);
//    len<0 => (-len) transparent pixels; width resets each row (width=13).
//    (Optional 10/12-byte pixel-offset header may prefix a frame; skip *cel.)
//
//  *** CRITICAL OFF-BY-ONE (cost us a debug cycle) ***
//  DrawLetter reads FontSpriteSmall[4*letterID] as the StartOffset. Because the
//  CEL begins with the 4-byte frameCount, GAME letterID L addresses standard-CEL
//  frame (L-1). i.e. the value you put in FontIndexSmall is letterID = stdFrame+1.
//  When you visually identify a glyph as "standard frame N", its FontIndexSmall
//  value must be N+1. The original Latin table is already letterID-based
//  ('A'->1 == std frame 0). So: reused Latin/digit entries are correct as-is, but
//  any NEW glyph you read off a contact sheet (0-based) needs +1.
//
//  THE FONT WE HAVE: smaltext_mod_01.cel  (84 frames; stock SmalText had 68)
//  ----------------------------------------------------------------------------
//  STATUS: DONE & VERIFIED IN-GAME (РАЗВЯЗЫВАТЕЛЬ / ЗОМБИ / СКЕЛЕТ render correctly).
//
//  TERMINOLOGY (keep these straight — this was the bug):
//    * "std frame"   = 0-based frame index in the CEL file / on a contact sheet.
//    * "game letterID" = what FontIndexSmall stores; = std frame + 1 (see off-by-one
//      above). DrawLetter does FontSpriteSmall[4*letterID]; the leading 4-byte
//      frameCount makes letterID L address std frame (L-1).
//
//  STD FRAMES in smaltext_mod_01.cel (what you SEE on a contact sheet):
//    std 1..26  = Latin A..Z      (but note: letterID for 'A' is 1 == std frame 0!)
//    std 27..66 = digits + punctuation/symbols
//    std 67..83 = 17 DEDICATED CYRILLIC CAPITALS:
//        67 Б  68 Г  69 Д  70 Ж  71 И  72 Л  73 П  74 Ф  75 Ц
//        76 Ч  77 Ш  78 Щ  79 Ь  80 Ы  81 Э  82 Ю  83 Я
//    => their GAME letterIDs are 68..84 (std+1).  Я = letterID 84 (the max).
//
//  MEASURED PIXEL WIDTHS of the 17 glyphs (raw extent; NOT the advance), by letter:
//      Б=7 Г=6 Д=13 Ж=13 И=9 Л=10 П=7 Ф=10 Ц=8 Ч=7 Ш=11 Щ=12 Ь=6 Ы=10
//      Э=8 Ю=11 Я=9
//  FontWidthSmall stores an ADVANCE width = pixel + ~3 (kern); 'A' pixel=7 but
//  FontWidthSmall[1]=10. Use (pixel+3), then eyeball-tune. Width is indexed by
//  game letterID, so Cyrillic widths live at FontWidthSmall[68..84].
//  (In-game these looked slightly loose/sloppy; fine-tune either the CEL art or
//   these 17 advance values.)
//
//  COVERAGE: 30 of 33 Russian letters render with THIS cel, no new art.
//  CP1251 BYTE -> GAME letterID (uppercase 0xC0..0xDF; lowercase 0xE0..0xFF same;
//  Ё at 0xA8). These are the ACTUAL values now in FontIndexSmall_Cyrillic:
//  ----------------------------------------------------------------------------
//   0xC0 А 1    0xC1 Б 68   0xC2 В 2    0xC3 Г 69   0xC4 Д 70   0xC5 Е 5
//   0xC6 Ж 71   0xC7 З 29   0xC8 И 72   0xC9 Й 72(И fb)  0xCA К 11  0xCB Л 73
//   0xCC М 13   0xCD Н 8    0xCE О 15   0xCF П 74   0xD0 Р 16   0xD1 С 3
//   0xD2 Т 20   0xD3 У 25   0xD4 Ф 75   0xD5 Х 24   0xD6 Ц 76   0xD7 Ч 77
//   0xD8 Ш 78   0xD9 Щ 79   0xDA Ъ 80(Ь fb)  0xDB Ы 81   0xDC Ь 80   0xDD Э 82
//   0xDE Ю 83   0xDF Я 84   0xA8 Ё 5(Е fb)
//  Notes on the 13 non-dedicated letters (reuse existing letterIDs, NO +1 needed,
//  they were already letterID-based in the original table):
//    А1 В2 Е5 К11 М13 Н8 О15 Р16 С3 Т20 Х24 У25 ; and З->29 (the digit '3' glyph).
//  Fallbacks: Ё->Е(5), Й->И(72), Ъ->Ь(80).
//
//  IMPLEMENTATION (all of this is ALREADY DONE in the tree):
//  ----------------------------------------------------------------------------
//   1. Cyrillic CEL kept ALONGSIDE the English one in the datafile (ctrlpan\):
//        english -> smaltext.cel ; russian -> smaltext_mod_01.cel.
//      Panel.cpp MayBeViewInit() loads by GameLanguage (LoadFile is case-insensitive).
//   2. dataseg.cpp: FontWidthSmall extended to [85] (indices 0..84; Cyrillic 68..84).
//   3. dataseg.cpp: per-script tables FontIndexSmall_{Latin,Cyrillic}[256] and
//      Codepage_{Latin,Cyrillic}[256]; active ones are pointers (FontIndexSmall is
//      uchar*, Codepage is CodepagePtr with operator[](unsigned char) to fix the
//      latent signed-char OOB at ~26 reader sites). Defaults = Latin.
//   4. Codepage_Cyrillic upper half is IDENTITY (passthrough); the byte->letterID
//      map lives in FontIndexSmall_Cyrillic per the table above.
//   5. Config.cpp InitLocalization() selects script by GameLanguage and holds the
//      Russian strings as CP1251 \xNN escapes (e.g. "\xC7\xEE\xEC\xE1\xE8" = Зомби).
//
//  FUTURE: a dozen languages -> ~3-4 glyph sets (Latin-ext / Cyrillic / Greek).
//  Each script = one more FontIndexSmall_*/Codepage_* table + one case in
//  InitLocalization(). The string layer (MonsterNameLoc etc.) is encoding-agnostic.
// =============================================================================

// Optional ready-to-use data. Values are GAME letterIDs (std frame + 1), matching
// what FontIndexSmall stores. Not wired to anything; reference only.
#ifdef CYRILLIC_NOTES_PROVIDE_TABLES
static const int Cyr_DedicatedLetterIdFirst = 68; // Б (std frame 67 + 1)
// index by (cp1251_byte - 0xC0) for А..Я. (Й/Ъ already resolved to fallbacks.)
static const short Cp1251UpperToLetterId[32] = {
    //  А   Б   В   Г   Д   Е   Ж   З   И   Й   К   Л   М   Н   О   П
        1, 68,  2, 69, 70,  5, 71, 29, 72, 72, 11, 73, 13,  8, 15, 74,
        //  Р   С   Т   У   Ф   Х   Ц   Ч   Ш   Щ   Ъ   Ы   Ь   Э   Ю   Я
           16,  3, 20, 25, 75, 24, 76, 77, 78, 79, 80, 81, 80, 82, 83, 84
};
// advance widths for dedicated letterIDs 68..84 (pixelWidth+3; tune as needed)
static const unsigned char Cyr_AdvanceWidth_68_84[17] = {
    //  Б   Г   Д   Ж   И   Л   П   Ф   Ц   Ч   Ш   Щ   Ь   Ы   Э   Ю   Я
       10,  9, 16, 16, 12, 13, 10, 13, 11, 10, 14, 15,  9, 13, 11, 14, 12
};
#endif
