// v2_envelope_identity.cpp — Rev 20 byte-identity check
//
// Verifies that the max-mode (A8L8AdditiveLight OFF) nearest-light row
// envelope in FillScreenSpaceLightingV2 produces BYTE-IDENTICAL output to
// the original per-light full-interval stamping loop.
//
// Both loops below are verbatim ports of the game code (integer arithmetic,
// Isqrt32, incremental d2/d tracking).  The old loop stamps every light over
// its full disc interval; the new loop stamps only each light's winner
// pieces within its radius group.
//
// Build (from c:\th4\tools):
//   cmd /c 'call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x86 >nul 2>&1 & cl /nologo /EHsc /O2 /fp:fast v2_envelope_identity.cpp'
//
#include <cstdio>
#include <cstdlib>
#include <cstring>

typedef unsigned char uchar;
typedef unsigned int uint;

// (th4) Rev 2.46: the game's runtime light-slot capacity is now 128
// (Lights_128_Count). The tool's light/stamp arrays mirror it and the
// generator occasionally exercises the 33..127 range.
static const int Lights_128_Count = 128;

static inline unsigned Isqrt32(unsigned n)
{
	unsigned res = 0;
	unsigned bit = 1u << 30;
	while (bit > n) bit >>= 2;
	while (bit) {
		if (n >= res + bit) {
			n -= res + bit;
			res = (res >> 1) + bit;
		} else {
			res >>= 1;
		}
		bit >>= 2;
	}
	return res;
}

static int ceilDiv2(int x) { return x >= 0 ? (x + 1) / 2 : x / 2; }
static int floorDiv2(int x) { return x >= 0 ? x / 2 : (x - 1) / 2; }

static int floorDivI(int n, int d)
{
	const int q = n / d, r = n % d;
	return (r != 0 && ((r < 0) != (d < 0))) ? q - 1 : q;
}
static int ceilDivI(int n, int d)
{
	const int q = n / d, r = n % d;
	return (r != 0 && ((r < 0) == (d < 0))) ? q + 1 : q;
}

struct Light {
	int drStart;   // row-iso offset of light center at t = 0 (constant per row)
	int dcStart;
	int D64;
	int D64sq;
	int rad;
	const uchar* lut;
};

// ---------------------------------------------------------------------------
// OLD: per-light full-interval stamping (verbatim port of Rev 15 game code,
// max mode).  t is the scanline coordinate; dst pre-filled with ambient.
// ---------------------------------------------------------------------------
static void oldRow(uchar* dst, int W, const Light* lights, int k)
{
	for (int s = 0; s < k; s++) {
		const Light& st = lights[s];
		const long long ssum = (long long)(st.drStart + st.dcStart) * (st.drStart + st.dcStart);
		if (ssum > 2LL * st.D64sq) continue; // row outside this light's disc
		const int b = st.dcStart - st.drStart;
		const int halfSpan = (int)Isqrt32((unsigned)(2 * st.D64sq - (int)ssum));
		int tMin = ceilDiv2(b - halfSpan);
		int tMax = floorDiv2(b + halfSpan);
		if (tMin < 0) tMin = 0;
		if (tMax > W - 1) tMax = W - 1;
		if (tMin > tMax) continue;

		const int t0 = tMin;
		int dr = st.drStart + t0;
		int dc = st.dcStart - t0;
		int d2 = dr * dr + dc * dc;
		int d = (int)Isqrt32((unsigned)d2);
		int dsq = d * d;
		int delta = 2 * (dr - dc) + 2;

		const uchar* lut = st.lut;
		const int D64 = st.D64;
		for (int x = tMin; x <= tMax; x++) {
			if ((uint)d <= (uint)D64) {
				const int B = (int)lut[d];
				const int lv = 255 - B;
				if (lv < dst[x]) dst[x] = (uchar)lv;
			}
			d2 += delta;
			delta += 4;
			while (d2 >= dsq + 2 * d + 1) { dsq += 2 * d + 1; d++; }
			while (d > 0 && d2 < dsq) { dsq -= 2 * d - 1; d--; }
		}
	}
}

// ---------------------------------------------------------------------------
// NEW: max-mode nearest-light envelope (verbatim port of the Rev 20 game
// code).  Pass 1 builds the row list; pass 2 computes each light's winner
// pieces inside its radius group and stamps only those.
// ---------------------------------------------------------------------------
struct RowLight {
	int drStart;
	int dcStart;
	int tMin;
	int tMax;
	int D64;
	const uchar* lut;
};

static void newRow(uchar* dst, int W, const Light* lights, int k)
{
	RowLight rl[Lights_128_Count];
	int rlCount = 0;
	for (int s = 0; s < k; s++) {
		const Light& st = lights[s];
		const long long ssum = (long long)(st.drStart + st.dcStart) * (st.drStart + st.dcStart);
		if (ssum > 2LL * st.D64sq) continue;
		const int b = st.dcStart - st.drStart;
		const int halfSpan = (int)Isqrt32((unsigned)(2 * st.D64sq - (int)ssum));
		int tMin = ceilDiv2(b - halfSpan);
		int tMax = floorDiv2(b + halfSpan);
		if (tMin < 0) tMin = 0;
		if (tMax > W - 1) tMax = W - 1;
		if (tMin > tMax) continue;
		RowLight& r = rl[rlCount++];
		r.drStart = st.drStart;
		r.dcStart = st.dcStart;
		r.tMin = tMin;
		r.tMax = tMax;
		r.D64 = st.D64;
		r.lut = st.lut;
	}

	int rmP[Lights_128_Count], rmQ[Lights_128_Count];
	for (int i = 0; i < rlCount; i++) {
		const RowLight& ri = rl[i];
		const int ai = 2 * (ri.drStart - ri.dcStart);
		const int bi = ri.drStart * ri.drStart + ri.dcStart * ri.dcStart;
		int rmCount = 0;
		bool beatenEverywhere = false;
		for (int j = 0; j < rlCount; j++) {
			if (j == i || rl[j].lut != ri.lut) continue;
			const RowLight& rj = rl[j];
			const int da = 2 * (rj.drStart - rj.dcStart) - ai;
			const int db = bi - (rj.drStart * rj.drStart + rj.dcStart * rj.dcStart);
			int p, q;
			if (da == 0) {
				if (db > 0) { beatenEverywhere = true; break; }
				continue;
			} else if (da > 0) {
				// Rev 27: skip the idiv when the removal is provably before
				// i's interval (same conservative test as the game).
				const int T = rj.tMin > ri.tMin ? rj.tMin : ri.tMin;
				if (db <= da * T) continue;
				q = ceilDivI(db, da) - 1;
				p = -0x40000000;
			} else {
				const int U = rj.tMax < ri.tMax ? rj.tMax : ri.tMax;
				if (db <= da * U) continue;
				p = floorDivI(db, da) + 1;
				q = 0x40000000;
			}
			if (p < rj.tMin) p = rj.tMin;
			if (q > rj.tMax) q = rj.tMax;
			if (p <= q) { rmP[rmCount] = p; rmQ[rmCount] = q; rmCount++; }
		}
		if (beatenEverywhere) continue;
		for (int a = 1; a < rmCount; a++) {
			const int pp = rmP[a], qq = rmQ[a];
			int b = a - 1;
			while (b >= 0 && rmP[b] > pp) { rmP[b + 1] = rmP[b]; rmQ[b + 1] = rmQ[b]; b--; }
			rmP[b + 1] = pp;
			rmQ[b + 1] = qq;
		}
		int cur = ri.tMin;
		for (int a = 0; a < rmCount && cur <= ri.tMax; a++) {
			if (rmP[a] > ri.tMax) break;
			if (rmP[a] > cur) {
				const int t0 = cur;
				int dr = ri.drStart + t0;
				int dc = ri.dcStart - t0;
				int d2 = dr * dr + dc * dc;
				int d = (int)Isqrt32((unsigned)d2);
				int dsq = d * d;
				int delta = 2 * (dr - dc) + 2;
				const uchar* lut = ri.lut;
				const int D64 = ri.D64;
				for (int x = cur; x <= rmP[a] - 1; x++) {
					if ((uint)d <= (uint)D64) {
						const int B = (int)lut[d];
						const int lv = 255 - B;
						if (lv < dst[x]) dst[x] = (uchar)lv;
					}
					d2 += delta;
					delta += 4;
					while (d2 >= dsq + 2 * d + 1) { dsq += 2 * d + 1; d++; }
					while (d > 0 && d2 < dsq) { dsq -= 2 * d - 1; d--; }
				}
			}
			if (rmQ[a] >= cur) cur = rmQ[a] + 1;
		}
		if (cur <= ri.tMax) {
			const int t0 = cur;
			int dr = ri.drStart + t0;
			int dc = ri.dcStart - t0;
			int d2 = dr * dr + dc * dc;
			int d = (int)Isqrt32((unsigned)d2);
			int dsq = d * d;
			int delta = 2 * (dr - dc) + 2;
			const uchar* lut = ri.lut;
			const int D64 = ri.D64;
			for (int x = cur; x <= ri.tMax; x++) {
				if ((uint)d <= (uint)D64) {
					const int B = (int)lut[d];
					const int lv = 255 - B;
					if (lv < dst[x]) dst[x] = (uchar)lv;
				}
				d2 += delta;
				delta += 4;
				while (d2 >= dsq + 2 * d + 1) { dsq += 2 * d + 1; d++; }
				while (d > 0 && d2 < dsq) { dsq -= 2 * d - 1; d--; }
			}
		}
	}
}

// ---------------------------------------------------------------------------
// Random monotone non-increasing LUT rows (plateaus allowed), zero past D64.
// ---------------------------------------------------------------------------
struct LutSet {
	uchar row[16][1026];
	int   d64[16]; // largest d with lut[d] > 0 (what the game's binary search yields)
};

static uint g_rng = 0x12345678u;
static uint rnd()
{
	g_rng = g_rng * 1664525u + 1013904223u;
	return g_rng;
}

static void genLut(LutSet& ls)
{
	for (int rad = 0; rad < 16; rad++) {
		int D64 = 64 + (int)(rnd() % 800);          // 64..863, like 64(rad+1)
		int v = 255;
		for (int d = 0; d < 1026; d++) {
			if (d <= D64) {
				if (rnd() % 3 == 0) {
					int drop = 1 + (int)(rnd() % 3);
					if (v - drop < 1) v = 1; else v -= drop;
				}
				ls.row[rad][d] = (uchar)v;
			} else {
				ls.row[rad][d] = 0;
			}
		}		ls.d64[rad] = D64;	}
}

static bool testRow(const LutSet& ls, int testId)
{
	const int W = (rnd() % 2) ? 640 : 320;
	uchar dstOld[640], dstNew[640];

	// ambient pre-fill: random values 0..255
	for (int x = 0; x < W; x++) {
		dstOld[x] = (uchar)(rnd() & 255);
		dstNew[x] = dstOld[x];
	}

	Light lights[Lights_128_Count];
	// (th4) Rev 2.46: 10% of the tests exercise the enlarged 33..127 slot
	// range; the rest stay in the classic 1..16 band.
	int k = ((rnd() % 10) == 0) ? 33 + (int)(rnd() % (Lights_128_Count - 33)) : 1 + (int)(rnd() % 16);

	// 70% of tests: a same-radius cluster (fire wall / NOVA shape) plus
	// scattered mixed-radius lights.  30%: all random.
	const bool clustered = (rnd() % 100) < 70;
	int clusterRad = (int)(rnd() % 4);       // 0..3 -> rads 4, 8, 12, 15
	static const int clusterRads[4] = { 4, 8, 12, 15 };
	int baseRad = clusterRads[clusterRad];
	int clusterCount = clustered ? 2 + (int)(rnd() % 11) : 0;   // 2..12 same-rad
	int cdr = (int)(rnd() % 4001) - 2000;
	int cdc = (int)(rnd() % 4001) - 2000;
	int spread = 32 + (int)(rnd() % 900);    // heavy overlap

	for (int i = 0; i < k; i++) {
		int rad;
		int dr, dc;
		if (clustered && i < clusterCount) {
			rad = baseRad;
			dr = cdr + (int)(rnd() % (2 * spread + 1)) - spread;
			dc = cdc + (int)(rnd() % (2 * spread + 1)) - spread;
		} else {
			rad = (int)(rnd() % 16);
			if ((rnd() % 3) == 0) {
				// game-scale coordinates, some far away
				dr = (int)(rnd() % 38001) - 19000;
				dc = (int)(rnd() % 38001) - 19000;
			} else {
				dr = (int)(rnd() % 6001) - 3000;
				dc = (int)(rnd() % 6001) - 3000;
			}
		}
		// special cases: identical centers (ties), parallel lines, duplicates
		if (i > 0 && (rnd() % 20) == 0) {
			// identical center + radius as lights[0] (same line, same value)
			dr = lights[0].drStart; dc = lights[0].dcStart; rad = lights[0].rad;
		}
		if (i > 0 && (rnd() % 25) == 0) {
			// parallel line: same slope a = 2(dr - dc), shifted intercept
			int delta = (int)(rnd() % 2001) - 1000;
			dr = lights[0].drStart + delta;
			dc = lights[0].dcStart + delta;
			rad = lights[0].rad;
		}
		lights[i].drStart = dr;
		lights[i].dcStart = dc;
		lights[i].D64 = ls.d64[rad];              // actual LUT support, as in-game
		lights[i].D64sq = lights[i].D64 * lights[i].D64;
		lights[i].rad = rad;
		lights[i].lut = ls.row[rad];
	}

	oldRow(dstOld, W, lights, k);
	newRow(dstNew, W, lights, k);

	for (int x = 0; x < W; x++) {
		if (dstOld[x] != dstNew[x]) {
			printf("MISMATCH test %d at x=%d: old=%d new=%d  k=%d\n",
				testId, x, (int)dstOld[x], (int)dstNew[x], k);
			for (int i = 0; i < k; i++)
				printf("  light[%d] dr=%d dc=%d D64=%d\n",
					i, lights[i].drStart, lights[i].dcStart, lights[i].D64);
			return false;
		}
	}
	return true;
}

int main()
{
	LutSet ls;
	genLut(ls);
	const int TESTS = 20000;
	for (int t = 0; t < TESTS; t++) {
		if (!testRow(ls, t)) {
			printf("FAILED at test %d\n", t);
			return 1;
		}
	}
	printf("ALL %d TESTS IDENTICAL (max-mode envelope == old full-interval loop)\n", TESTS);
	return 0;
}
