// v2_stamp_identity.cpp — byte-identity check for the CURRENT game state
// (post Rev 20/21/22/24/27: max-brightness only, nearest-light envelope with
// disjoint-interval early-out and idiv prefilter, SSE2 stamp segments,
// single-buffer fill + row-end base copy, full-bright memset fast path).
// Stage 3 adds winner-tint tracking (LightTintSurface): ambient = white,
// every strictly-brighter stamp writes the winning light's tint, ties keep
// the first writer.  The light bytes must stay byte-identical to the
// pre-Stage-3 loop, and the tint field is verified against an independent
// full-interval reference.
//
// Test 1 (stamps): the current game stamping (envelope + SIMD segment, max
//   mode only) produces BYTE-IDENTICAL output to the original Rev 15 scalar
//   full-interval loop, for random LUTs / light configurations.
// Test 2 (full-bright fast path): with an all-zero dst (the exact content
//   the ambient fill produces for an all-zero map), stamps are no-ops —
//   min(0, lv) == 0 — so the Rev 22 memset fast path is byte-identical to
//   running the general path.  Verified against BOTH the old scalar loop
//   and the new envelope loop over random configurations.
//
// All loops are verbatim ports of the game code (int arithmetic, Isqrt32,
// incremental d2/delta tracking, SSE2 intrinsics).
//
// Build (from c:\th4\tools):
//   cmd /c 'call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x86 >nul 2>&1 & cl /nologo /EHsc /O2 /fp:fast v2_stamp_identity.cpp'
//
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <emmintrin.h>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

// (th4) Rev 2.46: the game's runtime light-slot capacity is now 128
// (Lights_128_Count). The light/stamp arrays mirror it and the generators
// occasionally exercise the 33..127 range so the identity checks cover the
// enlarged capacity.
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
	// (th4) Stage 3: winning-light tint (white = no tint).
	uchar tintR, tintG, tintB;
};

// ---------------------------------------------------------------------------
// Reference: original Rev 15 scalar loop, max mode, full interval per light
// (VALUE only — byte-identity check for the light field).
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

// ---------------------------------------------------------------------------
// (th4) Stage 3 tint winner reference — INDEPENDENT per-pixel rule, computed
// from scratch (no envelope):
//   The Rev-20 nearest-light envelope (which Stage 3 is mandated to retain)
//   lets each pixel be stamped by, per (radius, tint color) class, only the
//   light with the smallest distance d2 (exact d2 ties stamp in order —
//   equal lv, so the second can never strictly brighten; skipping it is
//   equivalent). Rev 2.26 added the class split: same-radius lights of
//   DIFFERENT tint colors both stamp. Rev 2.44 replaced the win/lose pair
//   crossfade with a brightness-mass weighted average over ALL in-disc
//   class winners in stamp order (whites first — Rev 2.26):
//     the tint word's A byte = min(255, sum B_i) (shader strength); each
//     in-disc light blends the running RGB toward its color with weight
//     w = B/(m + B) against the TRUE mass m = sum B_i (Rev 2.45 — kept in
//     a per-row ushort scratch; the Rev 2.44 clamped mass survives only in
//     the A byte). White lights are ordinary contributors (color
//     255,255,255), so the result is a pure function of the per-pixel light
//     set — continuous and order-independent up to the rounding of the
//     incremental average (the canonical white-first stamp order keeps it
//     deterministic).
// The ambient fill is the baseline writer (white, mass 0).
// Callers fill tintOut with white first.
// ---------------------------------------------------------------------------
static const uint TINT_WHITE = 0x00FFFFFFu;

// (th4) Rev 2.44: per-pixel tint blend weight table, indexed [oldMass][B]:
// w = (B * 255 + (mass + B) / 2) / (mass + B) — mirrors the game's
// g_TintWeightLut (the game fills it in a static initializer; this tool
// fills it once in main and verifies it against the literal formula, so a
// LUT drift can never hide inside the identity tests).
static uchar g_TintWeightLut[256][256];

static bool InitTintWeightLut()
{
	for (int m = 0; m <= 255; m++) {
		for (int B = 0; B <= 255; B++) {
			uint w = 0;
			if (m > 0 && B > 0) {
				const uint sum = (uint)m + (uint)B; // 2..510
				w = ((uint)B * 255 + (sum >> 1)) / sum;
			}
			g_TintWeightLut[m][B] = (uchar)w;
		}
	}
	for (int m = 1; m <= 255; m++) {
		for (int B = 1; B <= 255; B++) {
			const uint sum = (uint)m + (uint)B;
			const uint ref = ((uint)B * 255 + (sum >> 1)) / sum;
			if (g_TintWeightLut[m][B] != (uchar)ref) return false;
		}
	}
	return true;
}

// (th4) Rev 2.44/2.45: per-pixel tint rule shared by the SIMD fallback and
// the scalar tail (mirrors the game's writeTintPixel lambda). Every in-disc
// pixel contributes its brightness-weighted color to the running mass
// average: B == 0 keeps the old word; mass 0 (white ambient baseline) gets
// the plain replace word | B<<24; otherwise the running RGB crossfades
// toward the incoming color with the mass-relative weight. Rev 2.45: the
// weight base is the TRUE mass kept in a per-row ushort scratch (massRow) —
// w = g_TintWeightLut[m][B] for m <= 255, else (B*255 + (m+B)/2)/(m+B) —
// and the tint word's A byte carries min(255, mass). White lights are
// ordinary contributors — no special-casing, which is what makes the result
// a pure function of the per-pixel light set (no event-sequence
// discontinuities with 3+ distinct colors).
static inline void writeTintPixel(uint* tintRow, ushort* massRow, int px, uint word, uint B)
{
	if (B == 0) return;                          // outside this light's disc
	const uint old = tintRow[px];
	const uint oldA = old >> 24;                 // saturated mass (A byte)
	if (oldA == 0) {                             // first contributor over the white pre-fill
		tintRow[px] = word | (B << 24);
		massRow[px] = (ushort)B;
	} else {
		const uint m = massRow[px];              // TRUE accumulated brightness mass
		const uint sum = m + B;                  // <= 8160 + 255
		const uint w = (m <= 255)
			? g_TintWeightLut[m][B]              // (B*255 + (m+B)/2) / (m+B)
			: ((B * 255 + (sum >> 1)) / sum);    // true-mass idiv (deep overlap only)
		const uint wr = (word >> 16) & 0xFF, wg = (word >> 8) & 0xFF, wb = word & 0xFF;
		const uint orr = (old >> 16) & 0xFF, og = (old >> 8) & 0xFF, ob = old & 0xFF;
		const uint nr = (wr * w + orr * (255 - w) + 127) >> 8;
		const uint ng = (wg * w + og * (255 - w) + 127) >> 8;
		const uint nb = (wb * w + ob * (255 - w) + 127) >> 8;
		const uint na = (sum > 255) ? 255 : sum;
		tintRow[px] = nb | (ng << 8) | (nr << 16) | (na << 24);
		massRow[px] = (ushort)sum;
	}
}

struct ClassMin {
	int rad; uchar r, g, b; int idx; long long d2;
};

static void refTintWinnerRow(uint* tintOut, int W, const Light* lights, int k)
{
	for (int x = 0; x < W; x++) {
		// 1) per-class (radius + tint color) d2 minimum (exact d2 ties:
		// first in stamp order keeps the class winner).
		ClassMin cm[Lights_128_Count];
		int cmCount = 0;
		for (int s = 0; s < k; s++) {
			const Light& st = lights[s];
			const int dr = st.drStart + x;
			const int dc = st.dcStart - x;
			const long long d2 = (long long)dr * dr + (long long)dc * dc;
			if (d2 > (long long)st.D64sq) continue; // outside the disc
			int c = -1;
			for (int t = 0; t < cmCount; t++) {
				if (cm[t].rad == st.rad && cm[t].r == st.tintR
					&& cm[t].g == st.tintG && cm[t].b == st.tintB) { c = t; break; }
			}
			if (c < 0) {
				c = cmCount++;
				cm[c].rad = st.rad; cm[c].r = st.tintR; cm[c].g = st.tintG;
				cm[c].b = st.tintB; cm[c].idx = s; cm[c].d2 = d2;
			} else if (d2 < cm[c].d2) {
				cm[c].idx = s;
				cm[c].d2 = d2;
			}
		}
		// 2) Rev 2.44/2.45 brightness-mass weighted average over the class
		// winners in stamp order (whites first — the game's canonical order):
		//   mass 0 -> plain replace word | B<<24;
		//   mass > 0 -> RGB crossfade toward the incoming color with
		//   w = g_TintWeightLut[mass][B] (mass <= 255) or the true-mass idiv
		//   (mass > 255) — Rev 2.45 keeps the TRUE mass (no clamp); the word's
		//   A byte carries min(255, mass). White lights are ordinary
		//   contributors, so the result depends only on the per-pixel light
		//   set — no win/lose event-sequence branches.
		uint mass = 0;
		uint word = TINT_WHITE;
		for (int s = 0; s < k; s++) {
			int c = -1;
			for (int t = 0; t < cmCount; t++) {
				if (cm[t].idx == s) { c = t; break; }
			}
			if (c < 0) continue;
			const Light& st = lights[s];
			const int dr = st.drStart + x;
			const int dc = st.dcStart - x;
			const long long d2 = (long long)dr * dr + (long long)dc * dc;
			const int d = (int)Isqrt32((unsigned)d2);
			const int B = (int)st.lut[d];
			if (B == 0) continue;
			// BGR byte order — byte 0 = blue, matching the A8R8G8B8 texture
			// layout (same packing as the game's rgbLUT).
			const uint wrd = (uint)st.tintB | ((uint)st.tintG << 8) | ((uint)st.tintR << 16);
			if (mass == 0) {
				word = wrd | ((uint)B << 24);
				mass = (uint)B;
			} else {
				const uint sum = mass + (uint)B; // <= 8160 + 255
				const uint w = (mass <= 255)
					? g_TintWeightLut[mass][B]
					: ((uint)B * 255 + (sum >> 1)) / sum;
				const uint wr = (wrd >> 16) & 0xFF, wg = (wrd >> 8) & 0xFF, wb = wrd & 0xFF;
				const uint orr = (word >> 16) & 0xFF, og = (word >> 8) & 0xFF, ob = word & 0xFF;
				const uint nr = (wr * w + orr * (255 - w) + 127) >> 8;
				const uint ng = (wg * w + og * (255 - w) + 127) >> 8;
				const uint nb = (wb * w + ob * (255 - w) + 127) >> 8;
				mass = sum;                      // Rev 2.45: TRUE mass, no clamp
				const uint na = sum > 255 ? 255 : sum;
				word = nb | (ng << 8) | (nr << 16) | (na << 24);
			}
		}
		tintOut[x] = word;
	}
}

// ---------------------------------------------------------------------------
// Current game code: Rev 20 envelope (Rev 24 disjoint early-out + Rev 27 idiv
// prefilter) + Rev 21 SIMD segment, max mode only, single-buffer fill with a
// row-end base copy (Rev 27 §4.3).
// ---------------------------------------------------------------------------
struct RowLight {
	int drStart;
	int dcStart;
	int tMin;
	int tMax;
	int D64;
	const uchar* lut;
	// (th4) Stage 3: winning-light tint (white = no tint).
	uchar tintR, tintG, tintB;
};

static void stampSegmentSimd(uchar* dst, uint* tintRow, ushort* massRow, int startX, const RowLight& r, int tS, int tE)
{
	const int t0 = tS;
	int dr = r.drStart + t0;
	int dc = r.dcStart - t0;
	int d2 = dr * dr + dc * dc;
	int delta = 2 * (dr - dc) + 2;

	const uchar* lut = r.lut;
	const int D64 = r.D64;

	// (th4) Stage 3: constant winner tint for this segment.
	// (th4) 2026-09-04: BGR byte order (byte 0 = blue) — the A8R8G8B8
	// texture memory layout; see refTintWinnerRow.
	const uint tintWord = (uint)r.tintB | ((uint)r.tintG << 8) | ((uint)r.tintR << 16);
	const __m128i tintVec = _mm_set1_epi32((int)tintWord);

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
		{
			const __m128i lv = _mm_xor_si128(bv, _mm_set1_epi8((char)0xFF));
			__m128i d0 = _mm_loadu_si128((const __m128i*)(dst + x));
			__m128i dn = _mm_min_epu8(d0, lv);
			_mm_storeu_si128((__m128i*)(dst + x), dn);
			// (th4) Rev 2.44: EVERY in-disc pixel (B > 0) contributes its
			// brightness-weighted color to the running mass average — win and
			// lose alike. The A byte of the tint word is the accumulated
			// brightness mass. Lanes with B > 0 AND old mass > 0 take the
			// per-pixel blend (scalar); every other lane is out-of-disc (keep
			// old) or a first contributor (plain replace word | B<<24).
			if (tintRow) {
				const __m128i s80 = _mm_set1_epi8((char)0x80);
				// in-disc mask (B > 0): cmpgt_epi8 is SIGNED, so sign-flip
				// the operand first for an unsigned-bytes compare.
				const __m128i mB = _mm_cmpgt_epi8(_mm_xor_si128(bv, s80), s80);
				__m128i* tp = (__m128i*)(tintRow + x);
				__m128i t0 = _mm_loadu_si128(tp + 0);
				__m128i t1 = _mm_loadu_si128(tp + 1);
				__m128i t2 = _mm_loadu_si128(tp + 2);
				__m128i t3 = _mm_loadu_si128(tp + 3);
				// blend needed in any lane with B > 0 AND old mass > 0?
				bool needScalar = false;
				{
					const __m128i zz = _mm_setzero_si128();
					const __m128i a0 = _mm_cmpgt_epi32(_mm_srli_epi32(t0, 24), zz);
					const __m128i a1 = _mm_cmpgt_epi32(_mm_srli_epi32(t1, 24), zz);
					const __m128i a2 = _mm_cmpgt_epi32(_mm_srli_epi32(t2, 24), zz);
					const __m128i a3 = _mm_cmpgt_epi32(_mm_srli_epi32(t3, 24), zz);
					needScalar = _mm_movemask_epi8(_mm_or_si128(
						_mm_or_si128(_mm_and_si128(a0, mB), _mm_and_si128(a1, mB)),
						_mm_or_si128(_mm_and_si128(a2, mB), _mm_and_si128(a3, mB)))) != 0;
				}
				if (!needScalar) {
					// whole group: every lane is out-of-disc (keep old) or a
					// first contributor (replace with word | B<<24).
					const __m128i z = _mm_setzero_si128();
					const __m128i b16lo = _mm_unpacklo_epi8(bv, z);
					const __m128i b16hi = _mm_unpackhi_epi8(bv, z);
					__m128i w0 = _mm_or_si128(_mm_slli_epi32(_mm_unpacklo_epi16(b16lo, z), 24), tintVec);
					__m128i w1 = _mm_or_si128(_mm_slli_epi32(_mm_unpackhi_epi16(b16lo, z), 24), tintVec);
					__m128i w2 = _mm_or_si128(_mm_slli_epi32(_mm_unpacklo_epi16(b16hi, z), 24), tintVec);
					__m128i w3 = _mm_or_si128(_mm_slli_epi32(_mm_unpackhi_epi16(b16hi, z), 24), tintVec);
					const __m128i mlo = _mm_unpacklo_epi8(mB, mB);
					const __m128i mhi = _mm_unpackhi_epi8(mB, mB);
					const __m128i m4a = _mm_unpacklo_epi16(mlo, mlo);
					const __m128i m4b = _mm_unpackhi_epi16(mlo, mlo);
					const __m128i m4c = _mm_unpacklo_epi16(mhi, mhi);
					const __m128i m4d = _mm_unpackhi_epi16(mhi, mhi);
					t0 = _mm_or_si128(_mm_and_si128(m4a, w0), _mm_andnot_si128(m4a, t0));
					t1 = _mm_or_si128(_mm_and_si128(m4b, w1), _mm_andnot_si128(m4b, t1));
					t2 = _mm_or_si128(_mm_and_si128(m4c, w2), _mm_andnot_si128(m4c, t2));
					t3 = _mm_or_si128(_mm_and_si128(m4d, w3), _mm_andnot_si128(m4d, t3));
					_mm_storeu_si128(tp + 0, t0);
					_mm_storeu_si128(tp + 1, t1);
					_mm_storeu_si128(tp + 2, t2);
					_mm_storeu_si128(tp + 3, t3);
					// (th4) Rev 2.45: seed the true-mass scratch for the
					// first-contributor lanes (mass = B, 16-bit lanes).
					{
						__m128i* mp = (__m128i*)(massRow + x);
						__m128i ms0 = _mm_loadu_si128(mp + 0);
						__m128i ms1 = _mm_loadu_si128(mp + 1);
						ms0 = _mm_or_si128(_mm_and_si128(mlo, b16lo), _mm_andnot_si128(mlo, ms0));
						ms1 = _mm_or_si128(_mm_and_si128(mhi, b16hi), _mm_andnot_si128(mhi, ms1));
						_mm_storeu_si128(mp + 0, ms0);
						_mm_storeu_si128(mp + 1, ms1);
					}
				} else {
					// some lane blends: per-pixel mass rule (16 lanes; same
					// math as the scalar tail below).
					for (int i = 0; i < 16; i++) {
						writeTintPixel(tintRow, massRow, x + i, tintWord, (uint)bbuf[i]);
					}
				}
			}
		}
		d2 += 16 * delta + 480;
		delta += 64;
	}

	int d = (int)Isqrt32((unsigned)d2);
	int dsq = d * d;
	for (; x <= xEnd; x++) {
		if ((uint)d <= (uint)D64) {
			const int B = (int)lut[d];
			const int lv = 255 - B;
			// (th4) Rev 2.44/2.45: every in-disc pixel contributes to the mass
			// average (win and lose alike), BEFORE the light byte.
			if (tintRow) writeTintPixel(tintRow, massRow, x, tintWord, (uint)B);
			if (lv < dst[x]) dst[x] = (uchar)lv;
		}
		d2 += delta;
		delta += 4;
		while (d2 >= dsq + 2 * d + 1) { dsq += 2 * d + 1; d++; }
		while (d > 0 && d2 < dsq) { dsq -= 2 * d - 1; d--; }
	}
}

// Envelope driver for the max-mode row: same winner-piece removal logic as
// the game, stamping the surface buffer only.  The base buffer receives the
// same bytes via one row-end copy in newMaxRow (Rev 27 §4.3).
static void envelopeRow(uchar* dst, uint* tintRow, ushort* massRow, int startX, int W, const Light* lights, int k)
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
		r.tintR = st.tintR;
		r.tintG = st.tintG;
		r.tintB = st.tintB;
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
			// (th4) Rev 2.26: the nearest-light envelope applies only between
			// same-radius lights of the SAME tint color (mirrors the game).
			if (rl[j].tintR != ri.tintR || rl[j].tintG != ri.tintG || rl[j].tintB != ri.tintB) continue;
			const RowLight& rj = rl[j];
			if (rj.tMax < ri.tMin || rj.tMin > ri.tMax) continue; // Rev 24: disjoint -> no constraint
			const int da = 2 * (rj.drStart - rj.dcStart) - ai;
			const int db = bi - (rj.drStart * rj.drStart + rj.dcStart * rj.dcStart);
			// (th4) Rev 2.37: exact ties go to the LOWER stamp index (mirrors
			// the game's g_LightTintActive-gated tie-break; this tool always
			// tests the tint-active state). The reference's class-min keeps
			// the first writer on ties, so this makes the envelope and the
			// independent reference agree on tie pixels.
			const bool tieToJ = j < i;
			int p, q;
			if (da == 0) {
				if (db > 0 || (db == 0 && tieToJ)) { beatenEverywhere = true; break; }
				continue;
			} else if (da > 0) {
				// Rev 27: skip the idiv when the removal is provably before
				// i's interval (same conservative test as the game).
				const int T = rj.tMin > ri.tMin ? rj.tMin : ri.tMin;
				if (tieToJ ? db < da * T : db <= da * T) continue;
				q = tieToJ ? floorDivI(db, da) : ceilDivI(db, da) - 1;
				p = -0x40000000;
			} else {
				const int U = rj.tMax < ri.tMax ? rj.tMax : ri.tMax;
				if (tieToJ ? db < da * U : db <= da * U) continue;
				p = tieToJ ? ceilDivI(db, da) : floorDivI(db, da) + 1;
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
			if (rmP[a] > cur) stampSegmentSimd(dst, tintRow, massRow, startX, ri, cur, rmP[a] - 1);
			if (rmQ[a] >= cur) cur = rmQ[a] + 1;
		}
		if (cur <= ri.tMax) stampSegmentSimd(dst, tintRow, massRow, startX, ri, cur, ri.tMax);
	}
}

// Current game row: white-fill the tint (the ambient baseline), stamp the
// surface buffer only, then give the base buffer the same bytes via one
// row-end copy (Rev 27 §4.3 — the old code wrote identical values to both
// buffers per pixel, so this is byte-identical; the tint field follows the
// same rule).
static void newMaxRow(uchar* dst, uchar* dstB, uint* tintDst, uint* tintDstB, ushort* massScratch, int startX, int W, const Light* lights, int k)
{
	if (tintDst) {
		for (int x = startX; x < W; x++) tintDst[x] = TINT_WHITE;
		memset(massScratch + startX, 0, (size_t)(W - startX) * sizeof(ushort));
	}
	envelopeRow(dst, tintDst, massScratch, startX, W, lights, k);
	memcpy(dstB + startX, dst + startX, (size_t)(W - startX));
	if (tintDstB) memcpy(tintDstB + startX, tintDst + startX, (size_t)(W - startX) * sizeof(uint));
}

// ---------------------------------------------------------------------------
// Random LUT / config generation (same scheme as the earlier identity tools)
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

static void genLights(const LutSet& ls, Light* lights, int& k)
{
	// (th4) Rev 2.46: 10% of the tests exercise the enlarged 33..127 slot
	// range; the rest stay in the classic 1..16 band.
	k = ((rnd() % 10) == 0) ? 33 + (int)(rnd() % (Lights_128_Count - 33)) : 1 + (int)(rnd() % 16);
	const bool clustered = (rnd() % 100) < 70;
	static const int clusterRads[4] = { 4, 8, 12, 15 };
	int baseRad = clusterRads[rnd() % 4];
	int clusterCount = clustered ? 2 + (int)(rnd() % 11) : 0;
	int cdr = (int)(rnd() % 4001) - 2000;
	int cdc = (int)(rnd() % 4001) - 2000;
	int spread = 32 + (int)(rnd() % 900);

	// (th4) Stage 3: mostly-white tints with a few strong colors — exercises
	// the winner-tint tracking without changing the light bytes.
	static const uchar tintCols[4][3] = {
		{255, 255, 255}, {255, 0, 0}, {0, 255, 0}, {64, 64, 255}
	};

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
		const int ti = (rnd() % 4) == 0 ? (int)(rnd() % 4) : 0;
		lights[i].tintR = tintCols[ti][0];
		lights[i].tintG = tintCols[ti][1];
		lights[i].tintB = tintCols[ti][2];
	}
}

// (th4) Rev 2.26: the game stamps WHITE lights before colored lights (stable
// partition in V2BuildStamps). Mirror it so the port and the reference see
// the same stamp order as the game.
static void sortWhiteFirst(Light* lights, int k)
{
	Light sorted[Lights_128_Count];
	int n = 0;
	for (int pass = 0; pass < 2; pass++) {
		for (int i = 0; i < k; i++) {
			const bool isWhite = lights[i].tintR == 255
				&& lights[i].tintG == 255 && lights[i].tintB == 255;
			if ((pass == 0) == isWhite) sorted[n++] = lights[i];
		}
	}
	for (int i = 0; i < k; i++) lights[i] = sorted[i];
}

// Test 1: current stamping == original scalar loop, random pre-fill.
static bool testStamps(const LutSet& ls, int testId)
{
	const int W = (rnd() % 2) ? 640 : 320;
	uchar dstOld[640], dstNew[640], dstBOld[640], dstBNew[640];
	uchar prefill[640];
	uint tintOld[640], tintNew[640], tintBOld[640], tintBNew[640];
	ushort massNew[640];
	for (int x = 0; x < W; x++) {
		dstOld[x] = (uchar)(rnd() & 255);
		dstNew[x] = dstOld[x];
		dstBOld[x] = dstOld[x];
		dstBNew[x] = dstOld[x];
		prefill[x] = dstOld[x];
		// (th4) Stage 3: ambient baseline = white tint on both sides.
		tintOld[x] = TINT_WHITE; tintNew[x] = TINT_WHITE;
		tintBOld[x] = TINT_WHITE; tintBNew[x] = TINT_WHITE;
	}

	Light lights[Lights_128_Count];
	int k;
	genLights(ls, lights, k);
	sortWhiteFirst(lights, k); // (th4) Rev 2.26: mirror the game's stamp order

	oldMaxRow(dstOld, dstBOld, W, lights, k);
	refTintWinnerRow(tintOld, W, lights, k);
	for (int x = 0; x < W; x++) tintBOld[x] = tintOld[x];
	newMaxRow(dstNew, dstBNew, tintNew, tintBNew, massNew, 0, W, lights, k);
	for (int x = 0; x < W; x++) {
		if (dstOld[x] != dstNew[x] || dstBOld[x] != dstBNew[x]) {
			printf("STAMP MISMATCH test %d at x=%d: old=%d new=%d newB=%d k=%d W=%d\n",
				testId, x, (int)dstOld[x], (int)dstNew[x], (int)dstBNew[x], k, W);
			// find which light set old[x] = 254 in the reference
			for (int i = 0; i < k; i++) {
				const Light& st = lights[i];
				const long long ssum = (long long)(st.drStart + st.dcStart) * (st.drStart + st.dcStart);
				if (ssum > 2LL * st.D64sq) { printf("  light[%d] OFF-ROW\n", i); continue; }
				int dr = st.drStart + x, dc = st.dcStart - x;
				int d2 = dr * dr + dc * dc;
				if (d2 > st.D64sq) { printf("  light[%d] dr=%d dc=%d D64=%d rad=%d OUT-OF-DISC d=%d\n",
					i, st.drStart, st.dcStart, st.D64, st.rad, (int)Isqrt32((unsigned)d2)); continue; }
				int d = (int)Isqrt32((unsigned)d2);
				printf("  light[%d] dr=%d dc=%d D64=%d rad=%d d=%d B=%d lv=%d\n",
					i, st.drStart, st.dcStart, st.D64, st.rad, d, (int)st.lut[d], 255 - (int)st.lut[d]);
			}
			return false;
		}
		// (th4) Stage 3: the winner tint must match the independent reference.
		if (tintOld[x] != tintNew[x] || tintBOld[x] != tintBNew[x]) {
			printf("TINT MISMATCH test %d at x=%d: ref=%08X new=%08X newB=%08X k=%d dst=%d\n",
				testId, x, tintOld[x], tintNew[x], tintBNew[x], k, (int)dstOld[x]);
			for (int i = 0; i < k; i++) {
				const Light& st = lights[i];
				const long long ssum = (long long)(st.drStart + st.dcStart) * (st.drStart + st.dcStart);
				if (ssum > 2LL * st.D64sq) { printf("  light[%d] OFF-ROW\n", i); continue; }
				int dr = st.drStart + x, dc = st.dcStart - x;
				int d2 = dr * dr + dc * dc;
				if (d2 > st.D64sq) { printf("  light[%d] rad=%d OUT-OF-DISC\n", i, st.rad); continue; }
				int d = (int)Isqrt32((unsigned)d2);
				printf("  light[%d] drStart=%d dcStart=%d rad=%d d=%d B=%d lv=%d tint=%02X%02X%02X\n",
					i, st.drStart, st.dcStart, st.rad, d, (int)st.lut[d], 255 - (int)st.lut[d],
					(int)st.tintR, (int)st.tintG, (int)st.tintB);
			}
			return false;
		}
	}
	return true;
}

// Test 2: full-bright fast path — stamps leave an all-zero buffer unchanged
// (the exact precondition behind the Rev 22 memset fast path).
static bool testFastPath(const LutSet& ls, int testId)
{
	const int W = (rnd() % 2) ? 640 : 320;
	uchar dstOld[640], dstNew[640], dstB[640];
	uint tintOld[640], tintNew[640], tintB[640];
	ushort massNew[640];
	memset(dstOld, 0, W);
	memset(dstNew, 0, W);
	memset(dstB, 0, W);
	for (int x = 0; x < W; x++) {
		tintOld[x] = TINT_WHITE; tintNew[x] = TINT_WHITE; tintB[x] = TINT_WHITE;
	}

	Light lights[Lights_128_Count];
	int k;
	genLights(ls, lights, k);
	sortWhiteFirst(lights, k); // (th4) Rev 2.26: mirror the game's stamp order

	oldMaxRow(dstOld, dstB, W, lights, k);
	refTintWinnerRow(tintOld, W, lights, k);
	newMaxRow(dstNew, dstB, tintNew, tintB, massNew, 0, W, lights, k);
	for (int x = 0; x < W; x++) {
		if (dstOld[x] != 0 || dstNew[x] != 0 || dstB[x] != 0) {
			printf("FASTPATH MISMATCH test %d at x=%d: old=%d new=%d B=%d k=%d\n",
				testId, x, (int)dstOld[x], (int)dstNew[x], (int)dstB[x], k);
			return false;
		}
		// (th4) Rev 2.44: on an all-zero field the LIGHT bytes stay 0 (the
		// Rev-22 memset fast-path precondition above), but the mass model
		// still paints each light's contribution into the tint field — a
		// light contributes its color regardless of whether it can strictly
		// brighten an already-full-bright pixel. The game never reaches this
		// state for tint: the zeroAmbient early-out returns right after the
		// white pre-fill and skips all stamps. So this test compares the
		// implementation against the independent reference instead of
		// requiring a white field.
		if (tintOld[x] != tintNew[x] || tintB[x] != tintNew[x]) {
			printf("FASTPATH TINT MISMATCH test %d at x=%d: ref=%08X new=%08X B=%08X k=%d\n",
				testId, x, tintOld[x], tintNew[x], tintB[x], k);
			return false;
		}
	}
	return true;
}

// Test 2b (Rev 2.37): deterministic EXACT-TIE scenarios. The envelope must
// stamp tie pixels EXACTLY ONCE (the lower stamp index keeps them), matching
// the independent reference's first-writer tie rule. The pre-Rev-2.37 strict
// removal double-stamped tie pixels: the tint crossfade compounded along the
// nearest-light bisectors (bright 1-px "sharp straight line edges" in a
// spammed charged-bolt field) and whole coincident discs doubled (the
// "crisp rotating sector disc").
static bool testTies(const LutSet& ls, int testId)
{
	const int W = 640;
	uchar dstOld[640], dstNew[640], dstBOld[640], dstBNew[640];
	uchar prefill[640];
	uint tintOld[640], tintNew[640], tintBOld[640], tintBNew[640];
	ushort massNew[640];
	for (int x = 0; x < W; x++) {
		dstOld[x] = (uchar)((x * 37 + 11) & 255);
		dstNew[x] = dstOld[x]; dstBOld[x] = dstOld[x]; dstBNew[x] = dstOld[x];
		prefill[x] = dstOld[x];
		tintOld[x] = TINT_WHITE; tintNew[x] = TINT_WHITE;
		tintBOld[x] = TINT_WHITE; tintBNew[x] = TINT_WHITE;
	}

	for (int c = 0; c < 3; c++) {
		for (int x = 0; x < W; x++) {
			dstOld[x] = (uchar)((x * 37 + 11) & 255);
			dstNew[x] = dstOld[x]; dstBOld[x] = dstOld[x]; dstBNew[x] = dstOld[x];
			prefill[x] = dstOld[x];
			tintOld[x] = TINT_WHITE; tintNew[x] = TINT_WHITE;
			tintBOld[x] = TINT_WHITE; tintBNew[x] = TINT_WHITE;
		}
		Light lights[4];
		int k = 0;
		// white light rad 3 — keeps the white-vs-colored value path exercised
		lights[k].drStart = 80; lights[k].dcStart = -40; lights[k].rad = 3;
		lights[k].tintR = 255; lights[k].tintG = 255; lights[k].tintB = 255; k++;
		// blue "portal" rad 8 — a colored winner under the bolts' tie line
		lights[k].drStart = -300; lights[k].dcStart = -40; lights[k].rad = 8;
		lights[k].tintR = 0; lights[k].tintG = 0; lights[k].tintB = 255; k++;
		// two bolts rad 4
		lights[k].drStart = 40; lights[k].dcStart = 120; lights[k].rad = 4;
		lights[k].tintR = 255; lights[k].tintG = 255; lights[k].tintB = 0; k++;
		lights[k].drStart = 40; lights[k].rad = 4;
		if (c == 0) {
			// symmetric bolts: exact-tie column at x = 0 (the bisector)
			lights[k].dcStart = -120;
			lights[k].tintR = 255; lights[k].tintG = 255; lights[k].tintB = 0;
		} else if (c == 1) {
			// coincident centers: every pixel of the disc ties
			lights[k].dcStart = 120;
			lights[k].tintR = 255; lights[k].tintG = 255; lights[k].tintB = 0;
		} else {
			// coincident centers, DIFFERENT colors: no envelope removal
			// (Rev 2.26 guard) — both stamp, order-dependent crossfade,
			// which the reference models with per-color classes
			lights[k].dcStart = 120;
			lights[k].tintR = 255; lights[k].tintG = 0; lights[k].tintB = 0;
		}
		k++;
		for (int i = 0; i < k; i++) {
			lights[i].D64 = ls.d64[lights[i].rad];
			lights[i].D64sq = lights[i].D64 * lights[i].D64;
			lights[i].lut = ls.row[lights[i].rad];
		}
		sortWhiteFirst(lights, k);

		oldMaxRow(dstOld, dstBOld, W, lights, k);
		refTintWinnerRow(tintOld, W, lights, k);
		for (int x = 0; x < W; x++) tintBOld[x] = tintOld[x];
		newMaxRow(dstNew, dstBNew, tintNew, tintBNew, massNew, 0, W, lights, k);
		for (int x = 0; x < W; x++) {
			if (dstOld[x] != dstNew[x] || dstBOld[x] != dstBNew[x]) {
				printf("TIE-STAMP MISMATCH test %d case %d at x=%d: old=%d new=%d newB=%d\n",
					testId, c, x, (int)dstOld[x], (int)dstNew[x], (int)dstBNew[x]);
				return false;
			}
			if (tintOld[x] != tintNew[x] || tintBOld[x] != tintBNew[x]) {
				printf("TIE-TINT MISMATCH test %d case %d at x=%d: ref=%08X new=%08X newB=%08X\n",
					testId, c, x, tintOld[x], tintNew[x], tintBNew[x]);
				return false;
			}
		}
	}
	return true;
}

// Test 3 (Rev 24): frame-level row-order independence — the exact property
// the MT dispatch relies on.  Simulates a real frame projection: per row
// wsY, drStart = rowIsoRow64 - centerRow64 etc. (same formulas as the game),
// then fills the frame once in row order and once in a shuffled order using
// the current game row code (SIMD segment + envelope + early-out).  Buffers
// must be byte-identical — MT workers execute exactly these rows in an
// arbitrary order.
struct FLight { int centerRow64, centerCol64, D64, D64sq, rad; const uchar* lut; uchar tintR, tintG, tintB; };

static void runRow(uchar* dst, uchar* dstB, uint* tintDst, uint* tintDstB, ushort* massScratch, int y, int W, int startX, int X0, int Y0,
	const FLight* fl, int k)
{
	const int dy2 = 2 * y;
	const int rowIsoRow64 = startX + dy2 - X0 - 2 * Y0;
	const int rowIsoCol64 = dy2 - startX - 2 * Y0 + X0;
	Light rl[Lights_128_Count];
	for (int i = 0; i < k; i++) {
		rl[i].drStart = rowIsoRow64 - fl[i].centerRow64;
		rl[i].dcStart = rowIsoCol64 - fl[i].centerCol64;
		rl[i].D64 = fl[i].D64;
		rl[i].D64sq = fl[i].D64sq;
		rl[i].rad = fl[i].rad;
		rl[i].lut = fl[i].lut;
		rl[i].tintR = fl[i].tintR;
		rl[i].tintG = fl[i].tintG;
		rl[i].tintB = fl[i].tintB;
	}
	newMaxRow(dst, dstB, tintDst, tintDstB, massScratch, startX, W, rl, k);
}

static bool testFrameOrder(const LutSet& ls, int testId)
{
	const int W = 640, H = 120;
	const int startX = 0;
	const int X0 = (int)(rnd() % 20001) - 10000;
	const int Y0 = (int)(rnd() % 20001) - 10000;

	FLight fl[Lights_128_Count];
	// (th4) Rev 2.46: 10% of the tests exercise the enlarged 33..127 slot
	// range; the rest stay in the classic 2..16 band.
	int k = ((rnd() % 10) == 0) ? 33 + (int)(rnd() % (Lights_128_Count - 33)) : 2 + (int)(rnd() % 15);
	// fire-wall-like same-radius cluster so stamps actually matter
	{
		int cr = (int)(rnd() % 4001) - 2000;
		int cc = (int)(rnd() % 4001) - 2000;
		int rad = 12;
		for (int i = 0; i < k; i++) {
			if (i < k - 3) {
				fl[i].centerRow64 = cr + (int)(rnd() % 641) - 320;
				fl[i].centerCol64 = cc + (int)(rnd() % 641) - 320;
				fl[i].D64 = ls.d64[rad];
				fl[i].D64sq = fl[i].D64 * fl[i].D64;
				fl[i].rad = rad;
				fl[i].lut = ls.row[rad];
			} else {
				// a few scattered mixed-radius lights (player light, torches)
				int r2 = (int)(rnd() % 16);
				fl[i].centerRow64 = (int)(rnd() % 14001) - 7000;
				fl[i].centerCol64 = (int)(rnd() % 14001) - 7000;
				fl[i].D64 = ls.d64[r2];
				fl[i].D64sq = fl[i].D64 * fl[i].D64;
				fl[i].rad = r2;
				fl[i].lut = ls.row[r2];
			}
			// (th4) Stage 3: one light in four carries a colored tint.
			const int ti = (rnd() % 4) == 0 ? (int)(rnd() % 3) + 1 : 0;
			static const uchar tintCols[4][3] = {
				{255, 255, 255}, {255, 0, 0}, {0, 255, 0}, {64, 64, 255}
			};
			fl[i].tintR = tintCols[ti][0];
			fl[i].tintG = tintCols[ti][1];
			fl[i].tintB = tintCols[ti][2];
		}
	}
	// (th4) Rev 2.26: mirror the game's white-first stamp order (stable).
	{
		FLight sorted[Lights_128_Count];
		int n = 0;
		for (int pass = 0; pass < 2; pass++) {
			for (int i = 0; i < k; i++) {
				const bool isWhite = fl[i].tintR == 255
					&& fl[i].tintG == 255 && fl[i].tintB == 255;
				if ((pass == 0) == isWhite) sorted[n++] = fl[i];
			}
		}
		for (int i = 0; i < k; i++) fl[i] = sorted[i];
	}

	static uchar frameA[120 * 640], frameB[120 * 640];
	static uchar frameAB[120 * 640], frameBB[120 * 640];
	static uint tintA[120 * 640], tintB[120 * 640];      // implementation output
	static uint tintAB[120 * 640], tintBB[120 * 640];    // implementation base copy
	static uint tintARef[120 * 640], tintBRef[120 * 640]; // independent reference
	static ushort massScratch[640];                       // Rev 2.45 true-mass scratch (row-scoped, reused per row)
	for (int y = 0; y < H; y++) {
		uchar* ra = frameA + y * W;
		uchar* rb = frameB + y * W;
		for (int x = 0; x < W; x++) {
			ra[x] = (uchar)(rnd() & 255);
			rb[x] = ra[x];
			// (th4) Stage 3: white tint baseline on both frames.
			tintA[y * W + x] = TINT_WHITE;
			tintB[y * W + x] = TINT_WHITE;
			tintARef[y * W + x] = TINT_WHITE;
			tintBRef[y * W + x] = TINT_WHITE;
		}
		memcpy(frameAB + y * W, ra, W);
		memcpy(frameBB + y * W, rb, W);
		memcpy(tintAB + y * W, tintA + y * W, W * sizeof(uint));
		memcpy(tintBB + y * W, tintB + y * W, W * sizeof(uint));
	}

	// sequential row order
	for (int y = 0; y < H; y++) {
		// (th4) Stage 3: independent per-pixel tint winner reference.
		Light rlRef[Lights_128_Count];
		const int dy2 = 2 * y;
		const int rowIsoRow64 = startX + dy2 - X0 - 2 * Y0;
		const int rowIsoCol64 = dy2 - startX - 2 * Y0 + X0;
		for (int i = 0; i < k; i++) {
			rlRef[i].drStart = rowIsoRow64 - fl[i].centerRow64;
			rlRef[i].dcStart = rowIsoCol64 - fl[i].centerCol64;
			rlRef[i].D64 = fl[i].D64;
			rlRef[i].D64sq = fl[i].D64sq;
			rlRef[i].rad = fl[i].rad;
			rlRef[i].lut = fl[i].lut;
			rlRef[i].tintR = fl[i].tintR;
			rlRef[i].tintG = fl[i].tintG;
			rlRef[i].tintB = fl[i].tintB;
		}
		refTintWinnerRow(tintARef + y * W, W, rlRef, k);
		runRow(frameA + y * W, frameAB + y * W, tintA + y * W, tintAB + y * W, massScratch, y, W, startX, X0, Y0, fl, k);
	}

	// shuffled row order (simulates work-stealing across threads)
	int order[120];
	for (int i = 0; i < H; i++) order[i] = i;
	for (int i = H - 1; i > 0; i--) {
		int j = (int)(rnd() % (unsigned)(i + 1));
		int t = order[i]; order[i] = order[j]; order[j] = t;
	}
	for (int y = 0; y < H; y++) {
		// (th4) Stage 3: independent per-pixel tint winner reference.
		Light rlRef[Lights_128_Count];
		const int dy2 = 2 * order[y];
		const int rowIsoRow64 = startX + dy2 - X0 - 2 * Y0;
		const int rowIsoCol64 = dy2 - startX - 2 * Y0 + X0;
		for (int i = 0; i < k; i++) {
			rlRef[i].drStart = rowIsoRow64 - fl[i].centerRow64;
			rlRef[i].dcStart = rowIsoCol64 - fl[i].centerCol64;
			rlRef[i].D64 = fl[i].D64;
			rlRef[i].D64sq = fl[i].D64sq;
			rlRef[i].rad = fl[i].rad;
			rlRef[i].lut = fl[i].lut;
			rlRef[i].tintR = fl[i].tintR;
			rlRef[i].tintG = fl[i].tintG;
			rlRef[i].tintB = fl[i].tintB;
		}
		refTintWinnerRow(tintBRef + order[y] * W, W, rlRef, k);
		runRow(frameB + order[y] * W, frameBB + order[y] * W, tintB + order[y] * W, tintBB + order[y] * W, massScratch, order[y], W, startX, X0, Y0, fl, k);
	}

	for (int i = 0; i < W * H; i++) {
		if (frameA[i] != frameB[i] || frameAB[i] != frameBB[i]) {
			printf("FRAME-ORDER MISMATCH test %d at %d (row %d): seq=%d shuffled=%d\n",
				testId, i, i / W, (int)frameA[i], (int)frameB[i]);
			return false;
		}
		// (th4) Stage 3: row order must not change the tint field either
		// (the property the MT dispatch relies on), and it must match the
		// independent per-pixel reference.
		if (tintA[i] != tintARef[i] || tintB[i] != tintBRef[i] || tintAB[i] != tintBB[i]) {
			printf("FRAME-ORDER TINT MISMATCH test %d at %d (row %d): seq=%08X refSeq=%08X shuffled=%08X refShuf=%08X\n",
				testId, i, i / W, tintA[i], tintARef[i], tintB[i], tintBRef[i]);
			return false;
		}
	}
	return true;
}

int main()
{
	if (!InitTintWeightLut()) {
		printf("TINT WEIGHT LUT CHECK FAILED (table != formula)\n");
		return 1;
	}
	LutSet ls;
	genLut(ls);
	const int STAMP_TESTS = 20000;
	const int FASTPATH_TESTS = 5000;
	const int FRAMEORDER_TESTS = 200;
	for (int t = 0; t < STAMP_TESTS; t++) {
		if (!testStamps(ls, t)) {
			printf("FAILED (stamps) at test %d\n", t);
			return 1;
		}
	}
	for (int t = 0; t < FASTPATH_TESTS; t++) {
		if (!testFastPath(ls, t)) {
			printf("FAILED (fast path) at test %d\n", t);
			return 1;
		}
	}
	for (int t = 0; t < FRAMEORDER_TESTS; t++) {
		if (!testFrameOrder(ls, t)) {
			printf("FAILED (frame order) at test %d\n", t);
			return 1;
		}
	}
	if (!testTies(ls, 0)) {
		printf("FAILED (ties)\n");
		return 1;
	}
	printf("ALL %d STAMP + %d FASTPATH + %d FRAME-ORDER + TIE TESTS IDENTICAL (light bytes + Stage-3 tint field)\n",
		STAMP_TESTS, FASTPATH_TESTS, FRAMEORDER_TESTS);
	return 0;
}
