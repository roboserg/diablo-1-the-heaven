// FsrEasu.hlsl -- AMD FidelityFX Super Resolution 1.0, EASU pass (edge-adaptive spatial upscale)
//
// Port of the FSR 1.0 EASU algorithm (AMD, MIT licence: github.com/GPUOpen-Effects/FidelityFX-FSR)
// to Direct3D 9 ps_3_0. D3D9 has no gather, so the 12 source taps are fetched one by one
// with tex2Dlod (point sampled, clamped). The maths follows ffx_fsr1.h FsrEasuF; a numpy model
// of exactly this code was checked against bilinear on a TH4 frame before it was written.
//
// Pass 1 of 2 (see dx_utility.cpp FsrPresent): game frame (ScreenWidth x ScreenHeight)
// -> intermediate texture at WINDOW size. Pass 2 is FsrRcas.hlsl (sharpening).
// Unlike the other post shaders this one works on finished RGB colour, not palette
// indices, so it has no Legacy / RGB / RimRGB variants: it runs after all of them.
//
// Constants (set by FsrPresent):
//   c0.xy = input size / output size        c0.zw = 0.5 * c0.xy - 0.5
//   c1.xy = 1 / input size (texel size)
sampler src : register(s0);
float4 con0 : register(c0);
float4 con1 : register(c1);

float3 Tap(float2 fp, float2 o)
{
	return tex2Dlod(src, float4((fp + o + 0.5) * con1.xy, 0, 0)).rgb;
}
float Luma(float3 c) { return 0.5 * c.b + (0.5 * c.r + c.g); }

void SetF(inout float2 dir, inout float len, float w, float lA, float lB, float lC, float lD, float lE)
{
	float dc = lD - lC, cb = lC - lB;
	float lenX = 1.0 / max(max(abs(dc), abs(cb)), 1.0 / 32768.0);
	float dirX = lD - lB;
	dir.x += dirX * w;
	lenX = saturate(abs(dirX) * lenX); lenX *= lenX;
	len += lenX * w;
	float ec = lE - lC, ca = lC - lA;
	float lenY = 1.0 / max(max(abs(ec), abs(ca)), 1.0 / 32768.0);
	float dirY = lE - lA;
	dir.y += dirY * w;
	lenY = saturate(abs(dirY) * lenY); lenY *= lenY;
	len += lenY * w;
}

void TapF(inout float3 aC, inout float aW, float2 off, float2 dir, float2 len2, float lob, float clp, float3 c)
{
	float2 v;
	v.x = (off.x * dir.x) + (off.y * dir.y);
	v.y = (off.x * (-dir.y)) + (off.y * dir.x);
	v *= len2;
	float d2 = min(v.x * v.x + v.y * v.y, clp);
	float wB = 2.0 / 5.0 * d2 - 1.0;
	float wA = lob * d2 - 1.0;
	wB *= wB; wA *= wA;
	wB = 25.0 / 16.0 * wB - (25.0 / 16.0 - 1.0);
	float w = wB * wA;
	aC += c * w; aW += w;
}

float4 main_easu(float2 vpos : VPOS) : COLOR
{
	float2 pp = vpos * con0.xy + con0.zw;
	float2 fp = floor(pp);
	pp -= fp;
	//    b c
	//  e f g h
	//  i j k l
	//    n o
	float3 b = Tap(fp, float2( 0,-1)), c = Tap(fp, float2( 1,-1));
	float3 e = Tap(fp, float2(-1, 0)), f = Tap(fp, float2( 0, 0)), g = Tap(fp, float2( 1, 0)), h = Tap(fp, float2( 2, 0));
	float3 i = Tap(fp, float2(-1, 1)), j = Tap(fp, float2( 0, 1)), k = Tap(fp, float2( 1, 1)), l = Tap(fp, float2( 2, 1));
	float3 n = Tap(fp, float2( 0, 2)), o = Tap(fp, float2( 1, 2));
	float bL = Luma(b), cL = Luma(c), eL = Luma(e), fL = Luma(f), gL = Luma(g), hL = Luma(h);
	float iL = Luma(i), jL = Luma(j), kL = Luma(k), lL = Luma(l), nL = Luma(n), oL = Luma(o);

	float2 dir = 0; float len = 0;
	SetF(dir, len, (1.0 - pp.x) * (1.0 - pp.y), bL, eL, fL, gL, jL);
	SetF(dir, len, pp.x * (1.0 - pp.y),         cL, fL, gL, hL, kL);
	SetF(dir, len, (1.0 - pp.x) * pp.y,         fL, iL, jL, kL, nL);
	SetF(dir, len, pp.x * pp.y,                 gL, jL, kL, lL, oL);

	float dirR = dir.x * dir.x + dir.y * dir.y;
	bool zro = dirR < (1.0 / 32768.0);
	dirR = zro ? 1.0 : rsqrt(dirR);
	dir.x = zro ? 1.0 : dir.x;
	dir *= dirR;
	len = len * 0.5; len *= len;
	float stretch = (dir.x * dir.x + dir.y * dir.y) / max(abs(dir.x), abs(dir.y));
	float2 len2 = float2(1.0 + (stretch - 1.0) * len, 1.0 - 0.5 * len);
	float lob = 0.5 + ((1.0 / 4.0 - 0.04) - 0.5) * len;
	float clp = 1.0 / lob;

	float3 aC = 0; float aW = 0;
	TapF(aC, aW, float2( 0,-1) - pp, dir, len2, lob, clp, b);
	TapF(aC, aW, float2( 1,-1) - pp, dir, len2, lob, clp, c);
	TapF(aC, aW, float2(-1, 1) - pp, dir, len2, lob, clp, i);
	TapF(aC, aW, float2( 0, 1) - pp, dir, len2, lob, clp, j);
	TapF(aC, aW, float2( 0, 0) - pp, dir, len2, lob, clp, f);
	TapF(aC, aW, float2(-1, 0) - pp, dir, len2, lob, clp, e);
	TapF(aC, aW, float2( 1, 1) - pp, dir, len2, lob, clp, k);
	TapF(aC, aW, float2( 2, 1) - pp, dir, len2, lob, clp, l);
	TapF(aC, aW, float2( 2, 0) - pp, dir, len2, lob, clp, h);
	TapF(aC, aW, float2( 1, 0) - pp, dir, len2, lob, clp, g);
	TapF(aC, aW, float2( 1, 2) - pp, dir, len2, lob, clp, o);
	TapF(aC, aW, float2( 0, 2) - pp, dir, len2, lob, clp, n);

	// deringing: clamp to the 2x2 neighbourhood
	float3 mn = min(min(f, g), min(j, k));
	float3 mx = max(max(f, g), max(j, k));
	return float4(min(mx, max(mn, aC / aW)), 1.0);
}
