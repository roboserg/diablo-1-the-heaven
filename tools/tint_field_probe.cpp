// tint_field_probe.cpp — Rev 2.44 seam analysis of the Stage-3 tint field
// (charged-bolt / portal / player-light interactions).
//
// Computes the per-pixel tint field with TWO models:
//   OLD = the pre-Rev-2.44 win/lose pairwise crossfade (Rev 2.30 rule)
//   NEW = the Rev 2.45 true-mass brightness-weighted average (the Rev 2.44
//         clamped-mass rule as its base)
// over the same per-(radius, tint)-class reduced stamping set in stamp order,
// and measures the largest adjacent-pixel channel jump (the "sharp seam").
//
// Build (from c:\th4\tools):
//   cmd /c 'call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x86 >nul 2>&1 & cl /nologo /EHsc /O2 /fp:fast tint_field_probe.cpp'
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

typedef unsigned char uchar;
typedef unsigned int uint;

static const int N64 = 64; // grid resolution in 1/64-tile units (probe = every 8)

static unsigned Isqrt32(unsigned n)
{
	unsigned res = 0, bit = 1u << 30;
	while (bit > n) bit >>= 2;
	while (bit) {
		if (n >= res + bit) { n -= res + bit; res = (res >> 1) + bit; }
		else res >>= 1;
		bit >>= 2;
	}
	return res;
}

struct Light {
	int cr, cc;     // center row64 / col64 (1/64-tile units)
	int rad;
	uchar r, g, b;  // tint (255,255,255 = white)
};

static uchar g_Lut[16][1025];

static void BuildLut()
{
	for (int rad = 0; rad < 16; rad++) {
		const int dMax = 64 * (rad + 1);
		for (int i = 0; i < 1025; i++) {
			if (i > dMax) { g_Lut[rad][i] = 0; continue; }
			double f = (double)i / (double)dMax;
			double oneMinusF = 1.0 - f;
			g_Lut[rad][i] = (uchar)(255.0 * oneMinusF * oneMinusF + 0.5);
		}
	}
}

static inline uchar BAt(const Light& l, int rr, int cc)
{
	int dr = rr - l.cr, dc = cc - l.cc;
	unsigned d2 = (unsigned)((long long)dr * dr + (long long)dc * dc);
	unsigned d = Isqrt32(d2);
	const int D64 = 64 * (l.rad + 1);
	if (d > (unsigned)D64) return 0;
	return g_Lut[l.rad][d];
}

static inline unsigned d2At(const Light& l, int rr, int cc)
{
	int dr = rr - l.cr, dc = cc - l.cc;
	return (unsigned)((long long)dr * dr + (long long)dc * dc);
}

// OLD (pre-Rev-2.44): win/lose pairwise crossfade over the stamping set in
// stamp order (the Rev 2.30 writeTintPixel rule).
static uint scanWordOld(const Light* lights, const int* order, int n, int rr, int cc, int ambient)
{
	int D = ambient;
	uint oldWord = 0x00FFFFFFu;
	for (int oi = 0; oi < n; oi++) {
		const Light& st = lights[order[oi]];
		const int B = (int)BAt(st, rr, cc);
		const int lv = 255 - B;
		const uint word = (uint)st.b | ((uint)st.g << 8) | ((uint)st.r << 16);
		const uint oldA = oldWord >> 24;
		const uint oldRgb = oldWord & 0x00FFFFFFu;
		if (lv < D) {
			const uint beat = (uint)(D - lv);
			if (oldA != 0 && oldRgb != 0x00FFFFFFu) {
				const uint Bp = 255 - (uint)D;
				const uint sum = (uint)B + Bp;
				const uint w = ((uint)B * 255 + (sum >> 1)) / sum;
				const uint wr = (word >> 16) & 0xFF, wg = (word >> 8) & 0xFF, wb = word & 0xFF;
				const uint orr = (oldWord >> 16) & 0xFF, og = (oldWord >> 8) & 0xFF, ob = oldWord & 0xFF;
				const uint nr = (wr * w + orr * (255 - w) + 127) >> 8;
				const uint ng = (wg * w + og * (255 - w) + 127) >> 8;
				const uint nb = (wb * w + ob * (255 - w) + 127) >> 8;
				uint na = oldA + beat; if (na > 255) na = 255;
				oldWord = nb | (ng << 8) | (nr << 16) | (na << 24);
			} else {
				oldWord = word | (beat << 24);
			}
			D = lv;
		} else if (B != 0 && oldA != 0 && oldRgb != 0x00FFFFFFu) {
			const uint Bp = 255 - (uint)D;
			const uint sum = (uint)B + Bp;
			const uint w = ((uint)B * 255 + (sum >> 1)) / sum;
			const uint wr = (word >> 16) & 0xFF, wg = (word >> 8) & 0xFF, wb = word & 0xFF;
			const uint orr = (oldWord >> 16) & 0xFF, og = (oldWord >> 8) & 0xFF, ob = oldWord & 0xFF;
			const uint nr = (wr * w + orr * (255 - w) + 127) >> 8;
			const uint ng = (wg * w + og * (255 - w) + 127) >> 8;
			const uint nb = (wb * w + ob * (255 - w) + 127) >> 8;
			oldWord = nb | (ng << 8) | (nr << 16) | (oldA << 24);
		}
	}
	return oldWord;
}

// NEW (Rev 2.45): brightness-mass weighted average over the stamping set,
// with the TRUE (unclamped) mass as the weight base. Every in-disc light
// contributes: mass 0 -> plain replace word | B<<24; mass > 0 -> RGB
// crossfade toward the incoming color with w = (B*255 + (m+B)/2)/(m+B) and
// the word's A byte = min(255, m+B). White lights are ordinary
// contributors, so the result depends only on the per-pixel light set (no
// win/lose event-sequence branches).
static uint scanWordMass(const Light* lights, const int* order, int n, int rr, int cc)
{
	uint mass = 0;
	uint word = 0x00FFFFFFu;
	for (int oi = 0; oi < n; oi++) {
		const Light& st = lights[order[oi]];
		const int B = (int)BAt(st, rr, cc);
		if (B == 0) continue;
		const uint wrd = (uint)st.b | ((uint)st.g << 8) | ((uint)st.r << 16);
		if (mass == 0) {
			word = wrd | ((uint)B << 24);
			mass = (uint)B;
		} else {
			const uint sum = mass + (uint)B;
			const uint w = ((uint)B * 255 + (sum >> 1)) / sum;
			const uint wr = (wrd >> 16) & 0xFF, wg = (wrd >> 8) & 0xFF, wb = wrd & 0xFF;
			const uint orr = (word >> 16) & 0xFF, og = (word >> 8) & 0xFF, ob = word & 0xFF;
			const uint nr = (wr * w + orr * (255 - w) + 127) >> 8;
			const uint ng = (wg * w + og * (255 - w) + 127) >> 8;
			const uint nb = (wb * w + ob * (255 - w) + 127) >> 8;
			mass = sum;                       // Rev 2.45: TRUE mass, no clamp
			const uint na = sum > 255 ? 255 : sum;
			word = nb | (ng << 8) | (nr << 16) | (na << 24);
		}
	}
	return word;
}

// Per-pixel stamping set: per-(radius,tint)-class nearest light (mirrors the
// game's Rev 2.26 envelope), emitted in array order (callers order the
// array whites-first like the game's stable partition).
static int classOrder(const Light* lights, int k, int rr, int cc, int* order)
{
	int clsIdx[16], clsD2[16], clsCount = 0;
	for (int i = 0; i < k; i++) {
		if (BAt(lights[i], rr, cc) == 0) continue;
		const unsigned d2 = d2At(lights[i], rr, cc);
		int c = -1;
		for (int t = 0; t < clsCount; t++) {
			const Light& o = lights[clsIdx[t]];
			if (o.rad == lights[i].rad && o.r == lights[i].r
				&& o.g == lights[i].g && o.b == lights[i].b) { c = t; break; }
		}
		if (c < 0) { c = clsCount++; clsIdx[c] = i; clsD2[c] = d2; }
		else if (d2 < clsD2[c]) { clsIdx[c] = i; clsD2[c] = d2; }
	}
	int n = 0;
	for (int oi = 0; oi < k; oi++)
		for (int t = 0; t < clsCount; t++)
			if (clsIdx[t] == oi) { order[n++] = oi; break; }
	return n;
}

// Fill OLD and NEW grids and report the largest adjacent-pixel channel jump
// (RGB + A bytes) and the number of jumps above a visibility threshold.
static void measure(const char* name, const Light* lights, int k, int ambient,
	int minC, int maxC, int step)
{
	const int w = (maxC - minC) / step + 1;
	static uint oldGrid[257 * 257], newGrid[257 * 257];
	long long oldJumps = 0, newJumps = 0, rgbDiff = 0, aDiff = 0;
	int oldMax = 0, newMax = 0;

	for (int rr = minC; rr <= maxC; rr += step) {
		for (int cc = minC; cc <= maxC; cc += step) {
			int order[16];
			const int n = classOrder(lights, k, rr, cc, order);
			const uint wo = scanWordOld(lights, order, n, rr, cc, ambient);
			const uint wn = scanWordMass(lights, order, n, rr, cc);
			const int gi = ((rr - minC) / step) * w + (cc - minC) / step;
			oldGrid[gi] = wo;
			newGrid[gi] = wn;
			if ((wo & 0x00FFFFFFu) != (wn & 0x00FFFFFFu)) rgbDiff++;
			if ((wo >> 24) != (wn >> 24)) aDiff++;
		}
	}
	for (int rr = minC; rr <= maxC; rr += step) {
		for (int cc = minC; cc <= maxC; cc += step) {
			const int gi = ((rr - minC) / step) * w + (cc - minC) / step;
			for (int dr = -step; dr <= step; dr += step) {
				for (int dc = -step; dc <= step; dc += step) {
					if (dr == 0 && dc == 0) continue;
					const int nr = rr + dr, nc = cc + dc;
					if (nr < minC || nr > maxC || nc < minC || nc > maxC) continue;
					const int ni = ((nr - minC) / step) * w + (nc - minC) / step;
					int dOld = 0, dNew = 0;
					for (int ch = 0; ch < 32; ch += 8) {
						int a = (int)((oldGrid[gi] >> ch) & 0xFF) - (int)((oldGrid[ni] >> ch) & 0xFF);
						int b = (int)((newGrid[gi] >> ch) & 0xFF) - (int)((newGrid[ni] >> ch) & 0xFF);
						if (a < 0) a = -a; if (b < 0) b = -b;
						if (a > dOld) dOld = a;
						if (b > dNew) dNew = b;
					}
					if (dOld > oldMax) oldMax = dOld;
					if (dNew > newMax) newMax = dNew;
					if (dOld > 16) oldJumps++;
					if (dNew > 16) newJumps++;
				}
			}
		}
	}
	printf("%s: OLD maxAdj=%d jumps>16=%lld | NEW maxAdj=%d jumps>16=%lld | RGBdiff=%lld Adiff=%lld\n",
		name, oldMax, oldJumps, newMax, newJumps, rgbDiff, aDiff);
}

int main()
{
	BuildLut();

	// (A) the reported scene: blue portal rad15 + orange floor lamp rad3 +
	// white player light rad9 (whites-first canonical stamp order).
	{
		Light l[3];
		l[0].cr = 0;    l[0].cc = 0;    l[0].rad = 15; l[0].r = 0;   l[0].g = 0;   l[0].b = 255;
		l[1].cr = 128;  l[1].cc = 64;   l[1].rad = 3;  l[1].r = 255; l[1].g = 190; l[1].b = 100;
		l[2].cr = 64;   l[2].cc = -32;  l[2].rad = 9;  l[2].r = 255; l[2].g = 255; l[2].b = 255;
		Light s[3] = { l[2], l[1], l[0] };
		measure("A portal+lamp+player", s, 3, 255, -6 * 64, 6 * 64, 4);
	}
	// (B) two lamps + player — the scene the user reported as clean.
	{
		Light l[3];
		l[0].cr = 128;  l[0].cc = 64;   l[0].rad = 3; l[0].r = 255; l[0].g = 190; l[0].b = 100;
		l[1].cr = 128;  l[1].cc = -64;  l[1].rad = 3; l[1].r = 255; l[1].g = 190; l[1].b = 100;
		l[2].cr = 0;    l[2].cc = 0;    l[2].rad = 9; l[2].r = 255; l[2].g = 255; l[2].b = 255;
		Light s[3] = { l[2], l[0], l[1] };
		measure("B lamp+lamp+player", s, 3, 255, -6 * 64, 6 * 64, 4);
	}
	// (C) portal + firebolt, no white light — the previously verified
	// 2-color scene: OLD and NEW RGB must match (RGBdiff == 0); only the A
	// byte (strength) changes: winner brightness -> accumulated mass.
	{
		Light l[2];
		l[0].cr = 0;   l[0].cc = 0;   l[0].rad = 15; l[0].r = 0;   l[0].g = 0;   l[0].b = 255;
		l[1].cr = 160; l[1].cc = 40;  l[1].rad = 5;  l[1].r = 255; l[1].g = 0;   l[1].b = 0;
		measure("C portal+firebolt", l, 2, 255, -6 * 64, 6 * 64, 4);
	}
	return 0;
}
