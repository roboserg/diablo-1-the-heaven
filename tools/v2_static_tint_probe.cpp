// v2_static_tint_probe.cpp — 2D reproduction of the committed Option-C
// pipeline (static bake -> V2 static tint pre-fill -> ambient fill ->
// envelope stamps -> mass tint blend) + BMP dumps of the resulting tint
// field, to hunt the "hard edge / disc / staircase" artifacts reported in
// torch-heavy dungeons.
//
// Build (from c:\th4\tools):
//   cmd /c 'call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x86 >nul 2>&1 & cl /nologo /EHsc /O2 /fp:fast v2_static_tint_probe.cpp & v2_static_tint_probe.exe'
//
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

static const int FineMap_112 = 112;
static const uint TINT_WHITE = 0x00FFFFFFu;
static const uint A8L8_SCALE = 17;
static const uint lmax_A8L8 = 255;

static inline unsigned Isqrt32(unsigned n)
{
	unsigned res = 0;
	unsigned bit = 1u << 30;
	while (bit > n) bit >>= 2;
	while (bit) {
		if (n >= res + bit) { n -= res + bit; res = (res >> 1) + bit; }
		else res >>= 1;
		bit >>= 2;
	}
	return res;
}

static uchar SMap[FineMap_112][FineMap_112];
static uint  TMap[FineMap_112][FineMap_112];
static uchar LightLevels[16][128];
static uchar BrightLut[16][1025];

static void BuildLightLevels(uchar lv[16][128], bool original, double gammaBlend = 1.0)
{
	if (original) {
		for (int rad = 0; rad < 16; ++rad) {
			for (int d = 0; d < 128; ++d) {
				if (d <= 8 * (rad + 1)) {
					const double factor = (double)d / ((double)(rad + 1) * 8.0);
					const double inner = (gammaBlend == 1.0) ? (2.0 - factor) : (1.0 + gammaBlend * (1.0 - factor));
					double v = factor * inner * 15.0 + 0.5;
					if (v > 15.0) v = 15.0;
					lv[rad][d] = (uchar)v;
				} else {
					lv[rad][d] = 15;
				}
			}
		}
	} else {
		for (int radius = 0; radius < 16; radius++) {
			const unsigned maxDistance = (radius + 1) * 8;
			for (int distance = 0; distance < 128; distance++) {
				if (distance > maxDistance) {
					lv[radius][distance] = 15;
				} else {
					const float factor = (float)distance / (float)maxDistance;
					const float inner = (gammaBlend == 1.0f) ? (2.0f - factor) : (1.0f + (float)gammaBlend * (1.0f - factor));
					float v = factor * inner * 15.0f + 0.5f;
					if (v > 15.0f) v = 15.0f;
					lv[radius][distance] = (uchar)v;
				}
			}
		}
	}
}

static void BuildBrightLut(uchar lut[16][1025], bool original, double gammaBlend = 1.0)
{
	for (int rad = 0; rad < 16; rad++) {
		uchar* dst = lut[rad];
		const int dMax = 64 * (rad + 1);
		for (int i = 0; i < 1025; i++) {
			if (i > dMax) { dst[i] = 0; continue; }
			const double f = (double)i / (double)dMax;
			dst[i] = (uchar)(255.0 * (1.0 - f) * (1.0 - gammaBlend * f) + 0.5);
		}
	}
}

static bool To112(int r, int c) { return (uint)r < FineMap_112 && (uint)c < FineMap_112; }
static int LightDist00(int x, int y) { return (int)(sqrt((double)(x * 8) * (x * 8) + (double)(y * 8) * (y * 8)) + 0.5); }

static void BakeLight(int row, int col, int radius)
{
	if (!To112(row, col)) return;
	if ((uint)radius > 15) radius = 15;
	SMap[row][col] = 0;
	int maxleft = row - 15 < 0 ? 1 + row : 15;
	int maxright = row + 15 > FineMap_112 ? FineMap_112 - row : 15;
	int maxup = col - 15 < 0 ? 1 + col : 15;
	int maxdown = col + 15 > FineMap_112 ? FineMap_112 - col : 15;
	int y, x;
	auto Quad = [&](int max1, int max2, auto d1, auto d2) {
		for (y = 0; y < max1; y++) {
			for (x = 1; x < max2; x++) {
				int dist = LightDist00(x, y);
				if (dist < 128) {
					int r = row + d1(); if ((uint)r >= FineMap_112) continue;
					int c = col + d2(); if ((uint)c >= FineMap_112) continue;
					int lightVal = LightLevels[radius][dist];
					if (lightVal < SMap[r][c]) SMap[r][c] = (uchar)lightVal;
				}
			}
		}
	};
#define E(expr) [&]{return expr;}
	Quad(maxup, maxright, E(x), E(y));
	Quad(maxdown, maxright, E(y), E(-x));
	Quad(maxdown, maxleft, E(-x), E(-y));
	Quad(maxup, maxleft, E(-y), E(x));
#undef E
}

static void BakeTint(int row, int col, int radius, uchar tr, uchar tg, uchar tb)
{
	if (!To112(row, col)) return;
	if ((uint)radius > 15) radius = 15;
	const uint colorWord = (uint)tb | ((uint)tg << 8) | ((uint)tr << 16);
	if (LightLevels[radius][0] <= SMap[row][col] && TMap[row][col] == TINT_WHITE) {
		TMap[row][col] = colorWord;
	}
	int maxleft = row - 15 < 0 ? 1 + row : 15;
	int maxright = row + 15 > FineMap_112 ? FineMap_112 - row : 15;
	int maxup = col - 15 < 0 ? 1 + col : 15;
	int maxdown = col + 15 > FineMap_112 ? FineMap_112 - col : 15;
	int y, x;
	auto Quad = [&](int max1, int max2, auto d1, auto d2) {
		for (y = 0; y < max1; y++) {
			for (x = 1; x < max2; x++) {
				int dist = LightDist00(x, y);
				if (dist < 128) {
					int r = row + d1(); if ((uint)r >= FineMap_112) continue;
					int c = col + d2(); if ((uint)c >= FineMap_112) continue;
					int lightVal = LightLevels[radius][dist];
					if (lightVal < SMap[r][c]) TMap[r][c] = colorWord;
				}
			}
		}
	};
#define E(expr) [&]{return expr;}
	Quad(maxup, maxright, E(x), E(y));
	Quad(maxdown, maxright, E(y), E(-x));
	Quad(maxdown, maxleft, E(-x), E(-y));
	Quad(maxup, maxleft, E(-y), E(x));
#undef E
}

static uchar g_TintWeightLut[256][256];
static void InitTintWeightLut()
{
	for (int m = 0; m <= 255; m++) {
		for (int B = 0; B <= 255; B++) {
			uint w = 0;
			if (m > 0 && B > 0) {
				const uint sum = (uint)m + (uint)B;
				w = ((uint)B * 255 + (sum >> 1)) / sum;
			}
			g_TintWeightLut[m][B] = (uchar)w;
		}
	}
}

static inline void writeTintPixel(uint* tintRow, ushort* massRow, int px, uint word, uint B)
{
	if (B == 0) return;
	const uint old = tintRow[px];
	const uint oldA = old >> 24;
	if (oldA == 0) {
		tintRow[px] = word | (B << 24);
		massRow[px] = (ushort)B;
	} else {
		const uint m = massRow[px];
		const uint sum = m + B;
		const uint w = (m <= 255) ? g_TintWeightLut[m][B] : ((B * 255 + (sum >> 1)) / sum);
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

static int ceilDiv2(int x) { return x >= 0 ? (x + 1) / 2 : x / 2; }
static int floorDiv2(int x) { return x >= 0 ? x / 2 : (x - 1) / 2; }
static int floorDivI(int n, int d) { const int q = n / d, r = n % d; return (r != 0 && ((r < 0) != (d < 0))) ? q - 1 : q; }
static int ceilDivI(int n, int d) { const int q = n / d, r = n % d; return (r != 0 && ((r < 0) == (d < 0))) ? q + 1 : q; }

struct FQ { int q, fr; };
static FQ floor64F(int val64)
{
	FQ r;
	if (val64 >= 0) { r.fr = val64 & 63; r.q = val64 >> 6; }
	else { int q = (val64 - 63) / 64; r.fr = val64 - q * 64; r.q = q; }
	return r;
}

struct StampLight {
	int drStart, dcStart;
	int D64, D64sq;
	int rad;
	const uchar* lut;
	uchar tintR, tintG, tintB;
};

struct RowLight {
	int drStart, dcStart;
	int tMin, tMax;
	int D64;
	const uchar* lut;
	uchar tintR, tintG, tintB;
};

static int staticTintActiveG = 1;

static int Lval(int r, int c)
{
	if (r < 0) r = 0; else if (r >= FineMap_112) r = FineMap_112 - 1;
	if (c < 0) c = 0; else if (c >= FineMap_112) c = FineMap_112 - 1;
	return SMap[r][c];
}
static uint TLval(int r, int c)
{
	if (r < 0) r = 0; else if (r >= FineMap_112) r = FineMap_112 - 1;
	if (c < 0) c = 0; else if (c >= FineMap_112) c = FineMap_112 - 1;
	return TMap[r][c];
}

static void FillRow(int W, int rowIsoRow64, int rowIsoCol64,
	uchar* dstRow, uint* tintRow, ushort* massRow,
	const StampLight* stamps, int stampCount)
{
	const int startX = 0, endX = W;

	if (tintRow) {
		for (int x = startX; x < endX; x++) tintRow[x] = TINT_WHITE;
	}
	// (th4) P3 parity: static-bake ambient contributor through the same
	// writeTintPixel the stamps use (mirrors live TintAccumulate path).
	if (tintRow && staticTintActiveG) {
		int isoRow_64 = rowIsoRow64;
			int isoCol_64 = rowIsoCol64;
			int wsX = startX;
			while (wsX < endX) {
				FQ qr = floor64F(isoRow_64);
				FQ qc = floor64F(isoCol_64);
				int iRow = qr.q, fr = qr.fr;
				int iCol = qc.q, fc = qc.fr;
				int v00 = Lval(iRow, iCol);
				int v10 = Lval(iRow + 1, iCol);
				int v01 = Lval(iRow, iCol + 1);
				int v11 = Lval(iRow + 1, iCol + 1);
				uint c00 = TLval(iRow, iCol);
				uint c10 = TLval(iRow + 1, iCol);
				uint c01 = TLval(iRow, iCol + 1);
				uint c11 = TLval(iRow + 1, iCol + 1);
				int toRow = 64 - fr;
				int toCol = fc + 1;
				int segLen = toRow < toCol ? toRow : toCol;
				int remaining = endX - wsX;
				if (segLen > remaining) segLen = remaining;

				if (v00 == v10 && v00 == v01 && v00 == v11
				 && c00 == c10 && c00 == c01 && c00 == c11) {
					int val = v00 * A8L8_SCALE;
					if ((unsigned)val > lmax_A8L8) val = lmax_A8L8;
					const uint cw = c00 & 0xFFFFFFu; // ignore the map word's A byte
					if (cw != TINT_WHITE) {
						const uint B = (uint)(lmax_A8L8 - val);
						if (B > 0) {
							for (int i = 0; i < segLen; i++)
								writeTintPixel(tintRow, massRow, wsX + i, cw, B);
						} else {
							for (int i = 0; i < segLen; i++) tintRow[wsX + i] = cw;
						}
					}
					wsX += segLen;
				} else {
					int val4096 = v00 * (64 - fr) * (64 - fc) + v10 * fr * (64 - fc)
						+ v01 * (64 - fr) * fc + v11 * fr * fc;
					const int A = -v00 + v10 + v01 - v11;
					const int delta2 = 2 * A;
					int delta = (v10 - v00) * (64 - fc) + (v11 - v01) * fc
						- (v01 - v00) * (64 - fr) - (v11 - v10) * fr + A;
					struct Chan { int val; int delta; int delta2; };
					auto MakeChan = [&](int ch00, int ch10, int ch01, int ch11) -> Chan {
						Chan ch;
						ch.val = ch00 * (64 - fr) * (64 - fc) + ch10 * fr * (64 - fc)
							+ ch01 * (64 - fr) * fc + ch11 * fr * fc;
						int a = -ch00 + ch10 + ch01 - ch11;
						ch.delta2 = 2 * a;
						ch.delta = (ch10 - ch00) * (64 - fc) + (ch11 - ch01) * fc
							- (ch01 - ch00) * (64 - fr) - (ch11 - ch10) * fr + a;
						return ch;
					};
					Chan cb = MakeChan((int)(c00 >> 0) & 0xFF, (int)(c10 >> 0) & 0xFF, (int)(c01 >> 0) & 0xFF, (int)(c11 >> 0) & 0xFF);
					Chan cg = MakeChan((int)(c00 >> 8) & 0xFF, (int)(c10 >> 8) & 0xFF, (int)(c01 >> 8) & 0xFF, (int)(c11 >> 8) & 0xFF);
					Chan cr = MakeChan((int)(c00 >> 16) & 0xFF, (int)(c10 >> 16) & 0xFF, (int)(c01 >> 16) & 0xFF, (int)(c11 >> 16) & 0xFF);
					for (int i = 0; i < segLen; i++, wsX++, val4096 += delta, delta += delta2,
						cb.val += cb.delta, cb.delta += cb.delta2,
						cg.val += cg.delta, cg.delta += cg.delta2,
						cr.val += cr.delta, cr.delta += cr.delta2) {
						int val = (val4096 * A8L8_SCALE + 2048) >> 12;
						if ((unsigned)val > lmax_A8L8) val = val < 0 ? 0 : lmax_A8L8;
						uint b8 = (uint)((cb.val + 2048) >> 12);
						uint g8 = (uint)((cg.val + 2048) >> 12);
						uint r8 = (uint)((cr.val + 2048) >> 12);
						uint w = b8 | (g8 << 8) | (r8 << 16);
						if (w != TINT_WHITE) {
							const uint B = (uint)(lmax_A8L8 - val);
							if (B > 0) writeTintPixel(tintRow, massRow, wsX, w, B);
							else tintRow[wsX] = w; // byte parity with the live P3 path
						}
					}
				}
				isoRow_64 += segLen;
				isoCol_64 -= segLen;
			}
	}

	{
		int isoRow_64 = rowIsoRow64;
		int isoCol_64 = rowIsoCol64;
		int wsX = startX;
		while (wsX < endX) {
			FQ qr = floor64F(isoRow_64);
			FQ qc = floor64F(isoCol_64);
			int iRow = qr.q, fr = qr.fr;
			int iCol = qc.q, fc = qc.fr;
			int v00 = Lval(iRow, iCol);
			int v10 = Lval(iRow + 1, iCol);
			int v01 = Lval(iRow, iCol + 1);
			int v11 = Lval(iRow + 1, iCol + 1);
			int toRow = 64 - fr;
			int toCol = fc + 1;
			int segLen = toRow < toCol ? toRow : toCol;
			int remaining = endX - wsX;
			if (segLen > remaining) segLen = remaining;
			if (v00 == v10 && v00 == v01 && v00 == v11) {
				int val = v00 * A8L8_SCALE;
				if ((unsigned)val > lmax_A8L8) val = lmax_A8L8;
				memset(&dstRow[wsX], (uchar)val, segLen);
				wsX += segLen;
			} else {
				int val4096 = v00 * (64 - fr) * (64 - fc) + v10 * fr * (64 - fc)
					+ v01 * (64 - fr) * fc + v11 * fr * fc;
				const int A = -v00 + v10 + v01 - v11;
				const int delta2 = 2 * A;
				int delta = (v10 - v00) * (64 - fc) + (v11 - v01) * fc
					- (v01 - v00) * (64 - fr) - (v11 - v10) * fr + A;
				for (int i = 0; i < segLen; i++, wsX++, val4096 += delta, delta += delta2) {
					int val = (val4096 * A8L8_SCALE + 2048) >> 12;
					if ((unsigned)val > lmax_A8L8) val = val < 0 ? 0 : lmax_A8L8;
					dstRow[wsX] = (uchar)val;
				}
			}
			isoRow_64 += segLen;
			isoCol_64 -= segLen;
		}
	}

	RowLight rl[128];
	int rlCount = 0;
	for (int s = 0; s < stampCount; s++) {
		const StampLight& st = stamps[s];
		const int drStart = rowIsoRow64 - st.drStart;
		const int dcStart = rowIsoCol64 - st.dcStart;
		const long long ssum = (long long)(drStart + dcStart) * (drStart + dcStart);
		if (ssum > 2LL * st.D64sq) continue;
		const int b = dcStart - drStart;
		const int halfSpan = (int)Isqrt32((unsigned)(2 * st.D64sq - (int)ssum));
		int tMin = ceilDiv2(b - halfSpan);
		int tMax = floorDiv2(b + halfSpan);
		if (tMin < 0) tMin = 0;
		if (tMax > W - 1) tMax = W - 1;
		if (tMin > tMax) continue;
		RowLight& r = rl[rlCount++];
		r.drStart = drStart; r.dcStart = dcStart;
		r.tMin = tMin; r.tMax = tMax;
		r.D64 = st.D64; r.lut = st.lut;
		r.tintR = st.tintR; r.tintG = st.tintG; r.tintB = st.tintB;
	}

	int rmP[128], rmQ[128];
	for (int i = 0; i < rlCount; i++) {
		const RowLight& ri = rl[i];
		const int ai = 2 * (ri.drStart - ri.dcStart);
		const int bi = ri.drStart * ri.drStart + ri.dcStart * ri.dcStart;
		int rmCount = 0;
		bool beatenEverywhere = false;
		for (int j = 0; j < rlCount; j++) {
			if (j == i || rl[j].lut != ri.lut) continue;
			if (rl[j].tintR != ri.tintR || rl[j].tintG != ri.tintG || rl[j].tintB != ri.tintB) continue;
			const RowLight& rj = rl[j];
			if (rj.tMax < ri.tMin || rj.tMin > ri.tMax) continue;
			const int da = 2 * (rj.drStart - rj.dcStart) - ai;
			const int db = bi - (rj.drStart * rj.drStart + rj.dcStart * rj.dcStart);
			int p = 0, q = 0;
			const bool tieToJ = j < i;
			if (da == 0) {
				if (db > 0 || (db == 0 && tieToJ)) { beatenEverywhere = true; break; }
				continue;
			} else if (da > 0) {
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
			rmP[b + 1] = pp; rmQ[b + 1] = qq;
		}
		auto stampPiece = [&](int tS, int tE) {
			const int t0 = tS;
			int dr = ri.drStart + t0;
			int dc = ri.dcStart - t0;
			int d2 = dr * dr + dc * dc;
			int delta = 2 * (dr - dc) + 2;
			const uchar* lut = ri.lut;
			const int D64 = ri.D64;
			const uint tintWord = (uint)ri.tintB | ((uint)ri.tintG << 8) | ((uint)ri.tintR << 16);
			int x = t0;
			const int xEnd = tE;
			int d = (int)Isqrt32((unsigned)d2);
			int dsq = d * d;
			for (; x <= xEnd; x++) {
				if ((uint)d <= (uint)D64) {
					const int B = (int)lut[d];
					const int lv = 255 - B;
					if (tintRow) writeTintPixel(tintRow, massRow, x, tintWord, (uint)B);
					if (lv < dstRow[x]) dstRow[x] = (uchar)lv;
				}
				d2 += delta; delta += 4;
				while (d2 >= dsq + 2 * d + 1) { dsq += 2 * d + 1; d++; }
				while (d > 0 && d2 < dsq) { dsq -= 2 * d - 1; d--; }
			}
		};
		int cur = ri.tMin;
		for (int a = 0; a < rmCount && cur <= ri.tMax; a++) {
			if (rmP[a] > ri.tMax) break;
			if (rmP[a] > cur) stampPiece(cur, rmP[a] - 1);
			if (rmQ[a] >= cur) cur = rmQ[a] + 1;
		}
		if (cur <= ri.tMax) stampPiece(cur, ri.tMax);
	}
}

static void IdealRow(int W, int rowIsoRow64, int rowIsoCol64,
	uint* tintOut, const StampLight* stamps, int stampCount)
{
	for (int x = 0; x < W; x++) {
		FQ qr = floor64F(rowIsoRow64 + x);
		FQ qc = floor64F(rowIsoCol64 - x);
		int iRow = qr.q, fr = qr.fr;
		int iCol = qc.q, fc = qc.fr;
		int v00 = Lval(iRow, iCol), v10 = Lval(iRow + 1, iCol), v01 = Lval(iRow, iCol + 1), v11 = Lval(iRow + 1, iCol + 1);
		uint c00 = TLval(iRow, iCol), c10 = TLval(iRow + 1, iCol), c01 = TLval(iRow, iCol + 1), c11 = TLval(iRow + 1, iCol + 1);
		int val4096 = v00 * (64 - fr) * (64 - fc) + v10 * fr * (64 - fc) + v01 * (64 - fr) * fc + v11 * fr * fc;
		int val = (val4096 * A8L8_SCALE + 2048) >> 12;
		if ((unsigned)val > lmax_A8L8) val = val < 0 ? 0 : lmax_A8L8;
		auto bil = [&](int ch00, int ch10, int ch01, int ch11) -> int {
			int v = ch00 * (64 - fr) * (64 - fc) + ch10 * fr * (64 - fc) + ch01 * (64 - fr) * fc + ch11 * fr * fc;
			int r = (v + 2048) >> 12;
			return r < 0 ? 0 : (r > 255 ? 255 : r);
		};
		int b8 = bil((c00 >> 0) & 0xFF, (c10 >> 0) & 0xFF, (c01 >> 0) & 0xFF, (c11 >> 0) & 0xFF);
		int g8 = bil((c00 >> 8) & 0xFF, (c10 >> 8) & 0xFF, (c01 >> 8) & 0xFF, (c11 >> 8) & 0xFF);
		int r8 = bil((c00 >> 16) & 0xFF, (c10 >> 16) & 0xFF, (c01 >> 16) & 0xFF, (c11 >> 16) & 0xFF);
		uint word = b8 | (g8 << 8) | (r8 << 16);
		uint mass = 0;
		if (word != TINT_WHITE) {
			mass = (uint)(lmax_A8L8 - val);
			word |= mass << 24;
		}
		for (int s = 0; s < stampCount; s++) {
			const StampLight& st = stamps[s];
			const int dr = st.drStart - (rowIsoRow64 + x);
			const int dc = st.dcStart - (rowIsoCol64 - x);
			const long long d2 = (long long)dr * dr + (long long)dc * dc;
			if (d2 > (long long)st.D64sq) continue;
			bool nearest = true;
			for (int t = 0; t < stampCount && nearest; t++) {
				if (t == s) continue;
				const StampLight& ot = stamps[t];
				if (ot.lut != st.lut) continue;
				if (ot.tintR != st.tintR || ot.tintG != st.tintG || ot.tintB != st.tintB) continue;
				const int odr = ot.drStart - (rowIsoRow64 + x);
				const int odc = ot.dcStart - (rowIsoCol64 - x);
				const long long od2 = (long long)odr * odr + (long long)odc * odc;
				if (od2 < d2) { nearest = false; break; }
			}
			if (!nearest) continue;
			const int d = (int)Isqrt32((unsigned)d2);
			const int B = (int)st.lut[d];
			if (B == 0) continue;
			const uint wrd = (uint)st.tintB | ((uint)st.tintG << 8) | ((uint)st.tintR << 16);
			if (mass == 0) {
				word = wrd | ((uint)B << 24);
				mass = (uint)B;
			} else {
				const uint sum = mass + (uint)B;
				const uint w = (mass <= 255) ? g_TintWeightLut[mass][B] : ((uint)B * 255 + (sum >> 1)) / sum;
				const uint wr = (wrd >> 16) & 0xFF, wg = (wrd >> 8) & 0xFF, wb = wrd & 0xFF;
				const uint orr = (word >> 16) & 0xFF, og = (word >> 8) & 0xFF, ob = word & 0xFF;
				const uint nr = (wr * w + orr * (255 - w) + 127) >> 8;
				const uint ng = (wg * w + og * (255 - w) + 127) >> 8;
				const uint nb = (wb * w + ob * (255 - w) + 127) >> 8;
				mass = sum;
				const uint na = sum > 255 ? 255 : sum;
				word = nb | (ng << 8) | (nr << 16) | (na << 24);
			}
		}
		tintOut[x] = word;
	}
}

static uint g_rng = 0x9E3779B9u;
static uint rnd() { g_rng = g_rng * 1664525u + 1013904223u; return g_rng; }

struct RtLight { int r, c, rad; uchar tr, tg, tb; };

static int makeStamps(const RtLight* rt, int nrt, StampLight* out)
{
	int k = 0;
	for (int i = 0; i < nrt; i++) {
		StampLight& st = out[k++];
		st.drStart = rt[i].r; st.dcStart = rt[i].c; st.rad = rt[i].rad;
		st.lut = BrightLut[rt[i].rad];
		st.tintR = rt[i].tr; st.tintG = rt[i].tg; st.tintB = rt[i].tb;
		int lo = 0, hi = 1025;
		while (lo + 1 < hi) { const int mid = (lo + hi) >> 1; if (st.lut[mid] > 0) lo = mid; else hi = mid; }
		st.D64 = lo;
		st.D64sq = lo * lo;
	}
	StampLight sorted[128];
	int n = 0;
	for (int pass = 0; pass < 2; pass++) {
		for (int i = 0; i < k; i++) {
			const bool isWhite = out[i].tintR == 255 && out[i].tintG == 255 && out[i].tintB == 255;
			if ((pass == 0) == isWhite) sorted[n++] = out[i];
		}
	}
	for (int i = 0; i < k; i++) out[i] = sorted[i];
	return k;
}

static void resetMaps(uchar darkLevel)
{
	memset(SMap, darkLevel, sizeof(SMap));
	// (th4) Step-1 P1 parity: the CPU white sentinel is TINT_WHITE
	// (0x00FFFFFF, A byte 0), same fill as live Main.cpp / Object.cpp.
	// A 0xFF memset (0xFFFFFFFF) is what shipped and caused the phantom-
	// mass artifacts — keep this loop in parity with the Step-1 fix.
	for (int r = 0; r < FineMap_112; r++)
		for (int c = 0; c < FineMap_112; c++) TMap[r][c] = TINT_WHITE;
}

static void bakeTorches(int K, int r0, int r1, int c0, int c1)
{
	for (int i = 0; i < K; i++) {
		int r = r0 + (int)(rnd() % (r1 - r0));
		int c = c0 + (int)(rnd() % (c1 - c0));
		BakeTint(r, c, 5, 255, 190, 100);
		BakeLight(r, c, 5);
	}
}

static void WriteBMP(const char* path, const uchar* rgb, int W, int H)
{
	FILE* f = fopen(path, "wb");
	if (!f) { printf("cannot write %s\n", path); return; }
	int rowSize = (W * 3 + 3) & ~3;
	int dataSize = rowSize * H;
	uchar hdr[54];
	memset(hdr, 0, 54);
	hdr[0] = 'B'; hdr[1] = 'M';
	*(int*)(hdr + 2) = 54 + dataSize;
	*(int*)(hdr + 10) = 54;
	*(int*)(hdr + 14) = 40;
	*(int*)(hdr + 18) = W;
	*(int*)(hdr + 22) = H;
	*(short*)(hdr + 26) = 1;
	*(short*)(hdr + 28) = 24;
	*(int*)(hdr + 34) = dataSize;
	fwrite(hdr, 1, 54, f);
	uchar* row = new uchar[rowSize];
	memset(row, 0, rowSize);
	for (int y = H - 1; y >= 0; y--) {
		const uchar* src = rgb + (size_t)y * W * 3;
		memcpy(row, src, W * 3);
		fwrite(row, 1, rowSize, f);
	}
	delete[] row;
	fclose(f);
	printf("wrote %s (%dx%d)\n", path, W, H);
}

int main()
{
	InitTintWeightLut();
	BuildLightLevels(LightLevels, false, 1.0);
	BuildBrightLut(BrightLut, false, 1.0);

	const int SW = 640, SH = 480;
	const int X0 = 320 - (62 - 32) * 32;
	const int Y0 = 240 - (62 + 32) * 16;
	const int startX = 0;

	RtLight rt[16];
	int nrt = 0;
	rt[nrt++] = { 64 * 66, 64 * 30, 10, 255, 255, 255 }; // player white
	rt[nrt++] = { 64 * 56, 64 * 40, 15, 0, 0, 255 };     // portal blue
	for (int i = 0; i < 6; i++) {
		int r = 55 + (int)(rnd() % 14);
		int c = 24 + (int)(rnd() % 14);
		rt[nrt++] = { 64 * r, 64 * c, 8, 255, 120, 20 }; // fireball orange
	}
	StampLight stamps[128];
	int stampCount = makeStamps(rt, nrt, stamps);

	static uchar dstRow[640];
	static uint tintRow[640];
	static ushort massRow[640];
	static uint idealRow[640];
	static uchar rgbOut[640 * 480 * 3];
	static uchar rgbIdeal[640 * 480 * 3];
	static uchar rgbDiff[640 * 480 * 3];
	static uchar rgbLight[640 * 480 * 3];
	static uchar rgbComposed[640 * 480 * 3];

	int maxDiff, maxJumpRGB, maxJumpA, cJumpRgb, cJumpA;

	for (int scenario = 0; scenario < 2; scenario++) {
		if (scenario == 0) {
			resetMaps(15);
			staticTintActiveG = 0;
		} else {
			resetMaps(15);
			bakeTorches(80, 40, 85, 5, 55);
			staticTintActiveG = 1;
		}
		maxDiff = 0; maxJumpRGB = 0; maxJumpA = 0; cJumpRgb = 0; cJumpA = 0;

		for (int wsY = 0; wsY < SH; wsY++) {
			const int rowIsoRow64 = startX + 2 * wsY - X0 - 2 * Y0;
			const int rowIsoCol64 = 2 * wsY - startX - 2 * Y0 + X0;
			memset(dstRow, 0xFF, sizeof(dstRow));
			memset(tintRow, 0, sizeof(tintRow));
			memset(massRow, 0, sizeof(massRow));
			FillRow(SW, rowIsoRow64, rowIsoCol64, dstRow, tintRow, massRow, stamps, stampCount);
			IdealRow(SW, rowIsoRow64, rowIsoCol64, idealRow, stamps, stampCount);
			for (int x = 0; x < SW; x++) {
				const uint w = tintRow[x];
				const uint iw = idealRow[x];
				uchar* p = rgbOut + ((size_t)wsY * SW + x) * 3;
				p[0] = (uchar)(w >> 0); p[1] = (uchar)(w >> 8); p[2] = (uchar)(w >> 16);
				uchar* pi = rgbIdeal + ((size_t)wsY * SW + x) * 3;
				pi[0] = (uchar)(iw >> 0); pi[1] = (uchar)(iw >> 8); pi[2] = (uchar)(iw >> 16);
				uchar* pd = rgbDiff + ((size_t)wsY * SW + x) * 3;
				uint dx = w ^ iw;
				int mx = 0;
				for (int ch = 0; ch < 32; ch += 8) { uint b = (dx >> ch) & 0xFF; if ((int)b > mx) mx = (int)b; }
				if (mx > maxDiff) maxDiff = mx;
				pd[0] = pd[1] = pd[2] = (uchar)(mx * 8 > 255 ? 255 : mx * 8);
				uchar* pl = rgbLight + ((size_t)wsY * SW + x) * 3;
				pl[0] = pl[1] = pl[2] = dstRow[x];
				// shader-composed view: lit brightness x lerp(white, tint.rgb, A/255)
				uchar* pc = rgbComposed + ((size_t)wsY * SW + x) * 3;
				for (int ch = 0; ch < 3; ch++) {
					int tch = (int)((w >> (8 * ch)) & 0xFF);
					int a = (int)(w >> 24);
					int b = 255 - dstRow[x];           // lit brightness 0..255
					int c = (255 * (255 - a) + tch * a + 127) >> 8;
					int v = (b * c + 127) >> 8;
					pc[ch] = (uchar)v;
				}
			}
			for (int x = 0; x + 1 < SW; x++) {
				int dRgb = 0;
				for (int ch = 0; ch < 24; ch += 8) {
					int d = (int)((tintRow[x] >> ch) & 0xFF) - (int)((tintRow[x + 1] >> ch) & 0xFF);
					if (d < 0) d = -d;
					if (d > dRgb) dRgb = d;
				}
				int dA = (int)(tintRow[x] >> 24) - (int)(tintRow[x + 1] >> 24);
				if (dA < 0) dA = -dA;
				if (dRgb > maxJumpRGB) maxJumpRGB = dRgb;
				if (dA > maxJumpA) maxJumpA = dA;
				if (dRgb > 60) cJumpRgb++;
				if (dA > 60) cJumpA++;
			}
		}
		printf("scenario %d (%s): maxDiff=%d maxJumpRGB=%d maxJumpA=%d countRGB(>60)=%d countA(>60)=%d\n",
			scenario, scenario ? "80 torches + lights" : "no static", maxDiff, maxJumpRGB, maxJumpA, cJumpRgb, cJumpA);
		if (scenario == 0) {
			WriteBMP("probe_s0_tint.bmp", rgbOut, SW, SH);
			WriteBMP("probe_s0_light.bmp", rgbLight, SW, SH);
			WriteBMP("probe_s0_composed.bmp", rgbComposed, SW, SH);
		} else {
			WriteBMP("probe_s1_tint.bmp", rgbOut, SW, SH);
			WriteBMP("probe_s1_ideal.bmp", rgbIdeal, SW, SH);
			WriteBMP("probe_s1_diff.bmp", rgbDiff, SW, SH);
			WriteBMP("probe_s1_light.bmp", rgbLight, SW, SH);
			WriteBMP("probe_s1_composed.bmp", rgbComposed, SW, SH);
		}
	}
	return 0;
}
