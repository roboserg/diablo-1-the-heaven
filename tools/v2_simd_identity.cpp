// v2_simd_identity.cpp — Rev 21 byte-identity check
//
// Verifies that the SSE2-vectorised stamp segment (16 px/iteration, float
// sqrt + truncation, byte-exact SSE2 application) produces BYTE-IDENTICAL
// output to the original scalar per-pixel loop, in BOTH modes:
//   max mode      (A8L8AdditiveLight OFF) via the Rev 20 envelope,
//   additive mode (A8L8AdditiveLight ON)  via full-interval stamping.
//
// All loops are verbatim ports of the game code (int arithmetic, Isqrt32,
// incremental d2/delta tracking, SSE2 intrinsics).  Output buffers must
// match exactly for every random configuration.
//
// Build (from c:\th4\tools):
//   cmd /c 'call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x86 >nul 2>&1 & cl /nologo /EHsc /O2 /fp:fast v2_simd_identity.cpp'
//
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <emmintrin.h>

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
	int drStart;
	int dcStart;
	int D64;
	int D64sq;
	int rad;
	const uchar* lut;
};

// ---------------------------------------------------------------------------
// OLD scalar loops (verbatim Rev 15 ports)
// ---------------------------------------------------------------------------
static void oldMaxRow(uchar* dst, uchar* dstB, int W, const Light* lights, int k)
{
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
				if (lv < dstB[x]) dstB[x] = (uchar)lv;
			}
			d2 += delta;
			delta += 4;
			while (d2 >= dsq + 2 * d + 1) { dsq += 2 * d + 1; d++; }
			while (d > 0 && d2 < dsq) { dsq -= 2 * d - 1; d--; }
		}
	}
}

static void oldAddRow(uchar* dst, uchar* dstB, int W, const uchar* ambient, const Light* lights, int k)
{
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
				const int c = B + (int)ambient[x] - 255;
				if (c > 0) {
					int nd = dst[x] - c;
					if (nd < 0) nd = 0;
					dst[x] = (uchar)nd;
					nd = dstB[x] - c;
					if (nd < 0) nd = 0;
					dstB[x] = (uchar)nd;
				}
			}
			d2 += delta;
			delta += 4;
			while (d2 >= dsq + 2 * d + 1) { dsq += 2 * d + 1; d++; }
			while (d > 0 && d2 < dsq) { dsq -= 2 * d - 1; d--; }
		}
	}
}

// ---------------------------------------------------------------------------
// NEW: Rev 21 SIMD stamp segment + Rev 20 envelope (verbatim game ports)
// ---------------------------------------------------------------------------
struct RowLight {
	int drStart;
	int dcStart;
	int tMin;
	int tMax;
	int D64;
	const uchar* lut;
};

static void stampSegmentSimd(uchar* dst, uchar* dstB, int startX, bool additive,
	const uchar* ambient, const RowLight& r, int tS, int tE)
{
	const int t0 = tS;
	int dr = r.drStart + t0;
	int dc = r.dcStart - t0;
	int d2 = dr * dr + dc * dc;
	int delta = 2 * (dr - dc) + 2;

	const uchar* lut = r.lut;
	const int D64 = r.D64;

	int x = startX + tS;
	const int xEnd = startX + tE;

	for (; x + 16 <= xEnd + 1; x += 16) {
		int laneD2[4];
		laneD2[0] = d2;
		laneD2[1] = d2 + delta;
		laneD2[2] = d2 + 2 * delta + 4;
		laneD2[3] = d2 + 3 * delta + 12;
		__m128i v = _mm_loadu_si128((const __m128i*)laneD2);
		int laneD[16];
		__m128i step = _mm_add_epi32(_mm_set1_epi32(4 * delta + 24),
									 _mm_setr_epi32(0, 16, 32, 48));
		for (int g = 0; g < 4; g++) {
			__m128 f = _mm_cvtepi32_ps(v);
			f = _mm_sqrt_ps(f);
			_mm_storeu_si128((__m128i*)(laneD + 4 * g), _mm_cvttps_epi32(f));
			v = _mm_add_epi32(v, step);
			step = _mm_add_epi32(step, _mm_set1_epi32(64));
		}
		uchar bbuf[16];
		for (int i = 0; i < 16; i++) {
			const int dd = laneD[i];
			bbuf[i] = ((uint)dd <= (uint)D64) ? lut[dd] : (uchar)0;
		}
		const __m128i bv = _mm_loadu_si128((const __m128i*)bbuf);
		if (additive) {
			const __m128i amb = _mm_loadu_si128((const __m128i*)(ambient + x));
			const __m128i c = _mm_subs_epu8(bv, _mm_xor_si128(amb, _mm_set1_epi8((char)0xFF)));
			__m128i d0 = _mm_loadu_si128((const __m128i*)(dst + x));
			d0 = _mm_subs_epu8(d0, c);
			_mm_storeu_si128((__m128i*)(dst + x), d0);
			__m128i d1 = _mm_loadu_si128((const __m128i*)(dstB + x));
			d1 = _mm_subs_epu8(d1, c);
			_mm_storeu_si128((__m128i*)(dstB + x), d1);
		} else {
			const __m128i lv = _mm_xor_si128(bv, _mm_set1_epi8((char)0xFF));
			__m128i d0 = _mm_loadu_si128((const __m128i*)(dst + x));
			d0 = _mm_min_epu8(d0, lv);
			_mm_storeu_si128((__m128i*)(dst + x), d0);
			__m128i d1 = _mm_loadu_si128((const __m128i*)(dstB + x));
			d1 = _mm_min_epu8(d1, lv);
			_mm_storeu_si128((__m128i*)(dstB + x), d1);
		}
		d2 += 16 * delta + 480;
		delta += 64;
	}

	int d = (int)Isqrt32((unsigned)d2);
	int dsq = d * d;
	for (; x <= xEnd; x++) {
		if ((uint)d <= (uint)D64) {
			const int B = (int)lut[d];
			if (additive) {
				const int c = B + (int)ambient[x] - 255;
				if (c > 0) {
					int nd = dst[x] - c;
					if (nd < 0) nd = 0;
					dst[x] = (uchar)nd;
					nd = dstB[x] - c;
					if (nd < 0) nd = 0;
					dstB[x] = (uchar)nd;
				}
			} else {
				const int lv = 255 - B;
				if (lv < dst[x]) dst[x] = (uchar)lv;
				if (lv < dstB[x]) dstB[x] = (uchar)lv;
			}
		}
		d2 += delta;
		delta += 4;
		while (d2 >= dsq + 2 * d + 1) { dsq += 2 * d + 1; d++; }
		while (d > 0 && d2 < dsq) { dsq -= 2 * d - 1; d--; }
	}
}

static void newMaxRow(uchar* dst, uchar* dstB, int startX, int W, const Light* lights, int k)
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
				q = ceilDivI(db, da) - 1;
				p = -0x40000000;
			} else {
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
			if (rmP[a] > cur) stampSegmentSimd(dst, dstB, startX, false, nullptr, ri, cur, rmP[a] - 1);
			if (rmQ[a] >= cur) cur = rmQ[a] + 1;
		}
		if (cur <= ri.tMax) stampSegmentSimd(dst, dstB, startX, false, nullptr, ri, cur, ri.tMax);
	}
}

static void newAddRow(uchar* dst, uchar* dstB, int startX, int W, const uchar* ambient,
	const Light* lights, int k)
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
	for (int i = 0; i < rlCount; i++)
		stampSegmentSimd(dst, dstB, startX, true, ambient, rl[i], rl[i].tMin, rl[i].tMax);
}

// ---------------------------------------------------------------------------
// Random LUT / config generation (same scheme as v2_envelope_identity.cpp)
// ---------------------------------------------------------------------------
struct LutSet {
	uchar row[16][1026];
	int   d64[16];
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
		int D64 = 64 + (int)(rnd() % 800);
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
		}
		ls.d64[rad] = D64;
	}
}

static bool testRow(const LutSet& ls, int testId)
{
	const int W = (rnd() % 2) ? 640 : 320;
	uchar ambRow[640], dstOld[640], dstNew[640], dstBOld[640], dstBNew[640];
	// dstB plays no role in the old single-buffer ports below, but the new
	// SIMD segment writes both — verify both against identical pre-fills.
	for (int x = 0; x < W; x++) {
		ambRow[x] = (uchar)(rnd() & 255);
		dstOld[x] = (uchar)(rnd() & 255);
		dstNew[x] = dstOld[x];
		dstBOld[x] = dstOld[x];   // old loops only touch dst; mirror for compare
		dstBNew[x] = dstOld[x];
	}

	Light lights[Lights_128_Count];
	// (th4) Rev 2.46: 10% of the tests exercise the enlarged 33..127 slot
	// range; the rest stay in the classic 1..16 band.
	int k = ((rnd() % 10) == 0) ? 33 + (int)(rnd() % (Lights_128_Count - 33)) : 1 + (int)(rnd() % 16);

	const bool clustered = (rnd() % 100) < 70;
	static const int clusterRads[4] = { 4, 8, 12, 15 };
	int baseRad = clusterRads[rnd() % 4];
	int clusterCount = clustered ? 2 + (int)(rnd() % 11) : 0;
	int cdr = (int)(rnd() % 4001) - 2000;
	int cdc = (int)(rnd() % 4001) - 2000;
	int spread = 32 + (int)(rnd() % 900);

	for (int i = 0; i < k; i++) {
		int rad, dr, dc;
		if (clustered && i < clusterCount) {
			rad = baseRad;
			dr = cdr + (int)(rnd() % (2 * spread + 1)) - spread;
			dc = cdc + (int)(rnd() % (2 * spread + 1)) - spread;
		} else {
			rad = (int)(rnd() % 16);
			if ((rnd() % 3) == 0) {
				dr = (int)(rnd() % 38001) - 19000;
				dc = (int)(rnd() % 38001) - 19000;
			} else {
				dr = (int)(rnd() % 6001) - 3000;
				dc = (int)(rnd() % 6001) - 3000;
			}
		}
		if (i > 0 && (rnd() % 20) == 0) {
			dr = lights[0].drStart; dc = lights[0].dcStart; rad = lights[0].rad;
		}
		if (i > 0 && (rnd() % 25) == 0) {
			int delta = (int)(rnd() % 2001) - 1000;
			dr = lights[0].drStart + delta;
			dc = lights[0].dcStart + delta;
			rad = lights[0].rad;
		}
		lights[i].drStart = dr;
		lights[i].dcStart = dc;
		lights[i].D64 = ls.d64[rad];
		lights[i].D64sq = lights[i].D64 * lights[i].D64;
		lights[i].rad = rad;
		lights[i].lut = ls.row[rad];
	}

	// max mode: old full-interval scalar vs new envelope + SIMD segments
	oldMaxRow(dstOld, dstBOld, W, lights, k);
	newMaxRow(dstNew, dstBNew, 0, W, lights, k);
	for (int x = 0; x < W; x++) {
		if (dstOld[x] != dstNew[x] || dstBOld[x] != dstBNew[x]) {
			printf("MAX MISMATCH test %d at x=%d: old=%d new=%d newB=%d k=%d\n",
				testId, x, (int)dstOld[x], (int)dstNew[x], (int)dstBNew[x], k);
			return false;
		}
	}

	// additive mode: old scalar vs new SIMD full-interval
	for (int x = 0; x < W; x++) {
		dstOld[x] = ambRow[x];
		dstNew[x] = ambRow[x];
		dstBOld[x] = ambRow[x];
		dstBNew[x] = ambRow[x];
	}
	oldAddRow(dstOld, dstBOld, W, ambRow, lights, k);
	newAddRow(dstNew, dstBNew, 0, W, ambRow, lights, k);
	for (int x = 0; x < W; x++) {
		if (dstOld[x] != dstNew[x] || dstBOld[x] != dstBNew[x]) {
			printf("ADD MISMATCH test %d at x=%d: old=%d new=%d newB=%d k=%d\n",
				testId, x, (int)dstOld[x], (int)dstNew[x], (int)dstBNew[x], k);
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
	printf("ALL %d TESTS IDENTICAL (SIMD stamp == old scalar loop, both modes)\n", TESTS);
	return 0;
}
