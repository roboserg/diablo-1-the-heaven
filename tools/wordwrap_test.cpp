// wordwrap_test.cpp
// Simulates DrawPlayerDescriptionText() from src/Player.cpp using the
// actual FontIndexSmall, FontWidthSmall, and Codepage tables from dataseg.cpp.
//
// Purpose: Verify the word-wrap hard-break fix by tracing through the algorithm
// with realistic test inputs and comparing output to expected rendering.
//
// Compile: cl /EHsc /W3 wordwrap_test.cpp  (or g++ -std=c++17 wordwrap_test.cpp)
// Run:     wordwrap_test.exe

#include <cstdio>
#include <cstring>
#include <cassert>
#include <string>
#include <vector>

// ──────────────────────────────────────────────────────────────────────────────
// Exact tables from d:\th4\src\dataseg.cpp (default namespace, line 2748-2821)
// ──────────────────────────────────────────────────────────────────────────────

static unsigned char FontIndexSmall[128] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0, 54, 44, 57, 58, 56, 55, 47, 40, 41, 59, 39, 50, 37, 51, 52,
    36, 27, 28, 29, 30, 31, 32, 33, 34, 35, 48, 49, 60, 38, 61, 53,
    62,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 42, 63, 43, 64, 65,
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 40, 66, 41, 67,  0
};

static unsigned char FontWidthSmall[68] = {
     8, 10,  7,  9,  8,  7,  6,  8,  8,  3,  3,  8,  6, 11,  9, 10,
     6,  9,  9,  6,  9, 11, 10, 13, 10, 11,  7,  5,  7,  7,  8,  7,
     7,  7,  7,  7, 10,  4,  5,  6,  3,  3,  4,  3,  6,  6,  3,  3,
     3,  3,  3,  2,  7,  6,  3, 10, 10,  6,  6,  7,  4,  4,  9,  6,
     6, 12,  3,  7
};

static unsigned char Codepage[256] = {
    0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
   32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
   48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
   64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
   80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
   96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111,
  112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,  1,
   67,117,101, 97, 97, 97, 97, 99,101,101,101,105,105,105, 65, 65,
   69, 97, 65,111,111,111,117,117,121, 79, 85, 99, 76, 89, 80,102,
   97,105,111,117,110, 78, 97,111, 63,  1,  1,  1,  1, 33, 60, 62,
  111, 43, 50, 51, 39,117, 80, 46, 44, 49, 48, 62,  1,  1,  1, 63,
   65, 65, 65, 65, 65, 65, 65, 67, 69, 69, 69, 69, 73, 73, 73, 73,
   68, 78, 79, 79, 79, 79, 79, 88, 48, 85, 85, 85, 85, 89, 98, 66,
   97, 97, 97, 97, 97, 97, 97, 99,101,101,101,101,105,105,105,105,
  111,110,111,111,111,111,111, 47, 48,117,117,117,117,121, 98,121
};

// ──────────────────────────────────────────────────────────────────────────────
// Helper: pixel cost of one character (matches v7 += FontWidthSmall[v12] + 1)
// ──────────────────────────────────────────────────────────────────────────────
static int charCost(unsigned char c) {
    unsigned char cp  = Codepage[c];
    unsigned char idx = (cp < 128) ? FontIndexSmall[cp] : 0;
    return FontWidthSmall[idx] + 1;
}
static bool isWordBreak(unsigned char c) {
    unsigned char cp  = Codepage[c];
    unsigned char idx = (cp < 128) ? FontIndexSmall[cp] : 0;
    return idx == 0;            // same condition as "v12 == 0" in the original
}

// ──────────────────────────────────────────────────────────────────────────────
// Simulate DrawPlayerDescriptionText with a given fix strategy:
//   fix == 0 → ORIGINAL BUG  (no fix)
//   fix == 1 → WRONG FIX     (str = v5 + 1, the session-1 fix)
//   fix == 2 → CORRECT FIX   (str = v8 - 1)
//
// Returns vector of up to 3 rendered row strings.
// ──────────────────────────────────────────────────────────────────────────────
static std::vector<std::string>
simulate(const char* sstr, unsigned int width, int fix)
{
    std::vector<std::string> rows;

    const char* v5 = sstr;         // draw head (current position)
    int v14 = 0;                    // row counter

    if (!*v5) return rows;

    do {
        const char* str = v5;       // word-break sentinel (reset each row)
        const char* v8  = v5;       // scanner
        unsigned int v7 = 0;        // accumulated pixel width

        bool overflow = false;
        while (true) {
            unsigned char v10 = (unsigned char)*v8;
            if (!v10) break;        // end of string
            ++v8;
            unsigned char v12 = FontIndexSmall[Codepage[v10]];
            v7 += FontWidthSmall[v12] + 1;
            if (v12) {
                if (v7 >= width) { overflow = true; break; }  // goto LABEL_10
            } else {
                str = v8;           // word-break: str = char after space
            }
        }
        if (!overflow) str = v8;    // normal end-of-string

        // LABEL_10:
        if (str == v5 && *v5) {
            // Hard-break fallback (str stalled at row start with no word-break)
            if      (fix == 0) { /* no fix: str stays == v5, draw nothing */  }
            else if (fix == 1) { str = v5 + 1;   }   // WRONG: draw 1 char
            else if (fix == 2) { str = v8 - 1;   }   // CORRECT: draw all that fit
        }

        // Draw loop: collect chars v5..str-1
        std::string rowText;
        const char* p = v5;
        while (p < str) {
            rowText += *p;
            ++p;
        }
        rows.push_back(rowText);
        v5 = str;

        ++v14;
    } while (v14 != 3 && *v5);

    return rows;
}

// ──────────────────────────────────────────────────────────────────────────────
// Helpers
// ──────────────────────────────────────────────────────────────────────────────
static int pixelWidth(const char* s) {
    int w = 0;
    while (*s) w += charCost((unsigned char)*s++);
    return w;
}

struct TestCase {
    const char* label;
    const char* input;
    unsigned int width;  // th4 uses 620 for status messages
};

// ──────────────────────────────────────────────────────────────────────────────
// Run one test case, printing results for all three strategies
// ──────────────────────────────────────────────────────────────────────────────
static bool runTest(const TestCase& tc)
{
    printf("══════════════════════════════════════════════════════════════════\n");
    printf("TEST: %s\n", tc.label);
    printf("Input (%zu chars, ~%dpx): \"%s\"\n",
           strlen(tc.input), pixelWidth(tc.input), tc.input);
    printf("Width limit: %u px\n", tc.width);
    printf("──────────────────────────────────────────────────────────────────\n");

    const char* fixName[] = { "ORIGINAL BUG", "WRONG FIX (v5+1)", "CORRECT FIX (v8-1)" };
    bool pass = true;

    for (int fix = 0; fix <= 2; ++fix) {
        auto rows = simulate(tc.input, tc.width, fix);
        int totalChars = 0;
        printf("[%s]\n", fixName[fix]);
        for (int r = 0; r < (int)rows.size(); ++r) {
            int pw = pixelWidth(rows[r].c_str());
            printf("  Row %d (%3dpx, %3zu chars): \"%s\"\n",
                   r+1, pw, rows[r].size(), rows[r].c_str());
            totalChars += (int)rows[r].size();
        }
        // Verify: total characters rendered should == total input chars
        // Empty strings and space-only strings count as "pass" since there's
        // nothing to lose; the simulator correctly renders 0 visible glyphs.
        int inputLen = (int)strlen(tc.input);
        bool allShown = (totalChars == inputLen) || (inputLen == 0);
        // Count non-space input chars: for space-only inputs any result is fine
        int nonSpaceInput = 0;
        for(const char* p = tc.input; *p; ++p) if(*p != ' ') ++nonSpaceInput;
        int nonSpaceRendered = 0;
        for(int ri = 0; ri < (int)rows.size(); ++ri)
            for(char ch : rows[ri]) if(ch != ' ') ++nonSpaceRendered;
        bool passCheck = (fix == 2) ? (nonSpaceInput == 0 || nonSpaceRendered == nonSpaceInput) : true;
        printf("  Chars rendered: %d / %d  %s\n",
               totalChars, inputLen, allShown ? "✓ ALL" : (passCheck ? "~ SPACE-SKIP" : "✗ MISSING"));
        if (fix == 2 && !passCheck) pass = false;
        printf("\n");
    }

    return pass;
}

// ──────────────────────────────────────────────────────────────────────────────
// Main
// ──────────────────────────────────────────────────────────────────────────────
int main()
{
    // ── Per-character pixel cost reference ──────────────────────────────────
    printf("Character pixel costs (+1 per char for kern gap):\n");
    const char* chars = " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.:()";
    for (const char* p = chars; *p; ++p) {
        unsigned char c = (unsigned char)*p;
        unsigned char cp  = Codepage[c];
        unsigned char idx = (cp < 128) ? FontIndexSmall[cp] : 0;
        printf("  '%c' (ASCII %3d): Codepage=%3d FontIndex=%2d Width=%2d CostPerStep=%d isWordBreak=%s\n",
               c, c, cp, idx, FontWidthSmall[idx], FontWidthSmall[idx]+1,
               isWordBreak(c) ? "YES" : "no");
    }
    printf("\n");

    // ── Prefix pixel width ───────────────────────────────────────────────────
    const char* prefix = "Jazreth (lvl 0): ";
    printf("Prefix: \"%s\"\n", prefix);
    printf("Prefix pixel width: %dpx\n", pixelWidth(prefix));
    printf("'9' cost per step: %d px\n", charCost('9'));
    printf("'0' cost per step: %d px\n", charCost('0'));
    {
        int remaining = 620 - pixelWidth(prefix);
        printf("Remaining width after prefix on row1: %dpx  → fits %d '9's or %d '0's\n",
               remaining, remaining / charCost('9'), remaining / charCost('0'));
    }
    printf("\n");

    // ── How many chars per clean row (no prefix) ─────────────────────────────
    {
        int n9 = 0, w = 0; while (w + charCost('9') < 620) { w += charCost('9'); ++n9; }
        int n0 = 0; w = 0; while (w + charCost('0') < 620) { w += charCost('0'); ++n0; }
        printf("Clean row capacity: %d × '9' (%dpx) or %d × '0' (%dpx)\n\n",
               n9, n9*charCost('9'), n0, n0*charCost('0'));
    }

    // ── Test cases ───────────────────────────────────────────────────────────
    // Build test strings
    char nines78[80];  memset(nines78, '9', 78); nines78[78] = 0;
    char zeros78[80];  memset(zeros78, '0', 78); zeros78[78] = 0;
    char mixed78[80];  // 30 nines + 18 zeros + 30 nines
    memset(mixed78, '9', 30); memset(mixed78+30, '0', 18); memset(mixed78+48, '9', 30);
    mixed78[78] = 0;

    // 77 nines = exactly one full row (77×8=616 < 620)
    char nines77[80];  memset(nines77, '9', 77); nines77[77] = 0;
    // 56 zeros = exactly one full row (56×11=616 < 620)
    char zeros56[60];  memset(zeros56, '0', 56); zeros56[56] = 0;
    // 100 zeros raw (no prefix) — hard-break twice
    char zeros100[110]; memset(zeros100, '0', 100); zeros100[100] = 0;
    // 129 As raw
    char aaa129[132]; memset(aaa129, 'A', 129); aaa129[129] = 0;
    // 57 As raw (57×11=627 ≥ 620, so 56 fit then hard-break)
    char aaa57[60]; memset(aaa57, 'A', 57); aaa57[57] = 0;
    // 200 digits alternating 1234567890
    char digits200[210];
    for(int k=0;k<200;k++) digits200[k]='0'+(k%10); digits200[200]=0;
    // spaces only
    char spaces20[30]; memset(spaces20,' ',20); spaces20[20]=0;

    // Formatted messages (as AddNewPlayerDescriptionSatusMessage would produce)
    char msg1[200]; snprintf(msg1, sizeof(msg1), "Jazreth (lvl 0): %s", nines78);
    char msg2[200]; snprintf(msg2, sizeof(msg2), "Jazreth (lvl 0): %s", zeros78);
    char msg3[200]; snprintf(msg3, sizeof(msg3), "Jazreth (lvl 0): %s", mixed78);
    char msg4[160]; snprintf(msg4, sizeof(msg4), "Jazreth (lvl 0): Hello world");
    char msg5[160]; snprintf(msg5, sizeof(msg5), "Jazreth (lvl 0): Short msg");
    char msgA[160]; snprintf(msgA, sizeof(msgA), "Jazreth (lvl 0): %s", aaa57);
    char msgB[200]; snprintf(msgB, sizeof(msgB), "Jazreth (lvl 0): %s", zeros100);
    char msgC[160]; snprintf(msgC, sizeof(msgC), "Jazreth (lvl 30): gg wp ez");
    char msgD[160]; snprintf(msgD, sizeof(msgD), "Bob (lvl 99): I need help here omg this is insane");
    char msgE[160]; snprintf(msgE, sizeof(msgE), "Jazreth (lvl 0): hey can someone help me with this quest? its really hard");
    char msgF[160]; snprintf(msgF, sizeof(msgF), "Jazreth (lvl 0): AAAAAAAAAA AAAAAAAAAA AAAAAAAAAA AAAAAAAAAA AAAAAAAAAA AAAAAAAAAA");
    char msgG[160]; snprintf(msgG, sizeof(msgG), "Jazreth (lvl 0): The quick brown fox jumps over the lazy dog");
    char msgH[160]; snprintf(msgH, sizeof(msgH), "Jazreth (lvl 0): word word word word word word word word word word");
    char msgI[160]; snprintf(msgI, sizeof(msgI), "Jazreth (lvl 0): !@#$%%^&*()");
    char msgJ[160]; snprintf(msgJ, sizeof(msgJ), "Jazreth (lvl 0): Hello, world. How are you? Fine! Thanks...");
    char msgK[160]; snprintf(msgK, sizeof(msgK), "Jazreth (lvl 0): 0123456789 0123456789 0123456789 0123456789 0123456789 01");
    char msgL[160]; snprintf(msgL, sizeof(msgL), "Jazreth (lvl 0): ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    char msgM[160]; snprintf(msgM, sizeof(msgM), "Jazreth (lvl 0): abcdefghijklmnopqrstuvwxyz");
    char msgN[160]; snprintf(msgN, sizeof(msgN), "Al (lvl 1): Hi");
    char msgO[160]; snprintf(msgO, sizeof(msgO), "Jazreth (lvl 0): WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"); // 55 Ws = 55×14=770 > 620, hard-break
    char msgP[160]; snprintf(msgP, sizeof(msgP), "Jazreth (lvl 0): iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii"); // 81 'i's (4px each)
    // 'i' costs 4, so 620/4=155 per row: all 81 fit in row 2 (prefix forces word-wrap)
    char msgQ[160]; snprintf(msgQ, sizeof(msgQ), "Jazreth (lvl 0): one two three four five six seven eight");
    char msgR[160]; snprintf(msgR, sizeof(msgR), "Jazreth (lvl 0): verylongwordthathasnospacesinit verylongwordthathasnospacesinit");
    char msgS[160]; snprintf(msgS, sizeof(msgS), "Jazreth (lvl 0): +++ --- *** ... ,,, ;;; ::: ''' \"\"\"");
    char msgT[160]; snprintf(msgT, sizeof(msgT), "Jazreth (lvl 0): %s", digits200);

    TestCase tests[] = {
        // ── Original 7 ──────────────────────────────────────────────────────
        { "01: 78 nines with prefix (user report #1, primary symptom)",
          msg1, 620 },
        { "02: 78 zeros with prefix (user asks: is 0 treated as EOL?)",
          msg2, 620 },
        { "03: 30 nines + 18 zeros + 30 nines with prefix (user report #2)",
          msg3, 620 },
        { "04: Normal message with word-wrap across space (regression)",
          msg4, 620 },
        { "05: Short message fits on row 1 (sanity check)",
          msg5, 620 },
        { "06: 129 'A' raw, no spaces (extreme unbreakable word, 3 rows needed)",
          aaa129, 620 },
        { "07: Single widest char 'W' (should render fine, 14px)",
          "W", 620 },
        // ── Digit tests ─────────────────────────────────────────────────────
        { "08: 77 nines raw (exactly one full row, 77×8=616px)",
          nines77, 620 },
        { "09: 56 zeros raw (exactly one full row, 56×11=616px)",
          zeros56, 620 },
        { "10: 100 zeros raw, no prefix (hard-break at 56+56=112, row3 gets 44)",
          zeros100, 620 },
        { "11: 57 'A' with prefix (56 fit on row 2, 1 'A' on row 3)",
          msgA, 620 },
        { "12: 200 alternating 0-9 digits with prefix (3 rows hard-break)",
          msgT, 620 },
        // ── Letter tests ────────────────────────────────────────────────────
        { "13: 26 uppercase letters ABCDE... no spaces (one long word, row 2)",
          msgL, 620 },
        { "14: 26 lowercase letters abcde... no spaces (same cost as uppercase)",
          msgM, 620 },
        { "15: 55 'W' with prefix (widest char, 55×14=770 > 620, hard-break)",
          msgO, 620 },
        { "16: 81 'i' with prefix ('i'=4px, 155 fit per row; all in row 2)",
          msgP, 620 },
        // ── Real English messages ────────────────────────────────────────────
        { "17: Short chat message — friendly player, lvl 30",
          msgC, 620 },
        { "18: Typical help request — short player name, lvl 99",
          msgD, 620 },
        { "19: Longer chat message with question mark",
          msgE, 620 },
        { "20: Repeated capitalized word blocks (many space word-breaks)",
          msgF, 620 },
        { "21: The quick brown fox sentence (pangram — all letters)",
          msgG, 620 },
        { "22: Repeated word 'word' ten times",
          msgH, 620 },
        { "23: Multi-word with various word lengths",
          msgQ, 620 },
        { "24: Two long words separated by space (each word > half row width)",
          msgR, 620 },
        // ── Special character tests ──────────────────────────────────────────
        { "25: Special chars !@#$%^&*()",
          msgI, 620 },
        { "26: Punctuation — commas, periods, question mark, exclamation",
          msgJ, 620 },
        { "27: Digit groups separated by spaces",
          msgK, 620 },
        { "28: Mixed punctuation +-- *** ... ,,, ;;; ::: ' \"\"\"",
          msgS, 620 },
        // ── Edge / boundary tests ────────────────────────────────────────────
        { "29: Very short message, short name (minimal prefix)",
          msgN, 620 },
        { "30: Single space only",
          " ", 620 },
        { "31: Two-char string 'AZ'",
          "AZ", 620 },
        { "32: Empty string (should render 0 rows)",
          "", 620 },
        { "33: 20 spaces raw (all word-breaks, nothing to draw per normal flow)",
          spaces20, 620 },
        { "34: Single 'i' (narrowest letter, 4px)",
          "i", 620 },
        { "35: String that is exactly the full width prefix with no message body",
          "Jazreth (lvl 0): ", 620 },
    };

    int passed = 0, total = (int)(sizeof(tests)/sizeof(tests[0]));
    for (auto& tc : tests) {
        if (runTest(tc)) ++passed;
    }

    printf("══════════════════════════════════════════════════════════════════\n");
    printf("SUMMARY: %d / %d test cases render all input chars with correct fix.\n", passed, total);
    printf("\n");
    printf("(Note: empty string and spaces-only inputs are counted as pass since\n");
    printf(" there are no non-space chars to lose.)\n");
    printf("\n");
    printf("Analysis of the hard-break formula:\n");
    printf("  At goto LABEL_10 with str==v5:\n");
    printf("    v8 has been incremented PAST the overflow char.\n");
    printf("    v8-1 = the overflow char (too wide to fit on this row).\n");
    printf("    v8-2 = last char that fits (was included in the < width check).\n");
    printf("    str = v8-1  =>  draw loop  while(v5 < str)  draws chars [v5 .. v8-2].\n");
    printf("    v5 then advances to v8-1 (overflow char starts the next row).\n");
    printf("    This guarantees at least 1 char progress per row (impossible for a\n");
    printf("    single char to exceed width=620 with the small font used here).\n");
    printf("\n");
    printf("Why str = v5+1 was WRONG:\n");
    printf("    It draws only 1 character per hard-break row regardless of how\n");
    printf("    many characters actually fit (typically 70-77 for the small font).\n");
    printf("    A 78-char message of '9's produces: prefix / 1 nine / 1 nine.\n");
    printf("    The missing chars are simply discarded after 3 rows.\n");

    return 0;
}
