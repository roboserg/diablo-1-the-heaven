// Retro.hlsl -- (new by th4) RETRO MODES, pass 1 of 2 (config RetroMode 1..18, RetroPixel).
// Driver: dx_utility.cpp Retro_Present, called from PostFxPresent after the mist / rain / glow /
// frost passes and BEFORE the PostFx.hlsl pass, so CRT, grain and the other PostFx effects lay on
// top of the retro image (CRT + a green monitor is the intended combination).
// The whole screen is affected: world, panels, text and cursor (no world/UI mask).
//
// This pass renders the retro image at LOW resolution: one output pixel = one "retro pixel" =
// BLOCK x BLOCK game pixels (r0.z). The window-size frame copy in s0 is sampled at game-pixel
// positions (uv = game px / game size), so the result is the same whatever the window size and
// upscaler. Pass 2 (RetroUp.hlsl) enlarges it to the window with crisp pixel edges.
// KINDs 0..4 compile here (entry main_retro); 5..7 compile through the wrapper RetroCell.hlsl
// (#define RETRO_CELL + #include this file, entry main_retrocell) -- the two halves together would
// pass the 512-instruction-slot ps_3_0 minimum. Edit THIS body, not the wrapper.
// Every mode's numbers (palette, block, dither, gamma) live in ONE table, dx_utility.cpp
// RetroModes[] -- tune there, not here. KIND (r0.w) picks the family:
//   0 RAMP   luminance -> a dark..bright palette ramp; dithered, or smooth when DITHER = 0
//            (Game Boy, Pocket, Virtual Boy, amber / green monitor, sepia, 1-bit Mac)
//   1 PAL    nearest colour of a fixed palette, optional ordered dither (CGA, EGA, C64, NES, PC-98)
//   2 PIX    pixelate only, colours untouched
//   3 QUANT  PALN levels per channel + ordered dither (PS1: 32 levels = 15-bit colour, 4x4 Bayer)
//   4 HALF   CMYK newspaper halftone, four dot screens at 15/75/0/45 degrees, CELL px apart
//   5 ZX     ZX Spectrum attribute clash: every 8x8 cell has only 2 colours (ink + paper) from the
//            same half (normal / bright) of the 16-entry palette; pixels dither between them
//   6 ASCII  8x8 character cells, glyph by brightness from s2 (10 glyphs " .-:=+%*#@"), cell colour
//   7 TTX    Teletext mosaic: 2x3 sixel cells, one colour of 7 per cell on black
//
// s0 = frame copy (window size, linear, clamp)
// s1 = palette, 64 x 1 (point, clamp); entry i at u = (i + 0.5) / 64
// s2 = ASCII glyphs, 80 x 8 (point, clamp), 10 glyphs side by side, white = ink
// c0 = xy low-res target size, z BLOCK (game px per retro pixel), w KIND
// c1 = xy 1 / game size (ScreenWidth, ScreenHeight), z gamma lift, w DITHER
//      (0 none, 1 Bayer 8x8, 2 Bayer 4x4, 3 interleaved-gradient noise = organic, error-diffusion-like)
// c2 = x PALN (palette entries / QUANT levels), y dither spread (PAL), z halftone CELL, w gain
sampler2D frame : register(s0);
sampler2D pal : register(s1);
sampler2D glyphs : register(s2);
float4 r0 : register(c0);
float4 r1 : register(c1);
float4 r2 : register(c2);

#define LOWSIZE r0.xy
#define BLOCK r0.z
#define KIND r0.w
#define INVGAME r1.xy
#define GAMMA r1.z
#define DITHER r1.w
#define PALN r2.x
#define SPREAD r2.y
#define CELL r2.z
#define GAIN r2.w

float3 src(float2 gamePx) { return tex2Dlod(frame, float4(gamePx * INVGAME, 0, 0)).rgb; }
// Diablo is a dark game: lift the shadows so 2..4-colour modes are not just their darkest entry
float3 lift(float3 c) { return saturate(pow(max(c, 0.0001), GAMMA) * GAIN); }
float luma(float3 c) { return dot(c, float3(0.299, 0.587, 0.114)); }
float3 palAt(float i) { return tex2Dlod(pal, float4((i + 0.5) / 64.0, 0.5, 0, 0)).rgb; }

// nearest palette entry in [first, first + count): rgb + error in w (weighted RGB distance)
float4 nearestPal(float3 c, float first, float count)
{
	float best = 1e9;
	float3 bc = 0;
	[loop] for (int i = 0; i < 64; i++) {
		if (i >= count) break;
		float3 p = palAt(first + i);
		float3 d = c - p;
		float e = dot(d * d, float3(2, 4, 3));
		if (e < best) { best = e; bc = p; }
	}
	return float4(bc, best);
}

float bayer2(float2 p) { p = fmod(p, 2.0); return fmod(p.x * 2.0 + p.y * 3.0, 4.0); }
// ordered-dither threshold in (0, 1) for retro pixel lp (integer coords); 0.5 when DITHER = 0
float threshold(float2 lp)
{
	if (DITHER < 0.5) return 0.5;
	if (DITHER < 1.5) return (bayer2(lp) * 16.0 + bayer2(floor(lp * 0.5)) * 4.0 + bayer2(floor(lp * 0.25)) + 0.5) / 64.0;
	if (DITHER < 2.5) return (bayer2(lp) * 4.0 + bayer2(floor(lp * 0.5)) + 0.5) / 16.0;
	return frac(52.9829189 * frac(dot(lp, float2(0.06711056, 0.00583715))));
}

// own colour of retro pixel lp: one tap at its centre, or the mean of four for big blocks
float3 lowPix(float2 lp)
{
	float2 o = lp * BLOCK;
	if (BLOCK < 1.5) return src(o + 0.5);
	float a = BLOCK * 0.25, b = BLOCK * 0.75;
	return 0.25 * (src(o + float2(a, a)) + src(o + float2(b, a)) + src(o + float2(a, b)) + src(o + float2(b, b)));
}

float3 halftone(float2 lp)
{
	float2 gp = (lp + 0.5) * BLOCK;	// game px
	float3 col = float3(0.95, 0.93, 0.86);	// paper
	float S = CELL;
	[loop] for (int k = 0; k < 4; k++) {
		float a = k == 0 ? 0.2618 : k == 1 ? 1.3090 : k == 2 ? 0.0 : 0.7854;	// C 15, M 75, Y 0, K 45 deg
		float cs = cos(a), sn = sin(a);
		float2 q = float2(cs * gp.x + sn * gp.y, -sn * gp.x + cs * gp.y);	// into the screen's grid
		float2 cc = (floor(q / S) + 0.5) * S;									// dot centre, grid space
		float2 cg = float2(cs * cc.x - sn * cc.y, sn * cc.x + cs * cc.y);	// back to game px
		float3 s = lift(src(cg));
		float K = 1.0 - max(s.r, max(s.g, s.b));
		float ch = k == 0 ? s.r : k == 1 ? s.g : s.b;
		float cov = k == 3 ? K : (1.0 - ch - K) / max(1.0 - K, 0.001);
		float rad = S * 0.78 * sqrt(saturate(cov));	// > 0.707 S: full ink closes the cell
		float ink = saturate(rad - length(q - cc) + 0.5);	// 1 px soft dot edge
		float3 inkCol = k == 0 ? float3(0.0, 0.62, 0.88) : k == 1 ? float3(0.90, 0.05, 0.50) : k == 2 ? float3(1.0, 0.92, 0.05) : float3(0.08, 0.08, 0.10);
		col *= lerp(float3(1, 1, 1), inkCol, ink);
	}
	return col;
}

float3 zx(float2 lp)
{
	float2 o = floor(lp / 8.0) * 8.0;	// 8x8 attribute cell
	float3 sum = 0;
	[loop] for (int y = 0; y < 8; y++) {
		[loop] for (int x = 0; x < 8; x++) sum += lift(src((o + float2(x, y) + 0.5) * BLOCK));
	}
	float3 mean = sum / 64.0;
	float ml = luma(mean);
	float3 hi = 0, lo = 0;
	float nh = 0, nl = 0;
	[loop] for (int y2 = 0; y2 < 8; y2++) {
		[loop] for (int x2 = 0; x2 < 8; x2++) {
			float3 s = lift(src((o + float2(x2, y2) + 0.5) * BLOCK));
			if (luma(s) >= ml) { hi += s; nh += 1; } else { lo += s; nl += 1; }
		}
	}
	hi = nh > 0 ? hi / nh : mean;
	lo = nl > 0 ? lo / nl : mean;
	// ink and paper must share the BRIGHT bit: pick the half (entries 0..7 or 8..15) that fits best
	float4 a0 = nearestPal(lo, 0, 8), b0 = nearestPal(hi, 0, 8);
	float4 a1 = nearestPal(lo, 8, 8), b1 = nearestPal(hi, 8, 8);
	float3 ink = a0.rgb, paper = b0.rgb;
	if (a1.w + b1.w < a0.w + b0.w) { ink = a1.rgb; paper = b1.rgb; }
	float3 c = lift(src((lp + 0.5) * BLOCK));
	float3 dd = paper - ink;
	float den = dot(dd, dd);
	if (den < 0.00001) return ink;
	float t = saturate(dot(c - ink, dd) / den);
	return t > threshold(lp) ? paper : ink;
}

float3 ascii(float2 lp)
{
	float2 o = floor(lp / 8.0) * 8.0;	// 8x8 character cell
	float2 at = lp - o;					// pixel inside the glyph
	float3 sum = 0;
	[loop] for (int y = 0; y < 4; y++) {
		[loop] for (int x = 0; x < 4; x++) sum += src((o + float2(x * 2 + 1, y * 2 + 1)) * BLOCK);
	}
	float3 m = lift(sum / 16.0);
	float l = luma(m);
	float g = min(floor(saturate((l - 0.1) / 0.9) * 10.0), 9.0);	// the darkest 10% stays blank
	float bit = tex2Dlod(glyphs, float4((g * 8.0 + at.x + 0.5) / 80.0, (at.y + 0.5) / 8.0, 0, 0)).r;
	float mx = max(m.r, max(m.g, m.b));
	float3 ink = (mx > 0.001 ? m / mx : float3(1, 1, 1)) * (0.6 + 0.4 * l);	// the cell's hue, readable
	return bit > 0.5 ? ink : float3(0, 0, 0);
}

float3 teletext(float2 lp)
{
	float2 o = floor(lp / float2(2, 3)) * float2(2, 3);	// 2x3 sixel cell
	float mn = 1, mxl = 0;
	[loop] for (int i = 0; i < 6; i++) {
		float l = luma(lift(src((o + float2(i % 2, i / 2) + 0.5) * BLOCK)));
		mn = min(mn, l); mxl = max(mxl, l);
	}
	float thr = mxl - mn > 0.1 ? max(0.5 * (mn + mxl), 0.25) : 0.25;	// dark cells stay black
	float3 sum = 0;
	float n = 0;
	[loop] for (int j = 0; j < 6; j++) {
		float3 s = lift(src((o + float2(j % 2, j / 2) + 0.5) * BLOCK));
		if (luma(s) > thr) { sum += s; n += 1; }
	}
	float3 own = lift(src((lp + 0.5) * BLOCK));
	if (n < 0.5 || luma(own) <= thr) return float3(0, 0, 0);
	float3 m = sum / n;
	float mx = max(m.r, max(m.g, m.b));
	return nearestPal(m / max(mx, 0.001), 1, 7).rgb;	// entries 1..7 = the 7 Teletext colours
}

#ifndef RETRO_CELL
float4 main_retro(float2 uv : TEXCOORD0) : COLOR
{
	float2 lp = floor(uv * LOWSIZE);	// retro pixel, integer coords
	float3 c;
	if (KIND < 0.5) {				// RAMP
		float l = luma(lift(lowPix(lp)));
		float v = l * (PALN - 1.0);
		if (DITHER < 0.5) {
			float i0 = clamp(floor(v), 0, PALN - 2.0);
			c = lerp(palAt(i0), palAt(i0 + 1.0), saturate(v - i0));
		} else {
			c = palAt(min(floor(v + threshold(lp)), PALN - 1.0));
		}
	} else if (KIND < 1.5) {		// PAL
		c = lift(lowPix(lp));
		if (DITHER > 0.5) c += (threshold(lp) - 0.5) * SPREAD;
		c = nearestPal(c, 0, PALN).rgb;
	} else if (KIND < 2.5) {		// PIX
		c = lift(lowPix(lp));
	} else if (KIND < 3.5) {		// QUANT
		float L = PALN - 1.0;
		c = saturate(floor(lift(lowPix(lp)) * L + threshold(lp)) / L);
	} else {
		c = halftone(lp);
	}
	return float4(c, 1);
}
#else
// RetroCell.hlsl: the three CELL families (one shader would exceed 512 instruction slots)
float4 main_retrocell(float2 uv : TEXCOORD0) : COLOR
{
	float2 lp = floor(uv * LOWSIZE);
	float3 c;
	if (KIND < 5.5) c = zx(lp);
	else if (KIND < 6.5) c = ascii(lp);
	else c = teletext(lp);
	return float4(c, 1);
}
#endif
