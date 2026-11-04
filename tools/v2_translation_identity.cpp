// v2_translation_identity.cpp — standalone verification for the Rev 27 (§4.1)
// translation reuse of the V2 frame cache.
//
// The real V2 field at a pixel is a PURE function of the iso coordinates
//   isoRow64 = x + 2y - X0 - 2Y0,  isoCol64 = 2y - x - 2Y0 + X0
// (given fixed light stamps / maps / LUT / dimensions).  Any deterministic
// byte function of (isoRow64, isoCol64) therefore stands in for the real fill
// when verifying the translation algebra + the copy/sliver ranges: this tool
// builds an "old field" at projection (X0, Y0), then a reference "new field"
// at (X0+dX0, Y0+dY0) by full recompute, and compares it byte-for-byte with
// the game's translation-hit path (shifted memcpy of the overlap + sliver
// fills via the same per-pixel function at the new projection).
//
// Compile (game tools convention): vcvars32.bat && cl /EHsc /O2 /fp:fast
//   v2_translation_identity.cpp
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

// Game projection constants (identical formulas to Draw.cpp V2BuildFrame).
static int X0Of(int baseRow, int baseCol, int offsetX) { return offsetX + 32 - 32 * baseRow + 32 * baseCol; }
static int Y0Of(int baseRow, int baseCol, int offsetY) { return offsetY - 15 - 16 * baseRow - 16 * baseCol; }

// Deterministic stand-in for the V2 field (pure function of iso coordinates).
static unsigned char fieldFn(int isoR64, int isoC64)
{
	unsigned x = (unsigned)isoR64 * 0x9E3779B1u + (unsigned)isoC64 * 0x85EBCA6Bu;
	x ^= x >> 16; x *= 0x7FEB352Du; x ^= x >> 15; x ^= x >> 13;
	return (unsigned char)(x & 0xFF);
}

// Integer floor-sqrt (any exact implementation works — both tool paths share
// it; the tool validates the game's band algebra, not the game's isqrt).
static unsigned Isqrt32u(unsigned n)
{
	unsigned r = 0, bit = 1u << 30;
	while (bit > n) bit >>= 2;
	while (bit) {
		if (n >= r + bit) { n -= r + bit; r = (r >> 1) + bit; }
		else r >>= 1;
		bit >>= 2;
	}
	return r;
}

// ---- Stage 4 item 7 (Rev 2.59): changed-stamp band fill ----
// The value field = min(base isoField, per-light 255 - B), B = d*255/D64
// inside the light's disc (d = isqrt(dr^2+dc^2), dr/dc in iso space) — a pure
// function of the iso coordinates standing in for the real fill. Lights move /
// appear / vanish between frames in INDEX order (like the game's stamp array),
// and the camera moves by (dX0, dY0). Reference = full recompute at the new
// projection; band path = clean rows shifted-copied from the cache (whole row
// width), dirty rows recomputed. Dirty rows = rows whose shifted copy source
// leaves the cached range, plus every changed stamp's old+new disc row range
// (the game's exact predicate: s(y) = 4y - 4*Y0 - cr - cc, s^2 <= 2*D64^2;
// old discs use the CURRENT Y0). Mirrors Draw.cpp V2BandHit exactly.
struct BandLight { int cr, cc, D64; };

static unsigned char bandLightByte(int isoR, int isoC, const BandLight& L)
{
	const long long dr = (long long)isoR - L.cr, dc = (long long)isoC - L.cc;
	const unsigned d = Isqrt32u((unsigned)(dr * dr + dc * dc));
	if (d > (unsigned)L.D64) return 255;
	const unsigned B = (d * 255u) / (unsigned)L.D64;
	return (unsigned char)(255u - B);
}

static unsigned char bandFieldAt(int x, int y, int X0, int Y0, const std::vector<BandLight>& lights)
{
	const int isoR = x + 2 * y - X0 - 2 * Y0;
	const int isoC = 2 * y - x - 2 * Y0 + X0;
	unsigned char m = fieldFn(isoR, isoC);
	for (size_t i = 0; i < lights.size(); i++) {
		const unsigned char lb = bandLightByte(isoR, isoC, lights[i]);
		if (lb < m) m = lb;
	}
	return m;
}

static int runBandTest()
{
	// Non-zero borders + padded row stride mirror the game's startX/startY
	// and WorkingWidth (the cache stores whole working-surface rows).
	const int startX = 23, startY = 19, W = 192, H = 160, w = 144, h = 96;
	const int endX = startX + w, endY = startY + h;
	std::vector<unsigned char> oldField((size_t)H * W), cacheSurf((size_t)H * W);
	std::vector<unsigned char> newRef((size_t)H * W), newVia((size_t)H * W);
	std::vector<unsigned char> dirty((size_t)h);
	std::vector<BandLight> oldLights, newLights;

	srand(0x00007B59);
	int tests = 0, fails = 0;
	// NOTE (2026-09-07): 30000 iterations keeps the run to a few seconds on
	// modern hardware. Each iteration recomputes the full field three times,
	// so raising this to 60000+ makes the tool appear stuck (it is just slow)
	// — only raise it for a dedicated long verification run.
	for (int iter = 0; iter < 30000; iter++) {
		// Random projections; skip when no row can be reused (game rejects).
		const int bR = rand() % 200 - 100, bC = rand() % 200 - 100;
		const int oX = rand() % 4000 - 2000, oY = rand() % 4000 - 2000;
		const int X0 = X0Of(bR, bC, oX), Y0 = Y0Of(bR, bC, oY);
		int dX0 = rand() % 101 - 50;
		int dY0 = rand() % 81 - 40;
		// Axis isolation: every third iteration is a pure-horizontal or
		// pure-vertical camera move (dX0-only stresses the x-shifted overlap
		// copy + slivers on clean rows; dY0-only stresses the row range).
		if (iter % 3 == 0) dY0 = 0;
		else if (iter % 3 == 1) dX0 = 0;
		const int ay = dY0 < 0 ? -dY0 : dY0;
		if (ay >= h) continue;
		const int X0n = X0 + dX0, Y0n = Y0 + dY0;

		// Old lights, positioned by a random screen point converted to iso.
		oldLights.clear();
		const int nOld = rand() % 5;
		for (int i = 0; i < nOld; i++) {
			const int cx = startX + rand() % w, cy = startY + rand() % h;
			BandLight L;
			L.cr = cx + 2 * cy - X0 - 2 * Y0;
			L.cc = 2 * cy - cx - 2 * Y0 + X0;
			static const int D64s[4] = { 64, 256, 512, 1024 };
			L.D64 = D64s[rand() % 4];
			oldLights.push_back(L);
		}
		for (int y = startY; y < endY; y++)
			for (int x = startX; x < endX; x++)
				oldField[(size_t)y * W + x] = bandFieldAt(x, y, X0, Y0, oldLights);
		memcpy(cacheSurf.data(), oldField.data(), (size_t)H * W);

		// Mutate in index order (game: stamp array rebuilt per frame): move /
		// remove / append.
		newLights = oldLights;
		for (size_t i = 0; i < newLights.size(); ) {
			const int r = rand() % 100;
			if (r < 25) { newLights.erase(newLights.begin() + i); continue; }
			if (r < 75) {
				const int mx = rand() % 65 - 32, my = rand() % 65 - 32;
				newLights[i].cr += mx + 2 * my;
				newLights[i].cc += 2 * my - mx;
			}
			i++;
		}
		if (rand() % 100 < 25) {
			const int cx = startX + rand() % w, cy = startY + rand() % h;
			BandLight L;
			L.cr = cx + 2 * cy - X0n - 2 * Y0n;
			L.cc = 2 * cy - cx - 2 * Y0n + X0n;
			static const int D64s2[4] = { 64, 256, 512, 1024 };
			L.D64 = D64s2[rand() % 4];
			newLights.push_back(L);
		}
		// Require at least one changed stamp (zero changes = translation path).
		bool anyChanged = newLights.size() != oldLights.size();
		const size_t cmpN = oldLights.size() < newLights.size() ? oldLights.size() : newLights.size();
		for (size_t i = 0; !anyChanged && i < cmpN; i++)
			if (oldLights[i].cr != newLights[i].cr || oldLights[i].cc != newLights[i].cc
			 || oldLights[i].D64 != newLights[i].D64) anyChanged = true;
		if (!anyChanged) continue;

		// Reference: full recompute at the new projection. Both buffers get the
		// same sentinel outside the region — the full-width memcmp below must
		// never see stale bytes.
		memset(newRef.data(), 0xAA, (size_t)H * W);
		memset(newVia.data(), 0xAA, (size_t)H * W);
		for (int y = startY; y < endY; y++)
			for (int x = startX; x < endX; x++)
				newRef[(size_t)y * W + x] = bandFieldAt(x, y, X0n, Y0n, newLights);

		// Band path (mirror of Draw.cpp V2BandHit).
		memset(dirty.data(), 0, (size_t)h);
		for (int y = startY; y < endY; y++)
			if (y - dY0 < startY || y - dY0 >= endY) dirty[y - startY] = 1;
		{
			auto markDisc = [&](long long cr, long long cc, long long D64) {
				const long long maxS2 = 2LL * D64 * D64;
				const long long s0 = 4LL * Y0n + cr + cc;
				const int maxS = (int)Isqrt32u((unsigned)maxS2);
				int yLo = (int)((s0 - maxS) / 4) - 1;
				int yHi = (int)((s0 + maxS) / 4) + 1;
				if (yLo < startY) yLo = startY;
				if (yHi > endY - 1) yHi = endY - 1;
				for (int y = yLo; y <= yHi; y++) {
					const long long s = 4LL * y - s0;
					if (s * s <= maxS2) dirty[y - startY] = 1;
				}
			};
			for (size_t i = 0; i < cmpN; i++) {
				const bool same = oldLights[i].cr == newLights[i].cr
					&& oldLights[i].cc == newLights[i].cc
					&& oldLights[i].D64 == newLights[i].D64;
				if (same) continue;
				markDisc(newLights[i].cr, newLights[i].cc, newLights[i].D64);
				markDisc(oldLights[i].cr, oldLights[i].cc, oldLights[i].D64);
			}
			for (size_t i = cmpN; i < newLights.size(); i++)
				markDisc(newLights[i].cr, newLights[i].cc, newLights[i].D64);
			for (size_t i = cmpN; i < oldLights.size(); i++)
				markDisc(oldLights[i].cr, oldLights[i].cc, oldLights[i].D64);
		}
		memset(newVia.data(), 0xAA, (size_t)H * W);
		// Clean rows mirror V2ShiftCopyRow: x-shifted overlap copy + sliver
		// recompute (a clean row has no changed-stamp influence, so the
		// translation algebra holds per row).
		const int axx = dX0 < 0 ? -dX0 : dX0;
		int xCopyDst, xCopySrc, xCopyLen, xSliverStart, xSliverEnd;
		if (dX0 >= 0) {
			xCopyDst = startX + axx; xCopySrc = startX; xCopyLen = w - axx;
			xSliverStart = startX;  xSliverEnd = startX + axx;
		} else {
			xCopyDst = startX; xCopySrc = startX + axx; xCopyLen = w - axx;
			xSliverStart = endX - axx; xSliverEnd = endX;
		}
		for (int y = startY; y < endY; y++) {
			if (!dirty[y - startY]) {
				const int srcY = y - dY0; // inside [startY,endY) by construction
				memcpy(newVia.data() + (size_t)y * W + xCopyDst,
					cacheSurf.data() + (size_t)srcY * W + xCopySrc, (size_t)xCopyLen);
				for (int x = xSliverStart; x < xSliverEnd; x++)
					newVia[(size_t)y * W + x] = bandFieldAt(x, y, X0n, Y0n, newLights);
			} else {
				for (int x = startX; x < endX; x++)
					newVia[(size_t)y * W + x] = bandFieldAt(x, y, X0n, Y0n, newLights);
			}
		}

		tests++;
		if (memcmp(newVia.data(), newRef.data(), (size_t)H * W) != 0) {
			fails++;
			if (fails <= 3)
				printf("BAND MISMATCH iter=%d dX0=%d dY0=%d old=%d new=%d\n",
					iter, dX0, dY0, (int)oldLights.size(), (int)newLights.size());
		}
	}
	if (fails == 0)
		printf("ALL %d BAND TESTS IDENTICAL (changed-stamp band fill == full recompute)\n", tests);
	else
		printf("FAILED: %d/%d band mismatches\n", fails, tests);
	return fails == 0 ? 0 : 1;
}

static unsigned char isoField(int x, int y, int X0, int Y0)
{
	const int isoR64 = x + 2 * y - X0 - 2 * Y0;
	const int isoC64 = 2 * y - x - 2 * Y0 + X0;
	return fieldFn(isoR64, isoC64);
}

int main()
{
	setvbuf(stdout, nullptr, _IONBF, 0); // piped stdout is fully buffered — never lose crash output
	// Non-zero borders + padded row stride mirror the game's startX/startY
	// and WorkingWidth (the cache stores whole working-surface rows).
	const int startX = 17, startY = 13, W = 160, H = 128, w = 96, h = 64;
	const int endX = startX + w, endY = startY + h;
	std::vector<unsigned char> oldField((size_t)H * W, 0xEE);
	std::vector<unsigned char> cacheSurf((size_t)H * W, 0xEE);
	std::vector<unsigned char> newRef((size_t)H * W, 0xDD);
	std::vector<unsigned char> newVia((size_t)H * W, 0xDD);

	srand(0x415F154B);
	int tests = 0, fails = 0;
	for (int iter = 0; iter < 20000; iter++) {
		const int bR = rand() % 200 - 100, bC = rand() % 200 - 100;
		const int oX = rand() % 4000 - 2000, oY = rand() % 4000 - 2000;
		const int X0 = X0Of(bR, bC, oX), Y0 = Y0Of(bR, bC, oY);

		// Old field + cache snapshot (game: V2CacheStore after the fill).
		for (int y = startY; y < endY; y++)
			for (int x = startX; x < endX; x++)
				oldField[(size_t)y * W + x] = isoField(x, y, X0, Y0);
		memcpy(cacheSurf.data(), oldField.data(), (size_t)H * W);

		// Random integer projection delta, occasionally zero or beyond overlap.
		const int dX0 = (iter % 7 == 0) ? 0 : (rand() % 161) - 80;
		const int dY0 = (iter % 11 == 0) ? 0 : (rand() % 121) - 60;
		const int X0n = X0 + dX0, Y0n = Y0 + dY0;
		const int ax = dX0 < 0 ? -dX0 : dX0, ay = dY0 < 0 ? -dY0 : dY0;
		if (ax >= w || ay >= h) continue; // game returns false -> full fill, nothing to verify here

		// Reference: full recompute at the new projection.
		for (int y = startY; y < endY; y++)
			for (int x = startX; x < endX; x++)
				newRef[(size_t)y * W + x] = isoField(x, y, X0n, Y0n);

		// Game's V2TranslationHit: shifted copy + sliver fills.
		memset(newVia.data(), 0xDD, (size_t)H * W);
		int xCopyDst, xCopySrc, xCopyLen, xSliverStart, xSliverEnd;
		if (dX0 >= 0) {
			xCopyDst = startX + ax; xCopySrc = startX; xCopyLen = w - ax;
			xSliverStart = startX;  xSliverEnd = startX + ax;
		} else {
			xCopyDst = startX; xCopySrc = startX + ax; xCopyLen = w - ax;
			xSliverStart = endX - ax; xSliverEnd = endX;
		}
		for (int y = startY; y < endY; y++) {
			const int srcY = y - dY0;
			if (srcY >= startY && srcY < endY) {
				memcpy(newVia.data() + (size_t)y * W + xCopyDst,
					cacheSurf.data() + (size_t)srcY * W + xCopySrc, (size_t)xCopyLen);
				for (int x = xSliverStart; x < xSliverEnd; x++)
					newVia[(size_t)y * W + x] = isoField(x, y, X0n, Y0n);
			} else {
				for (int x = startX; x < endX; x++)
					newVia[(size_t)y * W + x] = isoField(x, y, X0n, Y0n);
			}
		}

		tests++;
		if (memcmp(newVia.data(), newRef.data(), (size_t)H * W) != 0) {
			fails++;
			if (fails <= 3)
				printf("MISMATCH iter=%d dX0=%d dY0=%d\n", iter, dX0, dY0);
		}
	}
	if (fails == 0)
		printf("ALL %d TESTS IDENTICAL (translation reuse == full recompute)\n", tests);
	else
		printf("FAILED: %d/%d mismatches\n", fails, tests);
	const int bandFails = runBandTest();
	return (fails == 0 && bandFails == 0) ? 0 : 1;
}
