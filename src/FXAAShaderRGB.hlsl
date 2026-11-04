// FXAAShaderRGB.hlsl — ultra two-texture variant of FXAAShader.hlsl.
// Luminance-based edge anti-aliasing over the resolved color + brightness.
// Same logic as FXAAShader.hlsl; only the palette lookup was replaced.
sampler tx : register(s0);
sampler lt : register(s1);
sampler ov : register(s2); // Stage 2 overlay (SpriteLayerSurface RGB)
sampler tl : register(s3); // Stage 3 tint (LightTintSurface RGB)

static const float LIGHT_SCALE = 1.0; // L byte range is 0-255
static const float TINT_STRENGTH = 1.0; // Stage 3: colored-light tint strength (1.0 = full)

// (th4) Stage 2 rung 2 (Rev 2.12): c_alphaContrast.x = 1 enables the
// contrast-weighted overlay alpha (0 = rung 1 constant). Set per frame by
// RenderQuadTextureUltra from EnrichedLightUltraAlphaContrast.
float4 c_alphaContrast : register(c0);

// ITU-R BT.709 luminance (moved above sampleLit for the rung-2 composite).
float luma(float3 c)
{
	return dot(c, float3(0.2126, 0.7152, 0.0722));
}

// Stage 2: sample A8R8G8B8 color and L8 brightness at coord; overlay composite
// keyed on the A byte (overlay weight), output alpha 1.0.  FXAA keeps sampling
// the composite, so edges anti-alias across the translucency boundary.
float4 sampleLit(float2 coord)
{
	float4 rgba = tex2D(tx, coord);
	float bright = tex2D(lt, coord).r * LIGHT_SCALE;
	float3 litMain = rgba.rgb * saturate(1.0 - bright);
	// (th4) Stage 3: colored-light tint with the LUT-pattern falloff (Rev 2.25)
	// — the tint texture's A byte carries the winning light's win margin
	// (0..255, the LUT falloff relative to the local runner-up), so the color
	// glows strongest at the pool center and dissolves into white at the pool
	// edge and at light-interaction boundaries. FXAA keeps sampling the
	// composite, so edges anti-alias across the tint boundary.
	float4 tint = tex2D(tl, coord); // .rgb = winner color, .a = win-margin falloff
	litMain *= lerp(float3(1, 1, 1), tint.rgb, tint.a * TINT_STRENGTH);
	float3 sprite = tex2D(ov, coord).rgb;
	// (th4) Rung 2: base weight (A byte = EnrichedLightUltraAlpha) multiplied
	// per-pixel by the sprite/wall luminance contrast — bright sprite over a
	// dark wall pops, dark-on-dark fades so the wall keeps depth cues.
	float w = rgba.a * lerp(1.0,
		saturate(abs(luma(sprite) - luma(litMain)) * (255.0 / 96.0)),
		c_alphaContrast.x);
#ifdef ENRICHED_RIM
	// (th4) Rung 3: rim emphasis — the sprite mask's edge pixels keep the
	// full base weight even where contrast is low, so the occluded sprite's
	// outline stays readable. ps_3_0 can branch, but fxc hoists texture reads
	// above uniform branches, so the taps are compiled in/out via the
	// FXAAShaderRimRGB.hlsl wrapper (ENRICHED_RIM + _rim entry point);
	// duplicate FxCompile Includes on this source are not allowed — VS
	// rejects them at project load.
	if (c_alphaContrast.y > 0.5) {
		float2 xoff = float2(c_alphaContrast.z, 0.0);
		float2 yoff = float2(0.0, c_alphaContrast.w);
		float amin = min(min(tex2D(tx, saturate(coord + xoff)).a, tex2D(tx, saturate(coord - xoff)).a),
						min(tex2D(tx, saturate(coord + yoff)).a, tex2D(tx, saturate(coord - yoff)).a));
		w = max(w, saturate(rgba.a - amin));
	}
#endif
	rgba.rgb = lerp(litMain, sprite, w);
	rgba.a = 1.0;
	return rgba;
}

// (luma moved above sampleLit for the rung-2 composite — Rev 2.12)

struct PSInput {
	float2 cr : TEXCOORD0;
};

#ifndef ENTRY
#define ENTRY main_fxaa_rgb
#endif

float4 ENTRY(PSInput i) : COLOR
{
	// Texel size via screen-space derivatives — robust regardless of
	// vertex UV setup, viewport offset, or fullscreen-quad convention.
	float w = abs(ddx(i.cr.x));
	float h = abs(ddy(i.cr.y));

	// Neighbour samples clamped to [0,1] so we never read outside the
	// texture at screen edges (where the offset would produce negative
	// or >1 coordinates, wrapping via D3D default WRAP addressing).
	float4 px   = sampleLit(i.cr);
	float4 nU   = sampleLit(saturate(i.cr + float2(  0, -h)));
	float4 nD   = sampleLit(saturate(i.cr + float2(  0,  h)));
	float4 nL   = sampleLit(saturate(i.cr + float2( -w,  0)));
	float4 nR   = sampleLit(saturate(i.cr + float2(  w,  0)));

	float lC = luma(px.rgb);
	float lU = luma(nU.rgb);
	float lD = luma(nD.rgb);
	float lL = luma(nL.rgb);
	float lR = luma(nR.rgb);

	// Luminance range in the 5-pixel cross — a proxy for local contrast.
	// Skip FXAA when contrast is very low (flat-colour area) to avoid
	// blurring texture detail and amplifying single-pixel data artifacts.
	float lMin = min(min(min(lC, lU), min(lD, lL)), lR);
	float lMax = max(max(max(lC, lU), max(lD, lL)), lR);
	float range = lMax - lMin;
	if (range < 0.0625) // 16/256 — roughly one palette band step
		return px;

	// Estimate edge direction from horizontal / vertical second differences.
	// hGrad large → sharp change left↔right → edge runs VERTICALLY.
	// vGrad large → sharp change up↔down    → edge runs HORIZONTALLY.
	// To anti-alias, blend ACROSS the edge (perpendicular), not along it.
	float hGrad = abs(lL + lR - 2.0 * lC);
	float vGrad = abs(lU + lD - 2.0 * lC);

	// Blend factor: stronger edges get more blending.  Clamped to avoid
	// over-blurring.
	float factor = saturate(2.0 * max(hGrad, vGrad));

	if (hGrad > vGrad)
	{
		// Vertical edge — blend horizontally (across left/right boundary)
		float4 blended = lerp(px, (nL + px + nR) / 3.0, factor * 0.45);
		return float4(blended.rgb, 1.0);
	}
	else
	{
		// Horizontal edge — blend vertically (across top/bottom boundary)
		float4 blended = lerp(px, (nU + px + nD) / 3.0, factor * 0.45);
		return float4(blended.rgb, 1.0);
	}
}
