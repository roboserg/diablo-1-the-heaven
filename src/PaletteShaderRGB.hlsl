// PaletteShaderRGB.hlsl — ultra two-texture variant of PaletteShader.hlsl.
// tx (s0) = D3DFMT_A8R8G8B8: .rgb = true color resolved on the CPU at upload
//           time, .a = 1.0 (Stage 1 ships the A byte as constant 255).
// lt (s1) = D3DFMT_L8: .r = light level / 255 (0 = full bright, 255 = pitch black).
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

// Stage 2: lit main color, overlay composite keyed on the A byte (overlay
// weight), output alpha 1.0 always — the weight must never reach the target.
float4 sampleLit(float2 coord)
{
	float4 rgba = tex2D(tx, coord);
	float bright = tex2D(lt, coord).r * LIGHT_SCALE;
	float3 litMain = rgba.rgb * saturate(1.0 - bright);
	// (th4) Stage 3: colored-light tint with the LUT-pattern falloff (Rev 2.25)
	// — the tint texture's A byte carries the winning light's win margin
	// (how much it strictly brightened the pixel, 0..255), which follows the
	// light's LUT falloff relative to the local runner-up: 255 at an isolated
	// pool center, fading to 0 at the pool edge AND at every light-interaction
	// boundary, so the color glows strongest at the center and dissolves into
	// white instead of forming hard sectors. Applied to the lit main only;
	// the sprite overlay keeps its captured colors.
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
	// outline stays readable. ps_2_0 has no flow control (texture reads
	// cannot be skipped at runtime), so the taps are compiled in/out via the
	// PaletteShaderRimRGB.hlsl wrapper (ENRICHED_RIM + _rim entry point);
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

#ifndef ENTRY
#define ENTRY main_palette_rgb
#endif

float4 ENTRY(in float2 coord:TEXCOORD) : COLOR
{
	return sampleLit(coord); // .a = 1.0 — A byte is the overlay weight now
}
