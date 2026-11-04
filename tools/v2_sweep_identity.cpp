// v2_sweep_identity.cpp
//
// Standalone verification that the new row-sweep stamping in
// FillScreenSpaceRowV2 (src\Draw.cpp) produces BYTE-IDENTICAL output to the
// old per-light stamp loop, for both max-mode and bounded-additive mode.
// Ports both loops verbatim from Draw.cpp (Rev 18).
//
// Build:  cl /nologo /EHsc /O2 v2_sweep_identity.cpp
// Run:    v2_sweep_identity.exe
// Exit:   0 = all identical, 1 = mismatch found.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>

typedef unsigned char uchar;

// (th4) Rev 2.46: the game's runtime light-slot capacity is now 128
// (Lights_128_Count). The sweep's event/active arrays mirror it.
static const int Lights_128_Count = 128;

static unsigned Isqrt32(unsigned n)
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

// LUT: simplified (1-f)^2 / improved crypt blend rows — this is a LOOP
// IDENTITY test (sweep vs direct), not a curve-parity test; the LUT shape
// is arbitrary as long as both loops read the same table.
static uchar g_Lut[16][1025];
static void BuildLut(int graphType)
{
	for (int rad = 0; rad < 16; rad++) {
		uchar* dst = g_Lut[rad];
		const int dMax = 64 * (rad + 1);
		if (graphType >= 5) {
			const double blend = rad * 1.25 / 15.0 > 1.0 ? 1.0 : rad * 1.25 / 15.0;
			for (int i = 0; i <= 1024; i++) {
				if (i > dMax) { dst[i] = 0; continue; }
				const double f = (double)i / (double)dMax;
				const double g = blend * f + (1.0 - blend) * f * f;
				dst[i] = (uchar)(255.0 * (1.0 - g) + 0.5);
			}
		} else {
			for (int i = 0; i <= 1024; i++) {
				if (i > dMax) { dst[i] = 0; continue; }
				const double f = (double)i / (double)dMax;
				const double oneMinusF = 1.0 - f;
				dst[i] = (uchar)(255.0 * oneMinusF * oneMinusF + 0.5);
			}
		}
	}
}

struct Stamp {
	int centerRow64, centerCol64;
	int D64, D64sq;
	const uchar* lut;
};

static int ceilDiv2(int x) { return x >= 0 ? (x + 1) / 2 : x / 2; }
static int floorDiv2(int x) { return x >= 0 ? x / 2 : (x - 1) / 2; }

// OLD per-light loop (verbatim port of the pre-sweep code, single-buffer form).
static void OldStamps(uchar* dst, const uchar* ambient, const Stamp* stamps, int stampCount,
                      int startX, int endX, int rowIsoRow64, int rowIsoCol64, bool additive)
{
	for (int s = 0; s < stampCount; s++) {
		const Stamp& st = stamps[s];
		const int drStart = rowIsoRow64 - st.centerRow64;
		const int dcStart = rowIsoCol64 - st.centerCol64;
		const long long ssum = (long long)(drStart + dcStart) * (drStart + dcStart);
		if (ssum > 2LL * st.D64sq) continue;
		const int b = dcStart - drStart;
		const int halfSpan = (int)Isqrt32((unsigned)(2 * st.D64sq - (int)ssum));
		int tMin = ceilDiv2(b - halfSpan);
		int tMax = floorDiv2(b + halfSpan);
		if (tMin < 0) tMin = 0;
		if (tMax > endX - startX - 1) tMax = endX - startX - 1;
		if (tMin > tMax) continue;

		const int t0 = tMin;
		int dr = drStart + t0;
		int dc = dcStart - t0;
		int d2 = dr * dr + dc * dc;
		int d = (int)Isqrt32((unsigned)d2);
		int dsq = d * d;
		int delta = 2 * (dr - dc) + 2;

		const uchar* lut = st.lut;
		const int D64 = st.D64;
		for (int x = startX + tMin; x <= startX + tMax; x++) {
			if ((unsigned)d <= (unsigned)D64) {
				const int B = (int)lut[d];
				if (additive) {
					const int c = B + (int)ambient[x] - 255;
					if (c > 0) {
						int nd = dst[x] - c;
						if (nd < 0) nd = 0;
						dst[x] = (uchar)nd;
					}
				} else {
					const int lv = 255 - B;
					if (lv < dst[x]) dst[x] = (uchar)lv;
				}
			}
			d2 += delta;
			delta += 4;
			while (d2 >= dsq + 2 * d + 1) { dsq += 2 * d + 1; d++; }
			while (d > 0 && d2 < dsq) { dsq -= 2 * d - 1; d--; }
		}
	}
}

// NEW row sweep (verbatim port of the Rev 18 sweep in FillScreenSpaceRowV2).
static void NewStamps(uchar* dst, const uchar* ambient, const Stamp* stamps, int stampCount,
                      int startX, int endX, int rowIsoRow64, int rowIsoCol64, bool additive)
{
	struct RowEvent { int pos; int light; bool isEnd; };
	RowEvent events[2 * Lights_128_Count];
	int evCount = 0;
	int d2A[Lights_128_Count], dA[Lights_128_Count], dsqA[Lights_128_Count], deltaA[Lights_128_Count];
	int activeList[Lights_128_Count];
	int activeCount = 0;

	const int width = endX - startX;
	for (int s = 0; s < stampCount; s++) {
		const Stamp& st = stamps[s];
		const int drStart = rowIsoRow64 - st.centerRow64;
		const int dcStart = rowIsoCol64 - st.centerCol64;
		const long long ssum = (long long)(drStart + dcStart) * (drStart + dcStart);
		if (ssum > 2LL * st.D64sq) continue;
		const int b = dcStart - drStart;
		const int halfSpan = (int)Isqrt32((unsigned)(2 * st.D64sq - (int)ssum));
		int tMin = ceilDiv2(b - halfSpan);
		int tMax = floorDiv2(b + halfSpan);
		if (tMin < 0) tMin = 0;
		if (tMax > width - 1) tMax = width - 1;
		if (tMin > tMax) continue;
		events[evCount++] = { tMin, s, false };
		events[evCount++] = { tMax + 1, s, true };
	}
	if (evCount == 0) return;

	for (int i = 1; i < evCount; i++) {
		const RowEvent e = events[i];
		int j = i - 1;
		while (j >= 0 && events[j].pos > e.pos) { events[j + 1] = events[j]; j--; }
		events[j + 1] = e;
	}

	auto Activate = [&](int e) {
		const int s = events[e].light;
		const Stamp& st = stamps[s];
		const int t = events[e].pos;
		const int dr = rowIsoRow64 - st.centerRow64 + t;
		const int dc = rowIsoCol64 - st.centerCol64 - t;
		const int d2v = dr * dr + dc * dc;
		const int dv = (int)Isqrt32((unsigned)d2v);
		d2A[s] = d2v; dA[s] = dv; dsqA[s] = dv * dv; deltaA[s] = 2 * (dr - dc) + 2;
		activeList[activeCount++] = s;
	};
	auto Remove = [&](int s) {
		for (int a = 0; a < activeCount; a++) {
			if (activeList[a] == s) { activeList[a] = activeList[activeCount - 1]; activeCount--; return; }
		}
	};
	auto AdvanceActives = [&]() {
		for (int a = 0; a < activeCount; a++) {
			const int s = activeList[a];
			d2A[s] += deltaA[s];
			deltaA[s] += 4;
			while (d2A[s] >= dsqA[s] + 2 * dA[s] + 1) { dsqA[s] += 2 * dA[s] + 1; dA[s]++; }
			while (dA[s] > 0 && d2A[s] < dsqA[s]) { dsqA[s] -= 2 * dA[s] - 1; dA[s]--; }
		}
	};

	int eIdx = 0;
	while (eIdx < evCount) {
		const int cur = events[eIdx].pos;
		if (cur >= width) break;
		while (eIdx < evCount && events[eIdx].pos == cur) {
			const int s = events[eIdx].light;
			if (events[eIdx].isEnd) Remove(s); else Activate(eIdx);
			eIdx++;
		}
		const int nextPos = eIdx < evCount ? events[eIdx].pos : width;
		if (activeCount > 0 && cur < nextPos) {
			if (additive) {
				for (int px = cur; px < nextPos; px++) {
					const int av = (int)ambient[px];
					int total = 0;
					for (int a = 0; a < activeCount; a++) {
						const int s = activeList[a];
						const int B = (int)stamps[s].lut[dA[s]];
						const int c = B + av - 255;
						if (c > 0) total += c;
					}
					if (total > 0) {
						int nd = dst[px] - total;
						if (nd < 0) nd = 0;
						dst[px] = (uchar)nd;
					}
					AdvanceActives();
				}
			} else {
				for (int px = cur; px < nextPos; px++) {
					int Bmax = 0;
					for (int a = 0; a < activeCount; a++) {
						const int s = activeList[a];
						const int B = (int)stamps[s].lut[dA[s]];
						if (B > Bmax) Bmax = B;
					}
					if (Bmax > 0) {
						const int lv = 255 - Bmax;
						if (lv < dst[px]) dst[px] = (uchar)lv;
					}
					AdvanceActives();
				}
			}
		}
		if (eIdx >= evCount) break;
	}
}

int main()
{
	unsigned seed = 0xC0FFEE;
	auto rnd = [&seed]() -> unsigned {
		seed = seed * 1103515245 + 12345;
		return seed;
	};

	int failures = 0;
	for (int gt = 0; gt <= 6; gt++) {
		BuildLut(gt);
		for (int iter = 0; iter < 20000; iter++) {
			const int startX = 0;
			const int endX = 640 + (int)(rnd() % 640); // row widths 640..1279
			const int width = endX - startX;
			const int rowIsoRow64 = (int)(rnd() % 200000) - 100000;
			const int rowIsoCol64 = (int)(rnd() % 200000) - 100000;

			// (th4) Rev 2.46: 10% of the tests exercise the enlarged 33..127
			// slot range; the rest stay in the classic 1..16 band.
			const int stampCount = ((rnd() % 10) == 0) ? 33 + (int)(rnd() % (Lights_128_Count - 33)) : 1 + (int)(rnd() % 16);
			Stamp stamps[Lights_128_Count];
			for (int s = 0; s < stampCount; s++) {
				int rad = (int)(rnd() % 16);
				const uchar* lut = g_Lut[rad];
				int lo = 0, hi = 1025;
				while (lo + 1 < hi) { int m = (lo + hi) >> 1; if (lut[m] > 0) lo = m; else hi = m; }
				// Simulate the ambient-limited prune with a random threshold.
				int D64 = lo;
				const int threshold = (int)(rnd() % 256);
				if (threshold > 0) {
					int l2 = 0, h2 = lo + 1;
					while (l2 + 1 < h2) { int m = (l2 + h2) >> 1; if (lut[m] > threshold) l2 = m; else h2 = m; }
					D64 = l2;
				}
				if (D64 <= 0) { s--; continue; } // pruned away
				// Center: inside/near the row's viewport in iso space.
				stamps[s].centerRow64 = rowIsoRow64 - (int)(rnd() % 3000) + 1500;
				stamps[s].centerCol64 = rowIsoCol64 - (int)(rnd() % 3000) + 1500;
				stamps[s].D64 = D64;
				stamps[s].D64sq = D64 * D64;
				stamps[s].lut = lut;
			}

			uchar ambientRow[1280];
			uchar dstOld[1280], dstNew[1280];
			for (int x = 0; x < width; x++) ambientRow[x] = (uchar)(rnd() % 256);
			memcpy(dstOld, ambientRow, width);
			memcpy(dstNew, ambientRow, width);

			for (int mode = 0; mode < 2; mode++) {
				const bool additive = mode != 0;
				memcpy(dstOld, ambientRow, width);
				memcpy(dstNew, ambientRow, width);
				OldStamps(dstOld, ambientRow, stamps, stampCount, startX, endX, rowIsoRow64, rowIsoCol64, additive);
				NewStamps(dstNew, ambientRow, stamps, stampCount, startX, endX, rowIsoRow64, rowIsoCol64, additive);
				if (memcmp(dstOld, dstNew, width) != 0) {
					failures++;
					if (failures <= 3) {
						printf("MISMATCH gt=%d iter=%d mode=%s width=%d stampCount=%d\n",
							gt, iter, additive ? "additive" : "max", width, stampCount);
						for (int x = 0; x < width; x++) {
							if (dstOld[x] != dstNew[x]) {
								printf("  first diff at x=%d: old=%d new=%d\n", x, dstOld[x], dstNew[x]);
								break;
							}
						}
					}
				}
			}
		}
	}
	if (failures == 0) {
		printf("RESULT: ALL IDENTICAL (14 combos x 20000 random rows x both modes)\n");
		return 0;
	}
	printf("RESULT: %d MISMATCHES\n", failures);
	return 1;
}
