// BloomShaderRGB.hlsl — ultra two-texture variant of BloomShader.hlsl.
// sampleLit reads the resolved color from tx and brightness from lt.
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

// ITU-R BT.709 perceptual luminance (same weights as the FXAA shader).
float luma(float3 c)
{
	return dot(c, float3(0.2126, 0.7152, 0.0722));
}

// Stage 2: sample A8R8G8B8 color and L8 brightness at coord; overlay composite
// keyed on the A byte (overlay weight), output alpha 1.0 always.
float4 sampleLit(float2 coord)
{
	float4 rgba = tex2D(tx, coord);
	float bright = tex2D(lt, coord).r * LIGHT_SCALE;
	float3 litMain = rgba.rgb * saturate(1.0 - bright);
	// (th4) Stage 3: colored-light tint with the LUT-pattern falloff (Rev 2.25)
	// — the tint texture's A byte carries the winning light's win margin
	// (0..255, the LUT falloff relative to the local runner-up), so the color
	// glows strongest at the pool center and dissolves into white at the pool
	// edge and at light-interaction boundaries. The bloom taps sample the
	// composite, so the tint rides along.
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
	// BloomShaderRimRGB.hlsl wrapper (ENRICHED_RIM + _rim entry point);
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

struct PSInput{
	float2 sp : VPOS;
	float2 cr : TEXCOORD0;
};

#ifndef ENTRY
#define ENTRY main_bloom_rgb
#endif

float4 ENTRY(PSInput input) : COLOR
{
	float w = 1.0 / (1.0 / input.cr.x * input.sp.x);
	float h = 1.0 / (1.0 / input.cr.y * input.sp.y);
	float4 px = sampleLit(input.cr);

	float4 x1 = sampleLit(input.cr + float2(-w,-h));
	float4 x3 = sampleLit(input.cr + float2(w, h));
	float4 x6 = sampleLit(input.cr + float2(w,-h));
	float4 x8 = sampleLit(input.cr + float2(-w, h));

	float4 d2 = sampleLit(input.cr + float2(0, -(h/2)));
	float4 d4 = sampleLit(input.cr + float2(-(w/2), 0));
	float4 d5 = sampleLit(input.cr + float2(w/2, 0));
	float4 d7 = sampleLit(input.cr + float2(0, h/2));

	float4 m = (x1 + d2 + x3 + d4 + d5 + x6 + d7 + x8) / 8;
	m *= m;
	return clamp(px + m,0.0,1.0);
}
