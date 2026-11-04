// light_save_identity.cpp — identity/safety check for the Rev 2.50 light-pool
// save/load code (LightIndexes 32→128 + v30 full-fidelity block + v29 rebuild).
//
// Mirrors, verbatim, the game's pool semantics:
//   InitLighting (Lights.cpp): identity fill, LightsCount = 0.
//   AddLight (Lights.cpp): hands out LightIndexes[LightsCount], count++.
//   ProcessLightList retire (Lights.cpp): swap-to-tail of off lights.
//   SaveGame v30 (Saving.cpp): count + full 128-byte LightIndexes + all
//     LightsCount Light payloads.
//   LoadGame v30 (Saving.cpp): count (negative->0, clamped 128) + 128 bytes +
//     repair into a permutation (F1) + exactly one payload per active entry.
//   LoadGame v29 new (Saving.cpp): count (negative->0, clamped 32) + 32 bytes +
//     repair of the front and reconstruction of the tail from the values
//     MISSING in the front block (malformed/duplicate bytes replaced by missing
//     values) + exactly one payload per active entry.
//   LoadGame v29 old (pre-fix reference): identity fill of slots 32..127.
//
// Checks:
//   T1 V30 round trip: churned 128-cap state -> save -> load -> byte-identical
//      indexes, count, and every active slot's payload (missile/monster
//      LightIndex references keep resolving to the right slot).
//   T2a clean legacy (old 32-cap builds): v29 save -> NEW loader must equal the
//      OLD loader byte-for-byte (genuine old saves load unchanged).
//   T2b dirty legacy (buggy 128-cap v29 saves): when the front-32 block holds a
//      value >= 32, the OLD loader must violate the permutation invariant and
//      the NEW loader must restore it (self-heal) with the saved prefix/payloads
//      intact.
//   T3 corrupt-block fuzz: random 32-byte blocks (incl. bytes >= 128 and heavy
//      duplicates) -> NEW loader must always produce a valid permutation, never
//      write out of bounds (stack canaries around every array).
//   T4 snowball: dirty legacy load -> further churn -> v30 save/load -> still a
//      valid permutation with consistent payloads.
//   T5 v30 corrupt-block fuzz: random/duplicate/invalid (>=128) index bytes and
//      negative/oversized counts -> LoadV30 must repair the pool to a
//      permutation and consume EXACTLY one payload per active entry. (A skipped
//      payload desyncs the whole stream in the game — the F1 bug — and is
//      undetectable without the exact-consumption asserts added by this rev.)
//
// Build (from c:\th4\tools):
//   cmd /c 'call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x86 >nul 2>&1 & cl /nologo /EHsc /O2 light_save_identity.cpp'
//
#include <cstdio>
#include <cstdlib>
#include <cstring>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

static const int Lights_32_Count  = 32;
static const int Lights_128_Count = 128;

// --- game state mirror ----------------------------------------------------
// Stack canaries on both sides of every array: any out-of-bounds write by the
// loaders (the failure mode this test exists to catch) smashes a canary.
struct LightPayload { int row; int col; int radius; int off; };

static unsigned long long canaryIdx0 = 0x0123456789ABCDEFull;
static char        g_Indexes[Lights_128_Count];
static unsigned long long canaryIdx1 = 0x0FEDCBA987654321ull;

static unsigned long long canaryList0 = 0xA5A5A5A55A5A5A5Aull;
static LightPayload g_List[Lights_128_Count];
static unsigned long long canaryList1 = 0x5A5A5A5AA5A5A5A5ull;

static int g_Count = 0;

static bool canariesOk()
{
	return canaryIdx0 == 0x0123456789ABCDEFull && canaryIdx1 == 0x0FEDCBA987654321ull
	    && canaryList0 == 0xA5A5A5A55A5A5A5Aull && canaryList1 == 0x5A5A5A5AA5A5A5A5ull;
}

// --- pool semantics, verbatim from Lights.cpp -----------------------------
static void InitLighting()
{
	g_Count = 0;
	for (int i = 0; i < Lights_128_Count; i++) g_Indexes[i] = (char)i;
}

static int AddLight(int row, int col, int radius)
{
	int lightIndex = -1;
	if (g_Count < Lights_128_Count) {
		lightIndex = (uchar)g_Indexes[g_Count];
		g_Count++;
		LightPayload& L = g_List[lightIndex];
		L.row = row; L.col = col; L.radius = radius; L.off = 0;
	}
	return lightIndex;
}

static void LightOff(int lightIndex)
{
	if (lightIndex != -1) g_List[lightIndex].off = 1;
}

// ProcessLightList retire loop, verbatim (the swap-to-tail that keeps the
// whole 0..127 array a permutation of the slot numbers).
static void ProcessLightList()
{
	int l = 0;
	while (l < g_Count) {
		if (g_List[(uchar)g_Indexes[l]].off) {
			g_Count--;
			char removedLight = g_Indexes[g_Count];
			g_Indexes[g_Count] = g_Indexes[l];
			g_Indexes[l] = removedLight;
		} else {
			l++;
		}
	}
}

// --- save/load writers and readers (Saving.cpp ports) ---------------------
static void Write32(uchar* buf, int& p, uint v) { memcpy(buf + p, &v, 4); p += 4; }
static uint Read32(const uchar* buf, int& p) { uint v; memcpy(&v, buf + p, 4); p += 4; return v; }

static void WritePayload(uchar* buf, int& p, const LightPayload& L)
{
	memcpy(buf + p, &L, sizeof(L)); p += sizeof(L);
}
static void ReadPayload(const uchar* buf, int& p, LightPayload& L)
{
	memcpy(&L, buf + p, sizeof(L)); p += sizeof(L);
}

// SaveGame v30: FULL pool (count + 128-byte block + every live payload).
static int SaveV30(uchar* buf)
{
	int p = 0;
	Write32(buf, p, (uint)g_Count);
	for (int i = 0; i < Lights_128_Count; i++) buf[p++] = (uchar)g_Indexes[i];
	for (int i = 0; i < g_Count; i++) WritePayload(buf, p, g_List[(uchar)g_Indexes[i]]);
	return p;
}

// SaveGame v29 (the buggy-format writer): min(count,32) + front-32 block.
static int SaveV29(uchar* buf)
{
	int p = 0;
	int savedCount = g_Count < Lights_32_Count ? g_Count : Lights_32_Count;
	Write32(buf, p, (uint)savedCount);
	for (int i = 0; i < Lights_32_Count; i++) buf[p++] = (uchar)g_Indexes[i];
	for (int i = 0; i < savedCount; i++) WritePayload(buf, p, g_List[(uchar)g_Indexes[i]]);
	return p;
}

// (th4) Rev 2.50 fix (F1): mirror of Saving.cpp RepairLightPoolBlock().
// Repairs loadedCount entries into a PERMUTATION of 0..Lights_128_Count-1;
// invalid/duplicate entries are replaced by the missing values.
static int g_consumed = 0; // exact byte consumption of the last loader call

static void RepairLightPoolBlock(int loadedCount)
{
	bool used[Lights_128_Count] = {};
	int missing[Lights_128_Count];
	for (int i = 0; i < loadedCount; i++) {
		int v = (uchar)g_Indexes[i];
		if (v < Lights_128_Count && !used[v]) {
			used[v] = true;
		} else {
			g_Indexes[i] = (char)-1; // malformed or duplicate byte
		}
	}
	int missingCount = 0;
	for (int v = 0; v < Lights_128_Count; v++) if (!used[v]) missing[missingCount++] = v;
	int m = 0;
	for (int i = 0; i < loadedCount; i++) {
		if (g_Indexes[i] == (char)-1) g_Indexes[i] = (char)missing[m++];
	}
	for (int w = loadedCount; m < missingCount; m++, w++) g_Indexes[w] = (char)missing[m];
}

// LoadGame v30: count (negative->0, clamp 128) + 128 bytes + repair + payloads
// (exactly one payload consumed per active entry; malformed entry = discard).
static void LoadV30(const uchar* buf)
{
	int p = 0;
	g_Count = (int)Read32(buf, p);
	if (g_Count < 0) g_Count = 0; // (th4) F3 guard
	if (g_Count > Lights_128_Count) g_Count = Lights_128_Count;
	for (int i = 0; i < Lights_128_Count; i++) g_Indexes[i] = (char)buf[p++];
	RepairLightPoolBlock(Lights_128_Count); // (th4) F1: permutation before use
	for (int i = 0; i < g_Count; i++) {
		int li = (uchar)g_Indexes[i];
		if (li >= Lights_128_Count) { p += (int)sizeof(LightPayload); continue; } // unreachable after repair; mirrors the game's consume-and-drop
		ReadPayload(buf, p, g_List[li]);
	}
	g_consumed = p;
}

// LoadGame v29 NEW (Rev 2.50 rebuild + F3 guard), verbatim port of Saving.cpp.
static void LoadV29New(const uchar* buf)
{
	int p = 0;
	g_Count = (int)Read32(buf, p);
	if (g_Count < 0) g_Count = 0; // (th4) F3 guard
	if (g_Count > Lights_32_Count) g_Count = Lights_32_Count;
	for (int i = 0; i < Lights_32_Count; i++) g_Indexes[i] = (char)buf[p++];
	RepairLightPoolBlock(Lights_32_Count); // (th4) F1: front + reconstructed tail
	for (int i = 0; i < g_Count; i++) {
		int li = (uchar)g_Indexes[i];
		if (li >= Lights_128_Count) { p += (int)sizeof(LightPayload); continue; } // unreachable after repair; mirrors the game's consume-and-drop
		ReadPayload(buf, p, g_List[li]);
	}
	g_consumed = p;
}

// LoadGame v29 OLD (pre-fix reference): identity fill of the tail.
static void LoadV29Old(const uchar* buf)
{
	int p = 0;
	g_Count = (int)Read32(buf, p);
	if (g_Count > Lights_32_Count) g_Count = Lights_32_Count;
	for (int i = 0; i < Lights_32_Count; i++) g_Indexes[i] = (char)buf[p++];
	for (int i = Lights_32_Count; i < Lights_128_Count; i++) g_Indexes[i] = (char)i;
	for (int i = 0; i < g_Count; i++) {
		int li = (uchar)g_Indexes[i];
		if (li >= Lights_128_Count) continue;
		ReadPayload(buf, p, g_List[li]);
	}
	g_consumed = p;
}

// --- checks ---------------------------------------------------------------
static bool isPermutation(const char* idx)
{
	bool seen[Lights_128_Count] = {};
	for (int i = 0; i < Lights_128_Count; i++) {
		int v = (uchar)idx[i];
		if (v >= Lights_128_Count || seen[v]) return false;
		seen[v] = true;
	}
	return true;
}

// Random churn that pushes the count over 32 (cap 128) so >=32 values enter
// the active set; ProcessLightList runs after every op, exactly like the
// game's per-frame compaction.
static void churn(int steps, int seed, bool cap32)
{
	unsigned r = (unsigned)seed * 2654435761u + 0x9E3779B9u;
	for (int s = 0; s < steps; s++) {
		r = r * 1103515245u + 12345u;
		bool add = (r >> 16) % 100 < 55;
		int maxCount = cap32 ? Lights_32_Count : Lights_128_Count;
		if (g_Count == 0) add = true;
		if (add && g_Count < maxCount) {
			AddLight((int)(r % 112), (int)((r >> 8) % 112), (int)((r >> 16) % 16));
		} else {
			LightOff((uchar)g_Indexes[(r >> 16) % g_Count]);
		}
		ProcessLightList();
	}
}

static char snapIdx[Lights_128_Count];
static LightPayload snapList[Lights_128_Count];
static int snapCount;

static void snapshot()
{
	snapCount = g_Count;
	memcpy(snapIdx, g_Indexes, sizeof(snapIdx));
	memcpy(snapList, g_List, sizeof(snapList));
}

static bool payloadsMatch()
{
	for (int i = 0; i < g_Count; i++) {
		int v = (uchar)g_Indexes[i];
		if (memcmp(&g_List[v], &snapList[v], sizeof(LightPayload)) != 0) return false;
	}
	return true;
}

// T1: v30 round trip restores the exact pool.
static bool testV30(int t)
{
	InitLighting();
	churn(120 + t % 180, t * 3 + 1, false);
	snapshot();
	uchar buf[4096];
	int bufLen = SaveV30(buf);
	InitLighting();
	LoadV30(buf);
	if (!canariesOk()) { printf("V30 CANARY SMASHED test %d\n", t); return false; }
	if (g_consumed != bufLen) { printf("V30 CONSUMPTION MISMATCH test %d: got %d want %d\n", t, g_consumed, bufLen); return false; }
	if (g_Count != snapCount) { printf("V30 COUNT MISMATCH test %d: got %d want %d\n", t, g_Count, snapCount); return false; }
	if (memcmp(g_Indexes, snapIdx, sizeof(snapIdx)) != 0) { printf("V30 INDEX MISMATCH test %d\n", t); return false; }
	if (!isPermutation(g_Indexes)) { printf("V30 NOT PERMUTATION test %d\n", t); return false; }
	if (!payloadsMatch()) { printf("V30 PAYLOAD MISMATCH test %d\n", t); return false; }
	return true;
}

// T2a: clean legacy (old 32-cap build state) -> new loader == old loader.
static bool testV29Clean(int t)
{
	InitLighting();
	churn(60 + t % 80, t * 7 + 5, true); // cap 32: front block is always a 0..31 permutation
	uchar buf[4096];
	int bufLen = SaveV29(buf);
	snapshot();

	InitLighting();
	LoadV29New(buf);
	if (!canariesOk()) { printf("V29C NEW CANARY SMASHED test %d\n", t); return false; }
	if (g_consumed != bufLen) { printf("V29C NEW CONSUMPTION test %d: got %d want %d\n", t, g_consumed, bufLen); return false; }
	char newIdx[Lights_128_Count]; LightPayload newList[Lights_128_Count];
	memcpy(newIdx, g_Indexes, sizeof(newIdx)); memcpy(newList, g_List, sizeof(newList));

	InitLighting();
	LoadV29Old(buf);
	if (!canariesOk()) { printf("V29C OLD CANARY SMASHED test %d\n", t); return false; }
	if (g_consumed != bufLen) { printf("V29C OLD CONSUMPTION test %d: got %d want %d\n", t, g_consumed, bufLen); return false; }
	if (memcmp(g_Indexes, newIdx, sizeof(newIdx)) != 0) { printf("V29C INDEX DIVERGENCE test %d\n", t); return false; }
	if (memcmp(g_List, newList, sizeof(newList)) != 0) { printf("V29C PAYLOAD DIVERGENCE test %d\n", t); return false; }
	if (!isPermutation(newIdx)) { printf("V29C NOT PERMUTATION test %d\n", t); return false; }
	return true;
}

// T2b: dirty legacy (buggy 128-cap state). New loader must heal the pool and
// preserve the saved prefix; old loader must reproduce the corruption.
static bool testV29Dirty(int t)
{
	InitLighting();
	churn(120 + t % 180, t * 11 + 7, false);
	uchar buf[4096];
	int bufLen = SaveV29(buf);

	// saved prefix = first min(count,32) values and their payloads
	char savedIdx[Lights_32_Count]; LightPayload savedList[Lights_128_Count]; bool savedValid[Lights_128_Count];
	int savedCount = g_Count < Lights_32_Count ? g_Count : Lights_32_Count;
	for (int i = 0; i < savedCount; i++) {
		savedIdx[i] = g_Indexes[i];
		int v = (uchar)g_Indexes[i];
		savedList[v] = g_List[v]; savedValid[v] = true;
	}
	bool dirty = false;
	for (int i = 0; i < Lights_32_Count; i++) if ((uchar)g_Indexes[i] >= Lights_32_Count) dirty = true;
	if (!dirty) return true; // nothing to heal on this iteration

	InitLighting();
	LoadV29New(buf);
	if (!canariesOk()) { printf("V29D NEW CANARY SMASHED test %d\n", t); return false; }
	if (g_consumed != bufLen) { printf("V29D NEW CONSUMPTION test %d: got %d want %d\n", t, g_consumed, bufLen); return false; }
	if (!isPermutation(g_Indexes)) { printf("V29D NEW NOT PERMUTATION test %d\n", t); return false; }
	for (int i = 0; i < savedCount; i++) {
		if (g_Indexes[i] != savedIdx[i]) { printf("V29D PREFIX CHANGED test %d at %d: got %d want %d\n", t, i, (uchar)g_Indexes[i], (uchar)savedIdx[i]); return false; }
		int v = (uchar)savedIdx[i];
		if (memcmp(&g_List[v], &savedList[v], sizeof(LightPayload)) != 0) { printf("V29D PAYLOAD MISMATCH test %d slot %d\n", t, v); return false; }
	}

	InitLighting();
	LoadV29Old(buf);
	if (!canariesOk()) { printf("V29D OLD CANARY SMASHED test %d\n", t); return false; }
	if (isPermutation(g_Indexes)) { printf("V29D OLD LOADER SHOULD CORRUPT test %d\n", t); return false; }
	return true;
}

// T3: corrupt-block fuzz — any 32-byte front block must yield a permutation,
// consume exactly one payload per active entry, and never an OOB write.
static bool testV29Fuzz(int t)
{
	uchar buf[4096];
	int p = 0;
	uint rawCount = (uint)((t * 13 + 7) % 33); // random count 0..32
	if (t % 7 == 0) rawCount = 0x80000000u | (rawCount + 1u); // negative every 7th
	Write32(buf, p, rawCount);
	unsigned r = (unsigned)t * 2654435761u + 0xDEADBEEFu;
	for (int i = 0; i < Lights_32_Count; i++) {
		r = r * 1103515245u + 12345u;
		// mix full 0..255 and duplicate-heavy patterns
		buf[p++] = (t % 3 == 0) ? (uchar)(r >> 24) : (uchar)((r >> 16) & 7) + (t % 2 == 0 ? (uchar)(r >> 8) : 0);
	}
	// payload area may not exist in a corrupt file; loader must not read past
	// the block before touching payloads only for g_Count entries — provide
	// dummy payloads anyway.
	for (int i = 0; i < 33; i++) {
		LightPayload L = { 1, 2, 3, 0 };
		WritePayload(buf, p, L);
	}
	int effCount = (int)rawCount;
	if (effCount < 0) effCount = 0;
	if (effCount > Lights_32_Count) effCount = Lights_32_Count;
	InitLighting();
	LoadV29New(buf);
	if (!canariesOk()) { printf("FUZZ CANARY SMASHED test %d\n", t); return false; }
	if (!isPermutation(g_Indexes)) { printf("FUZZ NOT PERMUTATION test %d\n", t); return false; }
	if (g_consumed != 4 + Lights_32_Count + effCount * (int)sizeof(LightPayload)) {
		printf("FUZZ CONSUMPTION test %d: got %d want %d\n", t, g_consumed, 4 + Lights_32_Count + effCount * (int)sizeof(LightPayload));
		return false;
	}
	return true;
}

// T5: v30 corrupt-block fuzz — any 128-byte block (invalid/duplicate bytes,
// negative/oversized counts) must repair to a permutation, consume exactly one
// payload per active entry (F1: the old bare `continue` desynced the stream),
// and never write out of bounds.
static bool testV30Fuzz(int t)
{
	uchar buf[4096];
	int p = 0;
	unsigned r = (unsigned)t * 2654435761u + 0x1234ABCDu;
	r = r * 1103515245u + 12345u;
	uint rawCount = (r >> 8) % 129;                           // 0..128
	if (t % 7 == 0) rawCount = 0x80000000u | (rawCount + 1u); // negative every 7th
	if (t % 11 == 0) rawCount = 5000u + (r % 1000u);          // oversized every 11th
	Write32(buf, p, rawCount);
	for (int i = 0; i < Lights_128_Count; i++) {
		r = r * 1103515245u + 12345u;
		uchar b = (t % 3 == 0) ? (uchar)(r >> 24) : (uchar)((r >> 16) % 40); // full-range or duplicate-heavy <40
		if (t % 5 == 0 && i % 17 == 0) b = (uchar)(128 + (r >> 24) % 128);   // garbage >=128
		buf[p++] = b;
	}
	for (int i = 0; i < Lights_128_Count; i++) {
		LightPayload L = { 1, 2, 3, 0 };
		WritePayload(buf, p, L);
	}
	int effCount = (int)rawCount;
	if (effCount < 0) effCount = 0;
	if (effCount > Lights_128_Count) effCount = Lights_128_Count;
	InitLighting();
	LoadV30(buf);
	if (!canariesOk()) { printf("V30 FUZZ CANARY SMASHED test %d\n", t); return false; }
	if (!isPermutation(g_Indexes)) { printf("V30 FUZZ NOT PERMUTATION test %d\n", t); return false; }
	if (g_Count != effCount) { printf("V30 FUZZ COUNT test %d: got %d want %d\n", t, g_Count, effCount); return false; }
	if (g_consumed != 4 + Lights_128_Count + effCount * (int)sizeof(LightPayload)) {
		printf("V30 FUZZ CONSUMPTION test %d: got %d want %d\n", t, g_consumed, 4 + Lights_128_Count + effCount * (int)sizeof(LightPayload));
		return false;
	}
	return true;
}

// T4: snowball — dirty legacy load, further churn, then v30 round trip.
static bool testSnowball(int t)
{
	InitLighting();
	churn(150 + t % 120, t * 17 + 3, false);
	uchar buf[4096];
	int bufLen = SaveV29(buf);
	InitLighting();
	LoadV29New(buf);
	if (!isPermutation(g_Indexes)) { printf("SNOWBALL START NOT PERMUTATION test %d\n", t); return false; }
	if (g_consumed != bufLen) { printf("SNOWBALL V29 CONSUMPTION test %d: got %d want %d\n", t, g_consumed, bufLen); return false; }
	churn(80 + t % 100, t * 19 + 11, false);
	snapshot();
	bufLen = SaveV30(buf);
	InitLighting();
	LoadV30(buf);
	if (!canariesOk()) { printf("SNOWBALL CANARY SMASHED test %d\n", t); return false; }
	if (g_consumed != bufLen) { printf("SNOWBALL V30 CONSUMPTION test %d: got %d want %d\n", t, g_consumed, bufLen); return false; }
	if (g_Count != snapCount) { printf("SNOWBALL COUNT MISMATCH test %d\n", t); return false; }
	if (memcmp(g_Indexes, snapIdx, sizeof(snapIdx)) != 0) { printf("SNOWBALL INDEX MISMATCH test %d\n", t); return false; }
	if (!isPermutation(g_Indexes)) { printf("SNOWBALL NOT PERMUTATION test %d\n", t); return false; }
	if (!payloadsMatch()) { printf("SNOWBALL PAYLOAD MISMATCH test %d\n", t); return false; }
	return true;
}

int main()
{
	const int V30_TESTS = 20000;
	const int V29_CLEAN_TESTS = 5000;
	const int V29_DIRTY_TESTS = 20000;
	const int FUZZ_TESTS = 20000;
	const int V30_FUZZ_TESTS = 20000;
	const int SNOWBALL_TESTS = 2000;

	for (int t = 0; t < V30_TESTS; t++) if (!testV30(t)) { printf("FAILED (v30) at test %d\n", t); return 1; }
	for (int t = 0; t < V29_CLEAN_TESTS; t++) if (!testV29Clean(t)) { printf("FAILED (v29 clean) at test %d\n", t); return 1; }
	for (int t = 0; t < V29_DIRTY_TESTS; t++) if (!testV29Dirty(t)) { printf("FAILED (v29 dirty) at test %d\n", t); return 1; }
	for (int t = 0; t < FUZZ_TESTS; t++) if (!testV29Fuzz(t)) { printf("FAILED (fuzz) at test %d\n", t); return 1; }
	for (int t = 0; t < V30_FUZZ_TESTS; t++) if (!testV30Fuzz(t)) { printf("FAILED (v30 fuzz) at test %d\n", t); return 1; }
	for (int t = 0; t < SNOWBALL_TESTS; t++) if (!testSnowball(t)) { printf("FAILED (snowball) at test %d\n", t); return 1; }

	printf("ALL %d V30 + %d CLEAN-LEGACY + %d DIRTY-LEGACY + %d V29-FUZZ + %d V30-FUZZ + %d SNOWBALL LIGHT-SAVE TESTS IDENTICAL (pool permutation + exact consumption + payloads + canaries)\n",
		V30_TESTS, V29_CLEAN_TESTS, V29_DIRTY_TESTS, FUZZ_TESTS, V30_FUZZ_TESTS, SNOWBALL_TESTS);
	return 0;
}
