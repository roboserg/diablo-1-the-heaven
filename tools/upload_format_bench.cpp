// Upload micro-benchmark for the EnrichedLightUltra format decision (TH4).
// Measures best-of-N per-frame CPU cost of the candidate upload passes at 1920x1080:
//   1. today's A8L8 two-stream interleave (SSE2, 16 px/iter)  -> 16 bpp, 4.15 MB
//   2. new L8 light texture upload = plain memcpy             ->  8 bpp, 2.07 MB
//   3. 48-bpp draft XRGB pass (LUT gather)                    -> 32 bpp, 8.29 MB
//   4. 48-bpp draft AL pass with constant A (Stage 1)
//   5. 40-bpp ARGB pass with constant A = 255 (Stage 1)
//   6. 40-bpp ARGB pass with AlphaSurface stream (Stage 2, scalar)
//   7. 40-bpp ARGB pass with AlphaSurface stream (Stage 4, SSE2 4 px/iter)
//   8-10. whole-frame totals: today / 48-bpp Stage 1 / 40-bpp Stage 1
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <chrono>
#include <vector>
#include <emmintrin.h>

static const int W = 1920, H = 1080, N = W * H;

int main() {
    printf("Upload micro-benchmark, %dx%d = %d px  (best of 300 runs)\n\n", W, H, N);
    std::vector<uint8_t> idx(N), light(N), alpha(N);
    std::vector<uint8_t> dst16(N * 2), dst8(N);
    std::vector<uint32_t> dst32(N), src32(N);
    std::vector<uint32_t> lut(256);
    for (int i = 0; i < N; ++i) {
        idx[i]   = (uint8_t)((i * 31u) >> 8);
        light[i] = (uint8_t)((i * 97u) >> 8);
        alpha[i] = (uint8_t)((i * 193u) >> 8);
        src32[i] = (uint32_t)((i * 2654435761u) >> 8) | 0xFF000000u;
    }
    for (int i = 0; i < 256; ++i) {
        uint8_t r = (uint8_t)(i * 5), g = (uint8_t)(i * 7), b = (uint8_t)(i * 11);
        lut[i] = (uint32_t)b | ((uint32_t)g << 8) | ((uint32_t)r << 16); // 0x00RRGGBB, same as D3dPalette upload
    }
    const int ITERS = 300;
    auto run = [&](const char* name, auto&& fn) {
        fn();
        double best = 1e18;
        for (int it = 0; it < ITERS; ++it) {
            auto t0 = std::chrono::high_resolution_clock::now();
            fn();
            auto t1 = std::chrono::high_resolution_clock::now();
            double us = std::chrono::duration<double, std::micro>(t1 - t0).count();
            if (us < best) best = us;
        }
        printf("%-40s %9.1f us  (%6.2f ms)\n", name, best, best / 1000.0);
    };

    // 1. today's A8L8 interleave (SSE2 16px/iter, like RenderQuadTexture)
    run("A8L8 interleave today (16bpp, 4.15MB)", [&] {
        for (int i = 0; i + 16 <= N; i += 16) {
            __m128i p = _mm_loadu_si128((const __m128i*)(idx.data() + i));
            __m128i l = _mm_loadu_si128((const __m128i*)(light.data() + i));
            _mm_storeu_si128((__m128i*)(dst16.data() + i), _mm_unpacklo_epi8(p, l));
            _mm_storeu_si128((__m128i*)(dst16.data() + i + 8), _mm_unpackhi_epi8(p, l));
        }
    });

    // 2. new L8 light upload: plain memcpy
    run("L8 memcpy (8bpp, 2.07MB)", [&] {
        memcpy(dst8.data(), light.data(), N);
    });

    // 3. 48-bpp draft RGB pass (X8R8G8B8): LUT gather
    run("XRGB LUT pass 48bpp draft (8.29MB)", [&] {
        for (int i = 0; i < N; ++i) dst32[i] = lut[idx[i]];
    });

    // 4. 48-bpp draft AL pass, Stage 1 (constant A byte)
    run("AL const-A interleave 48bpp Stage1", [&] {
        for (int i = 0; i + 16 <= N; i += 16) {
            __m128i p = _mm_loadu_si128((const __m128i*)(idx.data() + i));
            __m128i c = _mm_set1_epi8((char)0xFF);
            _mm_storeu_si128((__m128i*)(dst16.data() + i), _mm_unpacklo_epi8(p, c));
            _mm_storeu_si128((__m128i*)(dst16.data() + i + 8), _mm_unpackhi_epi8(p, c));
        }
    });

    // 5. 40-bpp ARGB pass, Stage 1 (constant A = 255)
    run("ARGB LUT+constA 40bpp Stage1 (8.29MB)", [&] {
        for (int i = 0; i < N; ++i) dst32[i] = lut[idx[i]] | 0xFF000000u;
    });

    // 6. 40-bpp ARGB pass, Stage 2 (AlphaSurface stream, scalar)
    run("ARGB LUT+A stream 40bpp Stage2 scalar", [&] {
        for (int i = 0; i < N; ++i) dst32[i] = lut[idx[i]] | ((uint32_t)alpha[i] << 24);
    });

    // 7. 40-bpp ARGB pass, Stage 4 (SSE2, 4 px/iter)
    run("ARGB LUT+A stream 40bpp SSE2 4px", [&] {
        for (int i = 0; i + 4 <= N; i += 4) {
            __m128i a = _mm_cvtsi32_si128(*(const int*)(alpha.data() + i));
            a = _mm_unpacklo_epi8(a, _mm_setzero_si128());
            a = _mm_unpacklo_epi16(a, _mm_setzero_si128());
            a = _mm_slli_epi32(a, 24);
            __m128i rgb = _mm_setr_epi32((int)lut[idx[i + 0]], (int)lut[idx[i + 1]],
                                        (int)lut[idx[i + 2]], (int)lut[idx[i + 3]]);
            _mm_storeu_si128((__m128i*)&dst32[i], _mm_or_si128(rgb, a));
        }
    });

    // 8. future direct-RGBA upload: pure memcpy of a 32bpp system surface
    run("ARGB memcpy (future direct RGBA, 8.29MB)", [&] {
        memcpy(dst32.data(), src32.data(), (size_t)N * 4);
    });

    // 9-11. whole-frame totals
    run("TOTAL today: A8L8 interleave only", [&] {
        for (int i = 0; i + 16 <= N; i += 16) {
            __m128i p = _mm_loadu_si128((const __m128i*)(idx.data() + i));
            __m128i l = _mm_loadu_si128((const __m128i*)(light.data() + i));
            _mm_storeu_si128((__m128i*)(dst16.data() + i), _mm_unpacklo_epi8(p, l));
            _mm_storeu_si128((__m128i*)(dst16.data() + i + 8), _mm_unpackhi_epi8(p, l));
        }
    });
    run("TOTAL 48bpp Stage1: XRGB LUT + AL constA", [&] {
        for (int i = 0; i < N; ++i) dst32[i] = lut[idx[i]];
        for (int i = 0; i + 16 <= N; i += 16) {
            __m128i p = _mm_loadu_si128((const __m128i*)(idx.data() + i));
            __m128i c = _mm_set1_epi8((char)0xFF);
            _mm_storeu_si128((__m128i*)(dst16.data() + i), _mm_unpacklo_epi8(p, c));
            _mm_storeu_si128((__m128i*)(dst16.data() + i + 8), _mm_unpackhi_epi8(p, c));
        }
    });
    run("TOTAL 40bpp Stage1: ARGB LUT+constA + L8 memcpy", [&] {
        for (int i = 0; i < N; ++i) dst32[i] = lut[idx[i]] | 0xFF000000u;
        memcpy(dst8.data(), light.data(), N);
    });
    // hybrid 48 bpp Stage 1: A8R8G8B8 (unused) + today's A8L8 interleave + palette upload
    run("TOTAL hybrid 48bpp St1: A8L8 interleave + palette upload", [&] {
        for (int i = 0; i + 16 <= N; i += 16) {
            __m128i p = _mm_loadu_si128((const __m128i*)(idx.data() + i));
            __m128i l = _mm_loadu_si128((const __m128i*)(light.data() + i));
            _mm_storeu_si128((__m128i*)(dst16.data() + i), _mm_unpacklo_epi8(p, l));
            _mm_storeu_si128((__m128i*)(dst16.data() + i + 8), _mm_unpackhi_epi8(p, l));
        }
        for (int i = 0; i < 256; ++i) dst32[i] = lut[i];   // 256-entry palette upload (as today)
    });
    // hybrid variant with swapped channels: A = index, L = light (same instruction mix)
    run("TOTAL hybrid 48bpp swapped: interleave (A=idx,L=lt) + pal", [&] {
        for (int i = 0; i + 16 <= N; i += 16) {
            __m128i l = _mm_loadu_si128((const __m128i*)(light.data() + i));
            __m128i p = _mm_loadu_si128((const __m128i*)(idx.data() + i));
            _mm_storeu_si128((__m128i*)(dst16.data() + i), _mm_unpacklo_epi8(l, p));
            _mm_storeu_si128((__m128i*)(dst16.data() + i + 8), _mm_unpackhi_epi8(l, p));
        }
        for (int i = 0; i < 256; ++i) dst32[i] = lut[i];
    });

    // prevent dead-code elimination: touch everything once outside the timed region
    volatile uint64_t sink = 0;
    for (int i = 0; i < N; ++i) {
        sink += dst16[i];
        if (!(i & 1)) sink += dst8[i];
    }
    for (int i = 0; i < N; i += 997) sink += dst32[i];
    printf("\n(checksum %llu)\n", (unsigned long long)sink);
    return 0;
}
